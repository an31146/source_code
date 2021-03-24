/*
(100x + 10y + z)(100a + 10b + c)
= (10000ax + 1000(ay + bx) + 100(by + xc + az) + 10(bz + yc) + zc

4201174131347391599 x 
5063414205274294799
=
21272284775495278625996196172821993601

10639912827169169952687259457748227212

1 * 1 = 1
7 * 3 = 21
9 * 9 = 81
*/

using System;
using System.Numerics;

public class BruteForceFactorize
{
    private static string semiPrime = "21272284775495278625996196172821993601";
    
    static void recursive(string P, string Q, int p)
    {
        string substrSemiPrime = semiPrime.Substring(p, semiPrime.Length-p);
        Console.WriteLine("substrSemiPrime: {0}", substrSemiPrime);
        
        for (int i = 0x39; i >= 0x30; i--)
        {
            
            for (int j = 0x30; j <= 0x39; j++)
            {
                int match;
                if ( String.IsNullOrEmpty(Q) && String.IsNullOrEmpty(P) )
                {
                    Console.WriteLine("i = {0}\nj = {1}", i, j);
                    match = ((i & 0x0f) * (j & 0x0f)) % (int)Math.Pow(10, semiPrime.Length-p);
                }
                else
                {
                    Console.WriteLine("P = {0}\nQ = {1}", P, Q);
                    match = (Int32.Parse(P) * Int32.Parse(Q)) % (int)Math.Pow(10, semiPrime.Length-p);
                }

                if ( match == Int32.Parse(substrSemiPrime) )
                {
                    Console.WriteLine("match: {0}", match);
                    Console.ReadLine();

                    P = (char)i + P;
                    Q = (char)j + Q;
                    recursive(P, Q, p-1);
                }
            }
        }
        Console.WriteLine("P = {0}\nQ = {1}\n", P, Q);
    }
	
	static void Fibonacci()
	{
		BigInteger a = BigInteger.One;
		BigInteger b = BigInteger.One;
		
		while (true)
		{
			BigInteger f = a + b;
			b = a;
			a = f;
			var fib = f.ToString();
			if (fib.Length > 100)
				Console.Write("{0}\r", fib.Substring(fib.Length-100, 100));
			else
				Console.Write("{0}\r", fib);
		}
	}
	
    
    public static void Main(string[] args)
    {
        string reversePrimeDigits = "";
        
        for (int i = semiPrime.Length-1; i >= 0 ; i--)
            reversePrimeDigits += semiPrime[i];
            
        Console.WriteLine(semiPrime);
        
        string P = "", Q = "";
        //recursive(P, Q, semiPrime.Length-1);
		Fibonacci();
    }
}
