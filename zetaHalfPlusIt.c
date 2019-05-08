/***************************************************************************
*                                                                          *
* Program to implement the Riemann-Siegel formula to compute               *
* approximations to zeta(1/2+it) for t real. This code does not work       *
* reliably for t<10 though.                                                *
*                                                                          *
* Given the input LOWER, UPPER and NUMSAMPLES, the program computes        *
* NUMSAMPLES coordinate pairs (t,zeta(1/2+it)) for LOWER <= t <= UPPER     *
* and writes the results to the standard output.                           *
*                                                                          *
* Five of the error terms of the approximation are used for all            *
* calculations.                                                            *
*                                                                          *
* on RedHat Linux compile with                                             *
*                                                                          *
*        gcc -lm -o zetaHalfPlusIt zetaHalfPlusIt.c                        *
*                                                                          *
* and run with                                                             *
*                                                                          *
*        ./zetaHalfPlusIt                                                  *
*                                                                          *
***************************************************************************/

//        gcc.exe -lm -std=c99 -Wall -o zetaHalfPlusIt.exe zetaHalfPlusIt.c -D__USE_MINGW_ANSI_STDIO

 
#include <stdio.h>
#include <math.h>

#define LOWER         14.134L     /* lower bound of t domain              */
#define UPPER         14.135L     /* upper bound of t domain              */
#define NUMSAMPLES         100       /* number of samples to compute         */

/**************************************************************************/

int main()
{
  long double t1   ;              /* start of plotted interval            */
  long double t2   ;              /* end of plotted interval              */
  long double t    ;              /* sample to compute                    */
  long double rezeta;             /* real part of zeta(1/2+it)           */
  long double imzeta;             /* imaginary part of zeta(1/2+it)      */

  long double Z(long double,int); /* Riemann-Siegel Z(t) function         */
  long double theta(long double); /* theta function                       */


  int samples    ;                /* number samples to compute            */
  int sampleindex;                /* index of sample being computed       */ 

  t1          = LOWER         ;       
  t2          = UPPER         ;           
  samples     = NUMSAMPLES - 1;
  sampleindex = 0             ;
  
  while (sampleindex <= samples)
    { 

      t = t1 + (long double)sampleindex/samples*(t2 - t1); 
      rezeta = Z(t,4)*cos(theta(t));
      imzeta = -1.0*Z(t,4)*sin(theta(t));

      printf("t = %16.12Lf\t zeta(1/2+it) = %16.12Lf + i %16.12Lf\n", t, rezeta, imzeta); 
 
      ++sampleindex;

    }

  return(0);

}


long double Z(long double t, int n)
/***************************************************************************
*                                                                          *
* The Z(t) function from the Riemann-Siegel formula. This functions takes  *
* an additional integer argument which is the number of terms to use in    *
* the remainder. This integer argument can vary from 0 to 4 which          *
* corresponds to the first through fifth remainder terms.                  *
*                                                                          *
***************************************************************************/
{

  long double ZZ;                 /* the result                           */
  long double p;                  /* fractional part of sqrt(t/(2.0*pi))  */
  long double theta(long double); /* theta function                       */
  long double C(int,long double); /* coefficient of (2*pi/t)^(k*0.5)      */
  long double R;                  /* remainder term                       */
  int even(int);                  /* -1,+1 parity function                */
  int j;                          /* summation index for Z(t) function    */
  int k;                          /* summation index for remainder term   */
  int N;                          /* integer part of sqrt(t/(2.0*pi))     */

  const long double pi = 3.1415926535897932385L;  /* initializations...   */
  ZZ    =  0.0L;                                  /*                      */
  R     =  0.0L;                                  /*                      */
  j     =  1;                                     /*                      */
  k     =  0;                                     /*                      */
  N     =  sqrtl(t/(2.0L * pi));                  /*                      */
  p     =  sqrtl(t/(2.0L * pi)) - N;              /*                      */
       
  while (j <= N)                                  /* add up terms of      */
    {                                             /* main series...       */
      ZZ  = ZZ + 1.0L/sqrtl((long double) j )     /*                      */
               * cosl(fmodl(theta(t)              /*                      */
                      -t*logl((long double) j),   /*                      */
                      2.0L*pi));                  /*                      */
      ++j;                                        /*                      */
    }                                             /*                      */
  ZZ = 2.0L * ZZ;                                 /*                      */

  while (k <= n)                                  /* add up terms of      */
    {                                             /* remainder...         */
      R = R + C(k,2.0L*p-1.0L)                    /*                      */ 
            * powl(2.0L*pi/t,                     /*                      */
                   ((long double) k)*0.5L);       /*                      */
      ++k;                                        /*                      */
    }                                             /*                      */
  R = even(N-1) * powl(2.0L * pi / t,0.25L) * R;  /*                      */

  return(ZZ + R);

}


int even(int n)
/***************************************************************************
*                                                                          *
* function which returns -1 if argument is odd and +1 if argument is even  *
*                                                                          *
***************************************************************************/
{

  if (n%2 == 0)
    return(1);
  else
    return(-1);

}


long double theta(long double t)
/***************************************************************************
*                                                                          *
* Approximation to theta(t)=Im{log[Pi(it/2-3/4)]}-t/2*log(pi)              *
*                                                                          *
***************************************************************************/
{

  const long double pi = 3.1415926535897932385L;
  return(t/2.0L*logl(t/2.0L/pi) - t/2.0L - pi/8.0L 
         + 1.0L/48.0L/t + 7.0L/5760.0L/t/t/t);

}


long double C(int n, long double z)
/***************************************************************************
*                                                                          *
* Coefficients of remainder terms; n can range from 0 to 4.                *
*                                                                          *
***************************************************************************/
{

  if (n==0)
    return(.38268343236508977173L * powl(z, 0.0L) 
          +.43724046807752044936L * powl(z, 2.0L)
          +.13237657548034352332L * powl(z, 4.0L)
          -.01360502604767418865L * powl(z, 6.0L)
          -.01356762197010358089L * powl(z, 8.0L)
          -.00162372532314446528L * powl(z,10.0L)
          +.00029705353733379691L * powl(z,12.0L)
          +.00007943300879521470L * powl(z,14.0L)
          +.00000046556124614505L * powl(z,16.0L)
          -.00000143272516309551L * powl(z,18.0L)
          -.00000010354847112313L * powl(z,20.0L)
          +.00000001235792708386L * powl(z,22.0L)
          +.00000000178810838580L * powl(z,24.0L)
          -.00000000003391414390L * powl(z,26.0L)
          -.00000000001632663390L * powl(z,28.0L)
          -.00000000000037851093L * powl(z,30.0L)
          +.00000000000009327423L * powl(z,32.0L)
          +.00000000000000522184L * powl(z,34.0L)
          -.00000000000000033507L * powl(z,36.0L)
          -.00000000000000003412L * powl(z,38.0L)
          +.00000000000000000058L * powl(z,40.0L)
          +.00000000000000000015L * powl(z,42.0L));
  else if (n==1)
    return(-.02682510262837534703L * powl(z, 1.0L) 
           +.01378477342635185305L * powl(z, 3.0L)
           +.03849125048223508223L * powl(z, 5.0L)
           +.00987106629906207647L * powl(z, 7.0L)
           -.00331075976085840433L * powl(z, 9.0L)
           -.00146478085779541508L * powl(z,11.0L)
           -.00001320794062487696L * powl(z,13.0L)
           +.00005922748701847141L * powl(z,15.0L)
           +.00000598024258537345L * powl(z,17.0L)
           -.00000096413224561698L * powl(z,19.0L)
           -.00000018334733722714L * powl(z,21.0L)
           +.00000000446708756272L * powl(z,23.0L)
           +.00000000270963508218L * powl(z,25.0L)
           +.00000000007785288654L * powl(z,27.0L)
           -.00000000002343762601L * powl(z,29.0L)
           -.00000000000158301728L * powl(z,31.0L)
           +.00000000000012119942L * powl(z,33.0L)
           +.00000000000001458378L * powl(z,35.0L)
           -.00000000000000028786L * powl(z,37.0L)
           -.00000000000000008663L * powl(z,39.0L)
           -.00000000000000000084L * powl(z,41.0L)
           +.00000000000000000036L * powl(z,43.0L)
           +.00000000000000000001L * powl(z,45.0L));
  else if (n==2)
    return(+.00518854283029316849L * powl(z, 0.0L)
           +.00030946583880634746L * powl(z, 2.0L)
           -.01133594107822937338L * powl(z, 4.0L)
           +.00223304574195814477L * powl(z, 6.0L)
           +.00519663740886233021L * powl(z, 8.0L)
           +.00034399144076208337L * powl(z,10.0L) 
           -.00059106484274705828L * powl(z,12.0L) 
           -.00010229972547935857L * powl(z,14.0L) 
           +.00002088839221699276L * powl(z,16.0L) 
           +.00000592766549309654L * powl(z,18.0L) 
           -.00000016423838362436L * powl(z,20.0L)
           -.00000015161199700941L * powl(z,22.0L)
           -.00000000590780369821L * powl(z,24.0L)
           +.00000000209115148595L * powl(z,26.0L)
           +.00000000017815649583L * powl(z,28.0L)
           -.00000000001616407246L * powl(z,30.0L)
           -.00000000000238069625L * powl(z,32.0L)
           +.00000000000005398265L * powl(z,34.0L)
           +.00000000000001975014L * powl(z,36.0L)
           +.00000000000000023333L * powl(z,38.0L)
           -.00000000000000011188L * powl(z,40.0L)
           -.00000000000000000416L * powl(z,42.0L)
           +.00000000000000000044L * powl(z,44.0L)
           +.00000000000000000003L * powl(z,46.0L));
  else if (n==3) 
    return(-.00133971609071945690L * powl(z, 1.0L)
           +.00374421513637939370L * powl(z, 3.0L)
           -.00133031789193214681L * powl(z, 5.0L)
           -.00226546607654717871L * powl(z, 7.0L)
           +.00095484999985067304L * powl(z, 9.0L)
           +.00060100384589636039L * powl(z,11.0L)
           -.00010128858286776622L * powl(z,13.0L)
           -.00006865733449299826L * powl(z,15.0L)
           +.00000059853667915386L * powl(z,17.0L)
           +.00000333165985123995L * powl(z,19.0L)
           +.00000021919289102435L * powl(z,21.0L)
           -.00000007890884245681L * powl(z,23.0L)
           -.00000000941468508130L * powl(z,25.0L)
           +.00000000095701162109L * powl(z,27.0L)
           +.00000000018763137453L * powl(z,29.0L)
           -.00000000000443783768L * powl(z,31.0L)
           -.00000000000224267385L * powl(z,33.0L)
           -.00000000000003627687L * powl(z,35.0L)
           +.00000000000001763981L * powl(z,37.0L)
           +.00000000000000079608L * powl(z,39.0L)
           -.00000000000000009420L * powl(z,41.0L)
           -.00000000000000000713L * powl(z,43.0L)
           +.00000000000000000033L * powl(z,45.0L)
           +.00000000000000000004L * powl(z,47.0L));
  else
    return(+.00046483389361763382L * powl(z, 0.0L)
           -.00100566073653404708L * powl(z, 2.0L)
           +.00024044856573725793L * powl(z, 4.0L)
           +.00102830861497023219L * powl(z, 6.0L)
           -.00076578610717556442L * powl(z, 8.0L)
           -.00020365286803084818L * powl(z,10.0L)
           +.00023212290491068728L * powl(z,12.0L)
           +.00003260214424386520L * powl(z,14.0L)
           -.00002557906251794953L * powl(z,16.0L)
           -.00000410746443891574L * powl(z,18.0L)
           +.00000117811136403713L * powl(z,20.0L)
           +.00000024456561422485L * powl(z,22.0L)
           -.00000002391582476734L * powl(z,24.0L)
           -.00000000750521420704L * powl(z,26.0L)
           +.00000000013312279416L * powl(z,28.0L)
           +.00000000013440626754L * powl(z,30.0L)
           +.00000000000351377004L * powl(z,32.0L)
           -.00000000000151915445L * powl(z,34.0L)
           -.00000000000008915418L * powl(z,36.0L)
           +.00000000000001119589L * powl(z,38.0L)
           +.00000000000000105160L * powl(z,40.0L)
           -.00000000000000005179L * powl(z,42.0L)
           -.00000000000000000807L * powl(z,44.0L)
           +.00000000000000000011L * powl(z,46.0L)
           +.00000000000000000004L * powl(z,48.0L));    

}

/**************************************************************************/
