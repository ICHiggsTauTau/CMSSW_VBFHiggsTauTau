#include <vector>
#include <map>
#include <utility>
#include <string>
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/Candidate.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/EventInfo.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/PileupInfo.hh"

// Generator level objects
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/GenParticle.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/GenJet.hh"

// L1T Objects
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TObject.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TEGamma.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TMuon.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TTau.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TJet.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TSum.hh"

namespace { struct dictionary {

  
  std::pair<unsigned long, float>                dictEventContent1;
  std::vector< std::pair<unsigned long, float> > dictEventContent2;
  ic::EventInfo                                  dictEventInfo;
  
  ic::PileupInfo dictPilupInfo;
  
  ic::GenParticle              dictGenParticle;
  std::vector<ic::GenParticle> dictGenParticleCollection;
  ic::GenJet                   dictGenJet;
  std::vector<ic::GenJet>      dictGenJetCollection;

  ic::Candidate dictCandidate;
  std::vector<ic::Candidate> dictCandidateCollection;
  
  ic::L1TObject              dictL1TObject;
  std::vector<ic::L1TObject> dictL1TObjectCollection;
  ic::L1TEGamma              dictL1TEGamma;
  std::vector<ic::L1TEGamma> dictL1TEGammaCollection;
  ic::L1TMuon                dictL1TMuon;
  std::vector<ic::L1TMuon>   dictL1TMuonCollection;
  ic::L1TTau                 dictL1TTau;
  std::vector<ic::L1TTau>    dictL1TTauCollection;
  ic::L1TJet                 dictL1TJet;
  std::vector<ic::L1TJet>    dictL1TJetCollection;
  ic::L1TSum                 dictL1TSum;
  std::vector<ic::L1TSum>    dictL1TSumCollection;
  
};
}

