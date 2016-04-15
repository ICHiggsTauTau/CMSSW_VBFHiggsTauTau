#ifndef VBFHiggsToTauTau_L1Analysis_L1TAlgoPlots
#define VBFHiggsToTauTau_L1Analysis_L1TAlgoPlots

#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"

#include <string>
#include <vector>
#include <map>

class L1TAlgoPlots{
public:
  
  L1TAlgoPlots(std::string name, TDirectory* baseDirector);
  ~L1TAlgoPlots();
  
  void fill(icTrg::Event &iEvent);
  
  void setVerbose  (bool value);

public:
  
  std::string tag_l1tEG;
  std::string tag_l1tMuon;
  std::string tag_l1tTau;
  std::string tag_l1tIsoTau;
  std::string tag_l1tJet;
  std::string tag_l1tSum;
  
  std::string tag_l1tJetPair;
  
private:
  
  bool        m_verbose;
  std::string m_name;
  
  TDirectory *m_plotsDirectory;
  
  TH1D *m_L1TEGamma1_Et;
  TH1D *m_L1TEGamma1_Eta;
  TH1D *m_L1TEGamma1_Phi;

  TH1D *m_L1TMuon1_Et;
  TH1D *m_L1TMuon1_Eta;
  TH1D *m_L1TMuon1_Phi;
  
  TH1D *m_L1TTau_N;
  TH1D *m_L1TTau1_Et; 
  TH1D *m_L1TTau1_Eta;
  TH1D *m_L1TTau1_Phi;
  
  TH1D *m_L1TTau2_Et;
  TH1D *m_L1TTau2_Eta;
  TH1D *m_L1TTau2_Phi;
  
  TH1D *m_L1TIsoTau_N;
  TH1D *m_L1TIsoTau1_Et; 
  TH1D *m_L1TIsoTau1_Eta;
  TH1D *m_L1TIsoTau1_Phi;
  
  TH1D *m_L1TIsoTau2_Et;
  TH1D *m_L1TIsoTau2_Eta;
  TH1D *m_L1TIsoTau2_Phi;
  
  TH1D *m_L1TJet_N;

  TH1D *m_L1TJet_AvgPt;
  TH1D *m_L1TJet1_Et; 
  TH1D *m_L1TJet1_Eta;
  TH1D *m_L1TJet1_Phi;
  
  TH1D *m_L1TJet2_Et;
  TH1D *m_L1TJet2_Eta;
  TH1D *m_L1TJet2_Phi;
  
  TH1D *m_L1TJet_maxMjj;
  TH1D *m_L1TJet_maxDEta;
  
  TH1D *m_L1TTau1_minDRL1TJet;
  
  TH1D *m_L1TMET_Et;
  TH1D *m_L1TMHT_Et;
  
  TH2D *m_METvsMjj;
  
};

#endif