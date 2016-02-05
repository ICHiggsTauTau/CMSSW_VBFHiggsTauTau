#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/plugins/DecayAnalyzer.h"

#include "DataFormats/Math/interface/deltaPhi.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

// ROOT includes
#include "TMath.h"
#include "TString.h"

// C++ includes
#include <iostream>
#include <memory>

// user include files


using namespace edm;
using namespace std;

DecayAnalyzer::DecayAnalyzer(const edm::ParameterSet& pset){

  ps = pset;
    
  edm::InputTag inputTag_HepMCProduct          = pset.getUntrackedParameter<edm::InputTag>("inputTag_HepMCProduct",         edm::InputTag("generator"));
  edm::InputTag inputTag_GenJetCollection      = pset.getUntrackedParameter<edm::InputTag>("inputTag_GenJetCollection",     edm::InputTag("ak4GenJetsNoNu"));  
  edm::InputTag inputTag_GenParticleCollection = pset.getUntrackedParameter<edm::InputTag>("inputTag_GenParticleCollection",edm::InputTag("genParticles")); 
  
  produces< string >                     ("HiggsDecayMode");
  produces< reco::GenParticleCollection >("HiggsDecayTau1");
  produces< reco::GenParticleCollection >("HiggsDecayTau2");
  
  m_InputTag_HepMCProduct          = consumes<edm::HepMCProduct>          (inputTag_HepMCProduct);
  m_inputTag_GenJetCollection      = consumes<reco::GenJetCollection>     (inputTag_GenJetCollection);
  m_inputTag_GenParticleCollection = consumes<reco::GenParticleCollection>(inputTag_GenParticleCollection);  
  
  string outputFilename = pset.getUntrackedParameter<string>("outputFilename","DecayAnalyzer_Results.root");
  fOut = new TFile(outputFilename.c_str(),"RECREATE");
  
  m_EventCount  = new TH1D("EventCount","EventCount",1,-0.5,0.5); m_EventCount->SetDirectory(fOut);
  
  m_Parton_N    = new TH1D("Parton_N","Parton_N",11,-0.5,10.5);       m_Parton_N   ->SetDirectory(fOut);
  m_Tau_N       = new TH1D("Tau_N",   "Tau_N",   11,-0.5,10.5);       m_Tau_N      ->SetDirectory(fOut);
  m_Parton_Type = new TH1D("Parton_Type","Parton_Type", 2,-0.5, 2.5); m_Parton_Type->SetDirectory(fOut);
  m_Parton_Type->GetXaxis()->SetBinLabel(1,"Status 23");
  m_Parton_Type->GetXaxis()->SetBinLabel(2,"Status 24");
  
}

DecayAnalyzer::~DecayAnalyzer(){
  fOut->Write();
  delete m_EventCount;
}


void DecayAnalyzer::produce(edm::Event& iEvent,const edm::EventSetup& iSetup){
  
  // Counting the current event
  m_EventCount->Fill(0);
  
  Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByToken(m_inputTag_GenParticleCollection, genParticles);

  auto_ptr<reco::GenParticleCollection> vHiggsDecayTau1(new reco::GenParticleCollection);
  auto_ptr<reco::GenParticleCollection> vHiggsDecayTau2(new reco::GenParticleCollection);
  
  vector<const reco::GenParticle*> myTaus;
  int nTau     = 0;
  int TauToEle = 0;
  int TauToMuo = 0;
  int TauToHad = 0;
  
  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const reco::GenParticle & p = (*genParticles)[i];
    
    if(fabs(p.pdgId())==15){
//       cout << "Found a tau..." << endl;
      
      if(p.mother()->pdgId() == 25){
//         cout << "and it comes from a Higgs!" << endl;
        
        myTaus.push_back(&(*genParticles)[i]);
        
        bool decayEle = 0;
        bool decayMu  = 0;
        bool decayHad = 1;
        
        unsigned n = p.numberOfDaughters();
        for(size_t j = 0; j < n; ++ j) {
          
          const reco::GenParticle *d = (reco::GenParticle*) p.daughter( j );
          
          if     (nTau==0){vHiggsDecayTau1->push_back(*d);}
          else if(nTau==1){vHiggsDecayTau2->push_back(*d);}
          
//           cout << "Particle tau son id=" << d->pdgId() << endl;
          
          if(fabs(d->pdgId()) == 11){decayEle=1;decayHad=0;}
          if(fabs(d->pdgId()) == 13){decayMu =1;decayHad=0;}        
          
        }
        
        if(decayEle){TauToEle++;}
        if(decayMu) {TauToMuo++;}
        if(decayHad){TauToHad++;}
        nTau++;
      }
    }

  }

//   cout << "Number of decay particles Tau #1" << vHiggsDecayTau1->size() << endl;
//   cout << "Number of decay particles Tau #2" << vHiggsDecayTau2->size() << endl;
  
  auto_ptr< string > decayMode(new string);
  if     (TauToHad==2 && TauToEle==0 && TauToMuo==0){(*decayMode)="HadHad";}
  else if(TauToHad==1 && TauToEle==1 && TauToMuo==0){(*decayMode)="HadEle";}  
  else if(TauToHad==1 && TauToEle==0 && TauToMuo==1){(*decayMode)="HadMuo";}
  else if(TauToHad==0 && TauToEle==2 && TauToMuo==0){(*decayMode)="EleEle";}
  else if(TauToHad==0 && TauToEle==1 && TauToMuo==1){(*decayMode)="EleMuo";}
  else if(TauToHad==0 && TauToEle==0 && TauToMuo==2){(*decayMode)="MuoMuo";}
  else                                              {(*decayMode)="Error"; }

  m_Tau_N->Fill(myTaus.size());
  
  // and save the vectors
  iEvent.put(decayMode,      "HiggsDecayMode");
  iEvent.put(vHiggsDecayTau1,"HiggsDecayTau1");
  iEvent.put(vHiggsDecayTau2,"HiggsDecayTau2");
  
}
 
//define this as a plug-in
DEFINE_FWK_MODULE(DecayAnalyzer);
