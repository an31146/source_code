/*
 gmp_pi.cpp - calculate pi with arbitrary precision using binomial coefficients (Machin-like formula)
 
 g++ -Wall -O2 std=c++11 -o gmp_pi gmp_pi.cpp -fopenmp -lpthread -lgmp
 
 pi = -2 + sum(2^(n+1) / nCr(2n, n))
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gmp.h>
#include <omp.h>

double binomial_coeff(double n, double r)
{
    double i, j, s=1.0;
    
    if (r > n) return 0;
        
    for (i=n; i>(n-r); i-=1.0)
        s *= i;
        
    for (j=1.0; j<=r; j+=1.0)
        s /= j;
        
    return s;
}

void mpf_binomial_coeff(mpf_ptr b, mpf_srcptr n, mpf_srcptr r)
{
    mpf_t i, j, k, s;

    mpf_set_ui(b, 0);
    if (mpf_cmp(n, r) < 0) return;
        
    mpf_init_set(i, n);
    mpf_init_set_ui(s, 1L);

    mpf_init(k);
    mpf_sub(k, n, r);
    
    for (; mpf_cmp(i, k)>0; mpf_sub_ui(i, i, 1L))
        mpf_mul(s, s, i);
        
    mpf_init_set_ui(j, 1L);
    mpf_add_ui(k, r, 1L);

    for (; mpf_cmp(j, k)<0; mpf_add_ui(j, j, 1L))
        mpf_div(s, s, j);

    mpf_ceil(s, s);
    mpf_set(b, s);
}

// Slow binomial coefficient function
void mpz_binomial_coeff_2(mpz_ptr b, int n, int r)
{
    mpz_t s;

    mpz_set_ui(b, 0);
    if (n < r) return;
        
    mpz_init_set_ui(s, 1L);
    
    for (int i=n; i>(n-r); i--)
        mpz_mul_ui(s, s, i);
        
    for (int j=1; j<=r; j++)
        mpz_div_ui(s, s, j);
    
    //mpf_ceil(s, s);
    mpz_set(b, s);
}

/*
 * Machin-like formula
 *   80000 iterations = accurate to 24672 d.p.
 *   537 secs to perform 100000 iterations = accurate to 30098 d.p.
 *
 */
void machin_like(mpf_ptr sum, int i)
{
    mpf_t n, t;
    mpf_t pow_2;
    mpf_t ONE;
    mpz_t b;

    mpf_init(n);
    mpf_init(t);
    mpf_init(pow_2);
    mpf_init_set_ui(ONE, 1UL);
    mpz_init(b);

    omp_set_num_threads(2);
    //#pragma omp parallel for reduction(+ : sum)
    #pragma omp parallel for shared(sum, ONE) private(pow_2, n, b, t)
    for (int k=1; k<i; k++)
    {
        mpf_mul_2exp(pow_2, ONE, k);            // pow_2 = 1 * 2^k

        mpz_bin_uiui(b, 2*k, k);                // b = nCr(2k, k)
        mpf_set_z(n, b);
    
        mpf_div(t, pow_2, n);                   // t = pow_2 / n
        
        //#pragma omp critical
        {
            mpf_add(sum, sum, t);                   // sum += t
        }
        
        //printf("%5.1f%%\r", (float)k/100.0f);
        printf("%8d\r", k);
    }
    //#pragma omp barrier

    mpf_add(sum, sum, sum);
    mpf_sub_ui(sum, sum, 2L);
    
    mpz_clear(b);
    mpf_clears(n, t, pow_2, ONE, NULL);
    
    return;
}

/*
 * amazingly slow convergence algorithm - Leibniz series expansion for arctan(1) - Gregory's formula
 *
 * 100 million iterations to get 14 d.p.
 *
 */
void arctan_of_one(mpf_ptr sum, int N)
{
    mpf_t n, ONE;
    double sum2=1;

    mpf_init_set_ui(ONE, 1);
    mpf_init_set(sum, ONE);
    mpf_init(n);

    for (int i=0; i<N; i++)
    {
        //mpf_add_ui(i, i, 2UL);
        mpf_div_ui(n, ONE, i*4+3);          // n = 1/3
        //gmp_printf("%0.Ff - ", n);
        mpf_sub(sum, sum, n);               // sum -= n
        sum2 -= 1.0 / ((double)i*4.0+3.0);
        
        //mpf_add_ui(i, i, 2UL);
        mpf_div_ui(n, ONE, i*4+5);          // n = 1/5
        //gmp_printf("%0.Ff - ", n);
        mpf_add(sum, sum, n);               // sum += n
        sum2 += 1.0 / ((double)i*4.0+5.0);
    }
    mpf_mul_ui(sum, sum, 4);                // sum = 4*sum
    sum2 *= 4.0;
 
    //sum2 = -2.0 + 2*sum2;    
    printf("%0.20g\n", sum2);
    mpf_clears(n, ONE, NULL);

    //mpf_mul_ui(sum, sum, 4UL);
    gmp_printf("%0.Ff\n", sum);
    
    
    return;
}


/*
 *   (Lucas formula - doesn't work) 
 *   
    pi                ∞                 1 
   ----  =  1 - 16 * SUM  ------------------------------
     4               k=0  (4k + 1)^2(4k + 3)^2(4k + 5)^2
 *      
 */ 
void lucas_formula(mpf_ptr sum, int N)
{
    mpf_t n, t, four_k;
    mpf_t ONE, FOUR;

    mpf_init(n);
    mpf_init(t);
    mpf_init(four_k);
    mpf_init_set_ui(ONE, 1L);
    mpf_init_set_ui(FOUR, 4L);

    mpf_div_ui(sum, ONE, 15L);
    //gmp_printf("%0.Ff\n", sum);
        
    for (int k=0; k<N; k++)
    {
        mpf_set(n, ONE);                // initialize sum=1
        mpf_mul_ui(four_k, FOUR, k);    // four_k = 4*k
        
        mpf_add_ui(t, four_k, 1L);      // t = 4*k + 1
        mpf_div(n, n, t);               // n = 1 / t
        mpf_div(n, n, t);               // n = 1 / t^2
        
        mpf_add_ui(t, four_k, 3L);      // t = 4*k + 3
        mpf_div(n, n, t);               // n = 1 / t
        mpf_div(n, n, t);               // n = 1 / t^2

        mpf_add_ui(t, four_k, 5L);      // t = 4*k + 3
        mpf_div(n, n, t);               // n = 1 / t
        mpf_div(n, n, t);               // n = 1 / t^2

        gmp_printf("%0.Ff\n", n);
        
        mpf_add(sum, sum, n);
    }
    
    gmp_printf("%0.Ff\n", sum);
    
    return;
}

/*
 *          (Gosper formula 1974)
 *
            ∞     (25k - 3)(k!)(2k)!
     pi =  SUM   --------------------
           k=0       2^(k-1)(3k)!
 *
 *      20000 iterations accurate to 22600 d.p.
 *      ~60-75 secs - 26681 iterations accurate to 30137 d.p.
 */
void gosper_formula(mpf_ptr sum, int N)
{
    mpf_t pow_2, _25k_sub3;
    mpf_t n, t;
    mpz_t b;
    
    mpz_init(b);
    mpf_set_si(sum, -6L);                       // sum = -6
    mpf_inits(pow_2, _25k_sub3, n, t, NULL);

    for (int k=1; k<N; k++)
    {
        mpf_set_ui(_25k_sub3, k*25L-3L);        // _25k_sub3 = 25k - 3
        
        //gmp_printf("%0.Ff\n", _25k_sub3);
        
        //mpf_mul_2exp(pow_2, ONE, k-1);          // pow_2 = 1 * 2^(k-1)

        mpz_bin_uiui(b, 3*k, k);                // b = nCr(3k, k)
        mpf_set_z(n, b);                        // n = b
        
        mpf_div_2exp(t, _25k_sub3, k-1);        // t = (25k - 3) / 2^(k-1)
        mpf_div(t, t, n);                       //   = (25k - 3) / 2^(k-1) / nCr(3k, k)
        
        mpf_add(sum, sum, t);                   // sum += t

        printf("%8d\r", k);
    }

    //gmp_printf("%0.Ff\n", sum);
    mpf_clears(pow_2, _25k_sub3, n, t, b, NULL);
    
    return;
}

/*
 *                      (BBP Formula)
 *
            ∞     1    ┌    4        2        1        1   ┐
     pi =  SUM  ---- x | ------ - ------ - ------ - ------ |
           k=0  16^k   └ (8k+1)   (8k+4)   (8k+5)   (8k+6) ┘
 *
 *         4100 iterations accurate to 4939 d.p. ~0.31 secs
 *         7519 iterations accurate to 9061 d.p. ~0.74 secs
 *        25020 iterations accurate to 30137 d.p. ~4.8 secs
 */
void bbp_formula(mpf_ptr sum, int N)
{
    mpf_t pow_16k;
    mpf_t n, t;
    
    mpf_inits(pow_16k, n, t, NULL);
    
    for (int k=0; k<N; k++)
    {
        //mpf_set_ui(pow_16k, 16L);               // pow_16k = 16
        //mpf_pow_ui(pow_16k, pow_16k, k);        //         = 16^k
        
        mpf_set_ui(t, 4L);                      //       t = 4
        mpf_div_ui(t, t, 8*k+1);                //         = 4/(8k+1)
        mpf_set(n, t);                          //       n = 4/(8k+1)
        
        mpf_set_ui(t, 2);                       //       t = 2
        mpf_div_ui(t, t, 8*k+4);                //         = 2/(8k+4)
        mpf_sub(n, n, t);                       //       n -= 2/(8k+4)
        
        mpf_set_ui(t, 1);                       //       t = 1
        mpf_div_ui(t, t, 8*k+5);                //         = 1/(8k+5)
        mpf_sub(n, n, t);                       //       n -= 1/(8k+5)
        
        mpf_set_ui(t, 1);                       //       t = 1
        mpf_div_ui(t, t, 8*k+6);                //         = 1/(8k+6)
        mpf_sub(n, n, t);                       //       n -= 1/(8k+6)

        //mpf_div(n, n, pow_16k);                 //       n /= 16^k
        mpf_div_2exp(n, n, 4*k);                //       n /= 2^(4*k)
        mpf_add(sum, sum, n);                   //     sum += n

        printf("%8d\r", k);
    }

    //gmp_printf("%0.Ff\n", sum);
    mpf_clears(pow_16k, n, t, NULL);
    
    return;
}

int main(int argc, char *argv[])
{
    FILE *fs;
    int prec, iter;
    mpf_t sum;
    clock_t start, stop;
    
    if (argc != 3)
    {
        prec = 16400;
        iter = 17001;
    }
    else
    {
        prec = atoi(argv[1]);
        iter = atoi(argv[2]);
    }
    mpf_set_default_prec(prec);
    mpf_init(sum);
    
    //printf("%d\n", mpf_get_default_prec());
    
    //omp_set_num_threads(2);
    
    /*
    mpf_t pow_2, r, two_r, t;
    
    mpf_init(r);
    mpf_init(t);
    mpf_init(pow_2);
    mpf_init(two_r);

    mpf_sqrt_ui(t, 2L);
    mpf_mul(t, t, t);
    gmp_printf("%0.Ff\n", t);

    mpf_set_ui(r, 50);
    mpf_set_ui(two_r, 100);
    mpf_binomial_coeff(n, two_r, r);
    gmp_printf("%Ff\n", n);
    
    mpz_binomial_coeff_2(b, 100, 50);
    gmp_printf("%Zd\n", b);
    
    x = binomial_coeff(100, 50);
    printf("%f\n", x);    

    return 0;
    */
    
        //sum2 += pow(2, k) / binomial_coeff(2*k, k);

        /*
        mpf_set_d(r, k);                        // r = k
        mpf_set_d(two_r, k);                    // two_r = k
        mpf_mul_ui(two_r, two_r, 2);            // two_r *= 2
        mpf_binomial_coeff(n, two_r, r);        // n = binomial(two_r, r)
        */
        //mpz_binomial_coeff_2(b, 2*k, k);
    
    start = clock();
    //bbp_formula(sum, iter);
    gosper_formula(sum, iter);
    //machin_like(sum, iter);
    //arctan_of_one(sum, iter);
    stop = clock();
    
    printf("\n\nElapsed time: %6.2f seconds\n",  
            (float)(stop-start)/(float)CLOCKS_PER_SEC ); 
    
    if ((fs=fopen("pi.txt", "w+t")) != NULL)
    {
        //mpf_out_str(fs, 10, 0, sum);
        gmp_fprintf(fs, "%0.Ff", sum);
        fclose(fs);
    }
    //gmp_printf("%0.Ff\n", sum);
    mpf_clear(sum);
    
    return 0;
}
