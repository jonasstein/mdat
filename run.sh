#!/bin/bash

# break on first failed command
set -e 

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname ${SCRIPT}`

BUILDPATH=${SCRIPTPATH}/build


rm -rf CMakeFiles/ CMakeCache.txt 
rm -rf ${BUILDPATH}/*

cmake -B ${BUILDPATH} -DCMAKE_CXX_COMPILER=clang++ .  && make
