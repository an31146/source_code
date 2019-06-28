using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;


class Program
{
    static void Main(string[] args)
    {
        try
        {
            // Get a list of the files to use for the sorted set.
            IEnumerable<string> files1 = Directory.EnumerateFiles(@"C:\Users\rhammond\Documents", 
                                                                   "*", SearchOption.TopDirectoryOnly);
                                                                   //"*", SearchOption.AllDirectories);

            // Create a sorted set using the ByFileExtension comparer.
            SortedSet<string> mediaFiles1 = new SortedSet<string>(new ByFileExtension());

            // Note that there is a SortedSet constructor that takes an IEnumerable,
            // but to remove the path information they must be added individually.
            foreach (string f in files1)
            {
                mediaFiles1.Add(f.Substring(f.LastIndexOf(@"\") + 1));
            }

            // Remove elements that have non-media extensions.
            // See the 'isDoc' method.
            Console.WriteLine("Remove docs from the set...");
            Console.WriteLine("\tCount before: {0}", mediaFiles1.Count.ToString());
            mediaFiles1.RemoveWhere(isDoc);
            Console.WriteLine("\tCount after: {0}", mediaFiles1.Count.ToString());


            Console.WriteLine();

            Console.WriteLine("List of mediaFiles1:");
            foreach (string f in mediaFiles1)
            {
                Console.WriteLine("\t{0}",f);
            }


            Console.WriteLine();

            // List all the bas files.
            SortedSet<string> basFiles = mediaFiles1.GetViewBetween("bas", "bmp");

            Console.WriteLine("BAS files:");
            foreach (string bas in basFiles)
            {
                Console.WriteLine("\t{0}", bas);
            }


            Console.WriteLine();

            // Create another sorted set.
            IEnumerable<string> files2 = Directory.EnumerateFiles(@"C:\Users\rhammond\Documents", "*");
                                                                   //"*", SearchOption.AllDirectories);

            SortedSet<string> mediaFiles2 = new SortedSet<string>(new ByFileExtension());

            foreach (string f in files2)
            {
                mediaFiles2.Add(f.Substring(f.LastIndexOf(@"\") + 1));
            }

            // Remove elements in mediaFiles1 that are also in mediaFiles2.
            Console.WriteLine("Remove duplicates (of mediaFiles2) from the set...");
            Console.WriteLine("\tCount before: {0}", mediaFiles1.Count.ToString());
            mediaFiles1.ExceptWith(mediaFiles2);
            Console.WriteLine("\tCount after: {0}", mediaFiles1.Count.ToString());

            Console.WriteLine();

            Console.WriteLine("List of mediaFiles1:");
            foreach (string f in mediaFiles1)
            {
                Console.WriteLine("\t{0}",f);
            }

            // Create a set of the sets.
            IEqualityComparer<SortedSet<string>> comparer = SortedSet<string>.CreateSetComparer();

            Console.Write("Press Enter: ");
            Console.ReadLine();

            Console.WriteLine("List of mediaFiles1 & mediaFiles2:");

            HashSet<SortedSet<string>> allMedia = new HashSet<SortedSet<string>>(comparer);
            allMedia.Add(mediaFiles1);
            allMedia.Add(mediaFiles2);

            foreach (SortedSet<string> s in allMedia)
            {
                Console.WriteLine("{0} contains {1} items.", s, s.Count);
                
                foreach (string f in s)
                {
                    Console.WriteLine("\t{0}",f);
                }
            }
        }
        catch(IOException ioEx)
        {
            Console.WriteLine(ioEx.Message);
        }
        catch (UnauthorizedAccessException AuthEx)
        {
            Console.WriteLine(AuthEx.Message);
        }
    }   // static void Main

    // Defines a predicate delegate to use
    // for the SortedSet.RemoveWhere method.
    private static bool isDoc(string s)
    {
        if (s.ToLower().EndsWith(".txt") ||
            s.ToLower().EndsWith(".doc") ||
            s.ToLower().EndsWith(".dot") ||
            s.ToLower().EndsWith(".xls") ||
            s.ToLower().EndsWith(".xlsm") ||
            s.ToLower().EndsWith(".xlsx") ||
            s.ToLower().EndsWith(".xps") ||
            s.ToLower().EndsWith(".pdf") ||
            s.ToLower().EndsWith(".ppt") ||
            s.ToLower().EndsWith(".pptx") ||
            s.ToLower().EndsWith(".doc") ||
            s.ToLower().EndsWith(".docm") ||
            s.ToLower().EndsWith(".docx"))
        {
            return true;
        }
        else
        {
            return false;
        }
    }


}

// Defines a comparer to create a sorted set
// that is sorted by the file extensions.
public class ByFileExtension : IComparer<string>
{
    string xExt, yExt;

	CaseInsensitiveComparer caseiComp = new CaseInsensitiveComparer();

    public int Compare(string x, string y)
    {
        // Parse the extension from the file name. 
        xExt = x.Substring(x.LastIndexOf(".") + 1);
        yExt = y.Substring(y.LastIndexOf(".") + 1);

        // Compare the file extensions. 
        int vExt = caseiComp.Compare(xExt, yExt);
        if (vExt != 0)
        {
            return vExt;
        }
        else
        {
            // The extension is the same, 
            // so compare the filenames. 
            return caseiComp.Compare(x, y);
        }
    }
}