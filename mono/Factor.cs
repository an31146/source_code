using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
using System.Numerics;

class Program {

	private static string Factors(BigInteger N, uint[] primes)
	{
		var primes_list = primes.Where(p => N % p == 0).ToList();			
			
		return Factors(N, primes_list);
	}

	private static string Factors(BigInteger N, List<uint> primes)
	{
		string factorStr = "[";
		foreach (uint pr in primes)
		{
			int i = 0;
			while ((N % pr).IsZero)    // divisible by prime in factor_base[]
			{
				N /= pr;
				i++;
			}
			if (i > 0)
				factorStr += $"{pr}, {i}; ";
		}
		if (BigInteger.Abs(N).IsOne) 
		{
			factorStr = factorStr.Remove(factorStr.Length - 2, 2);		// Truncate trailing multiplication operator
			if (N.Sign == -1)
				factorStr = "-" + factorStr;
			return factorStr + "]";		// smooth number with prime bound in factor_base
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
		sw.Start();

		string factors = "\u0000";
		var primes_list = primes.Where(p => N % p == 0).ToList();			
		
		//for (int n = 0; n < 1000; n++)
			factors = Factors(N, primes_list);		// 12595 ms		19448 ms	18122 ms
		sw.Stop();
		Console.WriteLine($"Factors() took: {sw.ElapsedMilliseconds} ms\n{factors}");
	}
}
