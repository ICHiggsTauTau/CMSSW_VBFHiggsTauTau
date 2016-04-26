#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/AnalysisModule.h"

trgfw::AnalysisModule::AnalysisModule(){
  m_name = "";
  m_dir  = 0;
}

trgfw::AnalysisModule::AnalysisModule(std::string name,TDirectory* baseDirector){
  m_name = name;
  m_dir  = baseDirector;
}

trgfw::AnalysisModule::~AnalysisModule(){
  
}

std::string trgfw::AnalysisModule::getName(){
  return m_name;
}

void trgfw::AnalysisModule::beginJob(){
  
}

void trgfw::AnalysisModule::run(trgfw::Event&){
  
}

void trgfw::AnalysisModule::endJob(){
  
}