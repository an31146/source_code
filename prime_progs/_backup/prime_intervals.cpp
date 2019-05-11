#include <cstdio>
#include <cstring>
#include <cstdint>
#include <math.h>
#include <ctime>

#define LIMIT 49341

unsigned primes[LIMIT], num_primes;

unsigned sieve()
{
    unsigned p = 0;
    
    memset(primes, 0, sizeof(primes));

    primes[p] = 2;
    while (primes[p] <= LIMIT) 
    {
        for (unsigned i = primes[p]*primes[p]; i <= LIMIT; i+=primes[p])
            primes[i] = 1;
        primes[++p] = primes[p-1] + 1;
        for (; primes[p] <= LIMIT && primes[primes[p]] == 1; primes[p]++);      //find next prime (where s[p]==0)
    }
    return p;
}

unsigned isprime(unsigned n) {
    if (n<2)
        return 0;
    else
        for (unsigned i=1; i<num_primes && primes[i]<sqrt(n)+1; i++) {
            if (n==primes[i])
                return 1;
            if (n%primes[i] == 0) {
                return 0;
            }
        }
    return 1;
}

unsigned isprimeX(unsigned n) {
    if (n <= 3) {
        return n > 1;
    } else if (n % 2 == 0 || n % 3 == 0) {
        return 0;
    } else {
        for (unsigned i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) {
                return 0;
            }
        }
        return 1;
    }
}

int main() {
    unsigned cases, i, j, n, count = 0;
    unsigned start, stop;

    num_primes = sieve();
    
    fscanf(stdin, "%d", &cases);
    for (unsigned c=0; c<cases; c++)
    {
        fscanf(stdin, "%d %d", &i, &j);
        
        start = clock();
        if (i<=2)
            printf("2\n");
        
        for (n=i|1; n<=j; n+=2) {
            if (isprime(n))
            {
                count++;
                fprintf(stdout, "%d\n", n);
            }
        }
        stop = clock();
        
    }
    return 0;
}
