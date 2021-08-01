using System;
using System.Collections.Generic;
using System.Linq;
 
namespace AlmostPrime
{
    class Program
    {
        public static void Main(string[] args)
        {
            /*
			KPrime kprime = new KPrime() { K = 23 };
            var almostPrimes = kprime.GetAlmostPrimes(46137348);
			foreach (var k in almostPrimes.Keys)
                Console.WriteLine("k = {0,2}: {1}",
                    k, string.Join<int>(" ", almostPrimes[k]));
			*/
			
			foreach (int k in Enumerable.Range(1, 23))
            {
                KPrime kprime = new KPrime() { K = k };
                Console.WriteLine("k = {0,2}: {1}",
                    k, string.Join<int>(" ", kprime.GetFirstN(10)));
            }
        }
    }
 
    class KPrime
    {
		int[] P = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 
				   47, 53, 59, 61, 67, 71, 73, 79, 83, 87, 89, 97};

		public int K { get; set; }
 
		public bool IsKPrime(int number)
		{
			int primes = 0;
			//for (int p = 2; p * p <= number && primes < K; ++p)
			foreach (int p in P)
			{
				while (number % p == 0 && primes < K)
				{
					number /= p;
					++primes;
				}
			}
			if (number > 1)
			{
				++primes;
			}
			return primes == K;
		}

		public List<int> GetFirstN(int n)
		{
			List<int> result = new List<int>();
			for (int number = 2; result.Count < n; ++number)
			{
				if (IsKPrime(number))
				{
					result.Add(number);
				}
			}
			return result;
		}
		
		public List<int> PrimeFactors(int n)
        {
            var factors = new List<int>();
 
            foreach (int divisor in P)
                for (; n % divisor == 0; n /= divisor)
                    factors.Add(divisor);
 
            return factors;
        }

		public Dictionary<int, List<int>> GetAlmostPrimes(int n)
		{
			Dictionary<int, List<int>> almostP = new Dictionary<int, List<int>>();
			for (int i = 2; i < n; i++)
			{
				var factors = PrimeFactors(i);
				if (factors.Count <= K && factors.Count > 0)
				{
					if (!almostP.ContainsKey(factors.Count))
						almostP.Add(factors.Count, new List<int>());
					if (almostP[factors.Count].Count < 10)
						almostP[factors.Count].Add(i);
				}
			}
			return almostP;
		}
	}
}
