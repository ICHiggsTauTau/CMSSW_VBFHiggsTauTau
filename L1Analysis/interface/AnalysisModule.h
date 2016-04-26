#ifndef VBFHiggsToTauTau_L1Analysis_AnalysisModule
#define VBFHiggsToTauTau_L1Analysis_AnalysisModule

// Framework includes
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

// ROOT includes
#include "TDirectory.h"

// C++ STD includes
#include <string>

namespace trgfw{
  
  /***********************************************/
  /** \brief Module
   * 
   * Module
   ***********************************************/
  class AnalysisModule {
  public:
 
    
    AnalysisModule();
    AnalysisModule(std::string name,TDirectory* baseDirector);
    virtual ~AnalysisModule();
    
    std::string getName();

    virtual void beginJob();
    virtual void run     (trgfw::Event &iEvent);
    virtual void endJob  ();
    
  protected:
    
    std::string  m_name;
    TDirectory  *m_dir;
    
  };
  
}

#endif
