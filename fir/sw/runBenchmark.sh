#!/bin/bash

#Check that dependancies are built
echo "Building Dependencies"

STARTING_DIR=`pwd`
cd ../../depends/pcm
make libPCM.a
cd $STARTING_DIR
cd ../../common
make
cd $STARTING_DIR

echo "Setting Up Slack Reporting"
#Export API Key
source secretkey.sh

echo "Running"
#Run
./fir1_1_tester1_exersiser.py