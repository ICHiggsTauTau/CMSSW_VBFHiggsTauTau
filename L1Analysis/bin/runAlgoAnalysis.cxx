// CMSSW includes
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"

// CMSSW_VBFHiggsTauTau includes
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/Event.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoAnalysis.h"

// ICHiggsTauTau Objects (Moved to this repository)
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/EventInfo.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/GenParticle.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TObject.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TEGamma.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TMuon.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TTau.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TJet.hh"
#include "CMSSW_VBFHiggsTauTau/DataFormats/interface/L1TSum.hh"

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

unsigned char getDecayChannel(ic::GenParticleCollection *event_gen_particles){
  
  unsigned n_event_gen_particles = event_gen_particles->size();
  
  unsigned nHadTaus   = 0;
  unsigned nElectrons = 0;
  unsigned nMuons     = 0;
  
  for(unsigned j=0; j< n_event_gen_particles; j++){
    
    int genID = std::fabs(event_gen_particles->at(j).pdgid());
    
    if(genID == 15){
      
      if(std::fabs(event_gen_particles->at(event_gen_particles->at(j).mothers().at(0)).pdgid()) == 25){
        bool isMuon    = false;
        bool isElectron = false;
        
        for(unsigned l=0; l < event_gen_particles->at(j).daughters().size(); l++) {
          if(std::fabs(event_gen_particles->at(event_gen_particles->at(j).daughters().at(l)).pdgid()) == 11) isElectron = true;
          if(std::fabs(event_gen_particles->at(event_gen_particles->at(j).daughters().at(l)).pdgid()) == 13) isMuon     = true;
        }
        
        if     (isElectron){nElectrons++;}
        else if(isMuon)    {nMuons++;}
        else               {nHadTaus++;}
      }
    }
  }
  
  if     (nElectrons == 1 && nMuons == 1 && nHadTaus == 0){return VBFHiggs::HiggsDecay::EleMuo;}
  else if(nElectrons == 1 && nMuons == 0 && nHadTaus == 1){return VBFHiggs::HiggsDecay::EleHad;}
  else if(nElectrons == 0 && nMuons == 1 && nHadTaus == 1){return VBFHiggs::HiggsDecay::MuoHad;}
  else if(nElectrons == 0 && nMuons == 0 && nHadTaus == 2){return VBFHiggs::HiggsDecay::HadHad;}
 
  return 0;
}

class ProgramOptions{
public:
  
  ProgramOptions(){this->init();}
  
  ProgramOptions(int argc, char *argv[]){
    
    this->init();
    valid=true;
    
    // Check the number of parameters
    if (argc < 2) {valid=false;}
    
    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];
      if     (arg == "--verbose")                {verbose  = true;}
      else if(arg == "--doAnalysisL1TResolution"){doAnalysisL1TResolution = true;}
      else if(arg == "--doAnalysisL1TAlgoScan")  {doAnalysisL1TAlgoScan   = true;}
      else if(arg == "--doOfflineFilter")        {doOfflineFilter         = true;}
      else if(arg == "--inputType"){
        if(i+1<argc){i++; inputType = argv[i];} 
        else{valid=false;}
      }else if(arg == "--input"){
        if(i+1<argc){i++; input = argv[i];}
        else{valid=false;}
      }else if(arg == "--maxEvents"){
        if(i+1<argc){i++; maxEvents = atoi(argv[i]);}
        else{valid=false;}
      }else if(arg == "--runs"){
        if(i+1<argc){
          i++; 
          
          std::stringstream inVar(argv[i]);
          int i;
          while(inVar >> i){
            runs.push_back(i);
            if(inVar.peek() == ','){inVar.ignore();}
          }
        }
        else{valid=false;}
      }else if(arg == "--maxEvents"){
        if(i+1<argc){i++; maxEvents = atoi(argv[i]);}
        else{valid=false;}
      }else if(arg == "--jobType"){
        if(i+1<argc){i++; jobType = argv[i];}
        else{valid=false;}
      }else if(arg == "--decay"){
        if(i+1<argc){i++; 
          if     (std::string(argv[i]) == "EleEle"){decay = VBFHiggs::HiggsDecay::EleEle; decayName="EleEle";}
          else if(std::string(argv[i]) == "EleMuo"){decay = VBFHiggs::HiggsDecay::EleMuo; decayName="EleMuo";}
          else if(std::string(argv[i]) == "EleHad"){decay = VBFHiggs::HiggsDecay::EleHad; decayName="EleHad";}
          else if(std::string(argv[i]) == "MuoMuo"){decay = VBFHiggs::HiggsDecay::MuoMuo; decayName="MuoMuo";}
          else if(std::string(argv[i]) == "MuoHad"){decay = VBFHiggs::HiggsDecay::MuoHad; decayName="MuoHad";}
          else if(std::string(argv[i]) == "HadHad"){decay = VBFHiggs::HiggsDecay::HadHad; decayName="HadHad";}        
        }
        else{valid=false;}
      }else if(arg == "--outputFilename"){
        if(i+1<argc){i++; outputFilename = argv[i];}
        else{valid=false;}
      }else if (arg == "--sumRange"){
        if(i+1<argc){i++; sumRange = atoi(argv[i]);}
      }
    }
  }
  
  void init(){
    verbose        = false;
    valid          = false;
    inputType      = "file";
    input          = "input.root";
    maxEvents      = -1;
    jobType        = "mc";
    decay          = 0;
    outputFilename = "L1Analysis.root";
    sumRange       = 5;
    
    doAnalysisL1TAlgoScan   = false;
    doAnalysisL1TResolution = false;
    doOfflineFilter         = false;
  }
  
  void printHelpMessage(){
    
    cerr << "Usage: vbftautau_runAlgoAnalysis [commands]"                                        << endl;
    cerr << " --verbose         "                                                                << endl;
    cerr << " --inputType TYPE          options: file, filelist"                                 << endl;
    cerr << " --input INPUT"                                                                     << endl;
    cerr << " --runs RUNS               options: comma separated"                                << endl;
    cerr << " --maxEvents N"                                                                     << endl;
    cerr << " --jobType TYPE            options: mc, data"                                       << endl;
    cerr << " --decay DECAY             options: EleEle, EleMuo, EleHad, MuoMuo, MuoHad, HadHad" << endl;
    cerr << " --outputFilename NAME"                                                             << endl;
    cerr << endl;
    cerr << "ANALYSIS OPTIONS:" << endl;
    cerr << " --doAnalysisL1TAlgoScan"                                                           << endl;
    cerr << " --doAnalysisL1TResolution"                                                         << endl;
    cerr << " --doOfflineFilter"                                                                 << endl;
    cerr << " --sumRange MAX-ETA        options: 3, 5"                                           << endl;
  }
  
  void print(){
    cout << "##### vbftautau_runAlgoAnalysis #####" << endl;
    cout << "verbose        = " << verbose        << endl;
    cout << "inputType      = " << inputType      << endl;
    cout << "input          = " << input          << endl;
    cout << "maxEvents      = " << maxEvents      << endl;
    cout << "runs           = ";
    for(unsigned i=0; i<runs.size(); i++){
      if(i!=0){cout << ",";}
      cout << runs[i];
    }
    cout << endl;
    cout << "jobType                 = " << jobType        << endl;
    cout << "decay                   = " << decayName      << endl;  
    cout << "outputFilename          = " << outputFilename << endl;
    cout << "doAnalysisL1TResolution = " << doAnalysisL1TResolution << endl;
    cout << "doAnalysisL1TAlgoScan   = " << doAnalysisL1TAlgoScan << endl;
    cout << "doOfflineFilter         = " << doOfflineFilter << endl;
    cout << "sumRange                = " << sumRange        << endl;
    cout << endl;
    
  }
  
  bool             verbose;
  bool             valid;
  string           inputType;
  string           input;
  vector<int>      runs;
  Long64_t         maxEvents;
  string           jobType;
  unsigned char    decay;
  string           decayName;
  string           outputFilename;
  unsigned         sumRange;  
  
  // Analysis options
  bool doAnalysisL1TAlgoScan;
  bool doAnalysisL1TResolution;
  bool doOfflineFilter;
};

// Here is the application code
int main(int argc, char* argv[]){
  
  ProgramOptions options(argc,argv);
  if(!options.valid){
    cout << "[main] Options invalid... quiting!" << endl;
    options.printHelpMessage();
    return 1;
  }
  options.print();
  
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
    myAnalysis->setVerbose(options.verbose);
    myAnalysis->setDoSingleObjectsAnalysis(true);
    if(options.jobType=="mc"){
      myAnalysis->setDoGenAnalysis(false); // FIX THIS
    }
    
    moduleSequence.push_back(myAnalysis);
  }
  
  // Setting up tree
  TChain tree_event("EventTree");
  
  // Adding all input files to the event tree
  for(unsigned i=0; i<inputFileCollection.size(); i++){
    cout << "Input file #" << i << " - " << inputFileCollection[i] << endl;
    tree_event.Add(inputFileCollection[i].c_str());
  }
  
  // Now creating the content holders
  ic::EventInfo             *product_event_info        = 0;
  ic::L1TEGammaCollection   *product_l1t_egs           = 0;
  ic::L1TMuonCollection     *product_l1t_muons         = 0;  
  ic::L1TTauCollection      *product_l1t_taus          = 0;
  ic::L1TJetCollection      *product_l1t_jets          = 0;
  ic::L1TSumCollection      *product_l1t_sums_range3p0 = 0;
  ic::L1TSumCollection      *product_l1t_sums_range5p0 = 0;
  ic::GenParticleCollection *product_gen_particles     = 0; // Only available in MC
  bool                      product_passed_offline    = false; // Only available in MC
  
  // Allocating space for branches content
  product_event_info        = new ic::EventInfo();
  product_l1t_egs           = new ic::L1TEGammaCollection();
  product_l1t_muons         = new ic::L1TMuonCollection();
  product_l1t_taus          = new ic::L1TTauCollection();
  product_l1t_jets          = new ic::L1TJetCollection();
  product_l1t_sums_range3p0 = new ic::L1TSumCollection();
  product_l1t_sums_range5p0 = new ic::L1TSumCollection();

  // Setting branch address
  tree_event.SetBranchAddress("eventInfo",         &product_event_info);
  tree_event.SetBranchAddress("L1EGammas",         &product_l1t_egs);
  tree_event.SetBranchAddress("L1Muons",           &product_l1t_muons);
  tree_event.SetBranchAddress("L1Taus",            &product_l1t_taus);
  tree_event.SetBranchAddress("L1Jets",            &product_l1t_jets);
  tree_event.SetBranchAddress("L1Sums",            &product_l1t_sums_range3p0);
  tree_event.SetBranchAddress("L1SumsEtaRange5pt0",&product_l1t_sums_range5p0);
  
  // If we are processing MC events get addition data
  if(options.jobType=="mc"){

    //product_passed_offline = new bool();
    product_passed_offline = tree_event.SetBranchAddress("PassedOffline",&product_passed_offline);
    
    product_gen_particles = new ic::GenParticleCollection();
    tree_event.SetBranchAddress("genParticles",&product_gen_particles);
  }
  

  
  Long64_t entries_VBFHiggsToTauTau = tree_event.GetEntries();
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
    
    if(options.verbose){printf("[main] Loading TTree\n");}
    tree_event.LoadTree(ev);
    tree_event.GetEntry(ev);
    
    //TODO: Channel check
    
    // Checking if option runs is set
    if(options.runs.size()){
      bool passRun = false;
      
      // Check if this event belong to the selected runs
      for(unsigned i=0; i<options.runs.size(); i++){
        if(options.runs[i]==product_event_info->run()){passRun=true;}
      }
      if(!passRun){continue;}
    }
    
    // Checking decay
    if(options.decay!=0){
      
      unsigned char thisDecay  = getDecayChannel(product_gen_particles);
      if(options.verbose){cout << "[main] Decay = " << thisDecay << endl;}
      
      if(options.decay!=thisDecay){continue;}
    }
    
    // If we are running over MC and we want to filter on offline selection...
    if(options.doOfflineFilter && options.jobType=="mc"){
      
      // If the event has failed the offine selection we skip it
      // NOTE: This only effects the numerator of the efficiency

      if( !product_passed_offline){continue;}
    }
        
    eventsEffective++;
    
    if(options.doAnalysisL1TAlgoScan){
      myAnalysis->incrementEventTotal();
    }
    
    trgfw::Event myEvent;
    
    //if(options.jobType=="mc"){
    //  myEvent.genInfo = genInfo;
    //}else{
    //  myEvent.genInfo = 0;
    //}
    
    if(options.verbose){
      printf("[main] L1T EGamma         : %lu\n",product_l1t_egs          ->size());
      printf("[main] L1T Muon           : %lu\n",product_l1t_muons        ->size());
      printf("[main] L1T Tau            : %lu\n",product_l1t_taus         ->size());
      printf("[main] L1T Jets           : %lu\n",product_l1t_jets         ->size());
      printf("[main] L1T Sums (|eta|<3) : %lu\n",product_l1t_sums_range3p0->size());
      printf("[main] L1T Sums (|eta|<5) : %lu\n",product_l1t_sums_range5p0->size());
    }
    
    // Making vectors of pointer for L1T EG 
    vector<ic::L1TEGamma*> l1tEG;
    vector<ic::L1TEGamma*> l1tIsoEG;
    for(unsigned i=0; i<product_l1t_egs->size(); i++){
      
      ic::L1TEGamma *thisEG = &(product_l1t_egs->at(i));
      
      l1tEG.push_back(thisEG);
      if(thisEG->isolation!=0){l1tIsoEG.push_back(thisEG);}
    }
    
    // Making vectors of pointer for L1T Muon
    vector<ic::L1TMuon*> l1tMuons;
    vector<ic::L1TMuon*> l1tIsoMuons;
    for(unsigned i=0; i<product_l1t_muons->size(); i++){
      
      ic::L1TMuon *thisMuon = &(product_l1t_muons->at(i));
      
      //Only use muons with quality >= 12 - this is what is used for all single muon L1 seeds!
      if(thisMuon->quality>=12){
          l1tMuons.push_back(thisMuon);
          if(thisMuon->isolation!=0){l1tIsoMuons.push_back(thisMuon);}
      }
    }
    
    // Making vectors of pointer for L1T Tau
    vector<ic::L1TTau*> l1tTaus;
    vector<ic::L1TTau*> l1tIsoTaus;
    for(unsigned i=0; i<product_l1t_taus->size(); i++){
      
      ic::L1TTau *thisTau = &(product_l1t_taus->at(i));
      
      l1tTaus.push_back(thisTau);
      if(thisTau->isolation!=0){l1tIsoTaus.push_back(thisTau);}
    }
    
    // Making vectors of pointer for L1T Jets
    vector<ic::L1TJet*> l1tJets;
    for(unsigned i=0; i<product_l1t_jets->size(); i++){
      
      ic::L1TJet *thisJet = &(product_l1t_jets->at(i));
      l1tJets.push_back(thisJet);
    }
    
    // Making vectors of pointer for L1T Sums (for now taking 5.0 range sums)
    vector<ic::L1TSum*> l1tSums;
    if      (options.sumRange == 5){
      for(unsigned i=0; i<product_l1t_sums_range5p0->size(); i++){
      
        ic::L1TSum *thisSum = &(product_l1t_sums_range5p0->at(i));
        l1tSums.push_back(thisSum);
      }
    }else if(options.sumRange == 3){
      for(unsigned i=0; i<product_l1t_sums_range3p0->size(); i++){
      
        ic::L1TSum *thisSum = &(product_l1t_sums_range3p0->at(i));
        l1tSums.push_back(thisSum);
      } 
    }
    
    // This should not be needed but... just to be sure
    if(options.verbose){printf("===== Sorting Collections =====\n");}
    sort(l1tEG      .begin(),l1tEG      .end(),greater_ICCandidate());
    sort(l1tIsoEG   .begin(),l1tIsoEG   .end(),greater_ICCandidate());
    sort(l1tMuons   .begin(),l1tMuons   .end(),greater_ICCandidate());
    sort(l1tIsoMuons.begin(),l1tIsoMuons.end(),greater_ICCandidate());
    sort(l1tTaus    .begin(),l1tTaus    .end(),greater_ICCandidate());
    sort(l1tIsoTaus .begin(),l1tIsoTaus .end(),greater_ICCandidate());
    sort(l1tJets    .begin(),l1tJets    .end(),greater_ICCandidate());
    
    // Adding pruducts to event
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
