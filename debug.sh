#!/bin/bash

mkdir -p targets;
rm -f targets/yellow_debug;
clang++ -O0 -g -ffast-math -std=c++14 -lm -pthread -o targets/yellow_debug src/yellow.cpp;
./targets/yellow_debug;
