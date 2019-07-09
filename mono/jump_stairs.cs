
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

public class Challenge {
    public static long Solution(long n) {
        // Type your solution here
        long jumps = 1, a = 1, b = 1, c;

        if (n == 0) return 0;

        for (long i = 1; i < n; i++)
        {
            c = a;
            a = jumps;
            jumps += c;
        }
        Console.WriteLine();
    
        return jumps;
    }

    public static void Main(String[] args)
    {
        if (args.Length == 1)
            Console.WriteLine(Solution(Int64.Parse(args[0])));
        else
            Console.WriteLine(Solution(22L));
    }
}
