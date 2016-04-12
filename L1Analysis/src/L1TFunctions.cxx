#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

#include "DataFormats/Math/interface/deltaR.h"

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
  
  //##############################################
  // Filter collections
  //##############################################
  
  inline bool filterByLeading1(icTrg::Event &iEvent,std::string input,std::string output){
    
    // If this collection was already made return true
    if(iEvent.contains(output)){return true;}
    
    // Get the input collection from the event
    const vector<ic::L1TObject>* colIn = iEvent.get(input);
    if(colIn==0){
      cout << "[filterByLeading1] Failed too found input collection: " << input << endl;
      return false;
    }
    
    // Filter the collection and record the results
    vector<ic::L1TObject>* colOut = new vector<ic::L1TObject>();
    if(colIn->size()>0){
      colOut->push_back(colIn->at(0));
    }

    iEvent.add(output,colOut);
    
    return true;
  }
  
  inline bool filterByLeading2(icTrg::Event &iEvent,std::string input,std::string output){
    
    // If this collection was already made return true
    if(iEvent.contains(output)){return true;}
    
    // Get the input collection from the event
    const vector<ic::L1TObject>* colIn = iEvent.get(input);
    if(colIn==0){
      cout << "[filterByLeading1] Failed too found input collection: " << input << endl;
      return false;
    }
    
    // Filter the collection and record the results
    vector<ic::L1TObject>* colOut = new vector<ic::L1TObject>();
    if(colIn->size()>1){
      colOut->push_back(colIn->at(0));
      colOut->push_back(colIn->at(1));
    }
    
    iEvent.add(output,colOut);
    
    return true;
  }
  
  inline bool filterByMinPt(icTrg::Event &iEvent,std::string input,double pt,std::string output){
    
    // If this collection was already made return true
    if(iEvent.contains(output)){return true;}
    
    // Get the input collection from the event
    const vector<ic::L1TObject>* colIn = iEvent.get(input);
    if(colIn==0){
      cout << "[filterByMinPt] Failed too found input collection: " << input << endl;
      return false;
    }
    
    // Filter the collection and record the results
    vector<ic::L1TObject>* colOut = new vector<ic::L1TObject>();
    for(auto obj=colIn->begin(); obj!=colIn->end(); obj++){
      if(obj->pt()>=pt){colOut->push_back(*obj);}
    }
    iEvent.add(output,colOut);

    return true;
  }
  
  inline bool filterByMinDR(icTrg::Event &iEvent,std::string colReference,std::string colFilter,double dr,std::string output){
    
    // If this collection was already made return true
    if(iEvent.contains(output)){return true;}
    
    // Get the input collections from the event
    const vector<ic::L1TObject>* colRef = iEvent.get(colReference);
    if(colRef==0){
      cout << "[filterByMinDR] Failed too found input reference collection: " << colReference << endl;
      return false;
    }
    const vector<ic::L1TObject>* colFil = iEvent.get(colFilter);
    if(colFil==0){
      cout << "[filterByMinDR] Failed too found input to filter collection: " << colFilter << endl;
      return false;
    }
    
    vector<ic::L1TObject>* colOut = new vector<ic::L1TObject>();
    for(auto objTest=colFil->begin(); objTest!=colFil->end(); objTest++){
      bool pass = true;
      for(auto objRef=colRef->begin(); objRef!=colRef->end(); objRef++){
        
        if(deltaR<ic::Candidate,ic::Candidate>(*objTest,*objRef)<dr){
          pass=false;
          break;
        }
      }
      if(pass){colOut->push_back(*objTest);}
    }
    
    iEvent.add(output,colOut);
    return true;
  }
  
  inline bool pairFilter_vbfLikeSymmetric(icTrg::Event &iEvent,std::string input,bool oppSides,double dEta,double mjj,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsPair(output)){return true;}
    
    // Get the input collection from the event
    const vector<ic::L1TObject>* colIn = iEvent.get(input);
    if(colIn==0){
      cout << "[testVBFConditions] Failed too found input collection: " << input << endl;
      return false;
    }
    
    L1TObjectPairCollection* colOut = new L1TObjectPairCollection();
    
    for(unsigned a=0; a<colIn->size(); a++){
      const ic::L1TObject *objA = &(colIn->at(a));
      for(unsigned b=a+1; b<colIn->size(); b++){
        const ic::L1TObject *objB = &(colIn->at(b));
        
        if(oppSides){
          if(objA->eta()*objB->eta()>=0){continue;}
        }
        
        if(dEta>0){
          double valDEta = fabs(objA->eta()-objB->eta());
          if(dEta>valDEta){continue;}
        }
        
        if(mjj>0){
          ROOT::Math::PtEtaPhiEVector vec = objA->vector() + objB->vector();
          if(mjj>vec.mass()){continue;}
        }
        
        L1TObjectPair thisPair(objA,objB);
        colOut->push_back(thisPair);
      }
    }
    
    iEvent.addPairs(output,colOut);
    return true;
  }
  
  inline bool pairFilter_vbfLikeAverage(icTrg::Event &iEvent,std::string input,bool oppSides,double pt,double dEta,double mjj,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsPair(output)){return true;}
    
    // Get the input collection from the event
    const vector<ic::L1TObject>* colIn = iEvent.get(input);
    if(colIn==0){
      cout << "[testVBFConditions] Failed too found input collection: " << input << endl;
      return false;
    }
    
    L1TObjectPairCollection* colOut = new L1TObjectPairCollection();
    
    for(unsigned a=0; a<colIn->size(); a++){
      const ic::L1TObject *objA = &(colIn->at(a));
      for(unsigned b=a+1; b<colIn->size(); b++){
        const ic::L1TObject *objB = &(colIn->at(b));
        
        if(oppSides){
          if(objA->eta()*objB->eta()>=0){continue;}
        }
        
        if(pt>0){
          double valAvgPt = (objA->pt()+objB->pt())/2;
          if(pt>valAvgPt){continue;}
        }
        
        if(dEta>0){
          double valDEta = fabs(objA->eta()-objB->eta());
          if(dEta>valDEta){continue;}
        }
        
        if(mjj>0){
          ROOT::Math::PtEtaPhiEVector vec = objA->vector() + objB->vector();
          if(mjj>vec.mass()){continue;}
        }
        
        L1TObjectPair thisPair(objA,objB);
        colOut->push_back(thisPair);
      }
    }
    
    iEvent.addPairs(output,colOut);
    return true;
  }
  
  
  //##############################################
  // Test collections
  //##############################################
  
  inline bool testSize(icTrg::Event &iEvent,std::string input,unsigned minN){
    
    // Get the input collection from the event
    const vector<ic::L1TObject>* colIn = iEvent.get(input);
    if(colIn==0){
      cout << "[filterByMinPt] Failed too found input collection: " << input << endl;
      return false;
    }
    
    // Testing collection size
    if(colIn->size()<minN){return false;}
    else                  {return true;}
  }
  
  inline bool pairTest_size(icTrg::Event &iEvent,std::string input,unsigned minN){
    
    // Get the input collection from the event
    const L1TObjectPairCollection *colIn = iEvent.getPairs(input);
    if(colIn==0){
      cout << "[pairTest_size] Failed too found input collection: " << input << endl;
      return false;
    }
    
    // Testing collection size
    if(colIn->size()<minN){return false;}
    else                  {return true;}
  }
  
  inline bool testL1TObject1Pt(icTrg::Event &iEvent,std::string input,double pt){
    
    // Get the input collection from the event
    const vector<ic::L1TObject>* colIn = iEvent.get(input);
    if(colIn==0){
      cout << "[testL1TObject1Pt] Failed too found input collection: " << input << endl;
      return false;
    }
    
    if(colIn->size()<1){return false;}
    
    if(colIn->at(0).pt()>=pt){return true;}
    return false;
    
  }
  
  inline bool testL1TObject2Pt(icTrg::Event &iEvent,std::string input,double pt0,double pt1){
    
    // Get the input collection from the event
    const vector<ic::L1TObject>* colIn = iEvent.get(input);
    if(colIn==0){
      cout << "[testL1TObject1Pt] Failed too found input collection: " << input << endl;
      return false;
    }
    
    if(colIn->size()<2){return false;}
    
    if(colIn->at(0).pt()>=pt0 && colIn->at(1).pt()>=pt1){return true;}
    return false;
  }
  
  inline bool testVBFConditions(icTrg::Event &iEvent,std::string input,bool oppSides,double dEta,double mjj){
    
    // Get the input collection from the event
    const vector<ic::L1TObject>* colIn = iEvent.get(input);
    if(colIn==0){
      cout << "[testVBFConditions] Failed too found input collection: " << input << endl;
      return false;
    }
    
    if(colIn->size()<2){return false;}
    
    for(auto objA=colIn->begin(); objA!=colIn->end(); objA++){
      for(auto objB=objA+1; objB!=colIn->end(); objB++){
        
        if(oppSides){
          if(objA->eta()*objB->eta()>=0){continue;}
        }
        
        if(dEta>0){
          double valDEta = fabs(objA->eta()-objB->eta());
          if(dEta>valDEta){continue;}
        }
        
        if(mjj>0){
          ROOT::Math::PtEtaPhiEVector vec = objA->vector() + objB->vector();
          if(mjj>vec.mass()){continue;}
        }

        // If we get to here we found a pair that works
        return true;
      }
    }
    
    
    return false;
  }
  
  
  //##############################################
  //##############################################
  //##############################################
  //##############################################
  //##############################################
  
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
      if(iSum->sumType==ic::L1TSum::SumType::kMissingEt && iSum->vector().Et()>=et){return true;}
    }
    return false;
  }
  
  inline bool testL1TMHTEt(icTrg::Event &iEvent,double et){
    
    for(unsigned i=0; i<iEvent.l1tSumCollection.size(); i++){
      
      ic::L1TSum *iSum = &(iEvent.l1tSumCollection.at(i));
      if(iSum->sumType==ic::L1TSum::SumType::kMissingHt && iSum->vector().Et()>=et){return true;}
    }
    return false;
  }
  
}