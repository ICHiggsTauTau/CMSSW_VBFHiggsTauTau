#!/bin/sh

qsub -q hepshort.q -t 1-76:1 runJob_VBFHiggsToTau_METEtaRange3p0.sh
qsub -q hepshort.q -t 1-76:1 runJob_VBFHiggsToTau_METEtaRange5p0.sh

qsub -q hepshort.q -t 1-76:1 runJob_VBFHiggsToTau_EleMuo_METEtaRange3p0.sh
qsub -q hepshort.q -t 1-76:1 runJob_VBFHiggsToTau_EleMuo_METEtaRange5p0.sh
qsub -q hepshort.q -t 1-76:1 runJob_VBFHiggsToTau_EleTau_METEtaRange3p0.sh
qsub -q hepshort.q -t 1-76:1 runJob_VBFHiggsToTau_EleTau_METEtaRange5p0.sh
qsub -q hepshort.q -t 1-76:1 runJob_VBFHiggsToTau_MuoTau_METEtaRange3p0.sh
qsub -q hepshort.q -t 1-76:1 runJob_VBFHiggsToTau_MuoTau_METEtaRange5p0.sh
qsub -q hepshort.q -t 1-76:1 runJob_VBFHiggsToTau_TauTau_METEtaRange3p0.sh
qsub -q hepshort.q -t 1-76:1 runJob_VBFHiggsToTau_TauTau_METEtaRange5p0.sh

qsub -q hepshort.q -t 1-6:1 runJob_VBFHiggsToInv_METEtaRange3p0.sh
qsub -q hepshort.q -t 1-6:1 runJob_VBFHiggsToInv_METEtaRange5p0.sh

qsub -q hepshort.q -t 1-430:1 runJob_ZeroBias_run259721_METEtaRange3p0.sh
qsub -q hepshort.q -t 1-430:1 runJob_ZeroBias_run259721_METEtaRange5p0.sh
