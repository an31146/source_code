#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <array>
#include <mpir.h>

using namespace std;

#define LIMIT 49341
array<unsigned int, LIMIT> primes;

array<unsigned int, LIMIT> sieve()
{
    unsigned p = 0;
    
    //memset(primes, 0, sizeof(primes));

    primes[p] = 2;
    while (primes[p] <= LIMIT) 
    {
        for (unsigned i = primes[p]*primes[p]; i <= LIMIT; i+=primes[p])
            primes[i] = 1;
        primes[++p] = primes[p-1] + 1;
        for (; primes[p] <= LIMIT && primes[primes[p]] == 1; primes[p]++);      //find next prime (where s[p]==0)
    }
    return primes;
}

// Use LucasLehmer to determine if 2^n-1 is prime
bool LucasLehmer(unsigned int n)
{
    mpz_t seed, div, i, ONE;
    
    mpz_inits(seed, div, i, ONE, NULL);
    mpz_set_ui(seed, 4);

    mpz_mul_2exp(div, ONE, n);          // div = 2^n
    mpz_sub_ui(div, div, 1L);           // div = 2^n - 1

    mpz_set_ui(i, 3);
    while (mpz_cmp_si(i, n) == -1)
    {
        mpz_mul(seed, seed, seed);
        mpz_sub_ui(seed, seed, 2);
        mpz_mod(seed, seed, div);
        
        mpz_add_ui(i, i, 1);
    }
    
    bool bSeedIsZero = (seed == 0);
    mpz_clears(seed, div, i, ONE);
    
    return bSeedIsZero;
}

void Mersenne(unsigned int n)
{
    mpz_t PowerOf2Sub1, TWO;
    bool isMprime;
    string strPowerOf2Sub1;

    chrono::duration<double> elapsed_seconds;
    //chrono::duration start, end;
    //cout << "f(42) = " << fibonacci(42) << '\n';
    
    mpz_inits(PowerOf2Sub1, TWO, NULL);
    
    for (unsigned int i = 0, x = 1; i < primes.size(); i++)
    {
        auto start = chrono::system_clock::now();
        {
            isMprime = LucasLehmer(primes[i]);
        }
        auto end = chrono::system_clock::now();
        elapsed_seconds = end - start;

        if (isMprime)
        {
            //mpz_mul_2exp (mpz t rop, const mpz t op1, mp bitcnt t op2)
            mpz_mul_2exp(PowerOf2Sub1, TWO, primes[i]);
            mpz_sub_ui(PowerOf2Sub1, PowerOf2Sub1, 1);
            auto strPtr = strPowerOf2Sub1.c_str();
            mpz_get_str(strPtr, 10, PowerOf2Sub1);
            
            if (x < 10)
                cout << "M[" << primes[i] << "] = " << strPowerOf2Sub1;
            else
                cout << "M[" << primes[i] << "] = " << strPowerOf2Sub1.substr(0, 12) << "..." << strPowerOf2Sub1.substr(strPowerOf2Sub1.size() - 12, 12) << endl;
            x++;
            cout << "elapsed time: " << elapsed_seconds << " s" << endl;
        }
        if (n < x)
        {
            break;
        }
    }
}

int main(int argc, char** argv)
{
    //
    
    return EXIT_SUCCESS;
}
