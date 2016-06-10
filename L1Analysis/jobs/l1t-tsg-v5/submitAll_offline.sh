#!/bin/sh

cd task_offline_VBFHToTauTau_EleHad
./submit.sh
cd ../

cd task_offline_VBFHToTauTau_EleMuo
./submit.sh
cd ../

cd task_offline_VBFHToTauTau_HadHad
./submit.sh
cd ../

cd task_offline_VBFHToTauTau_MuoHad
./submit.sh
cd ../
