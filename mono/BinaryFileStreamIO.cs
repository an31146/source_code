// #!/usr/bin/mcs
using System;
using System.Diagnostics;
using System.IO;

#pragma warning disable CS0219
class MyStream
{
    private const string FILE_NAME = "Test.data";

    public static void Main(String[] args)
    {
        // Create the new, empty data file.
        if (File.Exists(FILE_NAME))
        {
            Console.WriteLine("{0} already exists!", FILE_NAME);
            return;
        }

        int COUNT = 1048576;
        if (args.Length == 1)
            COUNT = Int32.Parse(args[0]);

        FileStream fs = new FileStream(FILE_NAME, FileMode.CreateNew);
        Stopwatch sw = new Stopwatch();

        // Create the writer for data.
        BinaryWriter w = new BinaryWriter(fs);
        string strAscii = "";

        // Create a string of ASCII characters
        for (int i = 32; i < 255; i++)
        {
            strAscii += (char)i;
        }
        // Multiply and concatenate
        strAscii += strAscii;
        strAscii += strAscii;
        
        Console.WriteLine("Writing {0} bytes to file...", strAscii.Length * COUNT / 4);
        sw.Start();
        // Write the string for COUNT times to Test.data
        for (int j = 0; j < COUNT / 4; j++)
        {
            w.Write(strAscii.ToCharArray());
            Console.Write("{0:F1} %\r", (float)j / (COUNT / 400.0f));
        }
        w.Close();
        fs.Close();

        sw.Stop();
        
        Console.WriteLine("\nElapsed time: {0} ms\n", sw.ElapsedMilliseconds);

        // Create the reader for data.
        fs = new FileStream(FILE_NAME, FileMode.Open, FileAccess.Read);

        BinaryReader r = new BinaryReader(fs);
        char[] charArray = new char[1024];

        Console.WriteLine("Reading from file...");

        sw.Restart();
        while (fs.Position < fs.Length)
        {
            // Read data from Test.data.
            for (int i = 32; i < 255; i++)
            {
                //Console.Write(r.ReadChar());
                //r.ReadChar();			// Elapsed time: 115080 ms
                charArray = r.ReadChars(1024);
                Console.Write("{0:F1} %\r", (float)fs.Position / fs.Length * 100.0f);
            }
        }
        sw.Stop();

        Console.WriteLine("\nElapsed time: {0} ms\n", sw.ElapsedMilliseconds);

        r.Close();
        fs.Close();

        File.Delete("Test.data");
    }
}
#pragma warning restore CS0219
