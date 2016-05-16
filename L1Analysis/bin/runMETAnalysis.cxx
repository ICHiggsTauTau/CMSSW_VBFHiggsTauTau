// VBF inc
#include "CMSSW_VBFHiggsTauTau/Style/interface/Style.h"
#include "CMSSW_VBFHiggsTauTau/IO/interface/File.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"

#include <string>

using namespace std;

//#################################################################################
int main(int argc, char *argv[]){
  
  // Setting style
  hepfw::Style myStyle;
  myStyle.setTDRStyle();
  myStyle.tdrStyle->SetPadTopMargin   (0.05);
  myStyle.tdrStyle->SetPadBottomMargin(0.15);
  myStyle.tdrStyle->SetPadLeftMargin  (0.15);
  myStyle.tdrStyle->SetPadRightMargin (0.05);
  
  string codeTag = "l1t-integration-v34.0";
  TLatex *textCodeTag = new TLatex(0.60,0.96,codeTag.c_str());
  textCodeTag->SetNDC();
  textCodeTag->SetTextFont(44);
  textCodeTag->SetTextSize(20);
  textCodeTag->SetLineWidth(2);
  
  string sample = "VBF H#rightarrowInvisible";
  TLatex *textSample = new TLatex(0.16,0.96,sample.c_str());
  textSample->SetNDC();
  textSample->SetTextFont(44);
  textSample->SetTextSize(20);
  textSample->SetLineWidth(2);
  
  
  const double ratePerBunch     = 11246;
  const double run259721_lumi   = 0.15e34;
  const double run259721_nBunch = 517;
  const double targetLumi       = 1.15e34;
  double convFactor = ratePerBunch*run259721_nBunch*(targetLumi/run259721_lumi);
  
  // Getting input files
  hepfw::File *fData_METRange3p0 = new hepfw::File("ZeroBias_run259721_METEtaRange3p0.root");
  hepfw::File *fData_METRange5p0 = new hepfw::File("ZeroBias_run259721_METEtaRange5p0.root");
  
  hepfw::File *fInv_METRange3p0 = new hepfw::File("VBF_HToInvisible_METEtaRange3p0.root");
  hepfw::File *fInv_METRange5p0 = new hepfw::File("VBF_HToInvisible_METEtaRange5p0.root");
  
  // Getting Total event number
  TH1D* hData_METRange3p0_EventCount = (TH1D*) fData_METRange3p0->Get("L1TAlgoScan/EventCount");
  double nData_METRange3p0_EventCount = hData_METRange3p0_EventCount->GetBinContent(1);
  
  TH1D* hData_METRange5p0_EventCount = (TH1D*) fData_METRange5p0->Get("L1TAlgoScan/EventCount");
  double nData_METRange5p0_EventCount = hData_METRange5p0_EventCount->GetBinContent(1);
  
  //TH1D* hInv_METRange3p0 = (TH1D*) fInv_METRange3p0->Get("EventCount");
  //double nInv_METRange3p0_EventCount = hInv_METRange3p0->GetBinContent(1);
  
  TH1D* hInv_METRange5p0 = (TH1D*) fInv_METRange5p0->Get("L1TAlgoScan/EventCount");
  double nInv_METRange5p0_EventCount = hInv_METRange5p0->GetBinContent(1);
  
  // Getting MET plots
  TH1D* hData_METRange3p0 = (TH1D*) fData_METRange3p0->Get("L1TAlgoScan/SingleObject/L1TMet_Et");
  TH1D* hData_METRange5p0 = (TH1D*) fData_METRange5p0->Get("L1TAlgoScan/SingleObject/L1TMet_Et");
  
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
  
  c.SaveAs(Form("%s_METComparison_Rate.png",codeTag.c_str()));
  c.SaveAs(Form("%s_METComparison_Rate.pdf",codeTag.c_str()));
  
  // Getting MET plots
  TH1D* hData_MHT_METRange3p0 = (TH1D*) fData_METRange3p0->Get("L1TAlgoScan/SingleObject/L1TMHT_Et");
  TH1D* hData_MHT_METRange5p0 = (TH1D*) fData_METRange5p0->Get("L1TAlgoScan/SingleObject/L1TMHT_Et");
  
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
  
  c.SaveAs(Form("%s_MHTComparison_Rate.png",codeTag.c_str()));
  c.SaveAs(Form("%s_MHTComparison_Rate.pdf",codeTag.c_str()));
  
  // Getting 

  TH2D* hInv_MET_METRange5p0  = (TH2D*) fInv_METRange5p0 ->Get("L1TAlgoScan/Algos/NoClean/DijetAvg30deta25/METvsMjj");
  TH2D* hData_MET_METRange5p0 = (TH2D*) fData_METRange5p0->Get("L1TAlgoScan/Algos/NoClean/DijetAvg30deta25/METvsMjj");
  
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

  c.SaveAs(Form("%s_effInv_METvsMjj_METRange5p0.png",codeTag.c_str()));
  c.SaveAs(Form("%s_effInv_METvsMjj_METRange5p0.pdf",codeTag.c_str()));
  
  effData_MET_METRange5p0->GetYaxis()->SetTitleOffset(1.5);
  effData_MET_METRange5p0->GetXaxis()->SetTitle("L1T MET (GeV)");
  effData_MET_METRange5p0->GetXaxis()->SetRangeUser(0,200);
  effData_MET_METRange5p0->GetYaxis()->SetTitle("L1T Max Dijet Mjj (GeV)");
  effData_MET_METRange5p0->Draw("colz");
  
  c.SaveAs(Form("%s_rateInv_METvsMjj_METRange5p0.png",codeTag.c_str()));
  c.SaveAs(Form("%s_rateInv_METvsMjj_METRange5p0.pdf",codeTag.c_str()));
  
  //#################################################################################
  TH1D* hInv_L1TETM_Resolution_METRange3p0  = (TH1D*) fInv_METRange3p0 ->Get("L1TResolutions/L1TETM_Resolution");
  TH1D* hInv_L1TETM_Resolution_METRange5p0  = (TH1D*) fInv_METRange5p0 ->Get("L1TResolutions/L1TETM_Resolution");
  
  c.SetFillColor(0);
  c.SetGridx();
  c.SetLogy(false);
  c.SetTicky(false);

  hInv_L1TETM_Resolution_METRange3p0->GetXaxis()->SetTitle("#frac{MET_{L1T} - MET_{Gen}}{MET_{Gen}}");
  hInv_L1TETM_Resolution_METRange3p0->GetXaxis()->SetTitleOffset(1.5);  
  hInv_L1TETM_Resolution_METRange3p0->GetXaxis()->SetRangeUser(-1.0,1.0);
  hInv_L1TETM_Resolution_METRange3p0->GetYaxis()->SetTitle("Arbitrary Units");
  hInv_L1TETM_Resolution_METRange3p0->GetYaxis()->SetTitleOffset(2.0);
  hInv_L1TETM_Resolution_METRange3p0->Rebin(2);
  hInv_L1TETM_Resolution_METRange3p0->Scale(1/hInv_L1TETM_Resolution_METRange3p0->GetEntries());
  hInv_L1TETM_Resolution_METRange3p0->SetLineWidth(2);
  hInv_L1TETM_Resolution_METRange3p0->GetYaxis()->SetRangeUser(0,0.04);
  hInv_L1TETM_Resolution_METRange3p0->Draw();

  hInv_L1TETM_Resolution_METRange5p0->Rebin(2);
  hInv_L1TETM_Resolution_METRange5p0->Scale(1/hInv_L1TETM_Resolution_METRange5p0->GetEntries());
  hInv_L1TETM_Resolution_METRange5p0->SetLineWidth(2);
  hInv_L1TETM_Resolution_METRange5p0->SetLineColor(kRed);
  hInv_L1TETM_Resolution_METRange5p0->Draw("same");
  
  l = new TLegend(0.60,0.80,0.95,0.95);
  l->SetBorderSize(1);
  l->AddEntry(hInv_L1TETM_Resolution_METRange3p0,"VBF H#rightarrowInv (no HF)"  );
  l->AddEntry(hInv_L1TETM_Resolution_METRange5p0,"VBF H#rightarrowInv (with HF)");
  l->Draw();
  
  textSample ->Draw();
  textCodeTag->Draw();
  
  c.SaveAs(Form("%s_L1TETM_Resolution.png",codeTag.c_str()));
  c.SaveAs(Form("%s_L1TETM_Resolution.pdf",codeTag.c_str()));
  
  //#################################################################################
  TProfile* hInv_L1TETM_ResolutionVsGenPt_METRange3p0  = (TProfile*) fInv_METRange3p0 ->Get("L1TResolutions/L1TETM_ResolutionVsEt");
  TProfile* hInv_L1TETM_ResolutionVsGenPt_METRange5p0  = (TProfile*) fInv_METRange5p0 ->Get("L1TResolutions/L1TETM_ResolutionVsEt");
  
  c.SetFillColor(0);
  c.SetGridx(false);
  c.SetGridy(true);
  c.SetLogy(false);
  c.SetTicky(false);

  hInv_L1TETM_ResolutionVsGenPt_METRange3p0->GetXaxis()->SetTitleOffset(1.5);
  hInv_L1TETM_ResolutionVsGenPt_METRange3p0->GetXaxis()->SetTitle("MET_{Gen} [GeV]");
  hInv_L1TETM_ResolutionVsGenPt_METRange3p0->GetYaxis()->SetTitleOffset(2.0);
  hInv_L1TETM_ResolutionVsGenPt_METRange3p0->GetYaxis()->SetTitleSize(0.03);
  hInv_L1TETM_ResolutionVsGenPt_METRange3p0->GetYaxis()->SetTitle("#frac{MET_{L1T} - MET_{Gen}}{MET_{Gen}}");
  hInv_L1TETM_ResolutionVsGenPt_METRange3p0->GetYaxis()->SetRangeUser(-0.4,0.4);
  hInv_L1TETM_ResolutionVsGenPt_METRange3p0->SetLineWidth(2);
  hInv_L1TETM_ResolutionVsGenPt_METRange3p0->Draw("E1");
  
  hInv_L1TETM_ResolutionVsGenPt_METRange5p0->SetLineWidth(2);
  hInv_L1TETM_ResolutionVsGenPt_METRange5p0->SetLineColor(kRed);
  hInv_L1TETM_ResolutionVsGenPt_METRange5p0->Draw("E1 same");
  
  l = new TLegend(0.60,0.80,0.95,0.95);
  l->SetBorderSize(1);
  l->AddEntry(hInv_L1TETM_ResolutionVsGenPt_METRange3p0,"VBF H#rightarrowInv (no HF)"  );
  l->AddEntry(hInv_L1TETM_ResolutionVsGenPt_METRange5p0,"VBF H#rightarrowInv (with HF)");
  l->Draw();
  
  textSample ->Draw();
  textCodeTag->Draw();
  
  c.SaveAs(Form("%s_L1TETM_ResolutionVsGenPt.png",codeTag.c_str()));
  c.SaveAs(Form("%s_L1TETM_ResolutionVsGenPt.pdf",codeTag.c_str()));
  
  return 0;
  
};