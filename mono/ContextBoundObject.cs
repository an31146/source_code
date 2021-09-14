using System;
using System.Threading;
using System.Runtime.Remoting.Contexts;
 
[Synchronization]
public class AutoLock : ContextBoundObject
{
	static int delay;
	
	public AutoLock()
	{
		 delay = 1000;
	}
	
	public void Demo()
	{
		Console.Write ($"Start... Delay: {delay}ms");
		Thread.Sleep (delay);           // We can't be preempted here
		delay += 1000;
		Console.WriteLine (" ...end");     // thanks to automatic locking!
	} 
}
 
public class Test
{
  public static void Main()
  {
    AutoLock safeInstance = new AutoLock();
    new Thread (safeInstance.Demo).Start();     // Call the Demo
    new Thread (safeInstance.Demo).Start();     // method 3 times
    new Thread (safeInstance.Demo).Start();     // method 4 times
    new Thread (safeInstance.Demo).Start();     // method 5 times
    safeInstance.Demo();                        // concurrently.
  }
}
