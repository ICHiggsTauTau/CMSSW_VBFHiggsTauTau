#!/bin/bash

vbftautau_runRatesExtraction --inputA VBFHiggsToTauTau_EleMuo.root --inputB ZeroBias_run259721.root --outputDir output_tautau_EleMuo | tee output_tautau_EleMuo.txt
vbftautau_runRatesExtraction --inputA VBFHiggsToTauTau_EleHad.root --inputB ZeroBias_run259721.root --outputDir output_tautau_EleHad | tee output_tautau_EleHad.txt
vbftautau_runRatesExtraction --inputA VBFHiggsToTauTau_MuoHad.root --inputB ZeroBias_run259721.root --outputDir output_tautau_MuoHad | tee output_tautau_MuoHad.txt
vbftautau_runRatesExtraction --inputA VBFHiggsToTauTau_HadHad.root --inputB ZeroBias_run259721.root --outputDir output_tautau_HadHad | tee output_tautau_HadHad.txt