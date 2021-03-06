//
// gcc -O2 -Wall -std=c99 -o mpi_prime.exe mpi_prime.c -lmsmpi
//
/******************************************************************************
* FILE: mpi_prime.c
* DESCRIPTION:
*   Generates prime numbers.  All tasks distribute the work evenly, taking
*   every nth number, where n is the stride computed as:  (rank *2) + 1
*   so that even numbers are automatically skipped.  The method of using
*   stride is preferred over contiguous blocks of numbers, since numbers
*   in the higher range require more work to compute and may result in 
*   load imbalance.  This program demonstrates embarrassing parallelism.
*   Collective communications calls are used to reduce the only two data
*   elements requiring communications: the number of primes found and
*   the largest prime.
* AUTHOR: Blaise Barney 11/25/95 - adapted from version contributed by 
*   Richard Ng &  Wong Sze Cheong during MHPCC Singapore Workshop (8/22/95).
* LAST REVISED: 04/13/05
******************************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LIMIT     100000000     /* Increase this to find more primes */
#define FIRST     0             /* Rank of first task */


//TO-DO: convert to using array of pre-sieved primes
int isprime(int n) {
    int i, squareroot;
    if (n>10) {
        squareroot = (int) sqrt(n);
        for (i=3; i<=squareroot; i+=2)
           if ((n%i)==0)
              return 0;
        return 1;
    }
    /* Assume first four primes are counted elsewhere. Forget everything else */
    else
        return 0;
}


int main (int argc, char *argv[])
{
    int   ntasks,               /* total number of tasks in partitiion */
          rank,                 /* task identifier */
          n,                    /* loop variable */
          pc,                   /* prime counter */
          pcsum,                /* number of primes found by all tasks */
          foundone,             /* most recent prime found */
          maxprime,             /* largest prime found */
          mystart,              /* where to start calculating */
          stride;               /* calculate every nth number */
    
    double start_time,end_time;
    	
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    if (((ntasks%2) !=0) || ((LIMIT%ntasks) !=0)) {
        printf("Sorry - this exercise requires an even number of tasks ");
        printf("evenly divisible into %d.  Try 4 or 8.\n", LIMIT);
        MPI_Finalize();
        exit(0);
    }
    
    start_time = MPI_Wtime();   /* Initialize start time */
    // mystart = (rank*2)+1;       /* Find my starting point - must be odd number */
    // stride = ntasks*2;          /* Determine stride, skipping even numbers */
    stride = LIMIT/ntasks;      /* Determine stride (even number), divide LIMIT into (ntasks) intervals */
	mystart = 9;				/* start FIRST rank from this prime */
    pc=0;                       /* Initialize prime counter */
    foundone = 0;               /* Initialize */	
    
    /******************** task with rank 0 does this part ********************/
    if (rank == FIRST) {
        printf("Using %d tasks to scan %d numbers\n", ntasks, LIMIT);
        pc = 4;                  /* Assume first four primes are counted here */
        for (n=mystart; n<=stride; n+=2) {
            if (isprime(n)) {
                pc++;
                foundone = n;
                /***** Optional: print each prime as it is found
                printf("%d\n", foundone);
                *****/
            }
        }
        MPI_Reduce(&pc, &pcsum, 1, MPI_INT, MPI_SUM, FIRST, MPI_COMM_WORLD);
        MPI_Reduce(&foundone, &maxprime, 1, MPI_INT, MPI_MAX, FIRST, MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        
		printf("rank: %d\n", rank);
		printf("Done. Largest prime is %d\nTotal primes %d\n", maxprime, pcsum);
        printf("Wallclock time elapsed: %.2lf seconds\n", end_time-start_time);
    }
    
    
    /******************** all other tasks do this part ***********************/
    if (rank > FIRST) {
		printf("rank: %d\n", rank);
        for (n=rank*stride+1; n<=(rank+1)*stride; n+=2) {
           if (isprime(n)) {
                pc++;
                foundone = n;
                /***** Optional: print each prime as it is found
                printf("%d\n", foundone);
                *****/
           }
       }
       MPI_Reduce(&pc, &pcsum, 1, MPI_INT, MPI_SUM, FIRST, MPI_COMM_WORLD);
       MPI_Reduce(&foundone, &maxprime, 1, MPI_INT, MPI_MAX, FIRST, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
}

/*
13/08/2017 22:32:07
C:\Users\rhammond\Google Drive\source_code\prime_progs>mpiexec -n 4 mpi_prime_v2.exe
rank: 2
rank: 3
Using 4 tasks to scan 1000000000 numbers
Done. Largest prime is 999999937
Total primes 50847534
Wallclock time elapsed: 1656.38 seconds
rank: 1
*/