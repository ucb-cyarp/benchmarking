#!/bin/bash

echo "****Compilers****"
echo $CXX
echo $CX
echo "****g++ Verson****"
if [ -z "$CXX" ]; then
    g++ --version
else
    $CXX --version
fi

echo "****Building****"

#Using tecnique from http://bencane.com/2014/09/02/understanding-exit-codes-and-how-to-use-them-in-bash-scripts/
if [ "$(uname)" == "Darwin" ]; then
    cd ../intrin

    make USE_AMDuPROF=0
    if [ $? -eq 0 ]
    then
        echo "****Successfully made intrinsic benchmark with PCM ****"
    else
        echo "****Unable to make intrinsic benchmark with PCM ****" >&2
        exit 1
    fi

    make clean
    cd ../common
    make clean
    cd ../intrin
        if [ $? -eq 0 ]
    then
        echo "****Successfully cleaned intrinsic benchmark ****"
    else
        echo "****Unable to clean intrinsic benchmark ****" >&2
        exit 1
    fi

    make USE_PCM=0 USE_AMDuPROF=0
    if [ $? -eq 0 ]
    then
        echo "****Successfully made intrinsic benchmark with no PCM ****"
    else
        echo "****Unable to make intrinsic benchmark with no PCM ****" >&2
        exit 1
    fi
else
    cd ../intrin
    make
    if [ $? -eq 0 ]
    then
        echo "****Successfully made intrinsic benchmark with PCM and AMDuProf ****"
    else
        echo "****Unable to make intrinsic benchmark with PCM ****" >&2
        exit 1
    fi

    make clean
    cd ../common
    make clean
    cd ../intrin
        if [ $? -eq 0 ]
    then
        echo "****Successfully cleaned intrinsic benchmark ****"
    else
        echo "****Unable to clean intrinsic benchmark ****" >&2
        exit 1
    fi

    make USE_PCM=0 USE_AMDuPROF=0
    if [ $? -eq 0 ]
    then
        echo "****Successfully made intrinsic benchmark with no PCM and no AMDuProf ****"
    else
        echo "****Unable to make intrinsic benchmark with no PCM and no AMDuProf ****" >&2
        exit 1
    fi

    cd ../common
    make clean

    cd ../core-core
    make
    if [ $? -eq 0 ]
    then
        echo "****Successfully made core-core benchmark****"
    else
        echo "****Unable to make core-core benchmark****" >&2
        exit 1
    fi
fi