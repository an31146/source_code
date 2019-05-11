#!/bin/sh
gcc -Wall -O2 -std=c99 parallelQuickSort.c -o parallelQuickSort -lpthread
gcc -Wall -O2 -std=c99 parallelMergeSort.c -o parallelMergeSort -lpthread
