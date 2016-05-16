#!/bin/bash

#hadd VBF_HToInvisible_METEtaRange3p0.root   VBF_HToInvisible_METEtaRange3p0/*.root
#hadd VBF_HToInvisible_METEtaRange5p0.root   VBF_HToInvisible_METEtaRange5p0/*.root
#hadd VBF_HToTauTau_METEtaRange3p0.root      VBF_HToTauTau_METEtaRange3p0/*.root
#hadd VBF_HToTauTau_METEtaRange5p0.root      VBF_HToTauTau_METEtaRange5p0/*.root
#hadd ZeroBias_run259721_METEtaRange3p0.root ZeroBias_run259721_METEtaRange3p0/*.root
#hadd ZeroBias_run259721_METEtaRange5p0.root ZeroBias_run259721_METEtaRange5p0/*.root

hadd VBF_HToInvisible_METEtaRange3p0.root task_VBFHToInvisible_METEtaRange3p0/output/*.root
hadd VBF_HToInvisible_METEtaRange5p0.root task_VBFHToInvisible_METEtaRange5p0/output/*.root
hadd VBF_HToTauTau_EleHad_METEtaRange3p0.root task_VBFHToTauTau_EleHad_METEtaRange3p0/output/*.root
hadd VBF_HToTauTau_EleMuo_METEtaRange3p0.root task_VBFHToTauTau_EleMuo_METEtaRange3p0/output/*.root
hadd VBF_HToTauTau_HadHad_METEtaRange3p0.root task_VBFHToTauTau_HadHad_METEtaRange3p0/output/*.root
hadd VBF_HToTauTau_MuoHad_METEtaRange3p0.root task_VBFHToTauTau_MuoHad_METEtaRange3p0/output/*.root
hadd ZeroBias_run259721_METEtaRange3p0.root task_ZeroBias_METEtaRange3p0/output/*.root
hadd ZeroBias_run259721_METEtaRange5p0.root task_ZeroBias_METEtaRange5p0/output/*.root
