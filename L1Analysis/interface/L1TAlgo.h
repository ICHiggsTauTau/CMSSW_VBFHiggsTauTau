#ifndef VBFHiggsToTauTau_L1Analysis_L1TAlgo
#define VBFHiggsToTauTau_L1Analysis_L1TAlgo

// L1T Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"

#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"

#include <string>
#include <vector>
#include <map>

class L1TAlgo{
public:
    
  L1TAlgo();
  L1TAlgo(std::string name, TDirectory* baseDirector);
  ~L1TAlgo();
  
  void init();
  void initPlots();
  
  bool run(icTrg::Event &iEvent);
  
  void setVerbose  (bool value);
  void addCondition(std::function<bool(icTrg::Event &iEvent)> function);
  
  std::string getName();
  
private:
  
  bool m_verbose;
  
  std::vector<std::function<bool(icTrg::Event &iEvent)> > m_conditions;
  
  std::string m_name;
  
public:
  
  TDirectory *m_plotsDirectory;
  
  TH1D *m_L1TEGamma1_Et;
  TH1D *m_L1TEGamma1_Eta;
  TH1D *m_L1TEGamma1_Phi;
  
  TH1D *m_L1TMuon1_Et;
  TH1D *m_L1TMuon1_Eta;
  TH1D *m_L1TMuon1_Phi;
  
  TH1D *m_L1TTau1_Et; 
  TH1D *m_L1TTau1_Eta;
  TH1D *m_L1TTau1_Phi;
    
  TH1D *m_L1TTau2_Et;
  TH1D *m_L1TTau2_Eta;
  TH1D *m_L1TTau2_Phi;
  
  TH1D *m_L1TIsoTau1_Et; 
  TH1D *m_L1TIsoTau1_Eta;
  TH1D *m_L1TIsoTau1_Phi;
  
  TH1D *m_L1TIsoTau2_Et;
  TH1D *m_L1TIsoTau2_Eta;
  TH1D *m_L1TIsoTau2_Phi;
  
  TH1D *m_L1TJet1_Et; 
  TH1D *m_L1TJet1_Eta;
  TH1D *m_L1TJet1_Phi;
  
  TH1D *m_L1TJet2_Et;
  TH1D *m_L1TJet2_Eta;
  TH1D *m_L1TJet2_Phi;
  
  TH1D *m_L1TJet_minMjj;
  TH1D *m_L1TJet_minDEta;
  
  TH1D *m_L1TJetDijet20_maxMjj;
  TH1D *m_L1TJetDijet20_maxDEta;
  
  TH1D *m_L1TJetDijet30_maxMjj;
  TH1D *m_L1TJetDijet30_maxDEta;
  
  TH1D *m_L1TJetDijet40_maxMjj;
  TH1D *m_L1TJetDijet40_maxDEta;
  
  TH1D *m_L1TJetDijet50_maxMjj;
  TH1D *m_L1TJetDijet50_maxDEta;
  
 };
 
 #endif