#!/bin/bash
mpicc -Wall -O2 -std=c99 -o ${1%.*} $1 -lm
