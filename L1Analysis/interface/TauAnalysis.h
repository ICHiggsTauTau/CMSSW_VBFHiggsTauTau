#ifndef VBFHiggsToTauTau_L1Analysis_TauAnalysis
#define VBFHiggsToTauTau_L1Analysis_TauAnalysis

// CMSSW includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

// data formats
#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"

// ROOT includes
#include "TH1I.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"

// System include files
#include <memory>
#include <string>
#include <vector>
#include <map>

class TauAnalysis : public edm::EDAnalyzer {
public:
  
  TauAnalysis(const edm::ParameterSet&);
  ~TauAnalysis();
  
private:
  void beginJob() ;
  void endJob() ;
  
  void beginRun(edm::Run const&, edm::EventSetup const&);
  void endRun(edm::Run const&, edm::EventSetup const&);
  
  void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  
  void analyze(const edm::Event&, const edm::EventSetup&);
  
  void                     getEventContent(const edm::Event& iEvent);
  vector<const l1t::EGamma*>* getL1TEGammaCollection_EtSorted(int bx);
  vector<const l1t::Muon*>*   getL1TMuonCollection_EtSorted  (int bx);
  vector<const l1t::Jet*>*    getL1TJetCollection_EtSorted   (int bx);
  vector<const l1t::Tau*>*    getL1TTauCollection_EtSorted   (int bx);
  
  
  // ----------member data ---------------------------
  
  bool m_verbose;

  edm::ParameterSet ps;
  std::string m_L1TEra;
  
  
  // EDM Product Tokens
  edm::EDGetTokenT<std::string>                 m_EDToken_HiggsDecayMode;
  edm::EDGetTokenT<reco::GenParticleCollection> m_EDToken_HiggsDecayTau1;
  edm::EDGetTokenT<reco::GenParticleCollection> m_EDToken_HiggsDecayTau2;
  edm::EDGetTokenT<l1t::EGammaBxCollection>     m_EDToken_L1TEGamma;
  edm::EDGetTokenT<l1t::MuonBxCollection>       m_EDToken_L1TMuon;
  edm::EDGetTokenT<l1t::TauBxCollection>        m_EDToken_L1TTau;
  edm::EDGetTokenT<l1t::TauBxCollection>        m_EDToken_L1TTauIso;
  edm::EDGetTokenT<l1t::JetBxCollection>        m_EDToken_L1TJet;
  edm::EDGetTokenT<l1t::EtSumBxCollection>      m_EDToken_L1TSum;
  
  // EDM Handles
  edm::Handle<string>                      decayMode;
  edm::Handle<reco::GenParticleCollection> decayTau1;
  edm::Handle<reco::GenParticleCollection> decayTau2;
  edm::Handle<l1t::EGammaBxCollection>     m_Handle_L1TEGamma;
  edm::Handle<l1t::MuonBxCollection>       m_Handle_L1TMuon;
  edm::Handle<l1t::TauBxCollection>        m_Handle_L1TTau;
  edm::Handle<l1t::TauBxCollection>        m_Handle_L1TTauIso;
  edm::Handle<l1t::JetBxCollection>        m_Handle_L1TJet;
  edm::Handle<l1t::EtSumBxCollection>      m_Handle_L1TSums;
  
  std::vector<string> m_channels;
  
  // Output file and plots
  TFile *fOut;
  TH1D  *m_EventCount;
  TH1D  *m_HiggsDecay;
  
  TH1D  *m_L1Tau_N;
  TH1D  *m_L1Tau_Et;
  TH1D  *m_L1Tau_Eta;
  TH1D  *m_L1Tau_Phi;
  
  std::map<string,TH1D*> m_L1Tau1_Et;
  std::map<string,TH1D*> m_L1Tau2_Et;
  std::map<string,TH1D*> m_L1Object_InvMass;
  
  TH1D  *m_L1Jet_N;
  TH1D  *m_L1Jet_Et;
  TH1D  *m_L1Jet_Eta;
  TH1D  *m_L1Jet_Phi;
  
  std::map<string,TH1D*> m_L1Jet1_Et;
  std::map<string,TH1D*> m_L1Jet2_Et;
  
  std::map<string,TH1D*> m_L1Tau1_ResolutionEt;
  std::map<string,TH1D*> m_L1Tau2_ResolutionEt;
  std::map<string,TH1D*> m_L1Tau1_ResolutionEta;
  std::map<string,TH1D*> m_L1Tau2_ResolutionEta;
  std::map<string,TH1D*> m_L1Tau1_ResolutionPhi;
  std::map<string,TH1D*> m_L1Tau2_ResolutionPhi;
  
};

#endif
