#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoResults.h"

#include "DataFormats/Math/interface/deltaR.h"

// L1T Objects
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TObject.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TEGamma.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TMuon.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TTau.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TJet.hh"

#include "TMath.h"

using namespace std;

trgfw::L1TAlgoResults::L1TAlgoResults(string path,string name) :
  m_verbose(false){
  
  if(m_verbose){printf("[trgfw::L1TAlgoResults::L1TAlgoResults] Method called. (name=%s)\n",name.c_str());}
  
  m_verbose = false;
  m_path    = path;
  m_name    = name;
  
  tag_l1tEG      = "l1t_eg";
  tag_l1tIsoEG   = "l1t_isoeg";
  tag_l1tMuon    = "l1t_muon";
  tag_l1tIsoMuon = "l1t_isomuon";
  tag_l1tTau     = "l1t_tau";
  tag_l1tIsoTau  = "l1t_isotau";
  tag_l1tJet     = "l1t_jet";
  tag_l1tSum     = "l1t_sum";
  
  tag_l1tJetPair = "";
  
  m_L1TEGamma1_Et       = new trgfw::L1TVariableScanDataFormat(m_path,"L1TEGamma1_Pt",      125,           0,        250);
  m_L1TEGamma1_Eta      = new trgfw::L1TVariableScanDataFormat(m_path,"L1TEGamma1_Eta",     100,          -5,          5);
  m_L1TEGamma1_Phi      = new trgfw::L1TVariableScanDataFormat(m_path,"L1TEGamma1_Phi",     100,-TMath::Pi(),TMath::Pi());
  
  m_L1TIsoEGamma1_Et    = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoEG1_Pt",       125,           0,        250);
  m_L1TIsoEGamma1_Eta   = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoEG1_Eta",      100,          -5,          5);
  m_L1TIsoEGamma1_Phi   = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoEG1_Phi",      100,-TMath::Pi(),TMath::Pi());
  
  m_L1TMuon1_Et         = new trgfw::L1TVariableScanDataFormat(m_path,"L1TMuon1_Pt",        125,           0,        250);
  m_L1TMuon1_Eta        = new trgfw::L1TVariableScanDataFormat(m_path,"L1TMuon1_Eta",       100,          -5,          5);
  m_L1TMuon1_Phi        = new trgfw::L1TVariableScanDataFormat(m_path,"L1TMuon1_Phi",       100,-TMath::Pi(),TMath::Pi());
  
  m_L1TIsoMuon1_Et      = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoMuon1_Pt",     125,           0,        250);
  m_L1TIsoMuon1_Eta     = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoMuon1_Eta",    100,          -5,          5);
  m_L1TIsoMuon1_Phi     = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoMuon1_Phi",    100,-TMath::Pi(),TMath::Pi());
  
  m_L1TTau_N            = new trgfw::L1TVariableScanDataFormat(m_path,"L1TTau_N",            21,        -0.5,       20.5);
  m_L1TTau1_Et          = new trgfw::L1TVariableScanDataFormat(m_path,"L1TTau1_Pt",         125,           0,        250);
  m_L1TTau1_Eta         = new trgfw::L1TVariableScanDataFormat(m_path,"L1TTau1_Eta",        100,          -5,          5);
  m_L1TTau1_Phi         = new trgfw::L1TVariableScanDataFormat(m_path,"L1TTau1_Phi",        100,-TMath::Pi(),TMath::Pi());
  
  m_L1TTau2_Et          = new trgfw::L1TVariableScanDataFormat(m_path,"L1TTau2_Pt",         125,           0,        250);
  m_L1TTau2_Eta         = new trgfw::L1TVariableScanDataFormat(m_path,"L1TTau2_Eta",        100,          -5,          5);
  m_L1TTau2_Phi         = new trgfw::L1TVariableScanDataFormat(m_path,"L1TTau2_Phi",        100,-TMath::Pi(),TMath::Pi());
  
  m_L1TIsoTau_N         = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoTau_N",         21,        -0.5,       20.5);
  m_L1TIsoTau1_Et       = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoTau1_Pt",      125,           0,        250);
  m_L1TIsoTau1_Eta      = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoTau1_Eta",     100,          -5,          5);
  m_L1TIsoTau1_Phi      = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoTau1_Phi",     100,-TMath::Pi(),TMath::Pi());
  
  m_L1TIsoTau2_Et       = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoTau2_Pt",      125,           0,        250);
  m_L1TIsoTau2_Eta      = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoTau2_Eta",     100,          -5,          5);
  m_L1TIsoTau2_Phi      = new trgfw::L1TVariableScanDataFormat(m_path,"L1TIsoTau2_Phi",     100,-TMath::Pi(),TMath::Pi());
  
  m_L1TJet_N            = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet_N",            21,        -0.5,       20.5);
  m_L1TJet_AvgPt        = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet_Avg",         125,           0,        250);
  m_L1TJet_VecPt        = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet_VecPt",       125,           0,        250);
  m_L1TJet1_Et          = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet1_Pt",         125,           0,        250);
  m_L1TJet1_Eta         = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet1_Eta",        100,          -5,          5);
  m_L1TJet1_Phi         = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet1_Phi",        100,-TMath::Pi(),TMath::Pi());
  
  m_L1TJet2_Et          = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet2_Pt",         125,           0,        250);
  m_L1TJet2_Eta         = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet2_Eta",        100,          -5,          5);
  m_L1TJet2_Phi         = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet2_Phi",        100,-TMath::Pi(),TMath::Pi());
  
  m_L1TJet_maxMjj       = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet_maxMjj",      100,           0,       1000);
  m_L1TJet_maxDEta      = new trgfw::L1TVariableScanDataFormat(m_path,"L1TJet_maxDEta",     150,           0,         15);
  
  m_L1TTau1_minDRL1TJet = new trgfw::L1TVariableScanDataFormat(m_path,"L1TTau1_minDRL1TJet",150,           0,         15);
  
  m_L1TMHT_Et           = new trgfw::L1TVariableScanDataFormat(m_path,"L1TMHT_Et",          256,           0,        512);
  m_L1TMET_Et           = new trgfw::L1TVariableScanDataFormat(m_path,"L1TMET_Et",          256,           0,        512);
  
  //m_METvsMjj  = new TH2D("METvsMjj",      "MET vs Mjj",   100,0,200, 100,0,1000);       m_METvsMjj     ->SetDirectory(m_plotsDirectory);
}

trgfw::L1TAlgoResults::~L1TAlgoResults(){
  delete m_L1TEGamma1_Et;
  delete m_L1TEGamma1_Eta;
  delete m_L1TEGamma1_Phi;
  
  delete m_L1TIsoEGamma1_Et;
  delete m_L1TIsoEGamma1_Eta;
  delete m_L1TIsoEGamma1_Phi;
  
  delete m_L1TMuon1_Et;
  delete m_L1TMuon1_Eta;
  delete m_L1TMuon1_Phi;
  
  delete m_L1TIsoMuon1_Et;
  delete m_L1TIsoMuon1_Eta;
  delete m_L1TIsoMuon1_Phi;
  
  delete m_L1TTau_N;
  delete m_L1TTau1_Et; 
  delete m_L1TTau1_Eta;
  delete m_L1TTau1_Phi;
  
  delete m_L1TTau2_Et;
  delete m_L1TTau2_Eta;
  delete m_L1TTau2_Phi;
  
  delete m_L1TIsoTau_N;
  delete m_L1TIsoTau1_Et; 
  delete m_L1TIsoTau1_Eta;
  delete m_L1TIsoTau1_Phi;
  
  delete m_L1TIsoTau2_Et;
  delete m_L1TIsoTau2_Eta;
  delete m_L1TIsoTau2_Phi;
  
  delete m_L1TJet_N;
  
  delete m_L1TJet_AvgPt;
  delete m_L1TJet_VecPt;
  delete m_L1TJet1_Et; 
  delete m_L1TJet1_Eta;
  delete m_L1TJet1_Phi;
  
  delete m_L1TJet2_Et;
  delete m_L1TJet2_Eta;
  delete m_L1TJet2_Phi;
  
  delete m_L1TJet_maxMjj;
  delete m_L1TJet_maxDEta;
  
  delete m_L1TTau1_minDRL1TJet;
  
  delete m_L1TMET_Et;
  delete m_L1TMHT_Et;
}

void trgfw::L1TAlgoResults::updateAdress(TTree* tree){
  
  tree->SetBranchAddress("L1TEGamma1_Et"      ,&m_L1TEGamma1_Et      );
  tree->SetBranchAddress("L1TEGamma1_Eta"     ,&m_L1TEGamma1_Eta     );
  tree->SetBranchAddress("L1TEGamma1_Phi"     ,&m_L1TEGamma1_Phi     );

  tree->SetBranchAddress("L1TIsoEGamma1_Et"   ,&m_L1TIsoEGamma1_Et   );
  tree->SetBranchAddress("L1TIsoEGamma1_Eta"  ,&m_L1TIsoEGamma1_Eta  );
  tree->SetBranchAddress("L1TIsoEGamma1_Phi"  ,&m_L1TIsoEGamma1_Phi  );

  tree->SetBranchAddress("L1TMuon1_Et"        ,&m_L1TMuon1_Et        );
  tree->SetBranchAddress("L1TMuon1_Eta"       ,&m_L1TMuon1_Eta       );
  tree->SetBranchAddress("L1TMuon1_Phi"       ,&m_L1TMuon1_Phi       );

  tree->SetBranchAddress("L1TIsoMuon1_Et"     ,&m_L1TIsoMuon1_Et     );
  tree->SetBranchAddress("L1TIsoMuon1_Eta"    ,&m_L1TIsoMuon1_Eta    );
  tree->SetBranchAddress("L1TIsoMuon1_Phi"    ,&m_L1TIsoMuon1_Phi    );

  tree->SetBranchAddress("L1TTau_N"           ,&m_L1TTau_N           );
  tree->SetBranchAddress("L1TTau1_Et"         ,&m_L1TTau1_Et         );
  tree->SetBranchAddress("L1TTau1_Eta"        ,&m_L1TTau1_Eta        );
  tree->SetBranchAddress("L1TTau1_Phi"        ,&m_L1TTau1_Phi        );

  tree->SetBranchAddress("L1TTau2_Et"         ,&m_L1TTau2_Et         );
  tree->SetBranchAddress("L1TTau2_Eta"        ,&m_L1TTau2_Eta        );
  tree->SetBranchAddress("L1TTau2_Phi"        ,&m_L1TTau2_Phi        );

  tree->SetBranchAddress("L1TIsoTau_N"        ,&m_L1TIsoTau_N        );
  tree->SetBranchAddress("L1TIsoTau1_Et"      ,&m_L1TIsoTau1_Et      );
  tree->SetBranchAddress("L1TIsoTau1_Eta"     ,&m_L1TIsoTau1_Eta     );
  tree->SetBranchAddress("L1TIsoTau1_Phi"     ,&m_L1TIsoTau1_Phi     );

  tree->SetBranchAddress("L1TIsoTau2_Et"      ,&m_L1TIsoTau2_Et      );
  tree->SetBranchAddress("L1TIsoTau2_Eta"     ,&m_L1TIsoTau2_Eta     );
  tree->SetBranchAddress("L1TIsoTau2_Phi"     ,&m_L1TIsoTau2_Phi     );

  tree->SetBranchAddress("L1TJet_N"           ,&m_L1TJet_N           );
  tree->SetBranchAddress("L1TJet_AvgPt"       ,&m_L1TJet_AvgPt       );
  tree->SetBranchAddress("L1TJet_VecPt"       ,&m_L1TJet_VecPt       );
  tree->SetBranchAddress("L1TJet1_Et"         ,&m_L1TJet1_Et         );
  tree->SetBranchAddress("L1TJet1_Eta"        ,&m_L1TJet1_Eta        );
  tree->SetBranchAddress("L1TJet1_Phi"        ,&m_L1TJet1_Phi        );

  tree->SetBranchAddress("L1TJet2_Et"         ,&m_L1TJet2_Et         );
  tree->SetBranchAddress("L1TJet2_Eta"        ,&m_L1TJet2_Eta        );
  tree->SetBranchAddress("L1TJet2_Phi"        ,&m_L1TJet2_Phi        );

  tree->SetBranchAddress("L1TJet_maxMjj"      ,&m_L1TJet_maxMjj      );
  tree->SetBranchAddress("L1TJet_maxDEta"     ,&m_L1TJet_maxDEta     );

  tree->SetBranchAddress("L1TTau1_minDRL1TJet",&m_L1TTau1_minDRL1TJet);

  tree->SetBranchAddress("L1TMHT_Et"          ,&m_L1TMHT_Et          );
  tree->SetBranchAddress("L1TMET_Et"          ,&m_L1TMET_Et          );
  
}

void trgfw::L1TAlgoResults::fill(trgfw::Event &iEvent){
  
  vector<ic::L1TSum*>* mySums  = iEvent.getByName< vector<ic::L1TSum*> >("l1t_sum");
  
  ic::L1TSum* myMET = 0;
  ic::L1TSum* myMHT = 0;
  
  for(unsigned i=0; i<mySums->size(); i++){
    
    ic::L1TSum *iSum = mySums->at(i);
    
    if     (iSum->sumType==ic::L1TSum::SumType::kMissingEt){myMET = iSum;}
    else if(iSum->sumType==ic::L1TSum::SumType::kMissingHt){myMHT = iSum;}
  }
  
  // filling plots
  m_L1TMET_Et ->fill(myMET->vector().Et());
  m_L1TMHT_Et ->fill(myMHT->vector().Et());
  
  vector<ic::L1TEGamma*>* myEGs = iEvent.getByName< vector<ic::L1TEGamma*> >(tag_l1tEG);
  if(myEGs->size()>0){
    m_L1TEGamma1_Et ->fill(myEGs->at(0)->pt());
    m_L1TEGamma1_Eta->fill(myEGs->at(0)->phi());
    m_L1TEGamma1_Phi->fill(myEGs->at(0)->eta());
  }
  
  vector<ic::L1TEGamma*>* myIsoEGs = iEvent.getByName< vector<ic::L1TEGamma*> >(tag_l1tIsoEG);
  if(myIsoEGs->size()>0){
    m_L1TIsoEGamma1_Et ->fill(myEGs->at(0)->pt());
    m_L1TIsoEGamma1_Eta->fill(myEGs->at(0)->phi());
    m_L1TIsoEGamma1_Phi->fill(myEGs->at(0)->eta());
  }
  
  vector<ic::L1TMuon*>* myMuons = iEvent.getByName< vector<ic::L1TMuon*> >(tag_l1tMuon);
  if(myMuons->size()>0){
    m_L1TMuon1_Et ->fill(myMuons->at(0)->pt());
    m_L1TMuon1_Eta->fill(myMuons->at(0)->phi());
    m_L1TMuon1_Phi->fill(myMuons->at(0)->eta());
  }
  
  vector<ic::L1TMuon*>* myIsoMuons = iEvent.getByName< vector<ic::L1TMuon*> >(tag_l1tIsoMuon);
  if(myIsoMuons->size()>0){
    m_L1TIsoMuon1_Et ->fill(myMuons->at(0)->pt());
    m_L1TIsoMuon1_Eta->fill(myMuons->at(0)->phi());
    m_L1TIsoMuon1_Phi->fill(myMuons->at(0)->eta());
  }
  
  vector<ic::L1TTau*>* myTaus = iEvent.getByName< vector<ic::L1TTau*> >(tag_l1tTau);
  m_L1TTau_N->fill(myTaus->size());
  if(myTaus->size()>0){
    m_L1TTau1_Et ->fill(myTaus->at(0)->pt());
    m_L1TTau1_Phi->fill(myTaus->at(0)->phi());
    m_L1TTau1_Eta->fill(myTaus->at(0)->eta());
  }
  if(myTaus->size()>1){
    m_L1TTau2_Et ->fill(myTaus->at(1)->pt());
    m_L1TTau2_Phi->fill(myTaus->at(1)->phi());
    m_L1TTau2_Eta->fill(myTaus->at(1)->eta());
  }
  
  vector<ic::L1TTau*>* myIsoTaus = iEvent.getByName< vector<ic::L1TTau*> >(tag_l1tIsoTau);
  m_L1TIsoTau_N->fill(myIsoTaus->size());
  if(myIsoTaus->size()>0){
    m_L1TIsoTau1_Et ->fill(myIsoTaus->at(0)->pt());
    m_L1TIsoTau1_Phi->fill(myIsoTaus->at(0)->phi());
    m_L1TIsoTau1_Eta->fill(myIsoTaus->at(0)->eta());
  }
  if(myIsoTaus->size()>1){
    m_L1TIsoTau2_Et ->fill(myIsoTaus->at(1)->pt());
    m_L1TIsoTau2_Phi->fill(myIsoTaus->at(1)->phi());
    m_L1TIsoTau2_Eta->fill(myIsoTaus->at(1)->eta());
  }
  
  vector<ic::L1TJet*>* myJets = iEvent.getByName< vector<ic::L1TJet*> >(tag_l1tJet);
  if(tag_l1tJetPair==""){
    
    m_L1TJet_N->fill(myJets->size());
    
    if(myJets->size()>0){
      m_L1TJet1_Et ->fill(myJets->at(0)->pt());
      m_L1TJet1_Phi->fill(myJets->at(0)->phi());
      m_L1TJet1_Eta->fill(myJets->at(0)->eta());
    }
    if(myJets->size()>1){
      m_L1TJet2_Et ->fill(myJets->at(1)->pt());
      m_L1TJet2_Phi->fill(myJets->at(1)->phi());
      m_L1TJet2_Eta->fill(myJets->at(1)->eta());
    }
  }
  else{
    
    trgfw::L1TObjectPairCollection* myJetPair = iEvent.getByName< trgfw::L1TObjectPairCollection >(tag_l1tJetPair);
    
    m_L1TJet_N->fill(myJetPair->size());
    
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
    m_L1TJet1_Et    ->fill(maxObj1Pt);
    m_L1TJet2_Et    ->fill(maxObj2Pt);
    m_L1TJet_AvgPt  ->fill(maxAvgPt);
    m_L1TJet_VecPt  ->fill(maxVecPt);
    m_L1TJet_maxDEta->fill(maxDEta);
    m_L1TJet_maxMjj ->fill(maxMjj);
    
  }
  
  // Separation tau-jet
  if(myTaus->size()>0 && myJets->size()>0){
    
    double minDR = 999.;
    for(auto it=myJets->begin(); it!=myJets->end(); it++){
      double iDR = deltaR<ic::Candidate,ic::Candidate>(*(myTaus->at(0)),**it);
      if(iDR<minDR){minDR=iDR;}
    }
    m_L1TTau1_minDRL1TJet->fill(minDR);
  }
  
  
  
}

void trgfw::L1TAlgoResults::setVerbose(bool value){
  m_verbose = value;
}
