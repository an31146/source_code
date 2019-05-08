#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 4
//double A[N][N] = { {2, 1, -1, 1}, {-3, -1, 2, 2}, {-2, 1, 2, -3}, {1, 2, 1, -1} };
//double A[N][N] = { {2, 0, 0, 1}, {1, 0, 1, 0}, {3, 0, 1, 1}, {1, 1, 0, 0} };
double A[N][N];

int Gaussian_Elimination(double *A, int n, double *B);

double rnd()
{
    return(rand() / (double)RAND_MAX);
}

void init_matrix(double M[N][N])
{
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++)
            M[i][j] = floor(rnd()*10.0d + 1);
}

int main(int argc, char *argv[])
{
    double x[N] = {-1.0, 2.0, -1.0, 1.0};
    time_t seed = time(NULL);

    srand((uintmax_t)seed);
    init_matrix(A);

    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
            printf("%6.2f ", A[i][j]);
        printf("\n");
    }

    Gaussian_Elimination(&A[0][0], N, &x[0]);
        
    printf("\n---------------------------------\n");
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
            printf("%6.2f ", A[i][j]);
        printf("\n");
    }
    printf("---------------------------------\n");
    
    for (int i=0; i<N; i++)
        printf("%6.2f ", x[i]);
    printf("\n\nPress Enter: ");
    getchar();
    
    return 0;
}
