#! /usr/bin/env python

import os
import stat
import sys
import re
import collections

from optparse import OptionParser

class Algo:
  name = ""
  rate = ""
  effEleMuo = 0.
  effEleHad = 0.
  effMuoHad = 0.
  effHadHad = 0.
    
  def dump(self):
    print '{0:39s} & {1:10.1f} & {2:6.4f} & {3:6.4f} & {4:6.4f} & {5:6.4f} \\\\'.format(self.name,self.rate,self.effEleMuo,self.effEleHad,self.effMuoHad,self.effHadHad)

#parser = OptionParser()
#parser.add_option('--outputFolder',dest="outputFolder",help='')
#(options, args) = parser.parse_args()

fEleMuo = open('output_tautau_EleMuo.txt','r')
fEleHad = open('output_tautau_EleHad.txt','r')
fMuoHad = open('output_tautau_MuoHad.txt','r')
fHadHad = open('output_tautau_HadHad.txt','r')

algos = dict();

content = fEleMuo.readlines()
for x in content:
  if x.find("Algo:")!=-1:
    words = re.split(r'[;,\s]\s*', x)
    al = Algo();
    al.name      = words[1]
    al.effEleMuo = float(words[4])
    al.rate      = float(words[6])
    algos[ words[1] ] = al
    
content = fEleHad.readlines()
for x in content:
  if x.find("Algo:")!=-1:
    words = re.split(r'[;,\s]\s*', x)
    algos[ words[1] ].effEleHad = float(words[4])

content = fMuoHad.readlines()
for x in content:
  if x.find("Algo:")!=-1:
    words = re.split(r'[;,\s]\s*', x)
    algos[ words[1] ].effMuoHad = float(words[4])

content = fHadHad.readlines()
for x in content:
  if x.find("Algo:")!=-1:
    words = re.split(r'[;,\s]\s*', x)
    algos[ words[1] ].effHadHad = float(words[4])


OrderedAlgos = collections.OrderedDict(sorted(algos.items()))
#for key, value in OrderedAlgos.iteritems():
  #value.dump()

########################################################
print ""
print "---++++ Single Object"
print ""

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: EG\" hidelink=\"Hide Results: EG\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

singleEG = dict();
for key, value in OrderedAlgos.iteritems():

  if key.find("_")==-1 and key.startswith("EG"):
    singleEG[key] = value
    del OrderedAlgos[key]
orSingleEG = collections.OrderedDict(sorted(singleEG.items()))

for key, value in orSingleEG.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Mu\" hidelink=\"Hide Results: Mu\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

singleMu = dict();
for key, value in OrderedAlgos.iteritems():
  if key.find("_")==-1 and key.startswith("Mu"):
    singleMu[key] = value
    del OrderedAlgos[key]
orSingleMu = collections.OrderedDict(sorted(singleMu.items()))

for key, value in orSingleMu.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Tau\" hidelink=\"Hide Results: Tau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

singleTau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.find("_")==-1 and key.startswith("Tau"):
    singleTau[key] = value
    del OrderedAlgos[key]
orSingleTau = collections.OrderedDict(sorted(singleTau.items()))

for key, value in orSingleTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: IsoTau\" hidelink=\"Hide Results: IsoTau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

singleIsoTau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.find("_")==-1 and key.startswith("IsoTau"):
    singleIsoTau[key] = value
    del OrderedAlgos[key]
orSingleIsoTau = collections.OrderedDict(sorted(singleIsoTau.items()))

for key, value in orSingleIsoTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: MET\" hidelink=\"Hide Results: MET\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

met = dict();
for key, value in OrderedAlgos.iteritems():
  if key.find("_")==-1 and key.startswith("MET"):
    met[key] = value
    del OrderedAlgos[key]
orMET = collections.OrderedDict(sorted(met.items()))

for key, value in orMET.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: MHT\" hidelink=\"Hide Results: MHT\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

mht = dict();
for key, value in OrderedAlgos.iteritems():
  if key.find("_")==-1 and key.startswith("MHT"):
    mht[key] = value
    del OrderedAlgos[key]
orMHT = collections.OrderedDict(sorted(mht.items()))

for key, value in orMHT.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"
print ""

########################################################
print ""
print "---++++ Double object"
print ""

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet\" hidelink=\"Hide Results: Dijet\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijet = dict();
for key, value in OrderedAlgos.iteritems():
  if key.find("_")==-1 and key.startswith("Dijet"):
    dijet[key] = value
    del OrderedAlgos[key]
orDijet = collections.OrderedDict(sorted(dijet.items()))

for key, value in orDijet.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: EG + Muon\" hidelink=\"Hide Results: EG + Muon\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

eg_mu = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("EG") and key.find("_Mu")!=-1:
    eg_mu[key] = value
    del OrderedAlgos[key]
orEg_Mu = collections.OrderedDict(sorted(eg_mu.items()))

for key, value in orEg_Mu.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: EG + Tau\" hidelink=\"Hide Results: EGamma + Tau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

eg_tau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("EG") and key.find("_Tau")!=-1:
    eg_tau[key] = value
    del OrderedAlgos[key]
orEg_Tau = collections.OrderedDict(sorted(eg_tau.items()))

for key, value in orEg_Tau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: EG + IsoTau\" hidelink=\"Hide Results: EG + IsoTau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

eg_isotau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("EG") and key.find("_IsoTau")!=-1:
    eg_isotau[key] = value
    del OrderedAlgos[key]
orEg_IsoTau = collections.OrderedDict(sorted(eg_isotau.items()))

for key, value in orEg_IsoTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Muon + Tau\" hidelink=\"Hide Results: Muon + Tau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

mu_tau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("Mu") and key.find("_Tau")!=-1:
    mu_tau[key] = value
    del OrderedAlgos[key]
orMu_Tau = collections.OrderedDict(sorted(mu_tau.items()))

for key, value in orMu_Tau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Muon + IsoTau\" hidelink=\"Hide Results: Muon + IsoTau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

mu_isotau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("Mu") and key.find("_IsoTau")!=-1:
    mu_isotau[key] = value
    del OrderedAlgos[key]
orMu_IsoTau = collections.OrderedDict(sorted(mu_isotau.items()))

for key, value in orMu_IsoTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Double Tau\" hidelink=\"Hide Results: Double Tau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

ditau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==0 and key.startswith("DiTau"):
    ditau[key] = value
    del OrderedAlgos[key]
orDiTau = collections.OrderedDict(sorted(ditau.items()))

for key, value in orDiTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Double IsoTau\" hidelink=\"Hide Results: Double IsoTau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

diisotau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==0 and key.startswith("DiIsoTau"):
    diisotau[key] = value
    del OrderedAlgos[key]
orDiIsoTau = collections.OrderedDict(sorted(diisotau.items()))

for key, value in orDiIsoTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print ""
print "---++++ Dijet + Single Object"
print ""

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + EGamma\" hidelink=\"Hide Results: Dijet + EGamma\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetEgamma = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("Dijet") and key.find("EG")!=-1:
    dijetEgamma[key] = value
    del OrderedAlgos[key]
orDijetEgamma = collections.OrderedDict(sorted(dijetEgamma.items()))

for key, value in orDijetEgamma.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + Muon\" hidelink=\"Hide Results: Dijet + Muon\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetMu = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("Dijet") and key.find("Mu")!=-1:
    dijetMu[key] = value
    del OrderedAlgos[key]
orDijetMu = collections.OrderedDict(sorted(dijetMu.items()))

for key, value in orDijetMu.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + Tau\" hidelink=\"Hide Results: Dijet + Tau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetTau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("Dijet") and key.find("_Tau")!=-1:
    dijetTau[key] = value
    del OrderedAlgos[key]
orDijetTau = collections.OrderedDict(sorted(dijetTau.items()))

for key, value in orDijetTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + IsoTau\" hidelink=\"Hide Results: Dijet + IsoTau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetIsoTau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("Dijet") and key.find("_IsoTau")!=-1:
    dijetIsoTau[key] = value
    del OrderedAlgos[key]
orDijetIsoTau = collections.OrderedDict(sorted(dijetIsoTau.items()))

for key, value in orDijetIsoTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"
print ""

########################################################
print ""
print "---++++ Dijet + Double Object"
print ""

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + EGamma + Muon\" hidelink=\"Hide Results: Dijet + EGamma + Muon\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetEGMu = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==2 and key.startswith("Dijet") and key.find("_EG")!=-1 and key.find("_Mu")!=-1:
    dijetEGMu[key] = value
    del OrderedAlgos[key]
orDijetEGMu = collections.OrderedDict(sorted(dijetEGMu.items()))

for key, value in orDijetEGMu.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"


########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + EGamma + Tau\" hidelink=\"Hide Results: Dijet + EGamma + Tau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetEGTau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==2 and key.startswith("Dijet") and key.find("_EG")!=-1 and key.find("_Tau")!=-1:
    dijetEGTau[key] = value
    del OrderedAlgos[key]
orDijetEGTau = collections.OrderedDict(sorted(dijetEGTau.items()))

for key, value in orDijetEGTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + EGamma + IsoTau\" hidelink=\"Hide Results: Dijet + EGamma + IsoTau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetEGIsoTau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==2 and key.startswith("Dijet") and key.find("_EG")!=-1 and key.find("_IsoTau")!=-1:
    dijetEGIsoTau[key] = value
    del OrderedAlgos[key]
orDijetEGIsoTau = collections.OrderedDict(sorted(dijetEGIsoTau.items()))

for key, value in orDijetEGIsoTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + Mu + Tau\" hidelink=\"Hide Results: Dijet + Mu + Tau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetMuTau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==2 and key.startswith("Dijet") and key.find("_Mu")!=-1 and key.find("_Tau")!=-1:
    dijetMuTau[key] = value
    del OrderedAlgos[key]
orDijetMuTau = collections.OrderedDict(sorted(dijetMuTau.items()))

for key, value in orDijetMuTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + Muon + IsoTau\" hidelink=\"Hide Results: Dijet + Muon + IsoTau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetMuIsoTau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==2 and key.startswith("Dijet") and key.find("_Mu")!=-1 and key.find("_IsoTau")!=-1:
    dijetMuIsoTau[key] = value
    del OrderedAlgos[key]
orDijetMuIsoTau = collections.OrderedDict(sorted(dijetMuIsoTau.items()))

for key, value in orDijetMuIsoTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + Double Tau\" hidelink=\"Hide Results: Dijet + Double Tau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetDiTau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("Dijet") and key.find("_DiTau")!=-1:
    dijetDiTau[key] = value
    del OrderedAlgos[key]
orDijetDiTau= collections.OrderedDict(sorted(dijetDiTau.items()))

for key, value in orDijetDiTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"

########################################################
print "%TWISTY{mode=\"div\" start=\"hide\" showlink=\"Show Results: Dijet + Double IsoTau\" hidelink=\"Hide Results: Dijet + Double IsoTau\" showimgleft=\"%ICONURLPATH{toggleopen-small}%\" hideimgleft=\"%ICONURLPATH{toggleclose-small}%\" }%"
print "%BEGINLATEX%"
print "\\begin{tabular}{|l|r|c|c|c|c|}"
print "\hline"
print "     &              & \multicolumn{4}{c|}{Higgs Decay Channel} \\\\"
print "\hline"
print "Algo & L1 Rate [Hz] & $e\\mu$ & $e\\tau$ & $\\mu\\tau$ & $\\tau\\tau$ \\\\"
print "\hline\hline"

dijetDiIsoTau = dict();
for key, value in OrderedAlgos.iteritems():
  if key.count("_")==1 and key.startswith("Dijet") and key.find("_DiIsoTau")!=-1:
    dijetDiIsoTau[key] = value
    del OrderedAlgos[key]
orDijetDiIsoTau= collections.OrderedDict(sorted(dijetDiIsoTau.items()))

for key, value in orDijetDiIsoTau.iteritems():
  value.dump()

print "\hline"
print "\\end{tabular}"
print "%ENDLATEX%"
print "%ENDTWISTY%"
print ""

