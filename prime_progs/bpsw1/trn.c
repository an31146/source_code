/* trn.c               Dr. Thomas R. Nicely          2018.10.05.0010
 *                    http://www.trnicely.net
 *
 * Freeware copyright (c) 2018 Dr. Thomas R. Nicely
 * <http://www.trnicely.net>. Released into the public
 * domain by the author, who disclaims any legal
 * liability arising from its use. The routine iEvalExprMPZ
 * is included under the terms of the GNU GPL; see the header
 * of that routine (at the end of the code) for details.
 *
 * Common custom routines. Callable from co-compiled or linked codes.
 *
 * Header file is trn.h. It is assumed the compiler and
 * libraries have __non-trivial__ support for the long double
 * data type, and that the processor is Intel 386 compatible
 * (the macro __i386__ is defined).
 *
 * The macro __DJGPP__ is used to detect DJGPP.
 * The macro __DMC__ is used to detect 32-bit Digital Mars.
 * The macro __LINUX__ is used to detect LINUX GNU/Linux 10.0.
 * The macro __CYGWIN__ is used to detect Cygwin.
 * The macro __MINGW__ is used to detect MinGW.
 * The macro __BORLANDC__ is used to detect Borland/Inprise C.
 *   Version 5.00 or later is presumed.
 * The macro __MSVC__ is used to detect Microsoft Visual C++.
 *   Support for Microsoft Visual C++ is at alpha level.
 * The macro __WIN32__ is used to detect the Win32 API, which
 *   includes Cygwin, MinGW, Digital Mars in its default mode,
 *   and Borland C in its default mode.
 *
 * The macro __GMP__ indicates support for GMP, the GNU Multiple
 * Precision library. It is assumed to be present on all platforms
 * except Digital Mars, Borland C, and MSVC. To compile without GMP
 * on other platforms, add the command-line parameter "-D__NOGMP__"
 * (or the equivalent). If you _do_ have GMP with Digital Mars,
 * Borland C, or MSVC, activate the directive below defining
 *  __GMP__---and send me a copy of your libgmp files!
 *
 * The macro __MPFR__ indicates support for MPFR, the GNU Multiple
 * Precision Floating-point library with reliable rounding, version
 * 2.2.1 or later. It is assumed to be present on all platforms
 * except DJGPP, Digital Mars, Borland C, and MSVC. To compile
 * without MPFR on other platforms, add the command-line parameter
 * "-D__NOMPFR__" (or the equivalent). If you _do_ have MPFR with
 * DJGPP, Digital Mars, Borland C, or MSVC, activate the directive
 * below defining  __MPFR__---and send me a copy of your libmpfr
 * files!
 *
 * If support for DJGPP/Borland C style conio console functions
 * is required for compilers and platforms other than DJGPP and
 * Borland C, include conio3.h and compile and link conio3.c (q.v.).
 *
 * MinGW and Cygwin codes are targeted to run in an ordinary Windows
 * DOS box, _not_ within the MSYS/Cygwin UNIX emulation environments,
 * where the executables exhibit different behavior. Note that Cygwin
 * code compiled with the -mno-cygwin option exhibits its own
 * eccentricities, different from those of standalone Cygwin or
 * standalone MinGW.
 *
 * NOTE: The mathematical routines herein do not, in general,
 * attempt to treat all the exceptions and conditions spelled out
 * in documents such as IEEE754, IEEE854, and the C and C++
 * standards. These include detection, handling, and signalling
 * of overflow, underflow, denormals, infinities, and NaNs.
 *
 */

#if !defined(_TRN_H_)
  #include "trn.h"
#endif

/* M_EPSILON1 is the convergence tolerance in several calculations. */

#define M_EPSILON1 LDBL_EPSILON

static uint64_t seed64(void);  /* for internal use only */

/* The following external variables may be accessed from linked codes
   by means of global "extern" declarations. For the arrays, use,
   e.g., the global declaration "extern unsigned long ulPrime16[]". */

unsigned long ulDmax=0;  /* tracks global max of Lucas-Selfridge |D| */
unsigned long ulPrime16[6545];  /* array of 16-bit primes < 65538 */
int iPrime16Initialized=0;  /* Is ulPrime16 initialized? */
int iNoDivWarnings=0;  /* declare extern in the calling code and set to 1
			 to suppress warnings of non-exact
			 integer divisions */
long double ldZ[66];  /* Zeta function values zeta(2..65) */

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/* 80-bit long double floating point arithmetic routines for DJGPP,   */
/* MinGW, and Cygwin, using inline assembly (GNU-style AT&T syntax)   */
/* to compute the results in the x87 FPU.                             */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/* (1) These functions are already available in Borland C, Digital    */
/*     Mars, and SUSE 10.x Linux.                                     */
/* (2) Borland C, Digital Mars, and MSVC cannot parse the GNU-style   */
/*     AT&T assembler syntax employed.                                */
/* (3) MSVC does not support 80-bit long doubles, period, so it is    */
/*     incompatible with such functions.                              */
/* (4) An x387 or later FPU is assumed to be present.                 */
/* (5) Function identifiers have been changed, more than once, to     */
/*     resolve clashes with various compiler library functions.       */
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
long double __FABSL(long double ldArg)
{
if(ldArg >= 0)return(ldArg);
return(-ldArg);
}
/**********************************************************************/
/* The following compilers are syntactically incompatible with AT&T
   assembler syntax */
/**********************************************************************/
#if !defined(__BORLANDC__) && !defined(__DMC__) && !defined(__MSVC__)
/**********************************************************************/
long double __CEILL(long double ldArg)
{
unsigned short uhCWSave, uhCWTemp;
long double ldResult;

asm("fstcw %0" : "=m" (uhCWSave) :);
uhCWTemp=uhCWSave & 0xFBFF;
uhCWTemp=uhCWTemp | 0x0800;
asm("fldcw %0" : : "m" (uhCWTemp));
asm("frndint" : "=t" (ldResult) : "0" (ldArg));
asm("fldcw %0" : : "m" (uhCWSave));
return(ldResult);
}
/**********************************************************************/
long double __EXPL(long double ldExp)
{
long double ldExp2, ldInt, ldFrac, ldMant, ldResult;

ldExp2=M_LOG_E_BASE2*ldExp;
ldInt=__FLOORL(ldExp2);
ldFrac=ldExp2 - ldInt;
asm("f2xm1" : "=t" (ldMant) : "0" (ldFrac));
ldMant += 1.0L;
asm("fscale" : "=t" (ldResult) : "0" (ldMant), "u" (ldInt));
return(ldResult);
}
/**********************************************************************/
long double __FLOORL(long double ldArg)
{
unsigned short uhCWSave, uhCWTemp;
long double ldResult;

asm("fnstcw %0" : "=m" (uhCWSave) :);
uhCWTemp=uhCWSave & 0xF7FF;
uhCWTemp=uhCWTemp | 0x0400;
asm("fldcw %0" : : "m" (uhCWTemp));
asm("frndint" : "=t" (ldResult) : "0" (ldArg));
asm("fldcw %0" : : "m" (uhCWSave));
return(ldResult);
}
/**********************************************************************/
long double __FMODL(long double ldTop, long double ldBottom)
{
long double ldRem, ldNumerator;

if(ldBottom==0)
  {
  fprintf(stderr,
    "\n ERROR: Zero modulus passed to __FMODL.\n");
  signal(SIGFPE, SIG_DFL);
  raise(SIGFPE);
  exit(EXIT_FAILURE);
  }

ldNumerator=ldTop;
while(1)
  {
  asm("fprem" : "=t" (ldRem) : "0" (ldNumerator), "u" (ldBottom));
  if(__FABSL(ldRem) <= __FABSL(ldBottom))break;
  ldNumerator=ldRem;
  }
return(ldRem);
}
/**********************************************************************/
long double __LOGL(long double ldArg)
{
long double ldResult, ldLn_2=M_LN2;

if(ldArg <= 0)
  {
  fprintf(stderr,
    "\n ERROR: Non-positive argument passed to __LOGL.\n");
  signal(SIGFPE, SIG_DFL);
  raise(SIGFPE);
  exit(EXIT_FAILURE);
  }

asm("fyl2x" : "=t" (ldResult) : "0" (ldArg), "u" (ldLn_2) : "st(1)");
return(ldResult);
}
/**********************************************************************/
long double __LOG10L(long double ldArg)
{
long double ldResult, ldLog10_2=M_LOG_2_BASE10;

if(ldArg <= 0)
  {
  fprintf(stderr,
    "\n ERROR: Non-positive argument passed to __LOG10L.\n");
  signal(SIGFPE, SIG_DFL);
  raise(SIGFPE);
  exit(EXIT_FAILURE);
  }

asm("fyl2x" : "=t" (ldResult) : "0" (ldArg), "u" (ldLog10_2) : "st(1)");
return(ldResult);
}
/**********************************************************************/
long double __LOG2L(long double ldArg)
{
long double ldResult, ldOne=1.0L;

if(ldArg <= 0)
  {
  fprintf(stderr,
    "\n ERROR: Non-positive argument passed to __LOG2L.\n");
  signal(SIGFPE, SIG_DFL);
  raise(SIGFPE);
  exit(EXIT_FAILURE);
  }

asm("fyl2x" : "=t" (ldResult) : "0" (ldArg), "u" (ldOne) : "st(1)");
return(ldResult);
}
/**********************************************************************/
long double __POWL(long double ldBase, long double ldExp)
{
long double ld2Exp, ldInt, ldFrac, ldMant, ldResult;

if(ldBase <= 0)
  {
  fprintf(stderr,
    "\n ERROR: Non-positive base passed to __POWL.\n");
  signal(SIGFPE, SIG_DFL);
  raise(SIGFPE);
  exit(EXIT_FAILURE);
  }

/* Evaluate as 2^(ldExp*log(ldBase,2)); do exponent expression first */

asm("fyl2x" : "=t" (ld2Exp) : "0" (ldBase), "u" (ldExp) : "st(1)");

/* Separate exponent result into integer and fractional parts */

ldInt=__FLOORL(ld2Exp);
ldFrac=ld2Exp - ldInt;
asm("f2xm1" : "=t" (ldMant) : "0" (ldFrac));  /* 2^(fr part) - 1 */
ldMant += 1.0L;  /* 2^(fr part) */

/* Now multiply by 2^(integer part */

asm("fscale" : "=t" (ldResult) : "0" (ldMant), "u" (ldInt));

return(ldResult);
}
/**********************************************************************/
long double __SQRTL(long double ldArg)
{
long double ldResult;

if(ldArg < 0)
  {
  fprintf(stderr,
    "\n ERROR: Negative argument passed to __SQRTL.\n");
  signal(SIGFPE, SIG_DFL);
  raise(SIGFPE);
  exit(EXIT_FAILURE);
  }
asm("fsqrt" : "=t" (ldResult) : "0" (ldArg));
return(ldResult);
}
/**********************************************************************/
#endif  /* not Borland C and not DMC and not MSVC */
/**********************************************************************/
#ifndef __MSVC__
/**********************************************************************/
long double __NEARBYINTL(long double ldArg)
{
/* The nearbyintl function of C99 is simulated for 80-bit x87 long
   doubles. This is done for two reasons: (1) DJGPP 2.03 does not
   support nearbyintl; (2) GCC 4.14, at least as implemented in SUSE
   Linux 10.0, has a bug in nearbyintl which unpredictably interferes
   with the cast of its value to an unsigned long long.

   Thus, -4.5 and -3.5 must both be rounded to -4. No error trapping is
   included. Assuming a mantissa of 64 bits (as in the x87 hardware),
   the return will be unreliable for values of |ld| > 2^63. Be aware
   also that some implementations of strtold (also _atold, printf, etc.)
   have problems correctly converting the 19th and succeeding significant
   decimal digits (if present). */

#if defined(__BORLANDC__) || defined(__DMC__)

long double ld2, ldFrac, ldInt;

ld2=floorl(ldArg);
ldFrac=modfl(ldArg, &ldInt);
if(fabsl(ldFrac)==0.5L)  /* Round to nearest even integer */
  {
  if(fabsl(fmodl(ld2, 2)) < 0.5L)  /* Is ld2 even? */
    return(ld2);
  else
    return(ld2 + 1);
  }
return(floorl(ldArg + 0.5L));

#else

unsigned short uhCWSave, uhCWTemp;
long double ldResult;

asm("fstcw %0" : "=m" (uhCWSave) :);  /* store FPU control word */
uhCWTemp=uhCWSave & 0xF3FF;  /* clear rounding bits ==> nearest/even */
asm("fldcw %0" : : "m" (uhCWTemp));  /* load new control word */
asm("frndint" : "=t" (ldResult) : "0" (ldArg));  /* bankers' rounding */
asm("fldcw %0" : : "m" (uhCWSave));  /* restore previous control word */
return(ldResult);

#endif
}
/**********************************************************************/
long double __NEXTTOWARDL(long double ld, long double ldDirection)
{
/* Returns the rational number ld1 with the following three properties:

   (1) ld1 is representable exactly as a long double;
   (2) ld1 lies between ld and ldDirection;
   (3) No other such number lies between ld and ld1.

   If no such number can be found, ld is returned.

   Do _not_ use HUGE_VALL or similar manifest constants as ldDirection
   (they might be defined as infinities or NaNs); however, LDBL_MAX and
   -LDBL_MAX are OK.

   Under MSVC, this function will behave like nexttoward (the double
   precision version).
*/

int i, iSign;
long double eps, ld1;

if(ld==0)
  {
  if(ldDirection > 0)return(LDBL_MIN);
  if(ldDirection < 0)return(-LDBL_MIN);
  }

if(ldDirection > ld)iSign=+1;
else if(ldDirection < ld)iSign=-1;
else return(ld);

#ifdef __MSVC__
  eps=fabs(ld)*1e-17;
#else
  eps=fabsl(ld)*1e-20L;
#endif

ld1=ld;
for(i=1; i < 100; i++)
  {
  ld1 = ld + iSign*i*eps;
  if(ld1 != ld)return(ld1);
  }

return(ld);
}
/**********************************************************************/
char *szLDtoHex(char *sz, long double ld)
{
/*
 * Converts a long double to a string containing the ten-byte (Intel
 * x86/x87) hex representation. ASSUMPTIONS: short integers are 16 bits;
 * long longs are 64 bits; long doubles are 80-bit format (IEEE
 * 754 double extended), or 96 bits including 16 bits of padding.
 *
 * Returning the result as both a function argument and function value
 * may appear redundant. However, returning the result from a local
 * automatic variable is not allowed. Returning the result from a local
 * static variable is allowed, but would produce unexpected results in
 * a statement such as
 *
 * printf("\n 10==>%s   -10==>%s\n", szLDtoHex2(10), szLDtoHex2(-10));
 *
 * A single value (the result for 10) is printed twice! This will also
 * occur if the present function is used and the same target string
 * is referenced in both calls. Looks like a bug in printf to me---but
 * that's the way it works in both DJGPP 2.03 and SUSE Linux 10.0.
 *
 */

short h;
int64_t *pll, ll;

pll=(int64_t *)(&ld);
ll=*(pll);
h=*((short *)(++pll));
sprintf(sz, "0x%04hX%016" PRIX64 , h, ll);
return(sz);
}
/**********************************************************************/
#endif  /* not MSVC */
/**********************************************************************/
long double ldRand64(void)
{
/* Generates a pseudorandom long double, between zero and one (exclusive),
   with 64 bits of precision. The initializer seed64 will be invoked on the
   first call. Note that this code is not yet suitable for a multithreaded
   or parallelized environment.

   If the compiler fails to support long doubles with a 64-bit mantissa
   (e.g., Microsoft Visual C++), the return value will be only double
   precision (53 bits).

   This algorithm has not been tested for rigorous conformance to the
   standards for pseudorandom number generation. For practical
   purposes it appears to perform acceptably.

   This 64-bit linear congruential generator is based on those
   developed by the Florida State SPRNG group, and by Beck and
   Brooks at UCRL/LLNL as part of an effort to improve Monte Carlo
   simulations in radiation and neutron transport studies. See
   the following references:

o <http://nuclear.llnl.gov/CNP/rng/rngman/node4.html> (accessed
  27 April 2010).
o "The RNG Random Number Library," Bret R. Beck and Eugene D. Brooks III
  (8 December 2000), University of California, Lawrence Livermore National
  Laboratory, Livermore, California 94550, UCRL-MA-141673. See
  <http://nuclear.llnl.gov/CNP/rng/rngman/node3.html>.
o M. Mascagni, S. A. Cuccaro, D. V. Pryor and M. L. Robinson (1995),
  "A Fast, High Quality, and Reproducible Parallel Lagged-Fibonacci
  Pseudorandom Number Generator," Journal of Computational Physics
  119:211-219.
o M. Mascagni (1999), "SPRNG: A Scalable Library for Pseudorandom Number
  Generation," to appear in Proceedings of the Third International
  Conference on Monte Carlo and Quasi Monte Carlo Methods in Scientific
  Computing, J. Spanier et al., editors, Springer Verlag, Berlin. See
  <http://sprng.cs.fsu.edu/RNG/>.
*/

#define MULTIPLIER __ULL(2862933555777941757)
#define ADDEND     __ULL(3037000493)

static uint64_t ull=0;
char sz[256];

while(ull==0)ull=seed64();
ull = MULTIPLIER*ull + ADDEND;
return(ull/(1 + (long double)UINT64_MAX));

#undef MULTIPLIER
#undef ADDEND
}
/**********************************************************************/
static uint64_t seed64(void)
{
/* Generates a pseudorandom 64-bit unsigned integer to be used as the
   seed for the 64-bit pseudorandom number generator ldRand64. Not
   intended for use by any other routine except ldRand64. */

long NB, BA;
uint64_t ull, RM;

/* If available, use the time stamp counter maintained on the Pentium
   and compatible chips. */

#ifdef __DJ204__
  return(_rdtsc());
#elif defined(__WIN32__) && defined(WINVER) && (WINVER >= 0x0400)
  /* Thanks to Huang Yuanbing <bailuzhou(at)163(dot)com> for this
     code fragment, adapted from his PrimeNumber.cpp code (12 Feb 2009). */
  static LARGE_INTEGER s_freq;
  LARGE_INTEGER performanceCount;
  if(s_freq.QuadPart && QueryPerformanceFrequency(&s_freq))
    {
    QueryPerformanceCounter(&performanceCount);
    return(performanceCount.QuadPart);
    }
#elif defined(__WIN32__)  /* use GetTickCount() */
  srand(time(NULL));
  return((GetTickCount() + (uint64_t)rand())*((uint64_t)time(NULL) +
    rand()) + rand());
#endif

/* At this point neither Windows nor the Pentium time stamp is available. */

#ifdef __LINUX__  /* BSD gettimeofday is present in most Linux versions */
  static struct timeval tv;
  gettimeofday(&tv, NULL);
#endif

RM=(uint64_t)RAND_MAX;
NB=1;

while(1)  /* Compute the number of bits NB in RAND_MAX */
  {
  RM=RM/2;
  if(!RM)break;
  NB++;
  }
srand(time(NULL));
ull=rand();
BA=NB;  /* Number of bits already assigned random values */
while(BA < 64 - NB)
  {
  ull=(ull<<NB) + rand();
  BA += NB;
  }
ull=(ull << (64-BA)) + (rand() >> (NB-64+BA)) + (uint64_t)time(NULL);
#ifdef __LINUX__
  ull += tv.tv_usec;  /* enhances the period of ldRand64 */
#endif

return(ull);
}
/**********************************************************************/
long double ___strtold(const char *sz, char **ppch)
{
/* Convert a string to the equivalent long double value; specifically,
   to the long double whose value is nearer to the string value than
   any other long double. Intended to give greater and more consistent
   precision than strtold, _strold, and _atold.

   NOTES:

   (1) It is assumed that the long doubles are stored in IEEE 754
       extended double format (Intel x86/x87 ten-byte extended
       or temporary reals). Otherwise, use strtold and hope for
       the best. MSVC has no 80-bit long doubles, so simply return
       the result of strtod.
   (2) Some of the mpf's are redundant, but have been retained for
       readability.
   (3) Input strings containing syntax errors are handled by the
       default (possibly less precise) strtold or strtod functions;
       furthermore, if ppch is not NULL, its value is also set by the
       default strtold (or strtod).
   (4) The original function name __strtold was changed to ___strtold
       to avoid a linker clash in Cygwin code compiled with the
       -mno-cygwin option (to produce MinGW-like code). The linker
       complains that an intrinsic routine also named __strtold exists,
       although it appears to be dysfunctional, and I cannot find a
       prototype for it; see the "hack" comment in trn.h, where a
       #define should make the use of the old __strtold transparent---
       except, of course, when -mno-cygwin is used.
   (5) If GMP is not available, the highest precision native string-to-fp
       function available is called. Note that the DEFINEs in trn.h
       always make strtold an alias for this function, except in
       Cygwin and MSVC, which have no such function for long doubles.
       Furthermore, those DEFINEs, together with the IFNDEF __GMP__
       block below, also allow ___strtold to be called unconditionally.
   (6) MPFR is used if available.
*/

char ch, *pch;
unsigned short uh, *puh;
int i, iSign, iExp, iRet;
unsigned long ulMSL, ulLSL, *pMSL, *pLSL;
int64_t ll;
uint64_t *pull, ull;
long double ld;

static char szLocal[512];
static int iFirst=1;
#ifdef __GMP__
  static mpf_t mpf, mpf2, mpf3, mpf4, mpf5, mpfHalf;
#endif

#ifdef __MPFR__
  mpfr_t mpfr;
  mpfr_init2(mpfr, 128);
  mpfr_strtofr(mpfr, sz, ppch, 10, GMP_RNDN);
  ld=mpfr_get_ld(mpfr, GMP_RNDN);
  mpfr_clear(mpfr);
  return(ld);
#endif

if(!sz || !(*sz))return(0.0L);

/* Use the intrinsic functions to establish the default return values. */

#if defined(__CYGWIN__) || defined(__MSVC__)
  ld=(long double)strtod(sz, ppch);  /* Cygwin and MSVC have _no_ strtold */
#else
  ld=strtold(sz, ppch);  /* normal default for ppch and ld */
#endif

#if !defined(__GMP__) || defined(__MSVC__)
  return(ld);
#endif

#ifdef __GMP__

if(iFirst)
  {
  mpf_init2(mpf, 512);
  mpf_init2(mpf2, 512);
  mpf_init2(mpf3, 512);
  mpf_init2(mpf4, 512);
  mpf_init2(mpf5, 512);
  mpf_init2(mpfHalf, 32);
  mpf_set_ui(mpfHalf, 1);
  mpf_div_2exp(mpfHalf, mpfHalf, 1);
  iFirst=0;
  }

strncpy(szLocal, sz, 511);  /* sz is const */
szLocal[511]=0;

/* Isolate the token to be converted. We can't use strtok as it
   may modify static buffers employed by the calling code; and
   since the first argument sz is "const", we must make a local
   copy szLocal for modification. Stop at the first space or
   non-printing character. */

szTrimMWS(szLocal);
i=0;
for(i=0; i < strlen(szLocal); i++)
  {
  ch=szLocal[i];
  if(!isgraph(ch))
    {
    szLocal[i]=0;
    break;
    }
  }

iRet=__mpf_set_str(mpf, szLocal, 10);
iSign=mpf_sgn(mpf);
if(iRet==-1 || iSign==0)return(ld);  /* Handles input syntax errors */

mpf_set(mpf2, mpf);
mpf_abs(mpf2, mpf2);
iExp=0;
if(mpf_cmp_ui(mpf2, 1) > 0)
  {
  while(mpf_cmp_ui(mpf2, 2) >= 0)
    {
    mpf_div_2exp(mpf2, mpf2, 1);
    iExp++;
    }
  }
else
  {
  while(mpf_cmp_ui(mpf2, 1) < 0)
    {
    mpf_mul_2exp(mpf2, mpf2, 1);
    iExp--;
    }
  }
mpf_mul_2exp(mpf2, mpf2, 63);
mpf_add(mpf2, mpf2, mpfHalf);
mpf_floor(mpf2, mpf2);

/* mpf2 now contains the integer value of the 64-bit mantissa in
   the long double representation of pchToken. */

mpf_div_2exp(mpf3, mpf2, 32);
mpf_floor(mpf3, mpf3);  /* mpf3 = high 64-32 bits of mpf2 */
ulMSL=mpf_get_ui(mpf3);
mpf_mul_2exp(mpf4, mpf3, 32);  /* mpf2 with low 32 bits zeroed */
mpf_sub(mpf5, mpf2, mpf4);  /* mpf5 = low 64-32 bits of mpf2 */
ulLSL=mpf_get_ui(mpf5);

/* ulMSL and ulLSL are now the most significant and least significant
   32-bit unsigned integers of the 64-bit integer mantissa. */

pMSL=(unsigned long *)(&ld);
pLSL=pMSL++;
*pMSL=ulMSL;
*pLSL=ulLSL;

uh=iExp + 0x3FFF;  /* Exponent bias */
if(iSign < 0)uh += 0x8000;  /* Sign bit incorporated */

/* uh now contains the value of the 16-bit sign+biased exponent
   field of the long double representation of pchToken. */

pull=(uint64_t *)(&ld);
puh=((unsigned short *)(++pull));
*puh=uh;

return(ld);
#endif  /* GMP */
}
/**********************************************************************/
char *__szLD(char *sz, long double ld, char *szFMT)
{
/* Converts ld to an ASCII string, using the format specifier szFMT.
   If szFMT is NULL, empty, or lacks the signature 'L' for long doubles,
   a default format of "%.19Le" is used (or "%.15le" for MinGW, which
   is unable to process "L", and MSVC, which has no 80-bit long doubles).

   The ASCII string is stored in sz, which must have sufficient
   allocation. A pointer to the result is also returned.

   GMP, an mpf, and gmp_sprintf are used to process the long double
   argument. If GMP is not available, the compiler's native sprintf is
   called. MPFR is used if available.

   This routine should exhibit precision >= that of most compilers'
   sprintf or printf routines, and should be particularly useful in MinGW
   with GMP. It is of little value for MSVC.

   WARNING: Since the printing value is cast to a double (53-bit mantissa)
   for MSVC and MinGW, if |ld| is < DBL_MIN or > DBL_MAX, incorrect
   values may be returned on those platforms.

   NOTE: If the first 'L' or 'l' following the '%' character in an
   (invalid) format specifier is _not_ the size specifier, this routine
   may crash or return garbage; e.g., szFMT=="%.12e is the length".
*/

char szFMT2[128], *pch, *pch2;
#ifdef __GMP__
  static int iFirst=1;
  static mpf_t mpf;
#endif

/* Edit the format string. Replace the 'L' following '%' with an 'F'
   for gmp_printf, or with an 'l' for MinGW without GMP or MSVC, or
   leave it alone for other platforms without GMP. If szFMT is NULL
   or empty, use the default format. */

if((szFMT == NULL) || (*szFMT==0))
  {
#ifdef __MSVC__
  strcpy(szFMT2, "%.15le");
#elif defined(__GMP__)
  strcpy(szFMT2, "%.19Fe");
#elif defined(__MINGW__)
  strcpy(szFMT2, "%.15le");
#else
  strcpy(szFMT2, "%.19Le");
#endif
  }
else
  {
  strcpy(szFMT2, szFMT);
  pch=strchr(szFMT2, '%');
  if(pch)
    {
    pch2=strchr(pch, 'L');
    if(!pch2)pch2=strchr(pch, 'l');
    if(pch2)
      {
#ifdef __MSVC__
      *pch2='l';
#elif defined(__GMP__)
      *pch2='F';
#elif defined(__MINGW__)
      *pch2='l';
#endif
      }
    else
      {
#ifdef __MSVC__
      strcpy(szFMT2, "%.15le");
#elif defined(__GMP__)
      strcpy(szFMT2, "%.19Fe");
#elif defined(__MINGW__)
      strcpy(szFMT2, "%.15le");
#else
      strcpy(szFMT2, "%.19Le");
#endif
      }
    }
  else
    {
#ifdef __MSVC__
    strcpy(szFMT2, "%.15le");
#elif defined __GMP__
    strcpy(szFMT2, "%.19Fe");
#elif defined(__MINGW__)
    strcpy(szFMT2, "%.15le");
#else
    strcpy(szFMT2, "%.19Le");
#endif
    }
  }

#ifdef __MPFR__
  mpfr_t mpfr;
  mpf_t mpfTemp;
  mpfr_init2(mpfr, 128);
  mpf_init2(mpf, 128);
  mpfr_set_ld(mpfr, ld, GMP_RNDN);
  mpfr_get_f(mpf, mpfr, GMP_RNDN);
  gmp_sprintf(sz, szFMT2, mpf);
  mpf_clear(mpf);
  mpfr_clear(mpfr);
  return(sz);
#endif

/* Now print ld to sz using gmp_sprintf or sprintf and the edited format. */

#if (defined(__MINGW__) && !defined(__GMP__)) \
    || defined (__MSVC__)

/* MSVC and MinGW cannot print 80-bit long doubles correctly; edit for
   overflow and cast to double. */

if(ld > DBL_MAX)
  ld=DBL_MAX;
else if(ld < -DBL_MAX)
  ld=-DBL_MAX;

sprintf(sz, szFMT2, (double)ld);  /* MSVC has no 80-bit ld's */

#elif defined(__GMP__)

if(iFirst)  /* Initialize static mpf on the first invocation */
  {
  mpf_init(mpf);
  iFirst=0;
  }
__mpf_set_ld(mpf, ld);
gmp_sprintf(sz, szFMT2, mpf);

#else

sprintf(sz, szFMT, ld);

#endif

return(sz);
}
/**********************************************************************/
char *__szLL(char *sz, int64_t ll)
{
/* Converts ll to an ASCII string of minimum length. It is assumed
 * that ll is a 64-bit signed integer.
 *
 * The ASCII string is stored in sz, which must have sufficient
 * allocation. A pointer to the result is also returned.
 *
 * Targeted at printf-challenged environments, such as MinGW.
 *
 */

static char sz2[32];
unsigned long ul1, ul2, ul3;
uint64_t ull;

sz[0]=0;
if(ll==0)
  {
  strcpy(sz, "0");
  return(sz);
  }
if(ll < 0)
  {
  strcpy(sz, "-");
  ll = -ll;
  }

if(ll < 1e9)
  sprintf(sz2, "%lu", (unsigned long)ll);
else if(ll < 1e18)
  {
  ul1=(unsigned long)(ll/__ULL(1000000000));
  ul2=(unsigned long)(ll % __ULL(1000000000));
  sprintf(sz2, "%lu%09lu", ul1, ul2);
  }
else
  {
  ul1=(unsigned long)(ll/__ULL(1000000000000000000));
  ull=ll % __ULL(1000000000000000000);
  ul2=(unsigned long)(ull/__ULL(1000000000));
  ul3=(unsigned long)(ull % __ULL(1000000000));
  sprintf(sz2, "%lu%09lu%09lu", ul1, ul2, ul3);
  }
strcat(sz, sz2);

return(sz);
}
/**********************************************************************/
char *__szULL(char *sz, uint64_t ull)
{
/* Converts ull to an ASCII string of minimum length. It is assumed
 * that ull is a 64-bit unsigned integer.
 *
 * The ASCII string is stored in sz, which must have sufficient
 * allocation. A pointer to the result is also returned.
 *
 * Targeted at printf-challenged environments, such as MinGW.
 *
 */

static char sz2[32];
unsigned long ul1, ul2, ul3;
uint64_t ull2;

sz[0]=0;
if(ull==0)
  {
  strcpy(sz, "0");
  return(sz);
  }

if(ull < 1e9)
  sprintf(sz2, "%lu", (unsigned long)ull);
else if(ull < 1e18)
  {
  ul1=(unsigned long)(ull/__ULL(1000000000));
  ul2=(unsigned long)(ull % __ULL(1000000000));
  sprintf(sz2, "%lu%09lu", ul1, ul2);
  }
else
  {
  ul1=(unsigned long)(ull/__ULL(1000000000000000000));
  ull2=ull % __ULL(1000000000000000000);
  ul2=(unsigned long)(ull2/__ULL(1000000000));
  ul3=(unsigned long)(ull2 % __ULL(1000000000));
  sprintf(sz2, "%lu%09lu%09lu", ul1, ul2, ul3);
  }
strcat(sz, sz2);

return(sz);
}
/**********************************************************************/
/**********************************************************************/
#ifdef __GMP__
/**********************************************************************/
/**********************************************************************/
/*                    GMP mpz BIGINT functions                        */
/**********************************************************************/
/**********************************************************************/
int __mpz_set_str(mpz_t mpz, char *sz, int iBase)
{
/* Fixes a bug (feature?) in the GNU GMP floating-point library (noted
   in both version 4.01 and version 4.14). A leading plus sign in sz is
   not properly recognized, causing an erroneous value of zero to be
   assigned to mpz. The fix used is to check the first visible character
   in sz; if it is a '+', replace it with a space. */

char ch;
int i, j=-1, iRet;

for(i=0; i < strlen(sz); i++)
  {
  ch=sz[i];
  if(ch=='+')
    {
    sz[i]=' ';
    j=i;  /* save the change */
    break;
    }
  if(isgraph(ch))break; /* printing characters except space */
  }
iRet=mpz_set_str(mpz, sz, iBase);
if(j != -1)sz[i]='+';  /* restore sz */
return(iRet);
}
/**********************************************************************/
uint64_t __mpz_get_ull(mpz_t mpz)
{
/* Return the value of mpz as an unsigned long long.  If the value of mpz
   overflows the ULL range, abort.

   It is assumed that mpz limbs and unsigned longs have equal allocation
   (usually 32 bits), and an unsigned long long has twice as much
   allocation (usually 64 bits). */

unsigned long          iLimbs;
static int             iConforms=0;

/* If the host system does not conform to the above assumptions, use
   mpz_set_str, mpz_get_str, and sprintf to carry out the assignment. */

if(!iConforms)
  {
  unsigned long ul, ul2, ul3;
  uint64_t ull;
  ul=sizeof(mp_limb_t);
  ul2=sizeof(unsigned long);
  ul3=sizeof(uint64_t);
  if((ul != ul2) || (ul3 != ul2 << 1))
    {
    mpz_t mpzMAX;
    char *szULL = (char *)malloc(2 + ceil(log10(UINT64_MAX)));
    mpz_init2(mpzMAX, floor(0.5 + log10(UINT64_MAX)/log10(2)));
    iConforms=0;
    sprintf(szULL, "%" PRIu64 , UINT64_MAX);
    __mpz_set_str(mpzMAX, szULL, 10);
    if((mpz_sgn(mpz) >= 0) && (mpz_cmp(mpz, mpzMAX) <= 0))
      {
      mpz_get_str(szULL, 10, mpz);
      ull=strtoull(szULL, NULL, 10);
      free(szULL);
      mpz_clear(mpzMAX);
      return(ull);
      }
    else
      {
      fprintf(stderr, "\n\n ERROR: Domain error in __mpz_get_ull:\nmpz=");
      mpz_out_str(stderr, 10, mpz);
      fprintf(stderr, "\n\n");
      exit(EXIT_FAILURE);
      }
    }
  else
    iConforms=1;
  }

iLimbs=mpz->_mp_size;
if(iLimbs==0)return(0);
if(iLimbs==1)return((uint64_t)mpz->_mp_d[0]);
if(iLimbs==2)
  return((UINT32_MAX + __LL(1))*mpz->_mp_d[1] + mpz->_mp_d[0]);
fprintf(stderr, "\n\n ERROR: Domain error in __mpz_get_ull:\nmpz=");
mpz_out_str(stderr, 10, mpz);
fprintf(stderr, "\n\n");
exit(EXIT_FAILURE);
}
/**********************************************************************/
void __mpz_set_ull(mpz_t mpz, uint64_t ull)
{
/* Set a previously initialized mpz to the unsigned long long value ull.
   It is assumed that mpz limbs and unsigned longs have equal allocation
   (usually 32 bits), and an unsigned long long has twice as much
   allocation (usually 64 bits). */

unsigned long          ul;
static int             iConforms=0;

if(ull <= UINT32_MAX)
  {
  mpz_set_ui(mpz, (unsigned long)ull);
  return;
  }

/* If the host system does not conform to the above assumptions, use
   sprintf and mpz_set_str to carry out the assignment. */

if(!iConforms)
  {
  unsigned long ul2, ul3;
  ul=sizeof(mp_limb_t);
  ul2=sizeof(unsigned long);
  ul3=sizeof(uint64_t);
  if((ul != ul2) || (ul3 != ul2 << 1))
    {
    char *szULL=
      (char *)malloc(3 + (sizeof(uint64_t)*CHAR_BIT*3)/10);
    iConforms=0;
    sprintf(szULL, "%" PRIu64 , ull);
    __mpz_set_str(mpz, szULL, 10);
    free(szULL);
    return;
    }
  else
    iConforms=1;
  }

/* Conforming system, true unsigned long long */

if(mpz->_mp_alloc < 2)_mpz_realloc(mpz,2);
ul=ull/(UINT32_MAX + __LL(1));  /* high doubleword */
mpz->_mp_d[1]=ul;
mpz->_mp_d[0]=ull - ul*(UINT32_MAX + __LL(1));  /* low doubleword */
mpz->_mp_size=2;

return;
}
/**********************************************************************/
long double __mpz_get_ld(mpz_t mpz)
{
char            *pch;
long double     ld;

pch=(char *)malloc(mpz_sizeinbase(mpz, 10) + 2);
mpz_get_str(pch, 10, mpz);
ld=___strtold(pch, NULL);
free(pch);
return(ld);
}
/**********************************************************************/
void __mpz_set_ld(mpz_t mpz, long double ld)
{
char *pch;

if(ld==0)
  {
  mpz_set_ui(mpz, 0);
  return;
  }
pch=(char *)malloc((unsigned long)(5 + fabsl(log10l(fabsl(ld)))));
__szLD(pch, ld, "%.0Lf");
__mpz_set_str(mpz, pch, 10);
free(pch);
return;
}
/**********************************************************************/
int __mpz_cmp_ld(mpz_t mpz, long double ld)
{
int iSignMPZ, iComp;
mpz_t mpzLD;

iSignMPZ=mpz_sgn(mpz);
if((iSignMPZ < 0) && (ld >=0))return(-1);
if((iSignMPZ >= 0) && (ld < 0))return(1);
if((iSignMPZ==0) && (ld==0))return(0);

mpz_init2(mpzLD, (1 + ceill(log10l(fabsl(ld))))*mp_bits_per_limb);
__mpz_set_ld(mpzLD, ld);
iComp=mpz_cmp(mpz, mpzLD);
mpz_clear(mpzLD);
return(iComp);
}
/**********************************************************************/
long double __mpz_log10l(mpz_t mpz)
{
char            *pch, szMant[26];
int             i;
unsigned long   ulExp;
long double     ld;

if(mpz_sgn(mpz) <= 0)
  {
  fprintf(stderr, "\n ERROR: Domain error in __mpz_log10l.\n\n");
  exit(EXIT_FAILURE);
  }
gmp_asprintf(&pch, "%Zd", mpz);
ulExp=strlen(pch)-1;
if(mpz_sgn(mpz) < 0)
  {
  ulExp--;
  szMant[0]=pch[0];
  szMant[1]=pch[1];
  szMant[2]='.';
  for(i=3; i < 25; i++)szMant[i]=pch[i-1];
  szMant[25]=0;
  }
else
  {
  szMant[0]=pch[0];
  szMant[1]='.';
  for(i=2; i < 24; i++)szMant[i]=pch[i-1];
  szMant[24]=0;
  }
ld=ulExp + log10l(___strtold(szMant, NULL));
free(pch);
return(ld);
}
/**********************************************************************/
long double __mpz_logl(mpz_t mpz)
{
return(__mpz_log10l(mpz)*M_LN10);
}
/**********************************************************************/
void __mpz_powl(mpz_t mpz, long double ldBase, long double ldExp)
{
long double ld10Exp, ldIntExp, ldFracExp, ldMant, ldMant2, ld;
mpz_t mpz1, mpz2;

if(ldBase <= 0)
  {
  fprintf(stderr,
    "\n ERROR: Domain error (base <= 0) in __mpz_powl.\n\n");
  exit(EXIT_FAILURE);
  }
ld=powl(ldBase, ldExp);
__mpz_set_ld(mpz, ld);
return;
}
/**********************************************************************/
void __mpz_expl(mpz_t mpz, long double ldExp)
{
long double ld;

ld=expl(ldExp);
__mpz_set_ld(mpz, ld);
return;
}
/**********************************************************************/
/**********************************************************************/
/*                         GMP mpf functions                          */
/**********************************************************************/
/**********************************************************************/
int __mpf_set_str(mpf_t mpf, char *sz, int iBase)
{
/* Fixes a bug (feature?) in the GNU GMP floating-point library (noted
   in both version 4.01 and version 4.14). A leading plus sign in sz is
   not properly recognized, causing an erroneous value of zero to be
   assigned to mpf. The fix used is to check the first visible character
   in sz; if it is a '+', replace it with a space. */

char ch;
int i, j=-1, iRet;

for(i=0; i < strlen(sz); i++)
  {
  ch=sz[i];
  if(ch=='+')
    {
    sz[i]=' ';
    j=i;  /* save the change */
    break;
    }
  if(isgraph(ch))break; /* printing characters except space */
  }
iRet=mpf_set_str(mpf, sz, iBase);
if(j != -1)sz[i]='+';  /* restore sz */
return(iRet);
}
/***********************************************************************/
long double __mpf_get_ld(mpf_t mpf)
{
/* Convert an mpf to a long double (double in MSVC). */

static char sz[64];

#ifdef __MSVC__
  return((long double)mpf_get_d(mpf));
#else
  gmp_sprintf(sz, "%.25Fe", mpf);
  return(___strtold(sz, NULL));
#endif
}
/***********************************************************************/
void __mpf_set_ld(mpf_t mpf, long double ld)
{
/* Convert an intrinsic long double to its decimal mpf representation.
   It is assumed that the long doubles are stored in IEEE 754 extended
   double format (Intel x86/x87 ten-byte extended or temporary reals).
   Therefore MSVC will simply return the nearest double value, losing
   3 or 4 digits of precision. MSVC will also fail for values of ld
   whose magnitude is outside the range [DBL_MIN, DBL_MAX]. */

int iSign=1;
unsigned short uh;
int iExp;
unsigned long ulLS, ulMS, *pLS, *pMS;
uint64_t *pull, ull;

#ifdef __MSVC__

/* MSVC has no 80-bit long doubles. Edit for overflow and cast
   to double. */

if(ld > DBL_MAX)
  ld=DBL_MAX;
else if(ld < -DBL_MAX)
  ld=-DBL_MAX;
mpf_set_d(mpf, (double)ld);
return;

#else

pull=(uint64_t *)(&ld);
ull=*(pull);  /* Integer form of the ld mantissa */
uh=*((unsigned short *)(++pull));  /* Biased exponent plus sign bit */
if(uh & 0x8000)  /* Is ld negative? */
  {
  uh = uh & 0x7FFF;  /* Convert to magnitude */
  iSign=-1;
  }

iExp = uh - 0x3FFF;  /* Subtract bias */

/* Now iExp contains the exponent, iSign the sign, and ull the mantissa
   (shifted 63 bits to the right). Since GMP has no routine for assigning
   an unsigned long long to an mpf, we must retrieve the high and low
   32 bits of ull and calculate the mpf for ull indirectly. */

pMS=(unsigned long *)(&ld);
pLS=pMS++;
ulLS=*pLS;
ulMS=*pMS;

mpf_set_ui(mpf, ulMS);
mpf_mul_2exp(mpf, mpf, 32);
mpf_add_ui(mpf, mpf, ulLS);
mpf_div_2exp(mpf, mpf, 63);  /* True mantissa is shifted 63 bits */
if(iExp > 0)mpf_mul_2exp(mpf, mpf, iExp);
if(iExp < 0)mpf_div_2exp(mpf, mpf, -iExp);
if(iSign==-1)mpf_neg(mpf, mpf);

return;
#endif  /* not MSVC */
}
/**********************************************************************/
void __mpf_set_ld2(mpf_t mpf, long double ld)
{
/* Converts a long double ld to its mpf equivalent. This version has
   better speed than __mpf_set_ld, and is not dependent on the 80-bit
   IEEE floating point format. However, it may be inaccurate in the
   19th and succeeding significant decimal digits, due to limitations
   in some implementations of sprintf. There is no overflow or other
   error trapping. Since MSVC has no genuine long doubles, and MinGW
   cannot properly interpret a long double format specifier, mpf
   is assigned the nearest double value on those two platforms, and
   the 16th and succeeding significant digits may be in error; also,
   if |ld| is outside the range [DBL_MIN, DBL_MAX], the result will
   be in error. */

static char sz[64];

#if defined(__MINGW__) || defined(__MSVC__)
  if(ld > DBL_MAX)
    ld=DBL_MAX;
  else if(ld < -DBL_MAX)
    ld=-DBL_MAX;
  mpf_set_d(mpf, (double)ld);
#else
  sprintf(sz, "%.25Le", ld);
__mpf_set_str(mpf, sz, 10);
#endif

return;
}
/***********************************************************************/
/**********************************************************************/
#endif  /* GMP available */
#ifdef __MPFR__
/**********************************************************************/
/**********************************************************************/
/*                         GMP + MPFR functions                       */
/**********************************************************************/
/**********************************************************************/
#undef LIT_LI2
#undef LIT_C2
#undef LIT_C3
#undef LIT_C4
#undef LIT_R2
#undef DEF_PREC_BITS
#undef MAX_PREC_BITS
#define LIT_LI2 "1.0451637801174927848445888891946131365226155781512015758329091440750132052103595301727174056263833563"
#define LIT_C2  "0.6601618158468695739278121100145557784326233602847334133194484233354056423044952771437600314138398679"
#define LIT_C3  "0.6351663546042712072066965912725224173420656873323724508997344604867846131161391882080291386764046176"
#define LIT_C4  "0.3074948787583270931233544860710768530221785199506639282983083962608887672966929948138402646817149384"
#define LIT_R2  "1.5410090161871318832885037866275465435308992182709284694298743638692436905783372048127122002248898594"
#define DEF_PREC_BITS  128  /* 128 bits or 38 SDD */
#define MAX_PREC_BITS  320  /* 320 bits or 96 SDD */
/**********************************************************************/
/**********************************************************************/
void mpfrLIRZ(mpfr_t mpfrx, mpfr_t mpfrLi, mpfr_t mpfrHL2,
  mpfr_t mpfrHL3, mpfr_t mpfrHL4, mpfr_t mpfrR, int iLocalPrec)
{
/* The logarithmic integral expressions Li(x), HL2(x), HL3(x), and HL4(x),
   approximating pi(x), pi_2(x), pi_3(x), and pi_4(x) respectively---the
   counts from 0 to x of the primes, twin-prime pairs, basic prime triplets,
   and basic prime quadruplets---are approximated using a series obtained as
   explained below.

   In addition, Riemann's prime counting function R(x) is approximated,
   using a truncated Gram series; see "Prime numbers and computer methods
   for factorization," Hans Reisel (Birkhauser, Boston, 1994), pp. 50-51,
   especially eqn 2.27.

   NOTE: The domain of the algorithm is x > 1; the accuracy degrades
   near x=1, a singular point of these functions; and these functions
   are rarely called with arguments x < 2. Consequently, for x < 2,
   this routine returns artificial values of zero.

   The default precision is 128 bits (iLocalPrec=128) or 36 significant
   decimal digits (after typical rounding errors). The routine can be
   called with values of iLocalPrec as high as 320, for 96 significant
   decimal digits, or with iLocalPrec as low as 32 (9 SDD). Higher
   precisions could be obtained by a slight modification of the
   code---consisting mainly of providing more precise values for the
   literal constants LIT_LI2, LIT_C2, LIT_C3, LIT_C4, and LIT_R2, and
   changing the #define for MAX_PREC_BITS.

   In general, one can expect cumulative rounding errors of two or
   three SDD. Thus 128 bits will actually yield 35 or 36 correct
   decimal digits.

   The fact that Li(x) is asymptotic to pi(x) is one statement of the
   Prime Number Theorem. The use of L2(x), L3(x), and L4(x) as
   approximations to pi_2(x), pi_3(x), and pi_4(x) is a consequence
   of the prime k-tuples conjecture of Hardy and Littlewood (ca. 1922).

   The technique is as follows. In I4=int(1/(ln t)^4, t) substitute
   u=ln(t), yielding the integral int(exp(u)/u^4, u). Substitute the
   Maclaurin series for exp(u). Integrate the first five terms separately
   to yield

   I4=-1/(3u^3) - 1/(2u^2) - 1/(2u) + (ln u)/6 + u/24
                                          + sum(u^k/(k*(k+3)!), k, 2, inf).

   Replace u by ln(t) and apply the limits t=2 to t=x to produce the
   result for the integral in L4.  Note that the terms in the resulting
   series are related by

   T(k+1)=T(k)*(ln t)*k/((k+1)(k+4).

   Iterate the series until the ratio of successive terms is < M_EPSILON1
   (LDBL_EPSILON/4, approximately 2.71e-20 on an x386 system).

   Once I4 is evaluated, I3, I2, and I1 can be obtained using (reverse)
   integration by parts on I1(t)=int(1/(ln t), t):

   I1(t)=t/(ln t) + I2(t)=t/(ln t) + t/(ln t)^2 + 2*I3(t)
        =t/(ln t) + t/(ln t)^2 + 2t/(ln t)^3 + 6*I4(t) ,
   or

   I3(t)=t/(ln t)^3 + 3*I4(t)

   I2(t)=t/(ln t)^2 + 2*I3(t)

   I1(t)=t/(ln t) + I2(t).

   Now apply the limits 2 to x. Add ldLi2 to L1 to account for
   the lower limit being 0 rather than 2. Multiply I4, I3, and I2
   by the appropriate Hardy-Littlewood constants to obtain the
   estimates HL4(x), HL3(x), and HL2(x)for pi_4(x), pi_3(x),
   and pi_2(x).

   R(x), the Riemann prime-counting function, is calculated using
   a different algorithm (truncated Gram series); see the discussion
   in the routine ldRPCF.
*/

static int iFirst=1;
static mpf_t mpf;
static mpfr_t mpfrLogx, mpfrLogx2, mpfrLogx3, mpfrLI2, mpfrLN2, mpfrLN2_2,
  mpfrLN2_3, mpfr1_LN2, mpfr1_LN2_2, mpfr1_LN2_3, mpfrLN2_24, mpfrC2,
  mpfrC3, mpfrC4, mpfrI2, mpfrI3, mpfrI4, mpfr1, mpfr2, mpfrTerm1,
  mpfrTerm2, mpfrDelta, mpfrEpsilon, mpfr3, mpfr4, mpfr5, mpfrt,
  mpfrFactor, mpfrZeta;
int iGlobalPrec;
unsigned long ul;

iGlobalPrec=mpfr_get_default_prec();
if(iLocalPrec < 32)iLocalPrec=DEF_PREC_BITS;
if(iLocalPrec > MAX_PREC_BITS)iLocalPrec=MAX_PREC_BITS;
mpfr_set_default_prec(iLocalPrec);

if(iFirst)
  {
  mpfr_init_set_str(mpfrLI2, LIT_LI2, 10, GMP_RNDN);
  mpfr_init(mpfrLN2);
  mpfr_const_log2(mpfrLN2, GMP_RNDN);
  mpfr_init(mpfrLN2_24);
  mpfr_div_ui(mpfrLN2_24, mpfrLN2, 24, GMP_RNDN);
  mpfr_init(mpfrLN2_2);
  mpfr_sqr(mpfrLN2_2, mpfrLN2, GMP_RNDN);
  mpfr_init(mpfrLN2_3);
  mpfr_mul(mpfrLN2_3, mpfrLN2_2, mpfrLN2, GMP_RNDN);
  mpfr_init(mpfr1_LN2);
  mpfr_ui_div(mpfr1_LN2, 1, mpfrLN2, GMP_RNDN);
  mpfr_init(mpfr1_LN2_2);
  mpfr_sqr(mpfr1_LN2_2, mpfr1_LN2, GMP_RNDN);
  mpfr_init(mpfr1_LN2_3);
  mpfr_mul(mpfr1_LN2_3, mpfr1_LN2_2, mpfr1_LN2, GMP_RNDN);
  mpfr_init_set_str(mpfrC2, LIT_C2, 10, GMP_RNDN);
  mpfr_init_set_str(mpfrC3, LIT_C3, 10, GMP_RNDN);
  mpfr_init_set_str(mpfrC4, LIT_C4, 10, GMP_RNDN);
  mpfr_init_set_ui(mpfrI2, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrI3, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrI4, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrLogx, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrLogx2, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrLogx3, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfr1, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfr2, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfr3, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfr4, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfr5, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrTerm1, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrTerm2, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrDelta, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrt, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrFactor, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrZeta, 0, GMP_RNDN);
  iFirst=0;
  }

mpfr_init_set_ld(mpfrEpsilon, powl(2, -iLocalPrec-3), GMP_RNDN);
mpf_init2(mpf, iLocalPrec);
mpfr_set_str(mpfrLi, LIT_LI2, 10, GMP_RNDN);
mpfr_set_ui(mpfrHL2, 0, GMP_RNDN);
mpfr_set_ui(mpfrHL3, 0, GMP_RNDN);
mpfr_set_ui(mpfrHL4, 0, GMP_RNDN);
mpfr_set_str(mpfrR, LIT_R2, 10, GMP_RNDN);

if(mpfr_cmp_ui(mpfrx, 2)==0)return;
if(mpfr_cmp_ui(mpfrx, 2) < 0)
  {
  mpfr_set_ui(mpfrLi, 0, GMP_RNDN);
  mpfr_set_ui(mpfrR, 0, GMP_RNDN);
  return;
  }

mpfr_log(mpfrLogx, mpfrx, GMP_RNDN);
mpfr_sqr(mpfrLogx2, mpfrLogx, GMP_RNDN);
mpfr_mul(mpfrLogx3, mpfrLogx2, mpfrLogx, GMP_RNDN);

/* Initialize I4 */

mpfr_mul_ui(mpfrI4, mpfrLogx3, 3, GMP_RNDN);
mpfr_si_div(mpfrI4, -1, mpfrI4, GMP_RNDN);       //  -1/(3*ldLx3)

mpfr_div_ui(mpfr1, mpfr1_LN2_3, 3, GMP_RNDN);
mpfr_add(mpfrI4, mpfrI4, mpfr1, GMP_RNDN);      //  +1/(3*M_LN2_CUBED)

mpfr_mul_2ui(mpfr1, mpfrLogx2, 1, GMP_RNDN);
mpfr_ui_div(mpfr1, 1, mpfr1, GMP_RNDN);
mpfr_sub(mpfrI4, mpfrI4, mpfr1, GMP_RNDN);      //  -1/(2*ldLx2)

mpfr_mul_2ui(mpfr1, mpfrLN2_2, 1, GMP_RNDN);
mpfr_ui_div(mpfr1, 1, mpfr1, GMP_RNDN);
mpfr_add(mpfrI4, mpfrI4, mpfr1, GMP_RNDN);      //  +1/(2*M_LN2_SQUARED)

mpfr_mul_2ui(mpfr1, mpfrLogx, 1, GMP_RNDN);
mpfr_ui_div(mpfr1, 1, mpfr1, GMP_RNDN);
mpfr_sub(mpfrI4, mpfrI4, mpfr1, GMP_RNDN);      //  -1/(2*ldLx)

mpfr_mul_2ui(mpfr1, mpfrLN2, 1, GMP_RNDN);
mpfr_ui_div(mpfr1, 1, mpfr1, GMP_RNDN);
mpfr_add(mpfrI4, mpfrI4, mpfr1, GMP_RNDN);      //   +1/(2*M_LN2)

mpfr_div(mpfr1, mpfrLogx, mpfrLN2, GMP_RNDN);
mpfr_log(mpfr1, mpfr1, GMP_RNDN);
mpfr_div_ui(mpfr1, mpfr1, 6, GMP_RNDN);         //
mpfr_add(mpfrI4, mpfrI4, mpfr1, GMP_RNDN);      //   +logl(ldLx/M_LN2)/6

/* Initialize term1 and term2 */

mpfr_div_ui(mpfrTerm1, mpfrLogx, 24, GMP_RNDN);
mpfr_set(mpfrTerm2, mpfrLN2_24, GMP_RNDN);

for(ul=1; ; ul++)
  {
  mpfr_add(mpfrI4, mpfrI4, mpfrTerm1, GMP_RNDN);
  mpfr_sub(mpfrI4, mpfrI4, mpfrTerm2, GMP_RNDN);
  mpfr_mul(mpfrDelta, mpfrEpsilon, mpfrI4, GMP_RNDN);
  mpfr_abs(mpfrDelta, mpfrDelta, GMP_RNDN);
  mpfr_set_ui(mpfr1, ul+1, GMP_RNDN);
  mpfr_mul_ui(mpfr1, mpfr1, ul+4, GMP_RNDN);
  mpfr_ui_div(mpfr1, ul, mpfr1, GMP_RNDN);
  mpfr_mul(mpfrTerm1, mpfrTerm1, mpfr1, GMP_RNDN);
  mpfr_mul(mpfrTerm1, mpfrTerm1, mpfrLogx, GMP_RNDN);
  if(mpfr_cmp(mpfrTerm1, mpfrDelta) < 0)break;
  if(mpfr_cmp(mpfrTerm2, mpfrDelta) > 0)
    {
    mpfr_mul(mpfrTerm2, mpfrTerm2, mpfrLN2, GMP_RNDN);
    mpfr_mul(mpfrTerm2, mpfrTerm2, mpfr1, GMP_RNDN);
    }
  else
    mpfr_set_ui(mpfrTerm2, 0, GMP_RNDN);
  }

mpfr_mul(mpfrHL4, mpfrI4, mpfrC4, GMP_RNDN);
mpfr_mul_ui(mpfrHL4, mpfrHL4, 27, GMP_RNDN);
mpfr_div_2ui(mpfrHL4, mpfrHL4, 1, GMP_RNDN);  // HL4 = 27/2*c_4*I4

mpfr_mul_ui(mpfrI3, mpfrI4, 3, GMP_RNDN);  // I3 = 3*I4 + x/(ln x)^3 - 2/(ln 2)^3
mpfr_div(mpfr1, mpfrx, mpfrLogx3, GMP_RNDN);
mpfr_add(mpfrI3, mpfrI3, mpfr1, GMP_RNDN);
mpfr_mul_2ui(mpfr1, mpfr1_LN2_3, 1, GMP_RNDN);
mpfr_sub(mpfrI3, mpfrI3, mpfr1, GMP_RNDN);
mpfr_mul(mpfrHL3, mpfrI3, mpfrC3, GMP_RNDN);
mpfr_mul_ui(mpfrHL3, mpfrHL3, 9, GMP_RNDN);
mpfr_div_2ui(mpfrHL3, mpfrHL3, 1, GMP_RNDN);  // HL3 = 9/2*c_3*I3

mpfr_mul_2ui(mpfrI2, mpfrI3, 1, GMP_RNDN);  // I2 = 2*I3 + x/(ln x)^2 - 2/(ln 2)^2
mpfr_div(mpfr1, mpfrx, mpfrLogx2, GMP_RNDN);
mpfr_add(mpfrI2, mpfrI2, mpfr1, GMP_RNDN);
mpfr_mul_2ui(mpfr1, mpfr1_LN2_2, 1, GMP_RNDN);
mpfr_sub(mpfrI2, mpfrI2, mpfr1, GMP_RNDN);
mpfr_mul(mpfrHL2, mpfrI2, mpfrC2, GMP_RNDN);
mpfr_mul_2ui(mpfrHL2, mpfrHL2, 1, GMP_RNDN);  // HL2 = 2*c_2*I2

mpfr_div(mpfrLi, mpfrx, mpfrLogx, GMP_RNDN);  //  Li = I2 + x/(ln x) - 2/(ln 2) +
mpfr_add(mpfrLi, mpfrLi, mpfrI2, GMP_RNDN);   //          + Li(2)
mpfr_mul_2ui(mpfr1, mpfr1_LN2, 1, GMP_RNDN);
mpfr_sub(mpfrLi, mpfrLi, mpfr1, GMP_RNDN);
mpfr_add(mpfrLi, mpfrLi, mpfrLI2, GMP_RNDN);

/* Compute the Riemann prime-counting function R(x), using a truncated
   Gram series. See "Prime numbers and computer methods for
   factorization," Hans Reisel (Birkhauser, Boston, 1994), pp. 50-51,
   especially eqn 2.27. */

mpfr_set(mpfrt, mpfrLogx, GMP_RNDN);
mpfr_set(mpfrFactor, mpfrLogx, GMP_RNDN);
mpfr_set_ui(mpfr1, 2, GMP_RNDN);
mpfr_zeta(mpfrZeta, mpfr1, GMP_RNDN);
mpfr_div(mpfrR, mpfrFactor, mpfrZeta, GMP_RNDN);
for(ul=2; ; ul++)
  {
  mpfr_mul(mpfrFactor, mpfrFactor, mpfrt, GMP_RNDN);
  mpfr_mul_ui(mpfrFactor, mpfrFactor, ul-1, GMP_RNDN);
  mpfr_set_ui(mpfr1, ul, GMP_RNDN);
  mpfr_sqr(mpfr1, mpfr1, GMP_RNDN);
  mpfr_div(mpfrFactor, mpfrFactor, mpfr1, GMP_RNDN);
  mpfr_set_ui(mpfr1, ul+1, GMP_RNDN);
  mpfr_zeta(mpfrZeta, mpfr1, GMP_RNDN);
  mpfr_div(mpfrTerm1, mpfrFactor, mpfrZeta, GMP_RNDN);
  mpfr_add(mpfrR, mpfrR, mpfrTerm1, GMP_RNDN);
  mpfr_div(mpfr1, mpfrTerm1, mpfrR, GMP_RNDN);
  if(mpfr_cmp(mpfr1, mpfrEpsilon) < 0)break;
  }
mpfr_add_ui(mpfrR, mpfrR, 1, GMP_RNDN);

mpfr_set_default_prec(iGlobalPrec);
return;
}
/**********************************************************************/
void mpfrRRF(mpfr_t mpfrOut, mpfr_t mpfrIn, int iLocalPrec)
{
/* Compute the Riemann prime-counting function R(x), also known as
   Riemann's R-function, using a truncated Gram series. See "Prime
   numbers and computer methods for factorization," Hans Reisel
   (Birkhauser, Boston, 1994), pp. 50-51, especially eqn 2.27. */

static int iFirst=1;
static mpfr_t mpfrLogx, mpfr1, mpfrTerm1, mpfrEpsilon, mpfrt,
  mpfrFactor, mpfrZeta;
int iGlobalPrec;
unsigned long ul;

iGlobalPrec=mpfr_get_default_prec();
if(iLocalPrec < 32)iLocalPrec=DEF_PREC_BITS;
if(iLocalPrec > MAX_PREC_BITS)iLocalPrec=MAX_PREC_BITS;
mpfr_set_default_prec(iLocalPrec);

if(iFirst)
  {
  mpfr_init_set_ui(mpfrLogx, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfr1, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrTerm1, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrt, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrFactor, 0, GMP_RNDN);
  mpfr_init_set_ui(mpfrZeta, 0, GMP_RNDN);
  mpfr_init_set_ld(mpfrEpsilon, powl(2, -iLocalPrec-3), GMP_RNDN);
  iFirst=0;
  }
mpfr_set_str(mpfrOut, LIT_R2, 10, GMP_RNDN);

if(mpfr_cmp_ui(mpfrIn, 2)==0)return;
if(mpfr_cmp_ui(mpfrIn, 2) < 0)
  {
  mpfr_set_ui(mpfrOut, 0, GMP_RNDN);
  mpfr_set_default_prec(iGlobalPrec);
  return;
  }

mpfr_log(mpfrLogx, mpfrIn, GMP_RNDN);
mpfr_set(mpfrt, mpfrLogx, GMP_RNDN);
mpfr_set(mpfrFactor, mpfrLogx, GMP_RNDN);
mpfr_set_ui(mpfr1, 2, GMP_RNDN);
mpfr_zeta(mpfrZeta, mpfr1, GMP_RNDN);
mpfr_div(mpfrOut, mpfrFactor, mpfrZeta, GMP_RNDN);
for(ul=2; ; ul++)
  {
  mpfr_mul(mpfrFactor, mpfrFactor, mpfrt, GMP_RNDN);
  mpfr_mul_ui(mpfrFactor, mpfrFactor, ul-1, GMP_RNDN);
  mpfr_set_ui(mpfr1, ul, GMP_RNDN);
  mpfr_sqr(mpfr1, mpfr1, GMP_RNDN);
  mpfr_div(mpfrFactor, mpfrFactor, mpfr1, GMP_RNDN);
  mpfr_set_ui(mpfr1, ul+1, GMP_RNDN);
  mpfr_zeta(mpfrZeta, mpfr1, GMP_RNDN);
  mpfr_div(mpfrTerm1, mpfrFactor, mpfrZeta, GMP_RNDN);
  mpfr_add(mpfrOut, mpfrOut, mpfrTerm1, GMP_RNDN);
  mpfr_div(mpfr1, mpfrTerm1, mpfrOut, GMP_RNDN);
  if(mpfr_cmp(mpfr1, mpfrEpsilon) < 0)break;
  }
mpfr_add_ui(mpfrOut, mpfrOut, 1, GMP_RNDN);

mpfr_set_default_prec(iGlobalPrec);
return;
}
/**********************************************************************/
/**********************************************************************/
#endif  /* MPFR available */
/**********************************************************************/
/**********************************************************************/
/*              Prime number generation and testing                   */
/**********************************************************************/
/**********************************************************************/
uint64_t ullGCD(uint64_t N1, uint64_t N2)
{
/* Return the GCD of two unsigned 64-bit integers (Euclidean algorithm).
   The GMP documentation states that the binary algorithm is faster,
   but timing comparisons contradict this, at least for 64-bit integers. */

uint64_t R;

if(N1*N2==0)return(0);
while(1)
  {
  R = N1 % N2;
  if(R==0)break;
  N1=N2;  /* a <-- b      */
  N2=R;   /* b <-- a - qb */
  }
return(N2);
}
/**********************************************************************/
void vGenPrimesDiv(unsigned long *ulPrime, unsigned long *nPrimes,
  unsigned long *ulUB)         /* revised version of vGenPrimes */
{
/* Using only trial prime divisors to the square root of N, generate all
   the primes <= *ulUB. If this would be < *nPrimes, continue to the
   (*nPrimes)th prime. The values of the primes are stored in the
   array ulPrime, with markers ulPrime[0]=ulPrime[*nPrimes + 1]=0.

   Before this function is called, the array ulPrime must have been
   previously declared and allocated with sufficient storage for this
   number of primes, plus two additional elements used to mark the
   beginning and end of the array. Thus at least 4*(pi(*ulUB) + 2) or
   4*(2 + *nPrimes) bytes must have been allocated in the calling code;
   this can be accomplished by malloc(4*(2 + *nPrimes)) and/or
   malloc(4*(2 + ceil(ldLi(*ulUB)))) in the calling code.

   The return values are *nPrimes=(the actual number of primes generated)
   and *ulUB=(the actual value of the largest, or (*nPrime)th, prime
   generated).

   *ulUB may not exceed the largest 32-bit prime, UINT32_MAX - 4,
   and *nPrimes may not exceed the number of 32-bit primes (203280221).
   As a minimum, the primes 2, 3, and 5 are generated, requiring five
   elements in ulPrime (20 bytes).

   Note that this function differs from the old vGenPrimes and the
   new vGenPrimesSieve in that the lower bounds of *nPrimes=6543 and
   *ulUB=65537 are no longer present.
*/

unsigned long ulN, ul, ul2, ulSqrt, ulDivisor;

ulPrime[0]=0;  /* 0th prime---just a marker */
ulPrime[1]=2;  /* 1st prime */
ulPrime[2]=3;  /* 2nd prime */
ulPrime[3]=5;  /* 3rd prime */
if(*ulUB < 7)*ulUB=5;
if(*nPrimes < 3)*nPrimes=3;
if(*ulUB > UINT32_MAX - 4)*ulUB=UINT32_MAX - 4;  /* Largest 32-bit prime */
if(*nPrimes > NUM_32BIT_PRIMES)*nPrimes=NUM_32BIT_PRIMES;
ul=4;
ulN=7;
while(1)
  {
  ul2=2;
  ulSqrt=ceil(sqrt(ulN + 0.5));  /* faster than integer sqrt */
  while(1)
    {
    ulDivisor=ulPrime[ul2++];
    if(ulDivisor >= ulSqrt)
      {
      ulPrime[ul++]=ulN;
      break;
      }
    if(ulN % ulDivisor == 0)break;
    }
  ulN += 2;
  if((ulN > *ulUB) && (ul > *nPrimes))break;  /* post-increments */
  }
ulPrime[ul]=0;  /* end marker */
*nPrimes=ul-1;
*ulUB=ulPrime[ul-1];

return;
}
/**********************************************************************/
void vGenPrimes16(void)
{
/* Generate from scratch all the primes < 2^16 + 2. There are 6543 such
   primes, having a sum of 202353624. The array of primes ulPrime16 is
   global (static) to this code, and must be accessed from the calling
   code by means of the global declaration "extern unsigned long
   ulPrime16[]". Do not allocate it or otherwise declare ulPrime16
   in the calling code. */

unsigned long nPrimes, ulUB;

if(iPrime16Initialized)return;
nPrimes=6543;
ulUB=65537UL;
vGenPrimesDiv(ulPrime16, &nPrimes, &ulUB);
if(ulPrime16[6543]==65537UL)
  iPrime16Initialized=1;
else
  {
  fprintf(stderr,
    "\n ***FATAL ERROR: ulPrime16 initialization failed in "
    "vGenPrimes16().\n");
  exit(EXIT_FAILURE);
  }

return;
}
/**********************************************************************/
void vGenPrimesSieve(unsigned long *ulPrime, unsigned long *nPrimes,
  unsigned long *ulUB)
{
/* Using the Sieve of Eratosthenes, generate all the primes <= *ulUB.
   If this would be < *nPrimes, continue to the (*nPrimes)th prime.
   Before this function is called, the array ulPrime must have been
   previously declared and allocated with sufficient storage for this
   number of primes, plus two additional elements used to mark the
   beginning and end of the array. Thus at least 4*(pi(*ulUB) + 2) or
   4*(2 + *nPrimes) bytes must have been allocated in the calling code;
   this can be accomplished by malloc(4*(2 + *nPrimes)) and/or
   malloc(4*(2 + ceil(ldLi(*ulUB)))) in the calling code.

   The return values are *nPrimes=(the actual number of primes generated)
   and *ulUB=(the actual value of the largest, or (*nPrime)th, prime
   generated).

   *ulUB may not exceed the largest 32-bit prime, UINT32_MAX - 4,
   and *nPrimes may not exceed the number of 32-bit primes (203280221).
   Minimum values are also enforced: *ulUB >= 65537 and *nPrimes >= 6543.

   This algorithm bootstraps some base primes by first calling
   vGenPrimesDiv, which does not use sieving, to establish a base set.
   An array allocated for use as a sieve is then used to generate
   succeeding blocks of primes by assigning successive odd integers to
   the elements of the sieving array. The base primes, bootstrapped by
   use of trial divisors, are required for sieving the first block. Each
   sieving then generates more than enough new primes to sieve the
   succeeding block.
*/

unsigned char *uchSieve;
unsigned long nPrimes0, ulLB0, ulUB0, ulPMA, ulSpan, ulOffset,
  ulOffsetMax, ulN, ulSieveBytes, ulLB0In, ulUB0In, nP;
long double ldSB, ldSB1, ldSB2, lognP;

/* Adjust the bounds. */

if(*ulUB > UINT32_MAX - 4)*ulUB=UINT32_MAX - 4;  /* Largest 32-bit prime */
if(*nPrimes > NUM_32BIT_PRIMES)*nPrimes=NUM_32BIT_PRIMES;
if(*ulUB < 65537UL)*ulUB=65537UL;
if(*nPrimes < 6543)*nPrimes=6543;

/* Allocate memory for the sieving array. The size in bytes needs to
   be half the size of the interval to be sieved; a smaller size
   will cause iterations of the sieving process. */

ulPMA=__ulPhysicalMemoryAvailable() & 0xFFFFFFFE;  /* ulPMA is even */
ulSieveBytes=ulPMA/2;  /* Use no more than half the available memory */
/* Use even less if this exceeds half the projected upper bound */
ldSB1=(*ulUB + 1)/2 + 1;
/* The following formulas, due to J. B. Rosser (1983) and G. Robin (1983),
   provide upper bounds for the (*nPrimes)th prime. See "The new book
   of prime number records," Paulo Ribenboim (Springer, 1995). */
nP=*nPrimes;
if(nP > 6543)
  {
  lognP=logl(nP);
  if(nP < 7022)
    ldSB2=nP*(lognP + logl(lognP) + 8)/2 + 2;
  else
    ldSB2=nP*(lognP + logl(lognP) - 0.9385)/2 + 2;
  }
if(ldSB2 > ldSB1)ldSB=ldSB2; else ldSB=ldSB1;
if(ldSB < ulSieveBytes)ulSieveBytes=ceill(ldSB);
uchSieve=(unsigned char *)malloc(ulSieveBytes);
while(!uchSieve)
  {
  ulSieveBytes=ulSieveBytes/2;
  if(ulSieveBytes < 32000)
    {
    fprintf(stderr,
      "\n ERROR: uchSieve malloc failed in vGenPrimesSieve.\n");
    exit(EXIT_FAILURE);
    }
  uchSieve=(unsigned char *)malloc(ulSieveBytes);
  }
if(!uchSieve)
  {
  fprintf(stderr, "\n ERROR: uchSieve malloc failed in vGenPrimesSieve.\n");
  exit(EXIT_FAILURE);
  }
ulSpan=2*ulSieveBytes-2;  /* ulSpan is even */

/* Bootstrap the primes necessary for the initial sieve (to at least
   the next prime beyond the square root of the first sieve upper bound). */

nPrimes0=0;
ulUB0=1442 + ceil(sqrt(3 + 2*ulSieveBytes));  /* No gap > 1442 below 1e18 */
ulUB0=ulUB0 + (ulUB0 & 1);
vGenPrimesDiv(ulPrime, &nPrimes0, &ulUB0);

if((nPrimes0 >= *nPrimes) && (ulUB0 >= *ulUB))
  {
  *nPrimes=nPrimes0;
  *ulUB=ulUB0;
  goto DONE_vGP;
  }

ulLB0=3;  /* Bootstrap primes will be regenerated---avoids complications */
nPrimes0=1;  /* Even prime 2 is an exception */
while(1)
  {
  if(ulLB0 + (uint64_t)ulSpan > (uint64_t)UINT32_MAX - 4)
    {                      /* overflow trap */
    ulUB0=UINT32_MAX - 4;
    if(ulUB0==ulLB0)
      {
      nPrimes0++;
      *nPrimes=nPrimes0;
      *ulUB=ulUB0;
      break;
      }
    if(ulUB0 < ulLB0)
      {
      *nPrimes=nPrimes0;
      *ulUB=ulUB0;
      break;
      }
    ulSpan=ulUB0 - ulLB0;
    }
  else  /* normal case */
    {
    ulUB0=ulLB0 + ulSpan;
    ulOffsetMax=ulSpan/2;
    }
  ulLB0In=ulLB0;
  ulUB0In=ulUB0;
  vSieve(uchSieve, &ulLB0, &ulUB0, ulPrime);
  if((ulLB0In != ulLB0) || (ulUB0In != ulUB0))
    {
    fprintf(stderr, "\n ERROR, Bounds anomaly in call to vSieve:");
    fprintf(stderr, "\n LB==> %lu to %lu    UB==> %lu to %lu\n",
      ulLB0In, ulLB0, ulUB0In, ulUB0);
    exit(EXIT_FAILURE);
    }
  ulOffset=0;
  ulN=ulLB0;
  while(1)
    {
    if(uchSieve[ulOffset])ulPrime[++nPrimes0]=ulN;
    ulN += 2;  /* ulN is odd */
    if((ulN > *ulUB) && (nPrimes0 >= *nPrimes))
      {
      *nPrimes=nPrimes0;
      *ulUB=ulPrime[nPrimes0];
      goto DONE_vGP;
      }
    ulOffset++;
    if(ulOffset > ulOffsetMax)break;
    }
  ulLB0=ulN;
  }

DONE_vGP:

free(uchSieve);
return;
}
/**********************************************************************/
void vSieve(unsigned char *uchSieve, unsigned long *ulLB,
  unsigned long *ulUB, unsigned long *ulPrime)
{
/* A deprecated routine which now reverts to vSieveForDivisors. The
   reference array ulPrime is no longer needed, and may be replaced
   by a dummy variable in the call. The original routine is still
   coded below (conditionally excluded by an #if 0) for reference,
   but appears to offer little if any advantage. */

vSieveForDivisors(uchSieve, *ulLB, *ulUB, ceill(sqrtl(*ulUB + 0.5L)));
return;

#if 0

/* Applies the Sieve of Eratosthenes to the interval between the odd
   positive integers *ulLB and *ulUB (*ulLB >= 3; *ulUB <= 2^32-1;
   *ulLB <= *ulUB). Elements of the sieving array uchSieve represent
   successive odd integers from *ulLB to *ulUB inclusive; uchSieve is
   assumed pre-allocated with at least 1 + (*ulUB - *ulLB) elements
   (unsigned chars). Sieving is carried out by elements of ulPrime up
   to ceil(sqrt(*ulUB)); ulPrime is assumed previously populated with
   primes up to the next prime beyond ceil(sqrt(*ulUB)), with
   ulPrime[0]=0 (or 1) and ulPrime[i]=the ith prime.

   Upon return, the prime elements in uchSieve will be represented
   by 1's and the composite elements by 0's.
*/

unsigned long ul, ulSqrt, ulUBLocal, ulDiv, ulRem, ulOffset, ulOffsetMax;

/* Must have *ulLB <= 3 <= *ulUB <= 2^32-1 with *ulLB and *ulUB odd. */

if(*ulUB < *ulLB)return;
if(!(*ulLB & 1))(*ulLB)++;
if(!(*ulUB & 1))(*ulUB)--;
if(*ulUB < *ulLB)return;

ulOffsetMax=(*ulUB - *ulLB)/2;
for(ul=0; ul <= ulOffsetMax; ul++)uchSieve[ul]=1;
if(*ulLB==1)uchSieve[0]=0;
ulSqrt=floorl(sqrtl(*ulUB + 0.5L));
ul=2;     /* evens already sieved out; start with divisor 3 */
while(1)  /* Sieve by ulPrime[ul++] in each loop */
  {
  ulDiv=ulPrime[ul++];
  if(ulDiv==0)
    {
    fprintf(stderr, "\n ERROR: ulDiv=0 in vSieve for ul=%lu", ul-1);
    fprintf(stderr, "\n        *ulLB=%lu  *ulUB=%lu\n", *ulLB, *ulUB);
    fprintf(stderr, "\n        ulPrime[%lu]=%lu\n", ul-2, ulPrime[ul-2]);
    exit(EXIT_FAILURE);
    }
  if(ulDiv > ulSqrt)break;
  ulRem = *ulLB % ulDiv;
  if(ulRem==0)
    ulOffset = 0;
  else if(ulRem==2*(ulRem/2))           /* ulRem even */
    ulOffset = (2*ulDiv - ulRem)/2;     /* Assumes ulDiv < INT32_MAX */
  else
    ulOffset = (ulDiv - ulRem)/2;       /* ulRem odd */
  if(ulDiv >= *ulLB)ulOffset += ulDiv;  /* do not mark the prime itself */
  while(ulOffset <= ulOffsetMax)
    {
    uchSieve[ulOffset]=0;
    ulOffset += ulDiv;
    }
  }

return;

#endif
}
/**********************************************************************/
void vSieveForDivisors(unsigned char *uchPrime, unsigned long ulStart,
  unsigned long ulEnd, unsigned long ulMaxDiv)
{
/* Secondary sieve, for intervals from as little as ulStart=3 to
 * as much as ulEnd=4294967295. This is intended primarily for
 * generating trial divisors > 2^16, for use in primary sieving of
 * intervals exceeding the unsigned long range (ullEnd > 2^32).
 * It can function as a "partial" sieve, with trial divisors cut
 * off at ulMaxDiv for speed. This would leave a few composites
 * marked as primes; if they are used as trial divisors in a
 * primary sieve, this does not affect the final result, but may
 * result in improved performance overall.
 *
 * The interval [ulStart, ulEnd], with odd positive integer bounds,
 * is sieved for primes, using the algorithm of Eratosthenes and trial
 * divisors up to sqrt(ulEnd); i.e., for each prime q <= sqrt(ulEnd),
 * all integer multiples mq, ulStart <= mq <= ulEnd and m > 1, are
 * marked as composite. The sieved interval is represented by the byte
 * array uchPrime2, each byte representing an odd number---
 * uchPrime2[0]=ulStart and uchPrime[ulMaxIndex2]=ulEnd. For a given
 * prime divisor q, the offset ulIndex of the first odd mq in the
 * interval is computed, then the positions of successive odd multiples
 * are found simply by repeated addition of q.
 *
 * ulMaxDiv specifies an optional maximum value for the trial divisors
 * used within the secondary sieve. If ulMaxDiv is 0 or 1, all prime trial
 * divisors < 2^16 are available; otherwise, the sieving process is
 * terminated with the largest prime not exceeding ulMaxDiv. Such
 * termination has the effect of leaving some composites (those with
 * all prime factors exceeding ulMaxDiv) marked as primes. Experience
 * indicates that a proper choice of ulMaxDiv will save more time in
 * the secondary sieve than is lost by the presence of the spurious
 * composites in the primary sieve (their presence has no effect on
 * the final result, other than the time loss).
 *
 * uchPrime is the (unsigned character) array encoding the primes;
 *   byte 0 represents ulStart; byte (ulEnd - ulStart)/2 represents
 *   ulEnd; the values are either 0 (composite) or 1 (prime or almost
 *   prime). The proper space must have been allocated for uchPrime
 *   prior to calling this routine.
 * ulStart and ulEnd are odd positive integers, 2 < ulStart <= ulEnd < 2^32.
 * ulMaxDiv is the maximum trial divisor to be used in sieving. A
 *   value of 0 or 1 is treated as the largest 32-bit prime, 4294967291.
 *   If ulMaxDiv < sqrt(ulEnd), some of the "primes" returned may be
 *   composites with all prime factors > ulMaxDiv ("almost" primes).
 * ulPrime16 is an array of unsigned longs representing the primes
 *   to 65537 inclusive (all 16-bit primes plus one overflow value).
 *   Its use is internal, and the end user need not be aware of its
 *   existence. It provides the trial divisors for sieving.
 */

unsigned long ul=2, ulMaxIndex, r, ulIndex, ulDivisor, ulSqrt, ulQuot;

if(ulStart > ulEnd)return;
if((ulStart & 1)==0)ulStart++;
if((ulEnd   & 1)==0)ulEnd--;
ulMaxIndex=(ulEnd - ulStart)/2;
if(ulMaxDiv < 2)ulMaxDiv=MAX_32BIT_PRIME;

vGenPrimes16();  /* Will actually be done only once */
ulSqrt=floor(sqrt(ulEnd + 0.5));

/* Initially, all the odd integers in [ulStart, ulEnd] are assumed
   prime. */

for(ulIndex=0; ulIndex <= ulMaxIndex; ulIndex++)uchPrime[ulIndex]=1;
if(ulStart==1)uchPrime[0]=0;
while(1)
  {
  ulDivisor=ulPrime16[ul++];
  if(ulDivisor==MIN_32BIT_PRIME)break;  /* 65537 = trial divisor overflow */
  if(ulDivisor > ulSqrt)return;  /* surpassed sqrt of upper bound */
  if(ulDivisor > ulMaxDiv)break;

  /* Calculate the offset in uchPrime of the first odd multiple of
     ulDivisor in the sieve interval. */

  r=ulStart % ulDivisor;
  ulIndex=r?((r&1)?((ulDivisor-r)>>1):(((ulDivisor<<1)-r)>>1)):0;
#if 0
  /* The preceding statement is optimized for speed; it assumes
   * ulDivisor < INT32_MAX. Its less cryptic equivalent follows. */

  if(r==0)
    ulIndex=0;
  else if(r & 1 == 1)               /* if r is odd */
    ulIndex=(ulDivisor - r)/2;
  else                              /* if r is even and not zero */
    ulIndex = (2*ulDivisor - r)/2;
#endif
  /* Now avoid striking out the prime divisor itself. */

  if(ulDivisor==ulStart + 2*ulIndex)ulIndex += ulDivisor;

  while (1)
    {
    if(ulIndex > ulMaxIndex)break;
    if(uchPrime[ulIndex])uchPrime[ulIndex]=0;
    ulIndex += ulDivisor;
    }
  }

return;
}
/**********************************************************************/
void vSieveULL(unsigned char *uchPrime, uint64_t ullStart, uint64_t ullEnd)
{
/* Primary sieve, for intervals from as little as ullStart=1 to
 * as much as ullEnd=2^64-1 (approximately 1.8e19).
 *
 * uchPrime is the array representing the (successive odd integers of)
 *   the sieved interval (integer 2 is not represented or detected).
 *   It must be pre-allocated by the calling routine with at least
 *   (ullEnd - ullStart)/2 + 1 bytes of storage.
 * ullStart and ullEnd are odd positive integers,
 *   0 <= ullStart <= ullEnd < UINT64_MAX (approximately 1.8*10^19);
 *   however, ullEnd - ullStart must be < 2*UINT32_MAX - 1.
 *
 * The interval [ullStart, ullEnd], with odd positive integer bounds,
 * is sieved for primes, using the algorithm of Eratosthenes and trial
 * divisors up to sqrt(ullEnd); i.e., for each prime q <= sqrt(ullEnd),
 * all integer multiples mq, ullStart <= mq <= ullEnd and m > 1, are
 * marked as composite. The sieved interval is represented by the byte
 * array uchPrime, each byte representing an odd number---
 * uchPrime[0]=ullStart and uchPrime[ulMaxIndex]=ullEnd. For a given
 * prime divisor q, the offset ulIndex of the first odd mq in the
 * interval is computed, then the positions of successive odd multiples
 * are found simply by repeated addition of q.
 *
 * If trial divisors exceeding 2^16 are required (ullEnd > 2^32), they
 * are generated using a secondary sieve, which is tuned to return a
 * few composite trial divisors (this may improve efficiency), and a
 * call to vSieveForDivisors.
 *
 */

unsigned char *uchPrime2;
long slSS;
unsigned long ul, ulMaxIndex, r, ulIndex, ulDivisor, ulMaxIndex2, ulSqrt2;
uint64_t ullOffset, ull1, ull2, ullQuot;

if(ullStart > ullEnd)return;
if((ullStart & 1)==0)ullStart++;
if((ullEnd   & 1)==0)ullEnd--;
if(ullEnd - ullStart > __ULL(2)*UINT32_MAX - __ULL(2))
  {
  fprintf(stderr, "\n ERROR: Sieving interval too large in vSieveULL.\n");
  exit(EXIT_FAILURE);
  }
ulMaxIndex=(ullEnd - ullStart)/2;

/* Initially, all the odd integers (except 1) in [ullStart, ullEnd]
   are assumed prime. */

for(ulIndex=0; ulIndex <= ulMaxIndex; ulIndex++)uchPrime[ulIndex]=1;
if(ullStart==1)uchPrime[0]=0;

/* Main loop, for divisors 3 through 65521. */

vGenPrimes16();
ul=2;
ulSqrt2=ulSqrt(ullEnd) + 1;
while(1)
  {
  ulDivisor=ulPrime16[ul++];
  if(ulDivisor==MIN_32BIT_PRIME)break;    /* divisors exceed ulPrime16 */
  if(ulDivisor > ulSqrt2)return;  /* reached sqrt of upper bound */

  /* Calculate the offset in uchPrime of the first odd multiple of
     ulDivisor in the sieve interval. */

  r=ullStart % ulDivisor;

  if(r==0)
    ulIndex=0;
  else if(r & 1 == 1)  /* if r is odd */
    ulIndex=(ulDivisor - r)/2;
  else  /* if r is even and not zero */
    ulIndex = (2*ulDivisor - r)/2;

  /*
   * Faster but cryptic version of the above, for speed; both assume
   * ulDivisor < UINT32_MAX/2.
   *
   * ulIndex=r?((r&1)?((ulDivisor-r)>>1):(((ulDivisor<<1)-r)>>1)):0;
   *
   */

  /* Avoid striking out the prime divisor itself */

  if(ulDivisor==ullStart + 2*ulIndex)ulIndex += ulDivisor;

  while (1)
    {
    if(ulIndex > ulMaxIndex)break;
    if(uchPrime[ulIndex])uchPrime[ulIndex]=0;
    ulIndex += ulDivisor;
    }
  }

ull1=MIN_32BIT_PRIME;  /* this is 65537 */

/* If we arrive here, the prime divisors have exceeded 2^16, due to
 * ullEnd exceeding 2^32. A secondary sieve is called to generate
 * the necessary trial divisors. For efficiency reasons, these may
 * include some composite integers as well. Allocate memory for
 * the secondary sieve.
 */

slSS=__ulMem()/2;
slSS=1000000UL*(slSS/1000000UL);
if(slSS > 100000000L)slSS=100000000UL;
while(1)
  {
  if(slSS < 100000L)
    {
    fprintf(stderr, "\n ERROR: Insufficient free memory for uchPrime2.");
    exit(EXIT_FAILURE);
    }
  uchPrime2=(unsigned char *)malloc(slSS);
  if(uchPrime2)break;
  slSS=slSS - 1000000UL;
  }

while(1)
  {
  if(ull1 > MAX_32BIT_PRIME)break;  /* this is 4294967291 */
  if(ull1 > ulSqrt2)break;  /* reached sqrt of upper bound */
  ull2=ull1 + __ULL(2)*slSS - 2;
  if(ull2 > MAX_32BIT_PRIME)ull2=MAX_32BIT_PRIME;

  /* Initialize the secondary sieve array. Call the secondary sieve
   * with an upper limit of 1000 on the secondary trial divisors; this
   * will cause some composites (those with no prime factors < 1000)
   * to be returned as trial divisors by the secondary sieve, but the
   * hope is that the few spurious composites will be offset by the
   * time saved in skipping more than 97 % of the secondary trial
   * divisors. The presence of composite trial divisors has no effect
   * on the eventual value of the uchPrime array.
   */

  ulMaxIndex2=(ull2 - ull1)/2;
  for(ulIndex=0; ulIndex <= ulMaxIndex2; ulIndex++)
    uchPrime2[ulIndex]=1;
  if(ull1==1)uchPrime2[0]=0;
  vSieveForDivisors(uchPrime2, ull1, ull2, 1000);

  for(ul=0; ul <= ulMaxIndex2; ul++)
    {
    if(uchPrime2[ul]==0)continue;  /* not a prime divisor */
    ulDivisor=ull1 + 2*ul;
    if(ulDivisor > ulSqrt2)break;  /* reached sqrt of upper bound */

    /* Calculate the offset in uchPrime of the first odd multiple of
       ulDivisor in the sieve interval. */

    r=ullStart % ulDivisor;

    if(r==0)
      ullOffset=0;
    else if(r & 1 == 1)  /* if r is odd */
      ullOffset=(ulDivisor - r)/2;
    else  /* if r is even and not zero */
      ullOffset=(__ULL(2)*ulDivisor - r)/2;  /* Avoids UL overflow */

    /* Avoid striking out the prime divisor itself */

    if(ulDivisor==ullStart + 2*ullOffset)ullOffset += ulDivisor;

    while (1)
      {
      if(ullOffset > ulMaxIndex)break;
      if(uchPrime[(long)ullOffset])uchPrime[(long)ullOffset]=0;
      ullOffset += ulDivisor;
      }
    }
  ull1=ull2 + 2;  /* Proceed to next block */
  }

free(uchPrime2);
return;
}
/**********************************************************************/
int iIsPrime32(unsigned long ulN)
{
/* Returns 1 if ulN is prime, zero otherwise. No sieving is used. The
   routine simply checks for prime divisors up to the sqrt of ulN. */

unsigned long ulSqrtN, ul=2, ulDiv;

if((ulN < 3) || ((ulN & 1)==0))return(ulN==2 ? 1 : 0);

if(!iPrime16Initialized)vGenPrimes16();
ulSqrtN=ulSqrt(ulN);

while(1)
  {
  ulDiv=ulPrime16[ul++];
  if(ulDiv > ulSqrtN)return(1);
  if(ulN%ulDiv==0)return(0);
  }
}
/**********************************************************************/
#ifdef __GMP__
/**********************************************************************/
int iIsPrime64(uint64_t ullN, unsigned long ulMaxDivisor)
{
/* Returns 1 if ullN is prime, zero otherwise. No sieving is used.
   The routine checks for prime divisors up to the smaller of the
   sqrt of ullN or ulMaxDivisor. If no prime divisor is found, and
   N > ulMaxDivisor^2 exceeds 2^32, the strong BPSW primality test
   is invoked. If 0 or 1 is specified for ulMaxDivisor, a default
   value of 1000 is used. */

static long        iFirst=1;
unsigned long	   ulSqrtN, ul=2, ulDiv;
mpz_t              mpzN;

if((ullN < 3) || ((ullN & 1)==0))return(ullN==2 ? 1 : 0);
if(!iPrime16Initialized)vGenPrimes16();

if(ulMaxDivisor < 2)ulMaxDivisor=1000UL;
if(ulMaxDivisor > 65536UL)ulMaxDivisor=65536UL;

ulSqrtN=ulSqrt(ullN);
while(1)
  {
  ulDiv=ulPrime16[ul++];
  if(ulDiv > ulSqrtN)return(1);
  if(ulDiv > ulMaxDivisor)break;
  if(ullN%ulDiv==0)return(0);
  }

/* If there are no small prime divisors, we use the strong BPSW test
   for primality. */

if(iFirst)
  {
  mpz_init2(mpzN, 512);
  iFirst=0;
  }
__mpz_set_ull(mpzN, ullN);
return(iBPSW(mpzN));
}
/**********************************************************************/
int iPrP(mpz_t mpzN, unsigned long ulNMR, unsigned long ulMaxDivisor)
{
/* Returns 1 if mpzN is a probable prime according to the strong
 * modified Baillie-PSW test.
 *
 * Returns 0 if mpzN is definitely composite (and mpzN > 1).
 * Returns 0 if mpzN is not prime (and mpzN < 2).
 *
 * ulNMR indicates the total number of Miller's tests to be used;
 * the default is one (with B=2). If ulNMR > 1, ulNMR - 1 extra
 * Miller's tests will be carried out (ulNMR <= 6543), along with
 * an additional floor(ulNMR/5) extra strong Lucas tests.
 *
 * ulMaxDivisor specifies the upper bound for small prime trial divisors
 * If ulMaxDivisor < (# of binary digits in N) is specified, the
 * small prime divisors checked in Miller's test are used (up to
 * qMax = # of binary digits in N).
 *
 * This test consists of the strong Baillie-PSW test enhanced as
 * follows: (1) The domain of trial divisors may be altered;
 * (2) The number of Miller's tests may be increased by specifying
 * ulNMR > 1; (3) if the total number of tests ulNMR > 4, an
 * additional extra strong Lucas test is performed after each five
 * Miller's tests.
 *
 */

int iComp2;
unsigned long qMax;
unsigned long ulDiv, ul;

/* First eliminate all N < 3 and all even N. */

iComp2=mpz_cmp_si(mpzN, 2);
if(iComp2 < 0)return(0);
if(iComp2==0)return(1);
if(mpz_even_p(mpzN))return(0);

/* Any small prime divisors will be found in iMiller. */

qMax=mpz_sizeinbase(mpzN, 2);
if(ulMaxDivisor > qMax)
  {
  ulDiv=ulPrmDiv(mpzN, ulMaxDivisor);
  if(ulDiv==1)return(1);
  if(ulDiv > 1)return(0);
  }

if(iMiller(mpzN, 2)==0)return(0);  /* Carry out Miller's test with base 2 */

/* Now N is a prime, or a base-2 strong pseudoprime with no small
   prime divisors. Apply the strong Lucas-Selfridge primality test. */

if(iStrongLucasSelfridge(mpzN)==0)return(0);

/* The following is in addition to the strong Baillie-PSW test.
   Additional Miller's tests (numbering ulNMR - 1) can be
   mandated, a strategy rumored to be in use by Mathematica
   In addition, after each five Miller's tests, we perform
   an extra strong Lucas test. */

if(ulNMR < 2)return(1);
if(ulNMR > 6543)ulNMR=6543;
if(!iPrime16Initialized)vGenPrimes16();
for(ul=2; ul <= ulNMR; ul++)
  {
  if(iMiller(mpzN, ulPrime16[ul])==0)return(0);
  if(ul%5==0)
    if(iExtraStrongLucas(mpzN, ulPrime16[ul/5 + 1])==0)return(0);
  }

return(1);
}
/**********************************************************************/
unsigned long ulPrmDiv(mpz_t mpzN, unsigned long ulMaxDivisor)
{
/* Returns the smallest proper prime divisor (p <= ulMaxDivisor) of N.

   If N < 2, return 0.
   If N is prime and "small", return 1. "Small" means N < approximately
     ulMaxDivisor^2.
   If N is prime and "large", return 0. "Large" means N > approximately
     ulMaxDivisor^2.
   If N is composite and its smallest prime divisor p <= ulMaxDivisor,
     return p.
   If N is composite but its smallest prime divisor p > ulMaxDivisor,
     return 0. In this case N will be "large", as above.

   A return of 0 indicates "no conclusion"; N might be < 2
   (questionable input), or N might be "large" (either prime
   or composite) and have no prime divisor p <= ulMaxDivisor.

   A return of 1 indicates that N is a "small" prime.

   A return > 1 indicates that N is composite, and the
   returned value is the smallest proper prime divisor of N.

   If ulMaxDivisor is zero or one, a default value of 1000 is used.
*/

int iComp2, i;
unsigned long ul, ulDiv, ulBase;
static mpz_t mpzSqrt;
static int d[8]={1,7,11,13,17,19,23,29}, iFirst=1;

/* First eliminate all N < 3 and all even N. */

iComp2=mpz_cmp_si(mpzN, 2);
if(iComp2 < 0)return(0);  /* No conclusion (N < 2) */
if(iComp2==0)return(1);   /* Prime (N=2) */
if(mpz_even_p(mpzN))return(2);  /* Composite (even) */

if(!iPrime16Initialized)vGenPrimes16();
if(ulMaxDivisor < 2)ulMaxDivisor=1000UL;
if(ulMaxDivisor > MAX_32BIT_PRIME)ulMaxDivisor=MAX_32BIT_PRIME;

if(iFirst)
  {
  mpz_init(mpzSqrt);
  iFirst=0;
  }
mpz_sqrt(mpzSqrt, mpzN);

ul=2;  /* first trial divisor will be 3, the 2nd prime */
while(1)
  {
  ulDiv=ulPrime16[ul++];
  if(ulDiv > ulMaxDivisor)return(0);  /* No conclusion */
  if(ulDiv > 65536UL)break;
  if(mpz_cmp_ui(mpzSqrt, ulDiv) < 0)return(1);  /* Prime */
  if(mpz_divisible_ui_p(mpzN, ulDiv))return(ulDiv);  /* Composite */
  }

/* Once the 16-bit divisors have been exhausted, use trial divisors of
   the form 30n + d, where d=1,7,11,13,17,19,23,29. */

ulBase=30*(ulDiv/30);
while(1)
  {
  for(i=0; i < 8; i++)
    {
    ulDiv=ulBase + d[i];
    if(ulDiv > ulMaxDivisor)return(0);  /* No conclusion */
    if(mpz_cmp_ui(mpzSqrt, ulDiv) < 0)return(1);  /* Prime */
    if(mpz_divisible_ui_p(mpzN, ulDiv))return(ulDiv);  /* Composite */
    }
  ulBase += 30;
  }

return(0);  /* No conclusion */
}
/**********************************************************************/
int iMillerRabin(mpz_t mpzN, const long iB)
{
return(iMiller(mpzN, iB));
}
/**********************************************************************/
int iMiller(mpz_t mpzN, long iB)
{
/* Test N for primality using the Miller's strong probable prime
   test with base B. See Gary Miller's famous paper ("Riemann's
   hypothesis and tests for primality," Journal of Computation and
   System Science, 1976, Volume 13, pp 300-317).

   Returns 1 if N is a prime or a base-B strong probable prime.
   Returns 0 if N is definitely not a prime (composite or < 2).

   NOTE 1: Some will not regard this as a "pure" Miller's test with
   base B, since certain adjustments are made, prior to applying the
   algorithm, in order to work around invalid input values and
   improve performance:

   1) N < 3 and even N are screened out first.
   2) Multiples of the small primes (to qMax=# of binary digits in N)
      are returned as composite.
   3) Values of B < 2 are replaced by B=2.
   4) If N divides B exactly, B is replaced by B+1.

   If such adjustments are not made, a third return value (e.g., -1)
   must be allowed, indicating invalid input and an indeterminate result,
   and complicating the calling source code.

   NOTE 2: Not all authorities agree exactly on the conditions for
   Miller's test. Some allow B < 2 (Rosen, "Elementary number theory and
   its applications," 3rd ed., 1993, pp. 195-200), although there are good
   reasons to exclude such values. On the other hand, some require
   1 < B < N (Ribenboim, "The new book of prime number records,"
   3rd ed., 1996, pp. 113-116, 143-146). As far as I know, no one
   allows N to divide B, as this produces "false negatives"; e.g.,
   N=3 and B=6 fails Miller's test, thus indicating N=3 as composite.
   In practice, there appears to be no good reason to use any B < 2,
   and in fact its value is almost always chosen to be a small
   (positive) prime number. Furthermore, in my opinion, refusing to
   first screen out even values of N and N < 3 gratuitously adds
   unnecessary complication to the test.
*/

static long iFirst=1;
static mpz_t mpzB, mpzNm1, mpzd, mpzRem, mpzSqrt, mpzOne;
long iComp2, iBits, s, j, q, digits;
unsigned long qMax;

/* Allocate the static variables used in Miller's test. */

if(iFirst)
  {
  mpz_init(mpzB);  /* Never needs more than one limb */
  iBits=mp_bits_per_limb*(1 + mpz_size(mpzN));
  if(iBits < 512)iBits=512;
  mpz_init2(mpzNm1, iBits);
  mpz_init2(mpzOne, iBits);
  mpz_set_si(mpzOne, 1);
  mpz_init2(mpzd, iBits);
  mpz_init2(mpzRem, 2*iBits);  /* must contain products */
  if(!iPrime16Initialized)vGenPrimes16();
  iFirst=0;
  }

/* First take care of all N < 3 and all even N. */

iComp2=mpz_cmp_si(mpzN, 2);
if(iComp2 < 0)return(0);        /* N < 2 is by convention not prime */
if(iComp2==0)return(1);         /* N=2 is prime */
if(mpz_even_p(mpzN))return(0);  /* Even N > 2 is composite */

/* Try small prime divisors from 3 to an UB qMax determined by the size
   of N (qMax >= 31). */

mpz_sqrt(mpzSqrt, mpzN);
qMax=mpz_sizeinbase(mpzN, 2);  /* Number of binary digits in N */
if(qMax < 36)qMax=36;
j=2;  /* First trial divisor is 3, the second prime */
while(1)
  {
  q=ulPrime16[j++];
  if(q > qMax)break;
  if(mpz_cmp_si(mpzN, q)==0)return(1);
  if(mpz_cmp_si(mpzSqrt, q) < 0)return(1);
  if(mpz_divisible_ui_p(mpzN, q))return(0);
  }

/* Check for valid input. Miller's test requires B > 1, and N must not
   divide B exactly. Choose B=2 and B<--B+1 if these problems arise.
   This is technically a deviation from the pure Miller's test, but
   avoids the necessity of handling an error return of -1. */

if(iB < 2)iB=2;
mpz_set_si(mpzB, iB);
if(mpz_divisible_p(mpzB, mpzN))mpz_add_ui(mpzB, mpzB, 1);

/* Now compute d and s, where d is odd and N - 1 = (2^s)*d. */

mpz_sub_ui(mpzNm1, mpzN, 1);
s=mpz_scan1(mpzNm1, 0);
mpz_tdiv_q_2exp(mpzd, mpzNm1, s);

/* Now proceed with the Miller's algorithm. First, if B^d is
   congruent to 1 mod N, N is a strong probable prime to base B. */

mpz_powm(mpzRem, mpzB, mpzd, mpzN);
if(mpz_cmp_si(mpzRem, 1)==0)return(1);

/* Now calculate B^((2^j)*d), for j=0,1,...,s-1 by successive
   squaring. If any of these is congruent to -1 mod N, N is a
   sprp base B. Start with j=0 and B^d, already computed.
   Miller's test uses repeated modular squaring in place of repeated
   modular exponentiation for speed (squaring is an order of
   magnitude faster). */

if(mpz_cmp(mpzRem, mpzNm1)==0)return(1);  /* j=0 case */
for(j=1; j < s; j++)
  {
  mpz_mul(mpzRem, mpzRem, mpzRem);
  mpz_mod(mpzRem, mpzRem, mpzN);
  if(mpz_cmp(mpzRem, mpzNm1)==0)return(1);
  if(mpz_cmp(mpzRem, mpzOne)==0)return(0);
  }

return(0);
}
/**********************************************************************/
int iBPSW(mpz_t mpzN)
{
/* Returns 1 if N is a probable prime, that is, passes the primality
 * tests in this algorithm; in that case, N is prime, or a strong
 * Baillie-Pomerance-Selfridge-Wagstaff (Baillie-PSW or BPSW) pseudoprime
 * Returns 0 if N is definitely composite, or if N < 2.
 *
 * The strong Lucas-Selfridge test returns roughly 30 % as many
 * pseudoprimes as the standard test, at the price of an additiona
 * running time of roughly 10 %. Thus the strong Lucas-Selfridge test
 * appears to be more effective, and is the one employed here.
 *
 * Determines if N is a probable prime, using a version of the
 * algorithm outlined by Baillie, Pomerance, Selfridge, and Wagstaff
 * (ca. 1980). Values of N are tested successively as follows.
 *
 * (1) N < 2 is not prime. N=2 is prime. Even N > 2 are composite.
 * (2) Try small primes as trial divisors, up to qMax=the # of binary
 *     digits in N. This step is implicit here here as part of Miller's
 *     test.
 * (3) If there is small prime divisor, apply Miller's strong
 *     probable prime test with base 2. If N fails, it is definitely
 *     composite. If N passes, it is a prime or a strong pseudoprime
 *     to base 2.
 * (4) Apply the strong Lucas sequence test with Selfridge's parameters.
 *     If N fails the Lucas-Selfridge test, it is definitely composite
 *     (and a strong pseudoprime to base 2). If N passes the strong
 *     Lucas-Selfridge test, it is a strong Lucas probable prime (lprp),
 *     i.e., a prime or a strong Lucas-Selfridge pseudoprime.
 * (5) If N has passed all these tests, it is a strong BPSW probable
 *     prime---either prime, or a strong BPSW pseudoprime. In this event
 *     the relative frequency of primality is believed to be very close
 *     to 1, and possibly even equal to 1. With the aid of Pinch's tables
 *     of pseudoprimes, the author verified (May, 2005) that there exists
 *     no Baillie-PSW pseudoprime (either strong or standard) in N < 10^13.
 *     In January, 2007, with the aid of the present implementation and
 *     William Galway's table of pseudoprimes, Martin Fuller determined
 *     that no Baillie-PSW pseudoprime (standard or strong) exists for
 *     N < 10^15. More recently, using the author's code and a database of
 *     pseudoprimes prepared by Jan Feitsma, Jeff Gilchrist has determined
 *     (13 June 2009) that no Baillie-PSW pseudoprime (standard or strong)
 *     exists below 10^17. Furthermore, preliminary analysis by Gilchrist
 *     of Feitsma's database, further extended, indicates (24 October 2009)
 *     that no Baillie-PSW pseudoprime (standard or strong) exists below
 *     2^64 ~ 1.8446744e19 (double checking of this result is in progress).
 * (6) Note, however, that Carl Pomerance (1984) presented a heuristic
 *     argument that an infinite number of counterexamples exist to the
 *     standard BPSW test (and presumably to the strong BPSW test as well,
 *     based on similar reasoning), and even that (for sufficiently large x,
 *     dependent on t) the number of Baillie-PSW pseudoprimes < x exceeds
 *     x^(1-t), where t is an arbitrarily small pre-assigned positive number.
 *     Nevertheless, not a single Baillie-PSW pseudoprime has yet been found;
 *     consequently, the Baillie-PSW test carries an aura of dependability
 *     (justified or not) exceeding that of competing algorithms, such as
 *     multiple Miller's tests (the Miller-Rabin algorithm).
 *
 * In the unexpected event that no counterexample exists, this algorithm
 * (the strong BPSW test) would constitute a definitive fast certification
 * of primality with polynomial running time, O((log N)^3).
 *
 * References:
 *
 * o Arnault, Francois. The Rabin-Monier theorem for Lucas pseudoprimes.
 *   Math. Comp. 66 (1997) 869-881. See
 *   <http://www.unilim.fr/pages_perso/francois.arnault/publications.html>
 * o Baillie, Robert, and Samuel S. Wagstaff, Jr. Lucas pseudoprimes.
 *   Math. Comp. 35:152 (1980) 1391-1417. MR0583518 (81j:10005). See
 *   <http://mpqs.free.fr/LucasPseudoprimes.pdf>.
 * o Feitsma, Jan. The pseudoprimes below 10^16. June 2009. Available at
 *   <http://www.janfeitsma.nl/math/psp2/database>.
 * o Galway, William. The pseudoprimes below 10^15. 4 November 2002.
 *   Available at <http://oldweb.cecm.sfu.ca/pseudoprime/>.
 * o Gilchrist, Jeff. Pseudoprime enumeration with probabilistic
 *   primality tests. 13 June 2009. Available at
 *   <http://gilchrist.ca/jeff/factoring/pseudoprimes.htm>..
 * o Grantham, Jon. Frobenius pseudoprimes. Preprint (16 July 1998)
 *   available at <http://www.pseudoprime.com/pseudo1.ps>.
 * o Martin, Marcel. Re: Baillie-PSW - Which variant is correct?
 *   9 January 2004. See
 *   <http://groups.google.com/groups?hl=en&lr=&ie=UTF-8&oe=UTF-8&safe=off&selm=3FFF275C.2C6B5185%40ellipsa.no.sp.am.net>.
 * o Mo, Zhaiyu, and James P. Jones. A new primality test using Lucas
 *   sequences. Preprint (circa 1997).
 * o Pinch, Richard G. E. Pseudoprimes up to 10^13. 4th International
 *   Algorithmic Number Theory Symposium, ANTS-IV, Leiden, The
 *   Netherlands, 2--7 July 2000. Springer Lecture Notes in Computer
 *   Science 1838 (2000) 459-474. See
 *   <http://www.chalcedon.demon.co.uk/rgep/carpsp.html>.
 * o Pomerance, Carl. Are there counterexamples to the Baillie-PSW
 *   primality test? (1984) See <http://www.pseudoprime.com/dopo.pdf>.
 * o Pomerance, Carl, John L. Selfridge, and Samuel S. Wagstaff, Jr.
 *   The pseudoprimes to 25*10^9. Math. Comp. 35 (1980) 1003-1026. See
 *   <http://mpqs.free.fr/ThePseudoprimesTo25e9.pdf>.
 * o Ribenboim, Paulo. The new book of prime number records. 3rd ed.,
 *   Springer-Verlag, 1995/6, pp. 53-83, 126-132, 141-142 (note that on
 *   line 2, p. 142, "0 < r < s" should read "0 <= r < s").
 * o Weisstein, Eric W. Baillie-PSW primality test. See
 *   <http://mathworld.wolfram.com/Baillie-PSWPrimalityTest.html>.
 * o Weisstein, Eric W. Strong Lucas pseudoprime. See
 *   <http://mathworld.wolfram.com/StrongLucasPseudoprime.html>.
 *
 */

int iComp2;

/* First eliminate all N < 3 and all even N. */

iComp2=mpz_cmp_si(mpzN, 2);
if(iComp2 < 0)return(0);
if(iComp2==0)return(1);
if(mpz_even_p(mpzN))return(0);

/* Carry out Miller's test with base 2. This will also carry
   out a check for small prime divisors. */

if(iMiller(mpzN, 2)==0)return(0);

/* The rumored strategy of Mathematica could be imitated here by
 * performing additional Miller's tests. One could also carry out
 * one or more extra strong Lucas tests. See the routine iPrP for
 * an implementation.
 *
 * Now N is a prime, or a base-2 strong pseudoprime with no prime
 * divisor < 37. Apply the strong Lucas-Selfridge primality test.
 */

return(iStrongLucasSelfridge(mpzN));
}
/**********************************************************************/
int iLucasSelfridge(mpz_t mpzN)
{
/* Test mpzN for primality using Lucas's test with Selfridge's parameters.
   Returns 1 if mpzN is prime or a Lucas-Selfridge pseudoprime. Returns
   0 if mpzN is definitely composite. Note that a Lucas-Selfridge test
   typically requires three to seven times as many bit operations as a
   single Miller's test. The frequency of Lucas-Selfridge pseudoprimes
   appears to be roughly four times that of base-2 strong pseudoprimes;
   the Baillie-PSW test is based on the hope (verified by the author,
   May, 2005, for all N < 10^13; and by Martin Fuller, January, 2007,
   for all N < 10^15) that the two tests have no common pseudoprimes. */

int iComp2, iP, iJ, iSign;
long lDabs, lD, lQ;
unsigned long ulMaxBits, ulNbits, ul, ulGCD;
mpz_t mpzU, mpzV, mpzNplus1, mpzU2m, mpzV2m, mpzQm, mpz2Qm,
      mpzT1, mpzT2, mpzT3, mpzT4, mpzD;

#undef RETURN
#define RETURN(n)           \
  {                         \
  mpz_clear(mpzU);          \
  mpz_clear(mpzV);          \
  mpz_clear(mpzNplus1);     \
  mpz_clear(mpzU2m);        \
  mpz_clear(mpzV2m);        \
  mpz_clear(mpzQm);         \
  mpz_clear(mpz2Qm);        \
  mpz_clear(mpzT1);         \
  mpz_clear(mpzT2);         \
  mpz_clear(mpzT3);         \
  mpz_clear(mpzT4);         \
  mpz_clear(mpzD);          \
  return(n);                \
  }

/* This implementation of the algorithm assumes N is an odd integer > 2,
   so we first eliminate all N < 3 and all even N. As a practical matter,
   we also need to filter out all perfect square values of N, such as
   1093^2 (a base-2 strong pseudoprime); this is because we will later
   require an integer D for which Jacobi(D,N) = -1, and no such integer
   exists if N is a perfect square. The algorithm as written would
   still eventually return zero in this case, but would require
   nearly sqrt(N)/2 iterations. */

iComp2=mpz_cmp_si(mpzN, 2);
if(iComp2 < 0)return(0);
if(iComp2==0)return(1);
if(mpz_even_p(mpzN))return(0);
if(mpz_perfect_square_p(mpzN))return(0);

/* Allocate storage for the mpz_t variables. Most require twice
   the storage of mpzN, since multiplications of order O(mpzN)*O(mpzN)
   will be performed. */

ulMaxBits=2*mpz_sizeinbase(mpzN, 2) + mp_bits_per_limb;
mpz_init2(mpzU, ulMaxBits);
mpz_init2(mpzV, ulMaxBits);
mpz_init2(mpzNplus1, ulMaxBits);
mpz_init2(mpzU2m, ulMaxBits);
mpz_init2(mpzV2m, ulMaxBits);
mpz_init2(mpzQm, ulMaxBits);
mpz_init2(mpz2Qm, ulMaxBits);
mpz_init2(mpzT1, ulMaxBits);
mpz_init2(mpzT2, ulMaxBits);
mpz_init2(mpzT3, ulMaxBits);
mpz_init2(mpzT4, ulMaxBits);
mpz_init(mpzD);

/* Find the first element D in the sequence {5, -7, 9, -11, 13, ...}
   such that Jacobi(D,N) = -1 (Selfridge's algorithm). Although
   D will nearly always be "small" (perfect square N's having
   been eliminated), an overflow trap for D is present. */

lDabs=5;
iSign=1;
while(1)
  {
  lD=iSign*lDabs;
  iSign = -iSign;
  ulGCD=mpz_gcd_ui(NULL, mpzN, lDabs);
  /* if 1 < GCD < N then N is composite with factor lDabs, and
     Jacobi(D,N) is technically undefined (but often returned
     as zero). */
  if((ulGCD > 1) && mpz_cmp_ui(mpzN, ulGCD) > 0)RETURN(0);
  mpz_set_si(mpzD, lD);
  iJ=mpz_jacobi(mpzD, mpzN);
  if(iJ==-1)break;
  lDabs += 2;
  if(lDabs > ulDmax)ulDmax=lDabs;  /* tracks global max of |D| */
  if(lDabs > INT32_MAX-2)
    {
    fprintf(stderr,
      "\n ERROR: D overflows signed long in Lucas-Selfridge test.");
    fprintf(stderr, "\n N=");
    mpz_out_str(stderr, 10, mpzN);
    fprintf(stderr, "\n |D|=%ld\n\n", lDabs);
    exit(EXIT_FAILURE);
    }
  }

iP=1;         /* Selfridge's choice */
lQ=(1-lD)/4;  /* Required so D = P*P - 4*Q */

/* NOTE: The conditions (a) N does not divide Q, and
   (b) D is square-free or not a perfect square, are included by
   some authors; e.g., "Prime numbers and computer methods for
   factorization," Hans Riesel (2nd ed., 1994, Birkhauser, Boston),
   p. 130. For this particular application of Lucas sequences,
   these conditions were found to be immaterial. */

mpz_add_ui(mpzNplus1, mpzN, 1); /* must compute U_(N - Jacobi(D,N)) */

/* mpzNplus1 is always even, so the accumulated values U and V
   are initialized to U_0 and V_0 (if the target index were odd,
   U and V would be initialized to U_1=1 and V_1=P). In either case,
   the values of U_2m and V_2m are initialized to U_1 and V_1;
   the FOR loop calculates in succession U_2 and V_2, U_4 and
   V_4, U_8 and V_8, etc. If the corresponding bits of N+1 are
   on, these values are then combined with the previous totals
   for U and V, using the composition formulas for addition
   of indices. */

mpz_set_ui(mpzU, 0);           /* U=U_0 */
mpz_set_ui(mpzV, 2);           /* V=V_0 */
mpz_set_ui(mpzU2m, 1);         /* U_1 */
mpz_set_si(mpzV2m, iP);        /* V_1 */
mpz_set_si(mpzQm, lQ);
mpz_set_si(mpz2Qm, 2*lQ);

ulNbits=mpz_sizeinbase(mpzNplus1, 2);
for(ul=1; ul < ulNbits; ul++)  /* zero bit off, already accounted for */
  {
/* Formulas for doubling of indices (carried out mod N). Note that
 * the indices denoted as "2m" are actually powers of 2, specifically
 * 2^(ul-1) beginning each loop and 2^ul ending each loop.
 *
 * U_2m = U_m*V_m
 * V_2m = V_m*V_m - 2*Q^m
 */
  mpz_mul(mpzU2m, mpzU2m, mpzV2m);
  mpz_mod(mpzU2m, mpzU2m, mpzN);
  mpz_mul(mpzV2m, mpzV2m, mpzV2m);
  mpz_sub(mpzV2m, mpzV2m, mpz2Qm);
  mpz_mod(mpzV2m, mpzV2m, mpzN);
  if(mpz_tstbit(mpzNplus1, ul))
    {
/* Formulas for addition of indices (carried out mod N);
 *
 * U_(m+n) = (U_m*V_n + U_n*V_m)/2
 * V_(m+n) = (V_m*V_n + D*U_m*U_n)/2
 *
 * Be careful with division by 2 (mod N)!
 */
    mpz_mul(mpzT1, mpzU2m, mpzV);
    mpz_mul(mpzT2, mpzU, mpzV2m);
    mpz_mul(mpzT3, mpzV2m, mpzV);
    mpz_mul(mpzT4, mpzU2m, mpzU);
    mpz_mul_si(mpzT4, mpzT4, lD);
    mpz_add(mpzU, mpzT1, mpzT2);
    if(mpz_odd_p(mpzU))mpz_add(mpzU, mpzU, mpzN);
    mpz_fdiv_q_2exp(mpzU, mpzU, 1);
    mpz_add(mpzV, mpzT3, mpzT4);
    if(mpz_odd_p(mpzV))mpz_add(mpzV, mpzV, mpzN);
    mpz_fdiv_q_2exp(mpzV, mpzV, 1);
    mpz_mod(mpzU, mpzU, mpzN);
    mpz_mod(mpzV, mpzV, mpzN);
    }
/* Calculate Q^m for next bit position, doubling the exponent.
   The irrelevant final iteration is omitted. */
  if(ul < ulNbits - 1)  /* Q^m not needed for MSB. */
    {

    mpz_mul(mpzQm, mpzQm, mpzQm);
    mpz_mod(mpzQm, mpzQm, mpzN);  /* prevents overflow */
    mpz_add(mpz2Qm, mpzQm, mpzQm);
    }
  }

/* If U_(N - Jacobi(D,N)) is congruent to 0 mod N, then N is
   a prime or a Lucas pseudoprime; otherwise it is definitely
   composite. */

if(mpz_sgn(mpzU)==0)RETURN(1);
RETURN(0);
}
/**********************************************************************/
int iStrongLucasSelfridge(mpz_t mpzN)
{
/* Test N for primality using the strong Lucas test with Selfridge's
   parameters. Returns 1 if N is prime or a strong Lucas-Selfridge
   pseudoprime (in which case N is also a pseudoprime to the standard
   Lucas-Selfridge test). Returns 0 if N is definitely composite.

   The running time of the strong Lucas-Selfridge test is, on average,
   roughly 10 % greater than the running time for the standard
   Lucas-Selfridge test (3 to 7 times that of a single Miller's test).
   However, the frequency of strong Lucas pseudoprimes appears to be
   only (roughly) 30 % that of (standard) Lucas pseudoprimes, and only
   slightly greater than the frequency of base-2 strong pseudoprimes,
   indicating that the strong Lucas-Selfridge test is more computationally
   effective than the standard version. */

int iComp2, iP, iJ, iSign;
long lDabs, lD, lQ;
unsigned long ulMaxBits, uldbits, ul, ulGCD, r, s;
mpz_t mpzU, mpzV, mpzNplus1, mpzU2m, mpzV2m, mpzQm, mpz2Qm,
      mpzT1, mpzT2, mpzT3, mpzT4, mpzD, mpzd, mpzQkd, mpz2Qkd;

#undef RETURN
#define RETURN(n)           \
  {                         \
  mpz_clear(mpzU);          \
  mpz_clear(mpzV);          \
  mpz_clear(mpzNplus1);     \
  mpz_clear(mpzU2m);        \
  mpz_clear(mpzV2m);        \
  mpz_clear(mpzQm);         \
  mpz_clear(mpz2Qm);        \
  mpz_clear(mpzT1);         \
  mpz_clear(mpzT2);         \
  mpz_clear(mpzT3);         \
  mpz_clear(mpzT4);         \
  mpz_clear(mpzD);          \
  mpz_clear(mpzd);          \
  mpz_clear(mpzQkd);        \
  mpz_clear(mpz2Qkd);       \
  return(n);                \
  }

/* This implementation of the algorithm assumes N is an odd integer > 2,
   so we first eliminate all N < 3 and all even N. As a practical matter,
   we also need to filter out all perfect square values of N, such as
   1093^2 (a base-2 strong pseudoprime); this is because we will later
   require an integer D for which Jacobi(D,N) = -1, and no such integer
   exists if N is a perfect square. The algorithm as written would
   still eventually return zero in this case, but would require
   nearly sqrt(N)/2 iterations. */

iComp2=mpz_cmp_si(mpzN, 2);
if(iComp2 < 0)return(0);
if(iComp2==0)return(1);
if(mpz_even_p(mpzN))return(0);
if(mpz_perfect_square_p(mpzN))return(0);

/* Allocate storage for the mpz_t variables. Most require twice
   the storage of mpzN, since multiplications of order O(mpzN)*O(mpzN)
   will be performed. */

ulMaxBits=2*mpz_sizeinbase(mpzN, 2) + mp_bits_per_limb;
mpz_init2(mpzU, ulMaxBits);
mpz_init2(mpzV, ulMaxBits);
mpz_init2(mpzNplus1, ulMaxBits);
mpz_init2(mpzU2m, ulMaxBits);
mpz_init2(mpzV2m, ulMaxBits);
mpz_init2(mpzQm, ulMaxBits);
mpz_init2(mpz2Qm, ulMaxBits);
mpz_init2(mpzT1, ulMaxBits);
mpz_init2(mpzT2, ulMaxBits);
mpz_init2(mpzT3, ulMaxBits);
mpz_init2(mpzT4, ulMaxBits);
mpz_init(mpzD);
mpz_init2(mpzd, ulMaxBits);
mpz_init2(mpzQkd, ulMaxBits);
mpz_init2(mpz2Qkd, ulMaxBits);

/* Find the first element D in the sequence {5, -7, 9, -11, 13, ...}
   such that Jacobi(D,N) = -1 (Selfridge's algorithm). Theory
   indicates that, if N is not a perfect square, D will "nearly
   always" be "small." Just in case, an overflow trap for D is
   included. */

lDabs=5;
iSign=1;
while(1)
  {
  lD=iSign*lDabs;
  iSign = -iSign;
  ulGCD=mpz_gcd_ui(NULL, mpzN, lDabs);
  /* if 1 < GCD < N then N is composite with factor lDabs, and
     Jacobi(D,N) is technically undefined (but often returned
     as zero). */
  if((ulGCD > 1) && mpz_cmp_ui(mpzN, ulGCD) > 0)RETURN(0);
  mpz_set_si(mpzD, lD);
  iJ=mpz_jacobi(mpzD, mpzN);
  if(iJ==-1)break;
  lDabs += 2;
  if(lDabs > ulDmax)ulDmax=lDabs;  /* tracks global max of |D| */
  if(lDabs > INT32_MAX-2)
    {
    fprintf(stderr,
      "\n ERROR: D overflows signed long in Lucas-Selfridge test.");
    fprintf(stderr, "\n N=");
    mpz_out_str(stderr, 10, mpzN);
    fprintf(stderr, "\n |D|=%ld\n\n", lDabs);
    exit(EXIT_FAILURE);
    }
  }

iP=1;         /* Selfridge's choice */
lQ=(1-lD)/4;  /* Required so D = P*P - 4*Q */

/* NOTE: The conditions (a) N does not divide Q, and
   (b) D is square-free or not a perfect square, are included by
   some authors; e.g., "Prime numbers and computer methods for
   factorization," Hans Riesel (2nd ed., 1994, Birkhauser, Boston),
   p. 130. For this particular application of Lucas sequences,
   these conditions were found to be immaterial. */

/* Now calculate N - Jacobi(D,N) = N + 1 (even), and calculate the
   odd positive integer d and positive integer s for which
   N + 1 = 2^s*d (similar to the step for N - 1 in Miller's test).
   The strong Lucas-Selfridge test then returns N as a strong
   Lucas probable prime (slprp) if any of the following
   conditions is met: U_d=0, V_d=0, V_2d=0, V_4d=0, V_8d=0,
   V_16d=0, ..., etc., ending with V_{2^(s-1)*d}=V_{(N+1)/2}=0
   (all equalities mod N). Thus d is the highest index of U that
   must be computed (since V_2m is independent of U), compared
   to U_{N+1} for the standard Lucas-Selfridge test; and no
   index of V beyond (N+1)/2 is required, just as in the
   standard Lucas-Selfridge test. However, the quantity Q^d must
   be computed for use (if necessary) in the latter stages of
   the test. The result is that the strong Lucas-Selfridge test
   has a running time only slightly greater (order of 10 %) than
   that of the standard Lucas-Selfridge test, while producing
   only (roughly) 30 % as many pseudoprimes (and every strong
   Lucas pseudoprime is also a standard Lucas pseudoprime). Thus
   the evidence indicates that the strong Lucas-Selfridge test is
   more effective than the standard Lucas-Selfridge test, and a
   Baillie-PSW test based on the strong Lucas-Selfridge test
   should be more reliable. */


mpz_add_ui(mpzNplus1, mpzN, 1);
s=mpz_scan1(mpzNplus1, 0);
mpz_tdiv_q_2exp(mpzd, mpzNplus1, s);

/* We must now compute U_d and V_d. Since d is odd, the accumulated
   values U and V are initialized to U_1 and V_1 (if the target
   index were even, U and V would be initialized instead to U_0=0
   and V_0=2). The values of U_2m and V_2m are also initialized to
   U_1 and V_1; the FOR loop calculates in succession U_2 and V_2,
   U_4 and V_4, U_8 and V_8, etc. If the corresponding bits
   (1, 2, 3, ...) of t are on (the zero bit having been accounted
   for in the initialization of U and V), these values are then
   combined with the previous totals for U and V, using the
   composition formulas for addition of indices. */

mpz_set_ui(mpzU, 1);                      /* U=U_1 */
mpz_set_ui(mpzV, iP);                     /* V=V_1 */
mpz_set_ui(mpzU2m, 1);                    /* U_1 */
mpz_set_si(mpzV2m, iP);                   /* V_1 */
mpz_set_si(mpzQm, lQ);
mpz_set_si(mpz2Qm, 2*lQ);
mpz_set_si(mpzQkd, lQ);  /* Initializes calculation of Q^d */

uldbits=mpz_sizeinbase(mpzd, 2);
for(ul=1; ul < uldbits; ul++)  /* zero bit on, already accounted for */
  {
/* Formulas for doubling of indices (carried out mod N). Note that
 * the indices denoted as "2m" are actually powers of 2, specifically
 * 2^(ul-1) beginning each loop and 2^ul ending each loop.
 *
 * U_2m = U_m*V_m
 * V_2m = V_m*V_m - 2*Q^m
 */
  mpz_mul(mpzU2m, mpzU2m, mpzV2m);
  mpz_mod(mpzU2m, mpzU2m, mpzN);
  mpz_mul(mpzV2m, mpzV2m, mpzV2m);
  mpz_sub(mpzV2m, mpzV2m, mpz2Qm);
  mpz_mod(mpzV2m, mpzV2m, mpzN);
  /* Must calculate powers of Q for use in V_2m, also for Q^d later */
  mpz_mul(mpzQm, mpzQm, mpzQm);
  mpz_mod(mpzQm, mpzQm, mpzN);  /* prevents overflow */
  mpz_mul_2exp(mpz2Qm, mpzQm, 1);
  if(mpz_tstbit(mpzd, ul))
    {
/* Formulas for addition of indices (carried out mod N);
 *
 * U_(m+n) = (U_m*V_n + U_n*V_m)/2
 * V_(m+n) = (V_m*V_n + D*U_m*U_n)/2
 *
 * Be careful with division by 2 (mod N)!
 */
    mpz_mul(mpzT1, mpzU2m, mpzV);
    mpz_mul(mpzT2, mpzU, mpzV2m);
    mpz_mul(mpzT3, mpzV2m, mpzV);
    mpz_mul(mpzT4, mpzU2m, mpzU);
    mpz_mul_si(mpzT4, mpzT4, lD);
    mpz_add(mpzU, mpzT1, mpzT2);
    if(mpz_odd_p(mpzU))mpz_add(mpzU, mpzU, mpzN);
    mpz_fdiv_q_2exp(mpzU, mpzU, 1);
    mpz_add(mpzV, mpzT3, mpzT4);
    if(mpz_odd_p(mpzV))mpz_add(mpzV, mpzV, mpzN);
    mpz_fdiv_q_2exp(mpzV, mpzV, 1);
    mpz_mod(mpzU, mpzU, mpzN);
    mpz_mod(mpzV, mpzV, mpzN);
    mpz_mul(mpzQkd, mpzQkd, mpzQm);  /* Calculating Q^d for later use */
    mpz_mod(mpzQkd, mpzQkd, mpzN);
    }
  }

/* If U_d or V_d is congruent to 0 mod N, then N is a prime or a
   strong Lucas pseudoprime. */

if(mpz_sgn(mpzU)==0)RETURN(1);
if(mpz_sgn(mpzV)==0)RETURN(1);

/* NOTE: Ribenboim ("The new book of prime number records," 3rd ed.,
   1995/6) omits the condition Vð0 on p.142, but includes it on
   p. 130. The condition is NECESSARY; otherwise the test will
   return false negatives---e.g., the primes 29 and 2000029 will be
   returned as composite. */

/* Otherwise, we must compute V_2d, V_4d, V_8d, ..., V_{2^(s-1)*d}
   by repeated use of the formula V_2m = V_m*V_m - 2*Q^m. If any of
   these are congruent to 0 mod N, then N is a prime or a strong
   Lucas pseudoprime. */

mpz_mul_2exp(mpz2Qkd, mpzQkd, 1);  /* Initialize 2*Q^(d*2^r) for V_2m */
for(r=1; r < s; r++)
  {
  mpz_mul(mpzV, mpzV, mpzV);
  mpz_sub(mpzV, mpzV, mpz2Qkd);
  mpz_mod(mpzV, mpzV, mpzN);
  if(mpz_sgn(mpzV)==0)RETURN(1);
/* Calculate Q^{d*2^r} for next r (final iteration irrelevant). */
  if(r < s-1)
    {
    mpz_mul(mpzQkd, mpzQkd, mpzQkd);
    mpz_mod(mpzQkd, mpzQkd, mpzN);
    mpz_mul_2exp(mpz2Qkd, mpzQkd, 1);
    }
  }

/* Otherwise N is definitely composite. */

RETURN(0);
}
/**********************************************************************/
int iExtraStrongLucas(mpz_t mpzN, long lB)
{
/* Test N for primality using the extra strong Lucas test with base B,
   as formulated by Zhaiyu Mo and James P. Jones ("A new primality test
   using Lucas sequences," preprint, circa 1997), and described by Jon
   Grantham in "Frobenius pseudoprimes," (preprint, 16 July 1998),
   available at <http://www.pseudoprime.com/pseudo1.ps>.

   Returns 1 if N is prime or an extra strong Lucas pseudoprime (base B).
   Returns 0 if N is definitely composite.

   Even N and N < 3 are eliminated before applying the Lucas test.

   In this implementation of the algorithm, Q=1, and B is an integer
   in 2 < B < INT32_MAX (2147483647 on 32-bit machines); the default value
   is B=3. B is incremented as necessary if the values of B and N are
   inconsistent with the hypotheses of Jones and Mo: P=B, Q=1,
   D=P*P - 4*Q, GCD(N,2D)=1, Jacobi(D,N) <> 0.

   Since the base B is used solely to calculate the discriminant
   D=B*B - 4, negative values of B are redundant. The bases B=0 and
   B=1 are excluded because they produce huge numbers of pseudoprimes,
   and B=2 is excluded because the resulting D=0 fails the Jones-Mo
   hypotheses.

   Note that the choice Q=1 eliminates the computation of powers of Q
   which appears in the weak and strong Lucas tests.

   The running time of the extra strong Lucas-Selfridge test is, on
   average, roughly 80 % that of the standard Lucas-Selfridge test
   or 2 to 6 times that of a single Miller's test. This is superior
   in speed to both the standard and strong Lucas-Selfridge tests. The
   frequency of extra strong Lucas pseudoprimes also appears to be
   about 80 % that of the strong Lucas-Selfridge test and 30 % that of
   the standard Lucas-Selfridge test, comparable to the frequency of
   spsp(2).

   Unfortunately, the apparent superior peformance of the extra strong
   Lucas test is offset by the fact that it is not "backwards compatible"
   with the Lucas-Selfridge tests, due to the differing choice of
   parameters: P=B and Q=1 in the extra strong test, while P=1 and
   Q=(1 - D)/4 in the standard and strong Lucas-Selfridge tests (with D
   chosen from the sequence 5, -7, 9, ...). Thus, although every extra
   strong Lucas pseudoprime to base B is also both a strong and standard
   Lucas pseudoprime with parameters P=B and Q=1, the extra strong
   pseudoprimes do *NOT* constitute a proper subset of the Lucas-Selfridge
   standard and strong pseudoprimes. As a specific example, 4181 is an
   extra strong Lucas pseudoprime to base 3, but is neither a standard
   nor strong Lucas-Selfridge pseudoprime.

   As a result, the corresponding Baillie-PSW test is fatally flawed.
   Regardless of the base chosen for the extra strong Lucas test, it
   appears that there exist numerous N for which the corresponding
   extra strong Lucas pseudoprimes (xslpsp) will also be strong
   pseudoprimes to base 2 (or any other particular Miller's base).
   For example, 6368689 is both spsp(2) and xslpsp(3); 8725753
   is both spsp(2) and xslpsp(11); 80579735209 is spsp(2) and
   simultaneously xslpsp for the bases 3, 5, and 7; 105919633 is
   spsp(3) and xslpsp(11); 1121176981 is spsp(19) and xslpsp(31);
   and so on. Perhaps some combination of the extra strong test
   and multiple Miller's tests could match the performance of the
   Lucas-Selfridge BPSW tests, but the prospects do not look bright.
*/

int iComp2, iJ;
long lD, lP, lQ;
unsigned long ulMaxBits, uldbits, ul, ulGCD, r, s;
mpz_t mpzU, mpzV, mpzM, mpzU2m, mpzV2m, mpzT1, mpzT2, mpzT3, mpzT4,
      mpzD, mpzd, mpzTwo, mpzMinusTwo;

#undef RETURN
#define RETURN(n)           \
  {                         \
  mpz_clear(mpzU);          \
  mpz_clear(mpzV);          \
  mpz_clear(mpzM);          \
  mpz_clear(mpzU2m);        \
  mpz_clear(mpzV2m);        \
  mpz_clear(mpzT1);         \
  mpz_clear(mpzT2);         \
  mpz_clear(mpzT3);         \
  mpz_clear(mpzT4);         \
  mpz_clear(mpzD);          \
  mpz_clear(mpzd);          \
  mpz_clear(mpzTwo);        \
  mpz_clear(mpzMinusTwo);   \
  return(n);                \
  }

/* This implementation of the algorithm assumes N is an odd integer > 2,
   so we first eliminate all N < 3 and all even N. */

iComp2=mpz_cmp_si(mpzN, 2);
if(iComp2 < 0)return(0);
if(iComp2==0)return(1);
if(mpz_even_p(mpzN))return(0);

/* Allocate storage for the mpz_t variables. Most require twice
   the storage of mpzN, since multiplications of order O(mpzN)*O(mpzN)
   will be performed. */

ulMaxBits=2*mpz_sizeinbase(mpzN, 2) + mp_bits_per_limb;
mpz_init2(mpzU, ulMaxBits);
mpz_init2(mpzV, ulMaxBits);
mpz_init2(mpzM, ulMaxBits);
mpz_init2(mpzU2m, ulMaxBits);
mpz_init2(mpzV2m, ulMaxBits);
mpz_init2(mpzT1, ulMaxBits);
mpz_init2(mpzT2, ulMaxBits);
mpz_init2(mpzT3, ulMaxBits);
mpz_init2(mpzT4, ulMaxBits);
mpz_init(mpzD);
mpz_init2(mpzd, ulMaxBits);
mpz_init_set_si(mpzTwo, 2);
mpz_init_set_si(mpzMinusTwo, -2);

/* The parameters specified by Zhaiyu Mo and James P. Jones,
   as set forth in Grantham's paper, are P=B, Q=1, D=P*P - 4*Q,
   with (N,2D)=1 so that Jacobi(D,N) <> 0. As explained above,
   bases B < 3 are excluded. */

if(lB < 3)
  lP=3;
else
  lP=lB;
lQ=1;

/* We check to make sure that N and D are relatively prime. If not,
   then either 1 < (D,N) < N, in which case N is composite with
   divisor (D,N); or N = (D,N), in which case N divides D and may be
   either prime or composite, so we increment the base B=P and
   try again. */

while(1)
  {
  lD=lP*lP - 4*lQ;
  ulGCD=mpz_gcd_ui(NULL, mpzN, labs(lD));
  if(ulGCD==1)break;
  if(mpz_cmp_ui(mpzN, ulGCD) > 0)RETURN(0);
  lP++;
  }

/* Now calculate M = N - Jacobi(D,N) (M even), and calculate the
   odd positive integer d and positive integer s for which
   M = 2^s*d (similar to the step for N - 1 in Miller's
   test). The extra strong Lucas-Selfridge test then returns N as
   an extra strong Lucas probable prime (eslprp) if any of the
   following conditions is met: U_d=0 and V_dðñ2; or V_d=0; or
   V_2d=0, V_4d=0, V_8d=0, V_16d=0, ..., etc., ending with
   V_{2^(s-2)*d}=V_{M/4}ð0 (all equalities mod N). Thus d is the
   highest index of U that must be computed (since V_2m is
   independent of U), compared to U_M for the standard Lucas
   test; and no index of V beyond M/4 is required, compared to
   M/2 for the standard and strong Lucas tests. Furthermore,
   since Q=1, the powers of Q required in the standard and
   strong Lucas tests can be dispensed with. The result is that
   the extra strong Lucas test has a running time shorter than
   that of either the standard or strong Lucas-Selfridge tests
   (roughly two to six times that of a single Miller's test).
   The extra strong test also produces fewer pseudoprimes.
   Unfortunately, the pseudoprimes produced are *NOT* a subset
   of the standard or strong Lucas-Selfridge pseudoprimes (due
   to the incompatible parameters P and Q), and consequently the
   extra strong test does not combine with a single Miller's test
   to produce a Baillie-PSW test of the reliability level of the
   BPSW tests based on the standard or strong Lucas-Selfridge tests. */

mpz_set_si(mpzD, lD);
iJ=mpz_jacobi(mpzD, mpzN);
assert(iJ != 0);
if(iJ==1)
  mpz_sub_ui(mpzM, mpzN, 1);
else
  mpz_add_ui(mpzM, mpzN, 1);

s=mpz_scan1(mpzM, 0);
mpz_tdiv_q_2exp(mpzd, mpzM, s);

/* We must now compute U_d and V_d. Since d is odd, the accumulated
   values U and V are initialized to U_1 and V_1 (if the target
   index were even, U and V would be initialized instead to U_0=0
   and V_0=2). The values of U_2m and V_2m are also initialized to
   U_1 and V_1; the FOR loop calculates in succession U_2 and V_2,
   U_4 and V_4, U_8 and V_8, etc. If the corresponding bits
   (1, 2, 3, ...) of t are on (the zero bit having been accounted
   for in the initialization of U and V), these values are then
   combined with the previous totals for U and V, using the
   composition formulas for addition of indices. */

mpz_set_ui(mpzU, 1);                       /* U=U_1 */
mpz_set_si(mpzV, lP);                      /* V=V_1 */
mpz_set_ui(mpzU2m, 1);                     /* U_1 */
mpz_set_si(mpzV2m, lP);                    /* V_1 */

uldbits=mpz_sizeinbase(mpzd, 2);
for(ul=1; ul < uldbits; ul++)  /* zero bit on, already accounted for */
  {
/* Formulas for doubling of indices (carried out mod N). Note that
 * the indices denoted as "2m" are actually powers of 2, specifically
 * 2^(ul-1) beginning each loop and 2^ul ending each loop.
 *
 * U_2m = U_m*V_m
 * V_2m = V_m*V_m - 2*Q^m
 */
  mpz_mul(mpzU2m, mpzU2m, mpzV2m);
  mpz_mod(mpzU2m, mpzU2m, mpzN);
  mpz_mul(mpzV2m, mpzV2m, mpzV2m);
  mpz_sub_ui(mpzV2m, mpzV2m, 2);
  mpz_mod(mpzV2m, mpzV2m, mpzN);
  if(mpz_tstbit(mpzd, ul))
    {
/* Formulas for addition of indices (carried out mod N);
 *
 * U_(m+n) = (U_m*V_n + U_n*V_m)/2
 * V_(m+n) = (V_m*V_n + D*U_m*U_n)/2
 *
 * Be careful with division by 2 (mod N)!
 */
    mpz_mul(mpzT1, mpzU2m, mpzV);
    mpz_mul(mpzT2, mpzU, mpzV2m);
    mpz_mul(mpzT3, mpzV2m, mpzV);
    mpz_mul(mpzT4, mpzU2m, mpzU);
    mpz_mul_si(mpzT4, mpzT4, lD);
    mpz_add(mpzU, mpzT1, mpzT2);
    if(mpz_odd_p(mpzU))mpz_add(mpzU, mpzU, mpzN);
    mpz_fdiv_q_2exp(mpzU, mpzU, 1);
    mpz_add(mpzV, mpzT3, mpzT4);
    if(mpz_odd_p(mpzV))mpz_add(mpzV, mpzV, mpzN);
    mpz_fdiv_q_2exp(mpzV, mpzV, 1);
    mpz_mod(mpzU, mpzU, mpzN);
    mpz_mod(mpzV, mpzV, mpzN);
    }
  }

/* N first passes the extra strong Lucas test if V_dð0, or if V_dðñ2
   and U_dð0.  U and V are tested for divisibility by N, rather than
   zero, in case the previous FOR is a zero-iteration loop.*/

if(mpz_divisible_p(mpzV, mpzN))RETURN(1);
if(mpz_divisible_p(mpzU, mpzN))
  {
  if(mpz_congruent_p(mpzV, mpzTwo, mpzN))RETURN(1);
  if(mpz_congruent_p(mpzV, mpzMinusTwo, mpzN))RETURN(1);
  }

/* Otherwise, we must compute V_2d, V_4d, V_8d, ..., V_{2^(s-2)*d}
   by repeated use of the formula V_2m = V_m*V_m - 2*Q^m. If any of
   these are congruent to 0 mod N, then N is a prime or an extra
   strong Lucas pseudoprime. */

for(r=1; r < s-1; r++)
  {
  mpz_mul(mpzV, mpzV, mpzV);
  mpz_sub_ui(mpzV, mpzV, 2);
  mpz_mod(mpzV, mpzV, mpzN);
  if(mpz_sgn(mpzV)==0)RETURN(1);
  }

/* Otherwise N is definitely composite. */

RETURN(0);
}
/**********************************************************************/
#endif  /* GMP available */
/**********************************************************************/
/**********************************************************************/
/*              Li(x); Hardy-Littlewood Integrals;                    */
/*    Riemann Zeta function; Riemann Prime Counting function R(x)     */
/**********************************************************************/
/**********************************************************************/
long double ldLogInt(long double ldx, long double *ldHL2,
  long double *ldHL3, long double *ldHL4)
{
/* This routine is similar to mpfrLIRZ, but uses long double arithmetic
   (thus yielding only 19 significant decimal digits), and does not
   compute the Riemann prime-counting function R(x) [see ldRPCF].
   The argument ldx must be >= 2, else artificial values of zero are
   returned. See the comments in mpfrLIRZ for further details.

   In general, one can expect cumulative rounding errors of two or
   three SDD, yielding in the end 16 or 17 correct decimal digits.
*/

int i;
long double ldTerm1, ldTerm2, ldDelta, ldLx, ldLx2, ldLx3, ldIntegral2,
  ldIntegral3, ldIntegral4, ldI1, ld;

*ldHL2=0;
*ldHL3=0;
*ldHL4=0;
if(ldx < 2)return(0);
if(ldx==2)return(1.0451637801174927848445888891946131365226155781512L);

ldLx=logl(ldx);
ldLx2=ldLx*ldLx;
ldLx3=ldLx*ldLx2;

ldIntegral4=-1/(3*ldLx3) + 1/(3*M_LN2_CUBED) - 1/(2*ldLx2)
  + 1/(2*M_LN2_SQUARED) - 1/(2*ldLx) + 1/(2*M_LN2) + logl(ldLx/M_LN2)/6;
ldTerm1=ldLx/24;
ldTerm2=M_LN2/24;

for(i=1; ; i++)
  {
  ldIntegral4 += ldTerm1 - ldTerm2;
  ldDelta=LDBL_EPSILON*ldIntegral4;
  ld=((long double)i)/((i+1)*(i+4.0L));
  ldTerm1 *= ldLx*ld;
  if(ldTerm1 < ldDelta)break;
  if(ldTerm2 > ldDelta)ldTerm2 *= M_LN2*ld; else ldTerm2=0;
  }

*ldHL4=M_HL4*ldIntegral4;

ldIntegral3=3*ldIntegral4 + ldx/ldLx3 - 2/M_LN2_CUBED;
*ldHL3=M_HL3*ldIntegral3;

ldIntegral2=2*ldIntegral3 + ldx/ldLx2 - 2/M_LN2_SQUARED;
*ldHL2=M_HL2*ldIntegral2;

ldI1=ldIntegral2 + ldx/ldLx - 2/M_LN2 + M_LI2;

return(ldI1);
}
/**********************************************************************/
long double ldLi(long double ldx)
{
long double ld2, ld3, ld4;
return(ldLogInt(ldx, &ld2, &ld3, &ld4));
}
/**********************************************************************/
long double ldRPCF(long double ldx)
{
/* Approximate Riemann's prime counting function R(x) using a truncated
   Gram series. For additional details, see "Prime numbers and
   computer methods for factorization," Hans Reisel (Birkhauser, Boston,
   1994), pp. 50-51, especially eqn 2.27.

   NOTE:  The domain of the algorithm is x > 1, but there is a progressive
   loss of accuracy near the singularity x=1. Consequently, an
   artificial value of zero is returned for x < 2 [R(x) is rarely
   evaluated for x < 2].

   In contrast to mpfrLIRZ, this routine employs only long double
   arithmetic, which is much faster but yields only 19 significant
   decimal digits. In general, one can expect cumulative rounding errors
   of two or three SDD, yielding in the end 16 or 17 correct decimal digits.
*/

static int iMaxNumTerms=1000;
int i;
long double ldt, ldTerm, ldSum, ldFactor;

if(ldx < 2)return(0);
if(ldx==2)return(1.54100901618713188328850378662754654353089921827L);
ldt=logl(ldx);
ldFactor=ldt;
ldSum=ldFactor/ldZeta(2);
for(i=2; i < iMaxNumTerms; i++)
  {
  ldFactor *= (ldt/i)/i*(i-1.0L);
  ldTerm=ldFactor/ldZeta(i + 1);
  ldSum += ldTerm;
  if(ldTerm/(1+ldSum) < LDBL_EPSILON)break;
  }
return(1 + ldSum);
}
/**********************************************************************/
void vDefineZetaArray(void)
{
/* Stores pre-computed values of the Riemann zeta function for integer
   arguments 0, 2, 3, ..., 65. */

if(ldZ[0] != 0)return;  /* external array already initialized */
ldZ[ 0]=-0.5L;
ldZ[ 2]=1.64493406684822643647241516664602518921894990120680L;
ldZ[ 3]=1.20205690315959428539973816151144999076498629234050L;
ldZ[ 4]=1.08232323371113819151600369654116790277475095191873L;
ldZ[ 5]=1.03692775514336992633136548645703416805708091950191L;
ldZ[ 6]=1.01734306198444913971451792979092052790181749003285L;
ldZ[ 7]=1.00834927738192282683979754984979675959986356056524L;
ldZ[ 8]=1.00407735619794433937868523850865246525896079064985L;
ldZ[ 9]=1.00200839282608221441785276923241206048560585139489L;
ldZ[10]=1.00099457512781808533714595890031901700601953156448L;
ldZ[11]=1.00049418860411946455870228252646993646860643575821L;
ldZ[12]=1.00024608655330804829863799804773967096041608845800L;
ldZ[13]=1.00012271334757848914675183652635739571427510589551L;
ldZ[14]=1.00006124813505870482925854510513533374748169616915L;
ldZ[15]=1.00003058823630702049355172851064506258762794870686L;
ldZ[16]=1.00001528225940865187173257148763672202323738899047L;
ldZ[17]=1.00000763719763789976227360029356302921308824909026L;
ldZ[18]=1.00000381729326499983985646164462193973045469721895L;
ldZ[19]=1.00000190821271655393892565695779510135325857114484L;
ldZ[20]=1.00000095396203387279611315203868344934594379418741L;
ldZ[21]=1.00000047693298678780646311671960437304596644669478L;
ldZ[22]=1.00000023845050272773299000364818675299493504182178L;
ldZ[23]=1.00000011921992596531107306778871888232638725499778L;
ldZ[24]=1.00000005960818905125947961244020793580122750391884L;
ldZ[25]=1.00000002980350351465228018606370506936601184473092L;
ldZ[26]=1.00000001490155482836504123465850663069862886478817L;
ldZ[27]=1.00000000745071178983542949198100417060411945471903L;
ldZ[28]=1.00000000372533402478845705481920401840242323289306L;
ldZ[29]=1.00000000186265972351304900640390994541694806166533L;
ldZ[30]=1.00000000093132743241966818287176473502121981356796L;
ldZ[31]=1.00000000046566290650337840729892332512200710626919L;
ldZ[32]=1.00000000023283118336765054920014559759404950248298L;
ldZ[33]=1.00000000011641550172700519775929738354563095165225L;
ldZ[34]=1.00000000005820772087902700889243685989106305417312L;
ldZ[35]=1.00000000002910385044497099686929425227884046410698L;
ldZ[36]=1.00000000001455192189104198423592963224531842098381L;
ldZ[37]=1.00000000000727595983505748101452086901233805926485L;
ldZ[38]=1.00000000000363797954737865119023723635587327351265L;
ldZ[39]=1.00000000000181898965030706594758483210073008503059L;
ldZ[40]=1.00000000000090949478402638892825331183869490875386L;
ldZ[41]=1.00000000000045474737830421540267991120294885703390L;
ldZ[42]=1.00000000000022737368458246525152268215779786912138L;
ldZ[43]=1.00000000000011368684076802278493491048380259064374L;
ldZ[44]=1.00000000000005684341987627585609277182967524068553L;
ldZ[45]=1.00000000000002842170976889301855455073704942662074L;
ldZ[46]=1.00000000000001421085482803160676983430714173953768L;
ldZ[47]=1.00000000000000710542739521085271287735447995680002L;
ldZ[48]=1.00000000000000355271369133711367329846953405934299L;
ldZ[49]=1.00000000000000177635684357912032747334901440027957L;
ldZ[50]=1.00000000000000088817842109308159030960913863913863L;
ldZ[51]=1.00000000000000044408921031438133641977709402681213L;
ldZ[52]=1.00000000000000022204460507980419839993200942046539L;
ldZ[53]=1.00000000000000011102230251410661337205445699213827L;
ldZ[54]=1.00000000000000005551115124845481243723736590509430L;
ldZ[55]=1.00000000000000002775557562136124172581632453854069L;
ldZ[56]=1.00000000000000001387778780972523276283909490650022L;
ldZ[57]=1.00000000000000000693889390454415369744608532624980L;
ldZ[58]=1.00000000000000000346944695216592262474427149610933L;
ldZ[59]=1.00000000000000000173472347604757657204897296993759L;
ldZ[60]=1.00000000000000000086736173801199337283420550673429L;
ldZ[61]=1.00000000000000000043368086900206504874970235659062L;
ldZ[62]=1.00000000000000000021684043449972197850139101683209L;
ldZ[63]=1.00000000000000000010842021724942414063012711165461L;
ldZ[64]=1.00000000000000000005421010862456645410918700404388L;
ldZ[65]=1.00000000000000000002710505431223468831954621311949L;

return;
}
/**********************************************************************/
long double ldZeta(long double ldx)
{
/* Computes approximate values of the Riemann zeta function for real
   non-negative arguments. Pre-computed values are returned for integer
   arguments 0 to 65; extrapolated values for x > 65; Riemann's analytic
   continuation series is used for non-integer values < 65. The real
   zeros at {-2, -4, -6, ...} are recognized. For additional details,
   see "Prime numbers and computer methods for factorization," Hans
   Reisel (Birkhauser, Boston, 1994), pp. 44-46. */

static int iMaxNumTerms=1000;
static long double ldZ65m1=2.710505431223468831954621311949e-20L;
			   /* zeta(65) - 1 */
int i, iSign;
long double ldSum, ldSumOld, ldDelta, ldDivisor, ldResult;

if(ldx==1)
  {
  fprintf(stderr,
    "\n ERROR: ldZeta called with illegal argument x=1 (singularity)\n");
  raise(SIGFPE);
  }

if((ldx < 0) && (floorl(ldx/2)==ldx/2))return(0);  /* Zeta(-2n) = 0 */
if(ldx==0)return(-0.5L);                           /* Zeta(0) = -1/2 */

if(ldx < 0)  /* This algorithm is not valid for negative ldx */
  {
  fprintf(stderr,
    "\n ERROR: x=%.15le passed to ldZeta; domain is x >= 0\n",
    (double)ldx);
  raise(SIGFPE);
  }

if(ldZ[0] != -0.5L)vDefineZetaArray();  /* initialize zeta(n) */

/* Pre-computed values for n=0,1,2,..,65. */

if((ldx==floorl(ldx)) && (ldx > 1) && (ldx < 66))
  {
  i=floorl(ldx);
  return(ldZ[i]);
  }

/* For n > 65, the difference between zeta(n) and 1 is halved
   for each unit increase in n, to at least 20D precision (64 bits). */

if(ldx > 65)
  {
  if(ldx > 1-log2l(LDBL_EPSILON))return(1);
    /* in above case, zeta(ldx)==1 to the limit of long dbl precision */
  ldResult=1 + ldZ65m1*powl(2, 65 - ldx);
  return(ldResult);
  }

/* For positive non-integer arguments < 65, zeta is computed using
   Riemann's analytic continuation formula,

   zeta(s)=1/(1 - 2^(1-s))*sum((-1)^(n-1)/n^s, n, 1, inf). */

iSign=-1;
ldSum=1;
for(i=2; i < iMaxNumTerms; i++)
  {
  ldSumOld=ldSum;
  ldDelta=iSign*powl(i, -ldx);
  ldSum += ldDelta;
  if(fabsl(ldDelta) < LDBL_EPSILON)break;
  iSign *= -1;
  }
ldSum=(ldSum + ldSumOld)/2;
ldDivisor=(1 - powl(2, 1 - ldx));
ldResult=ldSum/ldDivisor;

return(ldResult);
}
/**********************************************************************/
/**********************************************************************/
/*                       string editing                               */
/**********************************************************************/
/**********************************************************************/
char *szTrimMWS(char *pch)
{
return(szTrimLWS(szTrimTWS(pch)));
}
/**********************************************************************/
char *szTrimTWS(char *pch)
{
char            *pchStart;
long            sl;
unsigned long   ulLen;

if(*pch==0)return(pch);
pchStart=pch;
ulLen=strlen(pch);
pch=pchStart + ulLen - 1;
while(1)
  {
  if(isgraph(*pch))
    return(pchStart);
  else
    *pch=0;
  if(pch==pchStart)
    {
    *pch=0;
    return(pch);
    }
  pch--;
  }
}
/**********************************************************************/
char *szTrimLWS(char *pch)
{
char            *pchStart;

pchStart=pch;
while(1)
  {
  if(*pch==0)
    {
    *pchStart=0;
    return(pch);
    }
  else if(isgraph(*pch))
    {
    if(pch==pchStart)
      return(pch);
    else
      {
      memmove(pchStart, pch, strlen(pch)+1);
      return(pch);
      }
    }
  else
    pch++;
  }
}
/**********************************************************************/
void vRemoveWS(char *szOut, char *szIn)
{
/* Remove from szIn all spaces, tabs, and control characters, and return
   the result in szOut. */

unsigned long ulLen, i, j;
char ch;

ulLen=strlen(szIn);
for(i=0, j=0; i < ulLen; i++)
  {
  ch=szIn[i];
  if((ch < 33) || ch==127)  /* Skip spaces, tabs, and control characters */
    continue;
  else
    {
    szOut[j]=ch;
    j++;
    }
  }
szOut[j]=0;
return;
}
/**********************************************************************/
/**********************************************************************/
/*          Analysis and processing of prime gap records              */
/**********************************************************************/
/**********************************************************************/
int iRecordValidExt(char *sz)
{
/* Returns 0 for invalid record, 6 for a valid gap6 record, 9 for a
   valid gap9 record, 1 for a record of the form gggg pppp.
   Continuation lines return zero (invalid). Merit is not checked. */

#undef RETURN
#define RETURN(n) {mpz_clear(mpzP1); return(n);}

static char szTemp[41];
char *ep;
long sl;
unsigned long ulMaxBits;
double lf;
#ifdef __GMP__
  mpz_t mpzP1;
#endif

if(strlen(sz) < 41)goto GAP1;

/* Test for gap6 format */

if((sz[7]==' ') && (sz[8]=='C')
    && (sz[11]==' ') && (sz[20]==' ') && (sz[29]=='.') && (sz[38]==' ')
    && (sz[39]==' '))
  {
  strncpy(szTemp, sz, 6); szTemp[6]=0;
  sl=strtol(szTemp, NULL, 10);  /* gap field */
  if(sl < 1)goto GAP9;
  if((sl&1==1) && (sl != 1))goto GAP9;
  sl=strtol(sz+32, NULL, 10);  /* digits field */
  if(sl < 1)goto GAP9;
  return(6);
  }

GAP9:

if(strlen(sz) < 47)goto GAP1;

if((sz[10]==' ') && (sz[11]=='C')
    && (sz[14]==' ') && (sz[23]==' ') && (sz[32]=='.') && (sz[45]==' ')
    && (sz[46]==' '))
  {
  strncpy(szTemp, sz, 9); szTemp[9]=0;
  sl=strtol(szTemp, NULL, 10);  /* gap field */
  if(sl < 1)return(0);
  if((sl&1==1) && (sl != 1))return(0);
  sl=strtol(sz+37, NULL, 10);  /* digits field */
  if(sl < 1)goto GAP1;
  return(9);
  }

GAP1:

lf=strtod(sz, &ep);  /* gap field */
if(lf < 1)return(0);
if(lf > 999999999.5)return(0);
sl=floor(lf + 0.5);
if(sl < 1)return(0);
if((sl&1==1) && (sl != 1))return(0);

#ifdef __GMP__

/* Check the P1 value of type 1 gaps for plausibility. Assume a
   merit > 1 in allocating space for mpzP1 directly (if this
   fails, fall back on GMP's erratic dynamic reallocation).
   The following calculation is based on M=G/ln(P1)=1 ==> P1=e^G
   ==> P1 = 2^(log_2(e)*G) = 2^(1.442695*G) ==> 1.442695*G bits. */

ulMaxBits=ceil(1.4426950408889634*sl) + mp_bits_per_limb;
if(ulMaxBits > __MAX_BITS__)ulMaxBits=__MAX_BITS__;

mpz_init2(mpzP1, ulMaxBits);
if(__mpz_set_str(mpzP1, ep, 0))
  if(iEvalExprMPZ(mpzP1, ep))
    RETURN(0);  /* unable to parse P1 */

/* The following plausibility test for P1 uses Nicely's observation
   that P1 > 0.122985*sqrt(g)*exp(sqrt(g)) for all first occurrence
   and maximal prime gaps to 5e16, with this relationship conjectured
   to hold for indefinitely large g and P1. See "New prime gaps
   between 1e15 and 5e16", Bertil Nyman and Thomas R. Nicely,
   Journal of Integer Sequences 6 (13 August 2003), no. 3,
   Article 03.3.1, 6 pp., MR1997838 (2004e:11143), available
   electronically at <http://www.math.uwaterloo.ca/JIS/>. */

if(mpz_sizeinbase(mpzP1, 10) <
  log10(0.122985) + 0.5*log10(sl) + M_LOG_E_BASE10*sqrt(sl))RETURN(0);

RETURN(1);

#else

return(1);

#endif  /* __GMP__ */
}
/**********************************************************************/
int iGetGapRecExt(char *szGapRec, FILE *fpIn)
{
/* Returns 0 for failure, 6 for successful gap6 record, 9 for successful
   gap8 record, 1 for gap1 record.  Terminating newline is removed. */

char *pchCont, *pch;
int iStat;
unsigned long ul;

if(fpIn)  /* If NULL is passed for fpIn, szGapRec is _input_ */
  {
  szGapRec[0]=0;
  if(!fgets(szGapRec, __MAX_DIGITS__, fpIn))return(0);
  if(feof(fpIn))return(0);
  }
szTrimTWS(szGapRec);
iStat=iRecordValidExt(szGapRec);
if(iStat < 2)return(iStat);

pchCont=strpbrk(szGapRec+24, "_~\\");  /* Continuation lines coming? */
if(pchCont)*pchCont=0;
while(pchCont)
  {
  ul=strlen(szGapRec);
  fgets(szGapRec+ul+1, __MAX_DIGITS__, fpIn);
  if(feof(fpIn))
    {
    szGapRec[0]=0;
    return(0);
    }
  pch=szGapRec+ul+1;
  while(*pch < 33)pch++;
  strcat(szGapRec, pch);
  szTrimTWS(szGapRec);
  pchCont=strpbrk(szGapRec+24, "_~\\");  /* More continuation lines coming? */
  if(pchCont)*pchCont=0;
  }
return(iStat);
}
/**********************************************************************/
void vGapContExt(char *szContRec, char *szGapRec)
{
/* Creates a line continued gap6 or gap9 structure, or NULL on failure.
   NOTE: No terminating newline is appended. */

static char szSpacer[48];
int iStat;
unsigned long ulLen, ulOffset=40, ul;

iStat=iRecordValidExt(szGapRec);
if(iStat==0)
  {
  szContRec[0]=0;
  return;
  }
else if(iStat==6)
  ulOffset=40;
else if(iStat==9)
  ulOffset=47;

szTrimTWS(szGapRec);
ulLen=strlen(szGapRec);
if(ulLen <= ulOffset + 200)
  {
  strcpy(szContRec, szGapRec);
  return;
  }

strncpy(szContRec, szGapRec, ulOffset);
szContRec[ulOffset]=0;
for(ul=0; ul < ulOffset; ul++)szSpacer[ul]=' ';
szSpacer[ulOffset]=0;
while(1)
  {
  strncat(szContRec, szGapRec + ulOffset, 200);
  ulOffset += 200;
  if(ulOffset >= ulLen)break;
  strcat(szContRec, "\\\n");
  strcat(szContRec, szSpacer);
  }
return;
}
/**********************************************************************/
int iGetGapRec(char *szGapRec, FILE *fpIn)
{
/* Returns -1 on EOF, -2 on memory failure, zero for invalid record,
   1 for success.  Terminating newline is removed. */

static int iInit=0;
static char *szIn;
char *pchCont;

if(!iInit)
  {
  szIn=(char *)malloc(32768L);
  if(!szIn)
    {
    szGapRec[0]=0;
    fprintf(stderr,
      "\n ERROR: malloc failed in iGetGapRec.\n\n");
    exit(EXIT_FAILURE);
    }
  iInit=1;
  }

fgets(szIn, 32000, fpIn);
if(feof(fpIn))return(-1);
if(!iRecordValid(szIn))
  {
  strcpy(szGapRec, szIn);
  return(0);
  }
szTrimTWS(szIn);
pchCont=strpbrk(szIn+24, "_~\\");  /* Continuation lines coming? */
if(pchCont)*pchCont=0;
strcpy(szGapRec, szIn);
while(pchCont)
  {
  fgets(szIn, 32000, fpIn);
  if(feof(fpIn))return(-1);
  szTrimTWS(szIn);
  pchCont=strpbrk(szIn+24, "_~\\");  /* More continuation lines coming? */
  if(pchCont)*pchCont=0;
  strcat(szGapRec, szIn+40);
  }
return(1);
}
/**********************************************************************/
int iRecordValid(char *szRec)
{
/* Returns 0 for failure, 1 for success. Continuation lines are
   returned as invalid. Merit is not checked. */

static char szTemp[41];
long    sl;
float   f;

if(strlen(szRec) < 41)return(0);
strncpy(szTemp, szRec, 40);
szTemp[40]=0;
sl=strtol(szTemp, NULL, 0);  /* gap field */
if(sl < 1)return(0);
if((sl&1==1) && (sl != 1))return(0);
sl=strtol(szTemp+20, NULL, 0);  /* year field */
if(sl==0)return(0);
sl=strtol(szTemp+32, NULL, 0);  /* digits field */
if(sl < 1)return(0);
if(*(szTemp+ 7) != ' ')return(0);
if(*(szTemp+11) != ' ')return(0);
if(*(szTemp+20) != ' ')return(0);
if(*(szTemp+38) != ' ')return(0);
if(*(szTemp+39) != ' ')return(0);

return(1);
}
/**********************************************************************/
void vGapCont(char *szContRec, char *szGapRec)
{
/* NOTE: No terminating newline is added. */

static char szSpacer[]="                                        ";
unsigned long ulLen, ulOffset=40;

szTrimTWS(szGapRec);
ulLen=strlen(szGapRec);
if(ulLen < 241)
  {
  strcpy(szContRec, szGapRec);
  return;
  }
strncpy(szContRec, szGapRec, 40);
szContRec[40]=0;
while(1)
  {
  strncat(szContRec, szGapRec + ulOffset, 200);
  ulOffset += 200;
  if(ulOffset >= ulLen)break;
  strcat(szContRec, "\\\n");
  strcat(szContRec, szSpacer);
  }
return;
}
/**********************************************************************/
/**********************************************************************/
/*                      miscellaneous routines                        */
/**********************************************************************/
/**********************************************************************/
unsigned long __ulMem(void)
{
/* Reports the amount of available memory (may include some virtual
   memory). Usually the result will be truncated to the next lower
   million bytes (unless the amount available is < 1000000 bytes).
   The technique simply makes iterative calls to malloc and free. */

char *puch;
int i;
unsigned long ul;
int64_t sll;

sll=64000000UL;
puch=(char *)malloc(sll);
while(puch)
  {
  free(puch);
  sll += 10000000UL;
  if(sll > INT32_MAX)return(sll - 10000000UL);
  puch=(char *)malloc(sll);
  if(!puch)
    {
    while(1)
      {
      sll -= 1000000UL;
      if(sll < 1000000UL)
	{
	for(i=15; i > 0; i--)
	  {
	  sll=i*64000UL;
	  puch=(char *)malloc(sll);
	  if(puch)
	    {
	    free(puch);
	    return(sll);
	    }
	  }
	return(0);  /* Actually means < 64000 bytes */
	}
      puch=(char *)malloc(sll);
      if(puch)
	{
	free(puch);
	return(sll);
	}
      }
    }
  }

while(!puch)
  {
  sll -= 9000000UL;
  if(sll < 1000000UL)
    {
    for(i=15; i > 0; i--)
      {
      sll=i*64000UL;
      puch=(char *)malloc(sll);
      if(puch)
	{
	free(puch);
	return(sll);
	}
      }
    return(0);  /* Actually means < 64000 bytes */
    }
  puch=(char *)malloc(sll);
  if(puch)
    {
    while(1)
      {
      free(puch);
      sll += 1000000UL;
      if(sll > INT32_MAX)return(sll - 1000000UL);
      puch=(char *)malloc(sll);
      if(!puch)return(sll - 1000000UL);
      }
    }
  }
return(0);
}
/**********************************************************************/
unsigned long __ulPhysicalMemoryAvailable(void)
{
#if defined(__DJGPP__)
  return((unsigned long)_go32_dpmi_remaining_physical_memory());
#elif defined(__WIN32__)  /* Borland, DM, MinGW, Cygwin */

  MEMORYSTATUS mstMemStat;

  mstMemStat.dwLength = sizeof(mstMemStat);
  GlobalMemoryStatus(&mstMemStat);
  return(mstMemStat.dwAvailPhys);

#elif defined(__LINUX__)
  return(
    ((unsigned long)sysconf(_SC_AVPHYS_PAGES))
      *((unsigned long)sysconf(_SC_PAGESIZE)) );
#else
  return(0);
#endif
}
/**********************************************************************/
double lfPMA(void)
{
/* Returns the number of bytes of available physical memory. The return
   value is a double to accommodate systems with very large memories.
   Returns -1.0 upon failure. */

#if defined(__DJGPP__)

  return((double)_go32_dpmi_remaining_physical_memory());

#elif defined(__LINUX__)

  return(
    ((double)sysconf(_SC_AVPHYS_PAGES))
      *((double)sysconf(_SC_PAGESIZE)) );

#elif defined(__WIN32__)  /* MinGW, Cygwin, DM, Borland C 5.00+ */

  #if defined(__BORLANDC__)  /* Version 5.00 or later assumed */

  MEMORYSTATUSEX mstMemStatEx;

  mstMemStatEx.dwLength = sizeof(mstMemStatEx);
  GlobalMemoryStatusEx(&mstMemStatEx);
  return((double)mstMemStatEx.ullAvailPhys);

  #else  /* ...not Borland C */

  #include <winbase.h>

  /* The following entities are specified in case they were
     not declared in <winbase.h>. */

  typedef uint64_t DWORDLONG;
  typedef struct _MEMORYSTATUSEX
    {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORDLONG ullTotalPhys;
    DWORDLONG ullAvailPhys;
    DWORDLONG ullTotalPageFile;
    DWORDLONG ullAvailPageFile;
    DWORDLONG ullTotalVirtual;
    DWORDLONG ullAvailVirtual;
    DWORDLONG ullAvailExtendedVirtual;
    }
  MEMORYSTATUSEX, *LPMEMORYSTATUSEX;

  MEMORYSTATUS mstMemStat;
  MEMORYSTATUSEX mstMemStatEx;
  HANDLE hModule;
  FARPROC GMSX;

  mstMemStatEx.dwLength = sizeof(mstMemStatEx);
  hModule=LoadLibrary("kernel32.dll");
  if(hModule)
    {
    GMSX=GetProcAddress(hModule, "GlobalMemoryStatusEx");
    if(GMSX)
      {
      GMSX(&mstMemStatEx);
      return((double)mstMemStatEx.ullAvailPhys);
      }
    }

  /* If we arrive here, the LoadLibrary and/or GetProcAddress
     calls have failed. Default to the old GlobalMemoryStatus
     routine. */

  mstMemStat.dwLength = sizeof(mstMemStat);
  GlobalMemoryStatus(&mstMemStat);
  return((double)mstMemStat.dwAvailPhys);

  #endif  /* Borland C 5.00+ */

#else

  return(-1.0);

#endif
}
/***********************************************************************/
int iSignum(long double ldArg)
{
if(ldArg==0)return(0);
return((ldArg > 0) ? +1 : -1);
}
/**********************************************************************/
void vFlush(void)
{
fflush(NULL);

#if defined(__DJGPP__) || defined(__LINUX__) || defined(__CYGWIN__)
sync();
#endif

return;
}
/**********************************************************************/
void __vSleep(double lfSeconds)
{
double t0, dt=0;

t0=lfSeconds2();
while(dt < lfSeconds)
  dt=lfSeconds2() - t0;
return;
}
/**********************************************************************/
int __iXOpen(FILE **fpp, char *szFullName, const char *szBaseName,
  const char *szMode)
{
/* Attempts to open the file whose basename is szBaseName, in the mode
   specified by szMode. The specified and current directory are
   searched, as well as all the directories in the PATH environment
   variable. szBaseName can in fact contain a directory and drive
   specification, in which case the true basename will be extracted
   if szBaseName itself is not accessible.

   Upon success, *fpp is the valid FILE pointer, the return value
     (formerly 1) is the file descriptor (often referred to as the
     file handle), and szFullName (if not NULL on input) is set to
     the path specification which produced a valid open.
   Upon failure, *fpp is NULL, the return value is -1 (formerly 0),
     and szFullName is NULL.
*/

char *pch, ch, szBase[512], szPath[512], szName[512];

/* Try the exact szBaseName first. */

if(szFullName)strcpy(szFullName, szBaseName);
*fpp=fopen(szBaseName, szMode);
if(*fpp)return(fileno(*fpp));

#ifdef __LINUX__

/* Retrieve the true basename from szBaseName. Strip off the
   characters up to the last '/'. */

if(szFullName)szFullName[0]=0;
strcpy(szBase, szBaseName);
pch=strrchr(szBase, '/');
if(pch)
  {
  if(++pch)
    strcpy(szBase, pch);
  else
    {
    *fpp=NULL;
    return(-1);
    }
  }

/* Check the current directory first. Use szPath as a temporary
   variable. */

if(szFullName)strcpy(szFullName, szBase);
*fpp=fopen(szBase, szMode);
if(*fpp)return(fileno(*fpp));

/* Not in the specified or current directory; search along the PATH. */

pch=getenv("PATH");
if(!pch)return(-1);
strcpy(szPath, pch);
pch=strtok(szPath, ":");
while(pch)
  {
  strcpy(szName, pch);
  if(szFullName)strcpy(szFullName, szName);
  ch=szName[strlen(szName) - 1];
  if(ch != '/')
    {
    if(strstr(szName, "/"))
      strcat(szName, "/");
    else
      strcat(szName, "/");
    }
  strcat(szName, szBase);
  if(szFullName)strcpy(szFullName, szName);
  *fpp=fopen(szName, szMode);
  if(*fpp)return(fileno(*fpp));
  pch=strtok(NULL, ":");
  }

*fpp=NULL;
if(szFullName)szFullName[0]=0;

return(-1);  /* failure */

#endif
#ifndef LINUX  /* Win32 and DOS version */

/* Retrieve the true basename from szBaseName. Strip off the
   characters up to the ':' in the drive specifier, then up
   to the last '\\', then up to the last '/'. */

if(szFullName)szFullName[0]=0;
strcpy(szBase, szBaseName);
pch=strrchr(szBase, ':');
if(pch)
  {
  if(++pch)
    strcpy(szBase, pch);
  else
    {
    *fpp=NULL;
    return(-1);
    }
  }

pch=strrchr(szBase, '\\');
if(pch)
  {
  if(++pch)
    strcpy(szBase, pch);
  else
    {
    *fpp=NULL;
    return(-1);
    }
  }

pch=strrchr(szBase, '/');
if(pch)
  {
  if(++pch)
    strcpy(szBase, pch);
  else
    {
    *fpp=NULL;
    return(-1);
    }
  }

/* Check the current directory first. Use szPath as a temporary
   variable. */

if(szFullName)strcpy(szFullName, szBase);
*fpp=fopen(szBase, szMode);
if(*fpp)return(fileno(*fpp));

/* Not in the specified or current directory; search along the PATH. */

pch=getenv("PATH");
if(!pch)return(-1);
strcpy(szPath, pch);
pch=strtok(szPath, ";\0");
while(pch)
  {
  strcpy(szName, pch);
  if(szFullName)strcpy(szFullName, szName);
  ch=szName[strlen(szName) - 1];
  if((ch != '\\') && (ch != '/'))
    {
    if(strstr(szName, "\\"))
      strcat(szName, "\\");
    else
      strcat(szName, "/");
    }
  strcat(szName, szBase);
  if(szFullName)strcpy(szFullName, szName);
  *fpp=fopen(szName, szMode);
  if(*fpp)return(fileno(*fpp));
  pch=strtok(NULL, ";\0");
  }

*fpp=NULL;
if(szFullName)szFullName[0]=0;

return(-1);  /* failure */

#endif
}
/**********************************************************************/
#ifdef __DMC__
  #ifndef S_IRUSR
    #define S_IRUSR 0x0100
  #endif
  #ifndef S_IWUSR
    #define S_IWUSR 0x0080
  #endif
#endif /* __DMC__ */
/**********************************************************************/
long __lFile(char *szFileName)
{
/* Returns filesize in bytes if the file exists as a regular file
   (e.g., not a directory or volume label); returns -1 otherwise. */

int iExists;
struct stat st;

iExists = !stat(szFileName, &st);
if(iExists && S_ISREG(st.st_mode))return(st.st_size);
return(-1);
}
/**********************************************************************/
long __lRFile(char *szFileName)
{
/* Returns filesize in bytes if the file exists as a regular file
  (e.g., not a directory or volume label) and the user has read
  privilege; returns -1 otherwise. */

int iExists;
struct stat st;

iExists = !stat(szFileName, &st);
if(iExists && S_ISREG(st.st_mode) && (st.st_mode & S_IRUSR))
  return(st.st_size);
return(-1);
}
/**********************************************************************/
long __lRWFile(char *szFileName)
{
/* Returns filesize in bytes if the file exists as a regular file
   (e.g., not a directory or volume label) and the user has read
   and write privileges; returns -1 otherwise. */

int iExists;
struct stat st;

iExists = !stat(szFileName, &st);
if(iExists && S_ISREG(st.st_mode) && (st.st_mode & S_IRUSR) &&
    (st.st_mode & S_IWUSR))return(st.st_size);
return(-1);
}
/**********************************************************************/
void __vREF(char *szFileName)
{
/* Delete a zero-length file. */

if(__lFile(szFileName)==0)
  {
  vFlush();
  remove(szFileName);
  vFlush();
  }
return;
}
/**********************************************************************/
double lfSeconds2(void)
{
/* NOTE: The peculiar design of this routine is due to MinGW's
   inablity to correctly parse the preprocessor directive
   #if (CLOCKS_PER_SEC <= 1000) and similar directives.

   Returns the number of seconds elapsed since some fixed event,
   dependent upon the function call and platform. The clock()
   based routine normally returns the number of seconds since
   either the beginning of program execution or the first call
   to the function. The gettimeofday and time(NULL) based routines
   return the number of seconds since the beginning of the UNIX epoch
   (00:00:00 GMT 1 Jan 1970). The Win32 GetTickCount returns the
   number of milliseconds since system boot (in reality, resolution
   may be as poor as +/- 55 ms); QueryPerformanceCounter can
   theoretically achieve nanosecond resolution. The granularity of the
   clock() routine is generally either 0.01 sec or 0.055 sec. The
   granularity of gettimeofday is nominally 1 microsecond, but
   in reality 0.01 second is more common. The granularity of
   time(NULL) is 1 second.

   PORTABILITY AND BUGS: The clock() and time(NULL) functions are
   part of standard C. The gettimeofday function is not part of
   standard C, but is available on most platforms. GetTickCount()
   and QueryPerformanceCounter(*LARGE_INTEGER) are part of Win32.
   GetTickCount has maximum compatibility, supported at least
   since Windows 3.0 (1991). QueryPerformanceCounter is a more
   recent addition to the Windows API, available since at least
   December, 2002; it is designed to take advantage of the
   Time Stamp Counter (TSC) on Pentium compatible processors,
   and has a potential resolution level in the nanosecond range.

   Known bugs in clock() include the rollover problem, which
   will usually cause INT32_MAX to rollover to INT32_MIN after
   2^31 ticks. This is a major problem on systems (including many
   GNU/Linux systems) which comply with the P*SIX standard
   CLOCKS_PER_SECOND=1000000; then the first rollover occurs after
   less than 36 minutes. Rollover results in clock() failing to be
   monotonic increasing, so that simply differencing clock() values
   may not reflect the true time difference (and may even generate
   a ridiculous negative time difference). Rollover can generally
   be ignored on systems where CLOCKS_PER_SECOND <= 1000, as
   rollover will take at least 24.85 days. Otherwise, it must be
   trapped in the routine, and this can become quite problematical
   because of the possibility of multiple rollovers and masked
   rollovers. Cygwin's clock() is avoided, as it yields erroneous
   values (typically twice the correct value) on some systems.

   Bugs in gettimeofday have been reported by several users; these
   are either "backward jumps" in value in rare instances, or
   anomalous values returned at local midnight and then quickly
   self-correcting. More recent versions of gettimeofday (starting
   with GNU/Linux 2.4) appear to be more reliable, but I have
   observed the midnight anomaly on my own Windows systems, using
   the gettimeofday in DJGPP 2.03. It appears to have no rollover
   problem, although one may be coming in 2038, when the UNIX
   epoch attains 2^31 seconds.

   The only bugs of which I am aware in time(NULL) are a midnight
   rollover anomaly, similar to the one exhibited by gettimeofday,
   on some Windows systems; and the same Y2K type problem
   looming in 2038. Its huge disadvantage is the poor granularity.

   As a last resort, time(NULL) is returned, with a granularity
   of only one second.

   The clock() routine has a correction factor to compensate for
   DJGPP's use of 91/5 PC clock ticks per second (the correct
   value is 1193180/65536 = 18.2046819336). */

static double lftPrevious=-1e308, lft, dt;

/* lftPrevious stores the value from the previous call. If the
   next computed value is slightly *less* than lftPrevious, it
   is almost certainly the result of an interfering OS layer
   blocking direct access to the hardware clock, and lftPrevious
   is returned instead, in an effort to keep the function
   monotonic non-decreasing (and time intervals non-negative).
   If lftPrevious is significantly less than the computed lft,
   it is probably due to a system clock rollover, for which
   there is no simple remedy; in that case, the inconsistent
   present value is returned (with apologies). NOTE: -DBL_MAX
   is avoided as the value of the initializer above due to a
   bug in the DJGPP compilers. */

#if defined(__WIN32__)  /* MinGW, Cygwin, DM, Borland */
  #if defined(WINVER) && (WINVER >= 0x0400)
  /* Thanks to Huang Yuanbing <bailuzhou(at)163(dot)com> for this
     code fragment, adapted from his PrimeNumber.cpp code
     (12 Feb 2009). The preceding conditional is a fudged test
     for the availability of the QueryPerformanceXXX routines, to
     be replaced by a more discriminating one if and when found. */
    static LARGE_INTEGER s_freq;
    LARGE_INTEGER performanceCount;
    if(s_freq.QuadPart == 0 && !QueryPerformanceFrequency(&s_freq))
      {
      /* Performance counter has failed; fall back on GetTickCount */
      lft=GetTickCount()/1000.0;
      dt=lft-lftPrevious;
      if((dt >= 0) || (dt < -1))
	{
	lftPrevious=lft;
	return(lft);
	}
      else
	return(lftPrevious);  /* guard against non-monotonic clock */
      }
    QueryPerformanceCounter(&performanceCount);
    return(performanceCount.QuadPart / (double)s_freq.QuadPart);
  #else
    lft=GetTickCount()/1000.0;
    dt=lft-lftPrevious;
    if((dt >= 0) || (dt < -1))
      {
      lftPrevious=lft;
      return(lft);
      }
    else
      return(lftPrevious);  /* guard against non-monotonic clock */
  #endif
#elif defined(CLOCKS_PER_SEC)
  static unsigned long ulCPS=CLOCKS_PER_SEC;
  if(ulCPS <= 1000)
    {
    lft=clock()/((double)CLOCKS_PER_SEC);
  #ifdef __DJGPP__
    if(ulCPS==91)lft=0.9996439766*lft;
  #endif
    dt=lft-lftPrevious;
    if((dt >= 0) || (dt < -1))
      {
      lftPrevious=lft;
      return(lft);
      }
    else
      return(lftPrevious);
    }
  else
    {
  #if defined(__LINUX__)
    static struct timeval tv;
    gettimeofday(&tv, NULL);
    lft=tv.tv_sec + tv.tv_usec/1000000.0;
    dt=lft-lftPrevious;
    if((dt >= 0) || (dt < -1))
      {
      lftPrevious=lft;
      return(lft);
      }
    else
      return(lftPrevious);
  #else
    return(time(NULL));  /* last resort */
  #endif
    }
#endif
}
/**********************************************************************/
char *__strlwr(char *sz)  /* For LINUX */
{
char *pch;

for(pch=sz; *pch; pch++)*pch=tolower((unsigned char)*pch);
return(sz);
}
/**********************************************************************/
char *__strupr(char *sz)  /* For LINUX */
{
char *pch;

for(pch=sz; *pch; pch++)*pch=toupper((unsigned char)*pch);
return(sz);
}
/**********************************************************************/
unsigned long ulSqrt(uint64_t ull)
{
/* Computes the (integer truncated) square root of ull, using a
 * Newton-Raphson method. Returns floor(sqrt(ull)).
 *
 * Adapted from a similar routine in the BIGINT ultraprecision
 * integer package, developed (1988-91) and graciously placed in the
 * public domain by Arjen K. Lenstra, Mark Riordan, and Marc Ringuette.
 * The original BIGINT package is available (19 March 2001) at
 * <http://www.funet.fi/pub/crypt/cryptography/rpem/rpem/>
 * (thanks to Charles Doty for this pointer).
 *
 */

uint64_t ull2, ull3, ull4;

if(ull==0)return(0);
if(ull < 4)return(1);
if(ull < 9)return(2);
if(ull >= ((uint64_t)UINT32_MAX)*((uint64_t)UINT32_MAX))return(UINT32_MAX);
ull2=ull/2;
while(1)
  {
  ull3=ull/ull2;
  ull4=(ull3 + ull2)/2;
  if((ull4 - ull3 < 2) || (ull3 - ull4 < 2))
    if(ull4*ull4 <= ull)
      return(ull4);
    else
      return(ull3);
  ull2=ull4;
  }
}
/**********************************************************************/
#if defined(__CYGWIN__) || defined(__LINUX__) || \
   (defined(__BORLANDC__) && defined(__WIN32__))
/* Emulation of these functions for Cygwin and Linux. */
/**********************************************************************/
int __cputs(const char *sz)
{
fprintf(stderr, sz);
fflush(stderr);
return(0);
}
/**********************************************************************/
#endif  /* CYGWIN or LINUX */
/**********************************************************************/
void vAtExit(void)
{
__OBSL__;
return;
}
/**********************************************************************/
#ifdef __GMP__
/**********************************************************************/
/**********************************************************************/
/*       iEvalExprMPZ --- expression parser for mpz bigints           */
/**********************************************************************/
/**********************************************************************/
/* parser.c    GCC 4.52    GMP 4.31    MinGW 3.9    2016.03.31.0500

Procedure for evaluating integer expressions in string form to multiple
precision integers (mpz_t) using the GNU Multiple Precision Arithmetic
Library (GMP).

Copyright 1997-2016 Free Software Foundation, Inc. (FSF) under the
terms of the GNU General Public License (GPL). Mailing address:
51 Franklin Street, Fifth Floor, Boston MA 02110-1301 USA. See also
<http://www.gnu.org/licenses/licenses.html> and
<http://www.gnu.org/fsf/fsf.html>.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU GPL as published by the FSF; either
version 2 of the license, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
GPL for additional details. */

/* This code is derived from PEXPR.C, received as part of the DJGPP
   GMP 4.01 package, downloaded April 2002 from the Arlington VA AOL
   mirror site. Modified 2002-2016 by Thomas R. Nicely
   <http://www.trnicely.net> from a standalone to a callable procedure.
   The error jumptables were eliminated in favor of an error return
   value, and the timing and printing options were removed (except
   after fatal errors). The procedure free_expr has been disabled
   (it simply returns) to alleviate an untraced fatal SIGSEGV violation,
   at the cost of a memory leak. This is one of many problems encountered
   with GMP's allocation, re-allocation, and memory clearing algorithms;
   either the GMP source code must be cleaned up, or workarounds must be
   introduced into each application code (as TRN has done in the
   preceding routines). However, iEvalExprMPZ, having been written by
   other parties, has largely been left "as is"; in general, TRN treats
   this code as a "black box"; until it breaks again, don't fix it.

   Nicely's contributions are assigned to the public domain; he
   disclaims any legal liability arising from their use. */

/* This expression evaluator works by building an expression tree (using a
   recursive descent parser) which is then evaluated.  The expression tree
   is useful since we want to optimize certain expressions (like a^b % c).

   int iEvalExprMPZ(mpz_t mpzResult, char *szExpression)

   Success returns zero. A return of -1 indicates failure, in which case
   mpzResult will be set to -1 (in older versions mpz was set to zero).
   However, any mpzResult=0 should be viewed with suspicion (as I said,
   parts of the code are still a "black box"); you might try calling
   atol(szExpression) or atof(szExpression) as a double-check.

   The expression may be in C or BASIC format, with some exceptions.
   Note that the exponentiation operator is "^" and the modulus
   operator is "%".  The primorial operator is "#" (unary postfix,
   like the factorial operator "!").

   An additional feature is available if the code is linked with
   the MPFR library (preferably version 2.4.1 or later): Literal
   integer constants can be expressed in floating-point form,
   such as 3.1416e9. */

#ifdef __THIS_MODULE_STANDS_ALONE__
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>
  #include <gmp.h>
  #include <mpfr.h>  /* optional */
#endif

#undef _PROTO
#if __GMP_HAVE_PROTOTYPES
#  define _PROTO(x) x
#else
#  define _PROTO(x) ()
#endif

enum op_t {NOP, LIT, NEG, NOT, PLUS, MINUS, MULT, DIV, MOD, REM, INVMOD, POW,
	   AND, IOR, XOR, SLL, SRA, POPCNT, HAMDIST, GCD, LCM, SQRT, ROOT, FAC,
	   LOG, LOG2, FERMAT, MERSENNE, FIBONACCI, RANDOM, NEXTPRIME, FLOOR,
           PRIMORIAL};

/* Type for the expression tree. */
struct expr
{
  enum op_t op;
  union
  {
    struct {struct expr *lhs, *rhs;} ops;
    mpz_t val;
  } operands;
};
typedef struct expr *expr_t;

struct functions
{
  char *spelling;
  enum op_t op;
  int arity; /* 1 or 2 means real arity; 0 means arbitrary.  */
};
static struct functions fns[] =
{
  {"sqrt", SQRT, 1},
#if __GNU_MP_VERSION >= 2
  {"root", ROOT, 2},
  {"popc", POPCNT, 1},
  {"hamdist", HAMDIST, 2},
#endif
  {"gcd", GCD, 0},
#if __GNU_MP_VERSION > 2 || __GNU_MP_VERSION_MINOR >= 1
  {"lcm", LCM, 0},
#endif
  {"and", AND, 0},
  {"ior", IOR, 0},
#if __GNU_MP_VERSION > 2 || __GNU_MP_VERSION_MINOR >= 1
  {"xor", XOR, 0},
#endif
  {"plus", PLUS, 0},
  {"pow", POW, 2},
  {"minus", MINUS, 2},
  {"mul", MULT, 0},
  {"div", DIV, 2},
  {"mod", MOD, 2},
  {"rem", REM, 2},
#if __GNU_MP_VERSION >= 2
  {"invmod", INVMOD, 2},
#endif
  {"log", LOG, 2},
  {"log2", LOG2, 1},
  {"F", FERMAT, 1},
  {"M", MERSENNE, 1},
  {"fib", FIBONACCI, 1},
  {"Fib", FIBONACCI, 1},
  {"random", RANDOM, 1},
  {"nextprime", NEXTPRIME, 1},
  {"floor", FLOOR, 1},
  {"", NOP, 0}
};

int iEvalExprMPZ(mpz_t mpzResult, char *szExpression);
static char *skipspace _PROTO ((char *));
static void makeexp _PROTO ((expr_t *, enum op_t, expr_t, expr_t));
static void free_expr _PROTO ((expr_t));
static char *expr _PROTO ((char *, expr_t *));
static char *term _PROTO ((char *, expr_t *));
static char *power _PROTO ((char *, expr_t *));
static char *factor _PROTO ((char *, expr_t *));
static int match _PROTO ((char *, char *));
static int matchp _PROTO ((char *, char *));
static void mpz_eval_expr _PROTO ((mpz_ptr, expr_t));
static void mpz_eval_mod_expr _PROTO ((mpz_ptr, expr_t, mpz_ptr));

char *error, *szExpressionString;
int iGlobalError;
gmp_randstate_t rstate;

/**********************************************************************/
int iEvalExprMPZ(mpz_t mpzResult, char *szExpression)
{
char *str, *pchAstAst, *szCopy;
struct expr *e;

iGlobalError=0;
gmp_randinit(rstate, GMP_RAND_ALG_LC, 128);
szExpressionString=(char *)malloc(strlen(szExpression) + 6);
strcpy(szExpressionString, szExpression);

#if 0

/* The following statements have been replaced (by the single
   active gmp_randseed_ui statement following) to avoid problems
   caused by the absence of gettimeofday on some platforms: */

   struct timeval tv;
   gettimeofday(&tv, NULL);
   gmp_randseed_ui(rstate, tv.tv_sec + tv.tv_usec);

#endif

gmp_randseed_ui(rstate, 1 + time(NULL)*(314159311UL + clock()));
szCopy=(char *)malloc(strlen(szExpression) + 6);
/* Remove all white space from a copy of the input. The original
   input itself will not be modified, only the copy, whose value
   will be returned. */
vRemoveWS(szCopy, szExpressionString);
pchAstAst=strstr(szCopy, "**");  /* Replace Fortran/Cobol "**" by "^" */
while(pchAstAst)
  {
  *pchAstAst=' ';
  *(pchAstAst+1)='^';
  pchAstAst=strstr(pchAstAst+2, "**");
  }
str=expr(szCopy, &e);
if (str[0] != 0)
  {
  mpz_set_si(mpzResult, -1);
  iGlobalError=-1;
  free_expr(e);
  free(szCopy);
  free(szExpressionString);
  return(iGlobalError);
  }
else
  {
  mpz_eval_expr(mpzResult, e);
  free_expr(e);
  free(szCopy);
  free(szExpressionString);
  return(iGlobalError);
  }
}
/**********************************************************************/
static char *expr (char *str, expr_t *e)
{
  expr_t e2;

  str = skipspace (str);
  if (str[0] == '+')
    {
      str = term (str + 1, e);
    }
  else if (str[0] == '-')
    {
      str = term (str + 1, e);
      makeexp (e, NEG, *e, NULL);
    }
  else if (str[0] == '~')
    {
      str = term (str + 1, e);
      makeexp (e, NOT, *e, NULL);
    }
  else
    {
      str = term (str, e);
    }

  for (;;)
    {
      str = skipspace (str);
      switch (str[0])
	{
	case 'p':
	  if (match ("plus", str))
	    {
	      str = term (str + 4, &e2);
	      makeexp (e, PLUS, *e, e2);
	    }
	  else
	    return str;
	  break;
	case 'm':
	  if (match ("minus", str))
	    {
	      str = term (str + 5, &e2);
	      makeexp (e, MINUS, *e, e2);
	    }
	  else
	    return str;
	  break;
	case '+':
	  str = term (str + 1, &e2);
	  makeexp (e, PLUS, *e, e2);
	  break;
	case '-':
	  str = term (str + 1, &e2);
	  makeexp (e, MINUS, *e, e2);
	  break;
	default:
	  return str;
	}
    }
}
/**********************************************************************/
static char *term (char *str, expr_t *e)
{
  expr_t e2;

  str = power (str, e);
  for (;;)
    {
      str = skipspace (str);
      switch (str[0])
	{
	case 'm':
	  if (match ("mul", str))
	    {
	      str = power (str + 3, &e2);
	      makeexp (e, MULT, *e, e2);
	      break;
	    }
	  if (match ("mod", str))
	    {
	      str = power (str + 3, &e2);
	      makeexp (e, MOD, *e, e2);
	      break;
	    }
	  return str;
	case 'd':
	  if (match ("div", str))
	    {
	      str = power (str + 3, &e2);
	      makeexp (e, DIV, *e, e2);
	      break;
	    }
	  return str;
	case 'r':
	  if (match ("rem", str))
	    {
	      str = power (str + 3, &e2);
	      makeexp (e, REM, *e, e2);
	      break;
	    }
	  return str;
	case 'i':
	  if (match ("invmod", str))
	    {
	      str = power (str + 6, &e2);
	      makeexp (e, REM, *e, e2);
	      break;
	    }
	  return str;
	case 't':
	  if (match ("times", str))
	    {
	      str = power (str + 5, &e2);
	      makeexp (e, MULT, *e, e2);
	      break;
	    }
	  if (match ("thru", str))
	    {
	      str = power (str + 4, &e2);
	      makeexp (e, DIV, *e, e2);
	      break;
	    }
	  if (match ("through", str))
	    {
	      str = power (str + 7, &e2);
	      makeexp (e, DIV, *e, e2);
	      break;
	    }
	  return str;
	case '*':
	  str = power (str + 1, &e2);
	  makeexp (e, MULT, *e, e2);
	  break;
	case '/':
	  str = power (str + 1, &e2);
	  makeexp (e, DIV, *e, e2);
	  break;
	case '%':
	  str = power (str + 1, &e2);
	  makeexp (e, MOD, *e, e2);
	  break;
	default:
	  return str;
	}
    }
}
/**********************************************************************/
static char *power (char *str, expr_t *e)
{
  expr_t e2;

  str = factor (str, e);
  while (str[0] == '!')
    {
      str++;
      makeexp (e, FAC, *e, NULL);
    }
  while (str[0] == '#')
    {
      str++;
      makeexp (e, PRIMORIAL, *e, NULL);
    }
  str = skipspace (str);
  if (str[0] == '^')
    {
      str = power (str + 1, &e2);
      makeexp (e, POW, *e, e2);
    }

  return str;
}
/**********************************************************************/
static int match (char *s, char *str)
{
  char *ostr = str;
  int i;

  for (i = 0; s[i] != 0; i++)
    {
      if (str[i] != s[i])
	return 0;
    }
  str = skipspace (str + i);
  return str - ostr;
}
/**********************************************************************/
static int matchp (char *s, char *str)
{
  char *ostr = str;
  int i;

  for (i = 0; s[i] != 0; i++)
    {
      if (str[i] != s[i])
	return 0;
    }
  str = skipspace (str + i);
  if (str[0] == '(')
    return str - ostr + 1;
  return 0;
}
/**********************************************************************/
static char *factor (char *str, expr_t *e)
{
  expr_t e1, e2;

  str = skipspace (str);

  if (isalpha (str[0]))
    {
      int i;
      int cnt;

      for (i = 0; fns[i].op != NOP; i++)
	{
	  if (fns[i].arity == 1)
	    {
	      cnt = matchp (fns[i].spelling, str);
	      if (cnt != 0)
		{
		  str = expr (str + cnt, &e1);
		  str = skipspace (str);
		  if (str[0] != ')')
		    {
		      iGlobalError=-1;
                      return("1");
		    }
		  makeexp (e, fns[i].op, e1, NULL);
		  return str + 1;
		}
	    }
	}

      for (i = 0; fns[i].op != NOP; i++)
	{
	  if (fns[i].arity != 1)
	    {
	      cnt = matchp (fns[i].spelling, str);
	      if (cnt != 0)
		{
		  str = expr (str + cnt, &e1);
		  str = skipspace (str);

		  if (str[0] != ',')
		    {
		      iGlobalError=-1;
                      return("1");
		    }

		  str = skipspace (str + 1);
		  str = expr (str, &e2);
		  str = skipspace (str);

		  if (fns[i].arity == 0)
		    {
		      while (str[0] == ',')
			{
			  makeexp (&e1, fns[i].op, e1, e2);
			  str = skipspace (str + 1);
			  str = expr (str, &e2);
			  str = skipspace (str);
			}
		    }

		  if (str[0] != ')')
		    {
		      iGlobalError=-1;
                      return("1");
		    }

		  makeexp (e, fns[i].op, e1, e2);
		  return str + 1;
		}
	    }
	}
    }

  if (str[0] == '(')
    {
      str = expr (str + 1, e);
      str = skipspace (str);
      if (str[0] != ')')
	{
	  iGlobalError=-1;
          return("1");
	}
      str++;
    }
  else if (str[0] >= '0' && str[0] <= '9')
    {
      expr_t res;
      char *s, *sc;

      res = (expr_t) malloc (sizeof (struct expr));
      res -> op = LIT;
      mpz_init (res->operands.val);

      s = str;
      while (isalnum (str[0]) || str[0]=='.')
	str++;
      sc = (char *) malloc (str - s + 1);
      memcpy (sc, s, str - s);
      sc[str - s] = 0;
#ifdef __MPFR__  /* To parse tokens such as 1.32e9 */
      {
      mpfr_t mpfr;
      mpfr_set_default_prec(332200);  /* 100,002 decimal digits */
      mpfr_init(mpfr);
      mpfr_set_str(mpfr, sc, 10, GMP_RNDN);
      mpfr_get_z(res->operands.val, mpfr, GMP_RNDN);
      mpfr_clear(mpfr);
      }
#else
      __mpz_set_str (res->operands.val, sc, 0);
#endif
      *e = res;
      free (sc);
    }
  else
    {
      iGlobalError=-1;
      return("1");
    }
  return str;
}
/**********************************************************************/
static char *skipspace(char *str)
{
  while (str[0] == ' ')
    str++;
  return str;
}
/**********************************************************************/
static void makeexp(expr_t *r, enum op_t op, expr_t lhs, expr_t rhs)
{
/* Make a new expression with operation OP and right hand side
   RHS and left hand side lhs.  Put the result in R.  */

  expr_t res;
  res = (expr_t) malloc (sizeof (struct expr));
  res -> op = op;
  res -> operands.ops.lhs = lhs;
  res -> operands.ops.rhs = rhs;
  *r = res;
  return;
}
/**********************************************************************/
static void free_expr(expr_t e)
{
/* Free the memory used by expression E.  */

  return;

/* Bug-fixing hack (T. R. Nicely 2003.10.10).  This routine was
   generating unpredictable crashes (non-trappable SIGSEGV violations).
   Signal trapping and other measures failed to solve the problem,
   and the ultimate fault has not been fixed or even pinned down.
   Disabling the routine leaves a potential memory leak in the
   program, which has thus far not been a problem. */

  if (e->op != LIT)
    {
      free_expr (e->operands.ops.lhs);
      if (e->operands.ops.rhs != NULL)
	free_expr (e->operands.ops.rhs);
    }
  else
    {
      mpz_clear (e->operands.val);
    }
  return;
}
/**********************************************************************/
static void mpz_eval_expr(mpz_ptr r, expr_t e)
{
/* Evaluate the expression E and put the result in R.  */

  int iFloor;
  mpz_t lhs, rhs, rem;

  switch (e->op)
    {
    case LIT:
      mpz_set (r, e->operands.val);
      return;
    case PLUS:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_add (r, lhs, rhs);
      mpz_clear (lhs); mpz_clear (rhs);
      return;
    case MINUS:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_sub (r, lhs, rhs);
      mpz_clear (lhs); mpz_clear (rhs);
      return;
    case MULT:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_mul (r, lhs, rhs);
      mpz_clear (lhs); mpz_clear (rhs);
      return;
    case DIV:
      mpz_init (lhs); mpz_init (rhs); mpz_init(rem);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_fdiv_qr(r, rem, lhs, rhs);
      if((!iNoDivWarnings) && mpz_sgn(rem) && !iFloor)
	/* rem != 0 and FLOOR not specified */
	{
	fprintf(stderr, "\n *** WARNING: non-exact division encountered"
	  " in expression %s (quotient rounding may be anomalous)\n",
	   szExpressionString);
	}
      mpz_clear (lhs); mpz_clear (rhs); mpz_clear(rem);
      return;
    case MOD:
      mpz_init (rhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_abs (rhs, rhs);
      mpz_eval_mod_expr (r, e->operands.ops.lhs, rhs);
      mpz_clear (rhs);
      return;
    case REM:
      /* Check if lhs operand is POW expression and optimize for that case.  */
      if (e->operands.ops.lhs->op == POW)
	{
	  mpz_t powlhs, powrhs;
	  mpz_init (powlhs);
	  mpz_init (powrhs);
	  mpz_init (rhs);
	  mpz_eval_expr (powlhs, e->operands.ops.lhs->operands.ops.lhs);
	  mpz_eval_expr (powrhs, e->operands.ops.lhs->operands.ops.rhs);
	  mpz_eval_expr (rhs, e->operands.ops.rhs);
	  mpz_powm (r, powlhs, powrhs, rhs);
	  if (mpz_cmp_si (rhs, 0L) < 0)
	    mpz_neg (r, r);
	  mpz_clear (powlhs);
	  mpz_clear (powrhs);
	  mpz_clear (rhs);
	  return;
	}
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_fdiv_r (r, lhs, rhs);
      mpz_clear (lhs); mpz_clear (rhs);
      return;
#if __GNU_MP_VERSION >= 2
    case INVMOD:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_invert (r, lhs, rhs);
      mpz_clear (lhs); mpz_clear (rhs);
      return;
#endif
    case POW:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      if (mpz_cmp_si (rhs, 0L) == 0)
	/* x^0 is 1 */
	mpz_set_ui (r, 1L);
      else if (mpz_cmp_si (lhs, 0L) == 0)
	/* 0^y (where y != 0) is 0 */
	mpz_set_ui (r, 0L);
      else if (mpz_cmp_ui (lhs, 1L) == 0)
	/* 1^y is 1 */
	mpz_set_ui (r, 1L);
      else if (mpz_cmp_si (lhs, -1L) == 0)
	/* (-1)^y just depends on whether y is even or odd */
	mpz_set_si (r, (mpz_get_ui (rhs) & 1) ? -1L : 1L);
      else if (mpz_cmp_si (rhs, 0L) < 0)
	/* x^(-n) is 0 */
	mpz_set_ui (r, 0L);
      else
	{
	  unsigned long int cnt;
	  unsigned long int y;
	  /* error if exponent does not fit into an unsigned long int.  */
	  if (mpz_cmp_ui (rhs, ~(unsigned long int) 0) > 0)
	    goto pow_err;

	  y = mpz_get_ui (rhs);
	  /* x^y == (x/(2^c))^y * 2^(c*y) */
#if __GNU_MP_VERSION >= 2
	  cnt = mpz_scan1 (lhs, 0);
#else
	  cnt = 0;
#endif
	  if (cnt != 0)
	    {
	      if (y * cnt / cnt != y)
		goto pow_err;
	      mpz_tdiv_q_2exp (lhs, lhs, cnt);
	      mpz_pow_ui (r, lhs, y);
	      mpz_mul_2exp (r, r, y * cnt);
	    }
	  else
	    mpz_pow_ui (r, lhs, y);
	}
      mpz_clear (lhs); mpz_clear (rhs);
      return;
    pow_err:
      error = "result of `pow' operator too large";
      mpz_clear (lhs); mpz_clear (rhs);
      iGlobalError=-1;
      mpz_set_ui(r,1);
      return;
    case GCD:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_gcd (r, lhs, rhs);
      mpz_clear (lhs); mpz_clear (rhs);
      return;
#if __GNU_MP_VERSION > 2 || __GNU_MP_VERSION_MINOR >= 1
    case LCM:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_lcm (r, lhs, rhs);
      mpz_clear (lhs); mpz_clear (rhs);
      return;
#endif
    case AND:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_and (r, lhs, rhs);
      mpz_clear (lhs); mpz_clear (rhs);
      return;
    case IOR:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_ior (r, lhs, rhs);
      mpz_clear (lhs); mpz_clear (rhs);
      return;
#if __GNU_MP_VERSION > 2 || __GNU_MP_VERSION_MINOR >= 1
    case XOR:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      mpz_xor (r, lhs, rhs);
      mpz_clear (lhs); mpz_clear (rhs);
      return;
#endif
    case NEG:
      mpz_eval_expr (r, e->operands.ops.lhs);
      mpz_neg (r, r);
      return;
    case NOT:
      mpz_eval_expr (r, e->operands.ops.lhs);
      mpz_com (r, r);
      return;
    case SQRT:
      mpz_init (lhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      if (mpz_sgn (lhs) < 0)
	{
	  error = "cannot take square root of negative numbers";
	  mpz_clear (lhs);
	  iGlobalError=-1;
          mpz_set_ui(r,1);
          return;
	}
      mpz_sqrt (r, lhs);
      return;
#if __GNU_MP_VERSION > 2 || __GNU_MP_VERSION_MINOR >= 1
    case ROOT:
      mpz_init (lhs); mpz_init (rhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      mpz_eval_expr (rhs, e->operands.ops.rhs);
      if (mpz_sgn (rhs) <= 0)
	{
	  error = "cannot take non-positive root orders";
	  mpz_clear (lhs); mpz_clear (rhs);
	  iGlobalError=-1;
          mpz_set_ui(r,1);
          return;
	}
      if (mpz_sgn (lhs) < 0 && (mpz_get_ui (rhs) & 1) == 0)
	{
	  error = "cannot take even root orders of negative numbers";
	  mpz_clear (lhs); mpz_clear (rhs);
	  iGlobalError=-1;
          mpz_set_ui(r,1);
          return;
	}

      {
	unsigned long int nth = mpz_get_ui (rhs);
	if (mpz_cmp_ui (rhs, ~(unsigned long int) 0) > 0)
	  {
	    /* If we are asked to take an awfully large root order, cheat and
	       ask for the largest order we can pass to mpz_root.  This saves
	       some error prone special cases.  */
	    nth = ~(unsigned long int) 0;
	  }
	mpz_root (r, lhs, nth);
      }
      mpz_clear (lhs); mpz_clear (rhs);
      return;
#endif
    case FAC:
      mpz_eval_expr (r, e->operands.ops.lhs);
      if (mpz_size (r) > 1)
	{
	  error = "result of `!' operator too large";
	  iGlobalError=-1;
          mpz_set_ui(r,1);
          return;
	}
      mpz_fac_ui (r, mpz_get_ui (r));
      return;
    case PRIMORIAL:
      mpz_eval_expr (r, e->operands.ops.lhs);
      if (mpz_size (r) > 1)
	{
	  error = "result of `#' operator too large";
	  iGlobalError=-1;
          mpz_set_ui(r,1);
          return;
	}
      { unsigned long ulArg;
        ulArg=mpz_get_ui(r);
        if(ulArg < 2){mpz_set_ui(r,1);return;}
        mpz_set_ui(r,2);
        if(ulArg < 3)return;
        mpz_init(lhs); mpz_init(rhs);
        mpz_set_ui(rhs, 2);
        while(1)
          {
          mpz_nextprime(lhs,rhs); /* rhs=prev_prime, lhs=next_prime */
          if(mpz_cmp_ui(lhs,ulArg) > 0)break;
          mpz_mul(r,lhs,r);
          mpz_set(rhs,lhs);
          }
        mpz_clear(lhs);mpz_clear(rhs);
      }
      return;
#if __GNU_MP_VERSION >= 2
    case POPCNT:
      mpz_eval_expr (r, e->operands.ops.lhs);
      { long int cnt;
	cnt = mpz_popcount (r);
	mpz_set_si (r, cnt);
      }
      return;
    case HAMDIST:
      { long int cnt;
        mpz_init (lhs); mpz_init (rhs);
	mpz_eval_expr (lhs, e->operands.ops.lhs);
	mpz_eval_expr (rhs, e->operands.ops.rhs);
	cnt = mpz_hamdist (lhs, rhs);
	mpz_clear (lhs); mpz_clear (rhs);
	mpz_set_si (r, cnt);
      }
      return;
#endif
    case LOG2:
      mpz_eval_expr (r, e->operands.ops.lhs);
      { unsigned long int cnt;
	if (mpz_sgn (r) <= 0)
	  {
	    error = "logarithm of non-positive number";
	    iGlobalError=-1;
            mpz_set_ui(r,1);
            return;
	  }
	cnt = mpz_sizeinbase (r, 2);
	mpz_set_ui (r, cnt - 1);
      }
      return;
    case LOG:
      { unsigned long int cnt;
	mpz_init (lhs); mpz_init (rhs);
	mpz_eval_expr (lhs, e->operands.ops.lhs);
	mpz_eval_expr (rhs, e->operands.ops.rhs);
	if (mpz_sgn (lhs) <= 0)
	  {
	    error = "logarithm of non-positive number";
	    mpz_clear (lhs); mpz_clear (rhs);
	    iGlobalError=-1;
            mpz_set_ui(r,1);
            return;
	  }
	if (mpz_cmp_ui (rhs, 256) >= 0)
	  {
	    error = "logarithm base too large";
	    mpz_clear (lhs); mpz_clear (rhs);
	    iGlobalError=-1;
            mpz_set_ui(r,1);
            return;
	  }
	cnt = mpz_sizeinbase (lhs, mpz_get_ui (rhs));
	mpz_set_ui (r, cnt - 1);
	mpz_clear (lhs); mpz_clear (rhs);
      }
      return;
    case FERMAT:
      {
	unsigned long int t;
	mpz_init (lhs);
	mpz_eval_expr (lhs, e->operands.ops.lhs);
	t = (unsigned long int) 1 << mpz_get_ui (lhs);
	if (mpz_cmp_ui (lhs, ~(unsigned long int) 0) > 0 || t == 0)
	  {
	    error = "too large Mersenne number index";
	    mpz_clear (lhs);
	    iGlobalError=-1;
            mpz_set_ui(r,1);
            return;
	  }
	mpz_set_ui (r, 1);
	mpz_mul_2exp (r, r, t);
	mpz_add_ui (r, r, 1);
	mpz_clear (lhs);
      }
      return;
    case MERSENNE:
      mpz_init (lhs);
      mpz_eval_expr (lhs, e->operands.ops.lhs);
      if (mpz_cmp_ui (lhs, ~(unsigned long int) 0) > 0)
	{
	  error = "too large Mersenne number index";
	  mpz_clear (lhs);
	  iGlobalError=-1;
          mpz_set_ui(r,1);
          return;
	}
      mpz_set_ui (r, 1);
      mpz_mul_2exp (r, r, mpz_get_ui (lhs));
      mpz_sub_ui (r, r, 1);
      mpz_clear (lhs);
      return;
    case FIBONACCI:
      { mpz_t t;
	unsigned long int n, i;
	mpz_init (lhs);
	mpz_eval_expr (lhs, e->operands.ops.lhs);
	if (mpz_sgn (lhs) <= 0 || mpz_cmp_si (lhs, 1000000000) > 0)
	  {
	    error = "Fibonacci index out of range";
	    mpz_clear (lhs);
	    iGlobalError=-1;
            mpz_set_ui(r,1);
            return;
	  }
	n = mpz_get_ui (lhs);
	mpz_clear (lhs);

#if __GNU_MP_VERSION > 2 || __GNU_MP_VERSION_MINOR >= 1
	mpz_fib_ui (r, n);
#else
	mpz_init_set_ui (t, 1);
	mpz_set_ui (r, 1);

	if (n <= 2)
	  mpz_set_ui (r, 1);
	else
	  {
	    for (i = 3; i <= n; i++)
	      {
		mpz_add (t, t, r);
		mpz_swap (t, r);
	      }
	  }
	mpz_clear (t);
#endif
      }
      return;
    case RANDOM:
      {
	unsigned long int n;
	mpz_init (lhs);
	mpz_eval_expr (lhs, e->operands.ops.lhs);
	if (mpz_sgn (lhs) <= 0 || mpz_cmp_si (lhs, 1000000000) > 0)
	  {
	    error = "random number size out of range";
	    mpz_clear (lhs);
	    iGlobalError=-1;
            mpz_set_ui(r,1);
            return;
	  }
	n = mpz_get_ui (lhs);
	mpz_clear (lhs);
	mpz_urandomb (r, rstate, n);
      }
      return;
    case NEXTPRIME:
      {
	mpz_eval_expr (r, e->operands.ops.lhs);
	mpz_nextprime (r, r);
      }
      return;
    case FLOOR:
      {
      iFloor=1;
      mpz_eval_expr(r, e->operands.ops.lhs);
      if(r < 0)r--;
      iFloor=0;
      return;
      }
    default:
      abort ();
    }
}
/**********************************************************************/
static void mpz_eval_mod_expr (mpz_ptr r, expr_t e, mpz_ptr mod)
{
/* Evaluate the expression E modulo MOD and put the result in R.  */
  mpz_t lhs, rhs;

  switch (e->op)
    {
      case POW:
	mpz_init (lhs); mpz_init (rhs);
	mpz_eval_mod_expr (lhs, e->operands.ops.lhs, mod);
	mpz_eval_expr (rhs, e->operands.ops.rhs);
	mpz_powm (r, lhs, rhs, mod);
	mpz_clear (lhs); mpz_clear (rhs);
	return;
      case PLUS:
	mpz_init (lhs); mpz_init (rhs);
	mpz_eval_mod_expr (lhs, e->operands.ops.lhs, mod);
	mpz_eval_mod_expr (rhs, e->operands.ops.rhs, mod);
	mpz_add (r, lhs, rhs);
	if (mpz_cmp_si (r, 0L) < 0)
	  mpz_add (r, r, mod);
	else if (mpz_cmp (r, mod) >= 0)
	mpz_sub (r, r, mod);
	mpz_clear (lhs); mpz_clear (rhs);
	return;
      case MINUS:
	mpz_init (lhs); mpz_init (rhs);
	mpz_eval_mod_expr (lhs, e->operands.ops.lhs, mod);
	mpz_eval_mod_expr (rhs, e->operands.ops.rhs, mod);
	mpz_sub (r, lhs, rhs);
	if (mpz_cmp_si (r, 0L) < 0)
	  mpz_add (r, r, mod);
	else if (mpz_cmp (r, mod) >= 0)
	  mpz_sub (r, r, mod);
	mpz_clear (lhs); mpz_clear (rhs);
	return;
      case MULT:
	mpz_init (lhs); mpz_init (rhs);
	mpz_eval_mod_expr (lhs, e->operands.ops.lhs, mod);
	mpz_eval_mod_expr (rhs, e->operands.ops.rhs, mod);
	mpz_mul (r, lhs, rhs);
	mpz_mod (r, r, mod);
	mpz_clear (lhs); mpz_clear (rhs);
	return;
      default:
	mpz_init (lhs);
	mpz_eval_expr (lhs, e);
	mpz_mod (r, lhs, mod);
	mpz_clear (lhs);
	return;
    }
}
/**********************************************************************/
/**********************************************************************/
#endif /* __GMP__ */
/**********************************************************************/
