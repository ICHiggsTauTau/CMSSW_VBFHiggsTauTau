vbfMakeJobs.py --task=task_VBFHToInvisible_METEtaRange3p0 --numberOfJobs=5 --fileList=../../../Filelist/data/l1t-integration-v34.0/VBFHToInvisible_METEtaRange3p0_v3.txt --options="--doAnalysisL1TAlgoScan --doAnalysisL1TResolution --jobType mc"
vbfMakeJobs.py --task=task_VBFHToInvisible_METEtaRange5p0 --numberOfJobs=5 --fileList=../../../Filelist/data/l1t-integration-v34.0/VBFHToInvisible_METEtaRange3p0_v3.txt --options="--doAnalysisL1TAlgoScan --doAnalysisL1TResolution --jobType mc"

vbfMakeJobs.py --task=task_VBFHToTauTau_EleMuo_METEtaRange3p0 --numberOfJobs=50 --fileList=../../../Filelist/data/l1t-integration-v34.0/VBFHToTauTau_METEtaRange3p0_v3.txt --options="--doAnalysisL1TAlgoScan --doAnalysisL1TResolution --jobType mc --decay EleMuo"
vbfMakeJobs.py --task=task_VBFHToTauTau_EleHad_METEtaRange3p0 --numberOfJobs=50 --fileList=../../../Filelist/data/l1t-integration-v34.0/VBFHToTauTau_METEtaRange3p0_v3.txt --options="--doAnalysisL1TAlgoScan --doAnalysisL1TResolution --jobType mc --decay EleHad"
vbfMakeJobs.py --task=task_VBFHToTauTau_MuoHad_METEtaRange3p0 --numberOfJobs=50 --fileList=../../../Filelist/data/l1t-integration-v34.0/VBFHToTauTau_METEtaRange3p0_v3.txt --options="--doAnalysisL1TAlgoScan --doAnalysisL1TResolution --jobType mc --decay MuoHad"
vbfMakeJobs.py --task=task_VBFHToTauTau_HadHad_METEtaRange3p0 --numberOfJobs=50 --fileList=../../../Filelist/data/l1t-integration-v34.0/VBFHToTauTau_METEtaRange3p0_v3.txt --options="--doAnalysisL1TAlgoScan --doAnalysisL1TResolution --jobType mc --decay HadHad"

vbfMakeJobs.py --task=task_ZeroBias_METEtaRange3p0 --numberOfJobs=60 --fileList=../../../Filelist/data/l1t-integration-v34.0/ZeroBias_METEtaRange3p0_v3.txt --options="--doAnalysisL1TAlgoScan --doAnalysisL1TResolution --jobType data --runs 259721"

vbfMakeJobs.py --task=task_ZeroBias_METEtaRange5p0 --numberOfJobs=60 --fileList=../../../Filelist/data/l1t-integration-v34.0/ZeroBias_METEtaRange5p0_v3.txt --options="--doAnalysisL1TAlgoScan --doAnalysisL1TResolution --jobType data --runs 259721"


