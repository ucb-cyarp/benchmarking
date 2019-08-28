#!/bin/bash
source ./secretkey.sh
module load AMDuProf
export PYTHONPATH="../common:${PYTHONPATH}"
./run_slack.py
