//
//
// gcc -O2 -Wall -std=c99 -o mpi_prime_v4 mpi_prime_v4.c -lmpi
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
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/time.h>

#define LIMIT     1000000000    /* Increase this to find more primes */
#define FIRST     0            /* Rank of first task */
// prime sieve size
#define MAX_PRIME 65521


//TO-DO: convert to using array of pre-sieved primes
int isprime_old(int n) {
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


static uint16_t *_primes;
static int size_primes;

int isprime(int number)
{
    int i, pr, sqrt_N = sqrt(number);
    
    for (i=0; pr=_primes[i], i<size_primes && pr<=sqrt_N; i++)
    {
        if (number == pr)
            return 1;
        if (number % pr == 0)
             return 0;
    }
    // test to see if exhausted array of trivial primes, before reaching the square root 
    if (pr < sqrt_N)
    {
        for (i=pr; i<=sqrt_N; i+=2)
            if (number % i == 0)
                return 0;
    }
    // finally...
    return 1;
}


int sieve(int max_int)
{
    int i, j;

    _primes = (uint16_t *)calloc(max_int+1, sizeof(uint16_t));
    _primes[0] = 2; // 2 is always the 0th prime

    for (i=2; i<sqrt(max_int); i++)
    {
        for (j=i*i; j<=max_int; j+=i)
            _primes[j] = 1;
    }
    
    int k = 1;
    for (j=3; j<=max_int; j++)
        if (_primes[j]==0)
        {
            _primes[k++] = j;
        }

    return k;   // return number of primes found
}


int init_sieve()
{
    // Initialize the sieve of trivial primes
    long t0, t1;
    struct timeval start, end;
    double diff;
    uint64_t sieve_time;

    t0 = clock();
    gettimeofday(&start, NULL);

    size_primes = sieve(MAX_PRIME);

    t1 = clock();
    gettimeofday(&end, NULL);
    sieve_time = t1 - t0;

    printf("\n_primes[%" PRIu32 "] = %" PRIu32 "\n",
    //printf("\n_primes[%" PRIu32 "] = %" PRIu32 "\nTime: %" PRIu64 " µs\n", 
            size_primes-1, _primes[size_primes-1], sieve_time);
    diff = ((end.tv_sec * 1000000 + end.tv_usec)
          - (start.tv_sec * 1000000 + start.tv_usec))/1000.0;
    printf("      %.3lf ms.\n\n", diff);
    
    return size_primes;
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
    
    double start_time, end_time;
        
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    if (((ntasks%2) !=0) || ((LIMIT%ntasks) !=0))
    {
        printf("Sorry - this exercise requires an even number of tasks ");
        printf("evenly divisible into %d.  Try 4 or 8.\n", LIMIT);
        MPI_Finalize();
        exit(0);
    }

    start_time = MPI_Wtime();   // Initialize start time
    // mystart = (rank*2)+1;       // Find my starting point - must be odd number
    // stride = ntasks*2;          // Determine stride, skipping even numbers
    stride = LIMIT/ntasks;      // Determine stride (even number), divide LIMIT into (ntasks) intervals
    mystart = 11;               // start FIRST rank from this prime
    pc = 0;                     // Initialize prime counter
    foundone = 0;               // Initialize 
    
    
    __attribute__((unused)) int num_primes = init_sieve();

    /******************** task with rank 0 does this part ********************/
    if (rank == FIRST)
    {
        
        printf("Using %d tasks to scan %d numbers\n", ntasks, LIMIT);
        pc = 4;                  /* Assume first four primes are counted here */
        for (n=mystart; n<=stride; n+=2) {
            if (isprime(n)) {
                pc++;
                foundone = n;
                /***** Optional: print each prime as it is found
                printf("%10d\n", foundone);
                *****/
            }
        }
        MPI_Reduce(&pc, &pcsum, 1, MPI_INT, MPI_SUM, FIRST, MPI_COMM_WORLD);
        MPI_Reduce(&foundone, &maxprime, 1, MPI_INT, MPI_MAX, FIRST, MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        
        printf("rank: %d\n\n", rank);
        printf("Done. Largest prime is %d\nTotal primes %d\n", maxprime, pcsum);
        printf("Wallclock time elapsed: %.2lf seconds\n", end_time-start_time);
        free(_primes);
    }
/*
    else
    {
        // allocate memory for _primes array
        __attribute__((unused)) unsigned int num_primes = init_sieve();
        //_primes = (uint32_t *)calloc(MAX_PRIME, sizeof(uint32_t));
    }

    
    MPI_Datatype my_mpi_array;
    int          blocklens[1];
    MPI_Aint     indices[1];
    MPI_Datatype old_types[1];
    
    // One value of each type - size of the array
    blocklens[0] = size_primes;
    // The base types
    old_types[0] = MPI_UNSIGNED;
    // The locations of each element - only one pointer
    indices[0] = 0;
*/
/*  int MPI_Type_create_struct(int count,
                               const int array_of_blocklengths[],
                               const MPI_Aint array_of_displacements[],
                               const MPI_Datatype array_of_types[],
                               MPI_Datatype *newtype)
*/
/*
    MPI_Type_create_struct( 1, blocklens, indices, old_types, &my_mpi_array );
    MPI_Type_commit( &my_mpi_array );
    
    MPI_Bcast( _primes, 1, my_mpi_array, FIRST, MPI_COMM_WORLD );
*/
    
    /******************** all other tasks do this part ***********************/
    if (rank > FIRST) {
        printf("rank: %d\n", rank);
        for (n=rank*stride+1; n<=(rank+1)*stride; n+=2) {
            if (isprime(n)) {
                pc++;
                foundone = n;
                /***** Optional: print each prime as it is found
                printf("%10d\n", foundone);
                *****/
            }
        }
        MPI_Reduce(&pc, &pcsum, 1, MPI_INT, MPI_SUM, FIRST, MPI_COMM_WORLD);
        MPI_Reduce(&foundone, &maxprime, 1, MPI_INT, MPI_MAX, FIRST, MPI_COMM_WORLD);
        free(_primes);
    }
    
    // Dealloc _primes array
    // Clean up the type
    //MPI_Type_free( &my_mpi_array );
    MPI_Finalize();
}
