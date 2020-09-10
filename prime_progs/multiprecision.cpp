/*
 * g++ -Wall -O2 -std=c++11 -I C:\Tools\gmp-6.1.2 
 *                          -L C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\x86_64-w64-mingw32\lib 
 *                          -o multiprecision.exe multiprecision.cpp 
 *                          -l mpir
 */
#include <boost/integer/integer_log2.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random/independent_bits.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdint>

using namespace std;

int main(int argc, char** argv)
{
    using namespace boost::random;
    using namespace boost::multiprecision;
	
    typedef mpz_int int_type;
    time_t time_seed = time(NULL);
    boost::random::mt11213b base_gen(time_seed);
    boost::random::independent_bits_engine<mt11213b, 256, int_type> gen(base_gen);

    // We must use a different generator for the tests and number generation, otherwise
    // we get false positives.
    //
    boost::random::mt19937 gen2(time_seed);
    int_type n = gen();

    uint32_t LIMIT = 100000000;
    if (argc == 2)
        LIMIT = atol(argv[1]);
        
    for (uint32_t i = 0; i < LIMIT; ++i)
    {
        if (miller_rabin_test(n, 13, gen2))
        {
            // Value n is probably prime, see if (n-1)/2 is also prime:
            cout << "probable prime: " << endl << hex << showbase << n << endl << endl;
            if (miller_rabin_test((n-1)/2, 13, gen2))
            {
                cout << "safe prime: " << endl << hex << showbase << n << endl << endl;
                return 0;
            }
        }
        n += 2;
    }
    cout << "Awwww, no safe primes were found." << endl;

    return 1;
}
