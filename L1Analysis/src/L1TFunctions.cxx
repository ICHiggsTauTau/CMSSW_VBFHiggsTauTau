#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

#include "DataFormats/Math/interface/deltaR.h"

// L1T Objects
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TObject.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TEGamma.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TMuon.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TTau.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TJet.hh"

#include <iostream>

#include "TH1D.h"

using namespace std;

namespace trgfw {
  
  //##############################################
  // Filter collections
  //##############################################
  
  template <class Product>
  inline bool filterByLeading1(trgfw::Event &iEvent,std::string input,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsProduct(output)){return true;}
    
    // Get the input collection from the event
    vector<Product*>* colIn = iEvent.getByName< vector<Product*> >(input);
    if(colIn==0){
      cout << "[filterByLeading1] Failed too found input collection: " << input << endl;
      return false;
    }
    
    // Filter the collection and record the results
    vector<Product*> colOut;
    if(colIn->size()>0){
      colOut.push_back(colIn->at(0));
    }

    iEvent.addProduct< vector<Product*> >(output,colOut);
    
    return true;
  }
  
  template <class Product>
  inline bool filterByLeading2(trgfw::Event &iEvent,std::string input,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsProduct(output)){return true;}
    
    // Get the input collection from the event
    vector<Product*>* colIn = iEvent.getByName< vector<Product*> >(input);
    if(colIn==0){
      cout << "[filterByLeading1] Failed too found input collection: " << input << endl;
      return false;
    }
    
    // Filter the collection and record the results
    vector<Product*> colOut;
    if(colIn->size()>1){
      colOut.push_back(colIn->at(0));
      colOut.push_back(colIn->at(1));
    }
    
    iEvent.addProduct< vector<Product*> >(output,colOut);
    
    return true;
  }
  
  template <class Product>
  inline bool filterByMinPt(trgfw::Event &iEvent,std::string input,double pt,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsProduct(output)){return true;}
    
    // Get the input collection from the event
    vector<Product*>* colIn = iEvent.getByName< vector<Product*> >(input);
    if(colIn==0){
      cout << "[filterByMinPt] Failed too found input collection: " << input << endl;
      return false;
    }
    
    // Filter the collection and record the results
    vector<Product*>  colOut;
    for(auto obj=colIn->begin(); obj!=colIn->end(); obj++){
      if((*obj)->pt()>=pt){colOut.push_back(*obj);}
    }
    iEvent.addProduct< vector<Product*> >(output,colOut);

    return true;
  }
  
  template <class ProductRef, class ProductFil>
  inline bool filterByMinDR(trgfw::Event &iEvent,std::string colReference,std::string colFilter,double dr,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsProduct(output)){return true;}
    
    // Get the input collections from the event
    vector<ProductRef*> *colRef = iEvent.getByName< vector<ProductRef*> >(colReference);
    if(colRef==0){
      cout << "[filterByMinDR] Failed too found input reference collection: " << colReference << endl;
      return false;
    }
    vector<ProductFil*> *colFil = iEvent.getByName< vector<ProductFil*> >(colFilter);
    if(colFil==0){
      cout << "[filterByMinDR] Failed too found input to filter collection: " << colFilter << endl;
      return false;
    }
    
    vector<ProductFil*> colOut;
    for(auto objTest=colFil->begin(); objTest!=colFil->end(); objTest++){
      bool pass = true;
      for(auto objRef=colRef->begin(); objRef!=colRef->end(); objRef++){
        
        if(deltaR<ic::Candidate,ic::Candidate>(**objTest,**objRef)<dr){
          pass=false;
          break;
        }
      }
      if(pass){colOut.push_back(*objTest);}
    }
    
    iEvent.addProduct< vector<ProductFil*> >(output,colOut);
    return true;
  }
  
  template <class Product>
  inline bool pairFilter_vbfLikeSymmetric(trgfw::Event &iEvent,std::string input,bool oppSides,double dEta,double mjj,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsProduct(output)){return true;}
    
    // Get the input collection from the event
    vector<Product*>* colIn = iEvent.getByName< vector<Product*> >(input);
    if(colIn==0){
      cout << "[pairFilter_vbfLikeSymmetric] Failed to found input collection: " << input << endl;
      return false;
    }
    
    L1TObjectPairCollection colOut;
    
    // If we do not have at least 2 jets we stop here
    // NOTE: Protecting going into the cycle with less that 2 elements
    //       this will give trouble with the unsigned vs. signed int
    if(colIn->size()<2){return false;}
    
    for(unsigned a=0; a<colIn->size()-1; a++){
      Product *objA = colIn->at(a);
      for(unsigned b=a+1; b<colIn->size(); b++){
        Product *objB = colIn->at(b);
        
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
        colOut.push_back(thisPair);
      }
    }
    
    iEvent.addProduct<L1TObjectPairCollection>(output,colOut);
    return true;
  }
  
  template <class Product>
  inline bool pairFilter_vbfLikeAsymmetric(trgfw::Event &iEvent,std::string input,bool oppSides,double pt1,double pt2,double dEta,double mjj,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsProduct(output)){return true;}
    
    // Get the input collection from the event
    vector<Product*>* colIn = iEvent.getByName< vector<Product*> >(input);
    if(colIn==0){
      cout << "[pairFilter_vbfLikeAsymmetric] Failed to found input collection: " << input << endl;
      return false;
    }

    L1TObjectPairCollection colOut;

    // If we do not have at least 2 jets we stop here
    // NOTE: Protecting going into the cycle with less that 2 elements
    //       this will give trouble with the unsigned vs. signed int
    if(colIn->size()<2){return false;}
    
    for(unsigned a=0; a<colIn->size()-1; a++){
      Product *objA = colIn->at(a);
      for(unsigned b=a+1; b<colIn->size(); b++){
        Product *objB = colIn->at(b);
        
        if(oppSides){
          if(objA->eta()*objB->eta()>=0){continue;}
        }
        
        if(pt1>0 && pt2>0){
          if(pt1>objA->pt()){continue;}
          if(pt2>objB->pt()){continue;}
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
        colOut.push_back(thisPair);
      }
    }
    
    iEvent.addProduct<L1TObjectPairCollection>(output,colOut);
    return true;
  }
  
  template <class Product>
  inline bool pairFilter_vbfLikeAverage(trgfw::Event &iEvent,std::string input,bool oppSides,double pt,double dEta,double mjj,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsProduct(output)){return true;}
    
    // Get the input collection from the event
    vector<Product*>* colIn = iEvent.getByName< vector<Product*> >(input);
    if(colIn==0){
      cout << "[pairFilter_vbfLikeAverage] Failed to found input collection: " << input << endl;
      return false;
    }
    
    L1TObjectPairCollection colOut;
    
    // If we do not have at least 2 jets we stop here
    // NOTE: Protecting going into the cycle with less that 2 elements
    //       this will give trouble with the unsigned vs. signed int
    if(colIn->size()<2){return false;}
    
    for(unsigned a=0; a<colIn->size()-1; a++){
      Product *objA = colIn->at(a);
      for(unsigned b=a+1; b<colIn->size(); b++){
        Product *objB = colIn->at(b);
        
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
        colOut.push_back(thisPair);
      }
    }
    
    iEvent.addProduct<L1TObjectPairCollection>(output,colOut);
    return true;
  }
  
    
  template <class Product>
  inline bool pairFilter_vbfLike(trgfw::Event &iEvent,std::string input,double pt_1, double pt_2, double pt_avg, double pt_vec, double dEta,double mjj,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsProduct(output)){return true;}
    
    // Get the input collection from the event
    vector<Product*>* colIn = iEvent.getByName< vector<Product*> >(input);
    if(colIn==0){
      cout << "[pairFilter_vbfLike] Failed to found input collection: " << input << endl;
      return false;
    }
    
    L1TObjectPairCollection colOut;
    
    // If we do not have at least 2 jets we stop here
    // NOTE: Protecting going into the cycle with less that 2 elements
    //       this will give trouble with the unsigned vs. signed int
    if(colIn->size()<2){return false;}
    
    for(unsigned a=0; a<colIn->size()-1; a++){
      Product *objA = colIn->at(a);
      for(unsigned b=a+1; b<colIn->size(); b++){
        Product *objB = colIn->at(b);
        
        if(pt_1>0){
          double valPt1 = objA->pt();
          if(pt_1>valPt1){continue;}
        }
        
        if(pt_2>0){
          double valPt2 = objB->pt();
          if(pt_2>valPt2){continue;}
        }
        
        if(pt_avg>0){
          double valAvgPt = (objA->pt()+objB->pt())/2;
          if(pt_avg>valAvgPt){continue;}
        }
        
        if(pt_vec>0){
          double valVecPt = (objA->vector()+objB->vector()).pt();
          if(pt_vec>valVecPt){continue;}
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
        colOut.push_back(thisPair);
      }
    }
    
    iEvent.addProduct<L1TObjectPairCollection>(output,colOut);
    return true;
  }
  
  //
  
  template <class Product>
  inline bool tripletFilter_vbfLike(trgfw::Event &iEvent,std::string input,double pt_1, double pt_2, double pt_3, double pt_avg, double pt_vec, double dEta,double mjj,std::string output){
    
    // If this collection was already made return true
    if(iEvent.containsProduct(output)){return true;}
    
    // Get the input collection from the event
    vector<Product*>* colIn = iEvent.getByName< vector<Product*> >(input);
    if(colIn==0){
      cout << "[tripletFilter_vbfLike] Failed to found input collection: " << input << endl;
      return false;
    }
    
    L1TObjectPairCollection colOut;
    
    // If we do not have at least 2 jets we stop here
    // NOTE: Protecting going into the cycle with less that 2 elements
    //       this will give trouble with the unsigned vs. signed int
    if(colIn->size()<3){return false;}
    
    for(unsigned a=0; a<colIn->size()-2; a++){
      Product *objA = colIn->at(a);
      for(unsigned b=a+1; b<colIn->size()-1; b++){
        Product *objB = colIn->at(b);
        for(unsigned c=b+1; c<colIn->size(); c++){
          Product *objC = colIn->at(c);
          
          if(pt_1>0){
            double valPt1 = objA->pt();
            if(pt_1>valPt1){continue;}
          }
          
          if(pt_2>0){
            double valPt2 = objB->pt();
            if(pt_2>valPt2){continue;}
          }
          
          if(pt_3>0){
            double valPt3 = objC->pt();
            if(pt_3>valPt3){continue;}
          }
          
          if(pt_avg>0){
            double valAvgPt1 = (objA->pt()+objB->pt())/2;
            double valAvgPt2 = (objA->pt()+objC->pt())/2;
            double valAvgPt3 = (objB->pt()+objC->pt())/2;
            
            if(pt_avg>valAvgPt1 && pt_avg>valAvgPt2 && pt_avg>valAvgPt3){continue;}
          }
          
          if(pt_vec>0){
            double valVecPt1 = (objA->vector()+objB->vector()).pt();
            double valVecPt2 = (objA->vector()+objC->vector()).pt();
            double valVecPt3 = (objB->vector()+objC->vector()).pt();
            if(pt_vec>valVecPt1 && pt_vec>valVecPt2 && pt_vec>valVecPt3){continue;}
          }
          
          if(dEta>0){
            double valDEta1 = fabs(objA->eta()-objB->eta());
            double valDEta2 = fabs(objA->eta()-objC->eta());
            double valDEta3 = fabs(objB->eta()-objC->eta());
            if(dEta>valDEta1 && dEta>valDEta2 && dEta>valDEta3){continue;}
          }
          
          if(mjj>0){
            ROOT::Math::PtEtaPhiEVector vec1 = objA->vector() + objB->vector();
            ROOT::Math::PtEtaPhiEVector vec2 = objA->vector() + objC->vector();
            ROOT::Math::PtEtaPhiEVector vec3 = objB->vector() + objC->vector();
            if(mjj>vec1.mass() && mjj>vec2.mass() && mjj>vec3.mass()){continue;}
          }
          
          L1TObjectPair thisPair1(objA,objB);
          L1TObjectPair thisPair2(objA,objC);
          L1TObjectPair thisPair3(objB,objC);
          colOut.push_back(thisPair1);
          colOut.push_back(thisPair2);
          colOut.push_back(thisPair3);
        }
      }
    }
    
    iEvent.addProduct<L1TObjectPairCollection>(output,colOut);
    return true;
  }
  
  
  //##############################################
  // Test collections
  //##############################################
  
  template <class Product>
  inline bool testSize(trgfw::Event &iEvent,std::string input,unsigned minN){
    
    // Get the input collection from the event
    vector<Product*>* colIn = iEvent.getByName< vector<Product*> >(input);
    if(colIn==0){
      cout << "[testSize] Failed too found input collection: " << input << endl;
      return false;
    }
    
    // Testing collection size
    if(colIn->size()<minN){return false;}
    else                  {return true;}
  }
  
  inline bool pairTest_size(trgfw::Event &iEvent,std::string input,unsigned minN){
    
    // Get the input collection from the event
    L1TObjectPairCollection *colIn = iEvent.getByName<L1TObjectPairCollection>(input);
    if(colIn==0){
      cout << "[pairTest_size] Failed too found input collection: " << input << endl;
      return false;
    }
    
    // Testing collection size
    if(colIn->size()<minN){return false;}
    else                  {return true;}
  }
  
  inline bool testL1TObject1Pt(trgfw::Event &iEvent,std::string input,double pt){
    
    // Get the input collection from the event
    vector<ic::L1TObject>* colIn = iEvent.getByName< vector<ic::L1TObject> >(input);
    if(colIn==0){
      cout << "[testL1TObject1Pt] Failed to found input collection: " << input << endl;
      return false;
    }
    
    if(colIn->size()<1){return false;}
    
    if(colIn->at(0).pt()>=pt){return true;}
    return false;
    
  }
  
  inline bool testL1TObject2Pt(trgfw::Event &iEvent,std::string input,double pt0,double pt1){
    
    // Get the input collection from the event
    vector<ic::L1TObject>* colIn = iEvent.getByName< vector<ic::L1TObject> >(input);
    if(colIn==0){
      cout << "[testL1TObject2Pt] Failed to found input collection: " << input << endl;
      return false;
    }
    
    if(colIn->size()<2){return false;}
    
    if(colIn->at(0).pt()>=pt0 && colIn->at(1).pt()>=pt1){return true;}
    return false;
  }
  
  template <class Product>
  inline bool testVBFConditions(trgfw::Event &iEvent,std::string input,bool oppSides,double dEta,double mjj){
    
    // Get the input collection from the event
    vector<Product*>* colIn = iEvent.getByName< vector<Product*> >(input);
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
  
  // Sum algos
  inline bool testL1TMetEt(trgfw::Event &iEvent,double et){
    
    vector<ic::L1TSum*>* mySums = iEvent.getByName< vector<ic::L1TSum*> >("l1t_sum");
    
    for(unsigned i=0; i<mySums->size(); i++){
      
      ic::L1TSum *iSum = mySums->at(i);
      if(iSum->sumType==ic::L1TSum::SumType::kMissingEt && iSum->vector().Et()>=et){return true;}
    }
    return false;
  }
  
  inline bool testL1TMHTEt(trgfw::Event &iEvent,double et){
    
    vector<ic::L1TSum*>* mySums = iEvent.getByName< vector<ic::L1TSum*> >("l1t_sum");
    
    for(unsigned i=0; i<mySums->size(); i++){
      
      ic::L1TSum *iSum = mySums->at(i);
      if(iSum->sumType==ic::L1TSum::SumType::kMissingHt && iSum->vector().Et()>=et){return true;}
    }
    return false;
  }
  
  //##############################################
  //##############################################
  //##############################################
  //##############################################
  //##############################################
  /*
  // Single object conditions
  inline bool testL1TEGamma1Pt(trgfw::Event &iEvent,double pt1){
    
    if(iEvent.l1tEGammaCollection.size()<1){return false;}
    
    if(iEvent.l1tEGammaCollection[0].pt()>=pt1){return true;}
    return false;
  }
  
  inline bool testL1TMuon1Pt(trgfw::Event &iEvent,double pt1){
    
    if(iEvent.l1tMuonCollection.size()<1){return false;}
    
    if(iEvent.l1tMuonCollection[0].pt()>=pt1){return true;}
    return false;
  }
  
  inline bool testL1TTau1Pt(trgfw::Event &iEvent,double pt1){
    
    if(iEvent.l1tTauCollection.size()<1){return false;}
    
    if(iEvent.l1tTauCollection[0].pt()>=pt1){return true;}
    return false;
  }
  
  inline bool testL1TIsoTau1Pt(trgfw::Event &iEvent,double pt1){
    
    if(iEvent.l1tIsoTauCollection.size()<1){return false;}
    
    if(iEvent.l1tIsoTauCollection[0].pt()>=pt1){return true;}
    return false;
  }
  
  inline bool testL1TJet1Pt(trgfw::Event &iEvent,double pt1){
    
    if(iEvent.l1tJetCollection.size()<1){return false;}
    
    if(iEvent.l1tJetCollection[0].pt()>=pt1){return true;}
    return false;
  }
  
  // Double objects conditions
  
  inline bool testL1TTau2Pt(trgfw::Event &iEvent,double pt1, double pt2){
    
    if(iEvent.l1tTauCollection.size()<2){return false;}
    
    if(iEvent.l1tTauCollection[0].pt()>=pt1 && iEvent.l1tTauCollection[1].pt()>=pt2){return true;}
    return false;
  }
  
  inline bool testL1TIsoTau2Pt(trgfw::Event &iEvent,double pt1, double pt2){
    
    if(iEvent.l1tIsoTauCollection.size()<2){return false;}
    
    if(iEvent.l1tIsoTauCollection[0].pt()>=pt1 && iEvent.l1tIsoTauCollection[1].pt()>=pt2){return true;}
    return false;
  }
  
  inline bool testL1TJet2Pt(trgfw::Event &iEvent,double pt1, double pt2){
    
    if(iEvent.l1tJetCollection.size()<2){return false;}
    
    if(iEvent.l1tJetCollection[0].pt()>=pt1 && iEvent.l1tJetCollection[1].pt()>=pt2){return true;}
    return false;
  }
  
  inline bool testL1TJet2Pt_Mjj(trgfw::Event &iEvent,double pt, double mjj,TH1D *hMjj=0){
    
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
  
  inline bool testL1TJet2Pt_DEta_Mjj(trgfw::Event &iEvent,double pt,double eta, double mjj,TH1D *hDEta=0,TH1D *hMjj=0){
    
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
  inline bool testL1TMetEt(trgfw::Event &iEvent,double et){
    
    for(unsigned i=0; i<iEvent.l1tSumCollection.size(); i++){
      
      ic::L1TSum *iSum = &(iEvent.l1tSumCollection.at(i));
      if(iSum->sumType==ic::L1TSum::SumType::kMissingEt && iSum->vector().Et()>=et){return true;}
    }
    return false;
  }
  
  inline bool testL1TMHTEt(trgfw::Event &iEvent,double et){
    
    for(unsigned i=0; i<iEvent.l1tSumCollection.size(); i++){
      
      ic::L1TSum *iSum = &(iEvent.l1tSumCollection.at(i));
      if(iSum->sumType==ic::L1TSum::SumType::kMissingHt && iSum->vector().Et()>=et){return true;}
    }
    return false;
  }
  
*/

  
}
