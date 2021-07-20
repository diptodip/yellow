#!/bin/bash

mkdir -p targets;
rm -f targets/yellow;
clang++ -Ofast -ffast-math -std=c++14 -lm -pthread -o targets/yellow src/yellow.cpp;
./targets/yellow;
