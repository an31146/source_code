/// @file   segmented_sieve.cpp
/// @author Kim Walisch, <kim.walisch@gmail.com> 
/// @brief  This is a simple implementation of the segmented sieve of
///         Eratosthenes with a few optimizations. It generates the
///         primes below 10^9 in 0.8 seconds (single-threaded) on an
///         Intel Core i7-4770 CPU (3.4 GHz) from 2013.
///         This is free software released into the public domain.

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>

const int L1D_CACHE_SIZE = 32768;

using namespace std;

/// Generate primes using the segmented sieve of Eratosthenes.
/// This algorithm uses O(n log log n) operations and O(sqrt(n)) space.
/// @param limit         Sieve primes <= limit.
/// @param segment_size  Size of the sieve array in bytes.
///
void segmented_sieve(int64_t limit, int segment_size = L1D_CACHE_SIZE)
{
  int sqrt = (int) std::sqrt((double) limit);
  //double twin_prime_const = 1.0d;
  
  // generate small primes <= sqrt
  vector<char> is_prime(sqrt + 1, 1);
  for (int i = 2; i * i <= sqrt; i++)
    if (is_prime[i])
      for (int j = i * i; j <= sqrt; j += i)
        is_prime[j] = 0;

  vector<int> primes;
  vector<int> next;
  int64_t count = (limit < 2) ? 0 : 1;
  int64_t s = 2;
  int64_t n = 3;
  
  for (int64_t low = 0; low <= limit; low += segment_size)
  {
    // vector used for sieving
    vector<char> segment(segment_size);
    fill(segment.begin(), segment.end(), 1);

    // current segment = interval [low, high]
    int64_t high = min(low + segment_size - 1, limit);

    // store small primes needed to cross off multiples
    //#pragma omp parallel for shared(primes,next)
    for (; s * s <= high; s++)
      if (is_prime[s])
      {
        primes.push_back((int) s);
        next.push_back((int)(s * s - low));
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
        count += segment[n - low];
//      if (segment[n - low])
//      {
//        //cout << setw(10) << n;
//        count++;
//        twin_prime_const *= 1.0d - 1.0d / (double)((n-1) * (n-1));
//      }  
  }

  cout << endl << count << " primes found." << endl;
  //cout.precision(18);
  //DebugBreak();
  //cout << "twin prime constant: " << twin_prime_const << endl;
}

int main(int argc, char** argv)
{
  // generate the primes below this number
  int64_t limit = 100000000;
  int start, stop;
  
  if (argc >= 2)
    limit = atoll(argv[1]);

  int size = L1D_CACHE_SIZE;
  if (argc >= 3)
    size = atoi(argv[2]);

  start = clock();
  segmented_sieve(limit, size);
  stop = clock();
  
  cout << "sieve time: " << (stop-start) << " ms." << endl;
  return 0;
}

