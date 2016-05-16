#!/bin/sh


cd task_VBFHToInvisible_METEtaRange3p0
./submit.sh
cd ../

cd task_VBFHToInvisible_METEtaRange5p0
./submit.sh
cd ../

cd task_VBFHToTauTau_EleHad_METEtaRange3p0
./submit.sh
cd ../

cd task_VBFHToTauTau_EleMuo_METEtaRange3p0
./submit.sh
cd ../

cd task_VBFHToTauTau_HadHad_METEtaRange3p0
./submit.sh
cd ../

cd task_VBFHToTauTau_MuoHad_METEtaRange3p0
./submit.sh
cd ../

cd task_ZeroBias_METEtaRange3p0
./submit.sh
cd ../

cd task_ZeroBias_METEtaRange5p0
./submit.sh
cd ../
