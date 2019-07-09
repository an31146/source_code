using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

using static System.Console;

namespace Solution
{
    class Solution
    {
        static void CountRequests(string strInFilename, string strOutFilename)
        {
            Dictionary<string, int> _HostRequestCounts = new Dictionary<string, int>();
                        
            try
            {
                StreamReader fsIn = new StreamReader(strInFilename);

                while (!fsIn.EndOfStream)
                {
                    string strRequest = fsIn.ReadLine();
                    string strHostname = strRequest.Substring(0, strRequest.IndexOf(" - "));
                    
                    //WriteLine($"{strHostname}\n{strHostname.Length}\n");
                    
                    // Check if host exists, then add OR increment as necessary
                    if (!_HostRequestCounts.ContainsKey(strHostname))
                    {
                        _HostRequestCounts.Add(strHostname, 1);
                    }
                    else
                    {
                        _HostRequestCounts[strHostname]++;
                    }
                }
                fsIn.Close();
                
                StreamWriter fsOut = new StreamWriter(strOutFilename);

                foreach (KeyValuePair<string, int> kvp in _HostRequestCounts)
                {
                    fsOut.WriteLine($"{kvp.Key} {kvp.Value}");
                }
                fsOut.Close();
                
            }
            catch (IOException ex)
            {
                WriteLine($"{ex.Message}\n{ex.StackTrace}\n\n");
            }

        }

        static void Main(string[] args)
        {
            // read the string filename
            string filename = Console.ReadLine();
            
            CountRequests(filename, "records_" + filename);
        }
    }
}

