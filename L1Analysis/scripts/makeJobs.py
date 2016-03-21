#! /usr/bin/env python

import os
import stat
import sys

from optparse import OptionParser

parser = OptionParser()
parser.add_option('--outputFolder',dest="outputFolder",help='')
parser.add_option('--fileList',    dest="fileList",help='')
parser.add_option('--options',     dest="options", help='')
(options, args) = parser.parse_args()

print "outputFolder: ",options.outputFolder
print "fileList    : ",options.fileList
print "options     : ",options.options

# If the output folder does not exist create it
if options.outputFolder is not None:
  outputFolder=options.outputFolder+"/"
  if not os.path.exists(outputFolder):
    os.makedirs(outputFolder)

inputFiles = list()
with open(options.fileList,'r') as f:
  for line in f:
    if line.find('.root') != -1:
      inputFiles.append(line.rstrip())


fSubmit = open(options.outputFolder+'/submitJobs.sh','w')
fSubmit.write("#!/bin/sh\n")
fSubmit.write("\n")


fileIndex = 0
for x in range(0,len(inputFiles)):
  
  fSubmit.write("qsub -q hepshort.q runJob_"+str(x)+".sh\n")
  
  with open(outputFolder+'runJob_'+str(x)+'.sh','w') as fOut:
    fOut.write("#!/bin/bash\n")
    fOut.write("\n")
    fOut.write("cd /vols/cms02/jca10/VBFHiggsToTauTau/l1t-tsg-v3/CMSSW_8_0_0_pre6/src/\n");
    fOut.write("eval `scramv1 runtime -sh`\n");
    fOut.write("cd "+os.getcwd()+"/"+outputFolder+"\n")
    fOut.write("vbftautau_runTriggerAnalysis --input "+inputFiles[x]+" --outputFilename results_"+str(x)+".root "+options.options+"\n")

  os.chmod(outputFolder+'/runJob_'+str(x)+'.sh', stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IROTH | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)


os.chmod(outputFolder+'/submitJobs.sh', stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IROTH | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)
