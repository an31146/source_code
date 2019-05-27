@echo off
gcc -Wall -O2 -std=gnu99 -o %~n1.exe %1 -lmsmpi

