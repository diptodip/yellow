@echo off
@rmdir /s /q .\targets
@mkdir .\targets
clang.exe -Ofast -ffast-math -llibcmt -lbcrypt -std=c++14 -o .\targets\yellow.exe .\src\yellow.cpp
.\targets\yellow.exe
