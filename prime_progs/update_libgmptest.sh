#!/bin/bash

new_prime=`tail -n 2 twin_primes.txt`
old_prime=`awk '/"([0-9]+)"/ { print $2 }' libgmp_test.c`
sed -i.bak 's/'${old_prime}'/"'${new_prime}'", /' libgmp_test.c
gcc -Wall -O2 -std=gnu99 -o libgmp_test libgmp_test.c -lm -lgmp
nohup ./libgmp_test > /dev/null 2>&1 &
