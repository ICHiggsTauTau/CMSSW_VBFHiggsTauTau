#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TTau.hh"

namespace ic {
  L1TTau::L1TTau() {}
  
  L1TTau::~L1TTau() {}
  
  void L1TTau::Print() const { Candidate::Print(); }
}
