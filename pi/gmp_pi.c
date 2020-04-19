/*
 * gmp_pi.c - calculate pi with arbitrary precision using binomial coefficients (Machin-like formula)
 * 
 * gcc -Wall -O2 -std=c99 -o gmp_pi.exe gmp_pi.c -fopenmp -l gomp -l gmp
 *
 * pi = -2 + sum(2^(n+1) / nCr(2n, n))
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
 *   80000 iterations = accurate to 24672 d.p.
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

    omp_set_num_threads(2);
    //#pragma omp parallel for reduction(+ : sum)
    #pragma omp parallel for shared(sum, ONE) private(pow_2, n, b, t)
    for (int k=1; k<N; k++)
    {
        mpf_set_ui(ONE, 1UL);
        mpf_mul_2exp(pow_2, ONE, k);            // pow_2 = 1 * 2^k

        mpz_bin_uiui(b, 2*k, k);                // b = nCr(2k, k)
        mpf_set_z(n, b);
    
        mpf_div(t, pow_2, n);                   // t = pow_2 / n
        
        //#pragma omp critical
        {
            mpf_add(sum, sum, t);               // sum += t
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


    return;
}


/*
 *   (Lucas formula - doesn't work, FIX this!) 
 *   
    π                ∞                 1 
   ---  =  1 - 16 * SUM  ------------------------------
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
    
    return;
}

/*
 *          (Gosper formula 1974)
 *
            ∞     (25k - 3)(k!)(2k)!
      π =  SUM   --------------------
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
    mpf_clears(pow_2, t, _25k_sub3, n, NULL);
    mpz_clear(b);
    
    return;
}

/*
 *                      (BBP Formula)
 *
            ∞     1    ┌    4        2        1        1   ┐
      π =  SUM  ---- x | ------ - ------ - ------ - ------ |
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

/*
 *                               (Chudnovsky Formula)
 * https://en.wikipedia.org/wiki/Approximations_of_%CF%80#20th_and_21st_centuries
 *
 *
 *         1           ∞    (-1)^k(6k)!(13591409 + 545140134k)
 *       -----  =  12  Σ   ------------------------------------
 *         π          k=0       (3k)!(k!)^3*640320^(3k+3/2)
 *
 */
 void chudnovsky_formula(mpf_ptr sum, int N)
 {
    mpf_t n, d1, s1, ONE;
	mpz_t fac, fac2, d2;
    
    mpf_inits(n, d1, s1, ONE, NULL);
	mpz_inits(fac, fac2, d2, NULL);
 
    for (unsigned int k = 0; k < N; k++)
    {
		mpz_fac_ui(fac, 6 * k);									// fac = (6k)!
		mpz_mul_ui(fac, fac, 13591409UL + 545140134UL * k);		// fac = (6k)! * (13591409 + 545140134*k)
		mpf_set_z(n, fac);										// n   = fac
		
		mpz_fac_ui(fac, 3 * k);									// fac = (3k)!
		mpz_mfac_uiui(fac2, k, 3);								// fac2 = (k!)^3
		mpz_mul(d2, fac, fac2);									// d2  = fac * fac2
		mpf_set_z(d1, d2);										// d1  = d2
		
		// re-use denominator mpz_t d2
		mpz_ui_pow_ui(d2, 640320UL, 6 * k + 3);					// d2 = 640320^(6k+3)
		gmp_printf("d2 = %Zd\n", d2);							// 
		mpf_set_z(s1, d2);										// s1 = d2
		mpf_sqrt(s1, s1);										// s1 = sqrt(s1)
		mpf_mul(d1, d1, s1);									// d1 = (3k)! (k!)^3 * sqrt(640320^(6k+3))
		gmp_printf("d1 = %.20Ff\n", d1);

		// divide numerator by denominator
		mpf_div(n, n, d1);										// n = fac / d1
		
		// take into account alternate signs
		if (k % 2 == 0) 
			mpf_add(sum, sum, n);
		else
			mpf_sub(sum, sum, n);
			
		//gmp_printf("%0.Ff\n", sum);
		//gmp_printf("%0.Fg\n\n", n);
	}
	
	mpf_mul_ui(sum, sum, 12);
	// reciprocal
	mpf_set_ui(ONE, 1);
	mpf_div(sum, ONE, sum);
	
	gmp_printf("%0.Ff\n\n", sum);
	//getchar();

	mpf_clears(n, d1, s1, ONE, NULL);
	mpz_clears(fac, fac2, d2, NULL);
	
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
 */
void gauss_legendre_algorithm(mpf_ptr sum, int N)
{
    mpf_t a0, b0, t0, p0;
    int n_digits = 50;
    
    mpf_inits(a0, b0, p0, NULL);
    mpf_set_ui(a0, 1L);
    mpf_set_ui(p0, 1L);
    mpf_sqrt_ui(b0, 2L);
    mpf_ui_div(b0, 1L, b0);
    mpf_init_set_d(t0, 0.25d);
    
    gmp_printf ("fixed point mpf %.*Ff with %d digits\n", n_digits, a0, n_digits);
    gmp_printf ("fixed point mpf %.*Ff with %d digits\n", n_digits, b0, n_digits);
    gmp_printf ("fixed point mpf %.*Ff with %d digits\n", n_digits, t0, n_digits);
    gmp_printf ("fixed point mpf %.*Ff with %d digits\n", n_digits, p0, n_digits);
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
    
    
    start = clock();
    // bbp_formula(sum, iter);
	// chudnovsky_formula(sum, iter);
    // gosper_formula(sum, iter);
    // machin_like(sum, iter);
    // arctan_of_one(sum, iter);
    gauss_legendre_algorithm(sum, iter);
    stop = clock();
    
    printf("\n\nElapsed time: %6.2f seconds\n",  
            (float)(stop-start)/(float)CLOCKS_PER_SEC ); 
    
    if ((fs = fopen("pi3.txt", "w+t")) != NULL)
    {
		char buffer[1048576];
        //mpf_out_str(fs, 10, 0, sum);
        gmp_sprintf(buffer, "%0.Ff", sum);
		fprintf(fs, "%s", buffer);
        fclose(fs);
    }
    //gmp_printf("%0.Ff\n", sum);
    mpf_clear(sum);
    
    return EXIT_SUCCESS;
}
