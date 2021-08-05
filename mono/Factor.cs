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

	private static List<(uint, uint)> Factors(BigInteger N, List<uint> primes)
	{
		List<(uint, uint)> facts = new List<(uint, uint)>();
	
		N = BigInteger.Abs(N);
		foreach (uint pr in primes)
		{
			uint i = 0;
			while ((N % pr).IsZero)    // divisible by prime in primes List
			{
				N /= pr;
				i++;
			}
			if (i > 0)
				facts.Add((pr, i));
		}
		
		while (!N.IsOne) 
		{
			for (uint pr = primes[primes.Count-1]; pr <= (ulong)Math.Sqrt((double)N); pr += 2)
			{
				uint i = 0;
				while ((N % pr).IsZero)    // trial division
				{
					N /= pr;
					i++;
				}
				if (i > 0)
					facts.Add((pr, i));
			}
			if (!N.IsOne)
			{
				facts.Add(((uint)N, 1));
				break;
			}
		}

		return facts;
	}
	
	private static string Factors(BigInteger N, List<uint> primes, uint highest_prime)
	{
		string factorStr = "[";
		string sign = N.Sign == -1 ? "-" : "";
		N = BigInteger.Abs(N);
		foreach (uint pr in primes)
		{
			int i = 0;
			while ((N % pr).IsZero)    // divisible by prime in primes[]
			{
				N /= pr;
				i++;
			}
			if (i > 0)
				factorStr += $"{pr}, {i}; ";
		}
		while (!N.IsOne) 
		{
			for (ulong pr = highest_prime; pr <= (ulong)Math.Sqrt((double)N); pr += 2)
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
			if (!N.IsOne)
			{
				factorStr += $"{N}, 1; ";
				break;
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
		List<uint> primesRev = primes;
		primesRev.Reverse();
		//while (divisor < N)
		{
			foreach (uint pr in primesRev)
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
				if (N / divisor == 150)
					Console.WriteLine("{0} * {1} = {2}", N / divisor, divisor, divisor);
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
			foreach (ulong d in quotients)
				if (!quotients.Contains((ulong)N / d) && 
					!divisors.Contains((ulong)N / d) &&
					(N % d).IsZero)
				{
					divisors.Add((ulong)N / d);
					divisorStr += $"{d} ";
				}
			divisors.AddRange(quotients);

		}
		divisorStr = divisorStr.Remove(divisorStr.Length - 1, 1) + "]"; 
		Console.WriteLine("divisorStr: {0}\ncount: {1}\n", divisorStr, divisors.Count);
		divisors.Sort();
		
		return divisors;
	}
	
	private static List<ulong> Divisors(BigInteger N)
	{
		string divisorStr = "[1 ";
		var saveN = N;
		List<ulong> even_divisors = new List<ulong>();
		
		{
			ulong divisor = 2;
			while ( (N & 1).IsZero && (N % divisor).IsZero )
			{
				even_divisors.Add(divisor);
				divisorStr += $"{divisor} ";
				
				divisor <<= 1;
			}
		}
		
		List<ulong> odd_divisors = new List<ulong>();
		for (uint divisor = 3; divisor <= (uint)Math.Sqrt((double)N); divisor += 2)
		{
			while ((N % divisor).IsZero)
			{
				odd_divisors.Add(divisor);
				divisorStr += $"{divisor}|";
				
				if (!even_divisors.Contains((uint)N / divisor) &&
					!odd_divisors.Contains((uint)N / divisor))
				{
					odd_divisors.Add((uint)N / divisor);
					divisorStr += $"{N / divisor}*";
				}
				divisor *= divisor;
			}
		}
		
		N = saveN;
		List<ulong> divisors2 = new List<ulong>();
		foreach (ulong even in even_divisors)
		{
			foreach (ulong odd in odd_divisors)
					
				if ((N % (even * odd)).IsZero)
				{
					if (!odd_divisors.Contains(even * odd))
					{
						divisors2.Add(even * odd);
						divisorStr += $"{even * odd}!";
					}
				}
		}
		divisors2.AddRange(even_divisors);
		divisors2.AddRange(odd_divisors);
		divisors2.Add(1);
		if (!divisors2.Contains((ulong)N))
			divisors2.Add((ulong)N);
		
		divisors2.Sort();
		
		divisorStr = divisorStr.Remove(divisorStr.Length - 1, 1) + "]"; 
		Console.WriteLine(divisorStr);
		
		return divisors2;
	}
	
	private static List<ulong> Divisors(int curIndex, ulong curDivisor,
										List<(uint, uint)> factors)
	{
		List<ulong> divisors = new List<ulong>();
		// Base case i.e. we do not have more
		// primeFactors to include
		//Debug.Assert(factors.Count == 3);
		//Console.WriteLine("[{0}; {1}] ", factors[curIndex].Item1, factors[curIndex].Item2);
		if (curIndex == factors.Count)
		{
			divisors.Add(curDivisor);
			Console.Write(curDivisor + " ");
			return new List<ulong>() { curDivisor };
		}

		for (int i = 0; i <= factors[curIndex].Item2; i++)		// loop over exponents
		{
			divisors.AddRange(Divisors(curIndex + 1, curDivisor, factors));
			
			curDivisor *= factors[curIndex].Item1;
		}
		divisors.Sort();
		return divisors;
	}

	private static ulong EulerPhi(BigInteger N, List<uint> primes)
	{
		ulong phi = 1;
		foreach (uint pr in primes)
		{
			int i = 0;
			while ((N % pr).IsZero)		// divisible by prime in primes List<uint>
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
		List<(uint, uint)> prime_factors;
		//Console.WriteLine("[" + string.Join<uint>(" ", primes_list) + "]");
			/*if (primes_list.Count > 0)
				prime_factors = Factors(N, primes_list);
			else
				prime_factors = Factors(N, primes.ToList());
			*/
			factors = Factors(N, primes);		// returns a string
			
		sw.Stop();
		//factors = "[" + string.Join<(uint, uint)>(" ", prime_factors) + "]";
		Console.WriteLine($"Factors() took: {sw.ElapsedMilliseconds} ms\n{factors}\n");
		
		sw.Restart();
			//string divisors = string.Join<ulong>(" ", Divisors(0, 1, prime_factors));
			string divisors = string.Join<ulong>(" ", Divisors(N, primes_list));
		sw.Stop();
		Console.WriteLine($"Divisors() took: {sw.ElapsedMilliseconds} ms\n[{divisors}]\n");
		
		Console.WriteLine($"EulerPhi(): {EulerPhi(N, primes_list)}");
	}
}
