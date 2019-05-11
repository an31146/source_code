#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

using namespace std;

int base_primes[25] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};

/* This function calculates (a^b)%c */
int powmod(int a, int b, int c)
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
    
    int64_t s=p-1;
    while ((s&1)==0)
        s >>=1;

    for (int i=0; i<iters; i++)
    {
        int64_t a = rand()%(p-1)+1;
        int64_t temp = s;
        int64_t mod = powmod(a, temp, p);
        
        while(temp!=p-1 && mod!=1 && mod!=p-1)
        {
            mod = mulmod(mod, mod, p);
            temp <<= 1;
        }
        if(mod!=p-1 && (temp&1)==0)
            return false;
    }
    return true;
}

int main() {
    string input;
    unsigned i, j, n, cases, count;
    char *st;

    getline (cin, input);
    cases = strtol(input.c_str(), NULL, 0);
    
    for (unsigned c=0; c<cases; c++)
    {
        getline (cin, input);
        i = strtoul(input.c_str(), &st, 0);
        j = strtoul(st, NULL, 0);

        count = 0;
        if (i==2)
            ;
            // cout << 2 << endl;
        for (n=i; n<=j; n++)
            if (Miller(n, 3))
            {
                count++;
                cout << setw(12) << n;
            }
                
        cout << endl << count << endl;
    }
    return 0;
}
