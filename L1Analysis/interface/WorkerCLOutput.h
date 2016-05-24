#ifndef VBFHiggsToTauTau_L1Analysis_WorkerCLOutput
#define VBFHiggsToTauTau_L1Analysis_WorkerCLOutput

// VBF includes
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/WorkerOptimiser.h"
#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/WorkerHelpers.h"

// C++ STD includes
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <map>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>
#include <ctime>
#include <mutex>

using namespace std;

namespace trgfw{
  
  class WorkerCLOutput{
  public:
    
    WorkerCLOutput(){
      taskDone           = false;
      m_workerOptimisers = 0;
      
      m_base_histograms_initial_size_efficiency = 0;
      m_base_histograms_initial_size_rate       = 0;
      
    }
    ~WorkerCLOutput(){}
    
    void start(){
      m_thread = std::thread(&WorkerCLOutput::run,this);
    }
    
    void run(){
      
      while(!taskDone){
        
        unsigned base_histograms_size_efficiency = 0;
        unsigned base_histograms_size_rates      = 0;
        
        if(m_input_type==trgfw::WorkerCLOutput::InputType::Histogram){
        
          // Getting data from base histograms
          m_base_histograms_mutex->lock();
          base_histograms_size_efficiency = m_base_histograms_efficiency->size();
          base_histograms_size_rates      = m_base_histograms_rates     ->size();
          m_base_histograms_mutex->unlock();
        }
        
        m_command_line_mutex->lock();
        // Printing 
        system("clear");
        printf("\n");
        printf("##### Input Histograms to be paired #####\n");
        printf("Intial  histograms efficiency: %5u\n",m_base_histograms_initial_size_efficiency);
        printf("Current histograms efficiency: %5u\n",base_histograms_size_efficiency);
        printf("Initial histograms rate      : %5u\n",m_base_histograms_initial_size_rate);        
        printf("Current histograms rate      : %5u\n",base_histograms_size_rates);
        
        printf("\n");
        printf("##### Worker statistics #####\n");
        cout << "Number of optimisation workers: " << m_workerOptimisers->size() << endl;
        for(unsigned i=0; i<m_workerOptimisers->size(); i++){
          printf("[Worker #%u] Events process %8u callsEmptyQueueWait: %8u\n",i,
                 m_workerOptimisers->at(i)->getNumberEventsProcessed(),
                 m_workerOptimisers->at(i)->getnumberCallsEmptyQueueWait());
          
        }
        m_command_line_mutex->unlock();
        
        if(m_input_type==trgfw::WorkerCLOutput::InputType::Histogram){
        
          m_input_mutex->lock();
          unsigned inputHistograms_total     = (*m_input_total);
          unsigned inputHistograms_processed = (*m_input_processed);
          unsigned inputHistograms_size      = m_input_histograms->size();
          m_input_mutex->unlock();
        
          m_command_line_mutex->lock();
          printf("\n");
          printf("##### Global processing #####\n");
          printf("Total histograms pairs found : %8u\n",inputHistograms_total);
          printf("Histograms to be analysed    : %8u\n",inputHistograms_size);
          printf("Histograms processed         : %8u\n",inputHistograms_processed);
          m_command_line_mutex->unlock();

        }if(m_input_type==trgfw::WorkerCLOutput::InputType::Tree){
          
          m_input_mutex->lock();
          unsigned input_total     = (*m_input_total);
          unsigned input_processed = (*m_input_processed);
          unsigned input_scan_size           = m_input_scans->size();
          m_input_mutex->unlock();
          
          m_command_line_mutex->lock();
          printf("\n");
          printf("##### Global processing #####\n");
          printf("Total scans pairs found : %8u\n",input_total);
          printf("Scans to be analysed    : %8u\n",input_processed);
          printf("Scans processed         : %8u\n",input_scan_size);
          m_command_line_mutex->unlock();
        }
        
        // Getting data from working points
        m_workingPoints_mutex->lock();
        unsigned workingPoints_size = m_workingPoints->size();
        m_workingPoints_mutex->unlock();
        
        m_command_line_mutex->lock();
        printf("\n");
        printf("##### Working point calculation #####\n");
        printf("Working points found : %8u\n",workingPoints_size);
        m_command_line_mutex->unlock();
        
        
        this_thread::sleep_for(chrono::seconds(1));
        
      }
    }
    
    void setInput(std::mutex* mutex,std::list<trgfw::InputHistograms*> *obj_h,list<trgfw::InputScan*> *obj_s,unsigned *nTotal,unsigned *nProcessed){
      m_input_mutex      = mutex;
      m_input_total      = nTotal;
      m_input_processed  = nProcessed;
      m_input_histograms = obj_h;
      m_input_scans      = obj_s;
    }
    
    enum InputType{
      None=0,
      Histogram=1,
      Tree=2
    };
    
    void setInputType(trgfw::WorkerCLOutput::InputType type){
      m_input_type=type;
    }
    
    void setPtrWorkerOptimisers(std::vector<trgfw::WorkerOptimiser*>* vec){
      m_workerOptimisers = vec;
    }
    
    void setBaseHistogramsInitialSize(unsigned sizeEfficiency,unsigned sizeRate){
      m_base_histograms_initial_size_efficiency = sizeEfficiency;
      m_base_histograms_initial_size_rate       = sizeRate;
    }
    
    void setPtrBaseHistograms(std::mutex *mtx,std::vector<TH1*> *hEff,std::vector<TH1*> *hRate){
      m_base_histograms_mutex      = mtx;
      m_base_histograms_efficiency = hEff;
      m_base_histograms_rates      = hRate;
    }
    
    void setPtrWorkingPoints(std::mutex *mtx,vector<trgfw::AlgoWorkingPoint> *vecWP){
      m_workingPoints_mutex = mtx;
      m_workingPoints       = vecWP;
    }
    
    void setPtrCommandLineMutex(std::mutex *mtx){
      m_command_line_mutex=mtx;
    }
    
    void setTaskDone(bool status){
      taskDone=status;
    }
    
    void join(){
      m_thread.join();
    }
    
  private:
    
    bool taskDone;
    
    std::thread m_thread;
    
    // Global mutex
    std::mutex *m_command_line_mutex;

    // Base histograms (cached at begining)
    unsigned m_base_histograms_initial_size_efficiency;
    unsigned m_base_histograms_initial_size_rate;

    // Pointers to base histograms (access need mutex)
    std::mutex        *m_base_histograms_mutex;
    std::vector<TH1*> *m_base_histograms_efficiency;
    std::vector<TH1*> *m_base_histograms_rates;
    
    // Pointers to histograms related stuff
    std::mutex                         *m_input_mutex;
    trgfw::WorkerCLOutput::InputType    m_input_type;
    unsigned                           *m_input_total;
    unsigned                           *m_input_processed;
    std::list<trgfw::InputHistograms*> *m_input_histograms;
    std::list<trgfw::InputScan*>       *m_input_scans;
    
    // Pointers to working points related stuff
    std::mutex                      *m_workingPoints_mutex;
    vector<trgfw::AlgoWorkingPoint> *m_workingPoints;
    
    // Pointers to all workers
    std::vector<trgfw::WorkerOptimiser*> *m_workerOptimisers;
    
  };
  
}

#endif
