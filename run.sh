#!/bin/bash

# break on first failed command
set -e 

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname ${SCRIPT}`

BUILDPATH=${SCRIPTPATH}/build


rm -rf CMakeFiles/ CMakeCache.txt 
rm -rf ${BUILDPATH}/*

cmake -B ${BUILDPATH} -DCMAKE_CXX_COMPILER=clang++ .  && make && \
${SCRIPTPATH}/tests/Bitslicer_test && \
${SCRIPTPATH}/tests/Lmfile_test && \
${SCRIPTPATH}/tests/Mdatevent_test  

