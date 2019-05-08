#include <stdio.h>
#include <stdlib.h>

#define N 4
double A[N][N] = { {2, 1, -1, 1}, {-3, -1, 2, 2}, {-2, 1, 2, -3}, {1, 2, 1, -1} };
//double A[N][N] = { {2, 1, -1}, {-3, -1, 2}, {-2, 1, 2} };
//double A[N][N] = { {2, 0, 0, 1}, {1, 0, 1, 0}, {3, 0, 1, 1}, {1, 1, 0, 0} };

int main(int argc, char *argv[])
{
    //double B[N] = {8.0, -11.0, -3.0};
    double B[N] = {-1.0, 2.0, -1.0, 1.0};
    double x[N];
    int i, j;
    int p[N];

    Doolittle_LU_Decomposition(&A[0][0], N);
    Doolittle_LU_Solve(&A[0][0], &B[0], &x[0], N);
    //Doolittle_LU_Decomposition_with_Pivoting(&A[0][0], &p[0], N);
    //Doolittle_LU_with_Pivoting_Solve(&A[0][0], &B[0], &p[0], &x[0], N);
        
    printf("\n---------------------------------\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
            printf("%.1f ", A[i][j]);
        printf("\n");
    }
    printf("---------------------------------\n");
    
    for (i=0; i<N; i++)
        printf("%.1f ", x[i]);
    printf("\n");
    
    return 0;
}
