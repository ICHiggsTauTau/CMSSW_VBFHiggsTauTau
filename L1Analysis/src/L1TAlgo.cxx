#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgo.h"

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

L1TAlgo::L1TAlgo() :
  m_verbose(false){
  init();
  if(m_verbose){printf("[L1TAlgo::L1TAlgo] Method called\n");}
}

L1TAlgo::L1TAlgo(string name,TDirectory* baseBirectory){
  
  init();
  string basePath = baseBirectory->GetPath();
  basePath = basePath.substr(basePath.find(':')+2,basePath.size()-1); // Removing file name for the path
  results  = new trgfw::L1TAlgoResults(Form("%s/%s",basePath.c_str(),name.c_str()),name);
  
  if(m_verbose){printf("[L1TAlgo::L1TAlgo] Method called. (name=%s)\n",name.c_str());}
  m_name = name;
}

L1TAlgo::~L1TAlgo(){
  
  if(results){delete results;}
}

void L1TAlgo::setVerbose(bool value){
  m_verbose = value;
}

std::string L1TAlgo::getName(){
  return m_name;
}

void L1TAlgo::init(){
  
  m_verbose = false;
  if(m_verbose){printf("[L1TAlgo::init] Method called\n");}
  
  results = 0;
  m_name  = "";
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
  
  results->fill(iEvent);
  //plots  .fill(iEvent);
  
  return true;
}



