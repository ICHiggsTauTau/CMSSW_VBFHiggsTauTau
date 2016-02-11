import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

import os
import sys
import commands

process = cms.Process('L1Analysis')

options = VarParsing.VarParsing ('analysis')

options.register ('sample', 'data',    VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string,  "The Sample type: data or mc")
options.register ('input',  '',        VarParsing.VarParsing.multiplicity.list,      VarParsing.VarParsing.varType.string,  "The input files")
options.register ('max',    '',        VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int,     "The maximum number of events to process")
options.register ('verbose', '',       VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool,    "Verbose")

options.sample  = "mc"
options.input   = '/store/data/Run2015D/ZeroBias/RAW/v1/000/260/627/00000/00A76FFA-0C82-E511-B441-02163E01450F.root'
options.max     = 100
options.verbose = False

options.parseArguments()

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

# PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2015_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

if options.sample=="mc":
  process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

elif options.sample=="data":
  process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
    
 

################################################################
### Input files
################################################################
process.source = cms.Source("PoolSource",
    fileNames          = cms.untracked.vstring(options.input),
    secondaryFileNames = cms.untracked.vstring()
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.max) )

################################################################
### Analysis
################################################################
process.tauAnalysis = cms.EDAnalyzer('TauAnalysis',
  verbose            = cms.untracked.bool    (options.verbose),
  input_L1TEra       = cms.untracked.string  ("stage2"),
  inputTag_L1TEGamma = cms.untracked.InputTag("simCaloStage2Digis"),
  inputTag_L1TMuon   = cms.untracked.InputTag("simGmtStage2Digis"),
  inputTag_L1TTau    = cms.untracked.InputTag("simCaloStage2Digis"),
  inputTag_L1TJet    = cms.untracked.InputTag("simCaloStage2Digis"),
  inputTag_L1TSum    = cms.untracked.InputTag("simCaloStage2Digis","")
)

process.analysis = cms.Path(
  process.tauAnalysis
)
