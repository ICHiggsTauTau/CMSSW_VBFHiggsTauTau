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
  tag_l1tIsoEG   = "l1t_isoeg";
  tag_l1tMuon    = "l1t_muon";
  tag_l1tIsoMuon = "l1t_isomuon";
  tag_l1tTau     = "l1t_tau";
  tag_l1tIsoTau  = "l1t_isotau";
  tag_l1tJet     = "l1t_jet";
  tag_l1tSum     = "l1t_sum";
  
  tag_l1tJetPair = "";
  
  m_L1TEGamma1_Et  = new TH1D("L1TEGamma1_Pt", "L1T EGamma1 Pt", 125,           0,        250); m_L1TEGamma1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TEGamma1_Eta = new TH1D("L1TEGamma1_Eta","L1T EGamma1 Eta",100,          -5,          5); m_L1TEGamma1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TEGamma1_Phi = new TH1D("L1TEGamma1_Phi","L1T EGamma1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TEGamma1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TIsoEGamma1_Et  = new TH1D("L1TIsoEG1_Pt", "L1T IsoEG1 Pt", 125,           0,        250); m_L1TIsoEGamma1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TIsoEGamma1_Eta = new TH1D("L1TIsoEG1_Eta","L1T IsoEG1 Eta",100,          -5,          5); m_L1TIsoEGamma1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TIsoEGamma1_Phi = new TH1D("L1TIsoEG1_Phi","L1T IsoEG1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TIsoEGamma1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TMuon1_Et  = new TH1D("L1TMuon1_Pt", "L1T Muon1 Pt", 125,           0,        250); m_L1TMuon1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TMuon1_Eta = new TH1D("L1TMuon1_Eta","L1T Muon1 Eta",100,          -5,          5); m_L1TMuon1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TMuon1_Phi = new TH1D("L1TMuon1_Phi","L1T Muon1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TMuon1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TIsoMuon1_Et  = new TH1D("L1TIsoMuon1_Pt", "L1T IsoMu1 Pt", 125,           0,        250); m_L1TIsoMuon1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TIsoMuon1_Eta = new TH1D("L1TIsoMuon1_Eta","L1T IsoMu1 Eta",100,          -5,          5); m_L1TIsoMuon1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TIsoMuon1_Phi = new TH1D("L1TIsoMuon1_Phi","L1T IsoMu1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TIsoMuon1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TTau_N    = new TH1D("L1TTau_N",     "L1T Tau N",  21,        -0.5,       20.5); m_L1TTau_N   ->SetDirectory(m_plotsDirectory);
  m_L1TTau1_Et  = new TH1D("L1TTau1_Pt", "L1T Tau1 Pt", 125,           0,        250); m_L1TTau1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TTau1_Eta = new TH1D("L1TTau1_Eta","L1T Tau1 Eta",100,          -5,          5); m_L1TTau1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TTau1_Phi = new TH1D("L1TTau1_Phi","L1T Tau1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TTau1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TTau2_Et  = new TH1D("L1TTau2_Pt", "L1T Tau2 Pt", 125,           0,        250); m_L1TTau2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TTau2_Eta = new TH1D("L1TTau2_Eta","L1T Tau2 Eta",100,          -5,          5); m_L1TTau2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TTau2_Phi = new TH1D("L1TTau2_Phi","L1T Tau2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TTau2_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TIsoTau_N    = new TH1D("L1TIsoTau_N",   "L1T IsoTau N",    21,        -0.5,       20.5); m_L1TIsoTau_N   ->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau1_Et  = new TH1D("L1TIsoTau1_Pt", "L1T IsoTau1 Pt", 125,           0,        250); m_L1TIsoTau1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau1_Eta = new TH1D("L1TIsoTau1_Eta","L1T IsoTau1 Eta",100,          -5,          5); m_L1TIsoTau1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau1_Phi = new TH1D("L1TIsoTau1_Phi","L1T IsoTau1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TIsoTau1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TIsoTau2_Et  = new TH1D("L1TIsoTau2_Pt", "L1T IsoTau2 Pt", 125,           0,        250); m_L1TIsoTau2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau2_Eta = new TH1D("L1TIsoTau2_Eta","L1T IsoTau2 Eta",100,          -5,          5); m_L1TIsoTau2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau2_Phi = new TH1D("L1TIsoTau2_Phi","L1T IsoTau2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TIsoTau2_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TJet_N     = new TH1D("L1TJet_N",    "L1T Jet N",       21,        -0.5,       20.5); m_L1TJet_N    ->SetDirectory(m_plotsDirectory);
  m_L1TJet_AvgPt = new TH1D("L1TJet_Avg",  "L1T Dijet AvgPt",125,           0,        250); m_L1TJet_AvgPt->SetDirectory(m_plotsDirectory);
  m_L1TJet_VecPt = new TH1D("L1TJet_VecPt","L1T Dijet VecPt",125,           0,        250); m_L1TJet_AvgPt->SetDirectory(m_plotsDirectory);
  m_L1TJet1_Et   = new TH1D("L1TJet1_Pt",  "L1T Jet1 Pt",    125,           0,        250); m_L1TJet1_Et  ->SetDirectory(m_plotsDirectory);
  m_L1TJet1_Eta  = new TH1D("L1TJet1_Eta", "L1T Jet1 Eta",   100,          -5,          5); m_L1TJet1_Eta ->SetDirectory(m_plotsDirectory);
  m_L1TJet1_Phi  = new TH1D("L1TJet1_Phi", "L1T Jet1 Phi",   100,-TMath::Pi(),TMath::Pi()); m_L1TJet1_Phi ->SetDirectory(m_plotsDirectory);
  
  m_L1TJet2_Et  = new TH1D("L1TJet2_Pt", "L1T Jet2 Pt", 125,           0,        250); m_L1TJet2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TJet2_Eta = new TH1D("L1TJet2_Eta","L1T Jet2 Eta",100,          -5,          5); m_L1TJet2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TJet2_Phi = new TH1D("L1TJet2_Phi","L1T Jet2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TJet2_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TJet_maxMjj  = new TH1D("L1TJet_maxMjj", "L1TJet maxMjj",  100,   0,1000); m_L1TJet_maxMjj ->SetDirectory(m_plotsDirectory);
  m_L1TJet_maxDEta = new TH1D("L1TJet_maxDEta","L1TJet maxDEta", 150,   0,  15); m_L1TJet_maxDEta->SetDirectory(m_plotsDirectory);
  
  m_L1TTau1_minDRL1TJet = new TH1D("L1TTau1_minDRL1TJet","minDR(L1TTau1,L1TJet)",150, 0, 15); m_L1TTau1_minDRL1TJet->SetDirectory(m_plotsDirectory);
  
  m_L1TMET_Et = new TH1D("L1TMET_Et","L1T MET Et",256,0,512); m_L1TMET_Et->SetDirectory(m_plotsDirectory);
  m_L1TMHT_Et = new TH1D("L1TMHT_Et","L1T MHT Et",256,0,512); m_L1TMHT_Et->SetDirectory(m_plotsDirectory);
  
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

void L1TAlgoPlots::fill(trgfw::Event &iEvent){
  
  vector<ic::L1TSum*>* mySums  = iEvent.getByName< vector<ic::L1TSum*> >("l1t_sum");
  
  ic::L1TSum* myMET = 0;
  ic::L1TSum* myMHT = 0;
  
  for(unsigned i=0; i<mySums->size(); i++){
    
    ic::L1TSum *iSum = mySums->at(i);
    
    if     (iSum->sumType==ic::L1TSum::SumType::kMissingEt){myMET = iSum;}
    else if(iSum->sumType==ic::L1TSum::SumType::kMissingHt){myMHT = iSum;}
  }
  
  // Filling plots
  m_L1TMET_Et ->Fill(myMET->vector().Et());
  m_L1TMHT_Et ->Fill(myMHT->vector().Et());
  
  vector<ic::L1TEGamma*>* myEGs = iEvent.getByName< vector<ic::L1TEGamma*> >(tag_l1tEG);
  if(myEGs->size()>0){
    m_L1TEGamma1_Et ->Fill(myEGs->at(0)->pt());
    m_L1TEGamma1_Eta->Fill(myEGs->at(0)->phi());
    m_L1TEGamma1_Phi->Fill(myEGs->at(0)->eta());
  }
  
  vector<ic::L1TEGamma*>* myIsoEGs = iEvent.getByName< vector<ic::L1TEGamma*> >(tag_l1tIsoEG);
  if(myIsoEGs->size()>0){
    m_L1TIsoEGamma1_Et ->Fill(myEGs->at(0)->pt());
    m_L1TIsoEGamma1_Eta->Fill(myEGs->at(0)->phi());
    m_L1TIsoEGamma1_Phi->Fill(myEGs->at(0)->eta());
  }
  
  vector<ic::L1TMuon*>* myMuons = iEvent.getByName< vector<ic::L1TMuon*> >(tag_l1tMuon);
  if(myMuons->size()>0){
    m_L1TMuon1_Et ->Fill(myMuons->at(0)->pt());
    m_L1TMuon1_Eta->Fill(myMuons->at(0)->phi());
    m_L1TMuon1_Phi->Fill(myMuons->at(0)->eta());
  }
  
  vector<ic::L1TMuon*>* myIsoMuons = iEvent.getByName< vector<ic::L1TMuon*> >(tag_l1tIsoMuon);
  if(myIsoMuons->size()>0){
    m_L1TIsoMuon1_Et ->Fill(myMuons->at(0)->pt());
    m_L1TIsoMuon1_Eta->Fill(myMuons->at(0)->phi());
    m_L1TIsoMuon1_Phi->Fill(myMuons->at(0)->eta());
  }
  
  vector<ic::L1TTau*>* myTaus = iEvent.getByName< vector<ic::L1TTau*> >(tag_l1tTau);
  m_L1TTau_N->Fill(myTaus->size());
  if(myTaus->size()>0){
    m_L1TTau1_Et ->Fill(myTaus->at(0)->pt());
    m_L1TTau1_Phi->Fill(myTaus->at(0)->phi());
    m_L1TTau1_Eta->Fill(myTaus->at(0)->eta());
  }
  if(myTaus->size()>1){
    m_L1TTau2_Et ->Fill(myTaus->at(1)->pt());
    m_L1TTau2_Phi->Fill(myTaus->at(1)->phi());
    m_L1TTau2_Eta->Fill(myTaus->at(1)->eta());
  }
  
  vector<ic::L1TTau*>* myIsoTaus = iEvent.getByName< vector<ic::L1TTau*> >(tag_l1tIsoTau);
  m_L1TIsoTau_N->Fill(myIsoTaus->size());
  if(myIsoTaus->size()>0){
    m_L1TIsoTau1_Et ->Fill(myIsoTaus->at(0)->pt());
    m_L1TIsoTau1_Phi->Fill(myIsoTaus->at(0)->phi());
    m_L1TIsoTau1_Eta->Fill(myIsoTaus->at(0)->eta());
  }
  if(myIsoTaus->size()>1){
    m_L1TIsoTau2_Et ->Fill(myIsoTaus->at(1)->pt());
    m_L1TIsoTau2_Phi->Fill(myIsoTaus->at(1)->phi());
    m_L1TIsoTau2_Eta->Fill(myIsoTaus->at(1)->eta());
  }
  
  vector<ic::L1TJet*>* myJets = iEvent.getByName< vector<ic::L1TJet*> >(tag_l1tJet);
  if(tag_l1tJetPair==""){
  
    m_L1TJet_N->Fill(myJets->size());
    
    if(myJets->size()>0){
      m_L1TJet1_Et ->Fill(myJets->at(0)->pt());
      m_L1TJet1_Phi->Fill(myJets->at(0)->phi());
      m_L1TJet1_Eta->Fill(myJets->at(0)->eta());
    }
    if(myJets->size()>1){
      m_L1TJet2_Et ->Fill(myJets->at(1)->pt());
      m_L1TJet2_Phi->Fill(myJets->at(1)->phi());
      m_L1TJet2_Eta->Fill(myJets->at(1)->eta());
    }
  }
  else{
      
    trgfw::L1TObjectPairCollection* myJetPair = iEvent.getByName< trgfw::L1TObjectPairCollection >(tag_l1tJetPair);
    
    m_L1TJet_N->Fill(myJetPair->size());
    
    double maxObj1Pt = 0.;
    double maxObj2Pt = 0.;
    double maxAvgPt  = 0.;
    double maxVecPt  = 0.;
    double maxDEta   = 0.;
    double maxMjj    = 0.;
    
    for(unsigned i=0; i<myJetPair->size(); i++){
      
      trgfw::L1TObjectPair iPair = myJetPair->at(i);
      
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
      
      // Getting dijet vector
      ROOT::Math::PtEtaPhiEVector vec = iPair.first->vector() + iPair.second->vector();

      // Checking dijet mjj
      double thisMjj = vec.mass();
      if(maxMjj<thisMjj){maxMjj=thisMjj;}
      
      // Checking dijet vector pt
      if(maxVecPt<vec.pt()){maxVecPt=vec.pt();}
    }
    
    // Now filling the maxs
    m_L1TJet1_Et    ->Fill(maxObj1Pt);
    m_L1TJet2_Et    ->Fill(maxObj2Pt);
    m_L1TJet_AvgPt  ->Fill(maxAvgPt);
    m_L1TJet_VecPt  ->Fill(maxVecPt);
    m_L1TJet_maxDEta->Fill(maxDEta);
    m_L1TJet_maxMjj ->Fill(maxMjj);
    
    m_METvsMjj      ->Fill(myMET->vector().Et(),maxMjj);
  }
  
  // Separation tau-jet
  if(myTaus->size()>0 && myJets->size()>0){
    
    double minDR = 999.;
    for(auto it=myJets->begin(); it!=myJets->end(); it++){
      double iDR = deltaR<ic::Candidate,ic::Candidate>(*(myTaus->at(0)),**it);
      if(iDR<minDR){minDR=iDR;}
    }
    m_L1TTau1_minDRL1TJet->Fill(minDR);
  }
  
  
  
}

void L1TAlgoPlots::setVerbose(bool value){
  m_verbose = value;
}
