﻿/// @file   segmented_sieve.cpp
/// @author Kim Walisch, <kim.walisch@gmail.com> 
/// @brief  This is a simple implementation of the segmented sieve of
///         Eratosthenes with a few optimizations. It generates the
///         primes below 10^9 in 0.8 seconds (single-threaded) on an
///         Intel Core i7-4770 CPU (3.4 GHz) from 2013.
///         This is free software released into the public domain.

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
//
#include <conio.h>
#include <time.h>
#include <windows.h>

const int L1D_CACHE_SIZE = 32768;

using namespace std;

/// Generate primes using the segmented sieve of Eratosthenes.
/// This algorithm uses O(n log log n) operations and O(sqrt(n)) space.
/// @param limit         Sieve primes <= limit.
/// @param segment_size  Size of the sieve array in bytes.
///
vector<int> segmented_sieve(int64_t limit, int segment_size = L1D_CACHE_SIZE)
{
    int _sqrt = (int) sqrt((double) limit);
    //double twin_prime_const = 1.0d;
    int64_t count = (limit < 2) ? 0 : 1;
    int64_t s = 2;
    int64_t n = 3;

    // vector used for sieving
    vector<char> segment(segment_size);

    // generate small primes <= sqrt
    vector<char> is_prime(_sqrt + 1, 1);
    for (int i = 2; i * i <= _sqrt; i++)
        if (is_prime[i])
            for (int j = i * i; j <= _sqrt; j += i)
                is_prime[j] = 0;

    vector<int> primes;
    vector<int> next;
    vector<int> final_primes;
    map<char, int> lastDigitCounts;
            
    for (int64_t low = 0; low <= limit; low += segment_size)
    {
        fill(segment.begin(), segment.end(), 1);

        // current segment = interval [low, high]
        int64_t high = min(low + segment_size - 1, limit);

        // store small primes needed to cross off multiples
        for (; s * s <= high; s++)
            if (is_prime[s])
            {
                primes.push_back((int) s);
                
                //cout << setw(8) << s;
                next.push_back((int)(s * s - low));
                //cout << setw(8) << (s * s - low);
            }

        // segmented sieve of Eratosthenes
        for (size_t i = 1; i < primes.size(); i++)
        {
            int j = next[i];
            for (int k = primes[i] * 2; j < segment_size; j += k)
                segment[j] = 0;
            next[i] = j - segment_size;
        }
        
        for (; n <= high; n += 2)
        {
            //count += segment[n - low];
            if (segment[n - low] == 1)
            {
                count++;
                final_primes.push_back(n);

                char lastDigit = (char)(n % 10 | 0x30);
                if (lastDigitCounts.find(lastDigit) == lastDigitCounts.end())
                    lastDigitCounts.insert({lastDigit, 1});
                else
                    lastDigitCounts[lastDigit]++;
            }
        }
            
//      if (segment[n - low])
//      {
//          cout << setw(10) << n;
//          count++;
//          twin_prime_const *= 1.0d - 1.0d / (double)((n-1) * (n-1));
//      }  
    }
    auto it = final_primes.begin();
    final_primes.insert(it, 2);

    if (lastDigitCounts.find('2') == lastDigitCounts.end())
        lastDigitCounts.insert({'2', 1});

    cout << endl << count << " primes found." << endl;

    //DebugBreak();
    cout << "Counts / % of last digits in primes:" << endl;
    int total_primes = 0;
    //for (const auto& [name, height] : playerHeights)
    for (const auto& c : lastDigitCounts)
        total_primes += c.second;

    cout << fixed << setprecision(4);
    for (const auto& c : lastDigitCounts)
    {
        cout << "[" << c.first << "]: " << c.second << "\t" << (float)c.second/total_primes*100.0f << " %" << endl;
    }
    cout << endl;
    //cout << "twin prime constant: " << twin_prime_const << endl;
    return final_primes;
}

int main(int argc, char** argv)
{
    // generate the primes below this number
    int64_t LIMIT = 100000000;
    vector<int> _primes;

    if (argc >= 2)
        LIMIT = atoll(argv[1]);

    int size = L1D_CACHE_SIZE;
    if (argc >= 3)
        size = atoi(argv[2]);

    auto start = clock();
    {        
        _primes = segmented_sieve(LIMIT, size);
    }
    auto stop = clock();

    cout << "vector size() = " << _primes.size() << endl;
    cout << "sieve time: " << (stop - start) << " ms" << endl << endl;
    
    cout << "Print primes? [Y/n]" << endl;
    char c = _getch();
    //cout << (int)c << endl;
    
    if (c == 'Y' || c == 'y' || c == 0xd)
    {
        cout.precision(18);
        for (auto p : _primes)
            //if (p > 1)
            cout << setw(12) << p;
            //else
            //    break;
    }

    return EXIT_SUCCESS;
}

