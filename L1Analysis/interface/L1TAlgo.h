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
  
private:
  
  bool m_verbose;
  
  std::vector<std::function<bool(icTrg::Event &iEvent)> > m_conditions;
  
  std::string m_name;
  
  TDirectory *m_plotsDirectory;
  
  TH1D *m_L1TTau1_Et; 
  TH1D *m_L1TTau1_Eta;
  TH1D *m_L1TTau1_Phi;
    
  TH1D *m_L1TTau2_Et;
  TH1D *m_L1TTau2_Eta;
  TH1D *m_L1TTau2_Phi;
  
  TH1D *m_L1TJet1_Et; 
  TH1D *m_L1TJet1_Eta;
  TH1D *m_L1TJet1_Phi;
  
  TH1D *m_L1TJet2_Et;
  TH1D *m_L1TJet2_Eta;
  TH1D *m_L1TJet2_Phi;
  
  
 };
 
 #endif