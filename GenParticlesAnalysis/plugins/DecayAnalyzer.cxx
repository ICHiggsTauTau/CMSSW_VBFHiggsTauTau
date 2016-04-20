#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/plugins/DecayAnalyzer.h"

#include "DataFormats/Math/interface/deltaPhi.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

// ROOT includes
#include "TMath.h"
#include "TString.h"

// C++ includes
#include <iostream>
#include <memory>

// user include files


using namespace edm;
using namespace std;

DecayAnalyzer::DecayAnalyzer(const edm::ParameterSet& pset){
  
  ps = pset;
  
  m_verbose       = ps.getUntrackedParameter<bool>("verbose",      false);
  m_output_edm    = ps.getUntrackedParameter<bool>("output_edm",   true);
  m_output_ntuple = ps.getUntrackedParameter<bool>("output_ntuple",true);
  
  edm::InputTag inputTag_HepMCProduct          = ps.getUntrackedParameter<edm::InputTag>("inputTag_HepMCProduct",         edm::InputTag("generator"));
  edm::InputTag inputTag_GenJetCollection      = ps.getUntrackedParameter<edm::InputTag>("inputTag_GenJetCollection",     edm::InputTag("ak4GenJetsNoNu"));  
  edm::InputTag inputTag_GenParticleCollection = ps.getUntrackedParameter<edm::InputTag>("inputTag_GenParticleCollection",edm::InputTag("genParticles")); 
  
  if(m_output_edm){
    produces< string >                     ("HiggsDecayMode");
    produces< reco::GenParticleCollection >("HiggsDecayTau1");
    produces< reco::GenParticleCollection >("HiggsDecayTau2");
    produces< reco::GenParticleCollection >("HiggsDecayTau1Stable");
    produces< reco::GenParticleCollection >("HiggsDecayTau2Stable");
  }
  
  if(m_output_ntuple){
    m_genAnalysisData = new VBFHiggs::GenAnalysisDataFormat();
    
    m_tree = m_fs->make<TTree>("VBFHiggsGenAnalysisTree","VBFHiggsGenAnalysisTree");
    //m_tree->Branch("GenAnalysis", "VBFHiggs::GenAnalysisDataFormat", &m_genAnalysisData, 32000, 3);
    m_tree->Branch("GenAnalysis", "VBFHiggs::GenAnalysisDataFormat", &m_genAnalysisData);
  }
  
  m_InputTag_HepMCProduct          = consumes<edm::HepMCProduct>          (inputTag_HepMCProduct);
  m_inputTag_GenJetCollection      = consumes<reco::GenJetCollection>     (inputTag_GenJetCollection);
  m_inputTag_GenParticleCollection = consumes<reco::GenParticleCollection>(inputTag_GenParticleCollection);  
  
  string outputFilename = pset.getUntrackedParameter<string>("outputFilename","DecayAnalyzer_Results.root");
  fOut = new TFile(outputFilename.c_str(),"RECREATE");
  
  m_EventCount  = new TH1D("EventCount","EventCount",1,-0.5,0.5); m_EventCount->SetDirectory(fOut);
  
  m_Parton_N    = new TH1D("Parton_N","Parton_N",11,-0.5,10.5);       m_Parton_N   ->SetDirectory(fOut);
  m_Tau_N       = new TH1D("Tau_N",   "Tau_N",   11,-0.5,10.5);       m_Tau_N      ->SetDirectory(fOut);
  m_Parton_Type = new TH1D("Parton_Type","Parton_Type", 2,-0.5, 2.5); m_Parton_Type->SetDirectory(fOut);
  m_Parton_Type->GetXaxis()->SetBinLabel(1,"Status 23");
  m_Parton_Type->GetXaxis()->SetBinLabel(2,"Status 24");
  
}

DecayAnalyzer::~DecayAnalyzer(){
  fOut->Write();
  delete m_EventCount;
}


void DecayAnalyzer::produce(edm::Event& iEvent,const edm::EventSetup& iSetup){
  
  // Resetting data formats
  if(m_output_ntuple){
    m_genAnalysisData->reset();
  }
  
  // Counting the current event
  m_EventCount->Fill(0);
  
  
  Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByToken(m_inputTag_GenParticleCollection, genParticles);
  
  auto_ptr<reco::GenParticleCollection> vHiggsDecayTau1(new reco::GenParticleCollection);
  auto_ptr<reco::GenParticleCollection> vHiggsDecayTau2(new reco::GenParticleCollection);
  
  auto_ptr<reco::GenParticleCollection> vHiggsDecayTau1Stable(new reco::GenParticleCollection);
  auto_ptr<reco::GenParticleCollection> vHiggsDecayTau2Stable(new reco::GenParticleCollection);
  
  vector<const reco::GenParticle*> myTaus;
  int nTau     = 0;
  int TauToEle = 0;
  int TauToMuo = 0;
  int TauToHad = 0;
  
  unsigned char tau1_decay = 0;
  unsigned char tau2_decay = 0;
  
  ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > vecNeutrino(0,0,0,0);
  
  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const reco::GenParticle & p = (*genParticles)[i];
    
    if(p.status()==1 && (fabs(p.pdgId())==12 || fabs(p.pdgId())==14 || fabs(p.pdgId())==16) ){
      vecNeutrino += p.p4();
    }
    
    if(fabs(p.pdgId())==15){
      //       cout << "Found a tau..." << endl;
      
      if(p.mother()->pdgId() == 25){
        //         cout << "and it comes from a Higgs!" << endl;
        
        bool decayEle = 0;
        bool decayMu  = 0;
        bool decayHad = 1;
        
        if     (nTau==0){vHiggsDecayTau1->push_back(p);}
        else if(nTau==1){vHiggsDecayTau2->push_back(p);}
        
        unsigned n = p.numberOfDaughters();
        for(size_t j = 0; j < n; ++ j) {
          
          const reco::GenParticle *d = (reco::GenParticle*) p.daughter( j );
          
          if     (nTau==0){vHiggsDecayTau1Stable->push_back(*d);}
          else if(nTau==1){vHiggsDecayTau2Stable->push_back(*d);}
          
          //cout << "Particle tau son id=" << d->pdgId() << endl;
          
          if(fabs(d->pdgId()) == 11){decayEle=1;decayHad=0;}
          if(fabs(d->pdgId()) == 13){decayMu =1;decayHad=0;}
        }
        
        if(nTau==0){
          if(decayEle){TauToEle++; tau1_decay=VBFHiggs::TauDecay::Ele;}
          if(decayMu) {TauToMuo++; tau1_decay=VBFHiggs::TauDecay::Muo;}
          if(decayHad){TauToHad++; tau1_decay=VBFHiggs::TauDecay::Had;}
        }
        else if(nTau==1){
          if(decayEle){TauToEle++; tau2_decay=VBFHiggs::TauDecay::Ele;}
          if(decayMu) {TauToMuo++; tau2_decay=VBFHiggs::TauDecay::Muo;}
          if(decayHad){TauToHad++; tau2_decay=VBFHiggs::TauDecay::Had;}
        }
        nTau++;
      }
    }
    
  }
  
  bool done  = false;
  while(!done){
    done=true;
    
    for(unsigned i=0; i<vHiggsDecayTau1Stable->size(); i++){
      const reco::GenParticle & p = (*vHiggsDecayTau1Stable)[i];
      
      if(p.status()!=1){
        
        for(size_t j = 0; j < p.numberOfDaughters(); ++ j) {
          const reco::GenParticle *d = (reco::GenParticle*) p.daughter( j );  
          vHiggsDecayTau1Stable->push_back(*d);
        }
        vHiggsDecayTau1Stable->erase(vHiggsDecayTau1Stable->begin()+i);
        done=false;
        break;
      }
    }
  }
  
  done  = false;
  while(!done){
    done=true;
    
    for(unsigned i=0; i<vHiggsDecayTau2Stable->size(); i++){
      const reco::GenParticle & p = (*vHiggsDecayTau2Stable)[i];
      
      if(p.status()!=1){
        
        for(size_t j = 0; j < p.numberOfDaughters(); ++ j) {
          const reco::GenParticle *d = (reco::GenParticle*) p.daughter( j );  
          vHiggsDecayTau2Stable->push_back(*d);
        }
        vHiggsDecayTau2Stable->erase(vHiggsDecayTau2Stable->begin()+i);
        done=false;
        break;
      }
    }
  }
  
  
  
  //   cout << "Number of decay particles Tau #1" << vHiggsDecayTau1->size() << endl;
  //   cout << "Number of decay particles Tau #2" << vHiggsDecayTau2->size() << endl;
  
  if(m_output_ntuple){
    m_genAnalysisData->tau1_decayType=tau1_decay;
    m_genAnalysisData->tau2_decayType=tau2_decay;
    
    m_genAnalysisData->genMet = vecNeutrino.pt();
    
    for(auto it=vHiggsDecayTau1Stable->begin(); it!=vHiggsDecayTau1Stable->end(); it++){
      m_genAnalysisData->tau1_stableDecayProducts.push_back(VBFHiggsToTauTau::MicroGenParticle(it->status(),it->pdgId(),it->charge(),it->p4()));
    }
    
    for(auto it=vHiggsDecayTau2Stable->begin(); it!=vHiggsDecayTau2Stable->end(); it++){
      m_genAnalysisData->tau2_stableDecayProducts.push_back(VBFHiggsToTauTau::MicroGenParticle(it->status(),it->pdgId(),it->charge(),it->p4()));
    }
  }

  auto_ptr< string > decayMode(new string);
  if(TauToHad==2 && TauToEle==0 && TauToMuo==0){
    (*decayMode)="HadHad"; 
    if(m_output_ntuple){m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::HadHad;}
  }
  else if(TauToHad==1 && TauToEle==1 && TauToMuo==0){
    (*decayMode)="HadEle"; 
    if(m_output_ntuple){m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::EleHad;}
  }  
  else if(TauToHad==1 && TauToEle==0 && TauToMuo==1){
    (*decayMode)="HadMuo"; 
    if(m_output_ntuple){m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::MuoHad;}
  }
  else if(TauToHad==0 && TauToEle==2 && TauToMuo==0){
    (*decayMode)="EleEle"; 
    if(m_output_ntuple){m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::EleEle;}
  }
  else if(TauToHad==0 && TauToEle==1 && TauToMuo==1){
    (*decayMode)="EleMuo"; 
    if(m_output_ntuple){m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::EleMuo;}
  }
  else if(TauToHad==0 && TauToEle==0 && TauToMuo==2){
    (*decayMode)="MuoMuo"; 
    if(m_output_ntuple){m_genAnalysisData->higgs_decayType=VBFHiggs::HiggsDecay::MuoMuo;}
  }
  else{
    (*decayMode)="Error";
    if(m_output_ntuple){m_genAnalysisData->higgs_decayType=0;}
  }
  
  m_Tau_N->Fill(nTau+1);
  
  // and save the vectors
  if(m_output_edm){
    iEvent.put(decayMode,            "HiggsDecayMode");
    iEvent.put(vHiggsDecayTau1,      "HiggsDecayTau1");
    iEvent.put(vHiggsDecayTau2,      "HiggsDecayTau2");
    iEvent.put(vHiggsDecayTau1Stable,"HiggsDecayTau1Stable");
    iEvent.put(vHiggsDecayTau2Stable,"HiggsDecayTau2Stable");
  }
  
  if(m_output_ntuple){
    m_tree->Fill();
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(DecayAnalyzer);
