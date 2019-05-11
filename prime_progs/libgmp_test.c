/*
<<<<<<< HEAD
 *  gcc -Wall -O2 -std=c99 -o libgmp_test libgmp_test.c -lm -lgmp
=======
 *  gcc -Wall -O2 -std=gnu99 -o libgmp_test.exe libgmp_test.c -lm -lmpir
>>>>>>> 5038afffe6ec5c04fb81aff43def55bfdbfbecd5
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

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
        
    mpz_inits(nm1, q, x, y, NULL);
    
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
    mpz_clears(nm1, q, x, y, NULL);
    return is_prime;
}

void powers_of_two()
{
    mpz_t a, b, c;
    unsigned long start, stop;
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
        printf("time: %ld ms\n", (stop-start)/CLOCKS_PER_SEC*1000);
        //printf("mpz_sizeinbase(2^1048576, 10) = %lu\n", mpz_sizeinbase(a, 10));
            
        mpz_set_ui(c, 13L);
        start = clock();
        //mpz_powm(c, c, b, a);                 // c = c^b mod a
        stop = clock();
    
        if (mpz_cmp_ui(c, 1L) == 0)
            printf("\npowm time: %ld ms\n\n", (stop-start)/1000);
    }
    mpz_clears(a, b, c, NULL);
}

void print_time(FILE *F)
{
    time_t t = time(NULL);
    char str_t[30];

    strncpy(str_t, asctime(localtime(&t)), 24);
    str_t[24] = '\00';
    printf("[%s]\n", str_t);
    fprintf(F, "[%s]\n", str_t);
}

int main()
{
<<<<<<< .merge_file_vmOKPW
    mpz_t a, b, c, d;
    float start, stop;
    
    mpz_inits(a, b, c, d, NULL);
    //mpz_set_ui(a, 1);
    //mpz_set_ui(b, 2);
    //mpz_set_ui(c, 1000000011L);
    //mpz_set_str(c, "1237940075278319013244460992103628753054475013", 10);
    //mpz_set_str(c, "67399868083273799563567977110671938751166936794494779375573996285276182286171", 10);
    
    mpz_set_str(c, "9502f934800000083400000001e0000005cd0000000d4800000080c000000f63", 16);
    
    /*
    for (long i=0; i<1; i++)
    {
        start = clock();
        while (!miller_rabin(c, 3))
            mpz_add_ui(c, c, 2);
        stop = clock();
            
        mpz_get_str(str_out, 16, c);
        printf("%s\n%d digits - %d bits - ", str_out, mpz_sizeinbase(c, 10), mpz_sizeinbase(c, 2));
        printf("time: %ld ms\n\n", (stop-start)/1000);
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

    mpz_set_str(c, "67399868083273799563567977110671938751166936794494779375573996285276248860913", 10);

    printf("CLOCKS_PER_SEC = %ld\n", CLOCKS_PER_SEC);
=======
	mpz_t a, b, c, d;
	//char str_out[262144];
	//char str_out2[262144];
	float start, stop;
	
	mpz_inits(a, b, c, d, NULL);
	//mpz_set_ui(c, 1000000011L);
<<<<<<< HEAD
	//mpz_set_str(c, 111, 10);
	//mpz_set_str(c, 111, 10);
=======
	//mpz_set_str(c, "1237940075278319013244460992103628753054475013", 10);
	//mpz_set_str(c, "67399868083273799563567977110671938751166936794494779375573996285276182286171", 10);
	
	//mpz_set_str(c, "9502f934800000083400000001e0000005cd0000000d4800000080c000000f63", 16);
	//mpz_set_str(c, "308189028181009f2665b909cc0b0d64ac7fe6840ea36e02b0d1b66eb78f9c78696db12031928ba4e5e22704eb79ffc953db182c435044f21c9a6a9ac2d5dceba0a3a638a3fbe10399cf9814f335c9fe6f05e128e110a8486bdeb9ccd186997e765d92f3e92cec3c2b2b3cedb8c2b3fb83af97641f11e024b67973863c67f8eed5e16b4138638b0203010001", 16);
	//gmp_printf ("mpz %Zd\n", c);
>>>>>>> 5038afffe6ec5c04fb81aff43def55bfdbfbecd5
	
	/*
	mpz_set_str(c, "9502f934800000083400000001e0000005cd0000000d4800000080c000000f63", 16);
	
	for (long i=0; i<1; i++)
	{
	    start = clock();
	    while (!miller_rabin(c, 3))
	        mpz_add_ui(c, c, 2);
	    stop = clock();
	        
	    //mpz_get_str(str_out, 16, c);
	    gmp_printf("%Zx\n%d digits - %d bits - ", c, mpz_sizeinbase(c, 10), mpz_sizeinbase(c, 2));
	    printf("time: %ld ms\n\n", (stop-start)/CLOCKS_PER_SEC);
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

<<<<<<< HEAD
	mpz_set_str(c, "70075197985927611076971005579058636974570607864407988181158931334268315880905650934137357364743068772075658810649075047312082034642592490448418403770075197985927611076971005579058636974570607878656722431",   10);

	start = (float)clock() / CLOCKS_PER_SEC;
	printf("clock() = %.0f ms\n\n", start*1000.f);
	stop = (float)clock() / CLOCKS_PER_SEC;
=======
	mpz_set_str(c, "2f478f5daf038b6471e79c49d193786042392e30013036855b0c7c08426cbbcda0b38276675cff7e1a774dc3a4803cf0726cc641b1feef7b96835a99", 16);
>>>>>>> 5038afffe6ec5c04fb81aff43def55bfdbfbecd5
>>>>>>> .merge_file_2QevcV

    start = clock() / (float)CLOCKS_PER_SEC;
    printf("clock() = %0f ms\n\n", start*1000.0f);
    stop = clock() / (float)CLOCKS_PER_SEC;

    FILE *F_TP = fopen("./twin_primes.txt", "a+");
    if(!F_TP) {
        printf("File opening failed.\n");
        return EXIT_FAILURE;
    }

    while (start+3600.0f > stop)
    {
        int f = 0;
        while (!miller_rabin(c, 23))
        {
            start = clock() / 1000.f;
            mpz_add_ui(c, c, 2);

            switch(f)
            {   
                case 0: printf("[|]\r");
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
        } // while

        mpz_add_ui(d, c, 2);
        if (miller_rabin(d, 23))
        {
            print_time(F_TP);

            gmp_printf("\n%Zd\n%Zd\n\n", c, d);
            gmp_fprintf(F_TP, "\n%Zd\n%Zd\n\n", c, d);
            fflush(F_TP);
        }
        mpz_add_ui(c, d, 2);
    }  // while 
    fclose(F_TP);

    return EXIT_SUCCESS;
    //powers_of_two();
    
    long n;
    printf("\nEnter n: ");
    scanf("%ld", &n);

    mpz_fac_ui(b, n);
    //mpz_sub_ui(b, b, 1);
    //mpz_get_str(str_out, 10, b);
    
    gmp_printf("\nfact(%ld) = %Zd\nfact(%ld).length = %d\n", n, b, n, mpz_sizeinbase(b, 10));
    
    mpz_set_ui(a, 2);

    for (long i=0; i<n; i++)
        mpz_mul_ui(a, a, 100);

    mpz_sqrt(b, a);
    
    //mpz_get_str(str_out, 10, a);
    gmp_printf("\nsqrt(%Zd) =\n", a);
    //mpz_get_str(str_out, 10, b);
    gmp_printf("%Zd\n", b);
    
    return EXIT_SUCCESS;
}

