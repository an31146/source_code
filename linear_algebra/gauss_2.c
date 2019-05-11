#include <stdio.h>
#include <stdlib.h>

#define M 4
#define N 5

//double A[M][N] = { {2, 1, -1, 1, -1}, {-3, -1, 2, 2, 2}, {-2, 1, 2, -3, -1}, {1, 2, 1, -1, 1} }; 
//double A[M][N] = { {1, 1, 0, 0, 1, 0, 0, 0}, {1, 0, 0, 1, 0, 1, 0, 0}, {0, 1, 0, 0, 0, 0, 1, 0}, {0, 0, 0, 1, 0, 0, 0, 1} };
//double A[M][N] = { {2, 1, -1}, {-3, -1, 2}, {-2, 1, 2} };
double A[M][N] = { {2, 1, 0, 1, 1}, {1, 4, 1, 0, -1}, {3, 0, 1, 1, 0}, {1, 1, -2, 2, 0} };

int Gaussian_Elimination_Aux(double *A, int nrows, int ncols);

int main(int argc, char *argv[])
{
    for (int i=0; i<M; i++)
    {
        for (int j=0; j<N; j++)
            printf("%6.1f", A[i][j]);
        printf("\n");
    }

    int res = Gaussian_Elimination_Aux(&A[0][0], M, N);
    
    if (res == -1)
    {
        printf("Matrix is singular.\n");
        return res;
    }
        
    printf("---------------------------------\n");
    for (int i=0; i<M; i++)
    {
        for (int j=0; j<N; j++)
        {
            printf("%6.1f", A[i][j]);
        }
        printf("\n");
    }
    printf("---------------------------------\n");

    printf("\nPress Enter: ");
    getchar();
        
    return 0;
}
