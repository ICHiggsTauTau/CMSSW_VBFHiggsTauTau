#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgo.h"

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

L1TAlgo::L1TAlgo() : plots("",0) {
  init();
  if(m_verbose){printf("[L1TAlgo::L1TAlgo] Method called\n");}
}

L1TAlgo::L1TAlgo(string name, TDirectory* baseBirectory) : plots(name,baseBirectory){
  init();
  if(m_verbose){printf("[L1TAlgo::L1TAlgo] Method called. (name=%s)\n",name.c_str());}
  m_name           = name;
}

L1TAlgo::~L1TAlgo(){}

void L1TAlgo::setVerbose(bool value){
  m_verbose = value;
}

std::string L1TAlgo::getName(){
  return m_name;
}

void L1TAlgo::init(){
  
  m_verbose = false;
  if(m_verbose){printf("[L1TAlgo::init] Method called\n");}
  
  m_name = "";
}

void L1TAlgo::addCondition(std::function<bool(trgfw::Event &iEvent)> function){
  m_conditions.push_back(function);
}

bool L1TAlgo::run(trgfw::Event &iEvent){
  
  if(m_verbose){printf("[L1TAlgo::run] Method called for L1TAlgo name=%s\n",m_name.c_str());}
  
  for(unsigned i=0; i<m_conditions.size(); i++){
    if(m_verbose){printf("[L1TAlgo::run] Testing condition #%u : ",i);}
    if(!(m_conditions[i])(iEvent)){
      if(m_verbose){printf("failed\n");}
      return false;
    }else{
      if(m_verbose){printf("true\n");}
    }
  }
  
  plots.fill(iEvent);
  
  return true;
}



