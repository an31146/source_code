using System;
using System.Collections.Generic;
using System.Globalization;

public class Example
{
    public static List<string> reformatDate(List<string> dates)
    {
        List<string> _list = new List<string>();
        DateTime _date;
        string strReformattedDate;

        foreach (string dt in dates)
        {
            string _dt;
            // get rid of the ordinal suffixes!
            _dt = dt.Replace("th", "").Replace("nd", "").Replace("rd", "").Replace("st", "");
            
            //Console.WriteLine(_dt);

            if (DateTime.TryParse(_dt, out _date))
            {
                strReformattedDate = _date.ToString("yyyy-MM-dd");
                _list.Add(strReformattedDate);
            }
        }

        return _list;
    }

    public static void Main()
    {
        string[] dateStrings = {"05/01/2009 14:57:32.8", "2009-05-01 14:57:32.8",
                                "2009-05-01T14:57:32.8375298-04:00", "5/01/2008",
                                "5/01/2008 14:57:32.80 -07:00", 
                                "1 May 2008 2:57:32.8 PM", "16-05-2009 1:00:32 PM", 
                                "Fri, 15 May 2009 20:10:57 GMT" };
        string[] datesHackerRank = {"20th Oct 2052", "6th Jun 1933", "26th May 1960", "20th Sep 1958", "16th Mar 2068", "25th May 1912", "16th Dec 2018", "26th Dec 2061", "4th Nov 2030", "28th Jul 1963" };
        DateTime dateValue;

        Console.WriteLine("Attempting to parse strings using {0} culture.", 
                        CultureInfo.CurrentCulture.Name);
        
        foreach (string dateString in dateStrings)
        {
            if (DateTime.TryParse(dateString, out dateValue)) 
                Console.WriteLine("  Converted '{0}' to {1} ({2}).", dateString, 
                                  dateValue, dateValue.Kind);
            else
                Console.WriteLine("  Unable to parse '{0}'.", dateString);
        }
        
        Console.WriteLine();
        
        List<string> reformatted_dates = reformatDate(new List<string>(datesHackerRank));
        foreach (string strDT in reformatted_dates)
        {
            Console.WriteLine(strDT);
        }
   }
}
// The example displays output like the following:
//    Attempting to parse strings using en-US culture.
//      Converted '05/01/2009 14:57:32.8' to 5/1/2009 2:57:32 PM (Unspecified).
//      Converted '2009-05-01 14:57:32.8' to 5/1/2009 2:57:32 PM (Unspecified).
//      Converted '2009-05-01T14:57:32.8375298-04:00' to 5/1/2009 11:57:32 AM (Local).
//
//      Converted '5/01/2008' to 5/1/2008 12:00:00 AM (Unspecified).
//      Converted '5/01/2008 14:57:32.80 -07:00' to 5/1/2008 2:57:32 PM (Local).
//      Converted '1 May 2008 2:57:32.8 PM' to 5/1/2008 2:57:32 PM (Unspecified).
//      Unable to parse '16-05-2009 1:00:32 PM'.
//      Converted 'Fri, 15 May 2009 20:10:57 GMT' to 5/15/2009 1:10:57 PM (Local).
