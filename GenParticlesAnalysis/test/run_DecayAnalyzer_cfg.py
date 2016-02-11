import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

import os
import sys
import commands

process = cms.Process('Analysis')

options = VarParsing.VarParsing ('analysis')

options.register ('era',    'stage2',  VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The data taking Era: stage1 or stage2")
options.register ('sample', 'data',    VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The Sample type: data or mc")
options.register ('output', 'DEFAULT', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The output file name")
options.register ('max',    '',        VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,     "The maximum number of events to process")

## options.input = '/store/data/Run2015D/ZeroBias/RAW/v1/000/260/627/00000/00A76FFA-0C82-E511-B441-02163E01450F.root'
options.max    = -1
options.sample = "mc"

options.parseArguments()

print "sample = ",options.sample

################################################################
### Messages
################################################################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

################################################################
### Aligment and condition
################################################################
# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')

# PostLS1 geometry used
#process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2015_cff')
#############################
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag


process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring(
     #'file:/afs/cern.ch/work/p/pela/private/sample/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer15GS-MCRUN2_71_V1-v1/GEN-SIM/8E765C66-7658-E511-A378-00215E2EAEDE.root'
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_1.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_10.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_100.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_101.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_103.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_104.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_105.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_106.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_107.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_108.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_109.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_11.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_110.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_111.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_112.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_113.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_114.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_115.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_116.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_117.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_118.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_119.root',
       '/store/user/pela/VBFHToTauTau_M125_13TeV_powheg_pythia8/Reprocessing_VBFHiggsTauTau_RAW_v3/160129_142736/0000/VBFHToTauTau_M125_step1_v2_12.root'
    ),
    #inputCommands = cms.untracked.vstring('keep *', 
        #'drop *_genParticles_*_*', 
        #'drop *_genParticlesForJets_*_*', 
        #'drop *_kt4GenJets_*_*', 
        #'drop *_kt6GenJets_*_*', 
        #'drop *_iterativeCone5GenJets_*_*', 
        #'drop *_ak4GenJets_*_*', 
        #'drop *_ak7GenJets_*_*', 
        #'drop *_ak8GenJets_*_*', 
        #'drop *_ak4GenJetsNoNu_*_*', 
        #'drop *_ak8GenJetsNoNu_*_*', 
        #'drop *_genCandidatesForMET_*_*', 
        #'drop *_genParticlesForMETAllVisible_*_*', 
        #'drop *_genMetCalo_*_*', 
        #'drop *_genMetCaloAndNonPrompt_*_*', 
        #'drop *_genMetTrue_*_*', 
        #'drop *_genMetIC5GenJs_*_*'),
    secondaryFileNames = cms.untracked.vstring()
)

################################################################
### Input files
################################################################
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.max) )

################################################################
### Analysis
################################################################
process.decayAnalyzer = cms.EDProducer('DecayAnalyzer')

# Output definition

process.output = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string('file:DecayAnalyzer_edm.root'),
  outputCommands = cms.untracked.vstring(
    'drop *', 
    'keep *_simCaloStage2Digis_*_*',    
    'keep *_simGmtStage2Digis_*_*',
    'drop l1tCaloClusterBXVector_simCaloStage2Digis_*_*',
    'drop l1tCaloTowerBXVector_simCaloStage2Digis_*_*',
    'keep *_decayAnalyzer_*_*'
  )
)

################################################################
### Sequence
################################################################
process.analysis = cms.Path(
  #process.RawToDigi*        
  #process.L1TReEmulateFromRAW*  
  process.decayAnalyzer
)
process.output_step = cms.EndPath(process.output)

# Re-emulating, so don't unpack L1T output, might not even exist...
# Also, remove uneeded unpackers for speed.


