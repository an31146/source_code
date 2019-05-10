// #!/usr/bin/mcs
#define TEST_FILE_EXISTS 0

using System;
using System.IO;
using System.Text;
using System.Diagnostics;


#pragma warning disable CS0219,CS0162
// CS0219: The variable 'xxx' is assigned but its value is never used.
// CS0162: Unreachable code detected
class MyStream
{
    private const string FILE_NAME = "Test.data";
    private const bool bDeleteFile = false;

    public static void Main(String[] args)
    {
        // Create the new, empty data file.
#if TEST_FILE_EXISTS
        if (File.Exists(FILE_NAME))
        {
            Console.WriteLine("{0} already exists!", FILE_NAME);
            return;
        }
#endif
        int REPETITIONS = 1048576;
        int READ_BLOCK_SIZE = 1024;
        if (args.Length > 0)
            REPETITIONS = Int32.Parse(args[0]);
        if (args.Length == 2)
            READ_BLOCK_SIZE = Int32.Parse(args[1]);

        try
        {
            FileStream fs = new FileStream(FILE_NAME, FileMode.CreateNew);
            Stopwatch sw = new Stopwatch();
            Encoding ascii = Encoding.ASCII;
            Encoding utf8 = Encoding.UTF8;

            // Write Byte-order mark preamble to file
            Byte[] bom = utf8.GetPreamble();
            fs.Write(bom, 0, bom.Length);

            // Create the writer for data.
            BinaryWriter w = new BinaryWriter(fs);
            string strAscii = "";

            // Create a string of ASCII characters
            for (int i = 32; i < 255; i++)
            {
                strAscii += (char)i;
            }
            // Multiply and concatenate - becomes 8 times the original length
            strAscii += strAscii;
            strAscii += strAscii;
            strAscii += strAscii;

            Console.WriteLine("Writing {0} bytes to file...", REPETITIONS / 8 * utf8.GetBytes(strAscii).Length + bom.Length);
            
            sw.Start();
            // Write the string for COUNT times to Test.data
            for (int j = 0; j < REPETITIONS / 8; j++)
            {
                w.Write(utf8.GetBytes(strAscii));
                Console.Write("{0:F1} %\r", (float)j / (REPETITIONS / 800.0f));
            }
            sw.Stop();
            Console.WriteLine("Wrote {0} bytes to file...", fs.Length);
            
            w.Close();
            fs.Close();
            
            Console.WriteLine("\nElapsed time: {0} ms\n", sw.ElapsedMilliseconds);
        }
        catch (IOException ex)
        {
            // error handling
            Console.WriteLine("Exception: {0}", ex.Message);
            throw ex;
        }

        // Create the reader for data.
        fs = new FileStream(FILE_NAME, FileMode.Open, FileAccess.Read);

        BinaryReader r = new BinaryReader(fs);
        char[] charArray = new char[READ_BLOCK_SIZE];

        Console.WriteLine("Reading from file...");

        sw.Restart();
        while (fs.Position < fs.Length)
        {
            // Read data from Test.data.
            for (int i = 32; i < 255; i++)
            {
                //Console.Write(r.ReadChar());
                //r.ReadChar();			// Elapsed time: 115080 ms
                charArray = r.ReadChars(READ_BLOCK_SIZE);
                Console.Write("{0:F1} %\r", (float)fs.Position / fs.Length * 100.0f);
            }
        }
        sw.Stop();

        Console.WriteLine("\nElapsed time: {0} ms\n", sw.ElapsedMilliseconds);

        r.Close();
        fs.Close();

        if (bDeleteFile)
            File.Delete("Test.data");
            
    }   // void Main
}   // class MyStream
