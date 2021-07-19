@echo off
@rmdir /s /q .\targets
@mkdir .\targets
clang.exe -O0 -g -ffast-math -llibcmt -lbcrypt -std=c++14 -o .\targets\yellow_debug.exe .\src\yellow.cpp
.\targets\yellow_debug.exe
