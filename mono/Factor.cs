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
		
		return Factors(N, primes_list, primes.Last());
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
		
		for (uint pr = primes.Last(); pr <= (ulong)Math.Sqrt((double)N); pr += 2)
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
		if (!N.IsOne)		// Might still be factorable
		{
			facts.Add(((uint)N, 1));
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

		factorStr = factorStr.Remove(factorStr.Length - 2, 2);		// Remove trailing semicolon and whitespace
		return sign + factorStr + "]";
	}

	private static List<ulong> DivisorsX(BigInteger N, List<uint> primes)
	{
		string divisorStr = "[1 ";
		HashSet<ulong> divisors = new HashSet<ulong>() { 1, (ulong)N };
		uint primorial = 1;
		//while (divisor < N)
		{
			foreach (uint pr in primes)
			{
				ulong divisor = pr;
				while ((N % divisor).IsZero) //&& !divisors.Contains(divisor))
				{
					divisors.Add(divisor);
					if (divisorStr.IndexOf(divisor.ToString()) == 0)
						divisorStr += $"{divisor} ";
					divisors.Add((ulong)N / divisor);
					if (divisorStr.IndexOf((N / divisor).ToString()) == 0)
						divisorStr += $"{N / divisor} ";
					divisor *= pr;
				}
				foreach (uint pr1 in primes)
				{
					Console.Write("{0} * {1} - ", pr, pr1);
					divisor = pr;
					while ((N % divisor).IsZero) //&& !divisors.Contains(divisor))
					{
						divisors.Add(divisor);
						if (divisorStr.IndexOf(divisor.ToString()) == 0)
							divisorStr += $"{divisor} ";
						divisors.Add((ulong)N / divisor);
						if (divisorStr.IndexOf((N / divisor).ToString()) == 0)
							divisorStr += $"{N / divisor} ";
						divisor *= pr1;

						if ((N % (divisor * divisor)).IsZero)
						{
							divisors.Add(divisor * divisor);
							if (divisorStr.IndexOf((divisor*divisor).ToString()) == 0)
								divisorStr += $"{divisor*divisor} ";
						}					
					}
				}
				ulong pr2 = primorial;
				while ((N % pr2).IsZero)
				{
					divisors.Add((ulong)N / pr2);
					if (divisorStr.IndexOf((N / pr2).ToString()) == 0)
						divisorStr += $"{N / pr2} ";
					pr2 *= pr;
				}
					primorial *= pr;
					divisors.Add(primorial);
					divisorStr += $"{primorial}%";
			}
		}
		divisorStr = divisorStr.Remove(divisorStr.Length - 1, 1) + "]"; 
		Console.WriteLine("divisorStr: {0}\ncount: {1}\n", divisorStr, divisors.Count);
		
		var div_list = divisors.ToList();
		div_list.Sort();
		
		return div_list;
	}

	private static List<ulong> DivisorsW(BigInteger N, List<uint> primes)
	{
		var prime_factors = Factors(N, primes);
		List<uint> factors = new List<uint>();
		uint primorial = 1;
		
		foreach (var fact in prime_factors)
		{
			uint pr, expo;
			(pr, expo) = fact;
			
			primorial *= pr;
			//factors.Add(primorial);
			
			for (uint e=0, d=pr; e < expo; d*=pr, e++)
			{
				Console.Write("{0}, ", d);
				factors.Add(d);
			}			
		}
		// Console.WriteLine($"#primorial: {primorial}");

		HashSet<ulong> divisors = new HashSet<ulong>();
		foreach (uint d in factors)
		{
			Console.Write("{0}, ", d);

			uint div = d;
			while ((N % div).IsZero)
			{
				divisors.Add(div);
				divisors.Add((ulong)N / div);
				
				foreach (uint d1 in factors)
				{
					uint div1 = d * d1;
					while ((N % div1).IsZero)
					{
						divisors.Add(div1);
						divisors.Add((ulong)N / div1);
						div1 += d;
					}
				}
				div *= d;
			}
			
		}
		divisors.Add(1);
		divisors.Add((ulong)N);
		Console.WriteLine("\n{0}\ncount: {1}", string.Join<ulong>(" ", divisors), divisors.Count);
		
		var sorted_divisors = divisors.ToList();
		sorted_divisors.Sort();
		
		return sorted_divisors;
	}
	
	private static List<ulong> Divisors(BigInteger N, List<(uint, uint)> prime_factors)
	{
		Debug.Assert(prime_factors.Count > 0);
		List<uint> factors = new List<uint>();
		uint primorial = 1;
		
		foreach (var fact in prime_factors)
		{
			uint pr, expo;
			(pr, expo) = fact;
			
			primorial *= pr;
			
			for (uint e = 0, d = pr; e < expo; d *= pr, e++)
			{
				Console.Write("{0}, ", d);
				factors.Add(d);
			}
		}
		Console.WriteLine();
		HashSet<ulong> divisors = new HashSet<ulong>();
		for (uint i = 0; i < 1 << factors.Count; i++)
		{
			uint mask = i, d = 1;
			int b = 0;
			while (mask > 0 && d < N)
			{
				if ((mask & 1) == 1)
				{
					d *= factors[b];
				}
				mask >>= 1; b++;
			}
			if (d > 0 && (N % d).IsZero)
				divisors.Add(d);
		}

		//Console.WriteLine("\n{0}\ncount: {1}", string.Join<ulong>(" ", divisors), divisors.Count);
		
		var sorted_divisors = divisors.ToList();
		sorted_divisors.Sort();
		
		return sorted_divisors;
	}
	
	private static List<ulong> Divisors(BigInteger N)
	{
		string divisorStr = "[1 ";
		HashSet<ulong> divisors = new HashSet<ulong>() { 1, (ulong)N };
		
		for (uint pr = 2; pr <= (uint)Math.Sqrt((double)N); pr++)
		{
			ulong divisor = pr;
			while ((N % divisor).IsZero)
			{
				divisors.Add(divisor);
				divisorStr += $"{divisor}|";

				divisors.Add((ulong)(N / divisor));
				divisorStr += $"{N / divisor}*";
				divisor *= pr;
			}
		}
		
		var div_list = divisors.ToList();
		div_list.Sort();
		
		divisorStr = divisorStr.Remove(divisorStr.Length - 1, 1) + "]"; 
		Console.WriteLine("{0}\ncount: {1}\n", divisorStr, divisors.Count);
		
		return div_list;
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
			// Console.Write(curDivisor + " ");
			return new List<ulong>() { curDivisor };
		}
		// if (curIndex < factors.Count)
			// Console.Write("{0} ", factors[curIndex]);

		for (int i = 0; i <= factors[curIndex].Item2; i++)		// loop over exponents
		{
			divisors.AddRange(Divisors(curIndex + 1, curDivisor, factors));
			
			//Console.Write(curDivisor + " ");
			curDivisor *= factors[curIndex].Item1;
		}
		//Console.WriteLine();
		
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
				phi *= (ulong)Math.Pow(pr, i-1);		// φ(n) = Π pªˉ¹(p-1)
			phi *= pr - 1;
		}
		if (!N.IsOne)
			phi *= (ulong)N - 1;
		return phi;
	}
	
	private static ulong EulerPhi(BigInteger N, List<(uint, uint)> prime_factors)
	{
		ulong phi = 1;
		foreach (var fact in prime_factors)
		{
			(uint pr, uintexpo) = fact;
			if (expo > 1)
				phi *= (ulong)Math.Pow(pr, expo-1);		// φ(n) = Π pªˉ¹(p-1)
			phi *= pr - 1;
		}
		return phi;
	}
	
	static void Main(string[] args) {
		
		if (args.Length == 0)
		{
			Console.WriteLine("Missing argument: Number to factor.");
			Environment.Exit(-1);
		}
		BigInteger N = BigInteger.Parse(args[0]);
		
		const uint LIMIT = 1000000;
		uint[] primes = new uint[LIMIT];
		uint p = 0;
		
		primes[0] = 2;
		while (primes[p] < LIMIT) 
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
		// List<uint> primes_list = primes.Where(p => N % p == 0).ToList();
		//Console.WriteLine("[" + string.Join<uint>(" ", primes_list) + "]");
			
			List<(uint, uint)> prime_factors;
			if (primes_list.Count() > 0)
				prime_factors = Factors(N, primes_list);
			else
				prime_factors = Factors(N, primes.ToList());
			factors = "[" + string.Join<(uint, uint)>(" ", prime_factors) + "]";
			
			//factors = Factors(N, primes);		// returns a string
		sw.Stop();
		Console.WriteLine($"Factors() took: {sw.ElapsedMilliseconds} ms\n{factors}\n");
		
		if (primes_list.Count > 0)
		{
			sw.Restart();
				// var divisors = Divisors(0, 1, prime_factors);
				var divisors = Divisors(N);
				string divStr = string.Join<ulong>(" ", divisors);
			sw.Stop();
			Console.WriteLine($"Divisors() took: {sw.ElapsedMilliseconds} ms\n\n[{divStr}]\n");
			Console.WriteLine($"#divisors: {divisors.Count}\n");
			Console.WriteLine($"EulerPhi(): {EulerPhi(N, prime_factors)}");
		}
	}
}
