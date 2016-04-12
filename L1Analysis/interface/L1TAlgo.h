#ifndef VBFHiggsToTauTau_L1Analysis_L1TAlgo
#define VBFHiggsToTauTau_L1Analysis_L1TAlgo

// L1T Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"

#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoPlots.h"

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
  
  bool run(icTrg::Event &iEvent);
  
  void setVerbose  (bool value);
  void addCondition(std::function<bool(icTrg::Event &iEvent)> function);
  
  std::string getName();
  
private:
  
  bool m_verbose;
  
  std::vector<std::function<bool(icTrg::Event &iEvent)> > m_conditions;
  
  std::string m_name;
  
public:
  
  L1TAlgoPlots plots;
  
 };
 
 #endif