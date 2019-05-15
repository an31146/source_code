@echo off
gcc -Wall -O2 -std=c99 -o %~n1.exe %1 -lmsmpi
