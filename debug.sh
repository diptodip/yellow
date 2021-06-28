#!/bin/bash

rm -rf targets;
mkdir targets;
clang++ -Ofast -ffast-math -std=c++14 -lm -pthread -o targets/yellow src/yellow.cpp;
./targets/yellow;
