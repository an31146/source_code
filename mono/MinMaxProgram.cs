using System;

class MinMaxProgram
{
    // How dangerous is the following function?
    unsafe static void Q()
    {
        try
        {
            byte* buffer = stackalloc byte[524288];
            
            byte[] buffer2 = new byte[524288];
            for (int i=0; i<buffer2.Length; i++)
            {
                buffer[i] = 0x55;
                buffer2[i] = 0x55;
            }
                
            fixed (byte* address_of_buffer = &buffer2[0])
            {
                Console.WriteLine("{0:X8}\t{1:X8}", (uint)buffer, (uint)address_of_buffer);
                for (int i = 0; i < buffer2.Length; i++)
                    if (address_of_buffer[i] != 0x55)
                    {
                        throw new ArgumentException();
                        // continue;
                    }
            }
        }
        catch (AccessViolationException ex)
        {
            Console.WriteLine("Caught exception: {0}", ex.Message);
        }
    }

    unsafe static void Q1(int recursion)
    {
        if (recursion == 5000)
            return;

        try
        {
            byte[] buffer = new byte[524288];
            for (int i=0; i<buffer.Length; i++)
            {
                buffer[i] = 0x55;
            }

            fixed (byte* address_of_buffer = &buffer[0])
            {
                Console.WriteLine("{0:X8}\trecursion: {1}", (uint)address_of_buffer, recursion);
                for (int i = 0; i < buffer.Length; i++)
                    if (address_of_buffer[i] != 0x55)
                    {
                        throw new ArgumentException();
                        // continue;
                    }
            }
        }
        catch (AccessViolationException ex)
        {
            Console.WriteLine("Caught exception: {0}", ex.Message);
        }
        Q1(recursion+1);
    }        
    
    static void X()
    {
        int min = Int32.MaxValue-1, max = Int32.MinValue;
        Random rand = new Random();
        System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
        
        sw.Start();
        while ( !(min == 0 && max == Int32.MaxValue-1) )
        {
            int rndInt = rand.Next(); // & Int32.MaxValue; // | (rand.Next() & 1) << 31;
            
            if (rndInt > max)
            {
                max = rndInt;
                Console.Write("{0,11}\t{1,11}\r", min, max);
            }
            else if (rndInt < min)
            {
                min = rndInt;
                Console.Write("{0,11}\t{1,11}\r", min, max);
            }
        }
        sw.Stop();
        //Console.WriteLine("{0,11}\t{1,11}", min, max);
        Console.WriteLine("\nElapsed time: {0:F1} secs.", sw.Elapsed.TotalSeconds);
    }
    
    static void Y()
    {
        double min = 1.0d, max = 0.0d;
        Random rand = new Random();
        System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
        
        // double MaxDouble = 1.0d - 1.0d / Math.Pow(2, 50);       // Random.NextDouble() will never generate this: 0.999999999999999000
        const double MaxDouble = 1.0d - 1.0d / int.MaxValue;        // i.e. 0.999999999534339 
        Console.WriteLine("1.0 - 1.0 / {1} = {0:F15}", MaxDouble, int.MaxValue);
        // Console.WriteLine("{0:F18} * 1.0000000004656612877414201272106 = {1:F18}", MaxDouble, MaxDouble * 1.0000000004656612877414201272106d);
        
        sw.Start();
        while ( !(min == 0 && max == MaxDouble) )
        {
            double rndDouble = rand.NextDouble();
            
            if (rndDouble > max)
            {
                max = rndDouble;
                Console.Write("{0,18:F15}\t{1,18:F15}\r", min, max);
            }
            else if (rndDouble < min)
            {
                min = rndDouble;
                Console.Write("{0,18:F15}\t{1,18:F15}\r", min, max);
            }
        }
        sw.Stop();
        //Console.WriteLine("{0,11}\t{1,11}", min, max);
        Console.WriteLine("\nElapsed time: {0:F1} secs.", sw.Elapsed.TotalSeconds);
    }
    
    public static void Main(string[] args)
    {
        X();
        
        Console.Write("\nPress Enter: ");
        Console.ReadLine();
    }
}