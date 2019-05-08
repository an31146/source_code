#include <stdio.h>
#include <stdlib.h>

#define SIZE 6

int matrix[SIZE][SIZE*2] = { {1,1,0,0,1,1,1,0,0,0,0,0},
                             {0,1,0,0,1,1,0,1,0,0,0,0},
                             {1,0,1,0,0,0,0,0,1,0,0,0},
                             {0,0,1,0,0,0,0,0,0,1,0,0},
                             {0,0,0,0,0,0,0,0,0,0,1,0},
                             {0,0,0,1,0,1,0,0,0,0,0,1} };

/*                             
int matrix[SIZE][SIZE*2] = { {1,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0},
                             {0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0},
                             {1,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0},
                             {0,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0},
                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
                             {1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
                             {0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
                             {0,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0},
                             {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                             {1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1} };
*/

void gauss()
{
    for (int p=0; p<SIZE; p++)
    {
        // find pivot row and swap 
        for (int i=p+1; i<SIZE; i++)
        {
            if (matrix[i][p] > matrix[p][p]) 
            {
                printf("Swap rows: %d and %d\n", p, i);

                for (int j=0; j<SIZE*2; j++)
                {
                    int t = matrix[i][j];
                    matrix[i][j] = matrix[p][j];
                    matrix[p][j] = t;
                }
            }
        
            if (matrix[i][p])
            {
                printf("Add row: %d to row: %d\n", p, i);
                for (int j=0; j<SIZE*2; j++)
                {
                    matrix[i][j] ^= matrix[p][j];
                }
            }            
        } // for i
    } // for p
}

void dump()
{
    for (int i=0; i<SIZE; i++)
    {
        printf("%2d:", i);
        for (int j=0; j<SIZE*2; j++)
            printf("%2d", matrix[i][j]);
        printf("\n");
    }
    for (int i=0; i<SIZE*2; i++)
        printf("--");
    printf("---\n");
}

int main(int argc, char *argv[])
{                       
    dump();
    gauss();
    dump();
    
    return 0;
}
