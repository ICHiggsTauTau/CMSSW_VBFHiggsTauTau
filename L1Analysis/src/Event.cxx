#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"

using namespace std;

trgfw::Event::Event(){
  genInfo=0;
  
}

trgfw::Event::~Event(){
  
}

bool trgfw::Event::containsProduct(std::string productName){
  
  if(m_products.find(productName)!=m_products.end()){return true;}
  else{return false;}
}

void trgfw::Event::printCollections(){
  
  printf("\n");
  printf("[Event::Event] ==> Events content:\n");
  
  for(auto it=m_products.begin(); it!=m_products.end(); it++){
    
    printf("[Event::Event]  * %s \n",it->first.c_str());
    
//     unsigned count=0;
//     for(auto obj=colObj->begin(); obj!=colObj->end(); obj++){
//       printf("[Event::Event] L1TObject #%u - et=%5.1f eta=%5.2f phi=%5.2f\n",count,obj->pt(),obj->eta(),obj->phi());
//       count++;
//     }
  }
 
}
