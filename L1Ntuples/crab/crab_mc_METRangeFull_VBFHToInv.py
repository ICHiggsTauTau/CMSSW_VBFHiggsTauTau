from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'VBF_HToInvisible'
config.General.workArea        = 'L1NTuplesV2_l1t-tsg-v3_METRangeFull_v1'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'cmssw_METRangeFull_cfg.py'

config.Data.inputDataset     = '/VBF_HToInvisible_M125_13TeV_powheg_pythia8/RunIIFall15DR76-25nsFlat10to25TSG_76X_mcRun2_asymptotic_v12-v1/GEN-SIM-RAW'
config.Data.inputDBS         = 'global'
config.Data.splitting        = 'EventAwareLumiBased'
config.Data.unitsPerJob      = 50000
config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication      = False
config.Data.outputDatasetTag = 'L1NTuplesV2_l1t-tsg-v3_METRangeFull_v1'

#config.Site.whitelist   = ["T2_UK_London_IC"]
config.Site.storageSite = 'T2_UK_London_IC'
