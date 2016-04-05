#!/bin/bash

cd /vols/cms02/jca10/VBFHiggsToTauTau/l1t-sums-eta-range-fix-CMSSW_8_0_2/CMSSW_8_0_2/src/
eval `scramv1 runtime -sh`
cd /vols/cms02/jca10/VBFHiggsToTauTau/l1t-sums-eta-range-fix-CMSSW_8_0_2/CMSSW_8_0_2/src/CMSSW_VBFHiggsTauTau/L1Analysis/jobs/l1t-sums-eta-range-fix-CMSSW_8_0_2/
mkdir ZeroBias_run259721_METEtaRange3p0
cd ZeroBias_run259721_METEtaRange3p0
vbftautau_runTriggerAnalysis --input root://cms-xrd-global.cern.ch//store/user/pela/ZeroBias/L1NTuplesV2_l1t-sums-eta-range-fix-CMSSW_8_0_2_METEtaRange3p0_v2/160329_034344/0000/L1Ntuple_${SGE_TASK_ID}.root --outputFilename results_${SGE_TASK_ID}.root --maxEvents -1 --jobType data --runs 259721
