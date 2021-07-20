@echo off
if not exist .\targets @mkdir .\targets
if exist .\targets\yellow_debug.exe @del .\targets\yellow_debug.exe
clang.exe -O0 -g -ffast-math -llibcmt -lbcrypt -std=c++14 -o .\targets\yellow_debug.exe .\src\yellow.cpp
.\targets\yellow_debug.exe
