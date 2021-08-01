using System;

public class Example
{
	const string privateKey = 
		"MIIEpQIBAAKCAQEAw3lgWI25NawqOHn5pHTqs+E4oNDGQ8ortr/Dl64KP5Kv91nx" +
		"+Ll0i48X583odNAdDw4vwvd5Efo2gMAO+NPqhpOTOK5oNR8+l0dR3HOiS4XRichW" +
		"sW6nFcVWqRKvlo9lt1b9RJKHagSQYCl+k4LZq9AiW6vDP4w9XhG+Ee9oXc00TGBC" +
		"KAfS2nypruUFdoC1sk2sVtRNqaGnaBdLd6RcoSeSZQTANNl5YqbcqFktNibVtN8o" +
		"FspoPGZzFZsLenkWYXxiBZEEeIZ6mj/v1HEtXGvyJV8tOjKdvMBevF0dAOd7JXqS" +
		"uN8mD9BRF1mz68pTprDtaK9MGhjNmJm/OPt1rQIDAQABAoIBADzqKysNZ0UhlLqq" +
		"0C/+u1E5A1OiG9buPrchhUtUUh9DkBCN+Ajhij8fZXy4PxN/8z5I1nt0cUc36Row" +
		"P7otF+2+OWe1egXr6a5s7Ea9xjqwxf6g7ts2N11idMIVb6CEqVb8BcsDuq1AJNnC" +
		"tI41fa28G6jjmuHHyyvwbYHbUHPCPmhWLPuZIlUzlvokMn58HpMmAVb0uj0EXmbJ" +
		"0Q+veR60BSGvFaXYZ848eoq49ah+67r4xfB4wmUVQysuarFznJje0Kzf3NBD1BZY" +
		"GpmdzrmzKK3vqzzJpPpaYO0vMCaouxChbw6EFrddmhZ/XoZya7Wbw9ySfmaMVhpQ" +
		"i6yjBiUCgYEA5pP1dmzF99oyJ1TpZon28t5agbZtjee8NYFH3d7CRjaQvpFcaAht" +
		"V6k76sJnV6OJGR+YMt3VSL5z+MStCejQYmuBIV2Yff5OHGo9svYuumtO476WFMpV" +
		"IDNgSscBkAKZ2K1bx1uDUIIAscm2wWGhI1krQbGpnd5GX/eqJwzaYZcCgYEA2Qad" +
		"cf8+uP1iSR7dvgB/npx8/nd2Kh5/FG1Ojw8eOEiOIVvYr/TKuXBCtfD7SxgXyTg5" +
		"cketX/+cwtZhnfF/Nj4QfgsaPEtbUsQeWq3+wZ3yK25dYYji5VaKt9hTTHDMZBDm" +
		"2uiYLJphKL8/O2t55hvD5oypj+LdfQafkmQ6A1sCgYEA2DoUTDQBCkbyFXRJVEE4" +
		"9dSSp2V+Iz8Xfk14KZZGNTXiUvNZ5q1QbI5lg+c+pi7T2F2KitkAtDD17Gnzo4FS" +
		"OZl7BOESqcSbV4ZByduXQnZzCpulfmWnooTLXhR1YKkvwMQBd1hYMl1XbWIuIfb9" +
		"93odWIPU/hh9gm1sgSZKfI0CgYEAoWOp8fw+SJMlY/rwKztzrB/zU8jCts+28RnY" +
		"McqsTm6mQ2hp22/O0dgJxANQ3KcuCRUmozVqWNldWgbS0QGl+FBvM0xOVkgzwXjW" +
		"mdiKerFaRbHs/3ssQB/7SHw2dcHsPP3UrzYI9CJZ3lNaUnW6UMSVCf3Qmo+65h7m" +
		"y5Z9DcECgYEAwgsE2r9yh+j1TiEmBnAC4L4YK6VVuzks49Qf93RQTHQwjYke85Tt" +
		"VM0FqZy+OVkhrG3U4iYIhbXeSKGB8SxAxYj7lC4+EYedmwAY9OFo8uqz9Er0sx51" +
		"I4Gy0F17sUJcdCvbgwmoJRZmcHFmLG12CHDp5Anla4PywzE7ZjkK9uA=";

	const string certBag =
		"MIIC/jCCAeagAwIBAgIQF3lfw9Z80YRHXxd/NtPwWTANBgkqhkiG9w0BAQUFADAQ" +
		"MQ4wDAYDVQQDEwVyaWNoYTAgFw0xOTA5MTMxMTM3MzFaGA8yMTE5MDgyMDExMzcz" +
		"MVowEDEOMAwGA1UEAxMFcmljaGEwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK" +
		"AoIBAQDVxROSCmBRpaR+9BQBFmXbI/mfLTGcXufSVCvzedd2Xue0mnK12pRBy44I" +
		"DdYW+o96qWZqwf2GoJ0S7+GtStfsib+mklIllZTpH15E+1NhaEPIsdVOUITcoRyu" +
		"xMuwmVPj8sci8abZZNWsbSCjrodh27mHoHxTtPuCRefla+JeN6zjczNznSnt+LM2" +
		"AOjd3N/i1/+s3yNEMMEce2kCWhRtNOOYrxKmQz4v+fe3Y5VvztOzj5DJUnU2W9ZJ" +
		"Y/x7OlGWK6dLtCAE6KgxWO7RlQ2e9A6OwQFa1KBIs4xxdeflk78w/4qVU8YZlMZN" +
		"5GN52ObIsHIw+GFFa/rmFsuAYgQBAgMBAAGjUjBQMBUGA1UdJQQOMAwGCisGAQQB" +
		"gjcKAwQwLAYDVR0RBCUwI6AhBgorBgEEAYI3FAIDoBMMEXJpY2hhQFRFQy1MQVAw" +
		"MTIAMAkGA1UdEwQCMAAwDQYJKoZIhvcNAQEFBQADggEBADlUzlHeIX9MF3mUcfwP" +
		"ciQUAC8l+WqY1PvCdiXqE8eyPYeUEMZuSPdHst4iQ8LGLbcepBNJmAYN2ag8RhgV" +
		"T/AtxlWR2GmeHoUxQYi5HZkf9jtgovkHoe8bGr1DQ9fYVw6B+wSlc3dSUvtJUi80" +
		"OI43DN5xdjmxmBOiBl+2wNjopoP0cRYAvDIlqtTQsJ2sXvSKLFM9027yKhveGtAQ" +
		"UxkINbsA0Ct0+Pz04HJ3xLNX/4LK6NeL6fdsOriZaSB8UJjh+y2CUlw3MkFPnzEi" +
		"0JFIeOdf57/gtivZrPX+ATETG8OLXKarDLT7LWHcck4VGeWte89T82MMbV7G2941" +
		"fVE=";

	public static void Main()
	{
		// Define a byte array.
		byte[] bytes = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
		Console.WriteLine("The byte array: ");
		Console.WriteLine("   {0}\n", BitConverter.ToString(bytes));

		// Convert the array to a base 64 string.
		string s = Convert.ToBase64String(bytes);
		Console.WriteLine("The base 64 string:\n   {0}\n", s);

		// Restore the byte array.
		byte[] newBytes = Convert.FromBase64String(certBag);
		Console.WriteLine("The restored byte array: ");
		Console.WriteLine("   {0}\n", BitConverter.ToString(newBytes));
	}
}
// The example displays the following output:
//     The byte array:
//        02-04-06-08-0A-0C-0E-10-12-14
//
//     The base 64 string:
//        AgQGCAoMDhASFA==
//
//     The restored byte array:
//        02-04-06-08-0A-0C-0E-10-12-14
