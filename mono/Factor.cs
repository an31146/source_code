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
			
		return Factors(N, primes_list, primes[primes.Length-1]);
	}

	private static string Factors(BigInteger N, List<uint> primes, uint highest_prime)
	{
		string factorStr = "[";
		string sign = N.Sign == -1 ? "-" : "";
		N = BigInteger.Abs(N);
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
		while (!N.IsOne) 
		{
			for (ulong pr = highest_prime; pr <= N; pr += 2)
			{
				int i = 0;
				while ((N % pr).IsZero)    // trial division
				{
					N /= pr;
					i++;
				}
				if (i > 0)
					factorStr += $"{pr}, {i}; ";
			}
		}
		factorStr = factorStr.Remove(factorStr.Length - 2, 2);		// Truncate trailing multiplication operator
		return sign + factorStr + "]";		// smooth number with prime bound in factor_base
	}

	private static List<ulong> Divisors(BigInteger N, List<uint> primes)
	{
		string divisorStr = "[1 ";
		List<ulong> divisors = new List<ulong>() { 1 };
		uint primorial = 1;
		//while (divisor < N)
		{
			primes.Reverse();
			foreach (uint pr in primes)
			{
				ulong divisor = pr;
				primorial *= pr;
				while ((N % divisor).IsZero && !divisors.Contains(divisor))
				{
					divisors.Add(divisor);
					divisorStr += $"{divisor} ";
					divisor *= pr;
				}
				if (primorial != pr)
				{
					divisors.Add(primorial);
					divisorStr += $"{primorial} ";
				}
			}
			//Console.WriteLine(divisorStr);

			List<ulong> quotients = new List<ulong>();
			foreach (ulong d in divisors)
				foreach (uint pr in primes)
				{	
					ulong d1 = d;
					while ((N % d1).IsZero)
					{
						if (!divisors.Contains((ulong)d1) &&
							!quotients.Contains((ulong)d1) &&
							(N % d1).IsZero)
						{
							quotients.Add((ulong)d1);
							divisorStr += $"{d1} ";
						}
						if (!divisors.Contains((ulong)N / d1) &&
							!quotients.Contains((ulong)N / d1) &&
							(N % d1).IsZero)
						{
							quotients.Add((ulong)N / d1);
							divisorStr += $"{N / d1} ";
						}
						d1 *= pr;
					}
				}
			divisors.AddRange(quotients);
			foreach (ulong d in quotients)
				if (!quotients.Contains((ulong)N / d) && 
					!divisors.Contains((ulong)N / d) &&
					(N % d).IsZero)
				{
					divisors.Add((ulong)N / d);
					divisorStr += $"{d} ";
				}

		}
		divisorStr = divisorStr.Remove(divisorStr.Length - 1, 1) + "]"; 
		//Console.WriteLine(divisorStr);
		divisors.Sort();
		
		return divisors;
	}
	
	private static ulong EulerPhi(BigInteger N, List<uint> primes)
	{
		ulong phi = 1;
		foreach (uint pr in primes)
		{
			int i = 0;
			while ((N % pr).IsZero)    // divisible by prime in primes List<uint>
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
		
		const uint LIMIT = 1000000;
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
			string divisors = string.Join<ulong>(" ", Divisors(N, primes_list));
		sw.Stop();
		Console.WriteLine($"Divisors() took: {sw.ElapsedMilliseconds} ms\n[{divisors}]\n");
		
		Console.WriteLine($"EulerPhi(): {EulerPhi(N, primes_list)}");
	}
}
