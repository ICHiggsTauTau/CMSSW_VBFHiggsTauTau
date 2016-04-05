#!/bin/sh

qsub -q hepshort.q -t 1-164:1 runJob_ZeroBias_run258440.sh
qsub -q hepshort.q -t 1-164:1 runJob_ZeroBias_run258448.sh
qsub -q hepshort.q -t 1-164:1 runJob_ZeroBias_run259721.sh

qsub -q hepshort.q -t 1-31:1 runJob_VBFHiggsToTauTau.sh
qsub -q hepshort.q -t 1-31:1 runJob_VBFHiggsToTauTau_EleMuo.sh
qsub -q hepshort.q -t 1-31:1 runJob_VBFHiggsToTauTau_EleHad.sh
qsub -q hepshort.q -t 1-31:1 runJob_VBFHiggsToTauTau_MuoHad.sh
qsub -q hepshort.q -t 1-31:1 runJob_VBFHiggsToTauTau_HadHad.sh

qsub -q hepshort.q -t 1-3:1 runJob_VBFHiggsToInv.sh


# Full MET range
#qsub -q hepshort.q -t 1-3:1   runJob_VBFHiggsToInv_L1TMETRangeFull.sh
#qsub -q hepshort.q -t 1-164:1 runJob_ZeroBias_run259721_L1TMETRangeFull.sh
