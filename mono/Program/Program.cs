using System;

namespace Program
{
    class Program
    {
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
            Console.WriteLine("\n# primes: {0}", count);
        }
    }
}
