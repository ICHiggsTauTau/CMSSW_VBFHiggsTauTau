# /dev/CMSSW_7_4_0/GRun/V79 (CMSSW_7_4_3)

import FWCore.ParameterSet.Config as cms

process = cms.Process( "RUN" )

################################################################
### Messages
################################################################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

###############################################################
# START: HLT Simple Analysis
###############################################################
process.hltAnalysis = cms.EDAnalyzer('TriggerSimpleAnalyzer',

  inputTag_HLTResults = cms.untracked.InputTag("TriggerResults"),
  
  HLTPaths  = cms.vstring(
    "HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v",
    "HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v",
    "HLT_Ele23_WPLoose_Gsf_v",
    "HLT_IsoMu18_v",
    "HLT_IsoMu22_v",
    "HLT_IsoTkMu27_v",
    "HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v",
    "HLT_Ele22_eta2p1_WPLoose_Gsf_v",
    "HLT_Ele32_eta2p1_WPTight_Gsf_v",
    "HLT_Ele27_eta2p1_WPLoose_Gsf_v",
    "HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v",
    "HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v",
    "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v",
    "HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v",
    "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"
  ), 

  outputFilename = cms.untracked.string("HLTSimpleAnalysis.root"),

)
###############################################################
# END: HLT Simple Analysis
###############################################################

process.HLTanalysis = cms.Path(process.hltAnalysis)
process.HLTSchedule = cms.Schedule(process.HLTanalysis)


###############################################################
# BEGIN: Running over the signal sample with PU20bx25
###############################################################
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/5C1A75E5-45A7-E511-A931-02163E00F8BA.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/88A5F652-40A7-E511-A417-02163E00E5BC.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/BE8325F3-61A7-E511-98E8-001E67397C79.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/DC311FF1-61A7-E511-93F5-02163E017895.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/041F97C7-2DA5-E511-8B3F-002590A83354.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/04242F25-16A4-E511-8B76-02163E0176F7.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/0C511A1D-36A3-E511-9499-02163E0177C9.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1036BCE3-FAA5-E511-98AF-001E67397620.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/12FB292D-16A3-E511-93F6-02163E0160E7.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1A94530F-05A3-E511-BAB6-02163E017883.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1AC8C429-FAA2-E511-ACDA-02163E014CB8.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1AEF3D1B-13A3-E511-ACC3-02163E0179B4.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1CAD52AF-31A3-E511-9F5B-02163E014CE5.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/1E55B6D5-2FA3-E511-AA37-02163E00BF30.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/22A4DA71-49A3-E511-B7E9-02163E01680D.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/22D1D805-B7A3-E511-AE87-0025905C431C.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/28AD4C40-04A3-E511-9862-02163E016143.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/2EDE446C-30A3-E511-A326-02163E0146E1.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3213E271-FFA2-E511-8FDE-02163E0139F5.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/32BAC5FE-ECA2-E511-8545-0025905C94D0.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/34CB1C8F-33A3-E511-8632-02163E014692.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/36E89A84-F6A2-E511-B6DA-02163E012FD9.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/38F16E5A-F6A2-E511-8CF9-001E675049F5.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/3A44DE7F-FDA2-E511-977A-02163E00BC86.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/4494EE7C-09A3-E511-BD8F-02163E0165D8.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/46FE9CEC-01A3-E511-AF35-001E673975F8.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/4AA4BCD2-FCA2-E511-9546-02163E013220.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/4CDFA920-07A3-E511-87DE-02163E013F59.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/523F0758-48A3-E511-B108-02163E0153D6.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/542E77B5-39A3-E511-89EE-02163E01668D.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/54D112D3-3CA3-E511-BD43-02163E0153D2.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/58E4874F-F8A2-E511-B971-02163E014F13.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/5A740967-22A3-E511-9C00-02163E013A05.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/5ED69298-01A3-E511-B0F1-02163E00F72A.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/60AA1A3B-10A3-E511-ACF2-02163E0116EF.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/66EF1B22-C8A3-E511-B87A-0CC47A4D99E6.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/6C60650D-F5A2-E511-932D-02163E013B38.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/72801C54-12A3-E511-98E3-02163E016551.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/7CD51265-0BA3-E511-B6EC-02163E0147F9.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/802449ED-30A3-E511-BD47-02163E017954.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/849ED255-20A3-E511-B9D8-02163E0114DE.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/884ACD99-31A3-E511-AEE2-02163E017686.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/88DBB021-E4A3-E511-820F-001E67E6F512.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/8E61161C-21A3-E511-B3C2-002590A3CA16.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/90D157D7-34A3-E511-9061-02163E01684F.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/92F125EC-0CA3-E511-B3B9-02163E015FFB.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/96BADE04-0FA3-E511-B9FE-02163E0177C3.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/9AA346BB-51A5-E511-A67D-02163E0133DF.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/9C0D8F5C-F4A2-E511-A1C3-02163E0136B4.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/9E684D22-09A3-E511-ABDE-02163E013C54.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/9EA722C7-FAA2-E511-8AFD-02163E0135AF.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/9EF76B72-0BA3-E511-BC41-0025904B2C4C.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A29B1F32-0EA3-E511-9D53-02163E0177FB.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A41C865A-2EA3-E511-92DE-02163E0178E3.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A81A8971-0DA3-E511-855E-02163E013C42.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/A85A0C5C-13A4-E511-A7F2-02163E01445E.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/B49C5148-08A3-E511-A353-02163E00AD56.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/B602F641-F8A2-E511-A804-02163E013B3E.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/BE9E25AC-ECA2-E511-963E-0025904C650A.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/BEC5A213-FFA2-E511-9993-0CC47A04CFFA.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/C2DAB11C-0AA3-E511-84FD-02163E013EA3.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/C4933532-1FA3-E511-A6C9-02163E015CD8.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/C6D23524-FBA2-E511-9B03-02163E013B8D.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/CE609087-31A5-E511-8352-002590A4C6AE.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/CEE1BE10-2DA3-E511-A0FB-02163E017642.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/D63F0A7C-0BA3-E511-B5AE-001E673975F8.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/D6E1B7D5-16A3-E511-B66C-02163E013B81.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/DC5B4C89-35A3-E511-BC31-02163E013F14.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/DCFFCC8D-02A3-E511-8195-02163E0139F5.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/E6668FAA-2AA3-E511-B65F-02163E0160E3.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/E86507B0-33A3-E511-85F1-02163E00F74F.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/EC151F42-5BA5-E511-A6B8-02163E0176FD.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/F49F58CC-39A3-E511-B70E-02163E014DFF.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/FC46441C-36A3-E511-A6DA-02163E014A60.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/08F4B062-F1A3-E511-9EB0-02163E0136B9.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/12D2FF61-3DA4-E511-8352-02163E013253.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/14FD712C-33A4-E511-B220-02163E013101.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/1A93484C-F1A3-E511-8602-02163E0178E3.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/1E971505-21A5-E511-8CC6-02163E0177F0.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/3A33D338-F6A3-E511-98B7-02163E014E69.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/449B25ED-EBA3-E511-8D64-02163E013EE6.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/4AE7B873-39A4-E511-9187-02163E015394.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/64AB0721-32A4-E511-B080-02163E01655D.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/6AEBBDF3-EFA3-E511-89CF-02163E016BBC.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/7E949BE7-EDA3-E511-8540-02163E01771E.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/8C4D40D4-EFA3-E511-8AA9-02163E016955.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/9CA0C7BD-39A4-E511-B8F4-02163E011924.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/A2F1BDD3-EFA3-E511-A4C5-02163E013EE6.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/A4513B87-FDA3-E511-9792-02163E012D1A.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/A8A827A5-E8A3-E511-84C4-02163E01307B.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/AEAB7EB9-41A4-E511-BB7B-02163E0178DE.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/C8691DF8-EBA3-E511-B86F-02163E016C07.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/CAFE1EE9-F3A3-E511-A715-02163E013A07.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/DABB5643-EAA3-E511-87F0-02163E016430.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/E0292FE4-EDA3-E511-B905-02163E0177BD.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/E2D31B1D-47A6-E511-9426-001E67E71B96.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/E4F7E8CF-34A4-E511-815D-02163E0149EF.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/E83A3AB4-E5A3-E511-B93B-02163E0178CA.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/EA0C0DCC-EFA3-E511-89AC-02163E01763C.root',
       '/store/mc/RunIIFall15DR76/VBFHToTauTau_M125_13TeV_powheg_pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/70000/F2CF5C43-EAA3-E511-9D1C-02163E017668.root'
  ),
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

