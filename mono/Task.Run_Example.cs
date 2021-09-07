using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using System.Threading.Tasks;

public class Example
{
	public static int Directories(string dir)
	{
		int count = 0;
		if (Directory.Exists(dir))
		{
			//Console.WriteLine(dir);
			var subDirs = Directory.GetDirectories(dir);
			count += subDirs.Length;
			foreach (var subDir in subDirs)
				count += Directories(subDir);
		}
		return count;
	}

	public static async Task Main()
	{
		var tokenSource = new CancellationTokenSource();
		var token = tokenSource.Token;
		var files = new List<Tuple<string, string, long, DateTime>>();

		var t = Task.Run( () => { 	
									string dir = @"C:\Google Drive\Documents\";
									object obj = new Object();
									if (Directory.Exists(dir)) {
										Parallel.ForEach(Directory.GetFiles(dir),
											f => {
													if (token.IsCancellationRequested)
														token.ThrowIfCancellationRequested();
													var fi = new FileInfo(f);
													lock(obj) {
														files.Add(Tuple.Create(fi.Name, fi.DirectoryName, fi.Length, fi.LastWriteTimeUtc));          
											   }
											});
								 }
							  }
						, token);
		await Task.Yield();
		tokenSource.CancelAfter(100);
		try {
			await t;
			Console.WriteLine("Retrieved information for {0} files.", files.Count);
		}
		catch (AggregateException e) {
			Console.WriteLine("Exception messages:");
			foreach (var ie in e.InnerExceptions)
				Console.WriteLine("   {0}: {1}", ie.GetType().Name, ie.Message);

			Console.WriteLine("\nTask status: {0}", t.Status);       
		}
		finally {
			tokenSource.Dispose();
		}
		
		Console.WriteLine("directory count: {0}", Directories(@"C:\Python39"));
	}
}
// The example displays the following output:
//       Exception messages:
//          TaskCanceledException: A task was canceled.
//          TaskCanceledException: A task was canceled.
//          ...
//       
//       Task status: Canceled