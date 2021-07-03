#!/bin/bash

rm -rf targets;
mkdir targets;
clang++ -O0 -g -ffast-math -std=c++14 -lm -pthread -o targets/yellow_debug src/yellow.cpp;
./targets/yellow;
