#!/bin/bash

makeJobs.py --fileList ../../Filelist/data/ZeroBias_L1NTuplesV2_l1t-tsg-v3.txt --options="--maxEvents -1 --jobType data --runs 258440" --outputFolder="ZeroBias_run258440"
makeJobs.py --fileList ../../Filelist/data/ZeroBias_L1NTuplesV2_l1t-tsg-v3.txt --options="--maxEvents -1 --jobType data --runs 258448" --outputFolder="ZeroBias_run258448"
makeJobs.py --fileList ../../Filelist/data/ZeroBias_L1NTuplesV2_l1t-tsg-v3.txt --options="--maxEvents -1 --jobType data --runs 259721" --outputFolder="ZeroBias_run259721"

makeJobs.py --fileList ../../Filelist/data/VBFHiggsToInv_L1NTuplesV2_l1t-tsg-v3.txt --options="--maxEvents -1 --jobType mc" --outputFolder="VBFHiggsToInv"

makeJobs.py --fileList ../../Filelist/data/VBFHiggsToTauTau_L1NTuplesV2_l1t-tsg-v3.txt --options="--maxEvents -1 --jobType mc" --outputFolder="VBFHiggsToTauTau"

makeJobs.py --fileList ../../Filelist/data/VBFHiggsToTauTau_L1NTuplesV2_l1t-tsg-v3.txt --options="--maxEvents -1 --jobType mc --decay EleMuo" --outputFolder="VBFHiggsToTauTau_EleMuo"
makeJobs.py --fileList ../../Filelist/data/VBFHiggsToTauTau_L1NTuplesV2_l1t-tsg-v3.txt --options="--maxEvents -1 --jobType mc --decay EleHad" --outputFolder="VBFHiggsToTauTau_EleHad"
makeJobs.py --fileList ../../Filelist/data/VBFHiggsToTauTau_L1NTuplesV2_l1t-tsg-v3.txt --options="--maxEvents -1 --jobType mc --decay MuoHad" --outputFolder="VBFHiggsToTauTau_MuoHad"
makeJobs.py --fileList ../../Filelist/data/VBFHiggsToTauTau_L1NTuplesV2_l1t-tsg-v3.txt --options="--maxEvents -1 --jobType mc --decay HadHad" --outputFolder="VBFHiggsToTauTau_HadHad"


