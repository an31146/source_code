// gcc -O2 -Wall -std=c99 -o openmp_for_loop.exe openmp_for_loop.c
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10000

int main (int argc, char *argv[]) 
{

    int nthreads, tid, i;
    float a[SIZE], b[SIZE], c[SIZE];
    
    /* Some initializations */
    for (i=0; i<SIZE; i++)
    {
        a[i] = b[i] = i*0.01f;
	c[i] = 0.0f;
    }
    
    printf("");
    
    #pragma omp parallel shared(a,b,c,nthreads) private(i,tid)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
    
        printf("\033[%d;0H Thread %d starting...", tid+2, tid);
        
        #pragma omp for 
        for (i=0; i<SIZE*100; i++)
        {
            c[i%SIZE] += a[i%SIZE] + b[i%SIZE];
            printf("\033[%d;0H Thread %d: c[%6d]= %f\r", tid+10, tid, i%SIZE, c[i%SIZE]);
        }
    
    }  /* end of parallel section */
    printf("\n\n");
}
