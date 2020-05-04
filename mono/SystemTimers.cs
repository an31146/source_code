using System;
using System.Timers;

class Timers
{
    static void MyTimerHandler(object sender, ElapsedEventArgs e)
    {
        Console.WriteLine("MyTimerHandler triggered at: {0}", e.SignalTime);
    }
    
    public static void Main(string[] args)
    {
        Timer t = new Timer(5000.0d);
        Timer t2 = new Timer(1000.0d);
        
        t.Elapsed += MyTimerHandler;
        t2.Elapsed += MyTimerHandler;
        t.Start();
        t2.Start();
        
        Console.Write("Timers started...  Press Enter to quit: ");
        Console.ReadLine();
        
        t.Stop();
        t2.Stop();
    }
}