#!/bin/sh
g++ -Wall -O2 -std=c++11 -o thread thread.cpp -lpthread
g++ -Wall -O2 -std=c++14 -o shared_mutex shared_mutex.cpp -lpthread
