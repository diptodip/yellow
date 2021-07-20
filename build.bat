@echo off
if not exist .\targets @mkdir .\targets
if exist .\targets\yellow.exe @del .\targets\yellow.exe
clang.exe -Ofast -ffast-math -llibcmt -lbcrypt -std=c++14 -o .\targets\yellow.exe .\src\yellow.cpp
.\targets\yellow.exe
