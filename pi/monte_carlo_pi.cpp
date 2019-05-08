/* 
    Calculate pi using Monte-Carlo method
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SEED 0xdeadbeef

int main(void)
{
    double r1, r2, pi;
    long m, n;
    long niter;
    
    m = n = 0;
    
    printf("Enter the number of iterations used to estimate pi: ");
    scanf("%lu", &niter);
    
    srand(SEED);

    while (1)
    {
        #pragma omp parallel for reduction(+:m,n) private(r1,r2)
        for (long i=0; i<niter; i++)
        {
            m++;
            r1 = (double)rand() / RAND_MAX;
            r2 = (double)rand() / RAND_MAX;
            
            if ( (r1*r1+r2*r2) < 1.0 )
                n++;
        }
        pi = 4.0f * (double)n / (double)m;

        printf("%.10f\t%.10f\t%8lu\t%8lu\t%.10f\r", r1, r2, m, n, pi);
    }
    
    return 0;
}
