/* 
 *  Calculate pi using arctan series method
 *	
 *	g++ -Wall -O2 -std=c++11 -o arctan_pi.exe arctan_pi.cpp -lgmp
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gmp.h>
#include <iomanip>
#include <iostream>

double fact(int n)
{
    double f = 1.0d;
    
    if (n == 0 || n == 1)
        return 1.0d;
    for (int i = 2; i <= n; i++)
        f *= (double)i;
    return f;
}

double double_fact(int n)
{
    double f;

    if (n % 2 == 0)
    {
        f = 2.0d;
        for (int i = n; i > 2; i -= 2)
            f *= (double)i;
    }
    else
    {
        f = 1.0d;
        for (int i = n; i > 1; i -= 2)
            f *= (double)i;
    }
    return f;
}

void arctan()
{
    double pi, sum = 0.0d;

    for (int j = 0; j < 6; j++)
    {
        for (int i = j*10+1; i <= j*10+10; i++)
        {
            //printf("%d\n", i);
            sum += fact(i) / double_fact(2*i+1);
        }
        
        pi = 2.0d + 2.0d * sum;
        printf("%.18f\n", pi);
    }
    printf("\n");
}

void sin_power_series(double x)
{
    double sum = 0;

    for (int j = 0; j < 10; j++)
    {
        //for (int i = j*10; i <= j*10+9; i++)
        {
            double t = pow(x, 2*j+1) / fact(2*j+1);
            
            if ((j & 1) == 0)
                sum += t;
            else
                sum -= t;
        }
        
        printf("%.18f\n", sum);
    }
    printf("\n");
}

// 0.57721566490153286060
void euler(int n)
{
    double sum = 0.0;

    //for (int j = 0; j < 10; j++)

    for (int k = 1; k < n; k++)
    {
        double recip = 1.0 / (double)k;
        sum += recip - log(1.0 + recip);
    }
    printf("%.18f\n", sum);

    printf("\n");
}

// difference method
void euler2(int n)
{
    double sum = 0.0;
    
    for (int k = 1; k < n; k++)
        sum += 1.0 / (double)k;
    
    printf("%.18f\n\n", sum - log((double)n));
}

void ln2()
{
    double a_third = 1.0 / 3.0;
    double sum = a_third;
    
        for (int n = 1; n < 15; n++)
        {
            a_third /= 9.0;
            sum += a_third / (double)(2*n+1);
        }
        std::cout << std::setprecision(18) << sum * 2.0 << std::endl;
        
    printf("\n");
}

void nlog(double x)
{
    double sum = 1.0, x_m1 = x - 1.0, x_p1 = x + 1.0;
    double x_m1_sqrd = x_m1 * x_m1;
    double x_p1_sqrd = x_p1 * x_p1;
    double term = x_m1_sqrd / x_p1_sqrd;
    
    for (int k = 3; k < 20; k+=2)
    {
        sum  += term / (double)k;
        term *= term;
        printf("%30.18f%30.18f\n", term, sum);
    }
    sum *= 2.0  * x_m1 / x_p1;
    printf("%.18f\n", sum);
}

void arctan_gmp()
{
    int ITERS = 149;
    mpf_t sum, t, u, pi;
    mpz_t fac, fac2;

    mpf_set_default_prec(848);
    mpf_inits(sum, t, u, pi, NULL);
    mpz_inits(fac, fac2, NULL);

    for (int j = 0; j < 6; j++)
    {
        for (int i = j*ITERS+1; i <= j*ITERS+ITERS; i++)
        {
            mpz_fac_ui(fac, i);
            mpz_2fac_ui(fac2, 2*i+1);
            mpf_set_z(t, fac);
            mpf_set_z(u, fac2);
            mpf_div(t, t, u);
            mpf_add(sum, sum, t);
        }
        
        mpf_mul_ui(pi, sum, 2);
        mpf_add_ui(pi, pi, 2);
        gmp_printf("%0.Ff\n\n", pi);
    }
    mpf_clears(sum, t, u, pi, NULL);
    mpz_clears(fac, fac2, NULL);
}

int main(void)
{
    const double PI = 3.1415926535897932385d;
    const int TERMS = 10000;
    
    arctan();
    /*
    sin_power_series(PI/6.0);
    sin_power_series(PI/4.0);
    sin_power_series(PI/3.0);
    sin_power_series(PI/2.0);
    */
    arctan_gmp();
    
    euler(TERMS);             // need ten thousand terms!
    //euler2(TERMS);
    
    ln2();
    //nlog(0.05);
    
    return EXIT_SUCCESS;
}