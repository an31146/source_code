#!/bin/sh
for f in *.cpp
do
    rm ${f%.*}
done

for f in *.c
do
    rm ${f%.*}
done
