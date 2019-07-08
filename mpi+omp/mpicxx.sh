#!/bin/bash
mpicxx -Wall -O2 -std=c++11 -I /usr/include/boost -o ${1%.*} $1 -lboost_mpi -lboost_serialization
