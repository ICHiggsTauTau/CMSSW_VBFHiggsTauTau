// VBF inc
#include "CMSSW_VBFHiggsTauTau/Style/interface/Style.h"
#include "CMSSW_VBFHiggsTauTau/IO/interface/File.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoResults.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"

#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

//#################################################################################
//#################################################################################
//#################################################################################
class ProgramOptions{
public:
  
  ProgramOptions(){this->init();}
  
  ProgramOptions(int argc, char *argv[]){
    
    this->init();
    
    // Check the number of parameters
    if (argc < 2) {
      printHelpMessage(); 
      valid=false;
    }
  
    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];
      if     (arg == "--verbose"){verbose = true;}
      else if(outputFile==""){
        outputFile = argv[i];
      }else{
        inputFiles.push_back(argv[i]);
      }
    }
    
    if(inputFiles.size()==0){
      valid=false;
    }
    
  }
  
  void init(){
    valid      = true;
    verbose    = false;  
    outputFile = "";
  }
  
  void printHelpMessage(){
    cerr << "Usage: vbftautau_mergeOptimisationTrees [commands] OUTPUTFILE INPUTFILE1 INPUTFILE2 (...)" << endl;
    cerr << " --verbose"                                     << endl;
  }
  
  void print(){
    cout << "##### vbftautau_mergeOptimisationTrees #####" << endl;
    cout << "=> Program parameters:"   << endl;
    cout << "verbose     : " << verbose << endl;
    cout << "output file : " << outputFile << endl;
    cout << "input files : " << endl;
    for(unsigned i=0; i<inputFiles.size(); i++){
      cout << "==> #" << i << " - " << inputFiles[i] << endl;
    }
    cout << endl;
  }
  
  bool                     valid;
  bool                     verbose;
  std::string              outputFile;
  std::vector<std::string> inputFiles;
  
};



//#################################################################################
int main(int argc, char *argv[]){
  
  ProgramOptions options(argc,argv);
  if(!options.valid){
    cout << "[main] Options invalid... quiting!" << endl;
    options.printHelpMessage();
    return 1;
  }
  options.print();
  
  // The input files
  std::vector<hepfw::File*> vecFileIn;
  for(unsigned i=0; i<options.inputFiles.size(); i++){
    vecFileIn.push_back(new hepfw::File(options.inputFiles[i].c_str()));
  }
  
  // The output file
  hepfw::File* fileOut = new hepfw::File(options.outputFile.c_str(),"RECREATE");
  
  // Copying the directory structure to the output file
  fileOut->copyDirectoryStructure(vecFileIn[0]);
  
  // Getting all input histograms
  vector< vector<TH1*> > histogramsIn;
  for(unsigned i=0; i<options.inputFiles.size(); i++){
    histogramsIn.push_back(vecFileIn[i]->getHistograms());
  }

  // ### Finding matches ###
  vector< vector<TH1D*> > histogramsMatched;
  
  for(auto iHistogram=histogramsIn[0].begin(); iHistogram!=histogramsIn[0].end(); iHistogram++){
    
    vector<TH1D*> iMatch;
    iMatch.push_back((TH1D*) *iHistogram); // Need to cast
    
    bool foundAllMatch = true;
    string baseName = (*iHistogram)->GetName();                          // Getting histogram name
    string basePath = (*iHistogram)->GetDirectory()->GetPath();          // Getting histogram directory path
    basePath  = basePath.substr(basePath.find(':')+2,basePath.size()-1); // Removing file name for the path
    
    // Here we need to start the search from in a the second file
    for(auto iFile=histogramsIn.begin()+1; iFile<histogramsIn.end(); iFile++){
      
      bool foundXMatch = false;
      
      for(auto xHistogram=iFile->begin(); xHistogram!=iFile->end(); xHistogram++){
        
        string xName = (*xHistogram)->GetName();                 // Getting histogram name
        string xPath = (*xHistogram)->GetDirectory()->GetPath(); // Getting histogram directory path
        xPath  = xPath.substr(xPath.find(':')+2,xPath.size()-1); // Removing file name for the path
        
        if(baseName == xName && basePath == xPath){
          foundXMatch = true;                    // Found match for the histogram in this file
          iMatch.push_back((TH1D*) *xHistogram); // Adding match to the match vector (need to cast)
          break;                                 // We can stop searching in this file
        }
      }
      
      if(!foundXMatch){
        // This histogram was not found in at least one file
        foundAllMatch=false;
      }
    }
    
    // If we get up to here without foundAllMatch being false,
    // histograms exists in all available files
    if(foundAllMatch){
      histogramsMatched.push_back(iMatch);
    }
  }
  
  // Informing used of how many matched histograms were found
  cout << "[main] Found matches for " << histogramsMatched.size() << " histograms..." << endl;
  
  // Now we clone, add and place the resulting histograms in the output file
  for(auto iMatch=histogramsMatched.begin(); iMatch!=histogramsMatched.end(); iMatch++){
    
    TH1D* iHistogram = iMatch->at(0);
    
    string baseName = iHistogram->GetName();                             // Getting histogram name
    string basePath = iHistogram->GetDirectory()->GetPath();             // Getting histogram directory path
    basePath  = basePath.substr(basePath.find(':')+2,basePath.size()-1); // Removing file name for the path
    
    cout << "[main] Now adding path=" << basePath << " name=" << baseName << " nBins=" << iHistogram->GetNbinsX() << endl;
    
    // Placing this histogram in the output file
    TDirectory* dirOut = (TDirectory*) fileOut->Get(basePath.c_str());
    iHistogram->SetDirectory(dirOut);

    // Now adding in other histogramas
    for(auto iAddHistograms=iMatch->begin()+1; iAddHistograms<iMatch->end(); iAddHistograms++){
      iHistogram->Add(*iAddHistograms);
    }

  }
  
  // Getting the input histograms trees
  vector<TTree*> treeIn;
  for(unsigned i=0; i<vecFileIn.size(); i++){
    
    TTree *iTree = (TTree*) vecFileIn[i]->Get("L1TAlgoScan/Scan/L1TAlgoResults");
    //iTree->SetDirectory(0);
    iTree->SetName(Form("input%u",i));
    treeIn.push_back(iTree);
  }
  
  vector<string> branchesToProcess;
  branchesToProcess.push_back("L1TEGamma1_Et");
  //branchesToProcess.push_back("L1TEGamma1_Eta");
  //branchesToProcess.push_back("L1TEGamma1_Phi");
  branchesToProcess.push_back("L1TIsoEGamma1_Et");
  //branchesToProcess.push_back("L1TIsoEGamma1_Eta");
  //branchesToProcess.push_back("L1TIsoEGamma1_Phi");
  branchesToProcess.push_back("L1TMuon1_Et");
  //branchesToProcess.push_back("L1TMuon1_Eta");
  //branchesToProcess.push_back("L1TMuon1_Phi");
  branchesToProcess.push_back("L1TIsoMuon1_Et");
  //branchesToProcess.push_back("L1TIsoMuon1_Eta");
  branchesToProcess.push_back("L1TIsoMuon1_Phi");
  //branchesToProcess.push_back("L1TTau_N");
  branchesToProcess.push_back("L1TTau1_Et");
  //branchesToProcess.push_back("L1TTau1_Eta");
  //branchesToProcess.push_back("L1TTau1_Phi");
  branchesToProcess.push_back("L1TTau2_Et"); 
  //branchesToProcess.push_back("L1TTau2_Eta");
  //branchesToProcess.push_back("L1TTau2_Phi");
  //branchesToProcess.push_back("L1TIsoTau_N");
  branchesToProcess.push_back("L1TIsoTau1_Et");
  //branchesToProcess.push_back("L1TIsoTau1_Eta");
  //branchesToProcess.push_back("L1TIsoTau1_Phi");
  branchesToProcess.push_back("L1TIsoTau2_Et");
  //branchesToProcess.push_back("L1TIsoTau2_Eta");
  //branchesToProcess.push_back("L1TIsoTau2_Phi");
  //branchesToProcess.push_back("L1TJet_N");
  branchesToProcess.push_back("L1TJet_AvgPt");
  branchesToProcess.push_back("L1TJet_VecPt");
  branchesToProcess.push_back("L1TJet1_Et");
  //branchesToProcess.push_back("L1TJet1_Eta");
  //branchesToProcess.push_back("L1TJet1_Phi");
  branchesToProcess.push_back("L1TJet2_Et");
  //branchesToProcess.push_back("L1TJet2_Eta");
  //branchesToProcess.push_back("L1TJet2_Phi");
  branchesToProcess.push_back("L1TJet3_Et");
  branchesToProcess.push_back("L1TJet_maxMjj");
  branchesToProcess.push_back("L1TJet_maxDEta");
  //branchesToProcess.push_back("L1TTau1_minDRL1TJet");
  branchesToProcess.push_back("L1TMHT_Et");
  branchesToProcess.push_back("L1TMET_Et");
  
  // Pre-allocating space to avoid ROOT pointer to pointer issues
  // ISSUE: When using vectors (or any dynamic structure) of pointers
  // to allocate space for TTree branches, the pointers container cannot
  // be moved in memory (example: by resize or push_back) or ROOT will
  // fail to put information in the correct memory place.
  map<string, vector<trgfw::L1TVariableScanDataFormat*> > inputData;
  map<string, trgfw::L1TVariableScanDataFormat* >         outputData;
  for(unsigned iBranch=0; iBranch<branchesToProcess.size(); iBranch++){
    
    string iB = branchesToProcess[iBranch];
    
    // Creating the entry
    if(options.verbose){cout << "[main] Now going to create vector of pointers for branch=" << iB << endl;}
    inputData[iB] = vector<trgfw::L1TVariableScanDataFormat*>();
    
    // Allocating the space
    if(options.verbose){cout << "[main] Now allocating space" << endl;}
    for(unsigned iTree=0; iTree<treeIn.size(); iTree++){
      if(options.verbose){cout << "[main] => Allocating space i=" << iTree << endl;}
      inputData[iB].push_back(new trgfw::L1TVariableScanDataFormat());
    }
    
    // And now for the output
    outputData[iB] = new trgfw::L1TVariableScanDataFormat();
  }
  
  // Now setting all addresses for input data
  for(unsigned iBranch=0; iBranch<branchesToProcess.size(); iBranch++){
    string iB = branchesToProcess[iBranch];
      
    for(unsigned iTree=0; iTree<treeIn.size(); iTree++){
      if(options.verbose){cout << "[main] Setting branch address for branch=" << iB << " tree=" << treeIn[iTree]->GetName() << endl;}
      treeIn[iTree]->SetBranchAddress(iB.c_str(),&(inputData[iB].at(iTree)));
    }
  }
  
  // Setting output tree directory
  TDirectory* dirTreeOut = (TDirectory*) fileOut->Get("L1TAlgoScan/Scan");
  dirTreeOut->cd();
  
  // Cloning first file tree structure to output file
  TTree *treeOut = new TTree("L1TAlgoResults","L1TAlgoResults");
  treeOut->SetDirectory(dirTreeOut);
  
  for(unsigned iBranch=0; iBranch<branchesToProcess.size(); iBranch++){
    string iB = branchesToProcess[iBranch];
    treeOut->Branch(iB.c_str(),"trgfw::L1TVariableScanDataFormat",&(outputData[iB]),32000,3);
  }
  
  cout << "===== Tree Summary ===== " << endl;
  for(unsigned iTree=0; iTree<treeIn.size(); iTree++){
    cout << "Input Tree #" << iTree << " number of entries: " << treeIn[iTree]->GetEntries() << endl;
  }
  
  Long64_t nEntries = treeIn[0]->GetEntries();

  // Looping over all histograms
  for(Long64_t iEntry=0; iEntry<nEntries ; iEntry++){
    
    if(!(iEntry % 100)){cout << "[main] Processing histogram #" << iEntry << endl;}
    
    // Updating all trees
    for(unsigned iTree=0; iTree<treeIn.size(); iTree++){
      
      if(options.verbose){cout << "[main] Getting entry=" << iEntry << " on input tree #" << iTree << " name=" << treeIn[iTree]->GetName() << endl;}
      treeIn[iTree]->GetEntry(iEntry);
    }
  
    for(unsigned iBranch=0; iBranch<branchesToProcess.size(); iBranch++){
      string iB = branchesToProcess[iBranch];
    
      for(unsigned iTree=0; iTree<treeIn.size(); iTree++){
        
        if(iTree==0){
          *(outputData[iB]) = *(inputData[iB].at(iTree));
        }else{
          outputData[iB]->add(inputData[iB].at(iTree));
        }
      }
    }
    
    // All done now writting new tree
    treeOut->Fill();
  }
  
  //if(options.verbose){cout << "[main] Finished adding now writing output tree" << endl;}
  //treeOut->Write();
  
  if(options.verbose){cout << "[main] Writting and closing file" << endl;}
  
  // Writing and closing output file
  fileOut->Write();
  fileOut->Close();
  
  if(options.verbose){cout << "[main] Final memory cleaning" << endl;}
  
  // ### Cleaning memory ###
  for(unsigned i=0; i<vecFileIn.size(); i++){delete vecFileIn[i];}
  delete fileOut;
  
  return 0;
}
