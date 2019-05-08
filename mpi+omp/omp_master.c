// omp_master.cpp
// compile with: /openmp 
#include <omp.h>
#include <stdio.h>

void MulSeq(int size, int* m1, int* m2, int* result)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            int tmp = 0;
            for (int k = 0; k < size; k++)
            {
                tmp += m1[i, k] * m2[k, j];
            }
            result[i, j] = tmp;
        }
}

int main( ) 
{
    int a[5], i;

    #pragma omp parallel
    {
        // Perform some computation.
        #pragma omp for
        for (i = 0; i < 5; i++)
            a[i] = i * i;

        // Print intermediate results.
        #pragma omp master
            for (i = 0; i < 5; i++)
                printf("a[%d] = %d\n", i, a[i]);
      
        // Wait.
        #pragma omp barrier

        // Continue with the computation.
        #pragma omp for
        for (i = 0; i < 5; i++)
            a[i] += i;

        // Print final results.
        #pragma omp master
        {
            printf("\n");
        
            for (i = 0; i < 5; i++)
                printf("a[%d] = %d\n", i, a[i]);
        }
    }
}
