#ifndef VBFHiggsToTauTau_L1Analysis_L1TResolutionAnalysis
#define VBFHiggsToTauTau_L1Analysis_L1TResolutionAnalysis

#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/AnalysisModule.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TH1D.h"
#include "TProfile.h"

#include <string>
#include <vector>
#include <map>

class L1TResolutionAnalysis : public trgfw::AnalysisModule {
public:
  
  L1TResolutionAnalysis(std::string name, TDirectory* baseDirector);
  ~L1TResolutionAnalysis();
  
  void setVerbose(bool value);
  
  void run(trgfw::Event &iEvent);
  
private:
  
  bool m_verbose;
  
  TH1D*     m_GenMET;
  
  TH1D*     m_L1TETM;
  TH1D*     m_L1THTM;
  
  TH1D*     m_L1TETM_Resolution;
  TProfile* m_L1TETM_ResolutionVsEt;
  
};

#endif