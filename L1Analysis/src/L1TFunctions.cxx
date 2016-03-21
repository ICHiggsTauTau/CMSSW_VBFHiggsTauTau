#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

// L1T Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"

#include <iostream>

#include "TH1D.h"

using namespace std;

namespace icTrg {
  
  // Single object conditions
  
  inline bool testL1TEGamma1Pt(icTrg::Event &iEvent,double pt1){
    
    if(iEvent.l1tEGammaCollection.size()<1){return false;}
    
    if(iEvent.l1tEGammaCollection[0].pt()>=pt1){return true;}
    return false;
  }
  
  inline bool testL1TMuon1Pt(icTrg::Event &iEvent,double pt1){
    
    if(iEvent.l1tMuonCollection.size()<1){return false;}
    
    if(iEvent.l1tMuonCollection[0].pt()>=pt1){return true;}
    return false;
  }
  
  inline bool testL1TTau1Pt(icTrg::Event &iEvent,double pt1){
    
    if(iEvent.l1tTauCollection.size()<1){return false;}
    
    if(iEvent.l1tTauCollection[0].pt()>=pt1){return true;}
    return false;
  }
  
  inline bool testL1TIsoTau1Pt(icTrg::Event &iEvent,double pt1){
    
    if(iEvent.l1tIsoTauCollection.size()<1){return false;}
    
    if(iEvent.l1tIsoTauCollection[0].pt()>=pt1){return true;}
    return false;
  }
  
  inline bool testL1TJet1Pt(icTrg::Event &iEvent,double pt1){
    
    if(iEvent.l1tJetCollection.size()<1){return false;}
    
    if(iEvent.l1tJetCollection[0].pt()>=pt1){return true;}
    return false;
  }
  
  // Double objects conditions
  
  inline bool testL1TTau2Pt(icTrg::Event &iEvent,double pt1, double pt2){
    
    if(iEvent.l1tTauCollection.size()<2){return false;}
    
    if(iEvent.l1tTauCollection[0].pt()>=pt1 && iEvent.l1tTauCollection[1].pt()>=pt2){return true;}
    return false;
  }
  
  inline bool testL1TIsoTau2Pt(icTrg::Event &iEvent,double pt1, double pt2){
    
    if(iEvent.l1tIsoTauCollection.size()<2){return false;}
    
    if(iEvent.l1tIsoTauCollection[0].pt()>=pt1 && iEvent.l1tIsoTauCollection[1].pt()>=pt2){return true;}
    return false;
  }
  
  inline bool testL1TJet2Pt(icTrg::Event &iEvent,double pt1, double pt2){
    
    if(iEvent.l1tJetCollection.size()<2){return false;}
    
    if(iEvent.l1tJetCollection[0].pt()>=pt1 && iEvent.l1tJetCollection[1].pt()>=pt2){return true;}
    return false;
  }
  
  inline bool testL1TJet2Pt_Mjj(icTrg::Event &iEvent,double pt, double mjj,TH1D *hMjj=0){
    
    if(iEvent.l1tJetCollection.size()<2){return false;}
    
    vector<ic::L1TJet*> jets;
    for(unsigned i=0; i<iEvent.l1tJetCollection.size(); i++){
      ic::L1TJet *j = &(iEvent.l1tJetCollection[i]);
      if(j->pt()>=pt){jets.push_back(j);}
    }
    
    bool   pass   = false;
    double minMjj = 0;
    
    for(unsigned a=0; a<jets.size(); a++){
      ic::L1TJet *jetA = jets[a];
      for(unsigned b=0; b<jets.size(); b++){
        ic::L1TJet *jetB = jets[b];
        
        double px = jetA->vector().px() + jetB->vector().px();
        double py = jetA->vector().py() + jetB->vector().py();
        double pz = jetA->vector().pz() + jetB->vector().pz();
        double normaP = pow(px,2) + pow(py,2) + pow(pz,2);
        double iMjj = sqrt(pow(jetA->vector().energy()+jetB->vector().energy(),2) - normaP);
        if(iMjj>mjj){
          pass   = true;
          minMjj = iMjj;
        }
      }
    }
    
    if(hMjj){hMjj->Fill(minMjj);}
    
    return pass;
  }
  
  inline bool testL1TJet2Pt_DEta_Mjj(icTrg::Event &iEvent,double pt,double eta, double mjj,TH1D *hDEta=0,TH1D *hMjj=0){
    
    if(iEvent.l1tJetCollection.size()<2){return false;}
    
    vector<ic::L1TJet*> jets;
    for(unsigned i=0; i<iEvent.l1tJetCollection.size(); i++){
      ic::L1TJet *j = &(iEvent.l1tJetCollection[i]);
      if(j->pt()>=pt){jets.push_back(j);}
    }
    
    bool   pass    = false;
    double minMjj  = 0;
    double minDEta = 0;
    
    for(unsigned a=0; a<jets.size(); a++){
      ic::L1TJet *jetA = jets[a];
      for(unsigned b=0; b<jets.size(); b++){
        ic::L1TJet *jetB = jets[b];
        
        double iDEta = fabs(jetA->eta()-jetB->eta());
        
        double px = jetA->vector().px() + jetB->vector().px();
        double py = jetA->vector().py() + jetB->vector().py();
        double pz = jetA->vector().pz() + jetB->vector().pz();
        double normaP = pow(px,2) + pow(py,2) + pow(pz,2);
        double iMjj = sqrt(pow(jetA->vector().energy()+jetB->vector().energy(),2) - normaP);

        if(iDEta>eta && iMjj>mjj){
          pass    = true;
          minMjj  = iMjj;
          minDEta = iDEta;
        }
      }
    }
    
    if(hMjj) {hMjj ->Fill(minMjj);}
    if(hDEta){hDEta->Fill(minDEta);}
      
    return pass;
  }
  
  // Sum algos
  inline bool testL1TMetEt(icTrg::Event &iEvent,double et){
    
    for(unsigned i=0; i<iEvent.l1tSumCollection.size(); i++){
      
      ic::L1TSum *iSum = &(iEvent.l1tSumCollection.at(i));
      if(iSum->sumType==ic::L1TSum::SumType::kMissingEt && iSum->et>=et){return true;}
    }
    return false;
  }
  
  inline bool testL1TMHTEt(icTrg::Event &iEvent,double et){
    
    for(unsigned i=0; i<iEvent.l1tSumCollection.size(); i++){
      
      ic::L1TSum *iSum = &(iEvent.l1tSumCollection.at(i));
      if(iSum->sumType==ic::L1TSum::SumType::kMissingHt && iSum->et>=et){return true;}
    }
    return false;
  }
  
}