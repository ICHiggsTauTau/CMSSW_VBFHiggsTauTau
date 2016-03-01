from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'VBFHiggsTauTau'
config.General.workArea        = 'L1NTuplesV2_l1t-tsg-v3_METRangeFull_v1'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'cmssw_METRangeFull_cfg.py'

config.Data.inputDataset     = '/VBFHToTauTau_M125_13TeV_powheg_pythia8/pela-Reprocessing_VBFHiggsTauTau_RAW_v3-02e742d2ba16f475bed5477b1348f921/USER'
config.Data.inputDBS         = 'phys03'
config.Data.splitting        = 'EventAwareLumiBased'
config.Data.unitsPerJob      = 50000
config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication      = False
config.Data.outputDatasetTag = 'L1NTuplesV2_l1t-tsg-v3_METRangeFull_v1'

config.Site.whitelist   = ["T2_UK_London_IC"]
config.Site.storageSite = 'T2_UK_London_IC'
