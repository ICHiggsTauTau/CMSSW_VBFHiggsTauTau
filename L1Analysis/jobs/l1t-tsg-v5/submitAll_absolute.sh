#!/bin/sh


#cd task_VBFHToInvisible
#./submit.sh
#cd ../

cd task_VBFHToTauTau_EleHad
./submit.sh
cd ../

cd task_VBFHToTauTau_EleMuo
./submit.sh
cd ../

cd task_VBFHToTauTau_HadHad
./submit.sh
cd ../

cd task_VBFHToTauTau_MuoHad
./submit.sh
cd ../


#cd task_ZeroBias
#./submit.sh
#cd ../
