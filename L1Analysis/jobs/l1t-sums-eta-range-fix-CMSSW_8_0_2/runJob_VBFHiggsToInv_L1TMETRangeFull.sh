#!/bin/bash

cd /vols/cms02/jca10/VBFHiggsToTauTau/l1t-sums-eta-range-fix-CMSSW_8_0_2/CMSSW_8_0_2/src/
eval `scramv1 runtime -sh`
cd /vols/cms02/jca10/VBFHiggsToTauTau/l1t-sums-eta-range-fix-CMSSW_8_0_2/CMSSW_8_0_2/src/CMSSW_VBFHiggsTauTau/L1Analysis/jobs/l1t-sums-eta-range-fix-CMSSW_8_0_2/
mkdir VBF_HToInvisible_METEtaRange5p0
cd VBF_HToInvisible_METEtaRange5p0
vbftautau_runTriggerAnalysis --input root://cms-xrd-global.cern.ch//store/user/pela/VBF_HToInvisible_M125_13TeV_powheg_pythia8/L1NTuplesV2_l1t-sums-eta-range-fix-CMSSW_8_0_2_METEtaRange5p0_v1/160329_034458/0000/L1Ntuple_${SGE_TASK_ID}.root --outputFilename results_${SGE_TASK_ID}.root --maxEvents -1 --jobType mc
