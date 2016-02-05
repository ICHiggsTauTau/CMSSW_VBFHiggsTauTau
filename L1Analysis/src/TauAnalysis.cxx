#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/TauAnalysis.h"

#include "TMath.h"

#include <stdio.h>
#include <math.h>

using namespace std;

struct greater_l1Jet{
  bool operator() (const l1t::Jet *j1,const l1t::Jet *j2) {
    return (j1->et() > j2->et());
  }
};

struct greater_l1Tau{
  bool operator() (const l1t::Tau *j1,const l1t::Tau *j2) {
    return (j1->et() > j2->et());
  }
};


TauAnalysis::TauAnalysis(const edm::ParameterSet& pset){
  
  ps = pset;
  
  edm::InputTag inputTag_HiggsDecayMode = pset.getUntrackedParameter<edm::InputTag>("inputTag_HiggsDecayMode",edm::InputTag("decayAnalyzer","HiggsDecayMode"));
  edm::InputTag inputTag_HiggsDecayTau1 = pset.getUntrackedParameter<edm::InputTag>("inputTag_HiggsDecayTau1",edm::InputTag("decayAnalyzer","HiggsDecayTau1"));  
  edm::InputTag inputTag_HiggsDecayTau2 = pset.getUntrackedParameter<edm::InputTag>("inputTag_HiggsDecayTau2",edm::InputTag("decayAnalyzer","HiggsDecayTau2")); 

  m_EDToken_HiggsDecayMode = consumes<string>                     (inputTag_HiggsDecayMode);
  m_EDToken_HiggsDecayTau1 = consumes<reco::GenParticleCollection>(inputTag_HiggsDecayTau1);
  m_EDToken_HiggsDecayTau2 = consumes<reco::GenParticleCollection>(inputTag_HiggsDecayTau2);
  
  m_L1TEra = pset.getUntrackedParameter<std::string>("inputTag_L1TEra",string("stage2"));
  if(m_L1TEra == "stage1"){
    
    edm::InputTag inputTag_EG     = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TEG",    edm::InputTag("simCaloStage1FinalDigis"));
    edm::InputTag inputTag_Muon   = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TMuon",  edm::InputTag("None"));
    edm::InputTag inputTag_Tau    = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TTau",   edm::InputTag("simCaloStage1FinalDigis:rlxTaus"));
    edm::InputTag inputTag_TauIso = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TTauIso",edm::InputTag("simCaloStage1FinalDigis:isoTaus"));
    edm::InputTag inputTag_Jet    = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TJet",   edm::InputTag("simCaloStage1FinalDigis"));
    edm::InputTag inputTag_Sum    = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TSum",   edm::InputTag("simCaloStage1FinalDigis"));
    
    m_EDToken_L1TEG     = consumes<l1t::EGammaBxCollection>(inputTag_EG);
    m_EDToken_L1TMuon   = consumes<l1t::MuonBxCollection>  (inputTag_Muon);
    m_EDToken_L1TTau    = consumes<l1t::TauBxCollection>   (inputTag_Tau);
    m_EDToken_L1TTauIso = consumes<l1t::TauBxCollection>   (inputTag_TauIso);
    m_EDToken_L1TJet    = consumes<l1t::JetBxCollection>   (inputTag_Jet);
    m_EDToken_L1TSum    = consumes<l1t::EtSumBxCollection> (inputTag_Sum);
  }
  else if(m_L1TEra == "stage2"){
    
    edm::InputTag inputTag_EG   = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TEG",    edm::InputTag("simCaloStage2Digis"));
    edm::InputTag inputTag_Muon = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TMuon",  edm::InputTag("simGmtStage2Digis"));
    edm::InputTag inputTag_Tau  = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TTau",   edm::InputTag("simCaloStage2Digis"));
    edm::InputTag inputTag_Jet  = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TJet",   edm::InputTag("simCaloStage2Digis"));
    edm::InputTag inputTag_Sum  = pset.getUntrackedParameter<edm::InputTag>("inputTag_L1TSum",   edm::InputTag("simCaloStage2Digis"));
    
    m_EDToken_L1TEG     = consumes<l1t::EGammaBxCollection>(inputTag_EG);
    m_EDToken_L1TMuon   = consumes<l1t::MuonBxCollection>  (inputTag_Muon);
    m_EDToken_L1TTau    = consumes<l1t::TauBxCollection>   (inputTag_Tau);
    m_EDToken_L1TJet    = consumes<l1t::JetBxCollection>   (inputTag_Jet);
    m_EDToken_L1TSum    = consumes<l1t::EtSumBxCollection> (inputTag_Sum);
  }

  // Settimng up output file and plots
  string outputFilename = pset.getUntrackedParameter<string>("outputFilename","TauAnalysis_Results.root");
  fOut = new TFile(outputFilename.c_str(),"RECREATE");
  
  m_channels.push_back("HadHad");
  m_channels.push_back("HadEle");
  m_channels.push_back("HadMuo");
  m_channels.push_back("EleEle");
  m_channels.push_back("EleMuo");
  m_channels.push_back("MuoMuo");
  
  m_EventCount = new TH1D("EventCount","EventCount",1, 0.5,1.5); m_EventCount->SetDirectory(fOut);
  m_HiggsDecay = new TH1D("HiggsDecay","HiggsDecay",6, 0.5,6.5); m_HiggsDecay->SetDirectory(fOut);
  m_HiggsDecay->GetXaxis()->SetBinLabel(1,"HadHad");
  m_HiggsDecay->GetXaxis()->SetBinLabel(2,"HadEle");
  m_HiggsDecay->GetXaxis()->SetBinLabel(3,"HadMuo");
  m_HiggsDecay->GetXaxis()->SetBinLabel(4,"EleEle");
  m_HiggsDecay->GetXaxis()->SetBinLabel(5,"EleMuo");
  m_HiggsDecay->GetXaxis()->SetBinLabel(6,"MuoMuo");
  
  m_L1Tau_N   = new TH1D("L1Tau_N",  "L1Tau_N",   21,        -0.5,       20.5); m_L1Tau_N  ->SetDirectory(fOut);
  m_L1Tau_Et  = new TH1D("L1Tau_Et", "L1Tau_Et", 250,           0,        250); m_L1Tau_Et ->SetDirectory(fOut);
  m_L1Tau_Eta = new TH1D("L1Tau_Eta","L1Tau_Eta",100,          -5,          5); m_L1Tau_Eta->SetDirectory(fOut);
  m_L1Tau_Phi = new TH1D("L1Tau_Phi","L1Tau_Phi",100,-TMath::Pi(),TMath::Pi()); m_L1Tau_Phi->SetDirectory(fOut);
  
  for(unsigned i=0; i<m_channels.size(); i++){
    m_L1Tau1_Et[m_channels[i]] = new TH1D(Form("L1Tau1_%s_Et",m_channels[i].c_str()),Form("L1Tau1_%s_Et",m_channels[i].c_str()),250,0,250); m_L1Tau1_Et[m_channels[i]] ->SetDirectory(fOut);
    m_L1Tau2_Et[m_channels[i]] = new TH1D(Form("L1Tau2_%s_Et",m_channels[i].c_str()),Form("L1Tau2_%s_Et",m_channels[i].c_str()),250,0,250); m_L1Tau2_Et[m_channels[i]] ->SetDirectory(fOut);
  }
  
  m_L1Jet_N   = new TH1D("L1Jet_N",  "L1Jet_N",   21,        -0.5,       20.5); m_L1Jet_N  ->SetDirectory(fOut);
  m_L1Jet_Et  = new TH1D("L1Jet_Et", "L1Jet_Et", 250,           0,        250); m_L1Jet_Et ->SetDirectory(fOut);
  m_L1Jet_Eta = new TH1D("L1Jet_Eta","L1Jet_Eta",100,          -5,          5); m_L1Jet_Eta->SetDirectory(fOut);
  m_L1Jet_Phi = new TH1D("L1Jet_Phi","L1Jet_Phi",100,-TMath::Pi(),TMath::Pi()); m_L1Jet_Phi->SetDirectory(fOut);
  
  for(unsigned i=0; i<m_channels.size(); i++){
    m_L1Jet1_Et[m_channels[i]] = new TH1D(Form("L1Jet1_%s_Et",m_channels[i].c_str()),Form("L1Jet1_%s_Et",m_channels[i].c_str()),250,0,250); m_L1Jet1_Et[m_channels[i]] ->SetDirectory(fOut);
    m_L1Jet2_Et[m_channels[i]] = new TH1D(Form("L1Jet2_%s_Et",m_channels[i].c_str()),Form("L1Jet2_%s_Et",m_channels[i].c_str()),250,0,250); m_L1Jet2_Et[m_channels[i]] ->SetDirectory(fOut);
  }
}

TauAnalysis::~TauAnalysis(){
  
  double totalEvents = m_EventCount->GetBinContent(1);
  
  cout << "=== Job Summary ===" << endl;
  cout << "Total of number events processed: " << totalEvents << endl;
  cout << endl;
  
  cout << "=== Higgs decay Summary ===" << endl;
  for(int i=1; i<m_HiggsDecay->GetNbinsX()+1; i++){
    cout << "H -> " << m_HiggsDecay->GetXaxis()->GetBinLabel(i) << " - " << (m_HiggsDecay->GetBinContent(i)/totalEvents)*100 << "%" << endl;
  }
  cout << endl;
  
  for(unsigned i=0; i<m_channels.size(); i++){
    
    double decayTotal=0;
    for(int x=1; x<m_HiggsDecay->GetNbinsX()+1; x++){
      if(m_HiggsDecay->GetXaxis()->GetBinLabel(x)==m_channels[i]){
        decayTotal=m_HiggsDecay->GetBinContent(x);
        break;
      }
    }
    
    TH1D *h1 = m_L1Tau1_Et[m_channels[i]];
    for(int i=0; i<=h1->GetNbinsX()+1; i++){
      h1->SetBinContent(i,h1->Integral(i,h1->GetNbinsX()+1));
    }
    h1->Scale(1/decayTotal);
  
    TH1D *h2 = m_L1Tau2_Et[m_channels[i]];
    for(int i=0; i<=h2->GetNbinsX()+1; i++){
      h2->SetBinContent(i,h2->Integral(i,h2->GetNbinsX()+1));
    }
    h2->Scale(1/decayTotal);
  }
  
  fOut->Write();
}

void TauAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  m_EventCount->Fill(1);   // Counting the current event
  getEventContent(iEvent); // Getting event quantities 

  // Accounting for Higgs Decay
  cout << "Higgs decau mode: " << *decayMode << endl;
  m_HiggsDecay->Fill((*decayMode).c_str(),1);
  
  // Sorting the L1T Collections 
  vector<const l1t::Tau*> *m_L1TauCollection_EtSorted = getL1TTauCollection_EtSorted(0);
  vector<const l1t::Jet*> *m_L1JetCollection_EtSorted = getL1TJetCollection_EtSorted(0);

  m_L1Tau_N->Fill(m_L1TauCollection_EtSorted->size()); 
  for(unsigned i=0; i<m_L1TauCollection_EtSorted->size(); i++){
    const l1t::Tau* tau = (*m_L1TauCollection_EtSorted)[i];
    cout << "Tau et=" << tau->et() << " eta=" << tau->eta() << " phi=" << tau->phi() << endl;
    m_L1Tau_Eta->Fill(tau->eta());
    m_L1Tau_Et ->Fill(tau->et());
    m_L1Tau_Phi->Fill(tau->phi());
    
    if(i==0){m_L1Tau1_Et[*decayMode]->Fill(tau->et());}
    if(i==1){m_L1Tau2_Et[*decayMode]->Fill(tau->et());}
  }
  
  //<< " iso=" << it->hwIso() << endl;
  
  m_L1Jet_N->Fill(m_L1JetCollection_EtSorted->size()); 
  for(unsigned i=0; i<m_L1JetCollection_EtSorted->size(); i++){
    const l1t::Jet* jet = (*m_L1JetCollection_EtSorted)[i];
    cout << "Jet et=" << jet->et() << " eta=" << jet->eta() << " phi=" << jet->phi() << endl;
    m_L1Jet_Eta->Fill(jet->eta());
    m_L1Jet_Et ->Fill(jet->et());
    m_L1Jet_Phi->Fill(jet->phi());
    
    if(i==0){m_L1Jet1_Et[*decayMode]->Fill(jet->et());}
    if(i==1){m_L1Jet2_Et[*decayMode]->Fill(jet->et());}
  }
  
  
  delete m_L1TauCollection_EtSorted;
  delete m_L1JetCollection_EtSorted;
}

void TauAnalysis::beginJob(){}

void TauAnalysis::endJob(){}

void TauAnalysis::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}

void TauAnalysis::endRun(edm::Run const&, edm::EventSetup const&){}

void TauAnalysis::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){}

void TauAnalysis::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){}

void TauAnalysis::getEventContent(const edm::Event& iEvent){
  
  iEvent.getByToken(m_EDToken_HiggsDecayMode, decayMode);
  iEvent.getByToken(m_EDToken_HiggsDecayTau1, decayTau1);
  iEvent.getByToken(m_EDToken_HiggsDecayTau2, decayTau2);
  
  iEvent.getByToken(m_EDToken_L1TEG,  m_Handle_L1TEG);
  iEvent.getByToken(m_EDToken_L1TMuon,m_Handle_L1TMuon);
  iEvent.getByToken(m_EDToken_L1TTau, m_Handle_L1TTau);
  iEvent.getByToken(m_EDToken_L1TJet, m_Handle_L1TJet);
  iEvent.getByToken(m_EDToken_L1TSum, m_Handle_L1TSums);
  
  if(m_L1TEra == "Stage1"){
    iEvent.getByToken(m_EDToken_L1TTauIso,m_Handle_L1TTauIso);
  }
  
}
//TODO: Does not handle stage1
vector<const l1t::Tau*>* TauAnalysis::getL1TTauCollection_EtSorted(int bx){
  
  vector<const l1t::Tau*> *taus = new vector<const l1t::Tau*>;
  
  for (int ibx = m_Handle_L1TTau->getFirstBX(); ibx <= m_Handle_L1TTau->getLastBX(); ++ibx) {
    
    cout << "Taus Bunch number: " << ibx << endl;
    
    if(ibx==bx){
      for (l1t::TauBxCollection::const_iterator it=m_Handle_L1TTau->begin(ibx); it!=m_Handle_L1TTau->end(ibx); it++){
	taus->push_back(&(*it));
      }
    }
  }
  sort(taus->begin(),taus->end(),greater_l1Tau());
  
  return taus;
}

vector<const l1t::Jet*>* TauAnalysis::getL1TJetCollection_EtSorted(int bx){
  
  vector<const l1t::Jet*> *jets = new vector<const l1t::Jet*>;
  for (int ibx = m_Handle_L1TJet->getFirstBX(); ibx <= m_Handle_L1TJet->getLastBX(); ++ibx) {
    
    cout << "Jets Bunch number: " << ibx << endl;
    
    if(ibx==bx){
      for (l1t::JetBxCollection::const_iterator it=m_Handle_L1TJet->begin(ibx); it!=m_Handle_L1TJet->end(ibx); it++){
	jets->push_back(&(*it));
      }
    }
  }
  sort(jets->begin(),jets->end(),greater_l1Jet());
  
  return jets;
}




// 	if (it->pt() > 0){
// 	
// 	l1upgrade_.tauEt .push_back(it->et());
// 	l1upgrade_.tauEta.push_back(it->eta());
// 	l1upgrade_.tauPhi.push_back(it->phi());
// 	l1upgrade_.tauIEt .push_back(it->hwPt());
// 	l1upgrade_.tauIEta.push_back(it->hwEta());
// 	l1upgrade_.tauIPhi.push_back(it->hwPhi());
// 	l1upgrade_.tauIso.push_back(it->hwIso());
// 	l1upgrade_.tauBx .push_back(ibx);
// 	l1upgrade_.nTaus++;
//       }


//define this as a plug-in
DEFINE_FWK_MODULE(TauAnalysis);