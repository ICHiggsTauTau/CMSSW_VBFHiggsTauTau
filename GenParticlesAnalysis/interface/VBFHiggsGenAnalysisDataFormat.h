#ifndef CMSSW_VBFHiggsTauTau_GenParticleAnalysis_GenAnalysisVBFHiggsToTauTauDataFormat
#define CMSSW_VBFHiggsTauTau_GenParticleAnalysis_GenAnalysisVBFHiggsToTauTauDataFormat

#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/interface/MicroGenParticle.h"

#include <vector>

namespace VBFHiggs {

  enum HiggsDecay {
    EleEle=1,
    EleMuo=2,
    EleHad=3,
    MuoMuo=4,
    MuoHad=5,
    HadHad=6,
    Invisible=7,
  };
  
  enum TauDecay {
    Ele=1,
    Muo=2,
    Had=3,
  };
  
  class GenAnalysisDataFormat {
  public: // Enumerator
    
    GenAnalysisDataFormat(){ reset();};
    ~GenAnalysisDataFormat(){};
    
    void reset(){
      higgs_decayType=0;
      tau1_decayType =0;
      tau2_decayType =0;
      genMet         =0;
      
      tau1_stableDecayProducts.clear();
      tau2_stableDecayProducts.clear();
    }
    
  public: // Members
    unsigned char higgs_decayType;
    unsigned char tau1_decayType;
    unsigned char tau2_decayType;
    
    double genMet;
    
    VBFHiggsToTauTau::MicroGenParticleCollection tau1_stableDecayProducts;
    VBFHiggsToTauTau::MicroGenParticleCollection tau2_stableDecayProducts;
  }; 
}
#endif