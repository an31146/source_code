// This code example demonstrates the Console.WriteLine() method.
// Formatting for this example uses the "en-US" culture.

using System;
class Sample 
{
    enum Color {Yellow = 1, Blue, Green};
    static DateTime thisDate = DateTime.Now;

    public static void Main() 
    {
    Console.Clear();

// Format a negative integer or floating-point number in various ways.
    Console.WriteLine("Standard Numeric Format Specifiers");
    Console.WriteLine(
        "(C) Currency: . . . . . . . . {0:C}\n" +
        "(D) Decimal:. . . . . . . . . {0:D}\n" +
        "(E) Scientific: . . . . . . . {1:E}\n" +
        "(F) Fixed point:. . . . . . . {1:F}\n" +
        "(G) General:. . . . . . . . . {0:G}\n" +
        "    (default):. . . . . . . . {0} (default = 'G')\n" +
        "(N) Number: . . . . . . . . . {0:N}\n" +
        "(P) Percent:. . . . . . . . . {1:P}\n" +
        "(R) Round-trip: . . . . . . . {1:R}\n" +
        "(X) Hexadecimal:. . . . . . . {0:X}\n",
        -123, -123.45f); 

// Format the current date in various ways.
    Console.WriteLine("Standard DateTime Format Specifiers");
    Console.WriteLine(
        "(d) Short date: . . . . . . . {0:d}\n" +
        "(D) Long date:. . . . . . . . {0:D}\n" +
        "(t) Short time: . . . . . . . {0:t}\n" +
        "(T) Long time:. . . . . . . . {0:T}\n" +
        "(f) Full date/short time: . . {0:f}\n" +
        "(F) Full date/long time:. . . {0:F}\n" +
        "(g) General date/short time:. {0:g}\n" +
        "(G) General date/long time: . {0:G}\n" +
        "    (default):. . . . . . . . {0} (default = 'G')\n" +
        "(M) Month:. . . . . . . . . . {0:M}\n" +
        "(R) RFC1123:. . . . . . . . . {0:R}\n" +
        "(s) Sortable: . . . . . . . . {0:s}\n" +
        "(u) Universal sortable: . . . {0:u} (invariant)\n" +
        "(U) Universal full date/time: {0:U}\n" +
        "(Y) Year: . . . . . . . . . . {0:Y}\n", 
        thisDate);

// Format a Color enumeration value in various ways.
    Console.WriteLine("Standard Enumeration Format Specifiers");
    Console.WriteLine(
        "(G) General:. . . . . . . . . {0:G}\n" +
        "    (default):. . . . . . . . {0} (default = 'G')\n" +
        "(F) Flags:. . . . . . . . . . {0:F} (flags or integer)\n" +
        "(D) Decimal number: . . . . . {0:D}\n" +
        "(X) Hexadecimal:. . . . . . . {0:X}\n", 
        Color.Green);       
    }
}
