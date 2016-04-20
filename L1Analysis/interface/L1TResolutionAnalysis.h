#ifndef VBFHiggsToTauTau_L1Analysis_L1TResolutionAnalysis
#define VBFHiggsToTauTau_L1Analysis_L1TResolutionAnalysis

#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TH1D.h"

#include <string>
#include <vector>
#include <map>

class L1TResolutionAnalysis{
public:
  
  L1TResolutionAnalysis(TDirectory* baseDirector);
  ~L1TResolutionAnalysis();
  
  void setVerbose(bool value);
  
  bool run(icTrg::Event &iEvent);
  
private:
  
  bool m_verbose;
  
  TH1D* m_GenMET;
  TH1D* m_L1TETM;
  TH1D* m_L1TETM_Resolution;
  
};

#endif