#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgo.h"

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>

#include "TMath.h"

using namespace std;

L1TAlgo::L1TAlgo(){
  init();
  if(m_verbose){printf("[L1TAlgo::L1TAlgo] Method called\n");}
}

L1TAlgo::L1TAlgo(string name, TDirectory* baseBirectory){
  init();
  if(m_verbose){printf("[L1TAlgo::L1TAlgo] Method called. (name=%s)\n",name.c_str());}
  m_name           = name;
  m_plotsDirectory = baseBirectory->mkdir(name.c_str());
  initPlots();
}

L1TAlgo::~L1TAlgo(){
  
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
  
  if(m_L1TJet_minMjj) {delete m_L1TJet_minMjj;}
  if(m_L1TJet_minDEta){delete m_L1TJet_minDEta;}
  
}

void L1TAlgo::setVerbose(bool value){
  m_verbose = value;
}

std::string L1TAlgo::getName(){
  return m_name;
}

void L1TAlgo::init(){
  
  m_verbose = false;
  if(m_verbose){printf("[L1TAlgo::init] Method called\n");}
  
  m_name = "";

  m_plotsDirectory = 0;
  
  m_L1TEGamma1_Et  = 0;
  m_L1TEGamma1_Eta = 0;
  m_L1TEGamma1_Phi = 0;
  
  m_L1TMuon1_Et  = 0;
  m_L1TMuon1_Eta = 0;
  m_L1TMuon1_Phi = 0;
  
  m_L1TTau1_Et  = 0; 
  m_L1TTau1_Eta = 0;
  m_L1TTau1_Phi = 0;
  
  m_L1TTau2_Et  = 0;
  m_L1TTau2_Eta = 0;
  m_L1TTau2_Phi = 0;
  
  m_L1TJet1_Et  = 0;
  m_L1TJet1_Eta = 0;
  m_L1TJet1_Phi = 0;
  
  m_L1TJet2_Et  = 0;
  m_L1TJet2_Eta = 0;
  m_L1TJet2_Phi = 0;
  
  m_L1TJet_minMjj  = 0;
  m_L1TJet_minDEta = 0;
}

void L1TAlgo::initPlots(){
  
  if(m_verbose){printf("[L1TAlgo::initPlots] Method called\n");}
  
  m_L1TEGamma1_Et  = new TH1D("L1TEGamma1_Et", "L1T EGamma1 Et", 250,           0,        250); m_L1TEGamma1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TEGamma1_Eta = new TH1D("L1TEGamma1_Eta","L1T EGamma1 Eta",100,          -5,          5); m_L1TEGamma1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TEGamma1_Phi = new TH1D("L1TEGamma1_Phi","L1T EGamma1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TEGamma1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TMuon1_Et  = new TH1D("L1TMuon1_Et", "L1T Muon1 Et", 250,           0,        250); m_L1TMuon1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TMuon1_Eta = new TH1D("L1TMuon1_Eta","L1T Muon1 Eta",100,          -5,          5); m_L1TMuon1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TMuon1_Phi = new TH1D("L1TMuon1_Phi","L1T Muon1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TMuon1_Phi->SetDirectory(m_plotsDirectory);

  m_L1TTau1_Et  = new TH1D("L1TTau1_Et", "L1T Tau1 Et", 250,           0,        250); m_L1TTau1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TTau1_Eta = new TH1D("L1TTau1_Eta","L1T Tau1 Eta",100,          -5,          5); m_L1TTau1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TTau1_Phi = new TH1D("L1TTau1_Phi","L1T Tau1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TTau1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TTau2_Et  = new TH1D("L1TTau2_Et", "L1T Tau2 Et", 250,           0,        250); m_L1TTau2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TTau2_Eta = new TH1D("L1TTau2_Eta","L1T Tau2 Eta",100,          -5,          5); m_L1TTau2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TTau2_Phi = new TH1D("L1TTau2_Phi","L1T Tau2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TTau2_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TIsoTau1_Et  = new TH1D("L1TIsoTau1_Et", "L1T IsoTau1 Et", 250,           0,        250); m_L1TIsoTau1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau1_Eta = new TH1D("L1TIsoTau1_Eta","L1T IsoTau1 Eta",100,          -5,          5); m_L1TIsoTau1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau1_Phi = new TH1D("L1TIsoTau1_Phi","L1T IsoTau1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TIsoTau1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TIsoTau2_Et  = new TH1D("L1TIsoTau2_Et", "L1T IsoTau2 Et", 250,           0,        250); m_L1TIsoTau2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau2_Eta = new TH1D("L1TIsoTau2_Eta","L1T IsoTau2 Eta",100,          -5,          5); m_L1TIsoTau2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TIsoTau2_Phi = new TH1D("L1TIsoTau2_Phi","L1T IsoTau2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TIsoTau2_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TJet1_Et  = new TH1D("L1TJet1_Et", "L1T Jet1 Et", 250,           0,        250); m_L1TJet1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TJet1_Eta = new TH1D("L1TJet1_Eta","L1T Jet1 Eta",100,          -5,          5); m_L1TJet1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TJet1_Phi = new TH1D("L1TJet1_Phi","L1T Jet1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TJet1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TJet2_Et  = new TH1D("L1TJet2_Et", "L1T Jet2 Et", 250,           0,        250); m_L1TJet2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TJet2_Eta = new TH1D("L1TJet2_Eta","L1T Jet2 Eta",100,          -5,          5); m_L1TJet2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TJet2_Phi = new TH1D("L1TJet2_Phi","L1T Jet2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TJet2_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TJet_minMjj  = new TH1D("L1TJet_minMjj", "L1TJet minMjj", 1000,   0,1000); m_L1TJet_minMjj ->SetDirectory(m_plotsDirectory);
  m_L1TJet_minDEta = new TH1D("L1TJet_minDEta","L1TJet minDEta", 100,-5.0, 5.0); m_L1TJet_minDEta->SetDirectory(m_plotsDirectory);
  
  m_L1TJetDijet20_maxMjj  = new TH1D("L1TJetDijet20_maxMjj", "L1TJetDijet20_maxMjj", 1000,   0,1000); m_L1TJetDijet20_maxMjj ->SetDirectory(m_plotsDirectory);
  m_L1TJetDijet20_maxDEta = new TH1D("L1TJetDijet20_maxDEta","L1TJetDijet20_maxDEta", 100,-5.0, 5.0); m_L1TJetDijet20_maxDEta->SetDirectory(m_plotsDirectory);
  
  m_L1TJetDijet30_maxMjj  = new TH1D("L1TJetDijet30_maxMjj", "L1TJetDijet30_maxMjj", 1000,   0,1000); m_L1TJetDijet30_maxMjj ->SetDirectory(m_plotsDirectory);
  m_L1TJetDijet30_maxDEta = new TH1D("L1TJetDijet30_maxDEta","L1TJetDijet30_maxDEta", 100,-5.0, 5.0); m_L1TJetDijet30_maxDEta->SetDirectory(m_plotsDirectory);
  
  m_L1TJetDijet40_maxMjj  = new TH1D("L1TJetDijet40_maxMjj", "L1TJetDijet40_maxMjj", 1000,   0,1000); m_L1TJetDijet40_maxMjj ->SetDirectory(m_plotsDirectory);
  m_L1TJetDijet40_maxDEta = new TH1D("L1TJetDijet40_maxDEta","L1TJetDijet40_maxDEta", 100,-5.0, 5.0); m_L1TJetDijet40_maxDEta->SetDirectory(m_plotsDirectory);
  
  m_L1TJetDijet50_maxMjj  = new TH1D("L1TJetDijet50_maxMjj", "L1TJetDijet50_maxMjj", 1000,   0,1000); m_L1TJetDijet50_maxMjj ->SetDirectory(m_plotsDirectory);
  m_L1TJetDijet50_maxDEta = new TH1D("L1TJetDijet50_maxDEta","L1TJetDijet50_maxDEta", 100,-5.0, 5.0); m_L1TJetDijet50_maxDEta->SetDirectory(m_plotsDirectory);
  
}

void L1TAlgo::addCondition(std::function<bool(icTrg::Event &iEvent)> function){
  m_conditions.push_back(function);
}

bool L1TAlgo::run(icTrg::Event &iEvent){
  
  if(m_verbose){printf("[L1TAlgo::run] Method called for L1TAlgo name=%s\n",m_name.c_str());}
  
  for(unsigned i=0; i<m_conditions.size(); i++){
    if(m_verbose){printf("[L1TAlgo::run] Testing condition #%u : ",i);}
    if(!(m_conditions[i])(iEvent)){
      if(m_verbose){printf("failed\n");}
      return false;
    }else{
      if(m_verbose){printf("true\n");}
    }
  }
  
  if(iEvent.l1tEGammaCollection.size()>0){
    m_L1TEGamma1_Et ->Fill(iEvent.l1tEGammaCollection[0].pt());
    m_L1TEGamma1_Eta->Fill(iEvent.l1tEGammaCollection[0].phi());
    m_L1TEGamma1_Phi->Fill(iEvent.l1tEGammaCollection[0].eta());
  }
  
  if(iEvent.l1tMuonCollection.size()>0){
    m_L1TMuon1_Et ->Fill(iEvent.l1tMuonCollection[0].pt());
    m_L1TMuon1_Eta->Fill(iEvent.l1tMuonCollection[0].phi());
    m_L1TMuon1_Phi->Fill(iEvent.l1tMuonCollection[0].eta());
  }
  
  if(iEvent.l1tTauCollection.size()>0){
    m_L1TTau1_Et ->Fill(iEvent.l1tTauCollection[0].pt());
    m_L1TTau1_Phi->Fill(iEvent.l1tTauCollection[0].phi());
    m_L1TTau1_Eta->Fill(iEvent.l1tTauCollection[0].eta());
  }
  if(iEvent.l1tTauCollection.size()>1){
    m_L1TTau2_Et ->Fill(iEvent.l1tTauCollection[1].pt());
    m_L1TTau2_Phi->Fill(iEvent.l1tTauCollection[1].phi());
    m_L1TTau2_Eta->Fill(iEvent.l1tTauCollection[1].eta());
  }
  
  if(iEvent.l1tIsoTauCollection.size()>0){
    m_L1TTau1_Et ->Fill(iEvent.l1tIsoTauCollection[0].pt());
    m_L1TTau1_Phi->Fill(iEvent.l1tIsoTauCollection[0].phi());
    m_L1TTau1_Eta->Fill(iEvent.l1tIsoTauCollection[0].eta());
  }
  if(iEvent.l1tIsoTauCollection.size()>1){
    m_L1TTau2_Et ->Fill(iEvent.l1tIsoTauCollection[1].pt());
    m_L1TTau2_Phi->Fill(iEvent.l1tIsoTauCollection[1].phi());
    m_L1TTau2_Eta->Fill(iEvent.l1tIsoTauCollection[1].eta());
  }
  
  if(iEvent.l1tJetCollection.size()>0){
    m_L1TJet1_Et ->Fill(iEvent.l1tJetCollection[0].pt());
    m_L1TJet1_Phi->Fill(iEvent.l1tJetCollection[0].phi());
    m_L1TJet1_Eta->Fill(iEvent.l1tJetCollection[0].eta());
  }
  if(iEvent.l1tJetCollection.size()>1){
    m_L1TJet2_Et ->Fill(iEvent.l1tJetCollection[1].pt());
    m_L1TJet2_Phi->Fill(iEvent.l1tJetCollection[1].phi());
    m_L1TJet2_Eta->Fill(iEvent.l1tJetCollection[1].eta());
  }
  
  m_L1TJetDijet20_maxMjj->Fill(iEvent.getDijetMaxMjj(20));
  m_L1TJetDijet30_maxMjj->Fill(iEvent.getDijetMaxMjj(30));
  m_L1TJetDijet40_maxMjj->Fill(iEvent.getDijetMaxMjj(40));
  m_L1TJetDijet50_maxMjj->Fill(iEvent.getDijetMaxMjj(50));
  
  return true;
}



