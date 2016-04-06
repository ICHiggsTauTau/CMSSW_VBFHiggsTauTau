# CMSSW_VBFHiggsTauTau
CMSSW Package for the Run II (2016) trigger development

Current this package is to be used with CMSSW_8_X_Y in onder to study the L1 Trigger performance.

*Intructions for Jim Brooke's L1T MET/MHT resolution bug fix*

*Get CMSSW code*
cmsrel CMSSW_8_0_2
cd CMSSW_8_0_2/src
cmsenv
git cms-init
git remote add jimbrooke https://github.com/jimbrooke/cmssw.git
git fetch jimbrooke
git cms-merge-topic jimbrooke:jetmet-update-forjoe-CMSSW_8_0_2
git cms-addpkg L1Trigger/L1TCommon
scram b -j 8

*Get IC Code*
mkdir UserCode
cd UserCode
git clone git@github.com:ajgilbert/ICHiggsTauTau.git
git fetch
git checkout JP-L1Trigger-dev
cd ../
scramv1 b -j8

*Get Joao's CMSSW analysis code*
git clone git@github.com:ICHiggsTauTau/CMSSW_VBFHiggsTauTau.git
cp CMSSW_VBFHiggsTauTau/ChangesToL1Code/L1AnalysisL1Upgrade.cc          L1Trigger/L1TNtuples/src/L1AnalysisL1Upgrade.cc
cp CMSSW_VBFHiggsTauTau/ChangesToL1Code/L1AnalysisL1UpgradeDataFormat.h L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h 
scramv1 b -j8




