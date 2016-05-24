#ifndef VBFHiggsToTauTau_L1Analysis_L1TAlgo
#define VBFHiggsToTauTau_L1Analysis_L1TAlgo

// L1T Objects
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TObject.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TEGamma.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TMuon.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TTau.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TJet.hh"

#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoPlots.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoResults.h"

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
  L1TAlgo(std::string path, std::string name);
  ~L1TAlgo();
  
  void init();
  
  bool run(trgfw::Event &iEvent);
  
  void setVerbose  (bool value);
  void addCondition(std::function<bool(trgfw::Event &iEvent)> function);
  
  std::string getName();
  
private:
  
  bool m_verbose;
  
  std::vector<std::function<bool(trgfw::Event &iEvent)> > m_conditions;
  
  std::string m_name;
  
public:
  
  //L1TAlgoPlots          plots;
  trgfw::L1TAlgoResults *results;
  
 };
 
 #endif
