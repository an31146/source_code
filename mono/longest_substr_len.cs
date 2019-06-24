
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

public class Challenge {
    public static long Solution(string s) {
        // Type your solution here
        long subStrLen = 0, subStrMaxLen = 0;
        List<char> listOfChars = new List<char>(s[0]);
        
        for (int i = 1; i < s.Length; i++)
        {
            if ( !listOfChars.Contains(s[i]) )
            {
                listOfChars.Add(s[i]);
                //subStrLen++;
            }
            else if (s[i] != s[i-1])
                    subStrLen++;
                else 
                {
                    if (subStrLen > subStrMaxLen)
                        subStrMaxLen = subStrLen;
                    listOfChars.Clear();
                    subStrLen = 0;    
                }
            }

        return subStrMaxLen;
    }

    public static void Main()
    {
        Console.WriteLine(Solution("RZYrOMwUehstjBTBdMoSTnaKvVjiQDcrLYZaDoZOfehZZTZpjQSpDCmKWWXnsUJemzNvDyBcTFjifJUiMQoDqvjZrVCjcxofYnXjJLduRKuZdWRtrqkiBjRJbjNNeHJUMheypCikQrhPCacBCqQLIKXqLsymlfYVYJgLOMRAGsifAgkOdjoOJkMEQNHvaimRucGgGlNgnBJLPmRHnsRsyalreGecCTubAptdZLMVmxVyOwubqimTNOsqPHLORipOXiXxVZVlikBAmczYUtqqJHDdgnbZLNvpzxsdBSLKvTJgBgbLVESeUCJQuZjpiBHfdIFhgYLvkZRtZufjJzCbYsvvABcDjnzJbSVfZqwqEozYvrdBHWFFtHbnSoKIKFdMadptJZjkBFddzQvxYNtsGJhusdFnYtlKbEcFYdhEeuYHgHGPHOEZNZeaMERsKhgqcaVsguNnlBCSdJTPdtLxMrGQRtYgjmsEsKgRewKrKHFlohmIKHAUKdFpCmX"));
    }
}

