#ifndef VBFHiggsToTauTau_L1Analysis_Event
#define VBFHiggsToTauTau_L1Analysis_Event

// L1T Objects
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TObject.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TEGamma.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TMuon.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TTau.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TJet.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TSum.hh"

#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/interface/MicroGenParticle.h"
#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/interface/VBFHiggsGenAnalysisDataFormat.h"

// BOOST includes
#include <boost/any.hpp>

// C++ STD includes
#include <string>
#include <vector>
#include <map>

namespace trgfw{
  
  typedef std::pair  <const ic::L1TObject*,const ic::L1TObject*> L1TObjectPair;
  typedef std::vector<L1TObjectPair>                             L1TObjectPairCollection;
  
  // Event owns information in it
  class Event{
  public:
    
    Event();
    ~Event();
    
    VBFHiggs::GenAnalysisDataFormat *genInfo;
    
    void   printCollections();
    
    template <class Product>
    void addProduct(std::string productName, Product &product);
    
    template <class Product>
    Product* getByName(std::string productName);
    
    bool containsProduct(std::string productName);
    
  private:

    std::map<std::string,boost::any>  m_products;
    
  };
}



template <class Product> void trgfw::Event::addProduct(std::string productName, Product &product){
  
  m_products[productName] = product;
}

template <class Product> Product* trgfw::Event::getByName(std::string productName){
  
  std::map<std::string,boost::any>::iterator it = m_products.find(productName);
  
  if(it != m_products.end()){
    return boost::any_cast<Product>(&it->second);
  }else{
    std::cout << "[trgfw::Event::getByName] ERROR: Branch=" << productName << " not found..." << std::endl;
    return 0;
  }
}

#endif
