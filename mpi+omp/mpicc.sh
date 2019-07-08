#!/bin/bash
mpicc -Wall -O2 -std=gnu99 -o ${1%.*} $1 -lm -lboost_mpi
