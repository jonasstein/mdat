#!/bin/bash

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname ${SCRIPT}`

rm -Rf CMakeFiles/ CMakeCache.txt && cmake -B ${SCRIPTPATH}/build -DCMAKE_CXX_COMPILER=clang++ .  && make
