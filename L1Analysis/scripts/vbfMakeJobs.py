#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os
import stat
import sys

from optparse import OptionParser

parser = OptionParser()
parser.add_option('--task',        dest="task",help='')
parser.add_option('--numberOfJobs',dest="numberOfJobs",help='')
parser.add_option('--fileList',    dest="fileList",help='')
parser.add_option('--options',     dest="options", help='')
(options, args) = parser.parse_args()

print ""
print "##### Processing Jobs #####"
print "numberOfJobs: ",options.numberOfJobs
print "fileList    : ",options.fileList
print "options     : ",options.options

# If the task folder does not exist create it
taskFolder   = os.getcwd()+"/"+options.task+"/"
jobsFolder   = taskFolder+"jobs/";
outputFolder = taskFolder+"output/";

if options.task is not None:
  if not os.path.exists(taskFolder):
    os.makedirs(taskFolder)
    os.makedirs(jobsFolder)
    os.makedirs(outputFolder)

# Making job lists
nJobs = int(options.numberOfJobs)
inputFiles = list()

with open(options.fileList, 'r') as f:
  for line in f:
    if line.find('.root') != -1:
      inputFiles.append(line.rstrip())

nInputFiles = len(inputFiles)

if nInputFiles<nJobs:
  print "Number of jobs is bigger than number of input files... exiting"
  sys.exit()

filesPerJob = float(nInputFiles) / float(nJobs)
print "Making Jobs "+options.numberOfJobs+" ("+str(filesPerJob)+" files per job) for filelist="+options.fileList

fileIndex = 0
for x in range(0,nJobs):
  with open(taskFolder+"jobs/inputFileList_"+str(x+1)+".txt",'w') as fOut:

    while fileIndex < (x+1)*filesPerJob:
      fOut.write(inputFiles[fileIndex]+"\n");
      fileIndex=fileIndex+1

fJob = open(taskFolder+'jobs.sh','w')
fJob.write("#!/bin/bash\n")
fJob.write("\n")
fJob.write("cd "+taskFolder+"\n")
fJob.write("eval `scramv1 runtime -sh`\n")
fJob.write("vbftautau_runAlgoAnalysis --inputType filelist --input "+jobsFolder+"inputFileList_${SGE_TASK_ID}.txt --outputFilename "+outputFolder+"results_${SGE_TASK_ID}.root --maxEvents -1 "+options.options+"\n")

os.chmod(taskFolder+'jobs.sh', stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IROTH | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)

fJob = open(taskFolder+'submit.sh','w')
fJob.write("#!/bin/bash\n")
fJob.write("\n")
fJob.write("qsub -q hepmedium.q -t 1-"+options.numberOfJobs+":1 jobs.sh\n")

os.chmod(taskFolder+'submit.sh', stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IROTH | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)
