/*
 * gcc -Wall -O2 -std=c99 -o exponent exponent.c -lgmp -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <gmp.h>

// 78 terms accurate to 116 d.p.
// 2.71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642742746639193200212
#define TERMS 78

double factorial(int n)
{
    double f = 1.0d;
    for (int i=1; i<=n; i++)
    {
        f *= (double)i;
    }

    return f;
}

void mpf_exp(mpf_ptr e, double x)
{
    mpf_t mpf_tmp, mpf_fact, mpf_r, mpf_x1;
    mpz_t mpz_fact;

    mpf_inits(mpf_fact, mpf_tmp, mpf_r, mpf_x1, NULL);
    mpz_init(mpz_fact);
    mpf_set_ui(e, 1);

    for (unsigned int n=1; n<=TERMS; n++)
    {
        mpf_set_d(mpf_x1, x);
        mpf_pow_ui(mpf_tmp, mpf_x1, n);
        mpz_fac_ui(mpz_fact, n);
        mpf_set_ui(mpf_r, 1);
        mpf_set_z(mpf_fact, mpz_fact);
        mpf_div(mpf_tmp, mpf_tmp, mpf_fact);
        mpf_add(e, e, mpf_tmp); 
    }
}

double e(double x)
{
    double e = 1.0d;

    for (int n=1; n<=TERMS; n++)
    {
        e += pow(x, n) / factorial(n);
    }
    return e;
}

int main(int argc, char **argv)
{
    double x = 1.0d;
    mpf_t exp;
    mpf_set_default_prec(355);

    if (argc == 2)
    {
        x = strtod(argv[1], NULL);
        if (errno == ERANGE)
        {
            printf("Error parsing decimal - defaulting to 1.0d\n");
            x = 1.0d;
        }
    }
    mpf_init(exp);
    mpf_exp(exp, x);

    gmp_printf("%0.Ff\n", exp);
    //printf("%.20f\n", e(x));

    return 0;
}
