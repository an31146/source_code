using System;
using System.Linq;
using System.Numerics;
using System.Collections.Generic;
using System.Threading.Tasks;
using Mpir.NET;

using static Mpir.NET.mpir;
using static System.Numerics.BigInteger;

namespace Program
{
    class Program
    {
		static uint LIMIT = 2000000;
		static uint[] primes = new uint[LIMIT];
		static BigInteger[] C = new BigInteger[] { Zero, Zero, Zero, Zero, Zero };

        static void TestMpirMemLeak(BigInteger x, uint[] _primes)
        {
            mpz_t t = new mpz_t(1);
            foreach (int i in _primes)
                t *= i;
            mpz_nextprime(t, t);
            mpz_sub_ui(t, t, 1);
            //Console.WriteLine("{0}\n", t);

            mpz_mul_2exp(t, 1, 4491);
            mpz_sub_ui(t, t, 1);

            List<mpz_t> u = new List<mpz_t>();
            for (int i = 0; i < 1000; i++)
            {
                x <<= 32768;
                u.Add(new mpz_t(x) * t);
                //mpz_nextprime(t, u[new System.Random().Next(u.Count)]);
                //Console.WriteLine("{0}\n", x);
                u.Add(new mpz_t(x));
            }

            Console.WriteLine("TestMpirMemLeak task ended...");
            //mpz_clear(t);
        }

		static BigInteger F(int a, int b) => 	Pow(a, 5) 	- C[0] * Pow(a, 4) * b
															+ C[1] * Pow(a, 3) * Pow(b, 2) 
															- C[2] * Pow(a, 2) * Pow(b, 3) 
															+ C[3] 		* a 	* Pow(b, 4) 
															- C[4] 				* Pow(b, 5);
		
		static void AFB_relations(BigInteger primorial, BigInteger primorial2)
		{

			BigInteger N = Parse("6121149868564177516789267858123628666058719298150814090183132869931525893881272355067160797193977749");
			BigInteger root = Parse("90649748298623833399");
			
			C[0] = N - Pow(root, 5);
			C[0] /= Pow(root, 4);
			C[1] = N - Pow(root, 5) - C[0] * Pow(root, 4);
			C[1] /= Pow(root, 3);
			C[2] = N - Pow(root, 5) - C[0] * Pow(root, 4) - C[1] * Pow(root, 3);
			C[2] /= root * root;
			C[3] = N - Pow(root, 5) - C[0] * Pow(root, 4) - C[1] * Pow(root, 3) - C[2] * root * root;
			C[3] /= root;
			C[4] = N - Pow(root, 5) - C[0] * Pow(root, 4) - C[1] * Pow(root, 3) - C[2] * root * root - C[3] * root;

			for (int b = 1; b <= 10; b++)		// 
			{
				for (int a = -10000; a < 10000; a++)
				{	
					BigInteger norm = F(a, b);
					
					mpz_t R = 0;
					mpz_gcd(R, new mpz_t(Abs(norm)), new mpz_t(primorial));
					
					if (R.ToBigInteger().Equals(Abs(norm)))
					//if (R != 1)
					{
						if (GreatestCommonDivisor(norm, primorial).Equals(Abs(norm)))
							Console.WriteLine($"F({a}, {b}) = {norm}");
					}
				}
			}
		}
		
        static void Main(string[] args)
        {
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

            //int [] primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};
            Console.WriteLine("\n# primes: {0}\n", p);

			BigInteger primorial = 1;
			primorial = primes.Aggregate(One, (a, b) => Multiply(a, b));;
			var hundred_primes = primes.Where(pr => pr < 100).Aggregate(One, (a, b) => Multiply(a, b));
				
			DateTime start = DateTime.Now;
			Console.WriteLine("Start: {0}", start.ToLongTimeString());
			AFB_relations(primorial, hundred_primes);
			DateTime stop = DateTime.Now;
			Console.WriteLine("Stop:  {0}\nTime:  {1:F1} s", stop.ToLongTimeString(), stop.Subtract(start).TotalSeconds);
			
			return;
			
            BigInteger bi = new BigInteger();
            var stype = bi.GetType();
            if (TryParse("2f478f5daf038b6471e79c49d193786042392e30013036855b0c7c08426cbbcda0b38276675cff7e1a774dc3a4803cf0726cc641b1feef7b7b692367", 
                    System.Globalization.NumberStyles.AllowHexSpecifier, System.Globalization.CultureInfo.CurrentCulture.NumberFormat, out bi))
                Console.WriteLine("Parsing succeeded!");


            List<Task> tasks = new List<Task>();
            for (int i = 0; i < 10; i++)
            {
                Task t = Task.Run(() => TestMpirMemLeak(bi, primes));
                tasks.Add(t);
            }
            Task.WaitAll(tasks.ToArray());
        }
    }
}
