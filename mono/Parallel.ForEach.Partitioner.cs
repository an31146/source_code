using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

class Program
{
    static void Main()
    {

        // Source must be array or IList.
        var source = Enumerable.Range(1, 10000000).ToArray();

        // Partition the entire source array.
        var rangePartitioner = Partitioner.Create(0, source.Length);

        double[] results = new double[source.Length];

        // Loop over the partitions in parallel.
        Parallel.ForEach(rangePartitioner, (range, loopState) =>
        {
            // Loop over each range element without a delegate invocation.
            for (int i = range.Item1; i < range.Item2; i++)
            {
                results[i] = Math.Sqrt(source[i]); // * Math.PI;
            }
        });

		var sw = new System.Diagnostics.Stopwatch();
		sw.Start();

            for (int i = 1; i < 10000000; i++)
            {
                results[i] = Math.Sqrt(source[i]); // * Math.PI;
            }

		sw.Stop();
		//Console.WriteLine("{0}", sw.ElapsedMilliseconds);

        Console.Write("Operation complete.  Print results?  Y/n: ");
        char input = Console.ReadKey(false).KeyChar;
        //Console.WriteLine();
        if (input == 'y' || input == 'Y' || input == 0xd)
        {
            Console.WriteLine();
            foreach(double d in results)
            {
                Console.Write("{0,12:F6}", d);
            }           
        }
        Console.WriteLine();
    }
}