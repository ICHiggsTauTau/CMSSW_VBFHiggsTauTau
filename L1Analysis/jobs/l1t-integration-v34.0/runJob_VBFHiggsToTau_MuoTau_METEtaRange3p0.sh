#!/bin/bash

TAG="l1t-integration-v34.0"
SAMPLE="VBF_HToTauTau_MuoHad_METEtaRange3p0"
REMOTEDIR="root://cms-xrd-global.cern.ch//store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/L1NTuplesV2_l1t-integration-v34p0_METEtaRange3p0_v2/160415_172530/0000"

cd /vols/cms02/jca10/VBFHiggsToTauTau/${TAG}/CMSSW_8_0_2/src/
eval `scramv1 runtime -sh`
cd /vols/cms02/jca10/VBFHiggsToTauTau/${TAG}/CMSSW_8_0_2/src/CMSSW_VBFHiggsTauTau/L1Analysis/jobs/${TAG}/
mkdir ${SAMPLE}
cd ${SAMPLE}

vbftautau_runTriggerAnalysis --doAnalysisL1TAlgoScan --doAnalysisL1TResolution --input ${REMOTEDIR}/L1Ntuple_${SGE_TASK_ID}.root --outputFilename results_${SGE_TASK_ID}.root --maxEvents -1 --jobType mc --decay MuoHad
