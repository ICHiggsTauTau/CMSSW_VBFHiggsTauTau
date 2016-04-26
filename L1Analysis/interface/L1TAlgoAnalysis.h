#ifndef VBFHiggsToTauTau_L1Analysis_L1TAlgoAnalysis
#define VBFHiggsToTauTau_L1Analysis_L1TAlgoAnalysis

// L1T Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"

// This algos
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgo.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TResolutionAnalysis.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/PlotsGenAnalysis.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/PlotsSingleObjects.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/AnalysisModule.h"

#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"

#include <string>
#include <vector>
#include <map>

class L1TAlgoAnalysis : public trgfw::AnalysisModule {
public:
  
  enum DataType{
    Data,
    MC
  };
  
  L1TAlgoAnalysis();
  L1TAlgoAnalysis(std::string name, TDirectory* baseDirector);
  ~L1TAlgoAnalysis();
  
  void init();
  
  void beginJob();
  void run(trgfw::Event &iEvent);
  
  void resetEvent();
  
  void setVerbose (bool value);
  void setDataType(L1TAlgoAnalysis::DataType type);
  
  // Task setters
  void setDoGenAnalysis          (bool value);
  void setDoSingleObjectsAnalysis(bool value);
  
  
private:
  
  void doGenAnalysis          (trgfw::Event &iEvent);
  void doSingleObjectsAnalysis(trgfw::Event &iEvent);
  
private:
  
  bool m_verbose;
  
  DataType m_dataType;
  
  std::string  m_fileOutName;
  
  std::vector<TH1*> m_h;
  TH1D *m_EventCount;
  TH1D *m_AlgoPass;
  TH1D *m_HiggsDecay;
  
  bool               m_doGenAnalysis;
  PlotsGenAnalysis   m_plotsGenAnalysis;

  bool               m_doSingleObjectsAnalysis;
  PlotsSingleObjects m_plotsSingleObjects;
  
  L1TResolutionAnalysis *m_L1TResolutionAnalysis;
  
  std::vector<L1TAlgo*> m_algos;
  
};

#endif