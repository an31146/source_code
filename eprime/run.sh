#!/bin/bash

set -e

PRIMES_PER_CORE=62500000


time ./prime ${PRIMES_PER_CORE}
