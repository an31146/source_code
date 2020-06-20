#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#define SIZE 20017

uint32_t *primes, k = 1;

void sieve(uint32_t S)
{
	uint32_t i, j;

	primes = (uint32_t *)calloc(S+1, sizeof(uint32_t));
	primes[0] = 2;
	for (i=2; i<sqrt(S); i++)
	{
		for (j=i*i; j<=S; j+=i)
			primes[j] = 1;
	}
	
    for (j=3; j<=S; j++)
    	if (primes[j]==0)
		{
		    primes[k++] = j;
		    //if (primes[j+2]==0)	// twin prime
			//	printf("[%d %d]\n", j, j+2);
        }
}

unsigned is_prime(int32_t N)
{
	unsigned i, pr, sqrt_N = sqrt(N);
	
	for (i=0; pr=primes[i], i<k && pr<sqrt_N; i++)
		if (N%pr == 0)
			return 0;
	return 1;
}

int32_t rand_prime32()
{
	//uint32_t i = (rand() & 0xffff) * (clock() & 0xff) | 1;
	int32_t i = rand() | 1;
	//printf("%u\n", i);
	
	i %= 1600000000L;
	while (!is_prime(i))
	{
		i += 2;
		//i &= 0xffff;
	}
	//printf("[%u]\n",i);
	return i;
}

void twin_primes(int32_t N)
{
	int32_t i;
	double Bruns_const = 0, Twin_prime_const = 1.0d, p;
	
	for (i=1; i<k && primes[i]<N; i++)
	{
		p = primes[i] - 1.0d;
		Twin_prime_const *= 1.0d - 1.0d / (p * p);
		if (primes[i+1] == primes[i]+2)
		{
			//printf("[%d %d]\n", primes[i], primes[i+1]);
			Bruns_const += 1.0d / (double)primes[i] + 1.0d / (double)primes[i+1];
		}
	}
	printf("Twin prime constant: %.18f\n", Twin_prime_const);
	Bruns_const += 4.0 * Twin_prime_const / log(p+1.0d);
	printf("Brun's constant: %.18f\n", Bruns_const);
}

int factor(uint32_t sqrt_n, uint64_t n)
{	
	uint32_t i = 0;
	char s[110], p_str[24];

	//if (n & 1 == 1) return 0;		// reject odd numbers
	sprintf(s, "%10u [%16llu] = ", sqrt_n, n);
	do {
		if (n % primes[i] == 0)
		{	
	    	sprintf(p_str, "%u * ", primes[i]);
	    	//printf("%d ", strlen(p_str));
        	strcat(s, p_str); 
        	n /= primes[i];
		} else {
		    i++;
		    //printf("%6d", i);
		}
	} while ((n > 1) && (i < k));

	if (n > 1)
	{
	    //printf(".");
	    //printf("%d ", strlen(s));
	    sprintf(p_str, "%llu", n);
	    strcat(s, p_str);
	    return 0;
	} else
    	if (n == 1)
    	{
    		//putchar('.');
    		//printf("%d ", strlen(s));
    	    s[strlen(s)-3] = '\0';
    	    printf("%s\n", s);
    	    return 1;
    	}
}

void smooth_num(uint64_t n)
{
	uint64_t i = sqrt(n)+1;
	uint64_t j = sqrt(n)-1;

	for (int s=0; s<200; )
	{
		if (factor(i, i*i-n) == 1) s++;
		i++;
		if (factor(j, n-j*j) == 1) s++;
		j--;
	}
}

int64_t bin_gcd(int64_t u, int64_t v) 
{ 
	int shift; 

	if (u == 0) return v; 
	if (v == 0) return u; 

	for (shift=0; !((u | v) & 1); ++shift) 
	{ 
		u >>= 1; 
		v >>= 1; 
	} 
	while (!(u & 1)) 
		u >>= 1;  
	do { 
		while (!(v & 1)) 
			v >>= 1; 
		if (u > v) { 
			int64_t t = v; v = u; u = t;
		} 
		v -= u; 
	} while (v != 0); 
	return u << shift; 
}

int64_t gcd(int64_t a, int64_t b)
{
	int64_t t;
	
	while (b > 0)
	{
		t = b;
		b = a % b;
		a = t;
	}
	return a;
}

int64_t primorial(unsigned int n)
{
    unsigned i;
    int64_t pr = 1;
    
    for (i=0; i<n; i++)
        pr *= primes[i];
        
    return pr;
}

int64_t g(int64_t x, int64_t n, unsigned a) 
{
	//printf("%lld\n", x*x);
	return (x * x + a) % n; 
}

unsigned pollard_rho(const int64_t n, unsigned a) 
{
	int64_t x_fixed = 2;
	int cycle_size = 2; 
	int64_t x = 2, h = 1; 
	
	while (h == 1) 
	{ 
		int count = 1; 
		while (count <= cycle_size && h == 1) 
		{ 
			x = g(x,n,a); 
			//printf("x = %lld\n", x);
			count++; 
			//printf("%d ", count);
			//if (x > x_fixed)
				h = bin_gcd(fabs(x - x_fixed), n); 
			//else
			//	h = bin_gcd(x_fixed - x, n);
		} 
		if (h != 1)
			if (h != n) 
			{
			//if (gcd(n, h) != n)
				break;
			} 
			else {
				//putchar('.');
				printf("%lld\n", h);
				//cycle_size = h = 1; x = 2; a++;		// reset and try again...
			}
		cycle_size *= 2; 
		printf("%10d\t%lld\n", cycle_size, x);
		x_fixed = x; 
	} 
	//printf("%d\t%lld\n", cycle_size, x);
	printf("\n");
	return h;
}

int main()
{
	int64_t n, x, pr1;
	int t0, t1;
	char strInt[10];
	
	t0 = clock();
	sieve(SIZE);
	t1 = clock();
	printf("sieve time: %d ms\n\n", (t1-t0)/1000);
	
    //for (int i=0; primes[i]<=SIZE && i<k; i++) 
    //    printf("%10d%s", primes[i], (i+1)%18==0?"\n":"");
	t0 = clock();
    twin_primes(SIZE);
	t1 = clock();
	printf("twin_primes time: %d ms\n\n", (t1-t0)/1000);
    //return;

    printf("\nπ(%d) = %u\n", SIZE, k);    
    x = INT32_MAX;
    //x += 0x1fffffff;
    x *= 10000L;
    //x <<= 1;
    //printf("sizeof(uint64_t) = %d\n%lld\n", sizeof(uint64_t), x);
    //printf("sizeof(uint32_t) = %d\n", sizeof(uint32_t));
	//getchar();
	//printf("sizeof(float) = %d\n", sizeof(float));
	//printf("sizeof(double) = %d\n", sizeof(double));
		
    //pr1 = primorial(n);
    //printf("%d# = %llu\n", n, pr1);

	//n = 2650265993LL;
	//n = 4898988649LL;
	//n = 21530071L;
	//n = 20632396777LL;
	//n = 1011127773444757LL;
	//smooth_num(n); return;

	printf("\nEnter a: ");
	scanf("%s", strInt);
	unsigned a = strtoul(strInt, NULL, 0);

	srand(1);
	unsigned avg_time = 0;
	for (int j=0; j<5; j++)
	{
		//printf("rand_prime32() = %u\n", rand_prime32()); }
		int32_t p = rand_prime32();
		sleep(1);
		int32_t q = rand_prime32();
		n = (int64_t)p * q;

		//smooth_num(n); continue;
		t0 = clock();
		int32_t f = pollard_rho(n, a);
		t1 = clock();
		int32_t d = bin_gcd(n, f);

		int32_t g = n / f;
		avg_time += t1 - t0;
		printf("factor(%llu) = %u x %u\n", n, f, g);
		printf("gcd(n, f) = %u\n", d);
		printf("time: %d ms\n\n", (t1-t0)/1000);
	}	
	printf("average: %d ms\n", avg_time/5000);

	free(primes);
	
	return 0;
}