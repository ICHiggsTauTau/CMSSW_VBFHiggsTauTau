#ifndef ICHiggsTauTau_L1TTau_hh
#define ICHiggsTauTau_L1TTau_hh

#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TObject.hh"

#include <vector>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Rtypes.h"

namespace ic {
  
  class L1TTau : public ic::L1TObject {
  public:
    L1TTau();
    virtual ~L1TTau();
    virtual void Print() const;
    
  #ifndef SKIP_CINT_DICT
  public:
    ClassDef(L1TTau, 1);
  #endif
    
  public:
    short int isolation;
    
  };
  
  typedef std::vector<ic::L1TTau> L1TTauCollection;
}

#endif
