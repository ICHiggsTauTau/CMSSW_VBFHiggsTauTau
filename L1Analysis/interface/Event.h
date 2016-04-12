#ifndef VBFHiggsToTauTau_L1Analysis_Event
#define VBFHiggsToTauTau_L1Analysis_Event

// L1T Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TSum.hh"

#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/interface/MicroGenParticle.h"
#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/interface/VBFHiggsToTauTauGenAnalysisDataFormat.h"

#include <string>
#include <vector>
#include <map>

namespace icTrg{
  
  typedef std::pair  <const ic::L1TObject*,const ic::L1TObject*> L1TObjectPair;
  typedef std::vector<L1TObjectPair>                             L1TObjectPairCollection;
  
  // Event owns information in it
  class Event{
  public:
    
    Event();
    ~Event();
    
    ic::L1TEGammaCollection l1tEGammaCollection;
    ic::L1TMuonCollection   l1tMuonCollection;
    ic::L1TTauCollection    l1tTauCollection;
    ic::L1TTauCollection    l1tIsoTauCollection; // All elements are duplicated from regular taus
    ic::L1TJetCollection    l1tJetCollection;
    ic::L1TSumCollection    l1tSumCollection;
    
    VBFHiggsToTauTau::GenAnalysisDataFormat *genInfo;
    
    void   printCollections();
    double getDijetMaxMjj(double pt);
    
    bool                              add     (std::string name,std::vector<ic::L1TObject>     *objs);
    const std::vector<ic::L1TObject>* get     (std::string name);
    bool                              contains(std::string name);
    
    bool                                  addPairs    (std::string name,icTrg::L1TObjectPairCollection *pairs);
    const icTrg::L1TObjectPairCollection* getPairs    (std::string name);
    bool                                  containsPair(std::string name);
    
  private:
    
    std::map<double,double>                                m_dijet_maxMjj;
    std::map<std::string,std::vector<ic::L1TObject>* >     m_collections;
    std::map<std::string,icTrg::L1TObjectPairCollection* > m_pairs;
  };
}

#endif
