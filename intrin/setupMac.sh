#!/bin/bash

#This script should be sourced so that the path is set

#Using trick from https://unix.stackexchange.com/questions/4650/determining-path-to-sourced-shell-script

SCRIPT_LOC=
CALLED=$_
if [[ ${CALLED} != $0 ]]; then
    #Script was sourced
    SCRIPT_LOC=`dirname "${BASH_SOURCE}"`
else
    #Script was called
    SCRIPT_LOC=`dirname "$0"`
fi

OLD_DIR=`pwd`
cd ${SCRIPT_LOC}/..
BASE_DIR=`pwd`
cd ${OLD_DIR}

export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:${BASE_DIR}/depends/pcm/MacMSRDriver/build/Release