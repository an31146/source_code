// #!/usr/bin/mcs
using System;
using System.IO;

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
        FileStream fs = new FileStream(FILE_NAME, FileMode.CreateNew);
        // Create the writer for data.
        BinaryWriter w = new BinaryWriter(fs);

        for (int j = 0; j < 16777216; j++)
        {
            // Write data to Test.data.
            for (int i = 32; i < 255; i++)
            {
                w.Write((char)i);
            }
        }
        w.Close();
        fs.Close();

        // Create the reader for data.
        fs = new FileStream(FILE_NAME, FileMode.Open, FileAccess.Read);
        BinaryReader r = new BinaryReader(fs);

        while (fs.Position < fs.Length)
        {
            // Read data from Test.data.
            for (int i = 32; i < 255; i++)
            {
                Console.Write(r.ReadChar());
            }
        }
        Console.WriteLine();
        r.Close();
        fs.Close();

        File.Delete("Test.data");
    }
}
