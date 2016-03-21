#ifndef VBFHiggsToTauTau_STYLE_STYLE
#define VBFHiggsToTauTau_STYLE_STYLE

#include "TStyle.h"

namespace hepfw{
  
  /***********************************************/
  /** \brief Style 
   * 
   * Style
   ***********************************************/
  class Style {
    
  public:
    
    Style();
    
    void setTDRStyle();
    
  public:  
    
    TStyle *tdrStyle;
    
  };

}

#endif
