from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'VBFHiggsTauTau'
config.General.workArea = 'Reprocessing_RAW_v1'
config.General.transferOutputs = True
config.General.transferLogs    = False

config.JobType.pluginName = 'ANALYSIS'
config.JobType.psetName   = 'make_GEN-SIM-RAW_likeTSG_cfg.py'

config.Data.inputDataset  = '/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer15GS-MCRUN2_71_V1-v1/GEN-SIM'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'FileBased'
config.Data.unitsPerJob   = 1
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication   = True
config.Data.outputDatasetTag = 'Reprocessing_VBFHiggsTauTau_RAW_20160122'

config.Site.storageSite     = 'T2_UK_London_IC'
