// VBF includes
#include "CMSSW_VBFHiggsTauTau/IO/interface/File.h"
#include "CMSSW_VBFHiggsTauTau/Style/interface/Style.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/AlgoWorkingPoint.h"

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
  
  void init(){
    valid        = true;
    verbose      = false;
    plots        = false;
    optimisation = false;
    signal       = "tautau";
    inputA       = "file";
    inputB       = "file";
    outputDir    = "output/";
  }
  
  void printHelpMessage(){
    cerr << "Usage: vbftautau_runRatesExtraction [commands]" << endl;
    cerr << " --verbose"                                     << endl;
    cerr << " --plots"                                       << endl;
    cerr << " --optimisation"                                << endl;
    cerr << " --signal CHANNEL"                              << endl;
    cerr << " --inputA INPUT"                                << endl;
    cerr << " --inputB INPUT"                                << endl;
    cerr << " --outputDir DIR"                               << endl;
  }
  
  void print(){
    cout << "##### vbftautau_runRatesExtraction #####" << endl;
    cout << "=> Program parameters:" << endl;
    cout << "verbose   : " << verbose   << endl;
    cout << "signal    : " << signal    << endl;
    cout << "inputA    : " << inputA    << endl;
    cout << "inputB    : " << inputB    << endl;
    cout << "outputDir : " << outputDir << endl;
    cout << "=> Tasks to perform:" << endl;
    cout << "optimisation : " << optimisation << endl;
    cout << "plots        : " << plots << endl;
    cout << endl;
  }
  
  bool   valid;
  bool   verbose;
  bool   plots;
  bool   optimisation;
  string signal;
  string inputA;
  string inputB;
  string outputDir;
};





//#################################################################################
//#################################################################################
//#################################################################################
TCanvas* doCanvas(TH1D* sig,TH1D* bkg,const char* name,const char* path,vector<trgfw::AlgoWorkingPoint> &resAlgos){
  
  double run259721_convFactor = getConvertionFactor(259721);
  
  TCanvas *c0 = new TCanvas(Form("rates_%s",name));
  
  TPad *pad = new TPad("pad","",0,0,1,1);
  pad->SetFillColor(0);
  pad->SetGridx();
  pad->SetTicky(false);
  pad->Draw();
  pad->cd();
  
  if(string(name)=="L1TMet_Et"){sig ->GetXaxis()->SetRangeUser(0,200);}
  if(string(name)=="L1TMHT_Et"){sig ->GetXaxis()->SetRangeUser(0,200);}
  
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
  if(string(name)=="L1TMet_Et"){rate->GetXaxis()->SetRangeUser(0,200);}
  if(string(name)=="L1TMHT_Et"){rate->GetXaxis()->SetRangeUser(0,200);}
  //   if(string(name)=="dijet_pt0 "){rate->GetXaxis()->SetTitle("Leading Jet p_{#perp}");}
  // rate->GetYaxis()->SetRangeUser(1,1.2e7);
  rate->GetYaxis()->SetTitle("L1T Rate (Hz)");
  rate->Scale(run259721_convFactor);
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
    if(sig->GetBinContent(i)>=0.1 && rate->GetBinContent(i)<=1100){
      
      trgfw::AlgoWorkingPoint wp;
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
  hepfw::File *fSig = new hepfw::File(options.inputA.c_str());
  hepfw::File *fBkg = new hepfw::File(options.inputB.c_str());
  
  
  
  // Getting all histograms
  vector<TH1*> hSig = fSig->getHistograms();
  vector<TH1*> hBkg = fBkg->getHistograms();
  
  TH1D* hSigTotal = (TH1D*) fSig->Get("L1TAlgoScan/EventCount");
  double nSigEvents = hSigTotal->GetBinContent(1);
  
  TH1D* hBkgTotal = (TH1D*) fBkg->Get("L1TAlgoScan/EventCount");
  double nBkgEvents = hBkgTotal->GetBinContent(1);

  cout << "Sig events: " << nSigEvents << endl;
  cout << "Bkg events: " << nBkgEvents << endl;
  
  
  printf("===== Decay Summary =====\n");
  TH1D* HiggsDecay = (TH1D*) fSig->Get("L1TAlgoScan/GenAnalysis/HiggsDecay");
  for(int i=1; i<HiggsDecay->GetNbinsX()+1; i++){
    printf("Decay: %10s - Faction: %6.4f\n",HiggsDecay->GetXaxis()->GetBinLabel(i),HiggsDecay->GetBinContent(i)/nSigEvents);
  }

  printf("===== Convertion factor =====\n");
  double run259721_convFactor = getConvertionFactor(259721);
  printf("Data convertion factor: %10.2f\n",run259721_convFactor);
  printf("\n");
  
  printf("===== Algo Efficiency Summary =====\n");
  TH1D* AlgoPass     = (TH1D*) fSig->Get("L1TAlgoScan/AlgoPass");
  TH1D* AlgoPassRate = (TH1D*) fBkg->Get("L1TAlgoScan/AlgoPass");
  for(int i=1; i<AlgoPass->GetNbinsX()+1; i++){
    printf("Algo: %40s - Eff: %6.4f rate: %10.1f Hz\n",AlgoPass->GetXaxis()->GetBinLabel(i),AlgoPass->GetBinContent(i)/nSigEvents,run259721_convFactor*AlgoPassRate->GetBinContent(i)/nBkgEvents);
  }
  
  if(options.optimisation || options.plots){
    
    vector<trgfw::AlgoWorkingPoint> resAlgos;
    
    hepfw::File *fOut = new hepfw::File(Form("%s/L1Algo_eff.root",options.outputDir.c_str()),"RECREATE");
    fOut->copyDirectoryStructure(fSig);
    fOut->Write();
    
    for(unsigned s=0; s<hSig.size(); s++){
      
      
      TH1D *pSig = (TH1D*) hSig[s];
      
      string sigTitle = pSig->GetName();
      string sigPath  = pSig->GetDirectory()->GetPath();

      // Generic histograms to ignore
      if(sigTitle=="EventCount")            {continue;}
      if(sigTitle=="AlgoPass")              {continue;}
      if(hasEnding(sigTitle,"_N"))          {continue;}
      if(hasEnding(sigTitle,"_Eta"))        {continue;}
      if(hasEnding(sigTitle,"_Phi"))        {continue;}
      if(hasEnding(sigTitle,"_minDRL1TJet")){continue;}
      if(hasEnding(sigTitle,"_METvsMjj"))   {continue;}
      
      // Histograms to ignore for VBF Higgs to Invisible optimisation
      if(options.signal=="inv"){
        if(sigTitle.find("L1TEGamma")!=std::string::npos){continue;}
        if(sigTitle.find("L1TMuon")  !=std::string::npos){continue;}
        if(sigTitle.find("L1TTau")   !=std::string::npos){continue;}
        if(sigTitle.find("L1TIsoTau")!=std::string::npos){continue;}
      }
      
      sigPath  = sigPath.substr(sigPath.find(':')+2,sigPath.size()-1);
      
      for(unsigned b=0; b<hBkg.size(); b++){
        
        TH1D *pBkg = (TH1D*) hBkg[b];
        
        string bkgTitle = pBkg->GetName();
        string bkgPath  = pBkg->GetDirectory()->GetPath();
        bkgPath = bkgPath.substr(bkgPath.find(':')+2,bkgPath.size()-1);
        
        if(sigTitle == bkgTitle && sigPath == bkgPath && sigPath != ""){
          
          std::cout << "Processing histogram (" << s << " out of " << hSig.size() << "): " << sigPath << " " << sigTitle << '\r' << std::flush;
          
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
          
          if(options.plots){
          
            //FIXME: directory handling
            system(Form("mkdir -p %s/%s",options.outputDir.c_str(),sigPath.c_str()));
            c->SaveAs(Form("%s/%s/%s.%s",options.outputDir.c_str(),sigPath.c_str(),pSig->GetName(),"png"));
            c->SaveAs(Form("%s/%s/%s.%s",options.outputDir.c_str(),sigPath.c_str(),pSig->GetName(),"pdf"));
            c->SaveAs(Form("%s/%s/%s.%s",options.outputDir.c_str(),sigPath.c_str(),pSig->GetName(),"C"));
          }
          
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
    
    if(options.optimisation){
      sort(resAlgos.begin(),resAlgos.end(),trgfw::sortGreaterEff_AlgoWorkingPoint());
      printf("===== Algo results =====\n");
      for(unsigned i=0; i<resAlgos.size(); i++){
        printf("Path: %40s Var: %30s Cut: %10.4f Eff: %6.4f Rate: %10.1f\n",
               resAlgos[i].name.c_str(),resAlgos[i].var.c_str(),resAlgos[i].value,resAlgos[i].eff,resAlgos[i].rate);
      }
    }
  }
  
  
  return 0;
  
};