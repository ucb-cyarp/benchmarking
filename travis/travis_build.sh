#!/bin/bash

if [ "$(uname)" == "Darwin" ]; then
    cd ../intrin
    make -f Makefile-noPCM
else
    cd ../intrin
    make
    cd ../core-core
    make
fi