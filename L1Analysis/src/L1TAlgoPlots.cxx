#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoPlots.h"

#include "DataFormats/Math/interface/deltaR.h"

// L1T Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"

#include "TMath.h"

using namespace std;

L1TAlgoPlots::L1TAlgoPlots(string name, TDirectory* baseBirectory){

  if(m_verbose){printf("[L1TAlgoPlots::L1TAlgoPlots] Method called. (name=%s)\n",name.c_str());}
  
  m_name           = name;
  m_plotsDirectory = baseBirectory->mkdir(name.c_str());

  tag_l1tEG      = "l1t_eg";
  tag_l1tMuon    = "l1t_mu";
  tag_l1tTau     = "l1t_tau";
  tag_l1tIsoTau  = "l1t_isotau";
  tag_l1tJet     = "l1t_jet";
  tag_l1tSum     = "l1t_sum";
  
  tag_l1tJetPair = "";
  
  m_L1TEGamma1_Et  = new TH1D("L1TEGamma1_Pt", "L1T EGamma1 Pt", 250,           0,        250); m_L1TEGamma1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TEGamma1_Eta = new TH1D("L1TEGamma1_Eta","L1T EGamma1 Eta",100,          -5,          5); m_L1TEGamma1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TEGamma1_Phi = new TH1D("L1TEGamma1_Phi","L1T EGamma1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TEGamma1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TMuon1_Et  = new TH1D("L1TMuon1_Pt", "L1T Muon1 Pt", 250,           0,        250); m_L1TMuon1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TMuon1_Eta = new TH1D("L1TMuon1_Eta","L1T Muon1 Eta",100,          -5,          5); m_L1TMuon1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TMuon1_Phi = new TH1D("L1TMuon1_Phi","L1T Muon1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TMuon1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TTau_N    = new TH1D("L1TTau_N",     "L1T Tau N",  21,        -0.5,       20.5); m_L1TTau_N   ->SetDirectory(m_plotsDirectory);
  m_L1TTau1_Et  = new TH1D("L1TTau1_Pt", "L1T Tau1 Pt", 250,           0,        250); m_L1TTau1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TTau1_Eta = new TH1D("L1TTau1_Eta","L1T Tau1 Eta",100,          -5,          5); m_L1TTau1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TTau1_Phi = new TH1D("L1TTau1_Phi","L1T Tau1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TTau1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TTau2_Et  = new TH1D("L1TTau2_Pt", "L1T Tau2 Pt", 250,           0,        250); m_L1TTau2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TTau2_Eta = new TH1D("L1TTau2_Eta","L1T Tau2 Eta",100,          -5,          5); m_L1TTau2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TTau2_Phi = new TH1D("L1TTau2_Phi","L1T Tau2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TTau2_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TIsoTau_N    = new TH1D("L1TIsoTau_N",   "L1T IsoTau N",    21,        -0.5,       20.5); m_L1TIsoTau_N   ->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau1_Et  = new TH1D("L1TIsoTau1_Pt", "L1T IsoTau1 Pt", 250,           0,        250); m_L1TIsoTau1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau1_Eta = new TH1D("L1TIsoTau1_Eta","L1T IsoTau1 Eta",100,          -5,          5); m_L1TIsoTau1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau1_Phi = new TH1D("L1TIsoTau1_Phi","L1T IsoTau1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TIsoTau1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TIsoTau2_Et  = new TH1D("L1TIsoTau2_Pt", "L1T IsoTau2 Pt", 250,           0,        250); m_L1TIsoTau2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau2_Eta = new TH1D("L1TIsoTau2_Eta","L1T IsoTau2 Eta",100,          -5,          5); m_L1TIsoTau2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau2_Phi = new TH1D("L1TIsoTau2_Phi","L1T IsoTau2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TIsoTau2_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TJet_N     = new TH1D("L1TJet_N",   "L1T Jet N",       21,        -0.5,       20.5); m_L1TJet_N    ->SetDirectory(m_plotsDirectory);
  m_L1TJet_AvgPt = new TH1D("L1TJet_Avg", "L1T Dijet AvgPt",250,           0,        250); m_L1TJet_AvgPt->SetDirectory(m_plotsDirectory);
  m_L1TJet1_Et   = new TH1D("L1TJet1_Pt", "L1T Jet1 Pt",    250,           0,        250); m_L1TJet1_Et  ->SetDirectory(m_plotsDirectory);
  m_L1TJet1_Eta  = new TH1D("L1TJet1_Eta","L1T Jet1 Eta",   100,          -5,          5); m_L1TJet1_Eta ->SetDirectory(m_plotsDirectory);
  m_L1TJet1_Phi  = new TH1D("L1TJet1_Phi","L1T Jet1 Phi",   100,-TMath::Pi(),TMath::Pi()); m_L1TJet1_Phi ->SetDirectory(m_plotsDirectory);
  
  m_L1TJet2_Et  = new TH1D("L1TJet2_Pt", "L1T Jet2 Pt", 250,           0,        250); m_L1TJet2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TJet2_Eta = new TH1D("L1TJet2_Eta","L1T Jet2 Eta",100,          -5,          5); m_L1TJet2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TJet2_Phi = new TH1D("L1TJet2_Phi","L1T Jet2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TJet2_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TJet_maxMjj  = new TH1D("L1TJet_maxMjj", "L1TJet maxMjj", 1000,   0,1000); m_L1TJet_maxMjj ->SetDirectory(m_plotsDirectory);
  m_L1TJet_maxDEta = new TH1D("L1TJet_maxDEta","L1TJet maxDEta", 150,   0,  15); m_L1TJet_maxDEta->SetDirectory(m_plotsDirectory);
  
  m_L1TTau1_minDRL1TJet = new TH1D("L1TTau1_minDRL1TJet","minDR(L1TTau1,L1TJet)",150, 0, 15); m_L1TTau1_minDRL1TJet->SetDirectory(m_plotsDirectory);
  
  m_L1TMET_Et = new TH1D("L1TMET_Et","L1T MET Et",512,0,512); m_L1TMET_Et->SetDirectory(m_plotsDirectory);
  m_L1TMHT_Et = new TH1D("L1TMHT_Et","L1T MHT Et",512,0,512); m_L1TMHT_Et->SetDirectory(m_plotsDirectory);
  
  m_METvsMjj  = new TH2D("METvsMjj",      "MET vs Mjj",   100,0,200, 100,0,1000);       m_METvsMjj     ->SetDirectory(m_plotsDirectory);
}

L1TAlgoPlots::~L1TAlgoPlots(){
  if(m_L1TTau1_Et) {delete m_L1TTau1_Et;}
  if(m_L1TTau1_Eta){delete m_L1TTau1_Eta;}
  if(m_L1TTau1_Phi){delete m_L1TTau1_Phi;}
  
  if(m_L1TTau2_Et) {delete m_L1TTau2_Et;}
  if(m_L1TTau2_Eta){delete m_L1TTau2_Eta;}
  if(m_L1TTau2_Phi){delete m_L1TTau2_Phi;}
  
  if(m_L1TJet1_Et ){delete m_L1TTau1_Et;}
  if(m_L1TJet1_Eta){delete m_L1TTau1_Eta;}
  if(m_L1TJet1_Phi){delete m_L1TTau1_Phi;}
  
  if(m_L1TJet2_Et ){delete m_L1TTau2_Et;}
  if(m_L1TJet2_Eta){delete m_L1TTau2_Eta;}
  if(m_L1TJet2_Phi){delete m_L1TTau2_Phi;}
  
  if(m_L1TJet_maxMjj) {delete m_L1TJet_maxMjj;}
  if(m_L1TJet_maxDEta){delete m_L1TJet_maxDEta;}
  
  delete m_L1TTau1_minDRL1TJet;
  delete m_L1TMET_Et;
  delete m_L1TMHT_Et;
}

void L1TAlgoPlots::fill(icTrg::Event &iEvent){
  
  double evMET = 0.;
//   double evMHT = 0.;
  
  for(unsigned i=0; i<iEvent.l1tSumCollection.size(); i++){
    
    ic::L1TSum *iSum = &(iEvent.l1tSumCollection.at(i));
    
    if(iSum->sumType==ic::L1TSum::SumType::kMissingEt){
      evMET = iSum->vector().Et();
      m_L1TMET_Et ->Fill(iSum->vector().Et());
    }
    else if(iSum->sumType==ic::L1TSum::SumType::kMissingHt){
//       evMHT = iSum->vector().Et();
      m_L1TMHT_Et ->Fill(iSum->vector().Et());
    }
  }
  
  ic::L1TEGammaCollection* colEG = (ic::L1TEGammaCollection*) iEvent.get(tag_l1tEG);
  if(colEG->size()>0){
    m_L1TEGamma1_Et ->Fill(colEG->at(0).pt());
    m_L1TEGamma1_Eta->Fill(colEG->at(0).phi());
    m_L1TEGamma1_Phi->Fill(colEG->at(0).eta());
  }
  
  ic::L1TMuonCollection* colMuon = (ic::L1TMuonCollection*) iEvent.get(tag_l1tMuon);
  if(colMuon->size()>0){
    m_L1TMuon1_Et ->Fill(colMuon->at(0).pt());
    m_L1TMuon1_Eta->Fill(colMuon->at(0).phi());
    m_L1TMuon1_Phi->Fill(colMuon->at(0).eta());
  }
  
  ic::L1TTauCollection* colTau = (ic::L1TTauCollection*) iEvent.get(tag_l1tTau);
  m_L1TTau_N->Fill(colTau->size());
  if(colTau->size()>0){
    m_L1TTau1_Et ->Fill(colTau->at(0).pt());
    m_L1TTau1_Phi->Fill(colTau->at(0).phi());
    m_L1TTau1_Eta->Fill(colTau->at(0).eta());
  }
  if(colTau->size()>1){
    m_L1TTau2_Et ->Fill(colTau->at(1).pt());
    m_L1TTau2_Phi->Fill(colTau->at(1).phi());
    m_L1TTau2_Eta->Fill(colTau->at(1).eta());
  }
  
  ic::L1TTauCollection* colIsoTau = (ic::L1TTauCollection*) iEvent.get(tag_l1tIsoTau);
  m_L1TIsoTau_N->Fill(colIsoTau->size());
  if(colIsoTau->size()>0){
    m_L1TIsoTau1_Et ->Fill(colIsoTau->at(0).pt());
    m_L1TIsoTau1_Phi->Fill(colIsoTau->at(0).phi());
    m_L1TIsoTau1_Eta->Fill(colIsoTau->at(0).eta());
  }
  if(colIsoTau->size()>1){
    m_L1TIsoTau2_Et ->Fill(colIsoTau->at(1).pt());
    m_L1TIsoTau2_Phi->Fill(colIsoTau->at(1).phi());
    m_L1TIsoTau2_Eta->Fill(colIsoTau->at(1).eta());
  }
  
  ic::L1TJetCollection* colJet = (ic::L1TJetCollection*) iEvent.get(tag_l1tJet);
  if(tag_l1tJetPair==""){
  
    m_L1TJet_N->Fill(colJet->size());
    
    if(colJet->size()>0){
      m_L1TJet1_Et ->Fill(colJet->at(0).pt());
      m_L1TJet1_Phi->Fill(colJet->at(0).phi());
      m_L1TJet1_Eta->Fill(colJet->at(0).eta());
    }
    if(colJet->size()>1){
      m_L1TJet2_Et ->Fill(colJet->at(1).pt());
      m_L1TJet2_Phi->Fill(colJet->at(1).phi());
      m_L1TJet2_Eta->Fill(colJet->at(1).eta());
    }
  }
  else{
      
    const icTrg::L1TObjectPairCollection* colJetPairs = iEvent.getPairs(tag_l1tJetPair);
    
    m_L1TJet_N->Fill(colJetPairs->size());
    
    double maxObj1Pt = 0.;
    double maxObj2Pt = 0.;
    double maxAvgPt  = 0.;
    double maxDEta   = 0.;
    double maxMjj    = 0.;
    
    for(unsigned i=0; i<colJetPairs->size(); i++){
      
      const icTrg::L1TObjectPair iPair = colJetPairs->at(i);
      
      // Checking lead jet max pt
      if(maxObj1Pt<iPair.first->pt()) {maxObj1Pt=iPair.first->pt();}
      
      // Checking sublead jet max pt
      if(maxObj2Pt<iPair.second->pt()){maxObj2Pt=iPair.second->pt();}
      
      // Checking dijet avgPt
      double thisAvgPt = (iPair.first->pt()+iPair.second->pt())/2;
      if(maxAvgPt<thisAvgPt){maxAvgPt=thisAvgPt;}
      
      // Checking dijet deta
      double thisDEta = fabs(iPair.first->eta()-iPair.second->eta());
      if(maxDEta<thisDEta){maxDEta=thisDEta;}
      
      // Checking dijet mjj
      ROOT::Math::PtEtaPhiEVector vec = iPair.first->vector() + iPair.second->vector();
      double thisMjj = vec.mass();
      if(maxMjj<thisMjj){maxMjj=thisMjj;}
    }
    
    // Now filling the maxs
    m_L1TJet1_Et    ->Fill(maxObj1Pt);
    m_L1TJet2_Et    ->Fill(maxObj2Pt);
    m_L1TJet_AvgPt  ->Fill(maxAvgPt);
    m_L1TJet_maxDEta->Fill(maxDEta);
    m_L1TJet_maxMjj ->Fill(maxMjj);
    
    m_METvsMjj      ->Fill(evMET,maxMjj);
  }
  
  // Separation tau-jet
  if(colTau->size()>0 && colJet->size()>0){
    
    double minDR = 999.;
    for(auto it=colJet->begin(); it!=colJet->end(); it++){
      double iDR = deltaR<ic::Candidate,ic::Candidate>(colTau->at(0),*it);
      if(iDR<minDR){minDR=iDR;}
    }
    m_L1TTau1_minDRL1TJet->Fill(minDR);
  }
  
  
  
}

void L1TAlgoPlots::setVerbose(bool value){
  m_verbose = value;
}
