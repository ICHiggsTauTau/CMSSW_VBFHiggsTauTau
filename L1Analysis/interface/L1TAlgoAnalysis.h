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

#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"

#include <string>
#include <vector>
#include <map>

class PlotsSingleObjects{
public:
  
  PlotsSingleObjects();
  ~PlotsSingleObjects();
  void create(TDirectory *dir);

  TH1D *m_L1TEGamma_N;
  TH1D *m_L1TEGamma_Et;
  TH1D *m_L1TEGamma_Eta;
  TH1D *m_L1TEGamma_Phi;
  
  TH1D *m_L1TMuon_N;
  TH1D *m_L1TMuon_Et;
  TH1D *m_L1TMuon_Eta;
  TH1D *m_L1TMuon_Phi;
  
  TH1D *m_L1TTau_N;
  TH1D *m_L1TTau_Et;
  TH1D *m_L1TTau_Eta;
  TH1D *m_L1TTau_Phi;
  
  TH1D *m_L1TJet_N;
  TH1D *m_L1TJet_Et;
  TH1D *m_L1TJet_Eta;
  TH1D *m_L1TJet_Phi;
};

class L1TAlgoAnalysis {
public:
  
  enum DataType{
    Data,
    MC
  };
  
  L1TAlgoAnalysis();
  ~L1TAlgoAnalysis();
  
  void init();
  void initPlots();
  void resetEvent();
  
  void setVerbose (bool value);
  void setDataType(L1TAlgoAnalysis::DataType type);
  
  void setDoSingleObjectsAnalysis(bool value);
  
  void processEvent(icTrg::Event &iEvent);
  
private:
  
  void doSingleObjectsAnalysis(icTrg::Event &iEvent);
  
private:
  
  bool m_verbose;
  
  DataType m_dataType;
  
  TFile       *m_fileOut;
  std::string  m_fileOutName;
  
  std::vector<TH1*> m_h;
  TH1D *m_EventCount;
  TH1D *m_HiggsDecay;
  
  bool               m_doSingleObjectsAnalysis;
  PlotsSingleObjects m_plotsSingleObjects;
  
  std::vector<L1TAlgo*> m_algos;
  
};

#endif