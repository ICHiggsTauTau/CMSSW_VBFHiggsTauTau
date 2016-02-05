#ifndef GenParticleAnalysis_DecayAnalyzer_h
#define GenParticleAnalysis_DecayAnalyzer_h

// CMSSW include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "DataFormats/JetReco/interface/GenJetCollection.h"

// ROOT includes
#include "TFile.h"
#include "TH1D.h"

// C++ include files
#include <memory>
#include <map>

//
// class declaration
//

class DecayAnalyzer : public edm::EDProducer {
public:
  explicit DecayAnalyzer(const edm::ParameterSet&);
  ~DecayAnalyzer();
  
  void produce(edm::Event& iEvent,const edm::EventSetup& iSetup);
private:

  //   std::vector<const reco::GenJet*> filterGenJets(const std::vector<reco::GenJet>* jets);
  
  //**************************
  // Private Member data *****
private:
  
  edm::ParameterSet ps;
  
  // Output file
  TFile* fOut;
  
  // Input tags
  edm::EDGetTokenT< edm::HepMCProduct >           m_InputTag_HepMCProduct;
  edm::EDGetTokenT< reco::GenJetCollection >      m_inputTag_GenJetCollection;
  edm::EDGetTokenT< reco::GenParticleCollection > m_inputTag_GenParticleCollection;
  
  
  TH1D *m_EventCount;
  
  TH1D *m_Parton_N;
  TH1D *m_Parton_Type;
  
  TH1D *m_Tau_N;
  
  

};

#endif