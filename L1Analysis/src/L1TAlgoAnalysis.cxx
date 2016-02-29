#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoAnalysis.h"

#include "CMSSW_VBFHiggsTauTau/L1Analysis/src/L1TFunctions.cxx"

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

PlotsSingleObjects::PlotsSingleObjects(){
  
  m_L1TEGamma_N   = 0;
  m_L1TEGamma_Et  = 0;
  m_L1TEGamma_Eta = 0;
  m_L1TEGamma_Phi = 0;
  
  m_L1TMuon_N   = 0;
  m_L1TMuon_Et  = 0;
  m_L1TMuon_Eta = 0;
  m_L1TMuon_Phi = 0;
  
  m_L1TTau_N   = 0;
  m_L1TTau_Et  = 0;
  m_L1TTau_Eta = 0;
  m_L1TTau_Phi = 0;
  
  m_L1TJet_N   = 0;
  m_L1TJet_Et  = 0;
  m_L1TJet_Eta = 0;
  m_L1TJet_Phi = 0;
}

PlotsSingleObjects::~PlotsSingleObjects(){
  
  if(m_L1TEGamma_N)  {delete m_L1TEGamma_N;}
  if(m_L1TEGamma_Et) {delete m_L1TEGamma_Et;}
  if(m_L1TEGamma_Eta){delete m_L1TEGamma_Eta;}
  if(m_L1TEGamma_Phi){delete m_L1TEGamma_Phi;}
  
  if(m_L1TMuon_N)  {delete m_L1TMuon_N;}
  if(m_L1TMuon_Et) {delete m_L1TMuon_Et;}
  if(m_L1TMuon_Eta){delete m_L1TMuon_Eta;}
  if(m_L1TMuon_Phi){delete m_L1TMuon_Phi;}
  
  if(m_L1TTau_N)  {delete m_L1TTau_N;}
  if(m_L1TTau_Et) {delete m_L1TTau_Et;}
  if(m_L1TTau_Eta){delete m_L1TTau_Eta;}
  if(m_L1TTau_Phi){delete m_L1TTau_Phi;}

  if(m_L1TJet_N)  {delete m_L1TJet_N;}
  if(m_L1TJet_Et) {delete m_L1TJet_Et;}
  if(m_L1TJet_Eta){delete m_L1TJet_Eta;}
  if(m_L1TJet_Phi){delete m_L1TJet_Phi;}
}

void PlotsSingleObjects::create(TDirectory *dir){
  
  m_L1TEGamma_N   = new TH1D("L1TEGamma_N",  "L1T Tau N",    21,        -0.5,       20.5); m_L1TEGamma_N  ->SetDirectory(dir);
  m_L1TEGamma_Et  = new TH1D("L1TEGamma_Et", "L1T Tau Et",  250,           0,        250); m_L1TEGamma_Et ->SetDirectory(dir);
  m_L1TEGamma_Eta = new TH1D("L1TEGamma_Eta","L1T Tau Eta", 100,          -5,          5); m_L1TEGamma_Eta->SetDirectory(dir);
  m_L1TEGamma_Phi = new TH1D("L1TEGamma_Phi","L1T Tau Phi", 100,-TMath::Pi(),TMath::Pi()); m_L1TEGamma_Phi->SetDirectory(dir);
  
  m_L1TMuon_N     = new TH1D("L1TMuon_N",    "L1T Muon N",   21,        -0.5,       20.5); m_L1TMuon_N    ->SetDirectory(dir);
  m_L1TMuon_Et    = new TH1D("L1TMuon_Et",   "L1T Muon Et", 250,           0,        250); m_L1TMuon_Et   ->SetDirectory(dir);
  m_L1TMuon_Eta   = new TH1D("L1TMuon_Eta",  "L1T Muon Eta",100,          -5,          5); m_L1TMuon_Eta  ->SetDirectory(dir);
  m_L1TMuon_Phi   = new TH1D("L1TMuon_Phi",  "L1T Muon Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TMuon_Phi  ->SetDirectory(dir);
  
  m_L1TTau_N      = new TH1D("L1TTau_N",     "L1T Tau N",    21,        -0.5,       20.5); m_L1TTau_N     ->SetDirectory(dir);
  m_L1TTau_Et     = new TH1D("L1TTau_Et",    "L1T Tau Et",  250,           0,        250); m_L1TTau_Et    ->SetDirectory(dir);
  m_L1TTau_Eta    = new TH1D("L1TTau_Eta",   "L1T Tau Eta", 100,          -5,          5); m_L1TTau_Eta   ->SetDirectory(dir);
  m_L1TTau_Phi    = new TH1D("L1TTau_Phi",   "L1T Tau Phi", 100,-TMath::Pi(),TMath::Pi()); m_L1TTau_Phi   ->SetDirectory(dir);
  
  m_L1TJet_N      = new TH1D("L1TJet_N",     "L1T Jet N",    21,        -0.5,       20.5); m_L1TJet_N     ->SetDirectory(dir);
  m_L1TJet_Et     = new TH1D("L1TJet_Et",    "L1T Jet Et",  250,           0,        250); m_L1TJet_Et    ->SetDirectory(dir);
  m_L1TJet_Eta    = new TH1D("L1TJet_Eta",   "L1T Jet Eta", 100,          -5,          5); m_L1TJet_Eta   ->SetDirectory(dir);
  m_L1TJet_Phi    = new TH1D("L1TJet_Phi",   "L1T Jet Phi", 100,-TMath::Pi(),TMath::Pi()); m_L1TJet_Phi   ->SetDirectory(dir);
  
}

L1TAlgoAnalysis::L1TAlgoAnalysis(){
  
  if(m_verbose){printf("[L1TAlgoAnalysis::L1TAlgoAnalysis] Method called\n");}
  init();
}

L1TAlgoAnalysis::~L1TAlgoAnalysis(){
  
  if(m_verbose){printf("[L1TAlgoAnalysis::~L1TAlgoAnalysis] Method called\n");}
  m_fileOut->Write();
}

void L1TAlgoAnalysis::init(){
  
  if(m_verbose){printf("[L1TAlgoAnalysis::init] Method called\n");}
  
  m_verbose                 = false;
  m_doSingleObjectsAnalysis = false;
  m_fileOutName             = "L1TAlgoAnalysis.root";
  
  m_dataType = L1TAlgoAnalysis::DataType::MC;
}

void L1TAlgoAnalysis::initPlots(){
  
  using namespace std::placeholders; 
  
  // Output file
  m_fileOut = new TFile(m_fileOutName.c_str(),"RECREATE");
  
  // ===== Histograms =====
  m_EventCount = new TH1D("EventCount","EventCount",1, 0.5,1.5); m_EventCount->SetDirectory(m_fileOut);
  m_HiggsDecay = new TH1D("HiggsDecay","HiggsDecay",6, 0.5,6.5); m_HiggsDecay->SetDirectory(m_fileOut);
  
  if(m_doSingleObjectsAnalysis){
    TDirectory* dirSingleObjects = m_fileOut->mkdir("SingleObject");
    m_plotsSingleObjects.create(dirSingleObjects);
  }
  
  TDirectory* dirAlgos = m_fileOut->mkdir("Algos");
  
  L1TAlgo *pAlgo;
  pAlgo = new L1TAlgo("NoCuts",dirAlgos);
  pAlgo->setVerbose(m_verbose);
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet30-30",dirAlgos);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(icTrg::testL1TJetPt,_1,30.,30.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet35-35",dirAlgos);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(icTrg::testL1TJetPt,_1,35.,35.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet40-40",dirAlgos);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(icTrg::testL1TJetPt,_1,40.,40.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet45-45",dirAlgos);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(icTrg::testL1TJetPt,_1,45.,45.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet50-50",dirAlgos);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(icTrg::testL1TJetPt,_1,50.,50.));
  m_algos.push_back(pAlgo);
 
  pAlgo = new L1TAlgo("Dijet30-30_Ditau30-30",dirAlgos);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(icTrg::testL1TJetPt,_1,30.,30.));
  pAlgo->addCondition(std::bind(icTrg::testL1TTauPt,_1,30.,30.));
  m_algos.push_back(pAlgo);
  
}

void L1TAlgoAnalysis::resetEvent(){
  
  if(m_verbose){printf("[L1TAlgoAnalysis::resetEvent] Method called\n");}
  
}

void L1TAlgoAnalysis::setVerbose(bool value){
  if(m_verbose){printf("[L1TAlgoAnalysis::setVerbose] Method called\n");}
  m_verbose=value;
}

void L1TAlgoAnalysis::setDataType(L1TAlgoAnalysis::DataType type){
  if(m_verbose){printf("[L1TAlgoAnalysis::setDataType] Method called\n");}
  m_dataType=type;
}

void L1TAlgoAnalysis::setDoSingleObjectsAnalysis(bool value){
  if(m_verbose){printf("[L1TAlgoAnalysis::setDoSingleObjectsAnalysis] Method called\n");}
  m_doSingleObjectsAnalysis=value;
}

void L1TAlgoAnalysis::doSingleObjectsAnalysis(icTrg::Event &iEvent){
  
  m_plotsSingleObjects.m_L1TEGamma_N->Fill(iEvent.l1tEGammaCollection.size());
  for(unsigned i=0; i<iEvent.l1tEGammaCollection.size(); i++){
    
    ic::L1TEGamma* iEGamma = &(iEvent.l1tEGammaCollection.at(i));
    
    if(m_verbose){printf("[L1TAlgoAnalysis::processEvent] L1T EGamma #%i - et=%5.1f eta=%5.2f phi=%5.2f\n",i,iEGamma->pt(),iEGamma->eta(),iEGamma->phi());}
    
    m_plotsSingleObjects.m_L1TEGamma_Et ->Fill(iEGamma->pt());
    m_plotsSingleObjects.m_L1TEGamma_Eta->Fill(iEGamma->eta());
    m_plotsSingleObjects.m_L1TEGamma_Phi->Fill(iEGamma->phi());
  }
  
  m_plotsSingleObjects.m_L1TMuon_N->Fill(iEvent.l1tMuonCollection.size());
  for(unsigned i=0; i<iEvent.l1tMuonCollection.size(); i++){
    
    ic::L1TMuon* iMuon = &(iEvent.l1tMuonCollection.at(i));
    
    if(m_verbose){printf("[L1TAlgoAnalysis::processEvent] L1T Muon #%i - et=%5.1f eta=%5.2f phi=%5.2f\n",i,iMuon->pt(),iMuon->eta(),iMuon->phi());}
    
    m_plotsSingleObjects.m_L1TMuon_Et ->Fill(iMuon->pt());
    m_plotsSingleObjects.m_L1TMuon_Eta->Fill(iMuon->eta());
    m_plotsSingleObjects.m_L1TMuon_Phi->Fill(iMuon->phi());
  }
  
  m_plotsSingleObjects.m_L1TTau_N->Fill(iEvent.l1tTauCollection.size());
  for(unsigned i=0; i<iEvent.l1tTauCollection.size(); i++){
    
    ic::L1TTau* iTau = &(iEvent.l1tTauCollection.at(i));
    
    if(m_verbose){printf("[L1TAlgoAnalysis::processEvent] L1T Tau #%i - et=%5.1f eta=%5.2f phi=%5.2f\n",i,iTau->pt(),iTau->eta(),iTau->phi());}
    
    m_plotsSingleObjects.m_L1TTau_Et ->Fill(iTau->pt());
    m_plotsSingleObjects.m_L1TTau_Eta->Fill(iTau->eta());
    m_plotsSingleObjects.m_L1TTau_Phi->Fill(iTau->phi());
    

    
  }
  
  m_plotsSingleObjects.m_L1TJet_N->Fill(iEvent.l1tJetCollection.size());
  for(unsigned i=0; i<iEvent.l1tJetCollection.size(); i++){
    
    ic::L1TJet* iJet = &(iEvent.l1tJetCollection.at(i));
    
    if(m_verbose){printf("[L1TAlgoAnalysis::processEvent] L1T Jet #%i - et=%5.1f eta=%5.2f phi=%5.2f\n",i,iJet->pt(),iJet->eta(),iJet->phi());}
    
    m_plotsSingleObjects.m_L1TJet_Et ->Fill(iJet->pt());
    m_plotsSingleObjects.m_L1TJet_Eta->Fill(iJet->eta());
    m_plotsSingleObjects.m_L1TJet_Phi->Fill(iJet->phi());
  }
}

void L1TAlgoAnalysis::processEvent(icTrg::Event &iEvent){
  if(m_verbose){printf("[L1TAlgoAnalysis::processEvent] Method called\n");}
  
  m_EventCount->Fill(1);

  if(m_doSingleObjectsAnalysis){doSingleObjectsAnalysis(iEvent);}
  
  for(unsigned i=0; i<m_algos.size(); i++){m_algos[i]->run(iEvent);}
  
   
  
}
