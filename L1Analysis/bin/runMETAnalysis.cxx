// VBF inc
#include "CMSSW_VBFHiggsTauTau/Style/interface/Style.h"
#include "CMSSW_VBFHiggsTauTau/IO/interface/File.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"

//#################################################################################
int main(int argc, char *argv[]){
  
  // Setting style
  hepfw::Style myStyle;
  myStyle.setTDRStyle();
  
  const double ratePerBunch     = 11246;
  const double run259721_lumi   = 0.15e34;
  const double run259721_nBunch = 517;
  const double targetLumi       = 1.15e34;
  double convFactor = ratePerBunch*run259721_nBunch*(targetLumi/run259721_lumi);
  
  // Getting input files
  hepfw::File *fData_METRange3p0 = new hepfw::File("ZeroBias_run259721_METEtaRange3p0.root");
  hepfw::File *fData_METRange5p0 = new hepfw::File("ZeroBias_run259721_METEtaRange5p0.root");
  
  hepfw::File *fInv_METRange5p0 = new hepfw::File("VBF_HToInvisible_METEtaRange5p0.root");
  
  // Getting Total event number
  TH1D* hData_METRange3p0_EventCount = (TH1D*) fData_METRange3p0->Get("EventCount");
  double nData_METRange3p0_EventCount = hData_METRange3p0_EventCount->GetBinContent(1);
  
  TH1D* hData_METRange5p0_EventCount = (TH1D*) fData_METRange5p0->Get("EventCount");
  double nData_METRange5p0_EventCount = hData_METRange5p0_EventCount->GetBinContent(1);
  
  TH1D* hInv_METRange5p0 = (TH1D*) fInv_METRange5p0->Get("EventCount");
  double nInv_METRange5p0_EventCount = hInv_METRange5p0->GetBinContent(1);
  
  // Getting MET plots
  TH1D* hData_METRange3p0 = (TH1D*) fData_METRange3p0->Get("SingleObject/L1TMet_Et");
  TH1D* hData_METRange5p0 = (TH1D*) fData_METRange5p0->Get("SingleObject/L1TMet_Et");
  
  TH1D *effData_METRange3p0 = (TH1D*) hData_METRange3p0->Clone("effData_METRange3p0");
  TH1D *effData_METRange5p0 = (TH1D*) hData_METRange5p0->Clone("effData_METRange5p0");
  
  for(int i=0; i<=effData_METRange3p0->GetNbinsX()+1; i++){effData_METRange3p0->SetBinContent(i,effData_METRange3p0->Integral(i,effData_METRange3p0->GetNbinsX()+1));}
  for(int i=0; i<=effData_METRange5p0->GetNbinsX()+1; i++){effData_METRange5p0->SetBinContent(i,effData_METRange5p0->Integral(i,effData_METRange5p0->GetNbinsX()+1));}
  
  effData_METRange3p0->Scale(1/nData_METRange3p0_EventCount);
  effData_METRange5p0->Scale(1/nData_METRange5p0_EventCount);
  
  TCanvas c;
  c.SetFillColor(0);
  c.SetGridx();
  c.SetLogy();
  c.SetTicky(false);
  
  effData_METRange3p0->GetYaxis()->SetTitleOffset(1.5);
  effData_METRange3p0->GetXaxis()->SetTitle("L1T MET (GeV)");
  effData_METRange3p0->GetXaxis()->SetRangeUser(0,200);
  effData_METRange3p0->GetYaxis()->SetTitle("L1T Rate (Hz)");
  effData_METRange3p0->SetLineColor(kRed);
  effData_METRange3p0->SetLineWidth(2);
  effData_METRange3p0->Scale(convFactor);
  effData_METRange3p0->Draw();
  
  effData_METRange5p0->SetLineColor(kGreen);
  effData_METRange5p0->SetLineWidth(2);
  effData_METRange5p0->Scale(convFactor);
  effData_METRange5p0->Draw("same");
  
  TLegend *l = new TLegend(0.50,0.80,0.85,0.95);
  l->SetBorderSize(1);
  l->AddEntry(effData_METRange3p0,"Without HF");
  l->AddEntry(effData_METRange5p0,"With HF");
  l->Draw();
  
  c.SaveAs("METComparison_Rate.png");
  c.SaveAs("METComparison_Rate.pdf");
  
  // Getting MET plots
  TH1D* hData_MHT_METRange3p0 = (TH1D*) fData_METRange3p0->Get("SingleObject/L1TMHT_Et");
  TH1D* hData_MHT_METRange5p0 = (TH1D*) fData_METRange5p0->Get("SingleObject/L1TMHT_Et");
  
  TH1D *effData_MHT_METRange3p0 = (TH1D*) hData_MHT_METRange3p0->Clone("effData_MHT_METRange3p0");
  TH1D *effData_MHT_METRange5p0 = (TH1D*) hData_MHT_METRange5p0->Clone("effData_MHT_METRange5p0");
  
  for(int i=0; i<=effData_MHT_METRange3p0->GetNbinsX()+1; i++){effData_MHT_METRange3p0->SetBinContent(i,effData_MHT_METRange3p0->Integral(i,effData_MHT_METRange3p0->GetNbinsX()+1));}
  for(int i=0; i<=effData_MHT_METRange5p0->GetNbinsX()+1; i++){effData_MHT_METRange5p0->SetBinContent(i,effData_MHT_METRange5p0->Integral(i,effData_MHT_METRange5p0->GetNbinsX()+1));}
  
  effData_MHT_METRange3p0->Scale(1/nData_METRange3p0_EventCount);
  effData_MHT_METRange5p0->Scale(1/nData_METRange5p0_EventCount);
  
  c.SetFillColor(0);
  c.SetGridx();
  c.SetLogy();
  c.SetTicky(false);
  
  effData_MHT_METRange3p0->GetYaxis()->SetTitleOffset(1.5);
  effData_MHT_METRange3p0->GetXaxis()->SetTitle("L1T MHT (GeV)");
  effData_MHT_METRange3p0->GetXaxis()->SetRangeUser(0,200);
  effData_MHT_METRange3p0->GetYaxis()->SetTitle("L1T Rate (Hz)");
  effData_MHT_METRange3p0->SetLineColor(kRed);
  effData_MHT_METRange3p0->SetLineWidth(2);
  effData_MHT_METRange3p0->Scale(convFactor);
  effData_MHT_METRange3p0->Draw();
  
  effData_MHT_METRange5p0->SetLineColor(kGreen);
  effData_MHT_METRange5p0->SetLineWidth(2);
  effData_MHT_METRange5p0->Scale(convFactor);
  effData_MHT_METRange5p0->Draw("same");
  
  l = new TLegend(0.50,0.80,0.85,0.95);
  l->SetBorderSize(1);
  l->AddEntry(effData_MHT_METRange3p0,"Without HF");
  l->AddEntry(effData_MHT_METRange5p0,"With HF");
  l->Draw();
  
  c.SaveAs("MHTComparison_Rate.png");
  c.SaveAs("MHTComparison_Rate.pdf");
  
  // Getting 

  TH2D* hInv_MET_METRange5p0  = (TH2D*) fInv_METRange5p0 ->Get("Algos/NoClean/DijetAvg30deta25/METvsMjj");
  TH2D* hData_MET_METRange5p0 = (TH2D*) fData_METRange5p0->Get("Algos/NoClean/DijetAvg30deta25/METvsMjj");
  
  TH2D *effInv_MET_METRange5p0  = (TH2D*) hInv_MET_METRange5p0 ->Clone("effInv_METvsMjj_METRange5p0");
  TH2D *effData_MET_METRange5p0 = (TH2D*) hData_MET_METRange5p0->Clone("effData_METvsMjj_METRange5p0");
  
  
  for(int x=0; x<=effData_MET_METRange5p0->GetNbinsX()+1; x++){
    for(int y=0; y<=effData_MET_METRange5p0->GetNbinsY()+1; y++){
      effData_MET_METRange5p0->SetBinContent(x,y,hData_MET_METRange5p0->Integral(x,hData_MET_METRange5p0->GetNbinsX()+1,y,hData_MET_METRange5p0->GetNbinsY()+1));
    }
  }
  
  effData_MET_METRange5p0->Scale(convFactor/nData_METRange5p0_EventCount);
  
  for(int x=0; x<=effInv_MET_METRange5p0->GetNbinsX()+1; x++){
    for(int y=0; y<=effInv_MET_METRange5p0->GetNbinsY()+1; y++){
      if(effData_MET_METRange5p0->GetBinContent(x,y)<1800 || effData_MET_METRange5p0->GetBinContent(x,y)>2200){
        effInv_MET_METRange5p0 ->SetBinContent(x,y,0);
      }
      else{
        effInv_MET_METRange5p0 ->SetBinContent(x,y,hInv_MET_METRange5p0 ->Integral(x,hInv_MET_METRange5p0 ->GetNbinsX()+1,y,hInv_MET_METRange5p0 ->GetNbinsY()+1));
      }
    }
  }
  
  effInv_MET_METRange5p0 ->Scale(1/nInv_METRange5p0_EventCount);
  
  c.SetFillColor(0);
  c.SetGridx();
  c.SetLogy(false);
  c.SetTicky(false);
  
  effInv_MET_METRange5p0->GetYaxis()->SetTitleOffset(1.5);
  effInv_MET_METRange5p0->GetXaxis()->SetTitle("L1T MET (GeV)");
  effInv_MET_METRange5p0->GetXaxis()->SetRangeUser(0,60);
  effInv_MET_METRange5p0->GetYaxis()->SetTitle("L1T Max Dijet Mjj (GeV)");
  effInv_MET_METRange5p0->Draw("colz");

  c.SaveAs("effInv_METvsMjj_METRange5p0.png");
  c.SaveAs("effInv_METvsMjj_METRange5p0.pdf");
  
  effData_MET_METRange5p0->GetYaxis()->SetTitleOffset(1.5);
  effData_MET_METRange5p0->GetXaxis()->SetTitle("L1T MET (GeV)");
  effData_MET_METRange5p0->GetXaxis()->SetRangeUser(0,200);
  effData_MET_METRange5p0->GetYaxis()->SetTitle("L1T Max Dijet Mjj (GeV)");
  effData_MET_METRange5p0->Draw("colz");
  
  c.SaveAs("rateInv_METvsMjj_METRange5p0.png");
  c.SaveAs("rateInv_METvsMjj_METRange5p0.pdf");
  
  return 0;
  
};