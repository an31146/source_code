// gcc -O2 -Wall -std=c99 -o openmp_for_loop openmp_for_loop.c -fopenmp -lgomp
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N       1000

int main (int argc, char *argv[]) 
{

    int nthreads, tid, i;
    float a[N], b[N], c[N];
    
    /* Some initializations */
    for (i=0; i<N; i++)
    {
        a[i] = b[i] = i*i;
        c[i] = 0;
    }
    
    
    #pragma omp parallel shared(a,b,c,nthreads) private(i,tid)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("\033[2JNumber of threads = %d\n", nthreads);
        }
    
        printf("Thread %d starting...\n\n",tid);
        
        for (int j=0; j<N; j++)
        #pragma omp parallel for 
        for (i=0; i<N; i++)
        {
            c[i] += a[i] + b[i];

            if (tid == 0)
                printf("\033[7;0HThread %d: c[%4d]= %.1f\r", tid, i, c[i]);
            else
                printf("\033[8;0HThread %d: c[%4d]= %.1f\r", tid, i, c[i]);
        }
    
        printf("\n\n");
    }  /* end of parallel section */

}
