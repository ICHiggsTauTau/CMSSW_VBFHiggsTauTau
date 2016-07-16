#ifndef VBFHiggsToTauTau_L1Analysis_L1TAlgoResults
#define VBFHiggsToTauTau_L1Analysis_L1TAlgoResults

#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TVariableScanDataFormat.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"

#include <string>
#include <vector>
#include <map>

namespace trgfw{
  
  class L1TAlgoResults{
  public:
    
    L1TAlgoResults(){}
    L1TAlgoResults(std::string path,std::string name);
    ~L1TAlgoResults();
    
    void fill(trgfw::Event &iEvent);
    
    void setVerbose  (bool value);
    void updateAdress(TTree* tree);
    
  public:
    
    std::string tag_l1tEG;
    std::string tag_l1tIsoEG;  
    std::string tag_l1tMuon;
    std::string tag_l1tIsoMuon;
    std::string tag_l1tTau;
    std::string tag_l1tIsoTau;
    std::string tag_l1tJet;
    std::string tag_l1tSum;
    
    std::string tag_l1tJetPair;
    
  private:
    
    bool        m_verbose;
    std::string m_name;
    std::string m_path;

    trgfw::L1TVariableScanDataFormat *m_L1TEGamma1_Et;
    trgfw::L1TVariableScanDataFormat *m_L1TEGamma1_Eta;
    trgfw::L1TVariableScanDataFormat *m_L1TEGamma1_Phi;
    
    trgfw::L1TVariableScanDataFormat *m_L1TIsoEGamma1_Et;
    trgfw::L1TVariableScanDataFormat *m_L1TIsoEGamma1_Eta;
    trgfw::L1TVariableScanDataFormat *m_L1TIsoEGamma1_Phi;
    
    trgfw::L1TVariableScanDataFormat *m_L1TMuon1_Et;
    trgfw::L1TVariableScanDataFormat *m_L1TMuon1_Eta;
    trgfw::L1TVariableScanDataFormat *m_L1TMuon1_Phi;
    
    trgfw::L1TVariableScanDataFormat *m_L1TIsoMuon1_Et;
    trgfw::L1TVariableScanDataFormat *m_L1TIsoMuon1_Eta;
    trgfw::L1TVariableScanDataFormat *m_L1TIsoMuon1_Phi;
    
    trgfw::L1TVariableScanDataFormat *m_L1TTau_N;
    trgfw::L1TVariableScanDataFormat *m_L1TTau1_Et; 
    trgfw::L1TVariableScanDataFormat *m_L1TTau1_Eta;
    trgfw::L1TVariableScanDataFormat *m_L1TTau1_Phi;
    
    trgfw::L1TVariableScanDataFormat *m_L1TTau2_Et;
    trgfw::L1TVariableScanDataFormat *m_L1TTau2_Eta;
    trgfw::L1TVariableScanDataFormat *m_L1TTau2_Phi;
    
    trgfw::L1TVariableScanDataFormat *m_L1TIsoTau_N;
    trgfw::L1TVariableScanDataFormat *m_L1TIsoTau1_Et; 
    trgfw::L1TVariableScanDataFormat *m_L1TIsoTau1_Eta;
    trgfw::L1TVariableScanDataFormat *m_L1TIsoTau1_Phi;
    
    trgfw::L1TVariableScanDataFormat *m_L1TIsoTau2_Et;
    trgfw::L1TVariableScanDataFormat *m_L1TIsoTau2_Eta;
    trgfw::L1TVariableScanDataFormat *m_L1TIsoTau2_Phi;
    
    trgfw::L1TVariableScanDataFormat *m_L1TJet_N;
    
    trgfw::L1TVariableScanDataFormat *m_L1TJet_AvgPt;
    trgfw::L1TVariableScanDataFormat *m_L1TJet_VecPt;
    trgfw::L1TVariableScanDataFormat *m_L1TJet1_Et; 
    trgfw::L1TVariableScanDataFormat *m_L1TJet1_Eta;
    trgfw::L1TVariableScanDataFormat *m_L1TJet1_Phi;
    
    trgfw::L1TVariableScanDataFormat *m_L1TJet2_Et;
    trgfw::L1TVariableScanDataFormat *m_L1TJet2_Eta;
    trgfw::L1TVariableScanDataFormat *m_L1TJet2_Phi;
    
    //trgfw::L1TVariableScanDataFormat *m_L1TJet3_Et;
    //trgfw::L1TVariableScanDataFormat *m_L1TJet3_Eta;
    //trgfw::L1TVariableScanDataFormat *m_L1TJet3_Phi;
    
    trgfw::L1TVariableScanDataFormat *m_L1TJet_maxMjj;
    trgfw::L1TVariableScanDataFormat *m_L1TJet_maxDEta;
    
    trgfw::L1TVariableScanDataFormat *m_L1TTau1_minDRL1TJet;
    
    trgfw::L1TVariableScanDataFormat *m_L1TMET_Et;
    trgfw::L1TVariableScanDataFormat *m_L1TMHT_Et;
    
  };
  
}

#endif
