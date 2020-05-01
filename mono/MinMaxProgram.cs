using System;

class MinMaxProgram
{
    public static void Main(string[] args)
    {
        int min = Int32.MaxValue, max = Int32.MinValue;
        Random rand = new Random();
        while (min != Int32.MinValue && max != Int32.MaxValue)
        {
            int rndInt = rand.Next() | (rand.Next() & 1) << 31;
            Console.Write("{0,10}\t{1,10}\r", min, max);
            
            if (rndInt > max)
                max = rndInt;
            if (rndInt < min)
                min = rndInt;
        }
        Console.ReadLine();
    }
}