#!/bin/bash

clang-format -style="file" -i src/*.h
clang-format -style="file" -i src/*.cpp
#clang-format -i src/*.hpp 
