using System;
using System.IO;
using System.Diagnostics;

public class Example
{
	public static void OpenWithStartInfo()
	{
		// Run "csc.exe /r:System.dll /out:sample.exe stdstr.cs". UseShellExecute is false because we're specifying
		// an executable directly and in this case depending on it being in a PATH folder. By setting
		// RedirectStandardOutput to true, the output of csc.exe is directed to the Process.StandardOutput stream
		// which is then displayed in this console window directly.
		using (Process compiler = new Process())
		{
			compiler.StartInfo.FileName = @"C:\Users\richa\AppData\Local\Programs\pychess\share\pychess\engines\stockfish_10_x32.exe";
			compiler.StartInfo.UseShellExecute = false;
			compiler.StartInfo.RedirectStandardInput = true;
			compiler.StartInfo.RedirectStandardOutput = true;
			compiler.Start();

			StreamReader sr = compiler.StandardOutput;
			StreamWriter sw = compiler.StandardInput;
			string stockfish, bestmove = "";
			
			sw.WriteLine("uci");
			
			stockfish = sr.ReadLine();
			while (!stockfish.Equals("uciok"))
			{
				Console.WriteLine(stockfish);
				stockfish = sr.ReadLine();
			}
			Console.WriteLine(stockfish);

			string strCmd = "position startpos moves ";
			var count = 0;
			var moves = "e2e4 ";

			while (count < 200)
			{
				//Console.ReadLine() + ' ';
				
				sw.WriteLine(strCmd + moves);
				sw.WriteLine("go depth 18");
				
				stockfish = sr.ReadLine();
				while (!stockfish.StartsWith("bestmove"))
				{
					Console.WriteLine(stockfish);
					stockfish = sr.ReadLine();
					bestmove = stockfish.Substring(9, stockfish.IndexOf(' ', 9) - 9) + ' ';
				}
				Console.WriteLine(stockfish);
				try
				{
					moves += bestmove;
				}
				catch (ArgumentOutOfRangeException ex)
				{
					Console.WriteLine("Exception: {0}", ex.Message);
					break;
				}
				count++;
			}
			
			sw.WriteLine("quit");
			Console.WriteLine("\n# moves: {0}\n{1}", count, strCmd + moves);
			
			compiler.WaitForExit();
		}
	}
		
    public static void Main(string[] args)
	{
		OpenWithStartInfo();
	}
}
