#ifndef VBFHiggsToTauTau_L1Analysis_AlgoWorkingPoint
#define VBFHiggsToTauTau_L1Analysis_AlgoWorkingPoint

namespace trgfw{
  
  class AlgoWorkingPoint{
  public:
    
    AlgoWorkingPoint(){};
    
    std::string name;
    std::string var;
    double      value;
    double      rate;
    double      eff;
  };
  
  struct sortGreaterEff_AlgoWorkingPoint{
    bool operator() (trgfw::AlgoWorkingPoint a,trgfw::AlgoWorkingPoint b) {
      return (a.eff > b.eff);
    }
  };
  
}

#endif
 
