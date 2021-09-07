// gcc -Wall -O2 -std=c99 -D__USE_MINGW_ANSI_STDIO -o divsum.exe divsum.c
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

unsigned primes[78498];

const int LIMIT = 1000000;

int main() {
	uint64_t sum_divisors, n;
    unsigned cases, i, j, num_primes, pr;
    unsigned start, stop, count;
    unsigned char is_prime[LIMIT];
    unsigned exponents[78498];
    FILE *in_file1;

    start = clock();
    memset(is_prime, 1, sizeof(is_prime));
    num_primes = 0;
    for (i = 2; i * i <= LIMIT; i++)
    {
        if (is_prime[i])
        {
            //printf("%d\n", i);
            primes[num_primes++] = i;

            for (j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }
    for (; i < LIMIT; i++)
        if (is_prime[i])
        {
            //printf("%d\n", i);
            primes[num_primes++] = i;
        }
    stop = clock();
    //printf("%d\t%d ms\n", num_primes, (stop-start));
    //return 0;

    in_file1 = fopen(".\\prime_progs\\prime_intervals2.out", "r+");
    fscanf(in_file1, "%d", &cases);
    
    count = 0;
    start = clock();
    
    while (cases--)
    {
        fscanf(in_file1, "%lld", &n);
        
        if (n == 1)
        {
            printf("0\n");
            continue;
        }

        sum_divisors = 0;

        /*for (i=1; i<n; i+=1)
            if (n%i == 0)
                sum_divisors += i;
        */
        
        uint64_t tmp = n;      // save n for later
        memset(exponents, 0, sizeof(exponents));
        for (i=0; pr=primes[i], i<num_primes && n>1; i++)
            while (n % pr == 0 && n > 1)
            {
                //printf("n = %d\tpr = %d\n", n, pr);
                exponents[i]++;
                //printf("n/pr = %d\n", n/pr);
                n /= pr;
            }
        n = tmp;
        //printf("\n\n");        

        sum_divisors = 1;
        for (j=0; j<i; j++)
        {
            if (exponents[j] > 0)
			{
				printf("primes[%d] = %d\texponents[%d] = %d\n", j, primes[j], j, exponents[j]);
                sum_divisors *= (uint64_t)(pow(primes[j], exponents[j]+1) - 1) / (primes[j]-1);
			}
        }

        // if (sum_divisors == 1)
            // printf("1\n");
        // else
            printf("%lld\n", sum_divisors);
        count++;
    }
    stop = clock();
    printf("count = %d\ntime: %d ms\n", count, (stop-start));
        
    fclose(in_file1);
    return 0;
}
