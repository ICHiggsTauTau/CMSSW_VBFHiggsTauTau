#!/bin/bash

TAG="jetmet-update-forjoe-CMSSW_8_0_2"
SAMPLE="VBF_HToTauTau_METEtaRange5p0"
REMOTEDIR="root://cms-xrd-global.cern.ch//store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/L1NTuplesV2_jetmet-update-forjoe-CMSSW_8_0_2_METEtaRange5p0_v1/160406_163323/0000"

cd /vols/cms02/jca10/VBFHiggsToTauTau/${TAG}/CMSSW_8_0_2/src/
eval `scramv1 runtime -sh`
cd /vols/cms02/jca10/VBFHiggsToTauTau/${TAG}/CMSSW_8_0_2/src/CMSSW_VBFHiggsTauTau/L1Analysis/jobs/${TAG}/
mkdir ${SAMPLE}
cd ${SAMPLE}
vbftautau_runTriggerAnalysis --input ${REMOTEDIR}/L1Ntuple_${SGE_TASK_ID}.root --outputFilename results_${SGE_TASK_ID}.root --maxEvents -1 --jobType mc
