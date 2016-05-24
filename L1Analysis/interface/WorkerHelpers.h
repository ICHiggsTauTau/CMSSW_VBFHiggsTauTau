#ifndef VBFHiggsToTauTau_L1Analysis_InputHistograms
#define VBFHiggsToTauTau_L1Analysis_InputHistograms

// TRGFW includes
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TVariableScanDataFormat.h"

// ROOT includes
#include "TH1D.h"

// STD includes
#include <string>
#include <vector>

namespace trgfw{
  
  class InputHistograms{
  public:
    
    InputHistograms(){
      path            = "";
      name            = "";
      reverseIntegral = false;
      hEfficiency     = 0;
      hRate           = 0;
    }
    
    std::string path;
    std::string name;
    
    bool reverseIntegral;
    
    TH1D* hEfficiency;
    TH1D* hRate;
    
  };
  
  class InputScan{
  public:
  
    InputScan(){
      path            = "";
      name            = "";
      reverseIntegral = false;
      hEfficiency     = 0;
      hRate           = 0;
    }
  
    ~InputScan(){
      delete hEfficiency;
      delete hRate;
    }
  
    std::string path;
    std::string name;
  
    bool reverseIntegral;
  
    trgfw::L1TVariableScanDataFormat* hEfficiency;
    trgfw::L1TVariableScanDataFormat* hRate;
  
  };

}

#endif
