/*
 * #!/usr/bin/mcs
 *
 * SharedInterestsGraphTheory.cs
 * "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\Roslyn\csc.exe" /target:exe /out:SharedInterestsGraphTheory.exe SharedInterestsGraphTheory.cs
 */
#define TEST_FILE_EXISTS

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
    static private long REPETITIONS = 1048576;
    static private int READ_BLOCK_SIZE = 4096;
    static private long DIVISOR = 1;

    public static void Main(String[] args)
    {
        // Create the new, empty data file.
#if TEST_FILE_EXISTS
        if (File.Exists(FILE_NAME))
        {
            Console.WriteLine("{0} already exists!  Deleting it...\n", FILE_NAME);
            
            try
            {
                File.Delete(FILE_NAME);
            }
            catch (UnauthorizedAccessException ex)
            {
                Console.WriteLine("Exception: {0}", ex.Message);
                return;
            }
        }
#endif
        
        try
        {
            if (args.Length > 0)
                REPETITIONS = Int32.Parse(args[0]);
            if (args.Length == 2)
                READ_BLOCK_SIZE = Int32.Parse(args[1]);
        }
        catch (FormatException ex)
        {
            Console.WriteLine("Exception: {0}", ex.Message);
            return;
        }
        
        // Writing
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
            
            // Multiply and concatenate - becomes 256 times the original length
            for (int i = 0; i < 8; i++)
            {
                strAscii += strAscii;
                DIVISOR *= 2;
            }

            Console.WriteLine("Writing {0} bytes to file...", REPETITIONS / DIVISOR * utf8.GetBytes(strAscii).Length + bom.Length);
            //Console.WriteLine("DIVISOR = {0}", DIVISOR);
            
            sw.Start();
            // Write the string for COUNT times to Test.data
            for (int j = 0; j < REPETITIONS / DIVISOR; j++)
            {
                w.Write(utf8.GetBytes(strAscii));
                Console.Write("{0:F1} %\r", (float)j / (REPETITIONS / DIVISOR) * 100.0f);
            }
            sw.Stop();
            
            Console.WriteLine("\nWrote {0} bytes to file...", fs.Length);
            Console.WriteLine("\nElapsed time: {0} ms", sw.ElapsedMilliseconds);

            double writeSpeed = (double)fs.Length / 1048576.0d / sw.Elapsed.TotalSeconds;
            Console.WriteLine("Approximate write speed: {0:F1} MB/s\n", writeSpeed);

            w.Close();
            fs.Close();
        }
        catch (IOException ex)
        {
            // error handling
            Console.WriteLine("Exception: {0}", ex.Message);
            return;
        }

        // Reading
        try
        {
            // Create the reader for data.  
            FileStream fs = new FileStream(FILE_NAME, FileMode.Open, FileAccess.Read);
            Stopwatch sw = new Stopwatch();

            BinaryReader r = new BinaryReader(fs);
            char[] charArray = new char[READ_BLOCK_SIZE];

            Console.WriteLine("Reading from file in {0}-byte chunks...", READ_BLOCK_SIZE);

            sw.Restart();
            while (fs.Position < fs.Length)
            {
                // Read data from Test.data.
                //for (int i = 32; i < 255; i++)
                //{
                    //Console.Write(r.ReadChar());
                    //r.ReadChar();			// Elapsed time: 115080 ms
                    charArray = r.ReadChars(READ_BLOCK_SIZE);
                    Console.Write("{0:F1} %\r", (float)fs.Position / fs.Length * 100.0f);
                //}
            }
            sw.Stop();

            Console.WriteLine("\nElapsed time: {0} ms", sw.ElapsedMilliseconds);

            double readSpeed = (double)fs.Length / 1048576.0d / sw.Elapsed.TotalSeconds;
            Console.WriteLine("Approximate read speed: {0:F1} MB/s\n", readSpeed);

            r.Close();
            fs.Close();

            if (bDeleteFile)
                File.Delete(FILE_NAME);
        }
        catch (IOException ex)
        {
            Console.WriteLine("Exception: {0}", ex.Message);
            return;
        }
            
    }   // void Main
}   // class MyStream
