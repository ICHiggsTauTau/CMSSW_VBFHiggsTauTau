#vbfMakeJobs.py --task=task_VBFHToInvisible --numberOfJobs=5 --fileList=../../../Filelist/data/ICVBFNtuples_l1t-tag-v5/VBFHiggsToInvisible.dat --options="--doAnalysisL1TAlgoScan --jobType mc"


vbfMakeJobs.py --task=task_VBFHToTauTau_EleMuo --numberOfJobs=50 --fileList=../../../Filelist/data/ICVBFNtuples_l1t-integration-v58.1/TauTau_int_v58pt1.dat --options="--doAnalysisL1TAlgoScan --jobType mc --decay EleMuo"
vbfMakeJobs.py --task=task_VBFHToTauTau_EleHad --numberOfJobs=50 --fileList=../../../Filelist/data/ICVBFNtuples_l1t-integration-v58.1/TauTau_int_v58pt1.dat --options="--doAnalysisL1TAlgoScan --jobType mc --decay EleHad"
vbfMakeJobs.py --task=task_VBFHToTauTau_MuoHad --numberOfJobs=50 --fileList=../../../Filelist/data/ICVBFNtuples_l1t-integration-v58.1/TauTau_int_v58pt1.dat --options="--doAnalysisL1TAlgoScan --jobType mc --decay MuoHad"
vbfMakeJobs.py --task=task_VBFHToTauTau_HadHad --numberOfJobs=50 --fileList=../../../Filelist/data/ICVBFNtuples_l1t-integration-v58.1/TauTau_int_v58pt1.dat --options="--doAnalysisL1TAlgoScan --jobType mc --decay HadHad"

#vbfMakeJobs.py --task=task_ZeroBias --numberOfJobs=100 --fileList=../../../Filelist/data/ICVBFNtuples_l1t-tag-v5/ZeroBias.dat --options="--doAnalysisL1TAlgoScan --jobType data"


