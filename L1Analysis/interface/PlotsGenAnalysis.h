#ifndef VBFHiggsToTauTau_L1Analysis_PlotsGenAnalysis
#define VBFHiggsToTauTau_L1Analysis_PlotsGenAnalysis

#include "TDirectory.h"
#include "TH1D.h"

#include <string>
#include <vector>
#include <map>

class PlotsGenAnalysis{
public:
  
  PlotsGenAnalysis();
  ~PlotsGenAnalysis();
  void create(TDirectory *dir);
  
  std::vector<std::string> m_channels;
  
  TH1D *m_HiggsDecay;
  
  std::map<std::string,TH1D*> m_L1Tau1_ResolutionEt;
  std::map<std::string,TH1D*> m_L1Tau2_ResolutionEt;
  std::map<std::string,TH1D*> m_L1Tau1_ResolutionEta;
  std::map<std::string,TH1D*> m_L1Tau2_ResolutionEta;
  std::map<std::string,TH1D*> m_L1Tau1_ResolutionPhi;
  std::map<std::string,TH1D*> m_L1Tau2_ResolutionPhi;
};

#endif