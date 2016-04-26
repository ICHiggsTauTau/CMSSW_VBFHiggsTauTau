#ifndef VBFHiggsToTauTau_L1Analysis_PlotsSingleObjects
#define VBFHiggsToTauTau_L1Analysis_PlotsSingleObjects

#include "TDirectory.h"
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
  
  TH1D *m_L1TMet_Et;
  TH1D *m_L1TMet_Phi;
  
  TH1D *m_L1TMHT_Et;
  TH1D *m_L1TMHT_Phi;
};

#endif