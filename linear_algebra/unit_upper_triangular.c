////////////////////////////////////////////////////////////////////////////////
// File: unit_upper_triangular.c                                              //
// Routines:                                                                  //
//    Unit_Upper_Triangular_Solve                                             //
//    Unit_Upper_Triangular_Inverse                                           //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  int Unit_Upper_Triangular_Solve(double *U, double *B, double x[], int n)  //
//                                                                            //
//  Description:                                                              //
//     This routine solves the linear equation Ux = B, where U is an n x n    //
//     unit upper triangular matrix.  (Only the superdiagonal part of the     //
//     matrix is addressed.)  The diagonal is assumed to consist of 1's and   //
//     is not addressed.                                                      //
//     The algorithm follows:                                                 //
//                  x[n-1] = B[n-1], and                                      //
//       x[i] = B[i] - (U[i][i+1] * x[i+1]  + ... + U[i][n-1] * x[n-1]),      //
//     for i = n-2, ..., 0.                                                   //
//                                                                            //
//  Arguments:                                                                //
//     double *U   Pointer to the first element of the upper triangular       //
//                 matrix.                                                    //
//     double *B   Pointer to the column vector, (n x 1) matrix, B.           //
//     double *x   Pointer to the column vector, (n x 1) matrix, x.           //
//     int     n   The number of rows or columns of the matrix U.             //
//                                                                            //
//  Return Values:                                                            //
//     void                                                                   //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double A[N][N], B[N], x[N];                                            //
//                                                                            //
//     (your code to create matrix A and column vector B)                     //
//     Unit_Upper_Triangular_Solve(&A[0][0], B, x, n);                        //
//     printf(" The solution is \n");                                         //
//           ...                                                              //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
void Unit_Upper_Triangular_Solve(double *U, double B[], double x[], int n)
{
   int i, k;

//         Solve the linear equation Ux = B for x, where U is an upper
//         triangular matrix.                                      
   x[n-1] = B[n-1]; 
   for (k = n-2, U += n * (n - 2); k >= 0; U -= n, k--) {
      x[k] = B[k];
      for (i = k + 1; i < n; i++) x[k] -= x[i] * *(U + i);
   }
}


////////////////////////////////////////////////////////////////////////////////
//  int Unit_Upper_Triangular_Inverse(double *U,  int n)                      //
//                                                                            //
//  Description:                                                              //
//     This routine calculates the inverse of the unit upper triangular matrix//
//     U.  The subdiagonal part of the matrix is not addressed.               //
//     The diagonal is assumed to consist of 1's and is not addressed.        //
//     The algorithm follows:                                                 //
//        Let M be the inverse of U, then U M = I,                            //
//          M[i][j] = -( U[i][i+1] M[i+1][j] + ... + U[i][j] M[j][j] ),       //
//     for i = n-2, ... , 0,  j = n-1, ..., i+1.                              //
//                                                                            //
//                                                                            //
//  Arguments:                                                                //
//     double *U   On input, the pointer to the first element of the matrix   //
//                 whose unit upper triangular elements form the matrix which //
//                 is to be inverted. On output, the upper triangular part is //
//                 replaced by the inverse.  The subdiagonal elements are     //
//                 not modified.                                              //
//     int     n   The number of rows and/or columns of the matrix U.         //
//                                                                            //
//  Return Values:                                                            //
//     void                                                                   //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double U[N][N];                                                        //
//                                                                            //
//     (your code to create the matrix U)                                     //
//     Unit_Upper_Triangular_Inverse(&U[0][0], N);                            //
//     printf(" The inverse is \n");                                          //
//           ...                                                              //
//     }                                                                      //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
void Unit_Upper_Triangular_Inverse(double *U, int n)
{
   int i, j, k;
   double *p_i, *p_j, *p_k;
   double sum;

//         Invert the superdiagonal part of the matrix U row by row where
//         the diagonal elements are assumed to be 1.0.

   for (i = n - 2, p_i = U + n * (n - 2); i >=0; p_i -= n, i-- ) {
      for (j = n - 1; j > i; j--) {
         *(p_i + j) = -*(p_i + j);
         for (k = i + 1, p_k = p_i + n; k < j; p_k += n, k++ ) 
            *(p_i + j) -= *(p_i + k) * *(p_k + j);
      }
   }
}
