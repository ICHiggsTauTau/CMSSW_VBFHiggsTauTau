// CMSSW includes
#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisEventDataFormat.h"

// CMSSW_VBFHiggsTauTau includes
#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/interface/VBFHiggsToTauTauGenAnalysisDataFormat.h"
#include "CMSSW_VBFHiggsTauTau/GenParticlesAnalysis/interface/MicroGenParticle.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TAlgoAnalysis.h"

// ICHiggsTauTau Objects
#include "UserCode/ICHiggsTauTau/interface/L1TObject.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TEGamma.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TMuon.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TTau.hh"
#include "UserCode/ICHiggsTauTau/interface/L1TJet.hh"

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
#include <iostream>
#include <fstream>
#include <cstdio>
#include <map>
#include <vector>

using namespace std;

struct greater_ICCandidate{
  bool operator() (ic::Candidate a,ic::Candidate b) {
    return (a.pt() > b.pt());
  }
};

struct ProgramOptions{
  ProgramOptions(){
    verbose        = false;
    inputType      = "file";
    input          = "input.root";
    maxEvents      = -1;
    jobType        = "signal";
    outputFilename = "L1Analysis.root";
    
  }

  bool     verbose;
  string   inputType;
  string   input;
  Long64_t maxEvents; 
  string   jobType;
  string   outputFilename;
};

void printHelpMessage(){
  
  cerr << "Usage: vbftautau_runTriggerAnalysis [commands]"    << endl;
  cerr << " --verbose         "                               << endl;
  cerr << " --inputType TYPE       options: file, filelist"   << endl;
  cerr << " --input INPUT"                                    << endl;
  cerr << " --maxEvents N"                                    << endl;
  cerr << " --jobType TYPE         options: signal, zerobias" << endl;
  cerr << " --outputFilename NAME"                            << endl;  
}

bool processArgs(int argc, char* argv[], ProgramOptions &opt){
  
  // Check the number of parameters
  if (argc < 2) {
    printHelpMessage();
    return 0;
  }
  
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if     (arg == "--verbose")  {opt.verbose  = true;}
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
    }else if(arg == "--jobType"){
      if(i+1<argc){i++; opt.jobType = argv[i];}
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
  cout << "verbose        = " << options.verbose   << endl;
  cout << "inputType      = " << options.inputType << endl;
  cout << "input          = " << options.input     << endl;
  cout << "maxEvents      = " << options.maxEvents << endl;
  cout << "jobType        = " << options.jobType   << endl;
  cout << "outputFilename = " << options.outputFilename   << endl;

  // Stuff
  vector<string> vSig;
  if(options.inputType=="file"){
    vSig.push_back(options.input);
  }else if(options.inputType=="filelist"){
    string line;
    ifstream myfile (options.input);
    if (myfile.is_open()){
      while ( getline(myfile,line) ){
        vSig.push_back(line);
      }
      myfile.close();
    }
  }
  
  L1TAlgoAnalysis myAnalysis;
  myAnalysis.setVerbose(options.verbose);
  myAnalysis.setDoSingleObjectsAnalysis(true);
  myAnalysis.initPlots();
  
  // Stuff
  TFile *fOut         = new TFile(options.outputFilename.c_str(),"RECREATE");
  TH1D  *m_EventCount = new TH1D("EventCount","EventCount",1, 0.5,1.5); m_EventCount->SetDirectory(fOut);
  TH1D  *m_HiggsDecay = new TH1D("HiggsDecay","HiggsDecay",6, 0.5,6.5); m_HiggsDecay->SetDirectory(fOut);
  m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::EleEle,"EleEle");
  m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::EleMuo,"EleMuo");
  m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::EleHad,"EleHad");
  m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::MuoMuo,"MuoMuo");
  m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::MuoHad,"MuoHad");
  m_HiggsDecay->GetXaxis()->SetBinLabel(VBFHiggsToTauTau::HiggsDecay::HadHad,"HadHad");
  
  
  cout << "===== Open File ===== " << endl;
  //TFile *t = TFile::Open(vSig[0].c_str());
  TChain treeEvent      ("l1EventTree/L1EventTree");
  TChain treeL1Upgrade  ("l1UpgradeTree/L1UpgradeTree");
  TChain treeGenAnalysis("decayAnalyzer/VBFHiggsToTauTauGenAnalysisTree");
  cout << endl;
  
  //fOut->ls();
  cout << "===== Objects ===== " << endl;
  L1Analysis::L1AnalysisEventDataFormat *productL1Event = new L1Analysis::L1AnalysisEventDataFormat();
  treeEvent.SetBranchAddress("Event",&productL1Event);
  //fOut->ls();
  
  L1Analysis::L1AnalysisL1UpgradeDataFormat *productL1Upgrade = new L1Analysis::L1AnalysisL1UpgradeDataFormat();
  treeL1Upgrade.SetBranchAddress("L1Upgrade",&productL1Upgrade);
  
  VBFHiggsToTauTau::GenAnalysisDataFormat *genInfo = new VBFHiggsToTauTau::GenAnalysisDataFormat();
  treeGenAnalysis.SetBranchAddress("GenAnalysis",&genInfo);
  cout << endl;

  
  for(unsigned i=0; i<vSig.size(); i++){
     cout << "Input file #" << i << " - " << vSig[i] << endl;
     treeEvent      .Add(vSig[i].c_str());
     treeL1Upgrade  .Add(vSig[i].c_str());
     treeGenAnalysis.Add(vSig[i].c_str());
  }
  
  
  cout << "===== Tree Summary ===== " << endl;
  Long64_t entries_VBFHiggsToTauTau = treeGenAnalysis.GetEntries();
  cout << "Number of entries: " << entries_VBFHiggsToTauTau << endl;
  cout << endl;
  
  // If maxEvents if -1 this means run over all events
  if(options.maxEvents==-1){options.maxEvents=entries_VBFHiggsToTauTau;}
  
  for(Long64_t ev=0; ev<entries_VBFHiggsToTauTau && ev<options.maxEvents; ev++){
    
    m_EventCount->Fill(1);   // Counting the current event
    
    if(!(ev % 10000)){cout << "Processing events #" << ev << endl;}
    
    treeEvent.LoadTree(ev);
    treeEvent.GetEntry(ev);
    
    treeL1Upgrade.LoadTree(ev);
    treeL1Upgrade.GetEntry(ev);
    
    treeGenAnalysis.LoadTree(ev);
    treeGenAnalysis.GetEntry(ev);
    
    vector<ic::L1TEGamma> myL1TEGamma;
    unsigned short int nL1TEGamma = productL1Upgrade->nEGs;
    for(unsigned i=0; i<nL1TEGamma; i++){
      
      if(productL1Upgrade->egBx[i] != 0){continue;}
      
      ic::L1TEGamma thisEG;
      thisEG.set_pt (productL1Upgrade->egEt [i]);
      thisEG.set_eta(productL1Upgrade->egEta[i]);
      thisEG.set_phi(productL1Upgrade->egPhi[i]);
      myL1TEGamma.push_back(thisEG);
    }
    
    vector<ic::L1TMuon> myL1TMuon;
    unsigned short int nL1TMuon = productL1Upgrade->nMuons;
    for(unsigned i=0; i<nL1TMuon; i++){
      
      if(productL1Upgrade->muonBx[i] != 0){continue;}
      
      ic::L1TMuon thisMuon;
      thisMuon.set_pt (productL1Upgrade->muonEt [i]);
      thisMuon.set_eta(productL1Upgrade->muonEta[i]);
      thisMuon.set_phi(productL1Upgrade->muonPhi[i]);
      myL1TMuon.push_back(thisMuon);
    }
    
    vector<ic::L1TTau> myL1TTaus;
    unsigned short int nL1TTaus = productL1Upgrade->nTaus;
    for(unsigned i=0; i<nL1TTaus; i++){
      
      if(productL1Upgrade->tauBx[i] != 0){continue;}
 
      ic::L1TTau thisTau;
      thisTau.set_pt (productL1Upgrade->tauEt [i]);
      thisTau.set_eta(productL1Upgrade->tauEta[i]);
      thisTau.set_phi(productL1Upgrade->tauPhi[i]);
      myL1TTaus.push_back(thisTau);
    }
    
    vector<ic::L1TJet> myL1TJets;
    unsigned short int nL1TJets = productL1Upgrade->nJets;
    for(unsigned i=0; i<nL1TJets; i++){
      
      if(productL1Upgrade->jetBx[i] != 0){continue;}
      
      ic::L1TJet thisJet;
      thisJet.set_pt (productL1Upgrade->jetEt [i]);
      thisJet.set_eta(productL1Upgrade->jetEta[i]);
      thisJet.set_phi(productL1Upgrade->jetPhi[i]);
      myL1TJets.push_back(thisJet);
    }

    if(options.verbose){printf("===== Sorting Collections =====\n");}
    sort(myL1TEGamma.begin(),myL1TEGamma.end(),greater_ICCandidate());
    sort(myL1TMuon  .begin(),myL1TMuon  .end(),greater_ICCandidate());
    sort(myL1TTaus  .begin(),myL1TTaus  .end(),greater_ICCandidate());
    sort(myL1TJets  .begin(),myL1TJets  .end(),greater_ICCandidate());
    
    //myAnalysis
    myAnalysis.setL1TEGammaCollection(&myL1TEGamma);
    myAnalysis.setL1TMuonCollection  (&myL1TMuon);
    myAnalysis.setL1TTauCollection   (&myL1TTaus);
    myAnalysis.setL1TJetCollection   (&myL1TJets);
    myAnalysis.processEvent();
    myAnalysis.resetEvent();
    
    string strHiggsDecay="";
    if     (genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::EleEle){strHiggsDecay="EleEle";}
    else if(genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::EleMuo){strHiggsDecay="EleMuo";}
    else if(genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::EleHad){strHiggsDecay="EleHad";}
    else if(genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::MuoMuo){strHiggsDecay="MuoMuo";}
    else if(genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::MuoHad){strHiggsDecay="MuoHad";}
    else if(genInfo->higgs_decayType==VBFHiggsToTauTau::HiggsDecay::HadHad){strHiggsDecay="HadHad";}
    
    m_HiggsDecay->Fill(genInfo->higgs_decayType);
    
    if(options.verbose){
      printf("Higgs decay type: %s (%u)\n",strHiggsDecay.c_str(),genInfo->higgs_decayType);
      printf("Tau decay 1     : %u\n", genInfo->tau1_decayType);
      printf("Tau decay 1 size: %lu\n",genInfo->tau1_stableDecayProducts.size());
      printf("Tau decay 2     : %u\n", genInfo->tau2_decayType);
      printf("Tau decay 2 size: %lu\n",genInfo->tau2_stableDecayProducts.size());
    }
  }
  
  fOut->Write();
  
  return 0;
}