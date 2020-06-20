/*
 * gcc -Wall -O2 -std=c99 -o exponent.exe exponent.c -lgmp -lm
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

void mpf_exp(mpf_ptr e, double x, int N)
{
    mpf_t mpf_tmp, mpf_fact, mpf_x;
    mpz_t mpz_fact;

    mpf_inits(mpf_fact, mpf_tmp, mpf_x, NULL);
    mpz_init(mpz_fact);
    mpf_set_ui(e, 1);

    for (int n = 1; n <= N; n++)
    {
        mpf_set_d(mpf_x, x);
        mpf_pow_ui(mpf_tmp, mpf_x, n);
        mpz_fac_ui(mpz_fact, n);
        //mpf_set_ui(mpf_r, 1);
        mpf_set_z(mpf_fact, mpz_fact);
        mpf_ui_div(mpf_tmp, 1, mpf_fact);
        mpf_add(e, e, mpf_tmp);
    }
    mpz_clear(mpz_fact);
    mpf_clears(mpf_fact, mpf_tmp, mpf_x, NULL);
}

double e(double x)
{
    double e = 1.0d;

    for (int n = 1; n <= TERMS; n++)
    {
        e += pow(x, n) / factorial(n);
    }
    return e;
}

int main(int argc, char **argv)
{
    double x = 1.0d;
    mpf_t exp;
    int N = TERMS;

    if (argc > 1)
    {
        x = strtod(argv[1], NULL);
        if (errno == ERANGE)
        {
            printf("Error parsing decimal - defaulting to 1.0d\n");
            x = 1.0d;
        }
        
        // Number of decimal places for N bits of precision = ceil(N / 3.3219281)
        // e.g. 1024-bits = 308 digits
        if (argc >= 3)
            mpf_set_default_prec(strtol(argv[2], NULL, 0));
        
        if (argc == 4)
            N = strtol(argv[3], NULL, 0);

    }
    printf("gmp_version: %s\n", gmp_version);
    mpf_init(exp);
    mpf_exp(exp, x, N);

    gmp_printf("%0.Ff\n", exp);
    //printf("%.20f\n", e(x));

    return 0;
}
