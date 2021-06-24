#!/bin/bash

rm -rf targets;
mkdir targets;
clang++ -Ofast -v -std=c++11 -lm -fopenmp -o targets/yellow src/yellow.cpp;
./targets/yellow;
