@echo off
gcc -Wall -O2 -std=c99 -o %1.exe %1.c -lmsmpi
