#ifndef VBFHiggsToTauTau_L1Analysis_Event
#define VBFHiggsToTauTau_L1Analysis_Event

// L1T Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"

namespace icTrg{
  
  // Event owns information in it
  class Event{
  public:
    
    Event();
    ~Event();
    
    ic::L1TEGammaCollection l1tEGammaCollection;
    ic::L1TMuonCollection   l1tMuonCollection;
    ic::L1TTauCollection    l1tTauCollection;
    ic::L1TJetCollection    l1tJetCollection;
    
  };
}

#endif
