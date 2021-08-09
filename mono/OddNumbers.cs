/*
 * Output the odd numbers between a specified range, inclusive, to the file passed in the environment variable OUTPUT_PATH
 */
using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Collections.Generic;
using System.Diagnostics;

class Solution {

    static List<int> oddNumbers(int l, int r) {
        List<int> odd = new List<int>();
		if (l % 2 == 0)
			l++;
        for (int i = l; i <= r; i+=2)
            //if (i % 2 == 1)
                odd.Add(i);
        return odd;
    }
	
	// 4-times slower!
	static List<int> oddNumbers2(int l, int r) {
		List<int> odd = Enumerable.Range(l, r-l+1).Where(n => (n & 1) == 1).ToList();
		return odd;
	}

    static void Main(string[] args) {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), append:false);

        int l = Convert.ToInt32(Console.ReadLine().Trim());

        int r = Convert.ToInt32(Console.ReadLine().Trim());
        
        Console.WriteLine($"{l}\n{r}");

		Stopwatch sw1 = new Stopwatch();
        sw1.Start();
		List<int> res = oddNumbers2(l, r);
		sw1.Stop();

        textWriter.WriteLine(String.Join("\n", res));
		Console.WriteLine("{0} ms", sw1.ElapsedMilliseconds);
		
		textWriter.Flush();
        textWriter.Close();
    }
}
