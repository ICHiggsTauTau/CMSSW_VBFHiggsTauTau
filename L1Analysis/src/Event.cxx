#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

using namespace std;

icTrg::Event::Event(){
  genInfo=0;
  
}

icTrg::Event::~Event(){
  
  for(auto i=m_collections.begin(); i!=m_collections.end(); i++){
    delete i->second;
  }
  
  for(auto i=m_pairs.begin(); i!=m_pairs.end(); i++){
    delete i->second;
  }
}

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


const std::vector<ic::L1TObject>* icTrg::Event::get(std::string name){
  
  auto p = m_collections.find(name);
  if(p!=m_collections.end()){
    return p->second;
  }else{
    return 0;
  }
}

bool icTrg::Event::contains(std::string name){
  
  if(m_collections.find(name)!=m_collections.end()){return true;}
  else{return false;}
}

bool icTrg::Event::add(std::string name,std::vector<ic::L1TObject> *objs){
  
  if(this->contains(name)){return false;}
  
  m_collections[name] = objs;
  return true;
}

void icTrg::Event::printCollections(){
  
  printf("\n");
  printf("[Event::Event] => Printing all contained L1TObjectsCollection\n");
  for(auto it=m_collections.begin(); it!=m_collections.end(); it++){
    
    vector<ic::L1TObject>* colObj = it->second;
    
    printf("\n");
    printf("[Event::Event] ===== List of %s : %lu =====\n",it->first.c_str(),colObj->size());
    
    unsigned count=0;
    for(auto obj=colObj->begin(); obj!=colObj->end(); obj++){
      printf("[Event::Event] L1TObject #%u - et=%5.1f eta=%5.2f phi=%5.2f\n",count,obj->pt(),obj->eta(),obj->phi());
      count++;
    }
  }
}

bool icTrg::Event::containsPair(std::string name){
  
  if(m_pairs.find(name)!=m_pairs.end()){return true;}
  else{return false;}
}

bool icTrg::Event::addPairs(std::string name,icTrg::L1TObjectPairCollection *pairs){
  
  if(this->containsPair(name)){return false;}
  
  m_pairs[name] = pairs;
  return true;
}

const icTrg::L1TObjectPairCollection* icTrg::Event::getPairs(std::string name){
  
  auto p = m_pairs.find(name);
  if(p!=m_pairs.end()){
    return p->second;
  }else{
    return 0;
  }
}
