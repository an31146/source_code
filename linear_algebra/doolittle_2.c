#include <stdio.h>
#include <stdlib.h>

int Doolittle_LU_Decomposition_with_Pivoting(double *A, int pivot[], int n);
int Doolittle_LU_with_Pivoting_Solve(double *LU, double *B, int pivot[],
                                     double *x,  int n);

#define N 4

    //double A[N][N] = { {2, 1, -1}, {-3, -1, 2}, {-2, 1, 2} };
    //double B[N] = {8.0, -11.0, -3.0};
//double A[N][N] = { {2, 0, 0, 1}, {1, 0, 1, 0}, {3, 0, 1, 1}, {1, 1, 0, 0} };

int main(int argc, char *argv[])
{
    double A[N][N] = { {2, 1, -1, 1}, {-3, -1, 2, 2}, {-2, 1, 2, -3}, {1, 2, 1, -1} };
    double B[N] = {-1.0, 2.0, -1.0, 1.0};
    double x[N];
    int i, j;
    int p[N];

    Doolittle_LU_Decomposition_with_Pivoting(&A[0][0], &p[0], N);
    Doolittle_LU_with_Pivoting_Solve(&A[0][0], &B[0], &p[0], &x[0], N);
        
    printf("\n---------------------------------\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
            printf("%4.1f ", A[i][j]);
        printf("\n");
    }
    printf("---------------------------------\n");
    
    for (i=0; i<N; i++)
        printf("%4.1f ", x[i]);
    printf("\n");
    
    return 0;
}
