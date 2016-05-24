#include "CMSSW_VBFHiggsTauTau/L1Analysis/interface/WorkerOptimiser.h"


void trgfw::WorkerOptimiser::runInputTypeTree(){
  
  if(m_verbose){
    std::lock_guard<std::mutex> lock(*m_command_line_mutex);
    printf("[WorkerOptimiser #%u] Started running with input type TTree\n",m_nWorker);
  }
  
  while(!taskDone){
    
    // TIMING ANALYSIS: Event processing starts
    auto time_total_start = std::chrono::high_resolution_clock::now();
    
    if(m_verbose){
      std::lock_guard<std::mutex> lock(*m_command_line_mutex);
      printf("[WorkerOptimiser #%u] Starting optimisation loop\n",m_nWorker);
    }
    
    trgfw::InputScan *iScan = 0;
    
    // TIMING ANALYSIS: Histogram retrieval starts
    auto time_histogram_retrieve_start = std::chrono::high_resolution_clock::now();
    
    m_input_mutex->lock();
    if(!m_input_scans->empty()){
      
      iScan = m_input_scans->back();
      m_input_scans->pop_back();
      (*m_input_processed)++;
      m_input_mutex->unlock(); // Unlock inputs for other threads
    }else{
      
      m_input_mutex->unlock(); // Unlock inputs for other threads
      
      if(m_verbose){
        std::lock_guard<std::mutex> lock(*m_command_line_mutex);
        printf("[WorkerOptimiser #%u] Input queue is empty. Incrementing stats on wait call... \n",m_nWorker);
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
      printf("[WorkerOptimiser #%u] Starting to process histogram path=%s name= %s\n",m_nWorker,iScan->path.c_str(),iScan->name.c_str());
    }
    
    // TIMING ANALYSIS: Histogram clone starts
    auto time_histogram_clone_start = std::chrono::high_resolution_clock::now();
    
    // Getting pointers
    trgfw::L1TVariableScanDataFormat *pSigEff = iScan->hEfficiency;
    trgfw::L1TVariableScanDataFormat *pBkgEff = iScan->hRate;
    
    // TIMING ANALYSIS: Histogram clone ends
    auto time_histogram_clone_end = std::chrono::high_resolution_clock::now();
    
    // TIMING ANALYSIS: Histogram integration starts
    auto time_histogram_integration_start = std::chrono::high_resolution_clock::now();
    
    if(iScan->reverseIntegral){
      for(int i=pSigEff->getNBins()+1; i>=0; i--){pSigEff->setBinContent(i,pSigEff->integral(0,i));}
      for(int i=pBkgEff->getNBins()+1; i>=0; i--){pBkgEff->setBinContent(i,pBkgEff->integral(0,i));}
    }else{
      for(int i=0; i<=pSigEff->getNBins()+1; i++){pSigEff->setBinContent(i,pSigEff->integral(i,pSigEff->getNBins()+1));}
      for(int i=0; i<=pBkgEff->getNBins()+1; i++){pBkgEff->setBinContent(i,pBkgEff->integral(i,pBkgEff->getNBins()+1));}
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
    pSigEff->scale(1/m_nEv_efficiency);
    pBkgEff->scale(m_conv_factor/m_nEv_rate);
    
    // TIMING ANALYSIS: Histogram scaling ends
    auto time_histogram_scaling_end = std::chrono::high_resolution_clock::now();
    
    if(m_verbose){
      std::lock_guard<std::mutex> lock(*m_command_line_mutex);
      printf("[WorkerOptimiser #%u] Scaling:DONE fEff=%0.8f fRate=%9.4f\n",m_nWorker,1/m_nEv_efficiency,m_conv_factor/m_nEv_rate);
    }
    
    // TIMING ANALYSIS: Working point determination starts
    auto time_workingPoint_determination_start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<=pSigEff->getNBins()+1; i++){
      if(pBkgEff->getBinContent(i)<=m_target_rate){
        
        trgfw::AlgoWorkingPoint wp;
        wp.name  = iScan->path;
        wp.var   = iScan->name;
        wp.value = pSigEff->getBinLowEdge(i);
        wp.rate  = pBkgEff->getBinContent(i);
        wp.eff   = pSigEff->getBinContent(i);
        
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
    
    if(m_verbose){
      std::lock_guard<std::mutex> lock(*m_command_line_mutex);
      printf("[WorkerOptimiser #%u] Memory cleanup completed\n",m_nWorker);
    }
    
    // Cleaning memory
    delete iScan;
    
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
  }
}


void trgfw::WorkerOptimiser::start(){
  
  if(m_verbose){
    std::lock_guard<std::mutex> lock(*m_command_line_mutex);
    printf("[WorkerOptimiser #%u] Called start() with option with inputType=%d\n",m_nWorker,m_input_type);
  }
  
  if      (m_input_type==trgfw::WorkerOptimiser::InputType::Histogram){
    m_thread = std::thread(&WorkerOptimiser::runInputTypeHistogram,this);
  }else if(m_input_type==trgfw::WorkerOptimiser::InputType::Tree){
    m_thread = std::thread(&WorkerOptimiser::runInputTypeTree,this);
  }
  
}






