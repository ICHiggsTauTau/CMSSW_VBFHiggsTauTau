// -*- C++ -*-
#ifndef VBFHiggsToInvisible_TriggerSimpleAnalyzer_H
#define VBFHiggsToInvisible_TriggerSimpleAnalyzer_H

// -*- C++ -*-
//
// Package:    TriggerSimpleAnalyzer
// Class:      TriggerSimpleAnalyzer
// 
/**\class TriggerSimpleAnalyzer TriggerSimpleAnalyzer.cc VBFHiggsToInvisible/TriggerSimpleAnalyzer/src/TriggerSimpleAnalyzer.cc
 * 
 * Description: TODO
 * 
 * Implementation:
 *     TODO
 */
//
// Original Author:  Joao Arnauth Pela,510 1-002,+41227679913,
//         Created:  Thu Apr 10 14:39:33 CEST 2014
// $Id$
//
//

// CMSSW includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/L1Trigger/interface/L1EtMissParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerEvmReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtTriggerMenuLite.h"
#include "CondFormats/L1TObjects/interface/L1GtPrescaleFactors.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

// ROOT includes
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

// System include files
#include <memory>
#include <string>
#include <vector>
#include <map>

//
// class declaration
//

class TriggerSimpleAnalyzer : public edm::EDAnalyzer {
public:
  
  TriggerSimpleAnalyzer(const edm::ParameterSet&);
  ~TriggerSimpleAnalyzer();
  
private:
  void beginJob() ;
  void endJob() ;
  
  void beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup);
  void endRun(edm::Run const&, edm::EventSetup const&);
  
  void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  
  void analyze(const edm::Event&, const edm::EventSetup&);
  
  
  void printFiredHLT(const edm::Event& iEvent, edm::Handle<edm::TriggerResults> iHLT);
  bool testTrigger  (const edm::Event& iEvent, edm::Handle<edm::TriggerResults> iHLT, std::string iTriggerName);
  
  // ----------member data ---------------------------
  
  std::vector<std::string> m_hltPaths;
  
  edm::Handle< L1GtTriggerMenuLite > m_triggerMenuLite;
  
  edm::EDGetTokenT< L1GtTriggerMenuLite >        m_l1GtTriggerMenuLiteToken; // optional (default: "l1GtTriggerMenuLite")
  edm::EDGetTokenT<L1GlobalTriggerReadoutRecord> m_inputToken_GTReadout;
  edm::EDGetTokenT<edm::TriggerResults>          m_inputToken_HLTResults;
  
  std::map<int,std::string> m_alias;
  
  TFile* m_fileOut;
  
  TH1D* m_hEventCount;

  TH1D* m_hL1TPathCount;
  TH2D* m_hL1TCorrelation;

  TH1D* m_hHLTPathCount;
  TH2D* m_hHLTCorrelation;
  
};


#endif