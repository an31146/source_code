@echo off
g++ -Wall -O2 -std=c++11 -o omp_segmented_sieve.exe omp_segmented_sieve.cpp -fopenmp
