#ifndef VBFHiggsToTauTau_DataFormats_File
#define VBFHiggsToTauTau_DataFormats_File
 
#include "TFile.h"
#include "TH1.h"

// C++ Included
#include <vector>

namespace hepfw{

  /***********************************************/
  /** \brief File
   * 
   * Class hepfw::File directly inherits from ROOT TFile and expands its 
   * functionality
   ***********************************************/ 
  class File : public TFile {
  public:
    
    File();
    File(const char* fname, Option_t* option = "", const char* ftitle = "", Int_t compress = 1);
    std::vector<TH1*> getHistograms();
    void              copyDirectoryStructure(TFile *file);
  };
  
}

#endif