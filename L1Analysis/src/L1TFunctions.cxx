#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

// L1T Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"

#include <iostream>

using namespace std;

namespace icTrg {
  
  inline bool testL1TTauPt(icTrg::Event &iEvent,double pt1, double pt2){
    
    if(iEvent.l1tTauCollection.size()<2){return false;}
    
    if(iEvent.l1tTauCollection[0].pt()>pt1 && iEvent.l1tTauCollection[0].pt()>pt2){return true;}
    return false;
  }
  
  
  inline bool testL1TJetPt(icTrg::Event &iEvent,double pt1, double pt2){
    
    if(iEvent.l1tJetCollection.size()<2){return false;}
    
    if(iEvent.l1tJetCollection[0].pt()>pt1 && iEvent.l1tJetCollection[0].pt()>pt2){return true;}
    return false;
  }
  
  inline bool testL1TJetPt_Mjj(icTrg::Event &iEvent,double pt, double mjj){
    
    if(iEvent.l1tJetCollection.size()<2){return false;}
    
    vector<ic::L1TJet*> jets;
    for(unsigned i=0; i<iEvent.l1tJetCollection.size(); i++){
      ic::L1TJet *j = &(iEvent.l1tJetCollection[i]);
      if(j->pt()>pt){jets.push_back(j);}
    }
    
    double minMjj = 0;
    for(unsigned a=0; a<jets.size(); a++){
      ic::L1TJet *jetA = jets[a];
      for(unsigned b=0; b<jets.size(); b++){
        ic::L1TJet *jetB = jets[b];
        
        double px = jetA->vector().px() + jetB->vector().px();
        double py = jetA->vector().py() + jetB->vector().py();
        double pz = jetA->vector().pz() + jetB->vector().pz();
        double normaP = pow(px,2) + pow(py,2) + pow(pz,2);
        double mjj = sqrt(pow(jetA->vector().energy()+jetB->vector().energy(),2) - normaP);
        if(mjj>minMjj){minMjj=mjj;}
      }
    }
    
    if(minMjj>mjj){return true;}
    
    return false;
  }
  
  
}