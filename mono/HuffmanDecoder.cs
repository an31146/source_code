
/*
 * HuffmanDecoder.cs
 * "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\Roslyn\csc.exe" /target:exe /out:CountHostRequests.exe CountHostRequests.cs
 */

using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Text.RegularExpressions;
using System.Text;
using System;


using static System.Console;

class Result
{
    const int CODE_LEN = 6;                     // Huffman codes should be *variable* length for optimal compression size!   Anyways...

    /*
     * Complete the 'decode' function below.
     *
     * The function is expected to return a STRING.
     * The function accepts following parameters:
     *  1. STRING_ARRAY codes
     *  2. STRING encoded
     */

    public static string decode(List<string> codes, string encoded)
    {
        Dictionary<string, char> _huffmanDict = new Dictionary<string, char>();

        // build dictionary of character mappings        
        foreach (string _code in codes)
        {
            string[] charmap = _code.Split('\t');
            
            if (!_huffmanDict.ContainsKey(charmap[1]))
            {
                // Handle special case for [newline]
                if (charmap[0].Equals("[newline]"))
                    _huffmanDict.Add(charmap[1], '\n');
                else
                    _huffmanDict.Add(charmap[1], charmap[0][0]);
            }
        }
        
        string strDecoded = "";
        
        for (int i = 0; i < encoded.Length; i += CODE_LEN)
        {
            try
            {
                string parseCode = encoded.Substring(i, 6);

                // does the code exist?
                Write(_huffmanDict[parseCode]);
                strDecoded += _huffmanDict[parseCode];
            }
            catch (ArgumentOutOfRangeException ex)
            {
                // do nothing!
            }
            catch (KeyNotFoundException ex)
            {
                WriteLine("{0}\n{1}\n", ex.Message, ex.StackTrace);
            }
        }
        
        return strDecoded;
    }   // public static string decode
}   // class Result

class Solution
{
    public static void Main(string[] args)
    {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        int codesCount = Convert.ToInt32(Console.ReadLine().Trim());

        List<string> codes = new List<string>();

        for (int i = 0; i < codesCount; i++)
        {
            string codesItem = Console.ReadLine();
            codes.Add(codesItem);
        }

        string encoded = Console.ReadLine();

        string result = Result.decode(codes, encoded);

        textWriter.WriteLine(result);

        textWriter.Flush();
        textWriter.Close();
    }
}