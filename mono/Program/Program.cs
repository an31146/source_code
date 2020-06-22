using System;
using System.Numerics;
using System.Collections.Generic;
using System.Threading.Tasks;
using Mpir.NET;

using static Mpir.NET.mpir;

namespace Program
{
    class Program
    {
        static void TestMpirMemLeak(BigInteger x, int[] _primes)
        {
            mpz_t t = new mpz_t(1);
            foreach (int i in _primes)
                t *= i;
            mpz_nextprime(t, t);
            mpz_sub_ui(t, t, 1);
            Console.WriteLine("{0}\n", t);

            mpz_mul_2exp(t, 1, 4491);
            mpz_sub_ui(t, t, 1);

            List<mpz_t> u = new List<mpz_t>();
            for (int i = 0; i < 2000; i++)
            {
                x <<= 32768;
                u.Add(new mpz_t(x) * t);
                //mpz_nextprime(t, u[new System.Random().Next(u.Count)]);
                //Console.WriteLine("{0}\n", x);
                u.Add(new mpz_t(x));
            }

            //mpz_clear(t);
        }

        static void Main(string[] args)
        {
            int [] primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};
            int count = 0;
            for (int i = 0; i < 2000; i++)
            {
                bool bPrime = true;
                foreach (int p in primes)
                    if (i % p == 0 && i != p || i == 1)
                        bPrime = false;
                if (bPrime)
                {
                    Console.WriteLine("[{0,4:D2}] Hello World!", i);
                    count++;
                }
            }
            Console.WriteLine("\n# primes: {0}\n", count);

            BigInteger bi = new BigInteger();
            var stype = bi.GetType();
            if (BigInteger.TryParse("2f478f5daf038b6471e79c49d193786042392e30013036855b0c7c08426cbbcda0b38276675cff7e1a774dc3a4803cf0726cc641b1feef7b7b692367", 
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
