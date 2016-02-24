from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName     = 'ZeroBias'
config.General.workArea        = 'L1NTuples_v1_20160212'
config.General.transferOutputs = True
config.General.transferLogs    = True

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'run_L1ReEmu_nTuplesWithTaus_cfg.py'
config.JobType.pyCfgParams = ['sample=data']

config.Data.inputDataset     = '/ZeroBias/Run2015D-v1/RAW'
config.Data.inputDBS         = 'global'
config.Data.splitting        = 'EventAwareLumiBased'
config.Data.unitsPerJob      = 50000
config.Data.outLFNDirBase    = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication      = False
config.Data.outputDatasetTag = 'L1NTuples_v1_20160212v2'

#config.Site.whitelist   = ["T2_UK_London_IC"]
config.Site.storageSite = 'T2_UK_London_IC'
