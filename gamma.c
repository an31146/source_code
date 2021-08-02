/*
 * gcc -Wall -O2 -std=c99 -IC:\Tools\gsl-2.6 -o gamma.exe gamma.c C:\Tools\gsl-2.6\err\error.c C:\Tools\gsl-2.6\specfunc\gamma.c C:\Tools\gsl-2.6\specfunc\exp.c C:\Tools\gsl-2.6\specfunc\log.c C:\Tools\gsl-2.6\specfunc\psi.c C:\Tools\gsl-2.6\specfunc\result.c C:\Tools\gsl-2.6\err\stream.c C:\Tools\gsl-2.6\complex\math.c C:\Tools\gsl-2.6\specfunc\trig.c C:\Tools\gsl-2.6\specfunc\zeta.c C:\Tools\gsl-2.6\sys\infnan.c C:\Tools\gsl-2.6\specfunc\elementary.c C:\Tools\gsl-2.6\sys\fdiv.c C:\Tools\gsl-2.6\sys\coerce.c
 *
 * gcc -Wall -O2 -o gamma gamma.c -lgsl -lm
 */
#define HAVE_INLINE 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_sf_zeta.h>
#include <gsl/gsl_inline.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

/* very simple approximation */
double st_gamma(double x)
{
    return sqrt(2.0*M_PI/x)*pow(x/M_E, x);
}
 
#define A 12
double sp_gamma(double z)
{
    const int a = A;
    static double c_space[A];
    static double *c = NULL;
    double accm;

    if ( c == NULL ) {
        double k1_factrl = 1.0; /* (k - 1)!*(-1)^k with 0!==1*/
        c = c_space;
        c[0] = sqrt(2.0*M_PI);
        for(int k=1; k < a; k++) {
            c[k] = exp(a-k) * pow(a-k, k-0.5) / k1_factrl;
            k1_factrl *= -k;
        }
    }
    accm = c[0];
    for(int k=1; k < a; k++) {
        accm += c[k] / ( z + k );
    }
    accm *= exp(-(z+a)) * pow(z+a, z+0.5); /* Gamma(z+1) */
    return accm/z;
}
 
int main()
{
    double x;


    printf("%20s%20s%20s%20s%20s\n", "x", "Stirling", "Spouge", "GSL", "libm");
    for(x=1.0; x <= 10.0; x+=1.0) {
        printf("%20.12lf%20.12lf%20.12lf%20.12lf%20.12lf\n", x/3.0, st_gamma(x/3.0), sp_gamma(x/3.0), 
                gsl_sf_gamma(x/3.0), tgamma(x/3.0));
        printf("%20.12lf%20.12lf%20.12lf%20.12lf%20.12lf\n", x/2.0, st_gamma(x/2.0), sp_gamma(x/2.0), 
                gsl_sf_gamma(x/2.0), tgamma(x/2.0));
    }
    
    gsl_complex z10, z50, z100, z150;
    gsl_complex inv_z10, inv_z50, inv_z100, inv_z150;
    
    z10 = gsl_complex_rect(0.5, 49.773832477672302182d);
    z50 = gsl_complex_rect(0.5, 143.111845807620632739d);
    z100 = gsl_complex_rect(0.5, 236.524229665816205802d);
    z150 = gsl_complex_rect(0.5, 318.853104256316597907d);
    
    inv_z10 = gsl_complex_inverse(z10);
    inv_z50 = gsl_complex_inverse(z50);
    inv_z100 = gsl_complex_inverse(z100);
    inv_z150 = gsl_complex_inverse(z150);
    
    printf("%20.12lf\n%20.12lf\n%20.12lf\n%20.12lf\n", gsl_complex_arg(z10), gsl_complex_arg(z50), gsl_complex_arg(z100), gsl_complex_arg(z150));
    printf("%20.12lf + %20.12lfj\n%20.12lf + %20.12lfj\n%20.12lf + %20.12lfj\n%20.12lf + %20.12lfj\n", GSL_REAL(inv_z10), GSL_IMAG(inv_z10), GSL_REAL(inv_z50), GSL_IMAG(inv_z50), 
                                                                                                    GSL_REAL(inv_z100), GSL_IMAG(inv_z100), GSL_REAL(inv_z150), GSL_IMAG(inv_z150));
    
    return 0;
}
