/*
 *  gcc -Wall -O2 -std=gnu11 -o libgmp_test libgmp_test.c -lm -lgmp
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <gmp.h>
#include <math.h>

int base_primes[25] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};

bool fermat_test(const mpz_t n, int rounds)
{
    mpz_t a, n1, n_sub_one;
    //char __UNUSED_PARAM(str1[1024]);
    int i;
    
    if ( mpz_tdiv_ui(n, 3L) == 0 ||
         mpz_tdiv_ui(n, 5L) == 0 ||
         mpz_tdiv_ui(n, 7L) == 0 )
        return false;
        
    mpz_init(n1);
    mpz_init_set(n_sub_one, n);
    mpz_sub_ui(n_sub_one, n_sub_one, 1);
    for (i=0; i<rounds; i++)
    {
        mpz_init_set_ui(a, base_primes[i]);
        mpz_mod(a, a, n);

        mpz_powm(n1, a, n_sub_one, n);
        //mpz_get_str(str1, 10, a);
        //printf("%s - ", str1);
        
        if (mpz_cmp_ui(n1, 1)!= 0)
            return false;
    }
    return true;
}

bool miller_rabin(const mpz_t n, int rounds)
{
    int r;
    bool is_prime = true;
    mpz_t nm1, x, y, q;
    unsigned long k, i;

    int base_primes[54] =  {  2,   3,   5,   7,  11,  13,  17,  19,
                             23,  29,  31,  37,  41,  43,  47,  53,
                             59,  61,  67,  71,  73,  79,  83,  89,
       	                     97, 101, 103, 107, 109, 113, 127, 131,
                            137, 139, 149, 151, 157, 163, 167, 173,
                            179, 181, 191, 193, 197, 199, 211, 223,
                            227, 229, 233, 239, 241, 251
                           };

    for (int i = 0; i < sizeof(base_primes)/sizeof(int); i++ )
        if (mpz_divisible_ui_p(n, base_primes[i]))
            return false;

    /*
    if ( mpz_divisible_ui_p(n, 3L) ||
         mpz_divisible_ui_p(n, 5L) ||
         mpz_divisible_ui_p(n, 7L) ||
         mpz_divisible_ui_p(n, 11L) ||
         mpz_divisible_ui_p(n, 13L) ||
         mpz_divisible_ui_p(n, 19L) ||
         mpz_divisible_ui_p(n, 23L) ||
         mpz_divisible_ui_p(n, 29L)  )
        return false;
    */
        
    //mpz_init(nm1);
    //mpz_init(x);
    //mpz_init(y);
    //mpz_init(q);
    mpz_inits(nm1, x, y, q, NULL);
    
    mpz_sub_ui(nm1, n, 1L);
    
    /* Find q and k, where q is odd and n = 1 + 2**k * q.  */
    k = mpz_scan1(nm1, 0L);
    mpz_tdiv_q_2exp(q, nm1, k);
    
    for (r=0; r<rounds && is_prime; r++)
    {
        /* 2 to n-2 inclusive, don't want 1, 0 or -1 */
        mpz_set_ui(x, rand());
        mpz_powm(y, x, q, n);
        
        if (mpz_cmp_ui (y, 1L) == 0 || mpz_cmp (y, nm1) == 0)
        {
            is_prime = true;
            continue;                       // next round
        }
        
        for (i=1; i<k; i++)
        {
            mpz_powm_ui (y, y, 2L, n);
            if (mpz_cmp (y, nm1) == 0)
            {
                is_prime = true;            // next round
                break;
            }
            if (mpz_cmp_ui (y, 1L) == 0)
            {
                is_prime = false;           // should exit
                break;
            }
        }
        if (i == k)
	{
            is_prime = false;
            break;
	}
    }   
    mpz_clears(nm1, x, y, q, NULL);

    return is_prime;
}

void powers_of_two()
{
    mpz_t a, b, c;
    long start, stop;
    unsigned int expos[6] = {0x1000, 9689L, 110503L, 216091L, 756839L, 1257787L};

    mpz_inits(a, b, c, NULL);

    for (int i=0; i<6; i++)
    {
        mpz_set_ui(a, 1);
    	mpz_mul_2exp(a, a, expos[i]);           // a = 2^4096
    	mpz_sub_ui(a, a, 1L);                   //   = 2^4096-1
        mpz_sub_ui(b, b, 1L);                   // b = 2^4096-2
    	
    	start = clock();
    	printf("\nmpz_sizeinbase(2^%d-1, 10) = %d\t\t", expos[i], mpz_sizeinbase(a, 10));
    	stop = clock();
    	printf("time: %.0f ms\n", (float)(stop-start)/CLOCKS_PER_SEC*1000.0f);
    	//printf("mpz_sizeinbase(2^1048576, 10) = %lu\n", mpz_sizeinbase(a, 10));
    			
        mpz_set_ui(c, 13L);
        start = clock();
        //mpz_powm(c, c, b, a);                 // c = c^b mod a
        stop = clock();
        
        if (mpz_cmp_ui(c, 1L) == 0)
            printf("\npowm time: %.0f ms\n\n", (float)(stop-start)/CLOCKS_PER_SEC*1000.0f);
    }
    mpz_clears(a, b, c, NULL);
}

void print_time(FILE *F)
{
	time_t t = time(NULL);
        char str_t[30];
        strcpy(str_t, asctime(localtime(&t)));
	str_t[24] = 0;
        //strncpy(str_t, str_t, strlen(str_t)-2);
        printf("[%s]\n", str_t);
        fprintf(F, "[%s]\n", str_t);
}

int main()
{
	mpz_t a, b, c, d;
	float start, stop;
	
	mpz_inits(a, b, c, d, NULL);
	//mpz_set_ui(c, 1000000011L);
	//mpz_set_str(c, "292f4c66283477e3ead1014d327eaf8138922b4ffa337d5f", 16);
	//mpz_set_str(c, "9502f934800000083400000001e0000005cd0000000d4800000080c000000f5b", 16);
	
	/*
	mpz_set_str(c, "9502f934800000083400000001e0000005cd0000000d4800000080c000000f63", 16);
	
	for (long i=0; i<1; i++)
	{
	    start = clock();
	    while (!miller_rabin(c, 3))
	        mpz_add_ui(c, c, 2);
	    stop = clock();
	        
	    mpz_get_str(str_out, 16, c);
	    printf("%s\n%d digits - %d bits - ", str_out, mpz_sizeinbase(c, 10), mpz_sizeinbase(c, 2));
	    printf("time: %.0f ms\n\n", (float)(stop-start)/CLOCKS_PER_SEC*1000.0f);
	    //mpz_mul_ui(c, c, 262144);
	    mpz_mul_2exp(c, c, 256);
	    mpz_add_ui(c, c, 1);
	}
	
	mpz_set_ui(a, 2L);
	mpz_pow_ui(a, a, 63);                   // 2^63
	mpz_sub_ui(a, a, 1);                    // 2^63-1
	mpz_get_str(str_out, 16, a);
	
	printf("2^63-1 = 0x%s\n\n", str_out);
	*/

	mpz_set_str(c, "9836446845059630420462915350160447601213137772030194160939572671041559819755716047653588883329322958477910668460536609833618777725465601211156925711807600063948532374320368592251",      10);

	start = (float)clock() / CLOCKS_PER_SEC;
	printf("clock() = %.0f ms\n\n", start*1000.0f);
	stop = (float)clock() / CLOCKS_PER_SEC;

	FILE *F_TP = fopen("./twin_primes.txt", "a+");
	if(!F_TP) {
		printf("File opening failed.\n");
		return EXIT_FAILURE;
	}

	start = (float)time(NULL);						// number of seconds elapsed since epoch
	while (start+28800.0f > stop)
	{
		int f = 0;
		while (!miller_rabin(c, 23))
		{
			stop = (float)time(NULL);
			mpz_add_ui(c, c, 2);
			switch(f)
			{
				case 0: printf("[|]\r");	// %8.3f\r", fabsf((float)clock() / CLOCKS_PER_SEC));
					break;
				case 1: printf("[/]\r");
					break;
				case 2: printf("[-]\r");
					break;
				case 3: printf("[\\]\r");
					break;
			}
			fflush(stdout);
			f++; f %= 4;
		}

		mpz_add_ui(d, c, 2);
		if (miller_rabin(d, 23))
		{
			print_time(F_TP);
			gmp_printf("\n%Zd\n%Zd\n\n", c, d);
			gmp_fprintf(F_TP, "\n%Zd\n%Zd\n\n", c, d);
			fflush(F_TP);
		}
		mpz_add_ui(c, d, 2);
	}
	fclose(F_TP);
	mpz_clears(a, b, c, d, NULL);

	return EXIT_SUCCESS;
	//powers_of_two();
    
	long n;
	printf("\nEnter n: ");
	int _len __attribute__((unused)) = scanf("%ld", &n);

	mpz_fac_ui(b, n);
	//mpz_sub_ui(b, b, 1);
	
	gmp_printf("\nfact(%ld) = %Zd\nfact(%ld).length = %d\n", n, b, n, mpz_sizeinbase(b, 10));
	
	mpz_set_ui(a, 2);

	for (long i=0; i<n; i++)
	    mpz_mul_ui(a, a, 100);

	mpz_sqrt(b, a);
	
	gmp_printf("\nsqrt(%Zd) =\n", a);
	gmp_printf("%Zd\n", b);
	
	mpz_clears(a, b, c, d, NULL);

	return EXIT_SUCCESS;	
}
