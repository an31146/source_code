// g++ -Wall -O2 -std=c++11 -D__USE_MINGW_ANSI_STDIO -g -o prime_intervals2.exe prime_intervals2.cpp

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cinttypes>
#include <cmath>
#include <ctime>

int base_primes[25] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};

/* This function calculates (ab)%c */
int64_t powmod(uint64_t a, uint64_t b, uint64_t c)
{
    int64_t x=1, y=a; // long long is taken to avoid overflow of intermediate results
    while (b > 0)
    {
        if ((b&1) == 1)
        {
            x=(x*y)%c;
        }
        y = (y*y)%c; // squaring the base
        b >>= 1;
    }
    return x%c;
}

/* this function calculates (a*b)%c taking into account that a*b might overflow */
int64_t mulmod(int64_t a, int64_t b, int64_t c)
{
    int64_t x=0, y=a%c;
    
    while (b > 0) {
        if ((b&1) == 1)
        {
            x = (x+y)%c;
        }
        y = (y<<1)%c;
        b >>= 1;
    }
    return x%c;
}

/* Fermat's test for checking primality, the more iterations the more accurate */
bool Fermat(int64_t p, int iters)
{
    if(p == 1)                       // 1 isn't prime
        return false;

    for (int i=0; i<25; i++)         // trial division for small primes
        if (p == base_primes[i])
            return true;             // found a base_prime
        else
            if (p%base_primes[i] == 0)
                return false;        // composite

    for (int i=0; i<iters; i++)
    {
        // choose a random integer between 1 and p-1 ( inclusive )
        int64_t a = rand()%(p-1)+1; 
        
        // modulo is the function we developed above for modular exponentiation.
        if (powmod(a, p-1, p) != 1)
        { 
            return false; /* p is definitely composite */
        }
    }
    return true; /* p is probably prime */
}

/* Miller-Rabin primality test, iters signifies the accuracy of the test */
bool Miller(int64_t p, int iters)
{
    if (p<2)
        return false;

    for (int i=0; i<25; i++)            // trial division for small primes
        if (p == base_primes[i])
            return true;                // found a base_prime
        else
            if (p%base_primes[i] == 0)  
                return false;           // composite
    
    int64_t p_sub1 = p-1;
    int s = 0;
	while ((p_sub1&1)==0)
	{
		p_sub1 >>=1;
		s++;
	}

	//printf("s: %lld\n", s);
	
	int is_prime = false;
    for (int i=0; i<iters && !is_prime; i++)
    {
        int64_t a = rand();
        int k;
        int64_t mod = powmod(a, p_sub1, p);
        
		if (mod == 1 || mod == p-1)
		{
			is_prime = true;
			continue;
		}
        //while(temp!=p-1 && mod!=1 && mod!=p-1)
        for(k = 0; k < s; k++)
		{
            mod = mulmod(mod, mod, p);
			if (mod == p-1)
			{
				is_prime = true;				
			    break;
			}
			if (mod == 1)
			{
				break;
			}
        }
		//printf("%lld %lld\n", mod, p);
        if (k == s)
		{
            break;
		}
    }
    return is_prime;
}

int main() {
    int32_t cases, c, count = 0;
    uint32_t start, stop;
	int64_t i, j, n;

    fscanf(stdin, "%d", &cases);
    for (c=0; c<cases; c++)
    {
        fscanf(stdin, "%" PRId64 " %" PRId64, &i, &j);
        
		// printf("%" PRId64 "\n", powmod(2, i-1, i));
		// printf("%" PRId64 "\n", mulmod(i-1, i-1, i));
		// printf("Miller: %d\n", Miller(i, 3));
		// break;
		
        start = clock();
        if (i<=2)
            printf("2\n");
        
        for (n=i|1; n<=j; n+=2) {
            if (Miller(n,3) == true)
            {
                count++;
                fprintf(stdout, "%" PRId64 "\n", n);
            }
        }
        stop = clock();
		printf("Elapsed time: %.2fs\n", (float)(stop-start)/CLOCKS_PER_SEC);
        
    }
    return 0;
}
