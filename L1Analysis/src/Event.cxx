#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

using namespace std;

icTrg::Event::Event(){
  genInfo=0;
  
}

icTrg::Event::~Event(){}

double icTrg::Event::getDijetMaxMjj(double pt){
  
  map<double,double>::iterator entry = m_dijet_maxMjj.find(pt);
  
  if(entry==m_dijet_maxMjj.end()){
    
    double maxMjj=0;
    
    vector<ic::L1TJet*> jets;
    for(unsigned i=0; i<l1tJetCollection.size(); i++){
      ic::L1TJet *j = &(l1tJetCollection[i]);
      if(j->pt()>=pt){jets.push_back(j);}
    }
    
    for(unsigned a=0; a<jets.size(); a++){
      ic::L1TJet *jetA = jets[a];
      for(unsigned b=0; b<jets.size(); b++){
        ic::L1TJet *jetB = jets[b];
        
        double px = jetA->vector().px() + jetB->vector().px();
        double py = jetA->vector().py() + jetB->vector().py();
        double pz = jetA->vector().pz() + jetB->vector().pz();
        double normaP = pow(px,2) + pow(py,2) + pow(pz,2);
        double iMjj = sqrt(pow(jetA->vector().energy()+jetB->vector().energy(),2) - normaP);
        if(iMjj>maxMjj){maxMjj = iMjj;}
      }
    }
    
    m_dijet_maxMjj[pt] = maxMjj;
    return maxMjj;
    
  }else{
    return entry->second;
  }
  
}
