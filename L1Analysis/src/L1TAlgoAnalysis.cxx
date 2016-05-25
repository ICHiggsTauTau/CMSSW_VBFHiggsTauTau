#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoAnalysis.h"

#include "CMSSW_VBFHiggsTauTau/L1Analysis/src/L1TFunctions.cxx"

#include "TTree.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

L1TAlgoAnalysis::L1TAlgoAnalysis(){
  init();
  if(m_verbose){printf("[L1TAlgoAnalysis::L1TAlgoAnalysis] Method called\n");}
}

L1TAlgoAnalysis::L1TAlgoAnalysis(std::string name,TDirectory* baseDirector){
  init();
  if(m_verbose){printf("[L1TAlgoAnalysis::L1TAlgoAnalysis] Method called\n");}
  
  m_name = name;
  m_dir  = baseDirector;
}

L1TAlgoAnalysis::~L1TAlgoAnalysis(){
  
}

void L1TAlgoAnalysis::init(){
  
  if(m_verbose){printf("[L1TAlgoAnalysis::init] Method called\n");}
  
  m_name                    = "";
  m_dir                     = 0;
  m_verbose                 = false;
  m_doGenAnalysis           = false;
  m_doSingleObjectsAnalysis = false;

  m_dataType = L1TAlgoAnalysis::DataType::MC;

}

void L1TAlgoAnalysis::beginJob(){
  
  if(m_verbose){cout << "[L1TAlgoAnalysis::beginJob] Method called" << endl;}
  
  using namespace std::placeholders; 
  
  // ===== Histograms =====
  m_EventCount = new TH1D("EventCount","EventCount",1, 0.5,1.5); m_EventCount->SetDirectory(m_dir);
  
  if(m_doGenAnalysis){
    if(m_verbose){cout << "[L1TAlgoAnalysis::beginJob] Generator level analysis will be performed" << endl;}
    TDirectory* dirGenAnalysis = m_dir->mkdir("GenAnalysis");
    m_plotsGenAnalysis.create(dirGenAnalysis);
  }
  
  if(m_doSingleObjectsAnalysis){
    if(m_verbose){cout << "[L1TAlgoAnalysis::beginJob] Single object analysis will be performed" << endl;}
    TDirectory* dirSingleObjects = m_dir->mkdir("SingleObject");
    m_plotsSingleObjects.create(dirSingleObjects);
  }
  
  TDirectory* dirAlgos = m_dir->mkdir("Algos");
  
  L1TAlgo *pAlgo;
  pAlgo = new L1TAlgo("NoCuts",dirAlgos);
  pAlgo->setVerbose(m_verbose);
  m_algos.push_back(pAlgo);
  
  TDirectory* dirNoClean                  = dirAlgos->mkdir("NoClean");
  TDirectory* dirOverlapClean_LeptonFirst = dirAlgos->mkdir("OverlapCleaned_LeptonFirst");
  
  // #######################################
  // ########### Single Objects ############
  // #######################################

  //#############################################################
  // Algo: EG
  //#############################################################
  for(double i=10; i<=40; i+=5){
    pAlgo = new L1TAlgo(Form("EG%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    string name_egCol = Form("l1t_eg_pt%.0f",i);
    pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,_1,"l1t_eg",i,name_egCol));
    pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,     _1,name_egCol,1));
    pAlgo->results->tag_l1tEG = name_egCol;
    m_algos.push_back(pAlgo);
    
    
    // Algo: Double Jet
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("Dijet%.0f_EG%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_egCol        = Form("l1t_eg_pt%.0f",i);
      string name_eg1Col       = Form("l1t_eg1_pt%.0f",i);
      string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
      string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanEG1Pt%.0f",jetPt,i);
      string name_dijetCol     = Form("l1t_dijet_pt%.0f_cleanEG1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,           _1,"l1t_eg",i,name_egCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                _1,name_egCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,        _1,name_egCol,name_eg1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,_1,name_eg1Col,name_jetCol,0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
      pAlgo->results->tag_l1tEG      = name_eg1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0fmjj%.0f_EG%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_egCol        = Form("l1t_eg_pt%.0f",i);
        string name_eg1Col       = Form("l1t_eg1_pt%.0f",i);
        string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
        string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanEG1Pt%.0f",jetPt,i);
        string name_dijetCol     = Form("l1t_dijetPt%.0fmjj%.0f_cleanEG1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,           _1,"l1t_eg",i,name_egCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                _1,name_egCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,        _1,name_egCol,name_eg1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,_1,name_eg1Col,name_jetCol,0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
        pAlgo->results->tag_l1tEG      = name_eg1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0fdeta%2.0fmjj%.0f_EG%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_egCol        = Form("l1t_eg_pt%.0f",i);
          string name_eg1Col       = Form("l1t_eg1_pt%.0f",i);
          string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
          string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanEG1Pt%.0f",jetPt,i);
          string name_dijetCol     = Form("l1t_dijetPt%.0fdEta%2.0fmjj%.0f_cleanEG1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,           _1,"l1t_eg",i,name_egCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                _1,name_egCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,        _1,name_egCol,name_eg1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,_1,name_eg1Col,name_jetCol,0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
          pAlgo->results->tag_l1tEG      = name_eg1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
        }
      }
    }
    
    
    // Algo: Double Jet asymmetric pT
    for(double jetPt1=10; jetPt1<=100; jetPt1+=10){
      for(double jetPt2=10; jetPt2<=jetPt1; jetPt2+=10){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0f_EG%.0f",jetPt1,jetPt2,i),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_eg_pt%.0f",i);
        string name_tau1Col      = Form("l1t_eg1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanEG1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0f_cleanEG1Pt%.0f",jetPt1,jetPt2,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,               _1,"l1t_eg",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                    _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,            _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,0,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
        pAlgo->results->tag_l1tEG  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fmjj%.0f_EG%.0f",jetPt1,jetPt2,jetMjj,i),dirNoClean);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_eg_pt%.0f",i);
          string name_tau1Col      = Form("l1t_eg1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanEG1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fmjj%.0f_cleanEG1Pt%.0f",jetPt1,jetPt2,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,               _1,"l1t_eg",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                    _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,            _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
          pAlgo->results->tag_l1tEG  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
          for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
            
            pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fdeta%2.0fmjj%.0f_EG%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i),dirNoClean);
            pAlgo->setVerbose(m_verbose);
            string name_tauCol       = Form("l1t_eg_pt%.0f",i);
            string name_tau1Col      = Form("l1t_eg1_pt%.0f",i);
            string name_jetCol_clean = Form("l1t_jet_cleanEG1Pt%.0f",i);
            string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fdeta%2.0fmjj%.0f_cleanEG1Pt%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i);
            pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,               _1,"l1t_eg",i,name_tauCol));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                    _1,name_tauCol,1));
            pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,            _1,name_tauCol,name_tau1Col));
            pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
            pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,jetDEta,jetMjj,name_dijetCol));
            pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
            pAlgo->results->tag_l1tEG  = name_tau1Col;
            pAlgo->results->tag_l1tJetPair = name_dijetCol;
            m_algos.push_back(pAlgo);
          }
        }
      }
    }
    
    
    // Algo: Double Jet Avg Pt
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("DijetAvg%.0f_EG%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_tauCol       = Form("l1t_eg_pt%.0f",i);
      string name_tau1Col      = Form("l1t_eg1_pt%.0f",i);
      string name_jetCol_clean = Form("l1t_jet_cleanEG1Pt%.0f",i);
      string name_dijetCol     = Form("l1t_dijetAvgPt%.0f_cleanEG1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,            _1,"l1t_eg",i,name_tauCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                 _1,name_tauCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,         _1,name_tauCol,name_tau1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
      pAlgo->results->tag_l1tEG  = name_tau1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("DijetAvg%.0fmjj%.0f_EG%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_eg_pt%.0f",i);
        string name_tau1Col      = Form("l1t_eg1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanEG1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetAvgPt%.0fmjj%.0f_cleanEG1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,            _1,"l1t_eg",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                 _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,         _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
        pAlgo->results->tag_l1tEG  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("DijetAvg%.0fdeta%2.0fmjj%.0f_EG%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_eg_pt%.0f",i);
          string name_tau1Col      = Form("l1t_eg1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanEG1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetAvgPt%.0fdeta%2.0fmjj%.0f_cleanEG1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,            _1,"l1t_eg",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                 _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,         _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
          pAlgo->results->tag_l1tEG  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
        }
      }
    }
  }
  
  //#############################################################
  // Algo: IsoEG
  //#############################################################
  for(double i=10; i<=40; i+=5){
    pAlgo = new L1TAlgo(Form("IsoEG%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    string name_egCol = Form("l1t_isoeg_pt%.0f",i);
    pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,_1,"l1t_isoeg",i,name_egCol));
    pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,     _1,name_egCol,1));
    pAlgo->results->tag_l1tIsoEG = name_egCol;
    m_algos.push_back(pAlgo);
    
    
    // Algo: Double Jet
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("Dijet%.0f_IsoEG%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_egCol        = Form("l1t_isoeg_pt%.0f",i);
      string name_eg1Col       = Form("l1t_isoeg1_pt%.0f",i);
      string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
      string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanIsoG1Pt%.0f",jetPt,i);
      string name_dijetCol     = Form("l1t_dijet_pt%.0f_cleanIsoEG1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,           _1,"l1t_isoeg",i,name_egCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                _1,name_egCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,        _1,name_egCol,name_eg1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,_1,name_eg1Col,name_jetCol,0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
      pAlgo->results->tag_l1tIsoEG  = name_eg1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0fmjj%.0f_IsoEG%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_egCol        = Form("l1t_isoeg_pt%.0f",i);
        string name_eg1Col       = Form("l1t_isoeg1_pt%.0f",i);
        string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
        string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanIsoEG1Pt%.0f",jetPt,i);
        string name_dijetCol     = Form("l1t_dijetPt%.0fmjj%.0f_cleanIsoEG1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,           _1,"l1t_isoeg",i,name_egCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                _1,name_egCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,        _1,name_egCol,name_eg1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,_1,name_eg1Col,name_jetCol,0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
        pAlgo->results->tag_l1tIsoEG   = name_eg1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0fdeta%2.0fmjj%.0f_IsoEG%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_egCol       = Form("l1t_isoeg_pt%.0f",i);
          string name_eg1Col      = Form("l1t_isoeg1_pt%.0f",i);
          string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
          string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanIsoEG1Pt%.0f",jetPt,i);
          string name_dijetCol     = Form("l1t_dijetPt%.0fdEta%2.0fmjj%.0f_cleanIsoEG1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,           _1,"l1t_isoeg",i,name_egCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                _1,name_egCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,        _1,name_egCol,name_eg1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,_1,name_eg1Col,name_jetCol,0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
          pAlgo->results->tag_l1tIsoEG  = name_eg1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
        }
      }
    }
    
    
    // Algo: Double Jet asymmetric pT
    for(double jetPt1=10; jetPt1<=100; jetPt1+=10){
      for(double jetPt2=10; jetPt2<=jetPt1; jetPt2+=10){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0f_IsoEG%.0f",jetPt1,jetPt2,i),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_isoeg_pt%.0f",i);
        string name_tau1Col      = Form("l1t_isoeg1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanIsoEG1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0f_cleanIsoEG1Pt%.0f",jetPt1,jetPt2,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,               _1,"l1t_isoeg",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                    _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,            _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,0,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
        pAlgo->results->tag_l1tIsoEG  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fmjj%.0f_IsoEG%.0f",jetPt1,jetPt2,jetMjj,i),dirNoClean);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_isoeg_pt%.0f",i);
          string name_tau1Col      = Form("l1t_isoeg1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanIsoEG1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fmjj%.0f_cleanIsoEG1Pt%.0f",jetPt1,jetPt2,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,            _1,"l1t_isoeg",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                 _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,         _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
          pAlgo->results->tag_l1tIsoEG  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
          for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
            
            pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fdeta%2.0fmjj%.0f_IsoEG%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i),dirNoClean);
            pAlgo->setVerbose(m_verbose);
            string name_tauCol       = Form("l1t_isoeg_pt%.0f",i);
            string name_tau1Col      = Form("l1t_isoeg1_pt%.0f",i);
            string name_jetCol_clean = Form("l1t_jet_cleanIsoEG1Pt%.0f",i);
            string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fdeta%2.0fmjj%.0f_cleanIsoEG1Pt%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i);
            pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,            _1,"l1t_isoeg",i,name_tauCol));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                 _1,name_tauCol,1));
            pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,         _1,name_tauCol,name_tau1Col));
            pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
            pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,jetDEta,jetMjj,name_dijetCol));
            pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
            pAlgo->results->tag_l1tIsoEG  = name_tau1Col;
            pAlgo->results->tag_l1tJetPair = name_dijetCol;
            m_algos.push_back(pAlgo);
          }
        }
      }
    }
    
    
    // Algo: Double Jet Avg Pt
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("DijetAvg%.0f_IsoEG%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_tauCol       = Form("l1t_isoeg_pt%.0f",i);
      string name_tau1Col      = Form("l1t_isoeg1_pt%.0f",i);
      string name_jetCol_clean = Form("l1t_jet_cleanIsoEG1Pt%.0f",i);
      string name_dijetCol     = Form("l1t_dijetAvgPt%.0f_cleanIsoEG1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,           _1,"l1t_isoeg",i,name_tauCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                _1,name_tauCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,        _1,name_tauCol,name_tau1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>,_1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,  _1,name_jetCol_clean,false,jetPt,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
      pAlgo->results->tag_l1tIsoEG  = name_tau1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("DijetAvg%.0fmjj%.0f_IsoEG%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_isoeg_pt%.0f",i);
        string name_tau1Col      = Form("l1t_isoeg1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanIsoEG1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetAvgPt%.0fmjj%.0f_cleanIsoEG1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,            _1,"l1t_isoeg",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                 _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,         _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
        pAlgo->results->tag_l1tIsoEG  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("DijetAvg%.0fdeta%2.0fmjj%.0f_IsoEG%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_isoeg_pt%.0f",i);
          string name_tau1Col      = Form("l1t_isoeg1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanIsoEG1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetAvgPt%.0fdeta%2.0fmjj%.0f_cleanIsoEG1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TEGamma>,            _1,"l1t_isoeg",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TEGamma>,                 _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TEGamma>,         _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TEGamma,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
          pAlgo->results->tag_l1tIsoEG  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
        }
      }
    }
  }
  
  //#############################################################
  // Algo: Mu
  //#############################################################
  for(double i=10; i<=40; i+=5){
    pAlgo = new L1TAlgo(Form("Mu%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    string name_muCol = Form("l1t_muon_pt%.0f",i);
    pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,_1,"l1t_muon",i,name_muCol));
    pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,     _1,name_muCol,1));
    pAlgo->results->tag_l1tMuon = name_muCol;
    m_algos.push_back(pAlgo);
    
    
    // Algo: Double Jet
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("Dijet%.0f_Muon%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_muonCol      = Form("l1t_muon_pt%.0f",i);
      string name_muon1Col     = Form("l1t_muon1_pt%.0f",i);
      string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
      string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanMuon1Pt%.0f",jetPt,i);
      string name_dijetCol     = Form("l1t_dijet_pt%.0f_cleanMuon1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,              _1,"l1t_muon",i,name_muonCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                   _1,name_muonCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,           _1,name_muonCol,name_muon1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,   _1,name_muon1Col,name_jetCol,0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
      pAlgo->results->tag_l1tMuon    = name_muon1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0fmjj%.0f_Muon%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_muonCol      = Form("l1t_muon_pt%.0f",i);
        string name_muon1Col     = Form("l1t_muon1_pt%.0f",i);
        string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
        string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanMuon1Pt%.0f",jetPt,i);
        string name_dijetCol     = Form("l1t_dijetPt%.0fmjj%.0f_cleanMuon1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,             _1,"l1t_muon",i,name_muonCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                  _1,name_muonCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,          _1,name_muonCol,name_muon1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,  _1,name_muon1Col,name_jetCol,0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
        pAlgo->results->tag_l1tMuon    = name_muon1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0fdeta%2.0fmjj%.0f_Muon%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_muonCol      = Form("l1t_muon_pt%.0f",i);
          string name_muon1Col     = Form("l1t_muon1_pt%.0f",i);
          string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
          string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanMuon1Pt%.0f",jetPt,i);
          string name_dijetCol     = Form("l1t_dijetPt%.0fdEta%2.0fmjj%.0f_cleanMuon1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,             _1,"l1t_muon",i,name_muonCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                  _1,name_muonCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,          _1,name_muonCol,name_muon1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,  _1,name_muon1Col,name_jetCol,0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
          pAlgo->results->tag_l1tMuon    = name_muon1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
        }
      }
    }
    
    
    // Algo: Double Jet asymmetric pT
    for(double jetPt1=10; jetPt1<=100; jetPt1+=10){
      for(double jetPt2=10; jetPt2<=jetPt1; jetPt2+=10){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0f_Muon%.0f",jetPt1,jetPt2,i),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_muon_pt%.0f",i);
        string name_tau1Col      = Form("l1t_muon1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanMuon1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0f_cleanMuon1Pt%.0f",jetPt1,jetPt2,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,              _1,"l1t_muon",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                   _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,           _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,   _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,0,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
        pAlgo->results->tag_l1tMuon  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fmjj%.0f_Muon%.0f",jetPt1,jetPt2,jetMjj,i),dirNoClean);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_muon_pt%.0f",i);
          string name_tau1Col      = Form("l1t_muon1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanMuon1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fmjj%.0f_cleanMuon1Pt%.0f",jetPt1,jetPt2,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,               _1,"l1t_muon",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                    _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,            _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                     _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
          pAlgo->results->tag_l1tMuon  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
          for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
            
            pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fdeta%2.0fmjj%.0f_Muon%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i),dirNoClean);
            pAlgo->setVerbose(m_verbose);
            string name_tauCol       = Form("l1t_muon_pt%.0f",i);
            string name_tau1Col      = Form("l1t_muon1_pt%.0f",i);
            string name_jetCol_clean = Form("l1t_jet_cleanMuon1Pt%.0f",i);
            string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fdeta%2.0fmjj%.0f_cleanMuon1Pt%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i);
            pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,               _1,"l1t_muon",i,name_tauCol));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                    _1,name_tauCol,1));
            pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,            _1,name_tauCol,name_tau1Col));
            pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
            pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,jetDEta,jetMjj,name_dijetCol));
            pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
            pAlgo->results->tag_l1tMuon  = name_tau1Col;
            pAlgo->results->tag_l1tJetPair = name_dijetCol;
            m_algos.push_back(pAlgo);
          }
        }
      }
    }
    
    
    // Algo: Double Jet Avg Pt
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("DijetAvg%.0f_Muon%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_tauCol       = Form("l1t_muon_pt%.0f",i);
      string name_tau1Col      = Form("l1t_muon1_pt%.0f",i);
      string name_jetCol_clean = Form("l1t_jet_cleanMuon1Pt%.0f",i);
      string name_dijetCol     = Form("l1t_dijetAvgPt%.0f_cleanMuon1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,            _1,"l1t_muon",i,name_tauCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                 _1,name_tauCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,         _1,name_tauCol,name_tau1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
      pAlgo->results->tag_l1tMuon  = name_tau1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("DijetAvg%.0fmjj%.0f_Muon%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_muon_pt%.0f",i);
        string name_tau1Col      = Form("l1t_muon1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanMuon1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetAvgPt%.0fmjj%.0f_cleanMuon1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,            _1,"l1t_muon",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                 _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,         _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
        pAlgo->results->tag_l1tMuon  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("DijetAvg%.0fdeta%2.0fmjj%.0f_Muon%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_muon_pt%.0f",i);
          string name_tau1Col      = Form("l1t_muon1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanMuon1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetAvgPt%.0fdeta%2.0fmjj%.0f_cleanMuon1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,            _1,"l1t_muon",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                 _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,         _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
          pAlgo->results->tag_l1tMuon  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
        }
      }
    }
  }
  
  //#############################################################
  // Algo: IsoMu
  //#############################################################
  for(double i=10; i<=40; i+=5){
    pAlgo = new L1TAlgo(Form("IsoMu%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    string name_muCol = Form("l1t_isomuon_pt%.0f",i);
    pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,_1,"l1t_isomuon",i,name_muCol));
    pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,     _1,name_muCol,1));
    pAlgo->results->tag_l1tMuon = name_muCol;
    m_algos.push_back(pAlgo);
    
    
    // Algo: Double Jet
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("Dijet%.0f_IsoMuon%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_muonCol       = Form("l1t_isomuon_pt%.0f",i);
      string name_muon1Col      = Form("l1t_isomuon1_pt%.0f",i);
      string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
      string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanIsoMuon1Pt%.0f",jetPt,i);
      string name_dijetCol     = Form("l1t_dijet_pt%.0f_cleanIsoMuon1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,              _1,"l1t_isomuon",i,name_muonCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                   _1,name_muonCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,           _1,name_muonCol,name_muon1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,   _1,name_muon1Col,name_jetCol,0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
      pAlgo->results->tag_l1tIsoMuon = name_muon1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0fmjj%.0f_IsoMuon%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_muonCol      = Form("l1t_isomuon_pt%.0f",i);
        string name_muon1Col     = Form("l1t_isomuon1_pt%.0f",i);
        string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
        string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanIsoMuon1Pt%.0f",jetPt,i);
        string name_dijetCol     = Form("l1t_dijetPt%.0fmjj%.0f_cleanIsoMuon1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,             _1,"l1t_isomuon",i,name_muonCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                  _1,name_muonCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,          _1,name_muonCol,name_muon1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,  _1,name_muon1Col,name_jetCol,0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
        pAlgo->results->tag_l1tIsoMuon = name_muon1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0fdeta%2.0fmjj%.0f_IsoMuon%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_muonCol      = Form("l1t_isomuon_pt%.0f",i);
          string name_muon1Col     = Form("l1t_isomuon1_pt%.0f",i);
          string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
          string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanIsoMuon1Pt%.0f",jetPt,i);
          string name_dijetCol     = Form("l1t_dijetPt%.0fdEta%2.0fmjj%.0f_cleanIsoMuon1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,             _1,"l1t_isomuon",i,name_muonCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                  _1,name_muonCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,          _1,name_muonCol,name_muon1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,  _1,name_muon1Col,name_jetCol,0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
          pAlgo->results->tag_l1tIsoMuon = name_muon1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
        }
      }
    }
    
    // Algo: Double Jet asymmetric pT
    for(double jetPt1=10; jetPt1<=100; jetPt1+=10){
      for(double jetPt2=10; jetPt2<=jetPt1; jetPt2+=10){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0f_IsoMuon%.0f",jetPt1,jetPt2,i),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_isomuon_pt%.0f",i);
        string name_tau1Col      = Form("l1t_isomuon1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanIsoMuon1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0f_cleanIsoMuon1Pt%.0f",jetPt1,jetPt2,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,               _1,"l1t_isomuon",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                    _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,            _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,0,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
        pAlgo->results->tag_l1tIsoMuon  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fmjj%.0f_IsoMuon%.0f",jetPt1,jetPt2,jetMjj,i),dirNoClean);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_isomuon_pt%.0f",i);
          string name_tau1Col      = Form("l1t_isomuon1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanIsoMuon1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fmjj%.0f_cleanIsoMuon1Pt%.0f",jetPt1,jetPt2,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,               _1,"l1t_isomuon",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                    _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,            _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
          pAlgo->results->tag_l1tIsoMuon  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
          for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
            
            pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fdeta%2.0fmjj%.0f_IsoMuon%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i),dirNoClean);
            pAlgo->setVerbose(m_verbose);
            string name_tauCol       = Form("l1t_isomuon_pt%.0f",i);
            string name_tau1Col      = Form("l1t_isomuon1_pt%.0f",i);
            string name_jetCol_clean = Form("l1t_jet_cleanIsoMuon1Pt%.0f",i);
            string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fdeta%2.0fmjj%.0f_cleanIsoMuon1Pt%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i);
            pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,               _1,"l1t_isomuon",i,name_tauCol));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                    _1,name_tauCol,1));
            pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,            _1,name_tauCol,name_tau1Col));
            pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
            pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,jetDEta,jetMjj,name_dijetCol));
            pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
            pAlgo->results->tag_l1tIsoMuon  = name_tau1Col;
            pAlgo->results->tag_l1tJetPair = name_dijetCol;
            m_algos.push_back(pAlgo);
          }
        }
      }
    }
    
    // Algo: Double Jet Avg Pt
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("DijetAvg%.0f_IsoMuon%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_tauCol       = Form("l1t_isomuon_pt%.0f",i);
      string name_tau1Col      = Form("l1t_isomuon1_pt%.0f",i);
      string name_jetCol_clean = Form("l1t_jet_cleanIsoMuon1Pt%.0f",i);
      string name_dijetCol     = Form("l1t_dijetAvgPt%.0f_cleanIsoMuon1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,            _1,"l1t_isomuon",i,name_tauCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                 _1,name_tauCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,         _1,name_tauCol,name_tau1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
      pAlgo->results->tag_l1tIsoMuon  = name_tau1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("DijetAvg%.0fmjj%.0f_IsoMuon%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_isomuon_pt%.0f",i);
        string name_tau1Col      = Form("l1t_isomuon1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanIsoMuon1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetAvgPt%.0fmjj%.0f_cleanIsoMuon1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,            _1,"l1t_isomuon",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                 _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,         _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
        pAlgo->results->tag_l1tIsoMuon  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("DijetAvg%.0fdeta%2.0fmjj%.0f_IsoMuon%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_isomuon_pt%.0f",i);
          string name_tau1Col      = Form("l1t_isomuon1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanIsoMuon1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetAvgPt%.0fdeta%2.0fmjj%.0f_cleanIsoMuon1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TMuon>,           _1,"l1t_isomuon",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TMuon>,                _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TMuon>,        _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TMuon,ic::L1TJet>,_1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
          pAlgo->results->tag_l1tIsoMuon = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
        }
      }
    }
  }
  
  //#############################################################
  // Algo: Tau
  //#############################################################
  for(double i=10; i<=40; i+=5){
    pAlgo = new L1TAlgo(Form("Tau%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    string name_tauCol = Form("l1t_tau_pt%.0f",i);
    pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,_1,"l1t_tau",i,name_tauCol));
    pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,     _1,name_tauCol,1));
    pAlgo->results->tag_l1tTau = name_tauCol;
    m_algos.push_back(pAlgo);
    
    // Algo: Double Jet
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("Dijet%.0f_Tau%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_tauCol       = Form("l1t_tau_pt%.0f",i);
      string name_tau1Col      = Form("l1t_tau1_pt%.0f",i);
      string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
      string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanTau1Pt%.0f",jetPt,i);
      string name_dijetCol     = Form("l1t_dijet_pt%.0f_cleanTau1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,              _1,"l1t_tau",i,name_tauCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                   _1,name_tauCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,           _1,name_tauCol,name_tau1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,   _1,name_tau1Col,name_jetCol,0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
      pAlgo->results->tag_l1tTau  = name_tau1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0fmjj%.0f_Tau%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_tau_pt%.0f",i);
        string name_tau1Col      = Form("l1t_tau1_pt%.0f",i);
        string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
        string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanTau1Pt%.0f",jetPt,i);
        string name_dijetCol     = Form("l1t_dijetPt%.0fmjj%.0f_cleanTau1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,              _1,"l1t_tau",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                   _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,           _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,   _1,name_tau1Col,name_jetCol,0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
        pAlgo->results->tag_l1tTau  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0fdeta%2.0fmjj%.0f_Tau%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_tau_pt%.0f",i);
          string name_tau1Col      = Form("l1t_tau1_pt%.0f",i);
          string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
          string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanTau1Pt%.0f",jetPt,i);
          string name_dijetCol     = Form("l1t_dijetPt%.0fdEta%2.0fmjj%.0f_cleanTau1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,              _1,"l1t_tau",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                   _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,           _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,   _1,name_tau1Col,name_jetCol,0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
          pAlgo->results->tag_l1tTau     = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
        }
      }
    }
    
    // Algo: Double Jet asymmetric pT
    for(double jetPt1=10; jetPt1<=100; jetPt1+=10){
      for(double jetPt2=10; jetPt2<=jetPt1; jetPt2+=10){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0f_Tau%.0f",jetPt1,jetPt2,i),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_tau_pt%.0f",i);
        string name_tau1Col      = Form("l1t_tau1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanTau1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0f_cleanTau1Pt%.0f",jetPt1,jetPt2,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,               _1,"l1t_tau",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                    _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,            _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,0,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
        pAlgo->results->tag_l1tTau  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fmjj%.0f_Tau%.0f",jetPt1,jetPt2,jetMjj,i),dirNoClean);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_tau_pt%.0f",i);
          string name_tau1Col      = Form("l1t_tau1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanTau1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fmjj%.0f_cleanTau1Pt%.0f",jetPt1,jetPt2,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,               _1,"l1t_tau",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                    _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,            _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
          pAlgo->results->tag_l1tTau  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
          for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
            
            pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fdeta%2.0fmjj%.0f_Tau%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i),dirNoClean);
            pAlgo->setVerbose(m_verbose);
            string name_tauCol       = Form("l1t_tau_pt%.0f",i);
            string name_tau1Col      = Form("l1t_tau1_pt%.0f",i);
            string name_jetCol_clean = Form("l1t_jet_cleanTau1Pt%.0f",i);
            string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fdeta%2.0fmjj%.0f_cleanTau1Pt%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i);
            pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,               _1,"l1t_tau",i,name_tauCol));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                    _1,name_tauCol,1));
            pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,            _1,name_tauCol,name_tau1Col));
            pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
            pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,jetDEta,jetMjj,name_dijetCol));
            pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
            pAlgo->results->tag_l1tTau  = name_tau1Col;
            pAlgo->results->tag_l1tJetPair = name_dijetCol;
            m_algos.push_back(pAlgo);
          }
        }
      }
    }
    
    
    // Algo: Double Jet Avg Pt
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("DijetAvg%.0f_Tau%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_tauCol       = Form("l1t_tau_pt%.0f",i);
      string name_tau1Col      = Form("l1t_tau1_pt%.0f",i);
      string name_jetCol_clean = Form("l1t_jet_cleanTau1Pt%.0f",i);
      string name_dijetCol     = Form("l1t_dijetAvgPt%.0f_cleanTau1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,            _1,"l1t_tau",i,name_tauCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                 _1,name_tauCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,         _1,name_tauCol,name_tau1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
      pAlgo->results->tag_l1tTau  = name_tau1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("DijetAvg%.0fmjj%.0f_Tau%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_tau_pt%.0f",i);
        string name_tau1Col      = Form("l1t_tau1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanTau1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetAvgPt%.0fmjj%.0f_cleanTau1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,            _1,"l1t_tau",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                 _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,         _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
        pAlgo->results->tag_l1tTau  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("DijetAvg%.0fdeta%2.0fmjj%.0f_Tau%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_tau_pt%.0f",i);
          string name_tau1Col      = Form("l1t_tau1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanTau1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetAvgPt%.0fdeta%2.0fmjj%.0f_cleanTau1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,            _1,"l1t_tau",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                 _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,         _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
          pAlgo->results->tag_l1tTau  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
        }
      }
    }
  }
  
  //#############################################################
  // Algo: IsoTau
  //#############################################################
  for(double i=10; i<=40; i+=5){
    pAlgo = new L1TAlgo(Form("IsoTau%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    string name_tauCol = Form("l1t_isotau_pt%.0f",i);
    pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,_1,"l1t_isotau",i,name_tauCol));
    pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,     _1,name_tauCol,1));
    pAlgo->results->tag_l1tIsoTau = name_tauCol;
    m_algos.push_back(pAlgo);
    
    // Algo: Double Jet
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("Dijet%.0f_IsoTau%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_tauCol       = Form("l1t_isotau_pt%.0f",i);
      string name_tau1Col      = Form("l1t_isotau1_pt%.0f",i);
      string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
      string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanIsoTau1Pt%.0f",jetPt,i);
      string name_dijetCol     = Form("l1t_dijet_pt%.0f_cleanIsoTau1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,              _1,"l1t_isotau",i,name_tauCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                   _1,name_tauCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,           _1,name_tauCol,name_tau1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,   _1,name_tau1Col,name_jetCol,0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
      pAlgo->results->tag_l1tIsoTau  = name_tau1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0fmjj%.0f_IsoTau%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_isotau_pt%.0f",i);
        string name_tau1Col      = Form("l1t_isotau1_pt%.0f",i);
        string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
        string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanIsoTau1Pt%.0f",jetPt,i);
        string name_dijetCol     = Form("l1t_dijetPt%.0fmjj%.0f_cleanIsoTau1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,              _1,"l1t_isotau",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                   _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,           _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,   _1,name_tau1Col,name_jetCol,0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
        pAlgo->results->tag_l1tIsoTau  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0fdeta%2.0fmjj%.0f_IsoTau%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_isotau_pt%.0f",i);
          string name_tau1Col      = Form("l1t_isotau1_pt%.0f",i);
          string name_jetCol       = Form("l1t_jet_pt%.0f",jetPt);
          string name_jetCol_clean = Form("l1t_jet_pt%.0f_cleanIsoTau1Pt%.0f",jetPt,i);
          string name_dijetCol     = Form("l1t_dijetPt%.0fdEta%2.0fmjj%.0f_cleanIsoTau1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,              _1,"l1t_isotau",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                   _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,           _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,   _1,name_tau1Col,name_jetCol,0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
          pAlgo->results->tag_l1tIsoTau  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
        }
      }
    }
    
    // Algo: Double Jet asymmetric pT
    for(double jetPt1=10; jetPt1<=100; jetPt1+=10){
      for(double jetPt2=10; jetPt2<=jetPt1; jetPt2+=10){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0f_IsoTau%.0f",jetPt1,jetPt2,i),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_isotau_pt%.0f",i);
        string name_tau1Col      = Form("l1t_isotau1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanIsoTau1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0f_cleanIsoTau1Pt%.0f",jetPt1,jetPt2,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,               _1,"l1t_isotau",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                    _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,            _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,0,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
        pAlgo->results->tag_l1tIsoTau  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fmjj%.0f_IsoTau%.0f",jetPt1,jetPt2,jetMjj,i),dirNoClean);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_isotau_pt%.0f",i);
          string name_tau1Col      = Form("l1t_isotau1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanIsoTau1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fmjj%.0f_cleanIsoTau1Pt%.0f",jetPt1,jetPt2,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,               _1,"l1t_isotau",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                    _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,            _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,0,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
          pAlgo->results->tag_l1tIsoTau  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
          for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
            
            pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fdeta%2.0fmjj%.0f_IsoTau%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i),dirNoClean);
            pAlgo->setVerbose(m_verbose);
            string name_tauCol       = Form("l1t_isotau_pt%.0f",i);
            string name_tau1Col      = Form("l1t_isotau1_pt%.0f",i);
            string name_jetCol_clean = Form("l1t_jet_cleanIsoTau1Pt%.0f",i);
            string name_dijetCol     = Form("l1t_dijetPt%.0f-%.0fdeta%2.0fmjj%.0f_cleanIsoTau1Pt%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj,i);
            pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,               _1,"l1t_isotau",i,name_tauCol));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                    _1,name_tauCol,1));
            pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,            _1,name_tauCol,name_tau1Col));
            pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,    _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
            pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                    _1,name_jetCol_clean, 2));
            pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt1,jetPt2,jetDEta,jetMjj,name_dijetCol));
            pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
            pAlgo->results->tag_l1tIsoTau  = name_tau1Col;
            pAlgo->results->tag_l1tJetPair = name_dijetCol;
            m_algos.push_back(pAlgo);
          }
        }
      }
    }
    
    
    // Algo: Double Jet Avg Pt
    for(double jetPt=20; jetPt<=60; jetPt+=10){
      
      pAlgo = new L1TAlgo(Form("DijetAvg%.0f_IsoTau%.0f",jetPt,i),dirOverlapClean_LeptonFirst);
      pAlgo->setVerbose(m_verbose);
      string name_tauCol       = Form("l1t_isotau_pt%.0f",i);
      string name_tau1Col      = Form("l1t_isotau1_pt%.0f",i);
      string name_jetCol_clean = Form("l1t_jet_cleanIsoTau1Pt%.0f",i);
      string name_dijetCol     = Form("l1t_dijetAvgPt%.0f_cleanIsoTau1Pt%.0f",jetPt,i);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,            _1,"l1t_isotau",i,name_tauCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                 _1,name_tauCol,1));
      pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,         _1,name_tauCol,name_tau1Col));
      pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
      pAlgo->results->tag_l1tIsoTau  = name_tau1Col;
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("DijetAvg%.0fmjj%.0f_IsoTau%.0f",jetPt,jetMjj,i),dirOverlapClean_LeptonFirst);
        pAlgo->setVerbose(m_verbose);
        string name_tauCol       = Form("l1t_isotau_pt%.0f",i);
        string name_tau1Col      = Form("l1t_isotau1_pt%.0f",i);
        string name_jetCol_clean = Form("l1t_jet_cleanIsoTau1Pt%.0f",i);
        string name_dijetCol     = Form("l1t_dijetAvgPt%.0fmjj%.0f_cleanIsoTau1Pt%.0f",jetPt,jetMjj,i);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,            _1,"l1t_isotau",i,name_tauCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                 _1,name_tauCol,1));
        pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,         _1,name_tauCol,name_tau1Col));
        pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
        pAlgo->results->tag_l1tIsoTau  = name_tau1Col;
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("DijetAvg%.0fdeta%2.0fmjj%.0f_IsoTau%.0f",jetPt,jetDEta*10,jetMjj,i),dirOverlapClean_LeptonFirst);
          pAlgo->setVerbose(m_verbose);
          string name_tauCol       = Form("l1t_isotau_pt%.0f",i);
          string name_tau1Col      = Form("l1t_isotau1_pt%.0f",i);
          string name_jetCol_clean = Form("l1t_jet_cleanIsoTau1Pt%.0f",i);
          string name_dijetCol     = Form("l1t_dijetAvgPt%.0fdeta%2.0fmjj%.0f_cleanIsoTau1Pt%.0f",jetPt,jetDEta*10,jetMjj,i);
          pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,            _1,"l1t_isotau",i,name_tauCol));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                 _1,name_tauCol,1));
          pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,         _1,name_tauCol,name_tau1Col));
          pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>, _1,name_tau1Col,"l1t_jet",0.4,name_jetCol_clean));
          pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                 _1,name_jetCol_clean, 2));
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,name_jetCol_clean,false,jetPt,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
          pAlgo->results->tag_l1tIsoTau  = name_tau1Col;
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
          
        }
      }
    }
  }
  
  // Algo: Double Jet
  for(double jetPt=20; jetPt<=60; jetPt+=10){
    
    pAlgo = new L1TAlgo(Form("Dijet%.0f",jetPt),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    string name_jetCol   = Form("l1t_jet_pt%.0f",jetPt);
    string name_dijetCol = "l1t_dijet_pt%.0f";
    pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
    pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol, 2));
    pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol,false,0,0,name_dijetCol));
    pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
    pAlgo->results->tag_l1tJetPair = name_dijetCol;
    m_algos.push_back(pAlgo);
    
    for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
      
      pAlgo = new L1TAlgo(Form("Dijet%.0fmjj%.0f",jetPt,jetMjj),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      string name_jetCol   = Form("l1t_jet_pt%.0f",jetPt);
      string name_dijetCol = Form("l1t_dijet_pt%.0f_mjj%.0f",jetPt,jetMjj);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol,false,0,jetMjj,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0fdeta%2.0fmjj%.0f",jetPt,jetDEta*10,jetMjj),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_jetCol   = Form("l1t_jet_pt%.0f",jetPt);
        string name_dijetCol = Form("l1t_dijet_pt%.0f_deta%3.1f_mjj%.0f",jetPt,jetDEta*10,jetMjj);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol,false,jetDEta,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
      }
    }
  }
  
  // Algo: Opp Sides Double Jet
  for(double jetPt=20; jetPt<=60; jetPt+=10){
    
    pAlgo = new L1TAlgo(Form("DijetOppSides%.0f",jetPt),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    string name_jetCol   = Form("l1t_jet_pt%.0f",jetPt);
    string name_dijetCol = "l1t_dijet_pt%.0f_oppSide";
    pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
    pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol, 2));
    pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol,true,0,0,name_dijetCol));
    pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
    pAlgo->results->tag_l1tJetPair = name_dijetCol;
    m_algos.push_back(pAlgo);
    
    for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
      
      pAlgo = new L1TAlgo(Form("DijetOppSides%.0fmjj%.0f",jetPt,jetMjj),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      string name_jetCol   = Form("l1t_jet_pt%.0f",jetPt);
      string name_dijetCol = Form("l1t_dijet_pt%.0f_oppSide_mjj%.0f",jetPt,jetMjj);
      pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
      pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol, 2));
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol,true,0,jetMjj,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
        
        pAlgo = new L1TAlgo(Form("DijetOppSides%.0fdeta%2.0fmjj%.0f",jetPt,jetDEta*10,jetMjj),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_jetCol   = Form("l1t_jet_pt%.0f",jetPt);
        string name_dijetCol = Form("l1t_dijet_pt%.0f_oppSide_deta%3.1f_mjj%.0f",jetPt,jetDEta*10,jetMjj);
        pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,              _1,"l1t_jet",jetPt,name_jetCol));
        pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                   _1,name_jetCol, 2));
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeSymmetric<ic::L1TJet>,_1,name_jetCol,true,jetDEta,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                          _1,name_dijetCol,1));
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
      }
    }
  }
  
  // Algo: Double Jet
  for(double jetPt1=10; jetPt1<=100; jetPt1+=10){
    for(double jetPt2=10; jetPt2<=jetPt1; jetPt2+=10){
      
      pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0f",jetPt1,jetPt2),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      string name_dijetCol = Form("l1t_dijet_pt%.0f-%.0f",jetPt1,jetPt2);
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,"l1t_jet",false,jetPt1,jetPt2,0,0,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fmjj%.0f",jetPt1,jetPt2,jetMjj),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_dijetCol = Form("l1t_dijet_pt%.0f-%.0f_mjj%.0f",jetPt1,jetPt2,jetMjj);
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,"l1t_jet",false,jetPt1,jetPt2,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fdeta%2.0fmjj%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj),dirNoClean);
          pAlgo->setVerbose(m_verbose);
          string name_dijetCol = Form("l1t_dijet_pt%.0f-%.0f_deta%3.1f_mjj%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj);
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,"l1t_jet",false,jetPt1,jetPt2,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
          pAlgo->results->tag_l1tJetPair = name_dijetCol;
          m_algos.push_back(pAlgo);
        }
      }
    }
  }
  
  // Algo: Double Jet Avg pT
  for(double jetPt=20; jetPt<=60; jetPt+=10){
    
    pAlgo = new L1TAlgo(Form("DijetAvg%.0f",jetPt),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    string name_dijetCol = Form("l1t_dijet_avgpt%.0f",jetPt);
    pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,"l1t_jet",false,jetPt,0,0,name_dijetCol));
    pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
    pAlgo->results->tag_l1tJetPair = name_dijetCol;
    m_algos.push_back(pAlgo);
    
    for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
      
      pAlgo = new L1TAlgo(Form("DijetAvg%.0fmjj%.0f",jetPt,jetMjj),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      string name_dijetCol = Form("l1t_dijet_avgpt%.0f_mjj%.0f",jetPt,jetMjj);
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,"l1t_jet",false,jetPt,0,jetMjj,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
      pAlgo->results->tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
        
        pAlgo = new L1TAlgo(Form("DijetAvg%.0fdeta%2.0fmjj%.0f",jetPt,jetDEta*10,jetMjj),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_dijetCol = Form("l1t_dijet_avgpt%.0f_deta%3.1f_mjj%.0f",jetPt,jetDEta*10,jetMjj);
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,"l1t_jet",false,jetPt,jetDEta,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
        pAlgo->results->tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
      }
    }
  }
  
  // ########### MET based ###########
  for(double i=10; i<=250; i+=10){
    pAlgo = new L1TAlgo(Form("MET%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TMetEt,_1,i));
    m_algos.push_back(pAlgo);
  }
  
  // MHT based
  for(double i=10; i<=250; i+=10){
    pAlgo = new L1TAlgo(Form("MHT%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TMHTEt,_1,i));
    m_algos.push_back(pAlgo);
  }
  
  
  // ##################################################
  // Processing algos
  // ##################################################
  int nAlgos = m_algos.size();
  m_AlgoPass = new TH1D("AlgoPass","AlgoPass",nAlgos,-0.5,nAlgos-0.5); m_AlgoPass->SetDirectory(m_dir);
  for(unsigned i=0; i<m_algos.size(); i++){
    m_AlgoPass->GetXaxis()->SetBinLabel(i+1,m_algos[i]->getName().c_str());
  }
  
  if(m_verbose){cout << "[L1TAlgoAnalysis::beginJob] Finished creating algos" << endl;}
  
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

void L1TAlgoAnalysis::setDoGenAnalysis(bool value){
  if(m_verbose){printf("[L1TAlgoAnalysis::setDoGenAnalysis] Method called\n");}
  m_doGenAnalysis=value;
}

void L1TAlgoAnalysis::doGenAnalysis(trgfw::Event &iEvent){
  
  if(m_verbose){printf("[L1TAlgoAnalysis::doGenAnalysis] Method called\n");}
  
  if(m_verbose){printf("[L1TAlgoAnalysis::doGenAnalysis] Higgs decay = %u\n",iEvent.genInfo->higgs_decayType);}
  m_plotsGenAnalysis.m_HiggsDecay->Fill(iEvent.genInfo->higgs_decayType);
  
  /*
  //##############################################################################
  if(m_verbose){printf("[L1TAlgoAnalysis::doGenAnalysis] ===== Analyzing Gen Tau #1 =====\n");}
  
  math::XYZTLorentzVector vecTau1Vis(0,0,0,0);
  math::XYZTLorentzVector vecTau1Inv(0,0,0,0);
  
  if()
  
  for(auto it=decayTau1->begin(); it!=decayTau1->end(); it++){
    
    if(m_verbose){printf("[TauAnalysis::analyze] Generator Particle id=%5d st=%d3 et=%5.1f eta=%5.2f phi=%5.2f",it->pdgId(),it->status(),it->et(),it->eta(),it->phi());}
    
    if(fabs(it->pdgId())==12 || fabs(it->pdgId())==14 || fabs(it->pdgId())==16){
      vecTau1Inv += it->p4();
      if(m_verbose){printf(" - Invisible\n");}
      continue;
    }
    vecTau1Vis += it->p4(); 
    if(m_verbose){printf(" - Visible\n");}
  }
  if(m_verbose){cout << endl;}
  
  //##############################################################################
  if(m_verbose){printf("[L1TAlgoAnalysis::doGenAnalysis] ===== Analyzing Gen Tau #2 =====\n");}
  
  math::XYZTLorentzVector vecTau2Vis(0,0,0,0);
  math::XYZTLorentzVector vecTau2Inv(0,0,0,0);
  
  for(auto it=decayTau2->begin(); it!=decayTau2->end(); it++){
    
    if(m_verbose){printf("[TauAnalysis::analyze] Generator Particle id=%5d st=%d3 et=%5.2f eta=%5.2f phi=%5.2f",it->pdgId(),it->status(),it->et(),it->eta(),it->phi());}
    
    if(fabs(it->pdgId())==12 || fabs(it->pdgId())==14 || fabs(it->pdgId())==16){
      vecTau2Inv += it->p4();
      if(m_verbose){printf(" - Invisible\n");}
      continue;
    }
    vecTau2Vis += it->p4();
    if(m_verbose){printf(" - Visible\n");}
  }
  if(m_verbose){cout << endl;}
  
  //##############################################################################
  
  double minDeltaR_Tau1 = 0.5;
  const l1t::Tau* matchL1Tau1 = 0;
  for(auto it=m_L1TauCollection_EtSorted->begin(); it!=m_L1TauCollection_EtSorted->end(); it++){
    
    double dR = deltaR((*it)->eta(),(*it)->phi(),vecTau1Vis.eta(),vecTau1Vis.phi());
    if(dR<minDeltaR_Tau1){
      minDeltaR_Tau1 = dR;
      matchL1Tau1    = (*it);
    }
  }
  
  if(matchL1Tau1){
    m_L1Tau1_ResolutionEt [*decayMode]->Fill(matchL1Tau1->et()  - vecTau1Vis.Et() ); 
    m_L1Tau1_ResolutionEta[*decayMode]->Fill(matchL1Tau1->eta() - vecTau1Vis.eta());
    m_L1Tau1_ResolutionPhi[*decayMode]->Fill(matchL1Tau1->phi() - vecTau1Vis.phi());
  }
  
  double minDeltaR_Tau2 = 0.5;
  const l1t::Tau *matchL1Tau2 = 0;
  for(auto it=m_L1TauCollection_EtSorted->begin(); it!=m_L1TauCollection_EtSorted->end(); it++){
    
    double dR = deltaR((*it)->eta(),(*it)->phi(),vecTau2Vis.eta(),vecTau2Vis.phi());
    if(dR<minDeltaR_Tau2){
      minDeltaR_Tau2 = dR;
      matchL1Tau2    = (*it);
    }
  }
  
  if(matchL1Tau2){
    m_L1Tau2_ResolutionEt [*decayMode]->Fill(matchL1Tau2->et()  - vecTau2Vis.Et() );
    m_L1Tau2_ResolutionEta[*decayMode]->Fill(matchL1Tau2->eta() - vecTau2Vis.eta());
    m_L1Tau2_ResolutionPhi[*decayMode]->Fill(matchL1Tau2->phi() - vecTau2Vis.phi());
  }
  */
  
  
}

void L1TAlgoAnalysis::doSingleObjectsAnalysis(trgfw::Event &iEvent){
  
  vector<ic::L1TEGamma*>* myEGs   = iEvent.getByName< vector<ic::L1TEGamma*> >("l1t_eg");
  vector<ic::L1TMuon*>*   myMuons = iEvent.getByName< vector<ic::L1TMuon*> >  ("l1t_muon");
  vector<ic::L1TTau*>*    myTaus  = iEvent.getByName< vector<ic::L1TTau*> >   ("l1t_tau");
  vector<ic::L1TJet*>*    myJets  = iEvent.getByName< vector<ic::L1TJet*> >   ("l1t_jet");
  vector<ic::L1TSum*>*    mySums  = iEvent.getByName< vector<ic::L1TSum*> >   ("l1t_sum");
  
  if(m_verbose){
    printf("[L1TAlgoAnalysis::doSingleObjectsAnalysis] ===== List of L1T EGamma: %lu =====\n",myEGs->size());
  }
  
  m_plotsSingleObjects.m_L1TEGamma_N->Fill(myEGs->size());
  for(unsigned i=0; i<myEGs->size(); i++){
    
    ic::L1TEGamma* iEGamma = myEGs->at(i);
    
    if(m_verbose){printf("[L1TAlgoAnalysis::doSingleObjectsAnalysis] L1T EGamma #%i - et=%5.1f eta=%5.2f phi=%5.2f\n",i,iEGamma->pt(),iEGamma->eta(),iEGamma->phi());}
    
    m_plotsSingleObjects.m_L1TEGamma_Et ->Fill(iEGamma->pt());
    m_plotsSingleObjects.m_L1TEGamma_Eta->Fill(iEGamma->eta());
    m_plotsSingleObjects.m_L1TEGamma_Phi->Fill(iEGamma->phi());
  }
  
  
  if(m_verbose){
    printf("\n");
    printf("[L1TAlgoAnalysis::doSingleObjectsAnalysis] ===== List of L1T Muon: %lu =====\n",myMuons->size());
  }
  
  m_plotsSingleObjects.m_L1TMuon_N->Fill(myMuons->size());
  for(unsigned i=0; i<myMuons->size(); i++){
    
    ic::L1TMuon* iMuon = myMuons->at(i);
    
    if(m_verbose){printf("[L1TAlgoAnalysis::doSingleObjectsAnalysis] L1T Muon #%i - et=%5.1f eta=%5.2f phi=%5.2f\n",i,iMuon->pt(),iMuon->eta(),iMuon->phi());}
    
    m_plotsSingleObjects.m_L1TMuon_Et ->Fill(iMuon->pt());
    m_plotsSingleObjects.m_L1TMuon_Eta->Fill(iMuon->eta());
    m_plotsSingleObjects.m_L1TMuon_Phi->Fill(iMuon->phi());
  }
  
  if(m_verbose){
    printf("\n");
    printf("[L1TAlgoAnalysis::doSingleObjectsAnalysis] ===== List of L1T Tau: %lu =====\n",myTaus->size());
  }
  
  m_plotsSingleObjects.m_L1TTau_N->Fill(myTaus->size());
  for(unsigned i=0; i<myTaus->size(); i++){
    
    ic::L1TTau* iTau = myTaus->at(i);
    
    if(m_verbose){printf("[L1TAlgoAnalysis::doSingleObjectsAnalysis] L1T Tau #%i - et=%5.1f eta=%5.2f phi=%5.2f\n",i,iTau->pt(),iTau->eta(),iTau->phi());}
    
    m_plotsSingleObjects.m_L1TTau_Et ->Fill(iTau->pt());
    m_plotsSingleObjects.m_L1TTau_Eta->Fill(iTau->eta());
    m_plotsSingleObjects.m_L1TTau_Phi->Fill(iTau->phi());
  }
  
  if(m_verbose){
    printf("\n");
    printf("[L1TAlgoAnalysis::doSingleObjectsAnalysis] ===== List of L1T Jet: %lu =====\n",myJets->size());
  }
  
  m_plotsSingleObjects.m_L1TJet_N->Fill(myJets->size());
  for(unsigned i=0; i<myJets->size(); i++){
    
    ic::L1TJet* iJet = myJets->at(i);
    
    if(m_verbose){printf("[L1TAlgoAnalysis::doSingleObjectsAnalysis] L1T Jet #%i - et=%5.1f eta=%5.2f phi=%5.2f\n",i,iJet->pt(),iJet->eta(),iJet->phi());}
    
    m_plotsSingleObjects.m_L1TJet_Et ->Fill(iJet->pt());
    m_plotsSingleObjects.m_L1TJet_Eta->Fill(iJet->eta());
    m_plotsSingleObjects.m_L1TJet_Phi->Fill(iJet->phi());
  }
  if(m_verbose){printf("\n");}
  
  if(m_verbose){
    printf("\n");
    printf("[L1TAlgoAnalysis::doSingleObjectsAnalysis] ===== List of L1T Sum: %lu =====\n",mySums->size());
  }
  
  for(unsigned i=0; i<mySums->size(); i++){
    
    ic::L1TSum *iSum = mySums->at(i);
    if(m_verbose){printf("[L1TAlgoAnalysis::doSingleObjectsAnalysis] L1T Sum #%i - type=%1u et=%5.1f phi=%5.2f\n",i,iSum->sumType,iSum->vector().Et(),iSum->vector().Phi());}
    
    if(iSum->sumType==ic::L1TSum::SumType::kMissingEt){
      m_plotsSingleObjects.m_L1TMet_Et ->Fill(iSum->vector().Et());
      m_plotsSingleObjects.m_L1TMet_Phi->Fill(iSum->vector().Phi());
    }
    else if(iSum->sumType==ic::L1TSum::SumType::kMissingHt){
      m_plotsSingleObjects.m_L1TMHT_Et ->Fill(iSum->vector().Et());
      m_plotsSingleObjects.m_L1TMHT_Phi->Fill(iSum->vector().Phi());
    }
  }
  if(m_verbose){printf("\n");}
}

void L1TAlgoAnalysis::incrementEventTotal(){
  m_EventCount->Fill(1);
}

void L1TAlgoAnalysis::run(trgfw::Event &iEvent){
  if(m_verbose){printf("[L1TAlgoAnalysis::processEvent] Method called\n");}
  
  
  // Do single object analysis
  if(m_doGenAnalysis)          {doGenAnalysis          (iEvent);}
  if(m_doSingleObjectsAnalysis){doSingleObjectsAnalysis(iEvent);}
  
  // Testing all set algos
  for(unsigned i=0; i<m_algos.size(); i++){
    if(m_algos[i]->run(iEvent)){m_AlgoPass->Fill(i);}
  }
  
  if(m_verbose){iEvent.printCollections();}
  
}

void L1TAlgoAnalysis::endJob(){
  
  if(m_verbose){printf("[L1TAlgoAnalysis::endJob] Method called\n");}
  
  TDirectory* dirScan = m_dir->mkdir("Scan");
  
  TTree *myTree = new TTree("L1TAlgoResults","L1TAlgoResults");
  myTree->SetDirectory(dirScan);
  
  myTree->Branch("L1TEGamma1_Et"      ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TEGamma1_Eta"     ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TEGamma1_Phi"     ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TIsoEGamma1_Et"   ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TIsoEGamma1_Eta"  ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TIsoEGamma1_Phi"  ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TMuon1_Et"        ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TMuon1_Eta"       ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TMuon1_Phi"       ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TIsoMuon1_Et"     ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TIsoMuon1_Eta"    ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TIsoMuon1_Phi"    ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TTau_N"           ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TTau1_Et"         ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TTau1_Eta"        ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TTau1_Phi"        ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TTau2_Et"         ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TTau2_Eta"        ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TTau2_Phi"        ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TIsoTau_N"        ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TIsoTau1_Et"      ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TIsoTau1_Eta"     ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TIsoTau1_Phi"     ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TIsoTau2_Et"      ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TIsoTau2_Eta"     ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TIsoTau2_Phi"     ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TJet_N"           ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TJet_AvgPt"       ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TJet_VecPt"       ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TJet1_Et"         ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TJet1_Eta"        ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TJet1_Phi"        ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TJet2_Et"         ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TJet2_Eta"        ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TJet2_Phi"        ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TJet_maxMjj"      ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TJet_maxDEta"     ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TTau1_minDRL1TJet","trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  myTree->Branch("L1TMHT_Et"          ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  myTree->Branch("L1TMET_Et"          ,"trgfw::L1TVariableScanDataFormat",0,32000,3);
  
  
  for(unsigned i=0; i<m_algos.size(); i++){
    m_algos[i]->results->updateAdress(myTree);
    myTree->Fill();
  }
}
