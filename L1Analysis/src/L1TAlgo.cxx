#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgo.h"

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>

#include "TMath.h"

using namespace std;

L1TAlgo::L1TAlgo(){
  if(m_verbose){printf("[L1TAlgo::L1TAlgo] Method called\n");}
  init();
}

L1TAlgo::L1TAlgo(string name, TDirectory* baseBirectory){
  if(m_verbose){printf("[L1TAlgo::L1TAlgo] Method called. (name=%s)\n",name.c_str());}
  init();
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
  
}

void L1TAlgo::setVerbose(bool value){
  m_verbose = value;
}

void L1TAlgo::init(){
  
  if(m_verbose){printf("[L1TAlgo::init] Method called\n");}
  
  m_verbose = true;
  
  m_name = "";

  m_plotsDirectory = 0;
  
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
  
}

void L1TAlgo::initPlots(){
  
  if(m_verbose){printf("[L1TAlgo::initPlots] Method called\n");}
  
  m_L1TTau1_Et    = new TH1D("L1TTau1_Et", "L1T Tau1 Et", 250,           0,        250); m_L1TTau1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TTau1_Eta   = new TH1D("L1TTau1_Eta","L1T Tau1 Eta",100,          -5,          5); m_L1TTau1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TTau1_Phi   = new TH1D("L1TTau1_Phi","L1T Tau1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TTau1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TTau2_Et    = new TH1D("L1TTau2_Et", "L1T Tau2 Et", 250,           0,        250); m_L1TTau2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TTau2_Eta   = new TH1D("L1TTau2_Eta","L1T Tau2 Eta",100,          -5,          5); m_L1TTau2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TTau2_Phi   = new TH1D("L1TTau2_Phi","L1T Tau2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TTau2_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TJet1_Et    = new TH1D("L1TJet1_Et", "L1T Jet1 Et", 250,           0,        250); m_L1TJet1_Et ->SetDirectory(m_plotsDirectory);
  m_L1TJet1_Eta   = new TH1D("L1TJet1_Eta","L1T Jet1 Eta",100,          -5,          5); m_L1TJet1_Eta->SetDirectory(m_plotsDirectory);
  m_L1TJet1_Phi   = new TH1D("L1TJet1_Phi","L1T Jet1 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TJet1_Phi->SetDirectory(m_plotsDirectory);
  
  m_L1TJet2_Et    = new TH1D("L1TJet2_Et", "L1T Jet2 Et", 250,           0,        250); m_L1TJet2_Et ->SetDirectory(m_plotsDirectory);
  m_L1TJet2_Eta   = new TH1D("L1TJet2_Eta","L1T Jet2 Eta",100,          -5,          5); m_L1TJet2_Eta->SetDirectory(m_plotsDirectory);
  m_L1TJet2_Phi   = new TH1D("L1TJet2_Phi","L1T Jet2 Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TJet2_Phi->SetDirectory(m_plotsDirectory);
  
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
  
  
  if(iEvent.l1tTauCollection.size()>0){
    m_L1TTau1_Et ->Fill(iEvent.l1tTauCollection[0].pt());
    m_L1TTau1_Phi->Fill(iEvent.l1tTauCollection[0].phi());
    m_L1TTau1_Eta->Fill(iEvent.l1tTauCollection[0].eta());
  }else if(iEvent.l1tTauCollection.size()>1){
    m_L1TTau2_Et ->Fill(iEvent.l1tTauCollection[1].pt());
    m_L1TTau2_Phi->Fill(iEvent.l1tTauCollection[1].phi());
    m_L1TTau2_Eta->Fill(iEvent.l1tTauCollection[1].eta());
  }
  
  if(iEvent.l1tJetCollection.size()>0){
    m_L1TJet1_Et ->Fill(iEvent.l1tJetCollection[0].pt());
    m_L1TJet1_Phi->Fill(iEvent.l1tJetCollection[0].phi());
    m_L1TJet1_Eta->Fill(iEvent.l1tJetCollection[0].eta());
  }else if(iEvent.l1tJetCollection.size()>1){
    m_L1TJet2_Et ->Fill(iEvent.l1tJetCollection[1].pt());
    m_L1TJet2_Phi->Fill(iEvent.l1tJetCollection[1].phi());
    m_L1TJet2_Eta->Fill(iEvent.l1tJetCollection[1].eta());
  }
  
  return true;
}



