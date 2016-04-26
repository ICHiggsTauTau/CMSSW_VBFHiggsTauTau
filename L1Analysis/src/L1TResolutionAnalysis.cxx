#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TResolutionAnalysis.h"

using namespace std;

L1TResolutionAnalysis::L1TResolutionAnalysis(std::string name,TDirectory* baseDirector){

  m_name = name;
  m_dir  = baseDirector;
  
  m_GenMET                = new TH1D("GenMET","GenMET",500,0,500); m_GenMET->SetDirectory(m_dir);
  
  m_L1TETM                = new TH1D("L1TETM","L1TETM",500,0,500); m_L1TETM->SetDirectory(m_dir);
  m_L1THTM                = new TH1D("L1THTM","L1THTM",500,0,500); m_L1THTM->SetDirectory(m_dir);
  
  m_L1TETM_Resolution     = new TH1D    ("L1TETM_Resolution",    "L1TETM_Resolution",    200,-1.0,1.0); m_L1TETM_Resolution    ->SetDirectory(m_dir);
  m_L1TETM_ResolutionVsEt = new TProfile("L1TETM_ResolutionVsEt","L1TETM_ResolutionVsEt",100,   0,500); m_L1TETM_ResolutionVsEt->SetDirectory(m_dir);
}

L1TResolutionAnalysis::~L1TResolutionAnalysis(){

}

void L1TResolutionAnalysis::setVerbose(bool value){
  m_verbose = value;
}

void L1TResolutionAnalysis::run(trgfw::Event &iEvent){
  
  double valGENMET = iEvent.genInfo->genMet;
  m_GenMET->Fill(valGENMET);
  
  const vector<ic::L1TSum*>* l1tSums = iEvent.getByName< vector<ic::L1TSum*> >("l1t_sum");
  
  // Pointers for the sums
  ic::L1TSum *l1tETM = 0; 
  ic::L1TSum *l1tHTM = 0;
  
  // Finding the specific sums in the sums container
  for(unsigned i=0; i<l1tSums->size(); i++){
    
    ic::L1TSum *iSum = l1tSums->at(i);
    
    if     (iSum->sumType==ic::L1TSum::SumType::kMissingEt){l1tETM = iSum;}
    else if(iSum->sumType==ic::L1TSum::SumType::kMissingHt){l1tHTM = iSum;}
  }
  
  // Caching values for easier use
  double valL1TETM = l1tETM->vector().Et();
  double valL1THTM = l1tHTM->vector().Et();
  
  // Plotting L1T quatities
  m_L1TETM->Fill(valL1TETM);
  m_L1THTM->Fill(valL1THTM);
  
  // Calculating resolutions
  if(valGENMET>0){
    double resL1TMET = (valL1TETM-valGENMET)/valGENMET;
    
    m_L1TETM_Resolution->Fill(resL1TMET);
    m_L1TETM_ResolutionVsEt->Fill(valGENMET,resL1TMET);
  }
  
}
