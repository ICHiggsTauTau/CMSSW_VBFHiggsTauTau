// VBF inc
#include "CMSSW_VBFHiggsTauTau/Style/interface/Style.h"
#include "CMSSW_VBFHiggsTauTau/IO/interface/File.h"

// ROOT includes
#include "TFile.h"
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
#include <functional>

using namespace std;

bool hasEnding (std::string const &fullString, std::string const &ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}

class ProgramOptions{
public:
  
  ProgramOptions(){
    valid     = false;
    verbose   = false;
    plots     = false;
    inputA    = "file";
    inputB    = "file";
    outputDir = "output/";
  }
  
  ProgramOptions(int argc, char *argv[]){
    valid=true;
    
    // Check the number of parameters
    if (argc < 2) {
      printHelpMessage(); 
      valid=false;
    }
    
    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];
      if     (arg == "--verbose"){verbose = true;}
      else if(arg == "--plots")  {plots   = true;}
      else if(arg == "--inputA"){
        if(i+1<argc){i++; inputA = argv[i];}
        else{printHelpMessage(); valid=false;}
      }
      else if(arg == "--inputB"){
        if(i+1<argc){i++; inputB = argv[i];}
        else{printHelpMessage(); valid=false;}
      }
      else if(arg == "--outputDir"){
        if(i+1<argc){i++; outputDir = argv[i];}
        else{printHelpMessage(); valid=false;}
      }
    }
    
  }
  
  void printHelpMessage(){
    cerr << "Usage: vbftautau_runRatesExtraction [commands]" << endl;
    cerr << " --verbose"                                     << endl;
    cerr << " --plots"                                       << endl;
    cerr << " --inputA INPUT"                                << endl;
    cerr << " --inputB INPUT"                                << endl;
    cerr << " --outputDir DIR"                               << endl;
  }
  
  bool   valid;
  bool   verbose;
  bool   plots;
  string inputA;
  string inputB;
  string outputDir;
};

class AlgoWorkingPoint{
public:
  
  AlgoWorkingPoint(){};
  
  std::string name;
  std::string var;
  double      value;
  double      rate;
  double      eff;
};

struct sortGreaterEff_AlgoWorkingPoint{
  bool operator() (AlgoWorkingPoint a,AlgoWorkingPoint b) {
    return (a.eff > b.eff);
  }
};

//#################################################################################
//#################################################################################
//#################################################################################
TCanvas* doCanvas(TH1D* sig,TH1D* bkg,const char* name,const char* path,vector<AlgoWorkingPoint> &resAlgos){
  
  //const int nMaxBunch50ns = 1380;
  const int nMaxBunch25ns = 2808;
  const int ratePerBunch  = 11246;
  
  TCanvas *c0 = new TCanvas(Form("rates_%s",name));
  
  TPad *pad = new TPad("pad","",0,0,1,1);
  pad->SetFillColor(0);
  pad->SetGridx();
  pad->SetTicky(false);
  pad->Draw();
  pad->cd();
  
  sig->GetYaxis()->SetTitleOffset(1.5);
  sig->GetYaxis()->SetTitle("Signal Efficiency");
  sig->Draw();
  
  ///////////
  c0->cd();
  TPad *overlay = new TPad("overlay","",0,0,1,1);
  overlay->SetFillStyle(4000);
  overlay->SetFillColor(0);
  overlay->SetFrameFillStyle(4000);
  overlay->Draw();
  overlay->cd();
  overlay->SetLogy();
  overlay->SetGridy();
  overlay->SetTicky(false);
  
  TH1D * rate = (TH1D*) bkg->Clone(Form("rate_%s",sig->GetTitle()));
//   if(string(name)=="l1t_etm"){rate->GetXaxis()->SetRangeUser(0,200);}
//   if(string(name)=="dijet_pt0 "){rate->GetXaxis()->SetTitle("Leading Jet p_{#perp}");}
  // rate->GetYaxis()->SetRangeUser(1,1.2e7);
  rate->GetYaxis()->SetTitle("L1T Rate (Hz)");
  rate->Scale(ratePerBunch*nMaxBunch25ns);
  rate->SetLineColor(kRed);
  rate->GetYaxis()->SetAxisColor(kRed);
  rate->GetYaxis()->SetTitleColor(kRed);
  rate->GetYaxis()->SetLabelColor(kRed);
  rate->GetYaxis()->SetTitleOffset(1.5);  
  rate->Draw("Y+");
  
  TLegend *l = new TLegend(0.50,0.80,0.85,0.95);
  l->SetBorderSize(1);
  l->AddEntry(sig,"VBF Inv Eff");

  l->AddEntry(rate, "Zero Bias");
  l->Draw();
  
  for(int i=0; i<=sig->GetNbinsX()+1; i++){
    if(sig->GetBinContent(i)>=0.2 && rate->GetBinContent(i)<=2100){
      
      AlgoWorkingPoint wp;
      wp.name  = path;
      wp.var   = sig->GetName();
      wp.value = sig->GetXaxis()->GetBinLowEdge(i);
      wp.rate  = rate->GetBinContent(i);
      wp.eff   = sig->GetBinContent(i);
      
      resAlgos.push_back(wp);
      break;
    }
  }
  
  return c0;
}

//#################################################################################
int main(int argc, char *argv[]){

  // Processing command line arguments
  ProgramOptions options(argc,argv);
  if(!options.valid){return 1;}
  
  // Setting style
  hepfw::Style myStyle;
  myStyle.setTDRStyle();
  
  // Making output file
  system(Form("mkdir -p %s",options.outputDir.c_str()));
  
  // Getting input files
  hepfw::File *fSig = new hepfw::File(options.inputA.c_str());
  hepfw::File *fBkg = new hepfw::File(options.inputB.c_str());
  
  // Getting all histograms
  vector<TH1*> hSig = fSig->getHistograms();
  vector<TH1*> hBkg = fBkg->getHistograms();
  
  TH1D* hSigTotal = (TH1D*) fSig->Get("EventCount");
  double nSigEvents = hSigTotal->GetBinContent(1);
  
  TH1D* hBkgTotal = (TH1D*) fBkg->Get("EventCount");
  double nBkgEvents = hBkgTotal->GetBinContent(1);

  cout << "Sig events: " << nSigEvents << endl;
  cout << "Bkg events: " << nBkgEvents << endl;
  
  
  printf("===== Decay Summary =====\n");
  TH1D* HiggsDecay = (TH1D*) fSig->Get("GenAnalysis/HiggsDecay");
  for(int i=1; i<HiggsDecay->GetNbinsX()+1; i++){
    printf("Decay: %10s - Faction: %6.4f\n",HiggsDecay->GetXaxis()->GetBinLabel(i),HiggsDecay->GetBinContent(i)/nSigEvents);
  }
  
  printf("===== Algo Efficiency Summary =====\n");
  TH1D* AlgoPass     = (TH1D*) fSig->Get("AlgoPass");
  TH1D* AlgoPassRate = (TH1D*) fBkg->Get("AlgoPass");
  for(int i=1; i<AlgoPass->GetNbinsX()+1; i++){
    printf("Algo: %40s - Eff: %6.4f rate: %10.1f Hz\n",AlgoPass->GetXaxis()->GetBinLabel(i),AlgoPass->GetBinContent(i)/nSigEvents,2808*11246*AlgoPassRate->GetBinContent(i)/nBkgEvents);
  }
  
  if(options.plots){
    
    vector<AlgoWorkingPoint> resAlgos;
    
    hepfw::File *fOut = new hepfw::File(Form("%s/L1Algo_eff.root",options.outputDir.c_str()),"RECREATE");
    fOut->copyDirectoryStructure(fSig);
    fOut->Write();
    
    for(unsigned s=0; s<hSig.size(); s++){
      
      TH1D *pSig = (TH1D*) hSig[s];
      
      string sigTitle = pSig->GetName();
      string sigPath  = pSig->GetDirectory()->GetPath();
      sigPath  = sigPath.substr(sigPath.find(':')+2,sigPath.size()-1);
      
      for(unsigned b=0; b<hBkg.size(); b++){
        
        TH1D *pBkg = (TH1D*) hBkg[b];
        
        string bkgTitle = pBkg->GetName();
        string bkgPath  = pBkg->GetDirectory()->GetPath();
        bkgPath = bkgPath.substr(bkgPath.find(':')+2,bkgPath.size()-1);
        
        if(sigTitle == bkgTitle && sigPath == bkgPath && sigPath != ""){
          
          // Histograms to ignore
          if(hasEnding(sigTitle,"_Eta")){continue;}
          if(hasEnding(sigTitle,"_Phi")){continue;}
          
          //cout << "Plot: " << sigPath << " name: " << pSig->GetName() << " title: " << pSig->GetTitle() << endl;
          
          TH1D *pSigEff   = (TH1D*) pSig->Clone(Form("Sig_%s_eff",pSig->GetName()));
          TH1D *pBkgEff   = (TH1D*) pBkg->Clone(Form("Bkg_%s_eff",pBkg->GetName()));
          
          bool reverseIntegral = false;
          
          // Plots that cut will be 'less than' so integral must be reversed
          //if(string(pSig->GetName()).find("dijet_dphi")       != std::string::npos){reverseIntegral=true;}
          //if(string(pSig->GetName()).find("jets_mindphi")     != std::string::npos){reverseIntegral=true;}
          //if(string(pSig->GetName()).find("jets_met_maxdphi") != std::string::npos){reverseIntegral=true;}
          
          if(reverseIntegral){
            for(int i=0; i<=pSigEff->GetNbinsX()+1; i++){pSigEff->SetBinContent(i,pSig->Integral(0,i));}
            for(int i=0; i<=pBkgEff->GetNbinsX()+1; i++){pBkgEff->SetBinContent(i,pSig->Integral(0,i));}
          }else{
            for(int i=0; i<=pSigEff->GetNbinsX()+1; i++){pSigEff->SetBinContent(i,pSigEff->Integral(i,pSigEff->GetNbinsX()+1));}
            for(int i=0; i<=pBkgEff->GetNbinsX()+1; i++){pBkgEff->SetBinContent(i,pBkgEff->Integral(i,pBkgEff->GetNbinsX()+1));}
          }
          pSigEff  ->Scale(1/nSigEvents);
          pBkgEff  ->Scale(1/nBkgEvents);
          
          TCanvas* c = doCanvas(pSigEff,pBkgEff,pSig->GetName(),sigPath.c_str(), resAlgos);
          
          TDirectoryFile *d  =0;
          if(sigPath==""){
            d = (TDirectoryFile*) fOut;
          }else{
            d = (TDirectoryFile*) fOut->Get(sigPath.c_str());
          }
          d->WriteTObject(c);
          
          //FIXME: directory handling
          system(Form("mkdir -p %s/%s",options.outputDir.c_str(),sigPath.c_str()));
          c->SaveAs(Form("%s/%s/%s.%s",options.outputDir.c_str(),sigPath.c_str(),pSig->GetName(),"png"));
          c->SaveAs(Form("%s/%s/%s.%s",options.outputDir.c_str(),sigPath.c_str(),pSig->GetName(),"pdf"));
          c->SaveAs(Form("%s/%s/%s.%s",options.outputDir.c_str(),sigPath.c_str(),pSig->GetName(),"C"));
          
          delete pSigEff;
          delete pBkgEff;
          delete c;
          
          break;
        }
      }  
      
    }
    
    cout << "Sig has " << hSig.size() << " histograms!" << endl;
    cout << "Bkg has " << hBkg.size() << " histograms!" << endl;
    
    fOut->Write();
    
    sort(resAlgos.begin(),resAlgos.end(),sortGreaterEff_AlgoWorkingPoint());
    printf("===== Algo results =====\n");
    for(unsigned i=0; i<resAlgos.size(); i++){
      printf("Path: %40s Var: %30s Cut: %10.4f Eff: %6.4f Rate: %10.1f\n",
             resAlgos[i].name.c_str(),resAlgos[i].var.c_str(),resAlgos[i].value,resAlgos[i].eff,resAlgos[i].rate);
    }
    
  }
  
  
  return 0;
  
};