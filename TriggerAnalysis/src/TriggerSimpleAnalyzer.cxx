#include "CMSSW_VBFHiggsTauTau/TriggerAnalysis/interface/TriggerSimpleAnalyzer.h"



#include <stdio.h>
#include <math.h>

using namespace std;
using namespace edm;

TriggerSimpleAnalyzer::TriggerSimpleAnalyzer(const edm::ParameterSet& pset){
  
  // Getting input tags
  InputTag inputTag_L1TMenu           = pset.getUntrackedParameter("inputTag_L1TMenu",          InputTag("L1GtTriggerMenuLite"));
  InputTag inputTag_L1GTReadoutRecord = pset.getUntrackedParameter("inputTag_L1GTReadoutRecord",InputTag("gtDigis"));
  InputTag inputTag_HLTResults        = pset.getUntrackedParameter("inputTag_HLTResults",       InputTag("TriggerResults",  "HLT"));
  
  // Creating tokens
  m_inputToken_GTReadout     = consumes<L1GlobalTriggerReadoutRecord>     (inputTag_L1GTReadoutRecord);
  m_inputToken_HLTResults    = consumes<edm::TriggerResults>              (inputTag_HLTResults);
  m_l1GtTriggerMenuLiteToken = consumes< L1GtTriggerMenuLite, edm::InRun >(inputTag_L1TMenu);
  
  m_hltPaths = pset.getParameter< vector<string> >("HLTPaths");
  
  string outputFilename = pset.getUntrackedParameter<string>("outputFilename","TriggerSimpleAnalyzerResults.root");
  
  m_fileOut = new TFile(outputFilename.c_str(),"RECREATE");
  
  // Making histograms
  m_hEventCount = new TH1D("EventCount","EventCount",1,0.5,1.5); 
  m_hEventCount->SetDirectory(m_fileOut);
  
  m_hL1TPathCount = new TH1D("L1TPathCount","L1TPathCount",128,-0.5,127.5);
  m_hL1TPathCount->SetDirectory(m_fileOut);
  
  m_hHLTPathCount = new TH1D("HLTPathCount","HLTPathCount",m_hltPaths.size(),-0.5,m_hltPaths.size()-0.5);
  m_hHLTPathCount->SetDirectory(m_fileOut);
  
  m_hHLTCorrelation = new TH2D("HLTCorrelation","HLTCorrelation",m_hltPaths.size(),-0.5,m_hltPaths.size()-0.5,m_hltPaths.size(),-0.5,m_hltPaths.size()-0.5);
  m_hHLTCorrelation->SetDirectory(m_fileOut);
  
  for(unsigned x=0; x<m_hltPaths.size(); x++){
    m_hHLTPathCount  ->GetXaxis()->SetBinLabel(x+1,m_hltPaths[x].c_str());
    m_hHLTCorrelation->GetXaxis()->SetBinLabel(x+1,m_hltPaths[x].c_str());
    m_hHLTCorrelation->GetYaxis()->SetBinLabel(x+1,m_hltPaths[x].c_str()); 
  }
  
}

TriggerSimpleAnalyzer::~TriggerSimpleAnalyzer(){
  
  m_fileOut->Write();
  delete m_hEventCount;
  
  delete m_hL1TPathCount;
  delete m_hHLTPathCount;
}


void TriggerSimpleAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){

  iRun.getByToken(m_l1GtTriggerMenuLiteToken, m_triggerMenuLite);

  m_alias.clear();
  
  int errorCode( 0 );
  
  
  // Filling Alias-Bit Map
  
  for(unsigned i=0; i<128; i++){
    
    cout << " => bit=" << i << " alias=" << m_triggerMenuLite->gtAlgorithmName(i,errorCode)->c_str() << endl;
    m_hL1TPathCount->GetXaxis()->SetBinLabel(i+1,m_triggerMenuLite->gtAlgorithmName(i,errorCode)->c_str() );
  }

}

void TriggerSimpleAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  m_hEventCount->Fill(1);
  
  // L1T Computations
  
  
  // Getting Final Decision Logic (FDL) Data from GT
  Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecordData;
  iEvent.getByToken(m_inputToken_GTReadout, gtReadoutRecordData);
 
  DecisionWord l1Word;
  
  if(gtReadoutRecordData.isValid()){
    
    const vector<L1GtFdlWord>& gtFdlVectorData = gtReadoutRecordData->gtFdlVector();
    
    // Running over FDL results to get which bits fired
    for(unsigned int a=0 ; a<gtFdlVectorData.size() ; a++){
      
      // Selecting the FDL that triggered
      if(gtFdlVectorData[a].bxInEvent()==0){
        
        const L1GtFdlWord* fdl = &(gtFdlVectorData[a]);
        l1Word = fdl->gtDecisionWord();
        
        for(int bit=0; bit<128; bit++){
          if(l1Word[bit]){m_hL1TPathCount->Fill(bit);}
        }
      }
    }
  }else{cout << "[TriggerSimpleAnalyzer::analyze] ERROR: GT Readout Record Data is not valid." << endl;}
  
  // HLT Computations
  edm::Handle<edm::TriggerResults> handleTriggerResults;
  iEvent.getByToken(m_inputToken_HLTResults, handleTriggerResults);
  
  //printFiredHLT(iEvent,handleTriggerResults);
  
  vector<bool> hltResults;
  
  for(unsigned i=0; i<m_hltPaths.size(); i++){
    if(testTrigger(iEvent,handleTriggerResults,m_hltPaths[i])){
      hltResults.push_back(true);
      m_hHLTPathCount->Fill(i);
    }else{
      hltResults.push_back(false);
    }
  }
  
  for(unsigned x=0;x<m_hltPaths.size();x++){
    for(unsigned y=0;y<m_hltPaths.size();y++){
      
      if(hltResults[x] && hltResults[y]){m_hHLTCorrelation->Fill(x,y);}
    }
  }
}

void TriggerSimpleAnalyzer::beginJob(){}

void TriggerSimpleAnalyzer::endJob(){}


void TriggerSimpleAnalyzer::endRun(edm::Run const&, edm::EventSetup const&){}

void TriggerSimpleAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){}

void TriggerSimpleAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){}

// TODO: This function can be optimised by caching the trigger names once per run (not once per event)
bool TriggerSimpleAnalyzer::testTrigger(const edm::Event& iEvent, edm::Handle<edm::TriggerResults> iHLT, string iTriggerName){
  
  bool Triggered = false;
  
  if( !iHLT.isValid() ) Triggered = false;
  int ntrigs = iHLT->size();
  if(ntrigs==0) Triggered = false;
  
  //check if trigger names are active
  const edm::TriggerNames &m_triggerNames = iEvent.triggerNames(*iHLT);
  //m_triggerNames.init( *iHLT.product() );
  
  for (int itrig = 0; itrig != ntrigs; ++itrig) 
  {
    if( !iHLT->wasrun(itrig) ) continue;
    if( iHLT->error(itrig) ) continue;
    string trigName = m_triggerNames.triggerName(itrig); 
    if( trigName.find(iTriggerName)==std::string::npos ) continue;
    if( !iHLT->accept(itrig) ) continue; 
    Triggered = true;
  }
  
  return Triggered;
  
}

void TriggerSimpleAnalyzer::printFiredHLT(const edm::Event& iEvent, edm::Handle<edm::TriggerResults> iHLT){
  
  int ntrigs = iHLT->size();
  
  const edm::TriggerNames &m_triggerNames = iEvent.triggerNames(*iHLT);
  
  for (int itrig = 0; itrig != ntrigs; ++itrig) 
  {
    if( !iHLT->wasrun(itrig) ) continue;
    if(  iHLT->error(itrig) )  continue;
    
    string trigName = m_triggerNames.triggerName(itrig); 
    
    if( !iHLT->accept(itrig) )        continue;
    
    cout << "Fired: " << trigName << endl;
    
  }
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerSimpleAnalyzer);