using System;
using System.Net;
using System.Net.Http;
using System.Threading;
using System.Threading.Tasks;
//using System.Web;

public class Program
{
    static readonly HttpClient client = new HttpClient();
    
    public static void Main()
    {
        /*HttpResponseMessage response = await client.GetAsync("http://localhost:8000/");
        Console.WriteLine(response.EnsureSuccessStatusCode());
        return;
        */
        
        ThreadPool.SetMinThreads(1, 1);
        ThreadPool.SetMaxThreads(12, 4);
        WaitHandle[] waitHandles = new WaitHandle[8];
 
        for (int i = 0; i < 8; i++)
        {
            Console.WriteLine("Queued {0}", i);
            try
            {
                waitHandles[i] = new AutoResetEvent(false);
                ThreadPool.QueueUserWorkItem(new WaitCallback(PoolFunc), waitHandles[i]);
            }
            catch (InvalidOperationException ex)
            {
                Console.WriteLine(ex);
            }
        }
        Console.WriteLine("Waiting for tasks to complete...");
        WaitHandle.WaitAll(waitHandles);
        
        Console.Write("\nPress Enter: ");
        Console.ReadLine();
    }

    private static void PoolFunc(object state)
    {
        int workerThreads, IOcompletionPortThreads;
        AutoResetEvent are = (AutoResetEvent) state;
        
        ThreadPool.GetAvailableThreads(out workerThreads, out IOcompletionPortThreads);
        Console.WriteLine(
            "Available: WorkerThreads: {0}, CompletionPortThreads: {1}",
            workerThreads,
            IOcompletionPortThreads);
        //Thread.Sleep(100);
        var cts = new CancellationTokenSource();
        cts.CancelAfter(2000);        // milliseconds

        string url = "http://localhost:8000/";

        //HttpWebRequest myHttpWebRequest;
        // Creates an HttpWebRequest for the specified URL.    
        //myHttpWebRequest = (HttpWebRequest)WebRequest.Create(url);
        // Sends the HttpWebRequest, and waits for a response.
        var bSuccess = false;
        try
        {
            //var myHttpWebResponse = await myHttpWebRequest.GetResponseAsync();
            Task<HttpResponseMessage> response = client.GetAsync(url, HttpCompletionOption.ResponseContentRead, cts.Token);
            Console.WriteLine("Wait for response.");
            // response.Wait(cts.Token);
            response.Wait();
            //var myHttpWebResponse = await client.GetStringAsync(url);
            string strResponse = response.Result.Content.ReadAsStringAsync().Result;
            bSuccess = response.Result.IsSuccessStatusCode;
            //myHttpWebResponse.Close();
        }
        catch (InvalidOperationException ex)
        {
            Console.WriteLine(ex);
        }
        catch (OperationCanceledException ex)
        {
            Console.WriteLine(ex);
        }
        catch (AggregateException ex)
        {
            Console.WriteLine(ex);
        }
        finally
        {
            Console.WriteLine("IsSuccessStatusCode: {0}", bSuccess);
            are.Set();
        }
    }
}
