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
    
    public static void Main(String[] args)
    {
        // StreamReader sr = new StreamReader(@"C:\Users\richa\Google Drive\source_code\pi\pi.txt");
        StreamReader sr = new StreamReader(@"C:\Users\richa\Google Drive\Documents II\python\pi_chudnovsky.txt");
        StreamWriter sw = new StreamWriter(@"C:\Users\richa\Google Drive\source_code\pi\pi-linefeeds#2.txt");
        
        do {
            string strLineIn = sr.ReadLine();
            Console.WriteLine("Length: {0}", strLineIn.Length);
            
            if (strLineIn.Length <= MAX_LINE_LENGTH)
                sw.WriteLine(strLineIn);
            else
            {
                while (strLineIn.Length > MAX_LINE_LENGTH)
                {
                    string strSplit;
                    if (strLineIn.Length > MAX_LINE_LENGTH)
                    {
                        strSplit = strLineIn.Substring(0, MAX_LINE_LENGTH);
                        strLineIn = strLineIn.Substring(MAX_LINE_LENGTH);
                    }
                    else
                        strSplit = strLineIn;
                    
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
