/*
 * #!/usr/bin/mcs
 *
 * SplitPi.cs - Split a single-line output file of digits of pi into multiple lines, 120 characters each.
 *
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
    const int MAX_LINE_LENGTH = 80;
    static string strFilenameReader = @"C:\Google Drive\Projects\ParallelProgrammingSamples_RC\ComputePi\ComputePi_CSharp\IntegerPi\bin\Debug\exponent.txt";
    static string strFilenameWriter = @"C:\Users\richa\AppData\Local\Temp\exponent-crlf.txt";
    
    public static void Main(String[] args)
    {
        if (File.Exists(args[0]))
            strFilenameReader = args[0];
        else
            Console.WriteLine("Input file not found!  Using: {0}\n", strFilenameReader);
        StreamReader sr = new StreamReader(strFilenameReader);

        if (args.Length == 2)
            strFilenameWriter = args[1];
        StreamWriter sw = new StreamWriter(strFilenameWriter);
        
        int lineCount = 1;
        do {
            string strLineIn = sr.ReadLine();
            Console.WriteLine("Line #{0} length: {1}", lineCount++, strLineIn.Length);
            
            if (strLineIn.Length <= MAX_LINE_LENGTH)
                sw.WriteLine(strLineIn);
            else
            {
                while (strLineIn.Length > MAX_LINE_LENGTH)
                {
                    string strSplit = "";
                    if (strLineIn.Length > MAX_LINE_LENGTH)
                    {
                        strSplit = strLineIn.Substring(0, MAX_LINE_LENGTH);
                        strLineIn = strLineIn.Substring(MAX_LINE_LENGTH);
                        //Console.Write("{0}, ", strLineIn.Length);
                    }
                    
                    if (strLineIn.Length <= MAX_LINE_LENGTH)
                        strSplit += "\n" + strLineIn;
                    
                    sw.Write(strSplit + "\n");
                    // Console.WriteLine("2. Length: {0}", );
                    // Console.ReadLine();
                    // if (strLineIn.Length > 121)
                    
                }               
            }
        } while (!sr.EndOfStream);
        sw.Close();
        sr.Close();
    }
}
