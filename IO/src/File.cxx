#include "CMSSW_VBFHiggsTauTau/IO/interface/File.h"

#include "TKey.h"

#include <iostream>

using namespace std;

hepfw::File::File() : TFile(){}

hepfw::File::File(const char* fname, Option_t* option,const char* ftitle,Int_t compress) : TFile(fname,option,ftitle,compress){}

vector<TH1*> hepfw::File::getHistograms(){
  
  vector<TH1*> out;

  vector<TDirectoryFile*>  dirs;
//   TDirectoryFile* d = this;
  dirs.push_back(this);
  
  for(unsigned i=0; i<dirs.size(); i++){
    
    TDirectoryFile *iDir  = dirs[i];
    TList          *iList = iDir->GetListOfKeys();
    
    TIter iter(iList);
    while (TObject *obj = iter()){
      
      string className = obj->ClassName();
      
      if(className == "TKey"){
        
        TKey* key = (TKey*) obj; 
        string keyClassName = key->GetClassName();        
        
        if(keyClassName=="TDirectoryFile"){
          TDirectoryFile* dd = (TDirectoryFile*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName()));
          dirs.push_back(dd);
        }
        
        // NOTE: Need to check it this is not doubling memory usage by getting TObject* and TH1*
        else if(keyClassName=="TH1C")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH1S")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH1I")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH1F")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH1D")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TProfile")  {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        
        else if(keyClassName=="TH2C")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH2S")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH2I")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH2F")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH2D")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TProfile2D"){out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        
        else if(keyClassName=="TH3C")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH3S")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH3I")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH3F")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TH3D")      {out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        else if(keyClassName=="TProfile3D"){out.push_back( (TH1*) this->Get(Form("%s/%s",iDir->GetPath(),obj->GetName())));}
        
      }
    }
  }
  
  return out;
  
}

// void hepfw::File::copyDirectoryStructure(hepfw::File *file){
//   copyDirectoryStructure( (TFile*) file);
// }

void hepfw::File::copyDirectoryStructure(TFile *file){

  vector<TDirectoryFile*>  dirs;

  dirs.push_back(file);
  
  for(unsigned i=0; i<dirs.size(); i++){
    
    TDirectoryFile *iDir  = dirs[i];
    TList          *iList = iDir->GetListOfKeys();
    
    string subPath = iDir->GetPath();
    subPath = subPath.substr(subPath.find(':')+2,subPath.size()-1);
    this->mkdir(subPath.c_str());

    //cout << "mkdir: " << subPath.c_str() << endl;
    
    TIter iter(iList);
    while (TObject *obj = iter()){
      
      string className = obj->ClassName();
      
      if(className == "TKey"){
        
        TKey* key = (TKey*) obj; 
        string keyClassName = key->GetClassName();        
        
        if(keyClassName=="TDirectoryFile"){
          TDirectoryFile* dd = (TDirectoryFile*) file->Get(Form("%s/%s",iDir->GetPath(),obj->GetName()));
          dirs.push_back(dd);
          
        }
      }
    }
  }
   
}

