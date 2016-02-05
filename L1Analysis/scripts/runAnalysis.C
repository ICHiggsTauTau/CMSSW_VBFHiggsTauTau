#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "TMath.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphAsymmErrors.h"


double deltaPhi(double phi1,double phi2)
{
  double result = phi1 - phi2;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
};

double deltaR(double eta1,double phi1,double eta2,double phi2)
{
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return std::sqrt(deta*deta + dphi*dphi);
};

void getL1RecoMatches(const unsigned nL1,
		      const unsigned nRec,
		      const std::vector<float>& L1eta,
		      const std::vector<float>& L1phi,
		      const double* Receta,
		      const double* Recphi,
		      std::vector<std::pair<int,double> > & recomatch){

  recomatch.resize(nL1,std::pair<int,double>(-1,1000));

  for (unsigned ij(0); ij<nL1; ++ij){
    double eta1 = L1eta[ij];
    double phi1 = L1phi[ij];
    double mindR = 1000;
    for (unsigned jj(0); jj<nRec; ++jj){
      double eta2 = Receta[jj];
      double phi2 = Recphi[jj];
      double dR = deltaR(eta1,phi1,eta2,phi2);
      if (dR<mindR) {
	mindR=dR;
	recomatch[ij].first = jj;
	recomatch[ij].second = mindR;
      }
    }
  }

};

struct JetPair {
  bool hasrecomatch;
  double pt1;
  double pt2;
  double eta1;
  double eta2;
  double mjj;
  double deta;
  double pt1rec;
  double pt2rec;
  double eta1rec;
  double eta2rec;
  double mjjrec;
  double detarec;
};

int runAnalysis(){

  unsigned debug = 0;
  double mindRmatch = 0.4;

  TFile *fout = TFile::Open("OutputHistos.root","RECREATE");
  if (!fout){
    std::cout << " Cannot open output file..." << std::endl;
    return 1;
  }
  fout->cd();
  //met histos
  TH2F *hMET_recvsL1 = new TH2F("hMET_recvsL1",";MET^{L1} (GeV);MET^{Reco} (GeV);events",100,0,500,100,0,500);
  TH2F *hMET_recvsgen = new TH2F("hMET_recvsgen",";MET^{Gen} (GeV);MET^{Reco} (GeV);events",100,0,500,100,0,500);

  //jet histos
  TH2F *hpT1_recvsL1 = new TH2F("hpT1_recvsL1",";p_{T}^{L1j1} (GeV);p_{T}^{Recoj1} (GeV);jet pairs",100,0,300,100,0,300);
  TH2F *hpT2_recvsL1 = new TH2F("hpT2_recvsL1",";p_{T}^{L1j2} (GeV);p_{T}^{Recoj2} (GeV);jet pairs",100,0,300,100,0,300);
  TH2F *heta1_recvsL1 = new TH2F("heta1_recvsL1",";p_{T}^{L1j1} (GeV);p_{T}^{Recoj1} (GeV);jet pairs",100,-5,5,100,-5,5);
  TH2F *heta2_recvsL1 = new TH2F("heta2_recvsL1",";p_{T}^{L1j2} (GeV);p_{T}^{Recoj2} (GeV);jet pairs",100,-5,5,100,-5,5);
  TH2F *hmjj_recvsL1 = new TH2F("hmjj_recvsL1",";M_{jj}^{L1} (GeV);M_{jj}^{Reco} (GeV);jet pairs",100,0,5000,100,0,5000);
  TH2F *hdeta_recvsL1 = new TH2F("hdeta_recvsL1",";#Delta#eta_{jj}^{L1};#Delta#eta_{jj}^{Reco};jet pairs",100,0,8,100,0,8);

  TH1F *hpT1_match = new TH1F("hpT1_match",";p_{T}^{j1} (GeV);jet pairs",100,0,300);
  TH1F *hpT2_match = new TH1F("hpT2_match",";p_{T}^{j2} (GeV);jet pairs",100,0,300);
  TH1F *heta1_match = new TH1F("heta1_match",";p_{T}^{j1} (GeV);jet pairs",100,-5,5);
  TH1F *heta2_match = new TH1F("heta2_match",";p_{T}^{j2} (GeV);jet pairs",100,-5,5);
  TH1F *hMjj_match = new TH1F("hMjj_match",";M_{jj} (GeV);jet pairs",100,0,5000);
  TH1F *hdeta_match = new TH1F("hdeta_match",";#Delta#eta_{jj};jet pairs",100,0,8);

  TH1F *hpT1_all = new TH1F("hpT1_all",";p_{T}^{j1} (GeV);jet pairs",100,0,300);
  TH1F *hpT2_all = new TH1F("hpT2_all",";p_{T}^{j2} (GeV);jet pairs",100,0,300);
  TH1F *heta1_all = new TH1F("heta1_all",";p_{T}^{j1} (GeV);jet pairs",100,-5,5);
  TH1F *heta2_all = new TH1F("heta2_all",";p_{T}^{j2} (GeV);jet pairs",100,-5,5);
  TH1F *hMjj_all = new TH1F("hMjj_all",";M_{jj} (GeV);jet pairs",100,0,5000);
  TH1F *hdeta_all = new TH1F("hdeta_all",";#Delta#eta_{jj};jet pairs",100,0,8);




  TFile *fL1 = TFile::Open("l1t_stage1_vbfh.root");
  if (!fL1) {
    std::cout << " L1 file not found." << std::endl;
    return 1;
  }

  //get L1 event number
  fL1->cd("l1EventTree");
  TTree *tL1evt = (TTree*)gDirectory->Get("L1EventTree");
  if (!tL1evt) {
    std::cout << " L1 event tree not found." << std::endl;
    return 1;
  }
  tL1evt->SetMakeClass(1); //<===== important

  Int_t l1event=0;
  tL1evt->SetBranchAddress("event",&l1event);


  fL1->cd();
  fL1->cd("l1UpgradeTree");
  TTree *tL1 = (TTree*)gDirectory->Get("L1UpgradeTree");
  if (!tL1) {
    std::cout << " L1 tree not found." << std::endl;
    return 1;
  }
  tL1->SetMakeClass(1); //<===== important

  //tL1->Print();

  //short sumType[1000];
  //float sumEt[1000];
  UShort_t nJets = 0;
  //float jetEt[1000];
  std::vector<float> jetEt;
  jetEt.resize(1000,0);
  std::vector<float> jetEta;
  jetEta.resize(1000,0);
  std::vector<float> jetPhi;
  jetPhi.resize(1000,0);

  UShort_t nSums = 0;
  std::vector<short> sumType;
  sumType.resize(4,0);
  std::vector<float> sumEt;
  sumEt.resize(4,0);


  tL1->SetBranchAddress("nJets",&nJets);
  tL1->SetBranchAddress("jetEt",&jetEt);
  tL1->SetBranchAddress("jetPhi",&jetPhi);
  tL1->SetBranchAddress("jetEta",&jetEta);

  tL1->SetBranchAddress("nSums",&nSums);
  tL1->SetBranchAddress("sumType",&sumType);
  tL1->SetBranchAddress("sumEt",&sumEt);

  TFile *freco = TFile::Open("VBFH125inv_ntuple_test.root");
  if (!freco) {
    std::cout << " Reco file not found." << std::endl;
    return 1;
  }
  freco->cd("icEventProducer");
  TTree *treco = (TTree*)gDirectory->Get("EventTree");
  if (!treco) {
    std::cout << " Reco tree not found." << std::endl;
    return 1;
  }
  treco->SetMakeClass(1); //<===== important
  ULong64_t event = 0;
  TBranch *brevt = treco->GetBranch("event_");
  if (brevt) brevt->SetAddress(&event);
  else {
    std::cout << "Branch event_ not found" << std::endl;
    return 1;
  }
  //treco->SetBranchAddress("event_",&event);
  Int_t nMets = 0;
  Int_t ngenMets = 0;

  Double_t recomet;
  Double_t genmet;
  Int_t nRecoJets = 0;
  Double_t recoJetPt[1000];
  Double_t recoJetEta[1000];
  Double_t recoJetPhi[1000];

  unsigned recomatchId[1000];
  unsigned l1matchId[1000];

  TBranch *brnmets = treco->GetBranch("pfMetType1Collection");
  if (brnmets) brnmets->SetAddress(&nMets);
  else {
    std::cout << "Branch pfMetType1Collection not found" << std::endl;
    return 1;
  }

  TBranch *brmet = treco->GetBranch("pfMetType1Collection.vector_.fCoordinates.fPt");
  if (brmet) brmet->SetAddress(&recomet);
  else {
    std::cout << "Branch pfMetType1Collection.vector_.fCoordinates.fPt not found" << std::endl;
    return 1;
  }

  TBranch *brngenmets = treco->GetBranch("genMetCollection");
  if (brngenmets) brngenmets->SetAddress(&ngenMets);
  else {
    std::cout << "Branch genMetCollection not found" << std::endl;
    return 1;
  }
  
  TBranch *brgenmet = treco->GetBranch("genMetCollection.vector_.fCoordinates.fPt");
  if (brgenmet) brgenmet->SetAddress(&genmet);
  else {
    std::cout << "Branch genMetCollection.vector_.fCoordinates.fPt  not found" << std::endl;
    return 1;
  }
  
  TBranch *brnjets = treco->GetBranch("pfJetsPFlow");
  if (brnjets) brnjets->SetAddress(&nRecoJets);
  else {
    std::cout << "Branch pfJetsPFlow not found" << std::endl;
    return 1;
  }
 
  TBranch *brjetpt = treco->GetBranch("pfJetsPFlow.vector_.fCoordinates.fPt");
  if (brjetpt) brjetpt->SetAddress(recoJetPt);
  else {
    std::cout << "Branch pfJetsPFlow.vector_.fCoordinates.fPt not found" << std::endl;
    return 1;
  }
  TBranch *brjeteta = treco->GetBranch("pfJetsPFlow.vector_.fCoordinates.fEta");
  if (brjeteta) brjeteta->SetAddress(recoJetEta);
  else {
    std::cout << "Branch pfJetsPFlow.vector_.fCoordinates.fEta not found" << std::endl;
    return 1;
  }

  TBranch *brjetphi = treco->GetBranch("pfJetsPFlow.vector_.fCoordinates.fPhi");
  if (brjetphi) brjetphi->SetAddress(recoJetPhi);
  else {
    std::cout << "Branch pfJetsPFlow.vector_.fCoordinates.fPhi not found" << std::endl;
    return 1;
  }

  //treco->SetBranchAddress("pfMetType1Collection.vector_.fCoordinates.fPt",recomet);
  //treco->SetBranchAddress("genMetCollection.vector_.fCoordinates.fPt",genmet);
  
  //treco->SetBranchAddress("pfJetsPFlow_",&nRecoJets);
  //treco->SetBranchAddress("pfJetsPFlow.vector_.fCoordinates.fPt",recoJetPt);
  //treco->SetBranchAddress("pfJetsPFlow.vector_.fCoordinates.fEta",recoJetEta);
  //treco->SetBranchAddress("pfJetsPFlow.vector_.fCoordinates.fPhi",recoJetPhi);
  
  //treco->Print();

  const unsigned nL1Evt = tL1evt->GetEntries();
  const unsigned nL1 = tL1->GetEntries();
  const unsigned nReco = treco->GetEntries();
  std::cout << " L1 tree contains " << nL1Evt << " " << nL1 << " events." << std::endl;
  std::cout << " Reco tree contains " << nReco << " events." << std::endl;

  for (unsigned ievt(0); ievt<nL1Evt;++ievt){//loop on l1evts
    if (ievt%100==0) std::cout << ievt <<":";
    if (ievt%1000==0) std::cout << std::endl;
    tL1evt->GetEntry(ievt);
    if (debug>0) std::cout << " evt " << ievt << " L1Evt " << l1event ;
    //std::cout << std::endl;
    //tL1evt->Show();

    //treco->GetEntry(ievt);
    unsigned recoevt = 0;
    for (; recoevt<nReco;++recoevt){//loop on recoevts
      brevt->GetEntry(recoevt);
      //treco->Show();
      if (event==static_cast<unsigned>(l1event)) break;
    }
    if (debug>0) std::cout << " reco event " << event ;
    if (debug>0) std::cout << std::endl;

    tL1->GetEntry(ievt);
    //tL1->Show();
    if (debug>0){
      std::cout << "L1: number of jets:" << nJets ;
      std::cout << ", pT0:" <<  jetEt[0];
      std::cout << ", nSums=" << nSums << " type[0] " << static_cast<int>(sumType[0]) << " MET: " <<  sumEt[0];
      
      std::cout << std::endl;
    }

    brnjets->GetEntry(recoevt);
    if (debug>0) std::cout << " njets " << nRecoJets;
    brjetpt->GetEntry(recoevt);
    brjeteta->GetEntry(recoevt);
    brjetphi->GetEntry(recoevt);
    if (debug>0){
      std::cout << " pT0=" << recoJetPt[0]; 
      std::cout << std::endl;
    }
    brnmets->GetEntry(recoevt);
    brmet->GetEntry(recoevt);
    brngenmets->GetEntry(recoevt);    
    brgenmet->GetEntry(recoevt);
    if (debug>0){
      std::cout << " nMets = " << nMets;
      std::cout << " ngenMets = " << ngenMets;
      std::cout << " MET = " << recomet;
      std::cout << " genMET = " << genmet;
      std::cout << std::endl;
    }


    hMET_recvsL1->Fill(sumEt[0],recomet);
    hMET_recvsgen->Fill(genmet,recomet);

    std::vector<std::pair<int,double> > l1recomatch;
    getL1RecoMatches(nJets,nRecoJets,jetEta,jetPhi,recoJetEta,recoJetPhi,l1recomatch);

    //make pairs and fill 2D histos
    std::vector<JetPair> pairVec;
    for (unsigned ij(0); ij<nJets; ++ij){
      for (unsigned jj(ij+1); jj<nJets; ++jj){
	JetPair lPair;
	lPair.pt1 = jetEt[ij];
	lPair.pt2 = jetEt[jj];
	lPair.eta1 = jetEta[ij];
	lPair.eta2 = jetEta[jj];
	lPair.mjj = sqrt(2*jetEt[ij]*jetEt[jj]*(cosh(jetEta[ij]-jetEta[jj])-cos(jetPhi[ij]-jetPhi[jj])));
	lPair.deta = fabs(jetEta[ij]-jetEta[jj]);
	lPair.hasrecomatch = l1recomatch[ij].second<mindRmatch && l1recomatch[jj].second<mindRmatch;
	if (lPair.hasrecomatch) {
	  unsigned rec1 = l1recomatch[ij].first;
	  unsigned rec2 = l1recomatch[jj].first;
	  lPair.pt1rec = recoJetPt[rec1];
	  lPair.pt2rec = recoJetPt[rec2];
	  lPair.eta1rec = recoJetEta[rec1];
	  lPair.eta2rec = recoJetEta[rec2];
	  lPair.mjjrec = sqrt(2*recoJetPt[rec1]*recoJetPt[rec2]*(cosh(recoJetEta[rec1]-recoJetEta[rec2])-cos(recoJetPhi[rec1]-recoJetPhi[rec2])));
	  lPair.detarec = fabs(recoJetEta[rec1]-recoJetEta[rec2]);
	  //fill histos
	  hpT1_recvsL1->Fill(lPair.pt1,lPair.pt1rec);
	  hpT2_recvsL1->Fill(lPair.pt2,lPair.pt2rec);
	  heta1_recvsL1->Fill(lPair.eta1,lPair.eta1rec);
	  heta2_recvsL1->Fill(lPair.eta2,lPair.eta2rec);
	  hmjj_recvsL1->Fill(lPair.mjj,lPair.mjjrec);
	  hdeta_recvsL1->Fill(lPair.deta,lPair.detarec);
	  hpT1_match->Fill(lPair.pt1);
	  hpT2_match->Fill(lPair.pt2);
	  heta1_match->Fill(lPair.eta1);
	  heta2_match->Fill(lPair.eta2);
	  hMjj_match->Fill(lPair.mjj);
	  hdeta_match->Fill(lPair.deta);
	}
	pairVec.push_back(lPair);
	hpT1_all->Fill(lPair.pt1);
	hpT2_all->Fill(lPair.pt2);
	heta1_all->Fill(lPair.eta1);
	heta2_all->Fill(lPair.eta2);
	hMjj_all->Fill(lPair.mjj);
	hdeta_all->Fill(lPair.deta);
      }
    }


  }//loop on l1evts


  fout->cd();
  TGraphAsymmErrors *grEff_pT1 = new TGraphAsymmErrors();
  TGraphAsymmErrors *grEff_pT2 = new TGraphAsymmErrors();
  TGraphAsymmErrors *grEff_eta1 = new TGraphAsymmErrors();
  TGraphAsymmErrors *grEff_eta2 = new TGraphAsymmErrors();
  TGraphAsymmErrors *grEff_Mjj = new TGraphAsymmErrors();
  TGraphAsymmErrors *grEff_deta = new TGraphAsymmErrors();
  grEff_pT1->SetName("grEff_pT1");
  grEff_pT2->SetName("grEff_pT2");
  grEff_eta1->SetName("grEff_eta1");
  grEff_eta2->SetName("grEff_eta2");
  grEff_Mjj->SetName("grEff_Mjj");
  grEff_deta->SetName("grEff_deta");

  grEff_pT1->Divide(hpT1_match,hpT1_all);
  grEff_pT2->Divide(hpT2_match,hpT2_all);
  grEff_eta1->Divide(heta1_match,heta1_all);
  grEff_eta2->Divide(heta2_match,heta2_all);
  grEff_Mjj->Divide(hMjj_match,hMjj_all);
  grEff_deta->Divide(hdeta_match,hdeta_all);

  grEff_pT1->Write();
  grEff_pT2->Write();
  grEff_eta1->Write();
  grEff_eta2->Write();
  grEff_Mjj->Write();
  grEff_deta->Write();

  fout->Write();

  return 0;

}//main
