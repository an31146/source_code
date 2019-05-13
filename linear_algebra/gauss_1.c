#include <stdio.h>
#include <stdlib.h>

#define N 4
double A[N][N] = { {2, 1, -1, 1}, {-3, -1, 2, 2}, {-2, 1, 2, -3}, {1, 2, 1, -1} };
//double A[N][N] = { {2, 1, -1}, {-3, -1, 2}, {-2, 1, 2} };
//double A[N][N] = { {2, 0, 0, 1}, {1, 0, 1, 0}, {3, 0, 1, 1}, {1, 1, 0, 0} };

int Gaussian_Elimination(double *A, int n, double *B);

int main(int argc, char *argv[])
{
    //double B[N] = {8.0, -11.0, -3.0};
    double x[N] = {-1.0, 2.0, -1.0, 1.0};
    int i, j;

    for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
            printf("%5.1f ", A[i][j]);
        printf("\n");
    }

    Gaussian_Elimination(&A[0][0], N, &x[0]);
        
    printf("\n---------------------------------\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
            printf("%5.1f ", A[i][j]);
        printf("\n");
    }
    printf("---------------------------------\n");
    
    for (i=0; i<N; i++)
        printf("%5.1f ", x[i]);
    printf("\n\nPress Enter: ");
    getchar();
    
    return 0;
}
