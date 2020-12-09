using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
using System.Numerics;

class Program {

	private static string GetPrimeFactorsII(BigInteger N, uint[] primes)
	{
		var primes_list = primes.Where(p => N % p == 0).ToList();			
			
		return GetPrimeFactorsII(N, primes_list);
	}

	private static string GetPrimeFactorsII(BigInteger N, List<uint> primes)
	{
		int i = 0;
		string factorStr = "";
		foreach (uint pr in primes)
		{
			while ((N % pr).IsZero)    // divisible by prime in factor_base[]
			{
				N /= pr;
				factorStr += pr.ToString() + " * ";
				i++;
			}

		}
		if (BigInteger.Abs(N).IsOne) 
		{
			factorStr = factorStr.Remove(factorStr.Length - 3, 3);		// Truncate trailing multiplication operator
			if (N.Sign == -1)
				factorStr = "-" + factorStr;
			return factorStr;		// smooth number with prime bound in factor_base
		}
		else
			return "No factors found.";
	}

	static void Main(string[] args) {
		
		BigInteger N = BigInteger.Parse(args[0]);
		
		const uint LIMIT = 2000000;
		uint[] primes = new uint[LIMIT];
		uint p;
		
		primes[0] = 2;
		for (p = 0; primes[p] < LIMIT; ) 
		{
			for (uint i = primes[p]; i < LIMIT; i += primes[p])
				primes[i] = 1;
			primes[++p] = primes[p-1] + 1;
			for (; primes[p] < LIMIT && primes[primes[p]] == 1; primes[p]++) ;     //find next prime (where s[p]==0)
		}
		Array.Resize(ref primes, (int)p);

		Stopwatch sw = new Stopwatch();
		string factors = "\u0000";
		var primes_list = primes.Where(p => N % p == 0).ToList();			
		
		sw.Start();
		for (int n = 0; n < 1000; n++)
			factors = GetPrimeFactorsII(N, primes_list);		// 12595 ms		19448 ms	18122 ms
		sw.Stop();
		Console.WriteLine($"1000 rounds of GetPrimeFactorsII() took: {sw.ElapsedMilliseconds} ms\n{factors}");
	}
}
