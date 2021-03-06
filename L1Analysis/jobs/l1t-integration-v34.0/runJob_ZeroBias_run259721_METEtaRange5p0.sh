#!/bin/bash

TAG="l1t-integration-v34.0"
SAMPLE="ZeroBias_run259721_METEtaRange5p0"
REMOTEDIR="root://cms-xrd-global.cern.ch//store/user/pela/ZeroBias/L1NTuplesV2_l1t-integration-v34p0_METEtaRange5p0_v1/160413_024800/0000"

cd /vols/cms02/jca10/VBFHiggsToTauTau/${TAG}/CMSSW_8_0_2/src/
eval `scramv1 runtime -sh`
cd /vols/cms02/jca10/VBFHiggsToTauTau/${TAG}/CMSSW_8_0_2/src/CMSSW_VBFHiggsTauTau/L1Analysis/jobs/${TAG}/
mkdir ${SAMPLE}
cd ${SAMPLE}

vbftautau_runTriggerAnalysis --doAnalysisL1TAlgoScan --input ${REMOTEDIR}/L1Ntuple_${SGE_TASK_ID}.root --outputFilename results_${SGE_TASK_ID}.root --maxEvents -1 --jobType data --runs 259721
