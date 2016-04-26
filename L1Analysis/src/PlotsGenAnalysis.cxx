#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/PlotsGenAnalysis.h"

#include "TMath.h"

using namespace std;

PlotsGenAnalysis::PlotsGenAnalysis(){

  m_channels.push_back("EleEle");
  m_channels.push_back("EleMuo");
  m_channels.push_back("EleHad");
  m_channels.push_back("MuoMuo");
  m_channels.push_back("MuoHad");
  m_channels.push_back("HadHad");
  
  m_HiggsDecay            = 0;
  
  for(unsigned i=0; i<m_channels.size(); i++){
    m_L1Tau1_ResolutionEt[m_channels[i]]  = 0;
    m_L1Tau2_ResolutionEt[m_channels[i]]  = 0;
    
    m_L1Tau1_ResolutionEta[m_channels[i]] = 0;
    m_L1Tau2_ResolutionEta[m_channels[i]] = 0;
    
    m_L1Tau1_ResolutionPhi[m_channels[i]] = 0;
    m_L1Tau2_ResolutionPhi[m_channels[i]] = 0;
  }

}

PlotsGenAnalysis::~PlotsGenAnalysis(){

}

void PlotsGenAnalysis::create(TDirectory *dir){
  
  m_HiggsDecay = new TH1D("HiggsDecay","HiggsDecay",6, 0.5,6.5); m_HiggsDecay->SetDirectory(dir);
  m_HiggsDecay->GetXaxis()->SetBinLabel(1,"EleEle");
  m_HiggsDecay->GetXaxis()->SetBinLabel(2,"EleMuo");
  m_HiggsDecay->GetXaxis()->SetBinLabel(3,"EleHad");
  m_HiggsDecay->GetXaxis()->SetBinLabel(4,"MuoMuo");
  m_HiggsDecay->GetXaxis()->SetBinLabel(5,"MuoHad");
  m_HiggsDecay->GetXaxis()->SetBinLabel(6,"HadHad");
  
  for(unsigned i=0; i<m_channels.size(); i++){
    m_L1Tau1_ResolutionEt[m_channels[i]]  = new TH1D(Form("L1Tau1_%s_ResolutionEt",m_channels[i].c_str()),Form("L1Tau1_%s_ResolutionEt",m_channels[i].c_str()),100,-50,50); m_L1Tau1_ResolutionEt[m_channels[i]]->SetDirectory(dir);
    m_L1Tau2_ResolutionEt[m_channels[i]]  = new TH1D(Form("L1Tau2_%s_ResolutionEt",m_channels[i].c_str()),Form("L1Tau2_%s_ResolutionEt",m_channels[i].c_str()),100,-50,50); m_L1Tau2_ResolutionEt[m_channels[i]]->SetDirectory(dir);
    
    m_L1Tau1_ResolutionEta[m_channels[i]] = new TH1D(Form("L1Tau1_%s_ResolutionEta",m_channels[i].c_str()),Form("L1Tau1_%s_ResolutionEta",m_channels[i].c_str()),40,-1,1); m_L1Tau1_ResolutionEta[m_channels[i]]->SetDirectory(dir);
    m_L1Tau2_ResolutionEta[m_channels[i]] = new TH1D(Form("L1Tau2_%s_ResolutionEta",m_channels[i].c_str()),Form("L1Tau2_%s_ResolutionEta",m_channels[i].c_str()),40,-1,1); m_L1Tau2_ResolutionEta[m_channels[i]]->SetDirectory(dir);
    
    m_L1Tau1_ResolutionPhi[m_channels[i]] = new TH1D(Form("L1Tau1_%s_ResolutionPhi",m_channels[i].c_str()),Form("L1Tau1_%s_ResolutionPhi",m_channels[i].c_str()),100,-TMath::Pi()/10,TMath::Pi()/10); m_L1Tau1_ResolutionPhi[m_channels[i]]->SetDirectory(dir);
    m_L1Tau2_ResolutionPhi[m_channels[i]] = new TH1D(Form("L1Tau2_%s_ResolutionPhi",m_channels[i].c_str()),Form("L1Tau2_%s_ResolutionPhi",m_channels[i].c_str()),100,-TMath::Pi()/10,TMath::Pi()/10); m_L1Tau2_ResolutionPhi[m_channels[i]]->SetDirectory(dir);
  }
  
}