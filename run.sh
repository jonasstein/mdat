#!/bin/bash

rm -Rf CMakeFiles/ CMakeCache.txt && cmake -DCMAKE_CXX_COMPILER=clang++ .  && make
