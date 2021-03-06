/*
 * Output the odd numbers between a specified range, inclusive, to the file passed in the environment variable OUTPUT_PATH
 */
using System;
using System.IO;
using System.Text;
using System.Collections.Generic;

class Solution {

    static List<int> oddNumbers(int l, int r) {
        List<int> odd = new List<int>();
        for (int i = l; i <= r; i++)
            if (i % 2 == 1)
                odd.Add(i);
        return odd;
    }

    static void Main(string[] args) {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        int l = Convert.ToInt32(Console.ReadLine().Trim());

        int r = Convert.ToInt32(Console.ReadLine().Trim());
        
        Console.WriteLine($"{l}\n{r}");

        List<int> res = oddNumbers(l, r);

        textWriter.WriteLine(String.Join("\n", res));

        textWriter.Flush();
        textWriter.Close();
    }
}
