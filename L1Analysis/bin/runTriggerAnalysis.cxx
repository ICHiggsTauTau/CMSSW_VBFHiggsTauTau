// CMSSW includes
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"

// CMSSW_VBFHiggsTauTau includes
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoAnalysis.h"
#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/interface/MicroGenParticle.h"
#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/interface/VBFHiggsGenAnalysisDataFormat.h"

// ICHiggsTauTau Objects
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TObject.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TEGamma.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TMuon.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TTau.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TJet.hh"

// ROOT includes
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "TBranchElement.h"

// C++ STD includes
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <map>
#include <vector>
#include <functional>

typedef std::vector<ic::L1TEGamma*> L1TEGammaPointerCollection;
typedef std::vector<ic::L1TMuon*>   L1TMuonPointerCollection;
typedef std::vector<ic::L1TTau*>    L1TTauPointerCollection;
typedef std::vector<ic::L1TJet*>    L1TJetPointerCollection;

using namespace std;

struct greater_ICCandidate{
  bool operator() (ic::Candidate *a,ic::Candidate *b) {
    return (a->pt() > b->pt());
  }
};

struct ProgramOptions{
  ProgramOptions(){
    verbose        = false;
    inputType      = "file";
    input          = "input.root";
    maxEvents      = -1;
    jobType        = "mc";
    decay          = 0;
    outputFilename = "L1Analysis.root";
    
    doAnalysisL1TAlgoScan   = false;
    doAnalysisL1TResolution = false;
  }

  bool             verbose;
  string           inputType;
  string           input;
  vector<unsigned> runs;
  Long64_t         maxEvents;
  string           jobType;
  unsigned char    decay;
  string           outputFilename;
  
  // Analysis options
  bool doAnalysisL1TAlgoScan;
  bool doAnalysisL1TResolution;
  
};

void printHelpMessage(){
  
  cerr << "Usage: vbftautau_runTriggerAnalysis [commands]"                                     << endl;
  cerr << " --verbose         "                                                                << endl;
  cerr << " --inputType TYPE          options: file, filelist"                                 << endl;
  cerr << " --input INPUT"                                                                     << endl;
  cerr << " --runs RUNS               options: comma separated"                                << endl;
  cerr << " --maxEvents N"                                                                     << endl;
  cerr << " --jobType TYPE            options: mc, data"                                       << endl;
  cerr << " --decay DECAY             options: EleEle, EleMuo, EleHad, MuoMuo, MuoHad, HadHad" << endl;
  cerr << endl;
  cerr << "ANALYSIS OPTIONS:" << endl;
  cerr << " --doAnalysisL1TAlgoScan"                                                           << endl;
  cerr << " --doAnalysisL1TResolution"                                                         << endl;
  cerr << " --outputFilename NAME"                                                             << endl;
}

bool processArgs(int argc, char* argv[], ProgramOptions &opt){
  
  // Check the number of parameters
  if (argc < 2) {
    printHelpMessage();
    return 0;
  }
  
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if     (arg == "--verbose")                {opt.verbose  = true;}
    else if(arg == "--doAnalysisL1TResolution"){opt.doAnalysisL1TResolution = true;}
    else if(arg == "--doAnalysisL1TAlgoScan")  {opt.doAnalysisL1TAlgoScan   = true;}
    else if(arg == "--inputType"){
      if(i+1<argc){i++; opt.inputType = argv[i];} 
      else{
        printHelpMessage();
        return 0;
      }
    }else if(arg == "--input"){
      if(i+1<argc){i++; opt.input = argv[i];}
      else{
        printHelpMessage();
        return 0;
      }
    }else if(arg == "--maxEvents"){
      if(i+1<argc){i++; opt.maxEvents = atoi(argv[i]);}
      else{
        printHelpMessage();
        return 0;
      }
    }else if(arg == "--runs"){
      if(i+1<argc){
        i++; 
        
        std::stringstream inVar(argv[i]);
        int i;
        while(inVar >> i){
          opt.runs.push_back(i);
          if(inVar.peek() == ','){inVar.ignore();}
        }
      }else{
        printHelpMessage();
        return 0;
      }
    }else if(arg == "--maxEvents"){
      if(i+1<argc){i++; opt.maxEvents = atoi(argv[i]);}
      else{
        printHelpMessage();
        return 0;
      }
    }else if(arg == "--jobType"){
      if(i+1<argc){i++; opt.jobType = argv[i];}
      else{
        printHelpMessage();
        return 0;
      }
    }else if(arg == "--decay"){
      if(i+1<argc){i++; 
        if     (std::string(argv[i]) == "EleEle"){opt.decay = VBFHiggs::HiggsDecay::EleEle;}
        else if(std::string(argv[i]) == "EleMuo"){opt.decay = VBFHiggs::HiggsDecay::EleMuo;}
        else if(std::string(argv[i]) == "EleHad"){opt.decay = VBFHiggs::HiggsDecay::EleHad;}
        else if(std::string(argv[i]) == "MuoMuo"){opt.decay = VBFHiggs::HiggsDecay::MuoMuo;}
        else if(std::string(argv[i]) == "MuoHad"){opt.decay = VBFHiggs::HiggsDecay::MuoHad;}
        else if(std::string(argv[i]) == "HadHad"){opt.decay = VBFHiggs::HiggsDecay::HadHad;}        
      }
      else{
        printHelpMessage();
        return 0;
      }
    }else if(arg == "--outputFilename"){
      if(i+1<argc){i++; opt.outputFilename = argv[i];}
      else{
        printHelpMessage();
        return 0;
      }
    }
  }
  
  return 1;
}


int main(int argc, char* argv[]){
  
  ProgramOptions options;
  if(!processArgs(argc, argv, options)){return -1;}
  
  cout << "===== program options ===== " << endl;
  cout << "verbose        = " << options.verbose        << endl;
  cout << "inputType      = " << options.inputType      << endl;
  cout << "input          = " << options.input          << endl;
  cout << "maxEvents      = " << options.maxEvents      << endl;
  cout << "runs           = ";
  for(unsigned i=0; i<options.runs.size(); i++){
    if(i!=0){cout << ",";}
    cout << options.runs[i];
  }
  cout << endl;
  cout << "jobType                 = " << options.jobType        << endl;
  cout << "decay                   = " << options.decay          << endl;  
  cout << "doAnalysisL1TResolution = " << options.doAnalysisL1TResolution << endl;
  cout << "doAnalysisL1TAlgoScan   = " << options.doAnalysisL1TAlgoScan << endl;
  cout << "outputFilename          = " << options.outputFilename << endl;
  cout << endl;
  
  
  // Input file processing
  vector<string> inputFileCollection;
  if(options.inputType=="file"){    

    stringstream ss(options.input);
    string token;
    
    while( ss.good() ){
      string substr;
      getline( ss, substr, ',' );
      inputFileCollection.push_back(substr);
    }
      
  }else if(options.inputType=="filelist"){
    string line;
    ifstream myfile (options.input);
    if (myfile.is_open()){
      while ( getline(myfile,line) ){
        inputFileCollection.push_back(line);
      }
      myfile.close();
    }
  }
  
  
  // Making output file
  TFile *fileOut = new TFile(options.outputFilename.c_str(),"RECREATE");
  
  // Making the analysis sequence
  vector<trgfw::AnalysisModule*> moduleSequence;
  
  if(options.doAnalysisL1TResolution && options.jobType=="mc"){
    TDirectory *dir = fileOut->mkdir("L1TResolutions");
    
    L1TResolutionAnalysis *myL1TRes = new L1TResolutionAnalysis("L1TResolutions",dir);
    myL1TRes->setVerbose(options.verbose);
    
    moduleSequence.push_back(myL1TRes);
  }
  
  L1TAlgoAnalysis *myAnalysis = 0;
  if(options.doAnalysisL1TAlgoScan){
    TDirectory *dir = fileOut->mkdir("L1TAlgoScan");

    myAnalysis = new L1TAlgoAnalysis("L1TAlgoAnalysis",dir);
    myAnalysis->setVerbose                (options.verbose);
    myAnalysis->setDoSingleObjectsAnalysis(true);
    if(options.jobType=="mc"){
      myAnalysis->setDoGenAnalysis(true);
    }
    
    moduleSequence.push_back(myAnalysis);
  }
  
  cout << "===== Open File ===== " << endl;
  TChain treeEvent      ("l1EventTree/L1EventTree");
  TChain treeL1Upgrade  ("l1UpgradeEmuTree/L1UpgradeTree");
  TChain treeGenAnalysis("decayAnalyzer/VBFHiggsGenAnalysisTree");
  cout << endl;
  
  L1Analysis::L1AnalysisEventDataFormat     *productL1Event   = 0;
  L1Analysis::L1AnalysisL1UpgradeDataFormat *productL1Upgrade = 0;
  VBFHiggs::GenAnalysisDataFormat           *genInfo          = 0;
  
  productL1Event = new L1Analysis::L1AnalysisEventDataFormat();
  treeEvent.SetBranchAddress("Event",&productL1Event);
  
  productL1Upgrade= new L1Analysis::L1AnalysisL1UpgradeDataFormat();
  treeL1Upgrade.SetBranchAddress("L1Upgrade",&productL1Upgrade);
  
  if(options.jobType=="mc"){
    genInfo = new VBFHiggs::GenAnalysisDataFormat();
    treeGenAnalysis.SetBranchAddress("GenAnalysis",&genInfo);
  }
  
  for(unsigned i=0; i<inputFileCollection.size(); i++){
     cout << "Input file #" << i << " - " << inputFileCollection[i] << endl;
     treeEvent      .Add(inputFileCollection[i].c_str());
     treeL1Upgrade  .Add(inputFileCollection[i].c_str());
     if(options.jobType=="mc"){
       treeGenAnalysis.Add(inputFileCollection[i].c_str());
     }
   }
  
  
  cout << "===== Tree Summary ===== " << endl;
  Long64_t entries_VBFHiggsToTauTau = treeEvent.GetEntries();
  cout << "Number of entries: " << entries_VBFHiggsToTauTau << endl;
  cout << endl;
  
  for(unsigned i=0; i<moduleSequence.size(); i++){
    moduleSequence[i]->beginJob();
  }
  
  // If maxEvents if -1 this means run over all events
  if(options.maxEvents==-1){options.maxEvents=entries_VBFHiggsToTauTau;}
  
  Long64_t eventsEffective=0;
  
  for(Long64_t ev=0; ev<entries_VBFHiggsToTauTau && ev<options.maxEvents; ev++){
    
    if(options.verbose){
      printf("\n\n");
      printf("[main] ##########################################\n");
      printf("[main] #### Processing event %14lld #####\n",ev);
      printf("[main] ##########################################\n");
    }
    
    
    if(!(ev % 10000)){cout << "[main] Processing events #" << ev << endl;}
    
    
    if(options.verbose){printf("[main] Loading L1 Event Tree\n");}
    treeEvent.LoadTree(ev);
    treeEvent.GetEntry(ev);
    
    if(options.verbose){printf("[main] Loading L1 Upgrade Tree\n");}
    treeL1Upgrade.LoadTree(ev);
    treeL1Upgrade.GetEntry(ev);
    
    // Loading trees for mc
    if(options.jobType=="mc"){
      
      if(options.verbose){printf("[main] Loading Generator Tree\n");}
      treeGenAnalysis.LoadTree(ev);
      treeGenAnalysis.GetEntry(ev);
      
      // Filtering on decays
      if(options.decay!=0){
        if(options.verbose){printf("[main] Decay: %u\n",genInfo->higgs_decayType);}
        if(options.decay!=genInfo->higgs_decayType){continue;}
      }
    }
    
    // Checking if option runs is set
    if(options.runs.size()){
      bool passRun = false;
      
      // Check if this event belong to the selected runs
      for(unsigned i=0; i<options.runs.size(); i++){
        if(options.runs[i]==productL1Event->run){passRun=true;}
      }
      
      if(!passRun){continue;}
    }
    
    eventsEffective++;
    
    if(options.doAnalysisL1TAlgoScan){
      myAnalysis->incrementEventTotal();
    }
    
    trgfw::Event myEvent;

    if(options.jobType=="mc"){
      myEvent.genInfo = genInfo;
    }else{
      myEvent.genInfo = 0;
    }
    
    if(options.verbose){
      printf("[main] L1T EGamma: %u\n",productL1Upgrade->nEGs);
      printf("[main] L1T Muon  : %u\n",productL1Upgrade->nMuons);
      printf("[main] L1T Tau   : %u\n",productL1Upgrade->nTaus);
      printf("[main] L1T Jets  : %u\n",productL1Upgrade->nJets);
    }
    
    vector<ic::L1TEGamma*> l1tEG;
    vector<ic::L1TEGamma*> l1tIsoEG;
    unsigned short int nL1TEGamma = productL1Upgrade->nEGs;
    for(unsigned i=0; i<nL1TEGamma; i++){
      
      if(productL1Upgrade->egBx[i] != 0){continue;}
      
      ic::L1TEGamma *thisEG = new ic::L1TEGamma;
      thisEG->set_pt    (productL1Upgrade->egEt    [i]);
      thisEG->set_eta   (productL1Upgrade->egEta   [i]);
      thisEG->set_phi   (productL1Upgrade->egPhi   [i]);
      thisEG->set_energy(productL1Upgrade->egEnergy[i]);
      thisEG->isolation = productL1Upgrade->egIso[i];
      
      //myEvent.l1tEGammaCollection.push_back(thisEG);
      
      l1tEG.push_back(thisEG);
      if(thisEG->isolation!=0){
        l1tIsoEG.push_back(thisEG);
      }
    }
    
    vector<ic::L1TMuon*> l1tMuons;
    vector<ic::L1TMuon*> l1tIsoMuons;
    unsigned short int nL1TMuon = productL1Upgrade->nMuons;
    for(unsigned i=0; i<nL1TMuon; i++){
      
      if(productL1Upgrade->muonBx[i] != 0){continue;}
      
      ic::L1TMuon *thisMuon = new ic::L1TMuon();
      thisMuon->set_pt    (productL1Upgrade->muonEt    [i]);
      thisMuon->set_eta   (productL1Upgrade->muonEta   [i]);
      thisMuon->set_phi   (productL1Upgrade->muonPhi   [i]);
      thisMuon->set_energy(productL1Upgrade->muonEnergy[i]);
      thisMuon->isolation = productL1Upgrade->muonIso  [i];
      //myEvent.l1tMuonCollection.push_back(thisMuon);
      
      l1tMuons.push_back(thisMuon);
      if(thisMuon->isolation!=0){
        l1tIsoMuons.push_back(thisMuon);
      }
    }
    
    vector<ic::L1TTau*> l1tTaus;
    vector<ic::L1TTau*> l1tIsoTaus;
    unsigned short int nL1TTaus = productL1Upgrade->nTaus;
    for(unsigned i=0; i<nL1TTaus; i++){
      
      if(productL1Upgrade->tauBx[i] != 0){continue;}
 
      ic::L1TTau *thisTau = new ic::L1TTau;
      thisTau->set_pt    (productL1Upgrade->tauEt    [i]);
      thisTau->set_eta   (productL1Upgrade->tauEta   [i]);
      thisTau->set_phi   (productL1Upgrade->tauPhi   [i]);
      thisTau->set_energy(productL1Upgrade->tauEnergy[i]);
      thisTau->isolation = productL1Upgrade->tauIso  [i];
      
      //myEvent->l1tTauCollection.push_back(thisTau);
      
      l1tTaus.push_back(thisTau);
      if(thisTau->isolation!=0){
        //myEvent.l1tIsoTauCollection.push_back(thisTau);
        l1tIsoTaus.push_back(thisTau);
      }
    }
    
    vector<ic::L1TJet*> l1tJets;
    unsigned short int nL1TJets = productL1Upgrade->nJets;
    for(unsigned i=0; i<nL1TJets; i++){
      
      if(productL1Upgrade->jetBx[i] != 0){continue;}
      
      ic::L1TJet *thisJet = new ic::L1TJet();
      thisJet->set_pt    (productL1Upgrade->jetEt    [i]);
      thisJet->set_eta   (productL1Upgrade->jetEta   [i]);
      thisJet->set_phi   (productL1Upgrade->jetPhi   [i]);
      thisJet->set_energy(productL1Upgrade->jetEnergy[i]);
      
      l1tJets.push_back(thisJet);
    }         
    
    vector<ic::L1TSum*> l1tSums;
    unsigned short int nL1TSums = productL1Upgrade->nSums;
    for(unsigned i=0; i<nL1TSums; i++){
      
      if(productL1Upgrade->sumBx[i] != 0){continue;}
      
      ic::L1TSum *thisSum = new ic::L1TSum;
      thisSum->set_pt    (productL1Upgrade->sumEt [i]);
      thisSum->set_eta   (0);
      thisSum->set_phi   (productL1Upgrade->sumPhi[i]);
      thisSum->set_energy(productL1Upgrade->sumEt [i]);
 
      short int type = productL1Upgrade->sumType[i];
      if     (type==L1Analysis::EtSumType::kTotalEt)  {thisSum->sumType = ic::L1TSum::SumType::kTotalEt;}
      else if(type==L1Analysis::EtSumType::kTotalHt)  {thisSum->sumType = ic::L1TSum::SumType::kTotalHt;}
      else if(type==L1Analysis::EtSumType::kMissingEt){thisSum->sumType = ic::L1TSum::SumType::kMissingEt;}
      else if(type==L1Analysis::EtSumType::kMissingHt){thisSum->sumType = ic::L1TSum::SumType::kMissingHt;}
      else if(type==L1Analysis::EtSumType::kTotalEtx) {thisSum->sumType = ic::L1TSum::SumType::kTotalEtx;}
      else if(type==L1Analysis::EtSumType::kTotalEty) {thisSum->sumType = ic::L1TSum::SumType::kTotalEty;}
      else if(type==L1Analysis::EtSumType::kTotalHtx) {thisSum->sumType = ic::L1TSum::SumType::kTotalHtx;}
      else if(type==L1Analysis::EtSumType::kTotalHty) {thisSum->sumType = ic::L1TSum::SumType::kTotalHty;}

      l1tSums.push_back(thisSum);
    }
    
    if(options.verbose){printf("===== Sorting Collections =====\n");}
    sort(l1tEG      .begin(),l1tEG      .end(),greater_ICCandidate());
    sort(l1tIsoEG   .begin(),l1tIsoEG   .end(),greater_ICCandidate());
    sort(l1tMuons   .begin(),l1tMuons   .end(),greater_ICCandidate());
    sort(l1tIsoMuons.begin(),l1tIsoMuons.end(),greater_ICCandidate());
    sort(l1tTaus    .begin(),l1tTaus    .end(),greater_ICCandidate());
    sort(l1tIsoTaus .begin(),l1tIsoTaus .end(),greater_ICCandidate());
    sort(l1tJets    .begin(),l1tJets    .end(),greater_ICCandidate());

    myEvent.addProduct< vector<ic::L1TEGamma*> >("l1t_eg",     l1tEG      );
    myEvent.addProduct< vector<ic::L1TEGamma*> >("l1t_isoeg",  l1tIsoEG   );
    myEvent.addProduct< vector<ic::L1TMuon*> >  ("l1t_muon",   l1tMuons   );
    myEvent.addProduct< vector<ic::L1TMuon*> >  ("l1t_isomuon",l1tIsoMuons);
    myEvent.addProduct< vector<ic::L1TTau*> >   ("l1t_tau",    l1tTaus    );
    myEvent.addProduct< vector<ic::L1TTau*> >   ("l1t_isotau", l1tIsoTaus );
    myEvent.addProduct< vector<ic::L1TJet*> >   ("l1t_jet",    l1tJets    );
    myEvent.addProduct< vector<ic::L1TSum*> >   ("l1t_sum",    l1tSums    );
    
    // Now processing all analysis
    for(unsigned iModule=0; iModule<moduleSequence.size(); iModule++){
      moduleSequence[iModule]->run(myEvent);
    }
    
    // Memory cleaning
    for(unsigned obj=0; obj<l1tEG.size();    obj++){delete l1tEG[obj];}
    for(unsigned obj=0; obj<l1tMuons.size(); obj++){delete l1tMuons[obj];}
    for(unsigned obj=0; obj<l1tTaus.size();  obj++){delete l1tTaus[obj];}
    for(unsigned obj=0; obj<l1tJets.size();  obj++){delete l1tJets[obj];}
    for(unsigned obj=0; obj<l1tSums.size();  obj++){delete l1tSums[obj];}
  }
  
  for(unsigned iModule=0; iModule<moduleSequence.size(); iModule++){
    moduleSequence[iModule]->endJob();
  }
  
  // Writting/closing output file
  fileOut->Write();
  fileOut->Close();
  delete fileOut;
  
  // Cleaning memory
  for(unsigned iModule=0; iModule<moduleSequence.size(); iModule++){
    delete moduleSequence[iModule];
  }
  
  // Reporting on effective entries
  cout << "===== end job report ===== " << endl;
  cout << "Effective events = " << eventsEffective << endl;
  
  return 0;
}
