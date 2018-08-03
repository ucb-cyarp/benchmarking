#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
    cd ../intrin
    make -f Makefile-noPCM
else
    cd ../intrin
    make
    make clean
    make -f Makefile-noPCM
    cd ../core-core
    make
fi