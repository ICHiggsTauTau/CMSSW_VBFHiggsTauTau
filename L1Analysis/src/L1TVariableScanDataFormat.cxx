#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/L1TVariableScanDataFormat.h"

#include <iostream>
#include <math.h>

using namespace std;

trgfw::L1TVariableScanDataFormat::L1TVariableScanDataFormat() : 
  m_path   (""),
  m_name   (""),
  m_nBins  (0),
  m_min    (0.),
  m_max    (0.),
  m_binSize(0.){}
  
trgfw::L1TVariableScanDataFormat::L1TVariableScanDataFormat(string path,string name,unsigned nBins,double min,double max) : 
  m_path   (path),
  m_name   (name),
  m_nBins  (nBins),
  m_min    (min),
  m_max    (max){

  m_binSize = (m_max-m_min)/nBins;
    
  for(unsigned i=0; i<nBins+2; i++){m_bins.push_back(0);}
}

trgfw::L1TVariableScanDataFormat::~L1TVariableScanDataFormat(){}

void trgfw::L1TVariableScanDataFormat::fill(double value=1.0){
  
  if     (value< m_min){m_bins[0]++;}
  else if(value>=m_max){m_bins[m_nBins+1]++;}
  else{
    unsigned iBin = floor( (value-m_min)/m_binSize );
    m_bins[iBin+1]++;
  }
}

void trgfw::L1TVariableScanDataFormat::setBinContent(unsigned bin,double value){
  m_bins[bin] = value;
}

double trgfw::L1TVariableScanDataFormat::getBinContent(unsigned bin){
  return m_bins[bin];
}

string trgfw::L1TVariableScanDataFormat::getPath(){
  return m_path;
}

string trgfw::L1TVariableScanDataFormat::getName(){
  return m_name;
}

int trgfw::L1TVariableScanDataFormat::getNBins(){
  return m_nBins;
}

double trgfw::L1TVariableScanDataFormat::getMin(){
  return m_min;
}

double trgfw::L1TVariableScanDataFormat::getMax(){
  return m_max;
}

const std::vector<double>* trgfw::L1TVariableScanDataFormat::getContent(){
  return &m_bins;
}

double trgfw::L1TVariableScanDataFormat::integral(unsigned binStart,unsigned binEnd){
  double sum=0;
  for(unsigned i=binStart; i<=binEnd; i++){
    sum+=m_bins[i];
  }
  return sum;
}

void trgfw::L1TVariableScanDataFormat::scale(double value){
  for(int i=0; i<=m_nBins+1; i++){
    m_bins[i]=m_bins[i]*value;
  }
}

double trgfw::L1TVariableScanDataFormat::getBinLowEdge(unsigned bin){
  
  if(bin==0){return 0;}
  else{
    return m_min+((bin-1)*m_binSize);
  }
}

void trgfw::L1TVariableScanDataFormat::add(trgfw::L1TVariableScanDataFormat *input){
  
  if(m_name!=input->getName()){
    cout << "[trgfw::L1TVariableScanDataFormat::add] ERROR: Different names this=" << m_name << " input=" << input->getName() << endl;
    return;
  }
  
  if(m_path!=input->getPath()){
    cout << "[trgfw::L1TVariableScanDataFormat::add] ERROR: Different path this=" << m_path << " input=" << input->getPath() << endl;
    return;
  }
  
  if(m_min!=input->getMin()){
    cout << "[trgfw::L1TVariableScanDataFormat::add] ERROR: Different min this=" << m_min << " input=" << input->getMin() << endl;
    return;
  }
  
  if(m_max!=input->getMax()){
    cout << "[trgfw::L1TVariableScanDataFormat::add] ERROR: Different max this=" << m_max << " input=" << input->getMax() << endl;
    return;
  }
  
  if(m_nBins!=input->getNBins()){
    cout << "[trgfw::L1TVariableScanDataFormat::add] ERROR: Different max this=" << m_nBins << " input=" << input->getNBins() << endl;
    return;
  }
  
  const std::vector<double>* inputBins = input->getContent();
  for(unsigned i=0; i<m_bins.size(); i++){
    m_bins[i]+=(*inputBins)[i];
  }
  
  
}
