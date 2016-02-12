#ifndef CMSSW_VBFHiggsTauTau_GenParticleAnalysis_MicroGenParticle
#define CMSSW_VBFHiggsTauTau_GenParticleAnalysis_MicroGenParticle

#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"

#include <vector>

namespace VBFHiggsToTauTau {

  typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > VecP4;
  
  class MicroGenParticle {
  public:
    MicroGenParticle(){};
    MicroGenParticle(unsigned char iStatus, int iPdgId, signed char iCharge,VecP4 iVector) : 
      status(iStatus),
      pdgId (iPdgId),
      charge(iCharge),
      vector(iVector){};
  
    ~MicroGenParticle(){};
    
  public: 
    unsigned char status;
    int           pdgId;
    signed char   charge;
    VecP4         vector;
  };
  
  typedef std::vector<VBFHiggsToTauTau::MicroGenParticle> MicroGenParticleCollection;
  
}

#endif
