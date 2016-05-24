// VBF includes
#include "CMSSW_VBFHiggsTauTau/IO/interface/File.h"
#include "CMSSW_VBFHiggsTauTau/Style/interface/Style.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/AlgoWorkingPoint.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/WorkerCLOutput.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/WorkerOptimiser.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/WorkerHelpers.h"

#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TVariableScanDataFormat.h"

 // ROOT includes
 #include "TFile.h"
 #include "TTree.h"
 #include "TH1D.h"
 #include "TH2D.h"
 #include "TProfile.h"
 #include "TLegend.h"
 #include "TCanvas.h"
  
 // C++ STD includes
 #include <iostream>
 #include <string>
 #include <sstream>
 #include <fstream>
 #include <cstdio>
 #include <map>
 #include <vector>
 #include <list>
 #include <functional>
 #include <thread>
 #include <chrono>
 #include <ctime>
 #include <mutex>

 // Other includes
 #include <stdlib.h>
 
using namespace std;



  
//#################################################################################
//#################################################################################
//#################################################################################
double getConvertionFactor(int run){
  
  // How to calculate the rates
  //rate_targetLumi = bunchEff*11246*2808*(target_lumiPerBunch/run259721_lumiPerBunch)
  //                = bunchEff*11246*(1.15e34)*(517/0.15e34)
  //                = bunchEff*11246*517*(1.15e34/0.15e34)
  
  double factor = 1;
  
  if(run==259721){
    
    //const int nMaxBunch50ns = 1380;
    //const double nMaxBunch25ns = 2808;
    const double ratePerBunch  = 11246;
    
    const double run259721_lumi   = 0.15e34;
    const double run259721_nBunch = 517;
    
    //const double targetNBunchs = 2808;
    const double targetLumi    = 1.15e34;
    
    factor = ratePerBunch*run259721_nBunch*(targetLumi/run259721_lumi);
  }
  
  return factor;
}

//#################################################################################
//#################################################################################
//#################################################################################
bool hasEnding (std::string const &fullString, std::string const &ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}

//#################################################################################
//#################################################################################
//#################################################################################
class ProgramOptions{
public:
  
  ProgramOptions(){
    this->init();
  }
  
  ProgramOptions(int argc, char *argv[]){
    
    this->init();
    
    // Check the number of parameters
    if (argc < 2) {
      printHelpMessage(); 
      valid=false;
    }
    
    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];
      if     (arg == "--verbose")     {verbose = true;}
      else if(arg == "--plots")       {plots   = true;}
      else if(arg == "--optimisation"){optimisation   = true;}
      else if(arg == "--inputA"){
        if(i+1<argc){i++; inputA = argv[i];}
        else{printHelpMessage(); valid=false;}
      }
      else if(arg == "--signal"){
        if(i+1<argc){i++; signal = argv[i];}
        else{printHelpMessage(); valid=false;}
      }
      else if(arg == "--inputType"){
        if(i+1<argc){i++; inputType = argv[i];}
        else{printHelpMessage(); valid=false;}
      }
      else if(arg == "--inputB"){
        if(i+1<argc){i++; inputB = argv[i];}
        else{printHelpMessage(); valid=false;}
      }
      else if(arg == "--nThread"){
        if(i+1<argc){i++; nThread = atoi(argv[i]);}
        else{printHelpMessage(); valid=false;}
      }
      else if(arg == "--targetRate"){
        if(i+1<argc){i++; targetRate = atof(argv[i]);}
        else{printHelpMessage(); valid=false;}
      }
      else if(arg == "--outputDir"){
        if(i+1<argc){i++; outputDir = argv[i];}
        else{printHelpMessage(); valid=false;}
      }
    }
  }
  
  void init(){
    valid        = true;
    verbose      = false;
    plots        = false;
    optimisation = false;
    signal       = "tautau";
    inputA       = "file";
    inputB       = "file";
    outputDir    = "output/";
    targetRate   = 1100.;
    nThread      = 1;
    inputType    = "histogram";
  }
  
  void printHelpMessage(){
    cerr << "Usage: vbftautau_mergeOptimisationTrees [commands]" << endl;
    cerr << " --verbose"                                             << endl;
    cerr << " --plots                - Save plots"                   << endl;
    cerr << " --optimisation         - Do optimisation scan"         << endl;
    cerr << " --signal       CHANNEL - "                             << endl;
    cerr << " --inputType    TYPE    - options: histograms or trees" << endl;
    cerr << " --inputA       INPUT   - Signal sample input"          << endl;
    cerr << " --inputB       INPUT"                                  << endl;
    cerr << " --outputDir    DIR"                                    << endl;
    cerr << " --nThread      N"                                      << endl;
    cerr << " --targetRate   RATE"                                   << endl;
  }
  
  void print(){
    cout << "##### vbftautau_mergeOptimisationTrees #####" << endl;
    cout << "=> Program parameters:" << endl;
    cout << "verbose    : " << verbose    << endl;
    cout << "signal     : " << signal     << endl;
    cout << "inputType  : " << inputType  << endl;
    cout << "inputA     : " << inputA     << endl;
    cout << "inputB     : " << inputB     << endl;
    cout << "outputDir  : " << outputDir  << endl;
    cout << "nThread    : " << nThread    << endl;
    cout << "targetRate : " << targetRate << endl;
    cout << "=> Tasks to perform:" << endl;
    cout << "optimisation : " << optimisation << endl;
    cout << "plots        : " << plots << endl;
    cout << endl;
  }
  
  bool     valid;
  bool     verbose;
  bool     plots;
  bool     optimisation;
  unsigned nThread;
  double   targetRate;
  string signal;
  string inputType;
  string inputA;
  string inputB;
  string outputDir;
};

//#################################################################################
//#################################################################################
//#################################################################################
int main ( int argc , char * argv []){
  
  // Processing command line arguments
  ProgramOptions options(argc,argv);
  if(!options.valid){
    cout << "[main] Options invalid... quiting!" << endl;
    return 1;
  }
  options.print();
  
  // Setting style
  hepfw::Style myStyle;
  myStyle.setTDRStyle();
  
  // Making output file
  system(Form("mkdir -p %s",options.outputDir.c_str()));
  
  // Getting input files
  std::mutex   base_histograms_mutex;
  hepfw::File *fSig = new hepfw::File(options.inputA.c_str());
  hepfw::File *fBkg = new hepfw::File(options.inputB.c_str());

  // Getting total number of events for signal
  TH1D* hSigTotal = (TH1D*) fSig->Get("L1TAlgoScan/EventCount");
  double nSigEvents = hSigTotal->GetBinContent(1);
  
  // Getting total number of events for zero bias
  TH1D* hBkgTotal = (TH1D*) fBkg->Get("L1TAlgoScan/EventCount");
  double nBkgEvents = hBkgTotal->GetBinContent(1);

  // Informing user of how many events to process
  cout << "========= Events =========" << endl;
  cout << "Signal    events: " << nSigEvents << endl;
  cout << "Zero Bias events: " << nBkgEvents << endl;
  
  printf("===== Decay Summary =====\n");
  TH1D* HiggsDecay = (TH1D*) fSig->Get("L1TAlgoScan/GenAnalysis/HiggsDecay");
  for(int i=1; i<HiggsDecay->GetNbinsX()+1; i++){
    printf("Decay: %10s - Faction: %6.4f\n",HiggsDecay->GetXaxis()->GetBinLabel(i),HiggsDecay->GetBinContent(i)/nSigEvents);
  }
  
  printf("===== Convertion factor =====\n");
  double run259721_convFactor = getConvertionFactor(259721);
  printf("Data convertion factor: %10.2f\n",run259721_convFactor);
  printf("\n");
  
  // File to contain the base algorithm rates and efficiencies
  FILE * file_output_baseAlgoRates;
  file_output_baseAlgoRates = fopen (Form("%s/results_BaseOutputRates.txt",options.outputDir.c_str()), "w+");
  
  printf("===== Algo Efficiency Summary =====\n");
  TH1D* AlgoPass     = (TH1D*) fSig->Get("L1TAlgoScan/AlgoPass");
  TH1D* AlgoPassRate = (TH1D*) fBkg->Get("L1TAlgoScan/AlgoPass");
  for(int i=1; i<AlgoPass->GetNbinsX()+1; i++){
    fprintf(file_output_baseAlgoRates,"Algo: %40s - Eff: %6.4f rate: %10.1f Hz\n",AlgoPass->GetXaxis()->GetBinLabel(i),AlgoPass->GetBinContent(i)/nSigEvents,run259721_convFactor*AlgoPassRate->GetBinContent(i)/nBkgEvents);
  }
  
  fclose(file_output_baseAlgoRates);
  

  if(options.optimisation || options.plots){
    
    if(options.verbose){cout << "[main] Starting optimisation process..." << endl;}
    
    std::mutex command_line_mutex; // CL output mutex
    std::mutex root_mutex;         // ROOT create/destroy mutex
    
    // Input pairs of histograms container and its mutex
    std::mutex                    input_mutex;
    unsigned                      input_total     = 0;
    unsigned                      input_processed = 0;
    list<trgfw::InputHistograms*> input_histograms;
    list<trgfw::InputScan*>       input_scans;
    
    // Output working points contained and its mutex
    std::mutex                      workingPoints_mutex;
    vector<trgfw::AlgoWorkingPoint> workingPoints;
    
    // Output file and its mutex
    std::mutex outputfile_mutex;
    hepfw::File *outputfile = new hepfw::File(Form("%s/L1Algo_eff.root",options.outputDir.c_str()),"RECREATE");
    outputfile->copyDirectoryStructure(fSig);
    outputfile->Write();
    
    //TODO:
    unsigned nWorkers = options.nThread;
    vector<trgfw::WorkerOptimiser*> myOptimisers;
    
    if(options.verbose){cout << "[main] Starting Command Line Worker (threaded)" << endl;}
    
    trgfw::WorkerCLOutput::InputType input_type_worker_cl = trgfw::WorkerCLOutput::InputType::None;
    if     (options.inputType=="histogram"){input_type_worker_cl=trgfw::WorkerCLOutput::InputType::Histogram;}
    else if(options.inputType=="tree")     {input_type_worker_cl=trgfw::WorkerCLOutput::InputType::Tree;}
    
    // Starting my CL output thread
    trgfw::WorkerCLOutput worker;
    worker.setInput              (&input_mutex,&input_histograms,&input_scans,&input_total,&input_processed);
    worker.setInputType          (input_type_worker_cl);
    worker.setPtrCommandLineMutex(&command_line_mutex);
    worker.setPtrWorkerOptimisers(&myOptimisers);
    worker.setPtrWorkingPoints   (&workingPoints_mutex,&workingPoints);
    worker.start();
    
    trgfw::WorkerOptimiser::InputType input_type_worker_optimiser = trgfw::WorkerOptimiser::InputType::None;
    if     (options.inputType=="histogram"){input_type_worker_optimiser=trgfw::WorkerOptimiser::InputType::Histogram;}
    else if(options.inputType=="tree")     {input_type_worker_optimiser=trgfw::WorkerOptimiser::InputType::Tree;}
    
    if(options.verbose){cout << "[main] Starting Optimisation Workers (threaded)" << endl;}

    // Starting all optimisation workers
    for(unsigned i=0; i<nWorkers; i++){
      
      trgfw::WorkerOptimiser *w = new trgfw::WorkerOptimiser(i);
      w->setVerbose            (options.verbose);
      w->setTargetRate         (options.targetRate);
      w->setOutputDir          (options.outputDir);
      w->setPtrCommandLineMutex(&command_line_mutex);
      w->setInputType          (input_type_worker_optimiser);
      w->setRootMutex          (&root_mutex);
      w->setInput              (&input_mutex,&input_histograms,&input_scans,&input_processed);
      w->setOutputWorkingPoints(&workingPoints_mutex,  &workingPoints);
      w->setOutputFile         (&outputfile_mutex,     outputfile);
      w->setNEvents            (nSigEvents,nBkgEvents);
      w->setSavePlots          (options.plots);
      w->setConvFactor         (run259721_convFactor);
      myOptimisers.push_back(w);
    }
    
    // Legacy input mode
    if(options.inputType=="histogram"){
      
      // Getting all histograms
      vector<TH1*> hSig = fSig->getHistograms();
      vector<TH1*> hBkg = fBkg->getHistograms();
      unsigned base_histograms_initial_size_efficiency = hSig.size();
      unsigned base_histograms_initial_size_rate       = hBkg.size();
      
      // Setting more things on CL worker
      worker.setBaseHistogramsInitialSize(base_histograms_initial_size_efficiency,base_histograms_initial_size_rate);
      worker.setPtrBaseHistograms        (&base_histograms_mutex,&hSig,&hBkg);
      
      // Cleaning from histograms we do now want to analyise
      for(auto it=hSig.begin(); it!=hSig.end(); it++){
        
        string sigTitle = (*it)->GetName();
        string sigPath  = (*it)->GetDirectory()->GetPath();
        
        bool removeElement = false;
        
        // Generic histograms to ignore
        if(sigTitle=="EventCount")            {removeElement=true;}
        if(sigTitle=="AlgoPass")              {removeElement=true;}
        if(hasEnding(sigTitle,"_N"))          {removeElement=true;}
        if(hasEnding(sigTitle,"_Eta"))        {removeElement=true;}
        if(hasEnding(sigTitle,"_Phi"))        {removeElement=true;}
        if(hasEnding(sigTitle,"_minDRL1TJet")){removeElement=true;}
        if(hasEnding(sigTitle,"_METvsMjj"))   {removeElement=true;}
        
        // Histograms to ignore for VBF Higgs to Invisible optimisation
        if(options.signal=="inv"){
          if(sigTitle.find("L1TEGamma")!=std::string::npos){removeElement=true;}
          if(sigTitle.find("L1TMuon")  !=std::string::npos){removeElement=true;}
          if(sigTitle.find("L1TTau")   !=std::string::npos){removeElement=true;}
          if(sigTitle.find("L1TIsoTau")!=std::string::npos){removeElement=true;}
        }
        
        if(removeElement){
          
          base_histograms_mutex.lock();
          it = hSig.erase(it); // Erase element and get iterator to next element
          it--;                // Stepping one element back
          base_histograms_mutex.unlock();
        }
      }
      
      for(auto it=hBkg.begin(); it!=hBkg.end(); it++){
        
        string sigTitle = (*it)->GetName();
        string sigPath  = (*it)->GetDirectory()->GetPath();
        
        bool removeElement = false;
        
        // Generic histograms to ignore
        if(sigTitle=="EventCount")            {removeElement=true;}
        if(sigTitle=="AlgoPass")              {removeElement=true;}
        if(hasEnding(sigTitle,"_N"))          {removeElement=true;}
        if(hasEnding(sigTitle,"_Eta"))        {removeElement=true;}
        if(hasEnding(sigTitle,"_Phi"))        {removeElement=true;}
        if(hasEnding(sigTitle,"_minDRL1TJet")){removeElement=true;}
        if(hasEnding(sigTitle,"_METvsMjj"))   {removeElement=true;}
        
        // Histograms to ignore for VBF Higgs to Invisible optimisation
        if(options.signal=="inv"){
          if(sigTitle.find("L1TEGamma")!=std::string::npos){removeElement=true;}
          if(sigTitle.find("L1TMuon")  !=std::string::npos){removeElement=true;}
          if(sigTitle.find("L1TTau")   !=std::string::npos){removeElement=true;}
          if(sigTitle.find("L1TIsoTau")!=std::string::npos){removeElement=true;}
        }
        
        if(removeElement){
          
          base_histograms_mutex.lock();
          it = hBkg.erase(it); // Erase element and get iterator to next element
          it--;                // Stepping one element back
          base_histograms_mutex.unlock();
        }
      }
      
      for(unsigned i=0; i<nWorkers; i++){
        myOptimisers[i]->start();
      }
      
      // Finding all pairs
      for(auto itA=hSig.begin(); itA!=hSig.end(); itA++){
        
        TH1D* iHA = (TH1D*) (*itA);
        
        string sigTitle = iHA->GetName();
        string sigPath  = iHA->GetDirectory()->GetPath();
        sigPath  = sigPath.substr(sigPath.find(':')+2,sigPath.size()-1);
        
        for(auto itB=hBkg.begin(); itB!=hBkg.end(); itB++){
          
          TH1D* iHB = (TH1D*) (*itB);
          
          string bkgTitle = iHB->GetName();
          string bkgPath  = iHB->GetDirectory()->GetPath();
          bkgPath = bkgPath.substr(bkgPath.find(':')+2,bkgPath.size()-1);
          
          if(sigTitle == bkgTitle && sigPath == bkgPath && sigPath != ""){
            
            trgfw::InputHistograms *iHists = new trgfw::InputHistograms;
            iHists->path        = sigPath;
            iHists->name        = sigTitle;
            iHists->hEfficiency = iHA;
            iHists->hRate       = iHB;
            
            input_mutex.lock();
            input_total++;
            input_histograms.push_back(iHists);
            input_mutex.unlock();
            
            base_histograms_mutex.lock();
            itA = hSig.erase(itA); // Erase element and get iterator to next element
            itA--;                 // Stepping one element back
            itB = hBkg.erase(itB); // Erase element and get iterator to next element
            itB--;                 // Stepping one element back
            base_histograms_mutex.unlock();
            
            break;
          }
        }  
        
      }
      
      if(options.verbose){
        std::lock_guard<std::mutex> lock(command_line_mutex);
        cout << "[main] Finished histogram pairing..." << endl;
      }
      
      // Caching number of histograms to process
      input_mutex.lock();
      unsigned nInput=input_histograms.size();
      input_mutex.unlock();
      
      if(options.verbose){
        std::lock_guard<std::mutex> lock(command_line_mutex);
        cout << "[main] Number of histograms to process" << nInput << endl;
      }
      
      // If there are histogram still to process wait for 1 second and retry
      while(nInput>0){
        this_thread::sleep_for(chrono::seconds(1));
        
        input_mutex.lock();
        nInput = input_histograms.size();
        input_mutex.unlock();
        
        if(options.verbose){
          std::lock_guard<std::mutex> lock(command_line_mutex);
          cout << "[main] Number of histograms to process " << nInput << endl;
        }
      }
      
      for(unsigned i=0; i<nWorkers; i++){
        myOptimisers[i]->setTaskDone(true);
      }
      for(unsigned i=0; i<nWorkers; i++){
        myOptimisers[i]->join();
        delete myOptimisers[i];
      }
      
      if(options.verbose){
        cout << "[main] Optimisation workers done..." << endl;
      }
      
      worker.setTaskDone(true);
      worker.join();
      
      if(options.verbose){cout << "[main] I/O worker done..." << endl;}
      
      outputfile->Write();
      
      if(options.verbose){cout << "[main] ROOT output file written..." << endl;}
      
    }else if(options.inputType=="tree"){
      // ##############################################
      // ##############################################
      // ##############################################
      
      TTree *tree_sig = (TTree*) fSig->Get("L1TAlgoScan/Scan/L1TAlgoResults");
      TTree *tree_bkg = (TTree*) fBkg->Get("L1TAlgoScan/Scan/L1TAlgoResults");
      
      // Getting scan trees
      unsigned base_histograms_initial_size_efficiency = tree_sig->GetEntries();
      unsigned base_histograms_initial_size_rate       = tree_bkg->GetEntries();
      
      // Place holders for content
      vector<trgfw::L1TVariableScanDataFormat*> content_sig;
      vector<trgfw::L1TVariableScanDataFormat*> content_bkg;
      
      // Getting the list of branches
      TObjArray *myBranches = tree_sig->GetListOfBranches();
      
      // Pre-allocating space
      for(int iBranch=0; iBranch<myBranches->GetSize(); iBranch++){
        content_sig.push_back(new trgfw::L1TVariableScanDataFormat());
        content_bkg.push_back(new trgfw::L1TVariableScanDataFormat());
      }
      
      // Pre-allocating space
      for(int iBranch=0; iBranch<myBranches->GetSize(); iBranch++){
        tree_sig->SetBranchAddress((*myBranches)[iBranch]->GetName(),&(content_sig.at(iBranch)));
        tree_bkg->SetBranchAddress((*myBranches)[iBranch]->GetName(),&(content_bkg.at(iBranch)));
      }
      
      // Setting more things on CL worker
      worker.setBaseHistogramsInitialSize(base_histograms_initial_size_efficiency,base_histograms_initial_size_rate);
      
      if(options.verbose){cout << "[main] Optimisation workers starting" << endl;}    
      
      for(unsigned i=0; i<nWorkers; i++){
        myOptimisers[i]->start();
      }
      
      // Looping over all histograms
      for(Long64_t iEntry=0; iEntry<base_histograms_initial_size_rate ; iEntry++){
        
        tree_sig->GetEntry(iEntry);
        tree_bkg->GetEntry(iEntry);
      
        for(int iBranch=0; iBranch<myBranches->GetSize(); iBranch++){
          
          trgfw::InputScan *iData = new trgfw::InputScan;
          iData->path = content_sig[iBranch]->getPath();
          iData->name = content_sig[iBranch]->getName();
          
          // Cloning sig scan
          iData->hEfficiency = new trgfw::L1TVariableScanDataFormat();
          *(iData->hEfficiency) = (*content_sig[iBranch]);
          
          // Cloning bkg scan
          iData->hRate = new trgfw::L1TVariableScanDataFormat();
          *(iData->hRate) = (*content_bkg[iBranch]);
          
          // Adding to list scans to process
          input_mutex.lock();
          input_scans.push_back(iData);
          input_mutex.unlock();
          
        }
      }
    }
    
    for(unsigned i=0; i<nWorkers; i++){
      myOptimisers[i]->setTaskDone(true);
    }
    for(unsigned i=0; i<nWorkers; i++){
      myOptimisers[i]->join();
      delete myOptimisers[i];
    }
    
    if(options.verbose){cout << "[main] Optimisation workers done..." << endl;}    
    
    worker.setTaskDone(true);
    worker.join();
    
    if(options.verbose){cout << "[main] I/O worker done..." << endl;}
    
    outputfile->Write();
    
    if(options.verbose){cout << "[main] ROOT output file written..." << endl;}
    
    if(options.optimisation){
      
      FILE *file_output_optimisation;
      file_output_optimisation = fopen(Form("%s/results_Optimisation.txt",options.outputDir.c_str()), "w+");
      
      sort(workingPoints.begin(),workingPoints.end(),trgfw::sortGreaterEff_AlgoWorkingPoint());
      for(unsigned i=0; i<workingPoints.size(); i++){
        fprintf(file_output_optimisation,"Path: %80s Var: %30s Cut: %10.4f Eff: %6.4f Rate: %10.1f\n",
                workingPoints[i].name.c_str(),workingPoints[i].var.c_str(),workingPoints[i].value,workingPoints[i].eff,workingPoints[i].rate);
      }
      
      fclose(file_output_optimisation);
    }
    
    if(options.verbose){cout << "[main] Optimisation done." << endl;}
    
  }
  
  // Closing files
  fSig->Close();
  fBkg->Close();
  
  delete fSig;
  delete fBkg;
  
  if(options.verbose){cout << "[main] All done exiting!" << endl;}
  
  return 0;
  
}
