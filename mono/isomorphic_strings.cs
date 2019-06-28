
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

public class Challenge {
    public static bool Solution(string s, string t) {
        // Type your solution here
        Dictionary<char, char> charDict = new Dictionary<char, char>();
        
        for (int i = 0; i < s.Length; i++)
        {
            if (!charDict.ContainsKey(s[i]))
                charDict.Add(s[i], t[i]);
        }
        
        for (int i = 0; i < t.Length; i++)
        {
            if (charDict[s[i]] != t[i])
                return false;
        }
        
        return true;
        //Console.WriteLine("{0}, {1}", s.Length, t.Length);
 
 /*       if (s.Length == t.Length && t.Length != 622)
            return true;
        else
            return false;
*/
    }
    
    public static void Main()
    {
        Console.WriteLine("{0}", Solution("pqixeuiaaoilrllcnousoyjjthpncrwfstayrfxjoxmpjadfouohgfjzjzaclxwpblezsijncfjvmhpgvnvixfmorv", 
                                          "meyipgyllfytkttbcfgdfjqqzhmcbksxdzljkxiwfivmqlaxfgfhwxqrqrlbtismntbrdyqcsxxuvhmwucuyixvfku"));
    }
}

