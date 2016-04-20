#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TResolutionAnalysis.h"

using namespace std;

L1TResolutionAnalysis::L1TResolutionAnalysis(TDirectory* baseDirector){
  cout << "Constructor" << endl;
  m_GenMET            = new TH1D("GenMET",           "GenMET"           ,500,   0,500); m_GenMET           ->SetDirectory(baseDirector);
  m_L1TETM            = new TH1D("L1TETM",           "L1TETM"           ,500,   0,500); m_L1TETM           ->SetDirectory(baseDirector);
  m_L1TETM_Resolution = new TH1D("L1TETM_Resolution","L1TETM_Resolution",200,-1.0,1.0); m_L1TETM_Resolution->SetDirectory(baseDirector);
}

L1TResolutionAnalysis::~L1TResolutionAnalysis(){
//   delete m_GenMET;
//   delete m_L1TETM;
//   delete m_L1TETM_Resolution;
}

void L1TResolutionAnalysis::setVerbose(bool value){
  m_verbose = value;
}

bool L1TResolutionAnalysis::run(icTrg::Event &iEvent){
  
  double genMET = iEvent.genInfo->genMet;
  m_GenMET->Fill(genMET);
  
  double evL1TMET = 0.;
  const vector<ic::L1TObject>* l1tSums = iEvent.get("l1t_sum");
  for(unsigned i=0; i<l1tSums->size(); i++){
    
    const ic::L1TSum *iSum = (const ic::L1TSum*) &(l1tSums->at(i));
    
    if(iSum->sumType==ic::L1TSum::SumType::kMissingEt){
      evL1TMET = iSum->vector().Et();
    }
  }
  
  m_L1TETM->Fill(evL1TMET);
  
  if(genMET>0){
    double diffMET   = evL1TMET-genMET;
    double resL1TMET = diffMET/genMET;
    m_L1TETM_Resolution->Fill(resL1TMET);
  }
  
  
  return true;
}

