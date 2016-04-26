#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/PlotsSingleObjects.h"

#include "TMath.h"

using namespace std;

PlotsSingleObjects::PlotsSingleObjects(){
  
  m_L1TEGamma_N   = 0;
  m_L1TEGamma_Et  = 0;
  m_L1TEGamma_Eta = 0;
  m_L1TEGamma_Phi = 0;
  
  m_L1TMuon_N   = 0;
  m_L1TMuon_Et  = 0;
  m_L1TMuon_Eta = 0;
  m_L1TMuon_Phi = 0;
  
  m_L1TTau_N   = 0;
  m_L1TTau_Et  = 0;
  m_L1TTau_Eta = 0;
  m_L1TTau_Phi = 0;
  
  m_L1TJet_N   = 0;
  m_L1TJet_Et  = 0;
  m_L1TJet_Eta = 0;
  m_L1TJet_Phi = 0;
  
  m_L1TMet_Et  = 0;
  m_L1TMet_Phi = 0;
  
  m_L1TMHT_Et  = 0;
  m_L1TMHT_Phi = 0;
}

PlotsSingleObjects::~PlotsSingleObjects(){
  
}

void PlotsSingleObjects::create(TDirectory *dir){
  
  m_L1TEGamma_N   = new TH1D("L1TEGamma_N",  "L1T Tau N",    21,        -0.5,       20.5); m_L1TEGamma_N  ->SetDirectory(dir);
  m_L1TEGamma_Et  = new TH1D("L1TEGamma_Et", "L1T Tau Et",  250,           0,        250); m_L1TEGamma_Et ->SetDirectory(dir);
  m_L1TEGamma_Eta = new TH1D("L1TEGamma_Eta","L1T Tau Eta", 100,          -5,          5); m_L1TEGamma_Eta->SetDirectory(dir);
  m_L1TEGamma_Phi = new TH1D("L1TEGamma_Phi","L1T Tau Phi", 100,-TMath::Pi(),TMath::Pi()); m_L1TEGamma_Phi->SetDirectory(dir);
  
  m_L1TMuon_N     = new TH1D("L1TMuon_N",    "L1T Muon N",   21,        -0.5,       20.5); m_L1TMuon_N    ->SetDirectory(dir);
  m_L1TMuon_Et    = new TH1D("L1TMuon_Et",   "L1T Muon Et", 250,           0,        250); m_L1TMuon_Et   ->SetDirectory(dir);
  m_L1TMuon_Eta   = new TH1D("L1TMuon_Eta",  "L1T Muon Eta",100,          -5,          5); m_L1TMuon_Eta  ->SetDirectory(dir);
  m_L1TMuon_Phi   = new TH1D("L1TMuon_Phi",  "L1T Muon Phi",100,-TMath::Pi(),TMath::Pi()); m_L1TMuon_Phi  ->SetDirectory(dir);
  
  m_L1TTau_N      = new TH1D("L1TTau_N",     "L1T Tau N",    21,        -0.5,       20.5); m_L1TTau_N     ->SetDirectory(dir);
  m_L1TTau_Et     = new TH1D("L1TTau_Et",    "L1T Tau Et",  250,           0,        250); m_L1TTau_Et    ->SetDirectory(dir);
  m_L1TTau_Eta    = new TH1D("L1TTau_Eta",   "L1T Tau Eta", 100,          -5,          5); m_L1TTau_Eta   ->SetDirectory(dir);
  m_L1TTau_Phi    = new TH1D("L1TTau_Phi",   "L1T Tau Phi", 100,-TMath::Pi(),TMath::Pi()); m_L1TTau_Phi   ->SetDirectory(dir);
  
  m_L1TJet_N      = new TH1D("L1TJet_N",     "L1T Jet N",    21,        -0.5,       20.5); m_L1TJet_N     ->SetDirectory(dir);
  m_L1TJet_Et     = new TH1D("L1TJet_Et",    "L1T Jet Et",  250,           0,        250); m_L1TJet_Et    ->SetDirectory(dir);
  m_L1TJet_Eta    = new TH1D("L1TJet_Eta",   "L1T Jet Eta", 100,          -5,          5); m_L1TJet_Eta   ->SetDirectory(dir);
  m_L1TJet_Phi    = new TH1D("L1TJet_Phi",   "L1T Jet Phi", 100,-TMath::Pi(),TMath::Pi()); m_L1TJet_Phi   ->SetDirectory(dir);
  
  m_L1TMet_Et     = new TH1D("L1TMet_Et",    "L1T Met Et",   512,           0,        512); m_L1TMet_Et    ->SetDirectory(dir);
  m_L1TMet_Phi    = new TH1D("L1TMet_Phi",   "L1T Met Phi",  100,-TMath::Pi(),TMath::Pi()); m_L1TMet_Phi   ->SetDirectory(dir);
  
  m_L1TMHT_Et     = new TH1D("L1TMHT_Et",    "L1T MHT Et",   512,           0,        512); m_L1TMHT_Et    ->SetDirectory(dir);
  m_L1TMHT_Phi    = new TH1D("L1TMHT_Phi",   "L1T MHT Phi",  100,-TMath::Pi(),TMath::Pi()); m_L1TMHT_Phi   ->SetDirectory(dir);
  
}
