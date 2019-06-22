/*
 * g++ -Wall -O2 -std=c++11 -o mersenne.exe mersenne.cpp -lmpir
 */
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <conio.h>
#include <mpir.h>

using namespace std;

#define L1D_CACHE_SIZE 32768
#define LIMIT 1000000
//array<unsigned long, LIMIT> primes;

// Use LucasLehmer to determine if 2^n-1 is prime
bool LucasLehmer(unsigned n)
{
    mpz_t seed, div, i, ONE;
    
    mpz_inits(seed, div, i, ONE, NULL);
    mpz_set_ui(seed, 4);
    mpz_set_ui(ONE, 1);

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
    mpz_clears(seed, div, i, ONE, NULL);
    
    return bSeedIsZero;
}   // LucasLehmer

void Mersenne(unsigned n, vector<unsigned> &pr)
{
    mpz_t PowerOf2Sub1, TWO;
    bool isMprime;
    string strPowerOf2Sub1;

    chrono::duration<double> elapsed_seconds;
    //chrono::duration start, end;
    //cout << "f(42) = " << fibonacci(42) << '\n';
    
    mpz_inits(PowerOf2Sub1, TWO, NULL);
    
    for (unsigned i = 0, x = 1; i < pr.size(); i++)
    {
        auto start = chrono::system_clock::now();
        {
            isMprime = LucasLehmer(pr[i]);
        }
        auto end = chrono::system_clock::now();
        elapsed_seconds = end - start;

        if (isMprime)
        {
            //mpz_mul_2exp (mpz t rop, const mpz t op1, mp bitcnt t op2)
            mpz_mul_2exp(PowerOf2Sub1, TWO, pr[i]);
            mpz_sub_ui(PowerOf2Sub1, PowerOf2Sub1, 1);

            auto strPtr1 = (char *)strPowerOf2Sub1.c_str();
            mpz_get_str(strPtr1, 10, PowerOf2Sub1);
            
            if (x < 10)
                cout << "M[" << primes[i] << "] = " << strPowerOf2Sub1;
            else
                cout << "M[" << primes[i] << "] = " << strPowerOf2Sub1.substr(0, 12) << "..." << strPowerOf2Sub1.substr(strPowerOf2Sub1.size() - 12, 12) << endl;
            x++;
            cout << "elapsed time: " << elapsed_seconds.count() << " s" << endl;
        }   // if (isMprime)
        if (n < x)
        {
            break;
        }
    }   // for
    mpz_clears(PowerOf2Sub1, TWO, NULL);
}   // Mersenne

/*
unsigned sieve(array<unsigned long, LIMIT> &pr)
{
    unsigned p = 0;
    
    //memset(primes, 0, sizeof(primes));

    pr[p] = 2;
    while (pr[p] <= LIMIT) 
    {
        for (unsigned long i = pr[p]*pr[p]; i <= LIMIT; i += pr[p])
            pr[i] = 1;
        pr[++p] = pr[p-1]+1;
        while (pr[p] <= LIMIT && pr[pr[p]] == 1)
            //find next prime (where s[p]==0)
            pr[p]++;
    }
    return p;
}
*/

/// Generate primes using the segmented sieve of Eratosthenes.
/// This algorithm uses O(n log log n) operations and O(sqrt(n)) space.
/// @param limit         Sieve primes <= limit.
/// @param segment_size  Size of the sieve array in bytes.
///
vector<unsigned> segmented_sieve(int64_t limit, unsigned segment_size = L1D_CACHE_SIZE)
{
    unsigned _sqrt = (unsigned) sqrt((double) limit);
    int64_t count = (limit < 2) ? 0 : 1;
    int64_t s = 2;
    int64_t n = 3;

    // vector used for sieving
    vector<char> segment(segment_size);

    // generate small primes <= sqrt
    vector<char> is_prime(_sqrt + 1, 1);
    for (unsigned i = 2; i * i <= _sqrt; i++)
        if (is_prime[i])
            for (unsigned j = i * i; j <= _sqrt; j += i)
                is_prime[j] = 0;

    vector<unsigned> next;
    vector<unsigned> primes;

    for (int64_t low = 0; low <= limit; low += segment_size)
    {
        fill(segment.begin(), segment.end(), 1);

        // current segment = interval [low, high]
        int64_t high = min(low + segment_size - 1, limit);

        // store small primes needed to cross off multiples
        for (; s * s <= high; s++)
            if (is_prime[s])
            {
                primes.push_back((unsigned) s);
                next.push_back((unsigned)(s * s - low));
            }

        // segmented sieve of Eratosthenes
        for (size_t i = 1; i < primes.size(); i++)
        {
            unsigned j = next[i];
            for (unsigned k = primes[i] * 2; j < segment_size; j += k)
                segment[j] = 0;
            next[i] = j - segment_size;
        }
        for (; n <= high; n += 2)
            count += segment[n - low];
    }
    cout << endl << count << " primes found." << endl << endl;

    return primes;
}

int main(int argc, char** argv)
{
    //
    vector<unsigned> _primes;
    chrono::duration<double> elapsed_seconds;
    auto start = chrono::system_clock::now();
    {
        _primes = segmented_sieve(LIMIT);
    }
    auto end = chrono::system_clock::now();
    elapsed_seconds = end - start;

    cout << "sieve time: " << setprecision(0) << elapsed_seconds.count() * 1000.0f << " ms." << endl << endl;
    
    cout << "Print primes? [Y/n]" << endl;
    char c = _getch();
    //cout << (int)c << endl;
    
    if (c == 'Y' || c == 'y' || c == 0xd)
    {
        for (auto p : _primes)
            if (p > 1)
                cout << setw(8) << p;
            else
                break;
    }
            
    cout << endl << "Vector contains " << n << " primes." << endl << endl;
    
    Mersenne(29, primes);
    
    return EXIT_SUCCESS;
}
