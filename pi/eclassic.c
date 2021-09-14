/*
** Pascal Sebah : July 1999
** 
** Subject:
**
**    A very easy program to compute e with many digits.
**    No optimisations, no tricks, just a basic program to learn how
**    to compute in multiprecision.  
**
** Formula:
**
**    e = 1+1/1!+1/2!+...+1/k!+...
**
** Data:
**
**    A big real (or multiprecision real) is defined in base B as:
**      X = x(0) + x(1)/B^1 + ... + x(n-1)/B^(n-1)
**      where 0<=x(i)<B
**
** Results: (PentiumII, 450Mhz)
**    
**    1000   decimals :   0.02seconds
**    10000  decimals :   1.2s
**    100000 decimals :  97.0s
**    200000 decimals : 375.0s
**
** With a little work it's possible to reduce those computation
** times by a factor of 3 and more.
*/
#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
long B=10000; /* Working base */
long LB=4;    /* Log10(base)  */
/*
** Set the big real x to the small integer Integer 
*/
void SetToInteger (long n, long *x, long Integer) {
  long i;
  for (i=1; i<n; i++) x[i] = 0;
  x[0] = Integer;
}
/*
** Is the big real x equal to zero ?
*/
long IsZero (long n, long *x) {
  long i;
  for (i=0; i<n; i++)  
    if (x[i])	return 0;
	return 1;
}
/*
** Addition of big reals : x += y
**  Like school addition with carry management
*/
void Add (long n, long *x, long *y) {
  long carry=0, i;
  for (i=n-1; i>=0; i--) {
    x[i] += y[i]+carry;
    if (x[i]<B) carry = 0;
    else {
      carry = 1;
      x[i] -= B;
    }
  }  
}
/*
** Division of the big real x by the integer d 
**  Like school division with carry management
*/
void Div (long n, long *x, long d) {
  long carry=0, xi, q, i;
  for (i=0; i<n; i++) {
    xi    = x[i]+carry*B;
    q     = xi/d;
    carry = xi-q*d;   
    x[i]  = q;        
  }  
}
/*
** Print the big real x
*/
void Print (long n, long *x) {
  long i;
  printf ("%ld.", x[0]);
  for (i=1; i<n; i++) {
    printf ("%.4ld", x[i]);
    if (i%25==0) printf ("%8ld\n", i*4);
  }
  printf ("\n");
}
/*
** Computation of the constant e
*/
int main () {
  long NbDigits=10100, size=1+NbDigits/LB;
  long *e  = (long *)malloc(size*sizeof(long));
  long *uk = (long *)malloc(size*sizeof(long));
  long k=1;
  /*
  ** Formula used : e = 1+1/1!+1/2!+...+1/k!+...
  */ 
  SetToInteger (size, e,  1); /* e  = 1 */
  SetToInteger (size, uk, 1);	/* uk = 1 */
   while (!IsZero(size, uk)) {
    Div (size, uk, k);  /* uk = u(k-1)/k */
    Add (size, e,  uk); /* e  = e + uk */
    k++;
  }  
  Print (size, e); /* Print out of e */
  free (e);
  free (uk);
  return 0;
}



