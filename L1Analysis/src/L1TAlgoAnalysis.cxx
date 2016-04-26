#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoAnalysis.h"

#include "CMSSW_VBFHiggsTauTau/L1Analysis/src/L1TFunctions.cxx"

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
  
  if(m_verbose){printf("[L1TAlgoAnalysis::~L1TAlgoAnalysis] Method called\n");}
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
  
  using namespace std::placeholders; 
  
  // ===== Histograms =====
  m_EventCount = new TH1D("EventCount","EventCount",1, 0.5,1.5); m_EventCount->SetDirectory(m_dir);
  
  if(m_doGenAnalysis){
    TDirectory* dirGenAnalysis = m_dir->mkdir("GenAnalysis");
    m_plotsGenAnalysis.create(dirGenAnalysis);
  }
  
  if(m_doSingleObjectsAnalysis){
    TDirectory* dirSingleObjects = m_dir->mkdir("SingleObject");
    m_plotsSingleObjects.create(dirSingleObjects);
  }
  
  TDirectory* dirAlgos = m_dir->mkdir("Algos");

  L1TAlgo *pAlgo;
  pAlgo = new L1TAlgo("NoCuts",dirAlgos);
  pAlgo->setVerbose(m_verbose);
  m_algos.push_back(pAlgo);
  
  TDirectory* dirNoClean = dirAlgos->mkdir("NoClean");
  
  // #######################################
  // ########### Single Objects ############
  // #######################################
  /*
  // Algo: EG
  for(double i=10; i<=40; i+=5){
    pAlgo = new L1TAlgo(Form("EG%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TEGamma1Pt,_1,i));
    m_algos.push_back(pAlgo);
  }
  
  // Algo: Muon
  for(double i=10; i<=40; i+=5){
    pAlgo = new L1TAlgo(Form("Mu%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TMuon1Pt,_1,i));
    m_algos.push_back(pAlgo);
  }
  
  // Algo: Tau
  for(double i=10; i<=40; i+=5){
    pAlgo = new L1TAlgo(Form("Tau%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TTau1Pt,_1,i));
    m_algos.push_back(pAlgo);
  }
  
  // Algo: IsoTau
  for(double i=10; i<=40; i+=5){
    pAlgo = new L1TAlgo(Form("IsoTau%.0f",i),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau1Pt,_1,i));
    m_algos.push_back(pAlgo);
  }
  
  // #######################################
  // ########### Double Objects ############
  // #######################################
  
  // Algo: Double Jet
  for(double jetPt=20; jetPt<=60; jetPt+=10){
    
    pAlgo = new L1TAlgo(Form("Dijet%.0f",jetPt),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt,_1,jetPt,jetPt));
    m_algos.push_back(pAlgo);
    
    for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
      
      pAlgo = new L1TAlgo(Form("Dijet%.0fmjj%.0f",jetPt,jetMjj),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_Mjj,_1,jetPt,jetMjj,0));
      m_algos.push_back(pAlgo);
      
      for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
      
        pAlgo = new L1TAlgo(Form("Dijet%.0fdeta%0.fmjj%.0f",jetPt,jetDEta,jetMjj),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,jetPt,jetDEta,jetMjj,0,0));
        m_algos.push_back(pAlgo);
      }
    }
  }

  // Algo: EG + Muon
  for(double eg=10; eg<=30; eg+=5){
    for(double mu=10; mu<=30; mu+=5){
      pAlgo = new L1TAlgo(Form("EG%.0f_Mu%.0f",eg,mu),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      pAlgo->addCondition(std::bind(trgfw::testL1TEGamma1Pt ,_1,eg));
      pAlgo->addCondition(std::bind(trgfw::testL1TMuon1Pt,   _1,mu));
      m_algos.push_back(pAlgo);
    }
  }
  
  // Algo: EG + Tau
  for(double eg=10; eg<=30; eg+=5){
    for(double ta=10; ta<=30; ta+=5){
      pAlgo = new L1TAlgo(Form("EG%.0f_Tau%.0f",eg,ta),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      pAlgo->addCondition(std::bind(trgfw::testL1TEGamma1Pt ,_1,eg));
      pAlgo->addCondition(std::bind(trgfw::testL1TTau1Pt,    _1,ta));
      m_algos.push_back(pAlgo);
    }
  }
  
  // Algo: EG + IsoTau
  for(double eg=10; eg<=30; eg+=5){
    for(double ta=10; ta<=30; ta+=5){
      pAlgo = new L1TAlgo(Form("EG%.0f_IsoTau%.0f",eg,ta),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      pAlgo->addCondition(std::bind(trgfw::testL1TEGamma1Pt ,_1,eg));
      pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau1Pt, _1,ta));
      m_algos.push_back(pAlgo);
    }
  }
  
  // Algo: Muon + Tau
  for(double mu=10; mu<=30; mu+=5){
    for(double ta=10; ta<=30; ta+=5){
      pAlgo = new L1TAlgo(Form("Mu%.0f_Tau%.0f",mu,ta),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      pAlgo->addCondition(std::bind(trgfw::testL1TMuon1Pt ,_1,mu));
      pAlgo->addCondition(std::bind(trgfw::testL1TTau1Pt,  _1,ta));
      m_algos.push_back(pAlgo);
    }
  }
  
  // Algo: Muon + IsoTau
  for(double mu=10; mu<=30; mu+=5){
    for(double ta=10; ta<=30; ta+=5){
      pAlgo = new L1TAlgo(Form("Mu%.0f_IsoTau%.0f",mu,ta),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      pAlgo->addCondition(std::bind(trgfw::testL1TMuon1Pt,  _1,mu));
      pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau1Pt,_1,ta));
      m_algos.push_back(pAlgo);
    }
  }
  
  // Algo: Double Tau
  for(double ta=10; ta<=50; ta+=5){
    pAlgo = new L1TAlgo(Form("DiTau%.0f",ta),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TTau2Pt,_1,ta,ta));
    m_algos.push_back(pAlgo);
  }
  
  // Algo: Double IsoTau
  for(double ta=10; ta<=50; ta+=5){
    pAlgo = new L1TAlgo(Form("DiIsoTau%.0f",ta),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TTau2Pt,_1,ta,ta));
    m_algos.push_back(pAlgo);
  }
  
  // #######################################
  // ###### Dijet + Single Objects #########
  // #######################################

  // Algo: Dijet + EG
  pAlgo = new L1TAlgo("Dijet30_EG20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt   ,_1,30.,30.));
  pAlgo->addCondition(std::bind(trgfw::testL1TEGamma1Pt,_1,20.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet30mjj300_EG20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_Mjj,_1,30.,300.));
  pAlgo->addCondition(std::bind(trgfw::testL1TEGamma1Pt ,_1,20.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet30deta3p0mjj300_EG20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0,300.));
  pAlgo->addCondition(std::bind(trgfw::testL1TEGamma1Pt ,     _1,20.));
  m_algos.push_back(pAlgo);
  
  // Algo: Dijet + Muon
  pAlgo = new L1TAlgo("Dijet30_Mu20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt ,_1,30.,30.));
  pAlgo->addCondition(std::bind(trgfw::testL1TMuon1Pt,_1,20.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet30mjj300_Mu20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_Mjj,_1,30.,300.));
  pAlgo->addCondition(std::bind(trgfw::testL1TMuon1Pt,   _1,20.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet30deta3p0mjj300_Mu20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0,300.));
  pAlgo->addCondition(std::bind(trgfw::testL1TMuon1Pt ,       _1,20.));
  m_algos.push_back(pAlgo);
  
  // Algo: Dijet + Tau
  pAlgo = new L1TAlgo("Dijet30_Tau20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt ,_1,30.,30.));
  pAlgo->addCondition(std::bind(trgfw::testL1TTau1Pt, _1,20.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet30mjj300_Tau20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_Mjj,_1,30.,300.));
  pAlgo->addCondition(std::bind(trgfw::testL1TTau1Pt,    _1,20.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet30deta3p0mjj300_Tau20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0,300.));
  pAlgo->addCondition(std::bind(trgfw::testL1TTau1Pt ,        _1,20.));
  m_algos.push_back(pAlgo);
  
  // Algo: Dijet + IsoTau
  pAlgo = new L1TAlgo("Dijet30_IsoTau20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt,   _1,30.,30.));
  pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau1Pt,_1,20.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet30mjj300_IsoTau20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_Mjj,_1,30.,300.));
  pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau1Pt, _1,20.));
  m_algos.push_back(pAlgo);
  
  pAlgo = new L1TAlgo("Dijet30deta3p0mjj300_IsoTau20",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0,300.));
  pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau1Pt,      _1,20.));
  m_algos.push_back(pAlgo);
  
  // #######################################
  // ###### Dijet + Double Objects #########
  // #######################################
  
  // Dijet Dijet + Electron + Muon 
  for(double elePt=10; elePt<=20; elePt+=10){
    for(double muoPt=10; muoPt<=20; muoPt+=10){
    for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
      pAlgo = new L1TAlgo(Form("Dijet30deta3p0mjj%.0f_EG%.0f_Mu%.0f",jetMjj,elePt,muoPt),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0,jetMjj));
        pAlgo->addCondition(std::bind(trgfw::testL1TEGamma1Pt,      _1,elePt));
        pAlgo->addCondition(std::bind(trgfw::testL1TMuon1Pt,        _1,muoPt));
        m_algos.push_back(pAlgo);
      }
    }
  }
  
  
  // Algo: Dijet + Electron + Tau 
  for(double elePt=10; elePt<=20; elePt+=10){
    for(double tauPt=10; tauPt<=20; tauPt+=10){
      for(double mjj=200; mjj<=400; mjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet30deta3p0mjj%.0f_EG%.0f_Tau%.0f",mjj,elePt,tauPt),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0,mjj));
        pAlgo->addCondition(std::bind(trgfw::testL1TEGamma1Pt,      _1,elePt));
        pAlgo->addCondition(std::bind(trgfw::testL1TTau1Pt,         _1,tauPt));
        m_algos.push_back(pAlgo);
      }
    }
  }
  
  // Algo: Dijet + Electron + IsoTau 
  for(double elePt=10; elePt<=20; elePt+=10){
    for(double tauPt=10; tauPt<=20; tauPt+=10){
      for(double mjj=200; mjj<=400; mjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet30deta3p0mjj%.0f_EG%.0f_IsoTau%.0f",mjj,elePt,tauPt),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0,mjj));
        pAlgo->addCondition(std::bind(trgfw::testL1TEGamma1Pt,      _1,elePt));
        pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau1Pt,      _1,tauPt));
        m_algos.push_back(pAlgo);
      }
    }
  }
  
  // Algo: Dijet + Muon + Tau
  for(double muoPt=10; muoPt<=20; muoPt+=10){
    for(double tauPt=10; tauPt<=20; tauPt+=10){
      for(double mjj=200; mjj<=400; mjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet30deta3p0mjj%.0f_Mu%.0f_Tau%.0f",mjj,muoPt,tauPt),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0,mjj));
        pAlgo->addCondition(std::bind(trgfw::testL1TMuon1Pt,        _1,muoPt));
        pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau1Pt,      _1,tauPt));
        m_algos.push_back(pAlgo);
      }
    }
  }
  
  // Algo: Dijet - Muon + IsoTau
  for(double muoPt=10; muoPt<=20; muoPt+=10){
    for(double tauPt=10; tauPt<=20; tauPt+=10){
      for(double mjj=200; mjj<=400; mjj+=100){
        pAlgo = new L1TAlgo(Form("Dijet30deta3p0mjj%.0f_Mu%.0f_IsoTau%.0f",mjj,muoPt,tauPt),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0,mjj));
        pAlgo->addCondition(std::bind(trgfw::testL1TMuon1Pt,        _1,muoPt));
        pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau1Pt,      _1,tauPt));
        m_algos.push_back(pAlgo);
      }
    }
  }
  
  // Algo: Dijet - DiTau
  for(double lep=10; lep<=20; lep+=10){
    
    pAlgo = new L1TAlgo(Form("Dijet30_DiTau%.0f",lep),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt,_1,30.,30.));
    pAlgo->addCondition(std::bind(trgfw::testL1TTau2Pt,_1,lep,lep));
    m_algos.push_back(pAlgo);
    
    for(double mjj=200; mjj<=400; mjj+=100){
      
      pAlgo = new L1TAlgo(Form("Dijet30mjj%.0f_DiTau%.0f",mjj,lep),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_Mjj,_1,30.,mjj));
      pAlgo->addCondition(std::bind(trgfw::testL1TTau2Pt,    _1,lep,lep));
      m_algos.push_back(pAlgo);
      
      pAlgo = new L1TAlgo(Form("Dijet30deta3p0mjj%.0f_DiTau%.0f",mjj,lep),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0,mjj));
      pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau2Pt,      _1,lep,lep));
      m_algos.push_back(pAlgo);
    }
  }

  // Algo: Dijet - DiIsoTau
  for(double lep=10; lep<=20; lep+=10){

    pAlgo = new L1TAlgo(Form("Dijet30_DiIsoTau%.0f",lep),dirNoClean);
    pAlgo->setVerbose(m_verbose);
    pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt,   _1,30.,30.));
    pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau2Pt,_1,lep,lep));
    m_algos.push_back(pAlgo);

    for(double mjj=200; mjj<=400; mjj+=100){

      pAlgo = new L1TAlgo(Form("Dijet30mjj%.0f_DiIsoTau%.0f",mjj,lep),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_Mjj,_1,30.,mjj));
      pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau2Pt,      _1,lep,lep));
      m_algos.push_back(pAlgo);
      
      pAlgo = new L1TAlgo(Form("Dijet30deta3p0mjj%.0f_DiIsoTau%.0f",mjj,lep),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      pAlgo->addCondition(std::bind(trgfw::testL1TJet2Pt_DEta_Mjj,_1,30.,3.0));
      pAlgo->addCondition(std::bind(trgfw::testL1TIsoTau2Pt,      _1,lep,lep));
      m_algos.push_back(pAlgo);
    }
  }
  
  */
  
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
    pAlgo->plots.tag_l1tJetPair = name_dijetCol;
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
      pAlgo->plots.tag_l1tJetPair = name_dijetCol;
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
        pAlgo->plots.tag_l1tJetPair = name_dijetCol;
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
    pAlgo->plots.tag_l1tJetPair = name_dijetCol;
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
      pAlgo->plots.tag_l1tJetPair = name_dijetCol;
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
        pAlgo->plots.tag_l1tJetPair = name_dijetCol;
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
      pAlgo->plots.tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
        
        pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fmjj%.0f",jetPt1,jetPt2,jetMjj),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_dijetCol = Form("l1t_dijet_pt%.0f-%.0f_mjj%.0f",jetPt1,jetPt2,jetMjj);
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,"l1t_jet",false,jetPt1,jetPt2,0,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
        pAlgo->plots.tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
        
        for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
          
          pAlgo = new L1TAlgo(Form("Dijet%.0f-%.0fdeta%2.0fmjj%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj),dirNoClean);
          pAlgo->setVerbose(m_verbose);
          string name_dijetCol = Form("l1t_dijet_pt%.0f-%.0f_deta%3.1f_mjj%.0f",jetPt1,jetPt2,jetDEta*10,jetMjj);
          pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAsymmetric<ic::L1TJet>,_1,"l1t_jet",false,jetPt1,jetPt2,jetDEta,jetMjj,name_dijetCol));
          pAlgo->addCondition(std::bind(trgfw::pairTest_size,                           _1,name_dijetCol,1));
          pAlgo->plots.tag_l1tJetPair = name_dijetCol;
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
    pAlgo->plots.tag_l1tJetPair = name_dijetCol;
    m_algos.push_back(pAlgo);
    
    for(double jetMjj=200; jetMjj<=400; jetMjj+=100){
      
      pAlgo = new L1TAlgo(Form("DijetAvg%.0fmjj%.0f",jetPt,jetMjj),dirNoClean);
      pAlgo->setVerbose(m_verbose);
      string name_dijetCol = Form("l1t_dijet_avgpt%.0f_mjj%.0f",jetPt,jetMjj);
      pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,"l1t_jet",false,jetPt,0,jetMjj,name_dijetCol));
      pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
      pAlgo->plots.tag_l1tJetPair = name_dijetCol;
      m_algos.push_back(pAlgo);
      
      for(double jetDEta=2.5; jetDEta<=3.5; jetDEta+=0.5){
        
        pAlgo = new L1TAlgo(Form("DijetAvg%.0fdeta%2.0fmjj%.0f",jetPt,jetDEta*10,jetMjj),dirNoClean);
        pAlgo->setVerbose(m_verbose);
        string name_dijetCol = Form("l1t_dijet_avgpt%.0f_deta%3.1f_mjj%.0f",jetPt,jetDEta*10,jetMjj);
        pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,"l1t_jet",false,jetPt,jetDEta,jetMjj,name_dijetCol));
        pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
        pAlgo->plots.tag_l1tJetPair = name_dijetCol;
        m_algos.push_back(pAlgo);
      }
    }
  }
  
  pAlgo = new L1TAlgo("DijetAvg30deta25",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  string name_dijetCol = "l1t_dijet_avgpt30_deta25";
  pAlgo->addCondition(std::bind(trgfw::pairFilter_vbfLikeAverage<ic::L1TJet>,_1,"l1t_jet",false,30,2.5,0,name_dijetCol));
  pAlgo->addCondition(std::bind(trgfw::pairTest_size,                        _1,name_dijetCol,1));
  pAlgo->plots.tag_l1tJetPair = name_dijetCol;
  m_algos.push_back(pAlgo);
  
  
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
  
  pAlgo = new L1TAlgo("Dijet30_Tau30",dirNoClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,_1,"l1t_tau",30,"l1t_tau_pt30"));
  pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,     _1,"l1t_tau_pt30",1));
  pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,_1,"l1t_jet",30,"l1t_jet_pt30"));
  pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,     _1,"l1t_jet_pt30", 2));
  pAlgo->plots.tag_l1tTau = "l1t_tau_pt30";
  pAlgo->plots.tag_l1tJet = "l1t_jet_pt30";
  m_algos.push_back(pAlgo);
  
  TDirectory* dirOverlapClean = dirAlgos->mkdir("OverlapCleaned");
  
  // ########### New algo scheme ###########
  pAlgo = new L1TAlgo("Dijet30_Tau30",dirOverlapClean);
  pAlgo->setVerbose(m_verbose);
  pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TTau>,           _1,"l1t_tau",30,"l1t_tau_pt30"));
  pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TTau>,                            _1,"l1t_tau_pt30",1));
  pAlgo->addCondition(std::bind(trgfw::filterByLeading1<ic::L1TTau>,        _1,"l1t_tau_pt30","l1t_tau_pt30_leading1"));
  pAlgo->addCondition(std::bind(trgfw::filterByMinDR<ic::L1TTau,ic::L1TJet>,_1,"l1t_tau_pt30_leading1","l1t_jet",0.4,"l1t_jet_cleanTauPt30Leading1"));
  pAlgo->addCondition(std::bind(trgfw::filterByMinPt<ic::L1TJet>,           _1,"l1t_jet_cleanTauPt30Leading1",30,"l1t_jet_cleanTauPt30Leading1_pt30"));
  pAlgo->addCondition(std::bind(trgfw::testSize<ic::L1TJet>,                _1,"l1t_jet_cleanTauPt30Leading1_pt30", 2));
  pAlgo->plots.tag_l1tTau = "l1t_tau_pt30";
  pAlgo->plots.tag_l1tJet = "l1t_jet_cleanTauPt30Leading1_pt30";
  m_algos.push_back(pAlgo);
  
  int nAlgos = m_algos.size();
  m_AlgoPass = new TH1D("AlgoPass","AlgoPass",nAlgos,-0.5,nAlgos-0.5); m_AlgoPass->SetDirectory(m_dir);
  for(unsigned i=0; i<m_algos.size(); i++){
    m_AlgoPass->GetXaxis()->SetBinLabel(i+1,m_algos[i]->getName().c_str());
  }
  
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

void L1TAlgoAnalysis::run(trgfw::Event &iEvent){
  if(m_verbose){printf("[L1TAlgoAnalysis::processEvent] Method called\n");}
  
  m_EventCount->Fill(1);
  
  // Do single object analysis
  if(m_doGenAnalysis)          {doGenAnalysis          (iEvent);}
  if(m_doSingleObjectsAnalysis){doSingleObjectsAnalysis(iEvent);}
  
  // Testing all set algos
  for(unsigned i=0; i<m_algos.size(); i++){
    if(m_algos[i]->run(iEvent)){m_AlgoPass->Fill(i);}
  }
  
  if(m_verbose){iEvent.printCollections();}
  
}

// Stuff
// TFile *fOut         = new TFile(options.outputFilename.c_str(),"RECREATE");
// TH1D  *m_EventCount = new TH1D("EventCount","EventCount",1, 0.5,1.5); m_EventCount->SetDirectory(fOut);
// TH1D  *m_HiggsDecay = new TH1D("HiggsDecay","HiggsDecay",6, 0.5,6.5); m_HiggsDecay->SetDirectory(fOut);
// m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::EleEle,"EleEle");
// m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::EleMuo,"EleMuo");
// m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::EleHad,"EleHad");
// m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::MuoMuo,"MuoMuo");
// m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::MuoHad,"MuoHad");
// m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::HadHad,"HadHad");



// string strHiggsDecay="";
// if     (genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::EleEle){strHiggsDecay="EleEle";}
// else if(genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::EleMuo){strHiggsDecay="EleMuo";}
// else if(genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::EleHad){strHiggsDecay="EleHad";}
// else if(genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::MuoMuo){strHiggsDecay="MuoMuo";}
// else if(genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::MuoHad){strHiggsDecay="MuoHad";}
// else if(genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::HadHad){strHiggsDecay="HadHad";}
// 
// m_HiggsDecay->Fill(genInfo->higgs_decayType);
// 
// if(options.verbose){
//   printf("Higgs decay type: %s (%u)\n",strHiggsDecay.c_str(),genInfo->higgs_decayType);
//   printf("Tau decay 1     : %u\n", genInfo->tau1_decayType);
//   printf("Tau decay 1 size: %lu\n",genInfo->tau1_stableDecayProducts.size());
//   printf("Tau decay 2     : %u\n", genInfo->tau2_decayType);
//   printf("Tau decay 2 size: %lu\n",genInfo->tau2_stableDecayProducts.size());
// }

//   fOut->Write();
