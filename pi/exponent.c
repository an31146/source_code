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

double factorial(int n)
{
    double f = 1.0d;
    for (int i=1; i<=n; i++)
    {
        f *= (double)i;
    }

    return f;
}

double f(double x, double a)
{
    return x * x - a;
}

double secant_method(double x1, double x2, double a)
{
    return x1 - f(x1, a) * (x1 - x2) / (f(x1, a) - f(x2, a));
}

void mpf_exp(mpf_ptr e, double x, int precision)
{
    mpf_t mpf_tmp, mpf_fact, mpf_x, mpf_near_zero, EPSILON;
    mpz_t mpz_fact;
    __attribute__((unused)) char mpf_str[(size_t)(precision / 3.3219)];
    __attribute__((unused)) mp_exp_t exponent;

    mpf_inits(mpf_fact, mpf_tmp, mpf_x, mpf_near_zero, EPSILON, NULL);
    mpz_init(mpz_fact);
    mpf_set_ui(e, 1);
    
    mpf_set_ui(EPSILON, 1);
    mpf_div_2exp(EPSILON, EPSILON, precision);
    //mpf_get_str(mpf_str, &exponent, 10, 0, EPSILON);
    gmp_printf("EPSILON = %.80Fg\n\n", EPSILON);

    mpf_set_d(mpf_x, x);
    mpf_set(mpf_near_zero, EPSILON);
    
    int n = 1;
    for (; mpf_cmp(mpf_near_zero, EPSILON) >= 0; n++)
    {
        mpf_pow_ui(mpf_tmp, mpf_x, n);
        mpz_fac_ui(mpz_fact, n);

        mpf_set_z(mpf_fact, mpz_fact);
        mpf_div(mpf_tmp, mpf_tmp, mpf_fact);
        
        mpf_ui_div(mpf_near_zero, 1, mpf_fact);
        
        //gmp_printf("%.Ff\n%.23Ff\n", mpf_near_zero, e);
        //mpf_reldiff(mpf_near_zero, EPSILON, mpf_near_zero);
        
        mpf_add(e, e, mpf_tmp);
    }
    printf("Iterations: %d\n", n);
    mpz_clear(mpz_fact);
    mpf_clears(mpf_fact, mpf_tmp, mpf_x, mpf_near_zero, EPSILON, NULL);
}

double e(double x)
{
    double e = 1.0, _e = 0;

    for (int n = 1; (e - _e) >= 0; n++)
    {
        _e = e;
        e += pow(x, n) / factorial(n);
    }
    return e;
}

double do_secant(double x)
{
    double g1 = x / 2.0, g0 = x / 4.0;
    for (int i = 0; i < 10; i++)
    {
        double g = secant_method(g1, g0, x);
        printf("%.20f\n", g);
        if (g == g1)
            break;
        g0 = g1; g1 = g;
    }
    return g1;
}

int main(int argc, char **argv)
{
    double x = 1.0d;
    mpf_t exp;
    int PREC = mpf_get_default_prec();

    if (argc > 1)
    {
        x = strtod(argv[1], NULL);
        //printf("x = %f\n", x);
        if (errno == ERANGE)
        {
            printf("Error parsing decimal - defaulting to 1.0d\n");
            x = 1.0d;
        }
        
        // Number of decimal places for N bits of precision = ceil(N / 3.3219281)
        // e.g. 1024-bits = 308 digits
        if (argc == 3)
        {
            PREC = strtol(argv[2], NULL, 0);
            mpf_set_default_prec(PREC);
        }

    }
    
    printf("GMP version: %s\n", gmp_version);
    mpf_init(exp);
    mpf_exp(exp, x, PREC);

    gmp_printf("%0.Ff\n", exp);
    //printf("%.20f\n", e(x));

    return 0;
}
