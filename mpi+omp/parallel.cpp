// g++ -O2 -Wall -std=c++11 -o parallel.exe parallel.cpp -fopenmp
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main(void)
{
    char s[1];
    
    omp_set_num_threads(8);
    omp_set_nested(0);
    
    #pragma omp parallel
    {   
        int tid = omp_get_thread_num();
        
        if (tid == 0)
        {
            int nthreads = omp_get_num_threads();
            int nested = omp_get_nested();
            // should output values as set above
            printf("Number of threads = %d\n", nthreads);
            printf("Nested parallelism = %d\n\n", nested);
        }
        
        // Code inside this region runs in parallel.
        printf("Hello! [tid=%d]\n", tid);
        int start=clock();
    
        //long n=0;
        #pragma omp parallel for
        for(long n=0; n<100000000L; ++n)
        {
            char t[20];
            strcpy(t, "01234567890abcdefghijkl");
            long x=n*n%1000000000L;
            double y=sqrt((double)x);
            double f=sin(log(y));
            f=f*1.0000000001d;

            //Sleep(10);
            if (tid == 0) ;
                //printf(" %10ld\t%.5f\r", x, f);
        }
        int stop=clock();
        printf("Exited [tid=%d] - %d ms\n", tid, stop-start);

    }
    #pragma omp barrier    
    printf("\n\nPress Enter: ");
    scanf("%c",s);
    
    return 0;
}
