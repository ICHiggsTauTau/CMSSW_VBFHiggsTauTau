#ifndef VBFHiggsToTauTau_L1Analysis_WorkerOptimiser
#define VBFHiggsToTauTau_L1Analysis_WorkerOptimiser

// VBF includes
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/WorkerHelpers.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/AlgoWorkingPoint.h"

// ROOT includes
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TLegend.h"

// C++ STD includes
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <map>
#include <vector>
#include <list>
#include <functional>
#include <thread>
#include <chrono>
#include <ctime>
#include <mutex>

using namespace std;

namespace trgfw{ 

  class WorkerOptimiser{
  public:
    
    WorkerOptimiser(unsigned n){
      taskDone      = false;
      m_verbose     = false;
      m_savePlots   = false;
      m_conv_factor = 1;
      m_nWorker     = n;
      m_target_rate = 1000.;
      
      m_outputDir   = "";
      
      // Init statistics
      m_numberEventsProcessed     = 0;
      m_numberCallsEmptyQueueWait = 0;
      
      // Timing init
      m_time_total                      = std::chrono::microseconds(0);
      m_time_histogram_retrieve         = std::chrono::microseconds(0);
      m_time_histogram_clone            = std::chrono::microseconds(0);
      m_time_histogram_integration      = std::chrono::microseconds(0);
      m_time_histogram_scaling          = std::chrono::microseconds(0);
      m_time_workingPoint_determination = std::chrono::microseconds(0);
      m_time_histogram_delete           = std::chrono::microseconds(0);
    }
    ~WorkerOptimiser(){}
    
    void runInputTypeHistogram(){

      if(m_verbose){
        std::lock_guard<std::mutex> lock(*m_command_line_mutex);
        printf("[WorkerOptimiser #%u] Started running with input type histograms\n",m_nWorker);
      }
      
      while(!taskDone){
        
        // TIMING ANALYSIS: Event processing starts
        auto time_total_start = std::chrono::high_resolution_clock::now();
        
        if(m_verbose){
          std::lock_guard<std::mutex> lock(*m_command_line_mutex);
          printf("[WorkerOptimiser #%u] Starting optimisation loop\n",m_nWorker);
        }
        
        trgfw::InputHistograms *iHists = 0;
        
        // TIMING ANALYSIS: Histogram retrieval starts
        auto time_histogram_retrieve_start = std::chrono::high_resolution_clock::now();
        
        m_input_mutex->lock();
        if(!m_input_histograms->empty()){
          
          iHists = m_input_histograms->back();
          m_input_histograms->pop_back();
          (*m_input_processed)++;
          m_input_mutex->unlock();
        }else{
          
          if(m_verbose){
            std::lock_guard<std::mutex> lock(*m_command_line_mutex);
            printf("[WorkerOptimiser #%u] Input queue is empty \n",m_nWorker);
          }
          m_input_mutex->unlock();
          
          if(m_verbose){
            std::lock_guard<std::mutex> lock(*m_command_line_mutex);
            printf("[WorkerOptimiser #%u] Incrementing stats on wait call \n",m_nWorker);
          }
          
          m_threadStatistics_mutex.lock();
          m_numberCallsEmptyQueueWait++;
          m_threadStatistics_mutex.unlock();
          
          if(m_verbose){
            std::lock_guard<std::mutex> lock(*m_command_line_mutex);
            printf("[WorkerOptimiser #%u] Waiting for 1s...\n",m_nWorker);
          }
          
          this_thread::sleep_for(chrono::seconds(1));
          
          if(m_verbose){
            std::lock_guard<std::mutex> lock(*m_command_line_mutex);
            printf("[WorkerOptimiser #%u] Resetting loop...\n",m_nWorker);
          }
          
          continue;
        }
        
        // TIMING ANALYSIS: Histogram retrieval ends
        auto time_histogram_retrieve_end = std::chrono::high_resolution_clock::now();
        
        if(m_verbose){
          std::lock_guard<std::mutex> lock(*m_command_line_mutex);
          printf("[WorkerOptimiser #%u] Starting to process histogram path=%s name= %s\n",m_nWorker,iHists->path.c_str(),iHists->name.c_str());
        }
        
        // TIMING ANALYSIS: Histogram clone starts
        auto time_histogram_clone_start = std::chrono::high_resolution_clock::now();
        
        // Getting pointers
        TH1D *pSigEff = iHists->hEfficiency;
        TH1D *pBkgEff = iHists->hRate;
        
        // TIMING ANALYSIS: Histogram clone ends
        auto time_histogram_clone_end = std::chrono::high_resolution_clock::now();
        
        if(m_verbose){
          std::lock_guard<std::mutex> lock(*m_command_line_mutex);
          printf("[WorkerOptimiser #%u] Cloning:DONE Eff=%s (%f) Rate=%s (%f)\n",m_nWorker,
                 iHists->hEfficiency->GetName(),
                 iHists->hEfficiency->GetEntries(),
                 iHists->hRate->GetName(),
                 iHists->hRate->GetEntries());
        }
        
        // TIMING ANALYSIS: Histogram integration starts
        auto time_histogram_integration_start = std::chrono::high_resolution_clock::now();
        
        if(iHists->reverseIntegral){
          for(int i=pSigEff->GetNbinsX()+1; i>=0; i--){pSigEff->SetBinContent(i,pSigEff->Integral(0,i));}
          for(int i=pBkgEff->GetNbinsX()+1; i>=0; i--){pBkgEff->SetBinContent(i,pBkgEff->Integral(0,i));}
        }else{
          for(int i=0; i<=pSigEff->GetNbinsX()+1; i++){pSigEff->SetBinContent(i,pSigEff->Integral(i,pSigEff->GetNbinsX()+1));}
          for(int i=0; i<=pBkgEff->GetNbinsX()+1; i++){pBkgEff->SetBinContent(i,pBkgEff->Integral(i,pBkgEff->GetNbinsX()+1));}
        }
        
        // TIMING ANALYSIS: Histogram integration ends
        auto time_histogram_integration_end = std::chrono::high_resolution_clock::now();
        
        if(m_verbose){
          std::lock_guard<std::mutex> lock(*m_command_line_mutex);
          printf("[WorkerOptimiser #%u] Integration completed\n",m_nWorker);
        }
        
        // TIMING ANALYSIS: Histogram scaling starts
        auto time_histogram_scaling_start = std::chrono::high_resolution_clock::now();
        
        // Scaling to obtain efficiency per bunch
        pSigEff->Scale(1/m_nEv_efficiency);
        pBkgEff->Scale(m_conv_factor/m_nEv_rate);
        
        // TIMING ANALYSIS: Histogram scaling ends
        auto time_histogram_scaling_end = std::chrono::high_resolution_clock::now();
        
        if(m_verbose){
          std::lock_guard<std::mutex> lock(*m_command_line_mutex);
          printf("[WorkerOptimiser #%u] Scaling:DONE fEff=%0.8f fRate=%9.4f\n",m_nWorker,1/m_nEv_efficiency,m_conv_factor/m_nEv_rate);
        }
        
        // TIMING ANALYSIS: Working point determination starts
        auto time_workingPoint_determination_start = std::chrono::high_resolution_clock::now();
        
        for(int i=0; i<=pSigEff->GetNbinsX()+1; i++){
          if(pBkgEff->GetBinContent(i)<=m_target_rate){
            
            trgfw::AlgoWorkingPoint wp;
            wp.name  = iHists->path;
            wp.var   = iHists->name;
            wp.value = pSigEff->GetXaxis()->GetBinLowEdge(i);
            wp.rate  = pBkgEff->GetBinContent(i);
            wp.eff   = pSigEff->GetBinContent(i);
            
            // Locking results and saving found working point
            m_workingPoints_mutex->lock();
            m_workingPoints      ->push_back(wp);
            m_workingPoints_mutex->unlock();
            break;
          }
        }
        
        // TIMING ANALYSIS: Working point determination ends
        auto time_workingPoint_determination_end = std::chrono::high_resolution_clock::now();
        
        if(m_verbose){
          std::lock_guard<std::mutex> lock(*m_command_line_mutex);
          printf("[WorkerOptimiser #%u] Working point determination completed\n",m_nWorker);
        }
        
        if(m_savePlots){
          
          if(m_verbose){
            std::lock_guard<std::mutex> lock(*m_command_line_mutex);
            printf("[WorkerOptimiser #%u] Starting save plots process\n",m_nWorker);
          }
          
          m_root_mutex->lock();
          TCanvas *c0 = new TCanvas(Form("%u_rates_%s",m_nWorker,iHists->name.c_str()));
          
          TPad *pad = new TPad("pad","",0,0,1,1);
          pad->SetFillColor(0);
          pad->SetGridx();
          pad->SetTicky(false);
          pad->Draw();
          pad->cd();
          
          if(iHists->name=="L1TMet_Et"){pSigEff->GetXaxis()->SetRangeUser(0,200);}
          if(iHists->name=="L1TMHT_Et"){pSigEff->GetXaxis()->SetRangeUser(0,200);}
          
          pSigEff->GetYaxis()->SetTitleOffset(1.5);
          pSigEff->GetYaxis()->SetTitle("Signal Efficiency");
          pSigEff->Draw();
          
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
          
          if(iHists->name=="L1TMet_Et"){pBkgEff->GetXaxis()->SetRangeUser(0,200);}
          if(iHists->name=="L1TMHT_Et"){pBkgEff->GetXaxis()->SetRangeUser(0,200);}
          
          pBkgEff->GetYaxis()->SetTitle("L1T Rate (Hz)");
          
          pBkgEff->SetLineColor(kRed);
          pBkgEff->GetYaxis()->SetAxisColor(kRed);
          pBkgEff->GetYaxis()->SetTitleColor(kRed);
          pBkgEff->GetYaxis()->SetLabelColor(kRed);
          pBkgEff->GetYaxis()->SetTitleOffset(1.5);  
          pBkgEff->Draw("Y+");
          
          TLegend *l = new TLegend(0.50,0.80,0.85,0.95);
          l->SetBorderSize(1);
          l->AddEntry(pSigEff,"VBF Inv Eff");
          l->AddEntry(pBkgEff, "Zero Bias");
          l->Draw();
          
          
          TDirectoryFile *d  =0;
          
          // Locking output file and writting results histogram
          m_outputfile_mutex->lock();
          if(iHists->path==""){
            d = (TDirectoryFile*) m_outputfile;
          }else{
            d = (TDirectoryFile*) m_outputfile->Get(iHists->path.c_str());
          }
          d->WriteTObject(c0);
          m_outputfile_mutex->unlock();
          
          //FIXME: directory handling
          system(Form("mkdir -p %s/%s", m_outputDir.c_str(),iHists->path.c_str()));
          c0->SaveAs(Form("%s/%s/%s.%s",m_outputDir.c_str(),iHists->path.c_str(),iHists->name.c_str(),"png"));
          c0->SaveAs(Form("%s/%s/%s.%s",m_outputDir.c_str(),iHists->path.c_str(),iHists->name.c_str(),"pdf"));
          c0->SaveAs(Form("%s/%s/%s.%s",m_outputDir.c_str(),iHists->path.c_str(),iHists->name.c_str(),"C"));
          
          delete c0;
          m_root_mutex->unlock();
        }
        
        // TIMING ANALYSIS: Histogram delete starts
        auto time_histogram_delete_start = std::chrono::high_resolution_clock::now();
        
        //m_root_mutex->lock();
        //delete pSigEff;
        //delete pBkgEff;
        //m_root_mutex->unlock();
        
        // TIMING ANALYSIS: Histogram delete ends
        auto time_histogram_delete_end = std::chrono::high_resolution_clock::now();
        
        if(m_verbose){
          std::lock_guard<std::mutex> lock(*m_command_line_mutex);
          printf("[WorkerOptimiser #%u] Memory cleanup completed\n",m_nWorker);
        }
        
        m_threadStatistics_mutex.lock();
        m_numberEventsProcessed++;
        m_threadStatistics_mutex.unlock();
        
        auto time_total_end = std::chrono::high_resolution_clock::now();
        
        // Calculating time spent
        m_time_total                      += std::chrono::duration_cast<std::chrono::microseconds>(time_total_end                     -time_total_start);
        m_time_histogram_retrieve         += std::chrono::duration_cast<std::chrono::microseconds>(time_histogram_retrieve_end        -time_histogram_retrieve_start);
        m_time_histogram_clone            += std::chrono::duration_cast<std::chrono::microseconds>(time_histogram_clone_end           -time_histogram_clone_start);
        m_time_histogram_integration      += std::chrono::duration_cast<std::chrono::microseconds>(time_histogram_integration_end     -time_histogram_integration_start);
        m_time_histogram_scaling          += std::chrono::duration_cast<std::chrono::microseconds>(time_histogram_scaling_end         -time_histogram_scaling_start);
        m_time_workingPoint_determination += std::chrono::duration_cast<std::chrono::microseconds>(time_workingPoint_determination_end-time_workingPoint_determination_start);
        m_time_histogram_delete           += std::chrono::duration_cast<std::chrono::microseconds>(time_histogram_delete_end          -time_histogram_delete_start);
      }
    }
    
    void runInputTypeTree();
    
    void start();
    
    void setVerbose(bool value){
      m_verbose=value;
    }
    
    void setInput(std::mutex* mutex,std::list<trgfw::InputHistograms*> *obj_h,list<trgfw::InputScan*> *obj_s,unsigned *nProcessed){
      m_input_mutex      = mutex;
      m_input_processed  = nProcessed;
      m_input_histograms = obj_h;
      m_input_scans      = obj_s;
    }
    
    enum InputType{
      None=0,
      Histogram=1,
      Tree=2
    };
    
    void setInputType(trgfw::WorkerOptimiser::InputType type){
      m_input_type=type;
    }
    
    void setOutputWorkingPoints(std::mutex* mutex,std::vector<trgfw::AlgoWorkingPoint>* obj){
      m_workingPoints_mutex = mutex;
      m_workingPoints       = obj;
    }
    
    void setOutputFile(std::mutex* mutex,TFile* obj){
      m_outputfile_mutex = mutex;
      m_outputfile       = obj;
    }
    
    void setNEvents(double nEvEff,double nEvRates){
      m_nEv_efficiency = nEvEff;
      m_nEv_rate       = nEvRates;
    }
    
    void setSavePlots(bool value){
      m_savePlots = value;
    }
    
    void setTaskDone(bool status){
      taskDone=status;
    }
    
    void setConvFactor(double factor){
      m_conv_factor=factor;
    }
    
    void setRootMutex(std::mutex *mtx){
      m_root_mutex=mtx;
    }
    
    void setPtrCommandLineMutex(std::mutex *mtx){
      m_command_line_mutex=mtx;
    }
    
    void setOutputDir(std::string dir){
      m_outputDir=dir;
    }
    
    void setTargetRate(double rate){
      m_target_rate=rate;
    }
    
    unsigned getNumberEventsProcessed(){
      std::lock_guard<std::mutex> lock(m_threadStatistics_mutex);
      return m_numberEventsProcessed;
    }
    
    unsigned getnumberCallsEmptyQueueWait(){
      std::lock_guard<std::mutex> lock(m_threadStatistics_mutex);
      return m_numberCallsEmptyQueueWait;
    }
    
    
    void join(){
      //m_numberEventsProcessed = std::max(m_numberEventsProcessed,(unsigned)1);
      double faction_time_histogram_retrieve         = 100.*m_time_histogram_retrieve.count()        /m_time_total.count();
//       double faction_time_histogram_clone            = 100.*m_time_histogram_clone.count()           /m_time_total.count();
      double faction_time_histogram_integration      = 100.*m_time_histogram_integration.count()     /m_time_total.count();
      double faction_time_histogram_scaling          = 100.*m_time_histogram_scaling.count()         /m_time_total.count();
      double faction_time_workingPoint_determination = 100.*m_time_workingPoint_determination.count()/m_time_total.count();
      double faction_time_histogram_delete           = 100.*m_time_histogram_delete.count()          /m_time_total.count();
      
      double ev_time_total                      = m_time_total.count()                     /m_numberEventsProcessed;
      double ev_time_histogram_retrieve         = m_time_histogram_retrieve.count()        /m_numberEventsProcessed;
//       double ev_time_histogram_clone            = m_time_histogram_clone.count()           /m_numberEventsProcessed;
      double ev_time_histogram_integration      = m_time_histogram_integration.count()     /m_numberEventsProcessed;
      double ev_time_histogram_scaling          = m_time_histogram_scaling.count()         /m_numberEventsProcessed;
      double ev_time_workingPoint_determination = m_time_workingPoint_determination.count()/m_numberEventsProcessed;
      double ev_time_histogram_delete           = m_time_histogram_delete.count()          /m_numberEventsProcessed;
      
      m_command_line_mutex->lock();
      printf("[WorkerOptimiser #%u] ===== TIMMING SUMMARY (per histogram processed) =====\n",m_nWorker);
      printf("[WorkerOptimiser #%u] Total                       : %8.0f ms \n",         m_nWorker,ev_time_total);
      printf("[WorkerOptimiser #%u] Histogram retrieve          : %8.0f ms (%7.3f%%)\n",m_nWorker,ev_time_histogram_retrieve        ,faction_time_histogram_retrieve        );
      //printf("[WorkerOptimiser #%u] Histogram clone             : %8.0f ms (%7.3f%%)\n",m_nWorker,ev_time_histogram_clone           ,faction_time_histogram_clone           );
      printf("[WorkerOptimiser #%u] Histogram integration       : %8.0f ms (%7.3f%%)\n",m_nWorker,ev_time_histogram_integration     ,faction_time_histogram_integration     );
      printf("[WorkerOptimiser #%u] Histogram scaling           : %8.0f ms (%7.3f%%)\n",m_nWorker,ev_time_histogram_scaling         ,faction_time_histogram_scaling         );
      printf("[WorkerOptimiser #%u] Working Point determination : %8.0f ms (%7.3f%%)\n",m_nWorker,ev_time_workingPoint_determination,faction_time_workingPoint_determination);
      printf("[WorkerOptimiser #%u] Histogram delete            : %8.0f ms (%7.3f%%)\n",m_nWorker,ev_time_histogram_delete          ,faction_time_histogram_delete          );
      m_command_line_mutex->unlock();
      
      m_thread.join();
    }
    
  private:
    
    unsigned m_nWorker;
    bool     m_verbose;
    bool     taskDone;
    
    // The thread for this worker
    std::thread m_thread;
    
    // Global mutex
    std::mutex *m_command_line_mutex;
    std::mutex *m_root_mutex;
    
    std::mutex                         *m_input_mutex;
    unsigned                           *m_input_processed;
    trgfw::WorkerOptimiser::InputType   m_input_type;
    std::list<trgfw::InputHistograms*> *m_input_histograms;
    std::list<trgfw::InputScan*>       *m_input_scans;
    
    std::mutex                      *m_workingPoints_mutex;
    vector<trgfw::AlgoWorkingPoint> *m_workingPoints;
    
    //
    std::string m_outputDir;
    
    std::mutex *m_outputfile_mutex;
    TFile      *m_outputfile;
    
    bool   m_savePlots;
    
    double m_nEv_efficiency;
    double m_nEv_rate;
    double m_conv_factor;
    double m_target_rate;
    
    // Thread statistics
    std::mutex m_threadStatistics_mutex;
    unsigned   m_numberEventsProcessed;
    unsigned   m_numberCallsEmptyQueueWait;
    
    // Thread timming
    std::chrono::microseconds m_time_total;
    std::chrono::microseconds m_time_histogram_retrieve;
    std::chrono::microseconds m_time_histogram_clone;
    std::chrono::microseconds m_time_histogram_integration;
    std::chrono::microseconds m_time_histogram_scaling;
    std::chrono::microseconds m_time_workingPoint_determination;
    std::chrono::microseconds m_time_histogram_delete;
  };
  
}

#endif
