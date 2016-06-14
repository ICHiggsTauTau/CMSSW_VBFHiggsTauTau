#!/bin/bash

qsub -q hep.q -l h_rt=6:0:0 -t 1-50:1 jobs.sh
