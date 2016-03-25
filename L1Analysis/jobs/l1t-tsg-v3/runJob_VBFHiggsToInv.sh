#!/bin/bash

cd /vols/cms02/jca10/VBFHiggsToTauTau/l1t-tsg-v3/CMSSW_8_0_0_pre6/src/
eval `scramv1 runtime -sh`
cd /vols/cms02/jca10/VBFHiggsToTauTau/l1t-tsg-v3/CMSSW_8_0_0_pre6/src/CMSSW_VBFHiggsTauTau/L1Analysis/jobs/VBFHiggsToInv/
vbftautau_runTriggerAnalysis --input root://cms-xrd-global.cern.ch//store/user/pela/VBF_HToInvisible_M125_13TeV_powheg_pythia8/L1NTuplesV2_l1t-tsg-v3/160301_004620/0000/L1Ntuple_${SGE_TASK_ID}.root --outputFilename results_${SGE_TASK_ID}.root --maxEvents -1 --jobType mc
