#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TMuon.hh"

namespace ic {
  L1TMuon::L1TMuon() {}
  
  L1TMuon::~L1TMuon() {}
  
  void L1TMuon::Print() const { Candidate::Print(); }
}
