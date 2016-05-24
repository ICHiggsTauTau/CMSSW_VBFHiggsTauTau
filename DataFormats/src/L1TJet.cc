#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TJet.hh"

namespace ic {
  L1TJet::L1TJet() {}
  
  L1TJet::~L1TJet() {}
  
  void L1TJet::Print() const { Candidate::Print(); }
}
