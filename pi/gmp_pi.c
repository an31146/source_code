/*
 * gmp_pi.c - calculate pi with arbitrary precision using binomial coefficients (Machin-like formula)
 * 
 * gcc -Wall -O2 -IC:\Tools\gmp-6.1.2 -std=gnu99 -o gmp_pi.exe gmp_pi.c -fopenmp -lgomp -lgmp
 *
 * pi = -2 + sum(2^(n+1) / nCr(2n, n))
 *
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
 * Machin-like formula:
 * https://en.wikipedia.org/wiki/List_of_formulae_involving_%CF%80#Machin-like_formulae
 *
 *   1021 iterations = accurate to 309 d.p.  
 *   32767 iterations = accurate to 9874 d.p.       5.21 seca
 *   
 *      check this timing:
 *   537 secs to perform 100000 iterations = accurate to 30098 d.p.
 *
 */
void machin_like(mpf_ptr sum, int N)
{
    mpf_t n, t;
    mpf_t pow_2;
    mpf_t ONE;
    mpz_t b;

    mpf_inits(n, t, pow_2, ONE, NULL);
    mpz_init(b);

    for (int k = 1; k < N; k++)
    {
        mpf_set_ui(ONE, 1UL);
        mpf_mul_2exp(pow_2, ONE, k);            // pow_2 = 1 * 2^k

        mpz_bin_uiui(b, 2*k, k);                // b = nCr(2k, k)
        mpf_set_z(n, b);
    
        mpf_div(t, pow_2, n);                   // t = pow_2 / n
        
        mpf_add(sum, sum, t);               // sum += t
    }
    //#pragma omp barrier
    mpf_add(sum, sum, sum);
    mpf_sub_ui(sum, sum, 2L);
    
    mpz_clear(b);
    mpf_clears(n, t, pow_2, ONE, NULL);
}

/*
 * (Don't bother! 19/04/2020)
 * Amazingly slow convergence algorithm.
 *
 * Leibniz series expansion for arctan(1) - 
 * https://en.wikipedia.org/wiki/Leibniz_formula_for_%CF%80
 *
 * Gregory's series:
 * https://en.wikipedia.org/wiki/Gregory%27s_series
 *
 * 100 million iterations to get 14 d.p.
 *
 */
void arctan_of_one(mpf_ptr sum, int N)
{
    mpf_t m, n;
    mpf_inits(m, n, NULL);
    int MUL = 1000000;
    // double sum2 = 1.0d, x, y;

    for (int j=0; j<N*MUL; j+=MUL)
    {
        for (int i=j; i<j+MUL; i++)
        {
            mpf_set_si(m, -1L);                 //
            mpf_div_ui(m, m, i*4+3);            // m = -1/3, -1/7, -1/11, etc
            mpf_add(sum, sum, m);               // sum -= m
            // gmp_printf("%.20Ff\n", m);
            
            // x = 1.0d / (double)(i*4+3);
            // printf("%.20f\n", x);
            // sum2 -= x;
            
            mpf_set_ui(n, 1UL);
            mpf_div_ui(n, n, i*4+1);            // n = 1, 1/5, 1/9, 1/13, etc
            mpf_add(sum, sum, n);               // sum += n
            // gmp_printf("%.20Ff\n", n);
            
            // y = 1.0d / (double)(i*4+5);
            // printf("%.20f\n", y);
            // sum2 += y;
        }
        putchar('.');
    }
    printf("\n");
    // sum2 *= 4.0d;
    // printf("%0.18g\n", sum2);
    mpf_mul_ui(sum, sum, 4L);               // sum = 4*sum
    mpf_clears(m, n, NULL);

    // gmp_printf("%0.Ff\n", sum);

}


/*
 *   (Lucas formula - doesn't work, FIX this!) 
 *
 * 
 *    π                ∞                 1 
 *   ---  =  1 - 16 * SUM  ------------------------------
 *    4               k=0  (4k + 1)^2(4k + 3)^2(4k + 5)^2
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

    mpf_mul_ui(sum, sum, 16L);
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
    
}

/*
 *           (Gosper formula 1974)
 *
 *
 *           ∞     (25k - 3)(k!)(2k)!
 *     π =  SUM   --------------------
 *          k=0       2^(k-1)(3k)!
 *
 *                  - 20000 iterations accurate to 22600 d.p.
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
    mpf_clears(pow_2, t, _25k_sub3, n, NULL);
    mpz_clear(b);
    
}

/*
 *                      (BBP Formula)
 *
            ∞     1    ┌    4        2        1        1   ┐
      π =  SUM  ---- x | ------ - ------ - ------ - ------ |
           k=0  16^k   └ (8k+1)   (8k+4)   (8k+5)   (8k+6) ┘
 *
 *          253 iterations accurate to 308 d.p. ~0.01 secs
 *          507 iterations accurate to 617 d.p. ~0.03 secs
 *         1021 iterations accurate to 1235 d.p. ~0.06 secs
 *         4091 iterations accurate to 4933 d.p. ~0.27 secs
 *         8183 iterations accurate to 9864 d.p. ~0.16 seconds
 *        16377 iterations accurate to 19729 d.p. ~0.64 seconds
 *       262141 iterations accurate to 315655 d.p. ~183.83 seconds
 *       262141 iterations accurate to 315660 d.p. ¬183.78 seconds
 */
void bbp_formula(mpf_ptr sum, int N)
{
    mpf_t n, t;
    mpf_inits(n, t, NULL);
    
    for (int k = 0; k < N; k++)
    {
        mpf_set_ui(t, 4UL);                     //       t = 4
        mpf_div_ui(t, t, 8*k+1);                //         = 4/(8k+1)
        mpf_set(n, t);                          //       n = 4/(8k+1)
        
        mpf_set_ui(t, 2UL);                     //       t = 2
        mpf_div_ui(t, t, 8*k+4);                //         = 2/(8k+4)
        mpf_sub(n, n, t);                       //       n -= 2/(8k+4)
        
        mpf_set_ui(t, 1UL);                     //       t = 1
        mpf_div_ui(t, t, 8*k+5);                //         = 1/(8k+5)
        mpf_sub(n, n, t);                       //       n -= 1/(8k+5)
        
        mpf_set_ui(t, 1UL);                     //       t = 1
        mpf_div_ui(t, t, 8*k+6);                //         = 1/(8k+6)
        mpf_sub(n, n, t);                       //       n -= 1/(8k+6)

        mpf_div_2exp(n, n, 4*k);                //       n /= 2^(4*k)
        mpf_add(sum, sum, n);                   //     sum += n

        //printf("%8d\r", k);                   // progress output adds significant time!
    }

    //gmp_printf("%0.Ff\n", sum);
    mpf_clears(n, t, NULL);
    
}

/*
 *                               (Chudnovsky Formula)
 * https://en.wikipedia.org/wiki/Approximations_of_%CF%80#20th_and_21st_centuries
 *
 *
 *         1             1        ∞  ┌     (6k)!      (13591409 + 545140134k) ┐
 *       -----  =  ------------   Σ  | ------------ * ----------------------- |
 *         π       46880*√10005  k=0 └ (3k)!*(k!)^3        (-640320)^3k       ┘
 *
 */
 void chudnovsky_formula(mpf_ptr Sum, int Limit)
 {
    mpf_t C1, C2, M, N, D;
	mpz_t L, M1, M2, X;
    
    mpf_inits(C1, C2, M, N, D, NULL);
	mpz_inits(L, M1, M2, X, NULL);
 
    for (unsigned int k = 0; k < Limit; k++)
    {
		mpz_fac_ui(M1, 6*k);                    // M1` = (6k)!
		mpz_fac_ui(M2, 3*k);					// M2  = (3k)!
        mpz_div(M1, M1, M2);                    // M1 /= M2

		mpz_fac_ui(M2, k);				        // M3  = k!
        mpz_pow_ui(M2, M2, 3);                  // M2  = M2^3
        mpz_div(M1, M1, M2);                    // M1 /= M2^3
		
        //gmp_printf("M%d = %Zd\n", k, M1);		// 

        mpz_set_ui(L, 545140134);               // L  = 545140134
        mpz_mul_ui(L, L, k);                    // L  = 545140134*k
        mpz_add_ui(L, L, 13591409);             // L += 13591409

        mpz_set_si(X, -640320);                 // X  = -640320
		mpz_pow_ui(X, X, 3*k);                  // X  = ‭(-640320)‬^3k

        //gmp_printf("L%d = %Zd\nX%d = %Zd\n", k, L, k, X);			// 
		
		// divide numerator by denominator
		mpf_set_z(N, L);						//
		mpf_set_z(D, X);						//
		mpf_div(N, N, D);						// 
        
        // multiply by M1
        mpf_set_z(M, M1);
        mpf_mul(N, M, N);

		//gmp_printf("N%d = %0.Fg\n\n", k, N);
		
        mpf_add(Sum, Sum, N);
	}
	mpf_set_ui(C1, 426880);
    mpf_sqrt_ui(C2, 10005);
    
	mpf_mul(C1, C2, C1);
	mpf_div(Sum, C1, Sum);
    //mpf_ui_div(Sum, 1, Sum);
	
	mpf_clears(C1, C2, M, N, D, NULL);
	mpz_clears(L, M1, M2, X, NULL);
}

/*
 *  Gauss-Legendre algorithm:
 *  https://en.wikipedia.org/wiki/Gauss%E2%80%93Legendre_algorithm
 *
 *  a0 = 1
 *  b0 = 1/√2
 *  t0 = 0.25
 *  p0 = 1
 *
 *  a_n+1 = (a_n + b_n) / 2
 *  b_n+1 = √(a_n * b_n)
 *  t_n+1 = t_n - p_n(a_n - a_n+1)^2
 *  p_n+1 = 2 * p_n
 * 
 *           (a_n+1 + b_n+1)^2
 *   π  ≈=   -----------------
 *               4 * t_n+1
 *
 *   gmp_pi.exe 3321921 19          // accurate to 1000019 digits  Elapsed time:   1.05 seconds
 */
void gauss_legendre_algorithm(mpf_ptr sum, int N)
{
    mpf_t a0, b0, t0, p0;
    mpf_t a1, b1, t1;
    int n_digits = 80;
    
    mpf_inits(a0, b0, p0, t0, NULL);
    mpf_inits(a1, b1, t1, NULL);
    
    mpf_set_ui(a0, 1L);
    mpf_set_ui(p0, 1L);
    mpf_sqrt_ui(b0, 2L);
    mpf_ui_div(b0, 1L, b0);
    mpf_init_set_d(t0, 0.25d);
    
    for (int i = 0; i < N; i++)
    {
        mpf_add(a1, a0, b0);                // a1 = (a_n + b_n)
        mpf_div_ui(a1, a1, 2UL);            //    = a1 / 2
        
        mpf_mul(b1, a0, b0);                // b1 = a0 * b0
        mpf_sqrt(b1, b1);                   //    = √(b1)
        
        mpf_sub(t1, a0, a1);                // t1 = a0 - a1
        mpf_pow_ui(t1, t1, 2UL);            //    = (a0 - a1)^2
        mpf_mul(t1, p0, t1);                //    = p0 * (a0 - a1)^2
        mpf_sub(t1, t0, t1);                //    = t0 - p0 * (a0 - a1)^2
        
        mpf_mul_ui(p0, p0, 2UL);            // p0 *= 2
    
        gmp_printf ("a1: %.*Ff with %d digits\n", n_digits, a1, n_digits);
        gmp_printf ("b1: %.*Ff with %d digits\n", n_digits, b1, n_digits);
        gmp_printf ("t1: %.*Ff with %d digits\n", n_digits, t1, n_digits);
        gmp_printf ("p0: %.*Ff with %d digits\n\n", n_digits, p0, n_digits);
        
        mpf_set(a0, a1);
        mpf_set(b0, b1);
        mpf_set(t0, t1);

        // getchar();
    }
    
    mpf_add(sum, a1, b1);               // sum = a1 + b1
    mpf_pow_ui(sum, sum, 2UL);          //     = (a1 + b1)^2
    mpf_div(sum, sum, t1);              //     = sum / t1
    mpf_div_ui(sum, sum, 4UL);          //     = sum / 4
    
    gmp_printf ("pi ~= %.*Ff with %d digits\n", n_digits, sum, n_digits);
    
    mpf_clears(a0, b0, p0, t0, a1, b1, t1, NULL);
}    

int main(int argc, char *argv[])
{
    FILE *fs;
    int prec, iter;
    mpf_t sum;
    clock_t start, stop;
    
    if (argc != 3)
    {
        prec = 1024;
        iter = 11;
    }
    else
    {
        prec = atoi(argv[1]);
        iter = atoi(argv[2]);
    }
    mpf_set_default_prec(prec);
    mpf_init(sum);
    
    
    start = clock();
    // bbp_formula(sum, iter);
	chudnovsky_formula(sum, iter);
    // gosper_formula(sum, iter);
    // machin_like(sum, iter);
    /*
     *  arctan_of_one(sum, iter);        // convergence like the harmonic series!  :-|
     */  
    // gauss_legendre_algorithm(sum, iter);
    stop = clock();
    
    printf("\n\nElapsed time: %6.2f seconds\n",  
            (float)(stop-start)/(float)CLOCKS_PER_SEC ); 
    
    if ((fs = fopen("gmp_pi.txt", "w+t")) != NULL)
    {
		//__attribute__((unused)) 
        char buffer[1048576];
        //mpf_out_str(fs, 10, 0, sum);
        gmp_sprintf(buffer, "%0.Ff\n", sum);
		fprintf(fs, "%s", buffer);
        fclose(fs);
    }
    //gmp_printf("%0.Ff\n", sum);
    mpf_clear(sum);
    
    return EXIT_SUCCESS;
}
