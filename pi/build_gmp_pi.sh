#!/bin/bash
gcc.exe -Wall -O2 --std=c99 gmp_pi.c -o gmp_pi -fopenmp -lgmp -lgomp
