#!/bin/bash
gcc -Wall -O2 -std=c99 -o $1 $1.c -lmsmpi
