/* 
    Calculate pi using Monte-Carlo method
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    double r1, r2, pi;
    double m, n;
    long niter;
    
    m = n = 0;
    
    printf("Enter the number of iterations used to estimate pi: ");
    scanf("%d",&niter);
    
    srand(SEED);

    while (1)
    {
        #pragma omp parallel for reduction(+:m,n) private(r1,r2)
        for (long i=0; i<niter; i++)
        {
            m++;
            r1 = (double)rand() / 16384.0 - 1.0;
            r2 = (double)rand() / 16384.0 - 1.0;
            
            if ( (r1*r1+r2*r2) < 1.0 )
                n++;
        }
        pi = 4.0 * n / m;
        printf("%.10f\r", pi);
    }
    
    return 0;
}
