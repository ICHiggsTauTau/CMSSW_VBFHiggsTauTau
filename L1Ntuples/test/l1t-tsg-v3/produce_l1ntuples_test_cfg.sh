#!/bin/bash

cmsRun reEmul.py era=stage1 max=-1 input=file:/afs/cern.ch/work/p/pela/private/sample/ZeroBias/Run2015D-v1/RAW/6A2C6308-F25B-E511-AE4D-02163E0138DB.root sample=data output=ntuples_stage1_zerobias.root
cmsRun reEmul.py era=stage2 max=-1 input=file:/afs/cern.ch/work/p/pela/private/sample/ZeroBias/Run2015D-v1/RAW/6A2C6308-F25B-E511-AE4D-02163E0138DB.root sample=data output=ntuples_stage1_zerobias.root

cmsRun reEmul.py era=stage1 max=-1 input=file:/afs/cern.ch/work/p/pela/private/VBFHTauTau/t/CMSSW_7_6_1/src/VBFHToTauTau_M125_step1.root sample=mc output=ntuples_stage1_vbfhiggstautau.root
cmsRun reEmul.py era=stage2 max=-1 input=file:/afs/cern.ch/work/p/pela/private/VBFHTauTau/t/CMSSW_7_6_1/src/VBFHToTauTau_M125_step1.root sample=mc output=ntuples_stage2_vbfhiggstautau.root

