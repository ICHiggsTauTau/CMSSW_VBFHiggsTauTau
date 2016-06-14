#!/bin/bash

cd /home/hep/dw515/JoaoOptimisation/CMSSW_8_0_2/src/CMSSW_VBFHiggsTauTau/L1Analysis/jobs/l1t-integration-v58.1/task_VBFHToTauTau_EleMuo/
source /vols/grid/cms/setup.sh
eval `scramv1 runtime -sh`
vbftautau_runAlgoAnalysis --inputType filelist --input /home/hep/dw515/JoaoOptimisation/CMSSW_8_0_2/src/CMSSW_VBFHiggsTauTau/L1Analysis/jobs/l1t-integration-v58.1/task_VBFHToTauTau_EleMuo/jobs/inputFileList_${SGE_TASK_ID}.txt --outputFilename /home/hep/dw515/JoaoOptimisation/CMSSW_8_0_2/src/CMSSW_VBFHiggsTauTau/L1Analysis/jobs/l1t-integration-v58.1/task_VBFHToTauTau_EleMuo/output/results_${SGE_TASK_ID}.root --maxEvents -1 --doAnalysisL1TAlgoScan --jobType mc --decay EleMuo
