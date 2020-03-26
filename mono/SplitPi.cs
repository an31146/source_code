/*
 * #!/usr/bin/mcs
 *
 * BinaryFileStreamIO.cs
 * "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\Roslyn\csc.exe" /target:exe /out:SplitPi.exe SplitPi.cs
 */
#define TEST_FILE_EXISTS

using System;
using System.IO;
using System.Text;
using System.Diagnostics;


#pragma warning disable CS0219,CS0162
// CS0219: The variable 'xxx' is assigned but its value is never used.
// CS0162: Unreachable code detected
class MyStream
{
	public static void Main(String[] args)
    {
 		StreamReader sr = new StreamReader(@"C:\Users\richa\Google Drive\source_code\pi\pi.txt");
		StreamWriter sw = new StreamWriter(@"C:\Users\richa\Google Drive\source_code\pi\pi-linefeeds.txt");
		
		do {
			string strLineIn = sr.ReadLine();
			Console.WriteLine("Length: {0}", strLineIn.Length);
			
			if (strLineIn.Length == 121)
				sw.WriteLine(strLineIn);
			else
			{
				while (strLineIn.Length > 0)
				{
					string strSplit;
					if (strLineIn.Length > 121)
						strSplit = strLineIn.Substring(0, 121);
					else
						strSplit = strLineIn;
					
					sw.Write(strSplit + "\n");
					strLineIn = strLineIn.Substring(122);
				}				
			}
		} while (!sr.EndOfStream);
		sw.Close();
		sr.Close();
	}
}
