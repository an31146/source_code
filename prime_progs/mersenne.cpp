/*
 * g++ -Wall -O2 -std=c++11 -o mersenne.exe mersenne.cpp -lmpir -lpthread
 */
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <mpir.h>
#include <sys/sysinfo.h>

using namespace std;

#define MILLIS(x) setprecision(0) << fixed << 1000.0f * x
#define SECONDS(x) setprecision(1) << fixed << x

#define L1D_CACHE_SIZE 32768
#define LIMIT 1000000

static unsigned N_CORES;

mutex g_i_mutex;

// Use LucasLehmer to determine if 2^n-1 is prime
bool LucasLehmer(unsigned n)
{
    mpz_t seed, div, ONE;
    
    mpz_inits(seed, div, ONE, NULL);
    mpz_set_ui(seed, 4);
    mpz_set_ui(ONE, 1);

    mpz_mul_2exp(div, ONE, n);          // div = 2^n
    mpz_sub_ui(div, div, 1L);           // div = 2^n - 1
    
    //mpz_set_ui(i, 3);
    //cout << mpz_cmp_si(i, n);
    //while (mpz_cmp_si(i, n) < 0)
    for (unsigned i = 3; i <= n; i++)
    {
        mpz_mul(seed, seed, seed);
        mpz_sub_ui(seed, seed, 2);
        mpz_mod(seed, seed, div);
        
        //mpz_add_ui(i, i, 1);
    }
    
    bool bSeedIsZero = (mpz_cmp_ui(seed, 0) == 0);
    //if (bSeedIsZero)
    //    gmp_printf("%Zd\n", div);
    mpz_clears(seed, div, ONE, NULL);
    
    return bSeedIsZero;
}   // LucasLehmer

void Mersenne(unsigned n, const vector<unsigned> &pr, unsigned primeOffset)
{
    mpz_t PowerOf2_Minus1, UNITY;
    bool isMprime;
    char *strPtr1 = (char *) malloc(16777216);
    string strPowerOf2_Minus1;

    mpz_inits(PowerOf2_Minus1, UNITY, NULL);
    mpz_set_ui(UNITY, 1);
    
    auto start = chrono::system_clock::now();
    for (unsigned i = primeOffset; i < pr.size(); i += N_CORES)
    {
        cout << "testing M[" << pr[i] << "]\r";
        cout.flush();
        isMprime = LucasLehmer(pr[i]);


        if (isMprime)
        {
            auto end = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;

            //mpz_mul_2exp (mpz t rop, const mpz t op1, mp bitcnt t op2)
            mpz_mul_2exp(PowerOf2_Minus1, UNITY, pr[i]);
            mpz_sub_ui(PowerOf2_Minus1, PowerOf2_Minus1, 1);

            mpz_get_str(strPtr1, 10, PowerOf2_Minus1);
            strPowerOf2_Minus1.assign(strPtr1);
            
            //cout << strPowerOf2_Minus1.size() << endl;
            lock_guard<mutex> lock(g_i_mutex);
            cout << "core #" << hex << this_thread::get_id() << ' ' << endl << "--------" << endl;

	    cout << dec;
            if (strPowerOf2_Minus1.size() < 24)
                cout << "M[" << pr[i] << "] = " << strPowerOf2_Minus1 << endl;
            else
                cout << "M[" << pr[i] << "] = " << strPowerOf2_Minus1.substr(0, 12) << "..." 
                     << strPowerOf2_Minus1.substr(strPowerOf2_Minus1.size() - 12, 12) << "\t (" 
                     << strPowerOf2_Minus1.size() << " digits)" << endl;

	    if (elapsed_seconds.count() >= 1.0f)
                cout << "elapsed time: " << SECONDS(elapsed_seconds.count()) << " s" << endl << endl;
	    else
                cout << "elapsed time: " << MILLIS(elapsed_seconds.count()) << " ms" << endl << endl;
        
            g_i_mutex.unlock();            
	    start = chrono::system_clock::now();
        }   // if (isMprime)
    }   // for
    mpz_clears(PowerOf2_Minus1, UNITY, NULL);
    free(strPtr1);
}   // Mersenne

void MersenneDeux(unsigned n, const vector<unsigned> &pr)
{
    mpz_t PowerOf2_Minus1, UNITY;
    bool isMprime;
    char *strPtr1 = (char *) malloc(16777216);
    string strPowerOf2_Minus1;
    
    mpz_inits(PowerOf2_Minus1, UNITY, NULL);
    mpz_set_ui(UNITY, 1);
    
    auto start = chrono::system_clock::now();
    for (unsigned i = 0; i < pr.size(); i++)
    {
	cout << "testing M[" << pr[i] << "]\r";
	cout.flush();
        {
            mpz_mul_2exp(PowerOf2_Minus1, UNITY, pr[i]);
            mpz_sub_ui(PowerOf2_Minus1, PowerOf2_Minus1, 1);
            
            //int mpz_probab_prime_p (const mpz t n, int reps)
            isMprime = mpz_probab_prime_p(PowerOf2_Minus1, 7);
        }

        if (isMprime)        
        {
            auto end = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;

            mpz_get_str(strPtr1, 10, PowerOf2_Minus1);
            strPowerOf2_Minus1.assign(strPtr1);
            
	    cout << endl;
            if (strPowerOf2_Minus1.size() < 24)
                cout << "M[" << pr[i] << "] = " << strPowerOf2_Minus1 << endl;
            else
                cout << "M[" << pr[i] << "] = " << strPowerOf2_Minus1.substr(0, 12) << "..." 
                     << strPowerOf2_Minus1.substr(strPowerOf2_Minus1.size() - 12, 12) << "\t (" 
                     << strPowerOf2_Minus1.size() << " digits)" << endl;

	    if (elapsed_seconds.count() >= 1.0f)
                cout << "elapsed time: " << SECONDS(elapsed_seconds.count()) << " s" << endl << endl;
	    else
                cout << "elapsed time: " << MILLIS(elapsed_seconds.count()) << " ms" << endl << endl;

            start = chrono::system_clock::now();
        }
    }   // for
    mpz_clears(PowerOf2_Minus1, UNITY, NULL);
    free(strPtr1);
}   // MersenneDeux

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
vector<unsigned> segmented_sieve(int64_t limit, const unsigned segment_size = L1D_CACHE_SIZE)
{
    unsigned _sqrt = (unsigned) sqrt((double) limit);
    int64_t count = (limit < 2) ? 0 : 1;
    int64_t s = 2;
    int64_t n = 3;

    // vector used for sieving
    vector<bool> segment(segment_size);

    // generate small primes <= sqrt
    vector<bool> is_prime(_sqrt + 1, true);
    for (unsigned i = 2; i * i <= _sqrt; i++)
        if (is_prime[i])
            for (unsigned j = i * i; j <= _sqrt; j += i)
                is_prime[j] = true;

    vector<unsigned> next;
    vector<unsigned> primes;
    vector<unsigned> final_primes;

    for (int64_t low = 0; low <= limit; low += segment_size)
    {
        fill(segment.begin(), segment.end(), true);

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
                segment[j] = false;
            next[i] = j - segment_size;
        }
        for (; n <= high; n += 2)
        {
            //count += segment[n - low] ? 1 : 0;
            
            if (segment[n - low])
            {
                count++;
                final_primes.push_back(n);
            }
            //if (segment[n - low])
            //    cout << setw(8) << segment[n - low];
                
            /* uncommenting the following will cause heap corruption!
            primes.push_back(n - low);
            */
        }
    }
    auto it = final_primes.begin();
    final_primes.insert(it, 2);

    cout << endl << count << " primes found." << endl << endl;

    return final_primes;
}

int GetNumberOfProcessorCores()
{
    return get_nprocs();
}

int main(int argc, char** argv)
{
    //
    vector<unsigned> _primes;
    vector<thread> _threads;
    chrono::duration<double> elapsed_seconds;
    auto start = chrono::system_clock::now();
    {
        _primes = segmented_sieve(LIMIT);
    }
    auto end = chrono::system_clock::now();
    elapsed_seconds = end - start;

    cout << "sieve time: " << setprecision(0) << fixed << elapsed_seconds.count() * 1000.0f << " ms" << endl << endl;
    
    cout << "Print primes? [Y/n]" << endl;
    char c = getchar();
    //cout << (int)c << endl;
    
    if (c == 'Y' || c == 'y' || c == 0xa)
    {
        for (auto p : _primes)
            //if (p > 1)
                cout << setw(8) << p;
            //else
            //    break;
    }
            
    cout << endl << "Vector contains " << _primes.size() << " primes." << endl << endl;
    
    N_CORES = GetNumberOfProcessorCores();
    cout << "System consists of " << N_CORES << " logical processors." << endl << endl;

    for (unsigned i = 0; i < N_CORES; i++)
    {
        _threads.emplace_back(thread(Mersenne, 23, _primes, i));
    }
    
    for (auto& t : _threads)
    {
        t.join();
    }

    
    cout << "Press Enter: ";
    getchar();

    return EXIT_SUCCESS;
}
