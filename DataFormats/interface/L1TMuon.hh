#ifndef ICHiggsTauTau_L1TMuon_hh
#define ICHiggsTauTau_L1TMuon_hh

#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TObject.hh"

#include <vector>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Rtypes.h"

namespace ic {
  
  class L1TMuon : public ic::L1TObject {
  public:
    L1TMuon();
    virtual ~L1TMuon();
    virtual void Print() const;
    
  #ifndef SKIP_CINT_DICT
  public:
    ClassDef(L1TMuon, 1);
  #endif
    
  public:
    short int          charge;
    unsigned short int isolation;
    unsigned short int quality;
  };
  
  typedef std::vector<ic::L1TMuon> L1TMuonCollection;
}

#endif
