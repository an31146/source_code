#!/bin/sh
for f in *.cpp
do
    rm ${f%.*} > /dev/null 2>&1
done

for f in *.c
do
    rm ${f%.*} >/dev/null 2>&1
done
