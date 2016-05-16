#ifndef VBFHiggsToTauTau_L1Analysis_InputHistograms
#define VBFHiggsToTauTau_L1Analysis_InputHistograms

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
  
}

#endif
