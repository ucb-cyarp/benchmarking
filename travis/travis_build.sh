#!/bin/bash

#Using tecnique from http://bencane.com/2014/09/02/understanding-exit-codes-and-how-to-use-them-in-bash-scripts/
if [ "$(uname)" == "Darwin" ]; then
    cd ../intrin
    make -f Makefile-noPCM

    if [ $? -eq 0 ]
    then
        echo "****Successfully made intrinsic benchmark with no PCM ****"
        exit 0
    else
        echo "****Unable to make intrinsic benchmark with no PCM ****" >&2
        exit 1
    fi
else
    cd ../intrin
    make
    if [ $? -eq 0 ]
    then
        echo "****Successfully made intrinsic benchmark with PCM ****"
        exit 0
    else
        echo "****Unable to make intrinsic benchmark with PCM ****" >&2
        exit 1
    fi

    make clean
        if [ $? -eq 0 ]
    then
        echo "****Successfully cleaned intrinsic benchmark ****"
        exit 0
    else
        echo "****Unable to clean intrinsic benchmark ****" >&2
        exit 1
    fi

    make -f Makefile-noPCM
    if [ $? -eq 0 ]
    then
        echo "****Successfully made intrinsic benchmark with no PCM ****"
        exit 0
    else
        echo "****Unable to make intrinsic benchmark with no PCM ****" >&2
        exit 1
    fi

    cd ../core-core
    make
    if [ $? -eq 0 ]
    then
        echo "****Successfully made core-core benchmark****"
        exit 0
    else
        echo "****Unable to make core-core benchmark****" >&2
        exit 1
    fi
fi