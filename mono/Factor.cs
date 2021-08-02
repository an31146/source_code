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
			return "No factors found, probable prime.";
	}

	private static List<uint> Divisors(BigInteger N, List<uint> primes)
	{
		string divisorStr = "[";
		List<uint> divisors = new List<uint>() { 1 };
		uint primorial = 1;
		//while (divisor < N)
		{
			foreach (uint pr in primes)
			{
				uint divisor = pr;
				primorial *= pr;
				while ((N % divisor).IsZero && !divisors.Contains(divisor))
				{
					divisors.Add(divisor);
					divisorStr += $"{divisor} ";
					divisor *= pr;
				}
			}
			//divisors.Add(primorial);
			//divisorStr += $"{primorial} ";

			List<uint> quotients = new List<uint>();
			foreach (uint d in divisors)
			{	
				if (!divisors.Contains((uint)N/d))
				{
					quotients.Add((uint)N/d);
					divisorStr += $"{N/d} ";
				}
			}
			divisors.AddRange(quotients);
		}
		divisorStr = divisorStr.Remove(divisorStr.Length - 1, 1) + "]"; 
		divisors.Sort();
		return divisors;
	}
	
	private static ulong EulerPhi(BigInteger N, List<uint> primes)
	{
		ulong phi = 1;
		foreach (uint pr in primes)
		{
			int i = 0;
			while ((N % pr).IsZero)    // divisible by prime in factor_base[]
			{
				N /= pr;
				i++;
			}
			if (i > 1)
				phi *= (ulong)Math.Pow(pr, i-1);
			phi *= pr - 1;
		}
		return phi;
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
		
			factors = Factors(N, primes);
		sw.Stop();
		Console.WriteLine($"Factors() took: {sw.ElapsedMilliseconds} ms\n{factors}\n");
		
		sw.Restart();
			string divisors = string.Join<uint>(" ", Divisors(N, primes_list));
		sw.Stop();
		Console.WriteLine($"Divisors() took: {sw.ElapsedMilliseconds} ms\n{divisors}\n");
		
		Console.WriteLine($"EulerPhi(): {EulerPhi(N, primes_list)}");
	}
}
