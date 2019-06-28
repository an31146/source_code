
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

public class Challenge {
    public static long Solution(string parentheses, long index) {
        // Type your solution here
        if (parentheses[(int)index].Equals(')'))
            return -1;
        
        long p = 0, res = 0;
        for (int i = (int)index; i < parentheses.Length; i++)
        {
            if (parentheses[i].Equals('('))
            {
                //Console.Write("+");
                p++;
            }
            else if (parentheses[i].Equals(')'))
            {
                //Console.Write('-');
                p--;
            }
            if (p == 0 && i > index)
                res = i;
            //Console.WriteLine(i);
        }
        return res;
    }

    public static void Main(String[] args)
    {
        if (args.Length == 2)
            Console.WriteLine(Solution( args[0], Int64.Parse(args[1]) ));
        else
            Console.WriteLine(Solution( "((((()())())))", 1L ));
    }
}
