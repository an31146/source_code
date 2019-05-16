// #!/usr/bin/mcs
#define TEST_FILE_EXISTS

using System;
using System.IO;
using System.Text;
using System.Diagnostics;
using System.Security.Cryptography;


#pragma warning disable CS0219,CS0162,CS0168
// CS0219: The variable 'xxx' is assigned but its value is never used.
// CS0162: Unreachable code detected
class MyStream
{
    private const string FILE_NAME = "Test.data";
    private const bool bDeleteFile = false;
    static private long REPETITIONS = 1048576;
    static private int READ_BLOCK_SIZE = 4096;
    static private long DIVISOR = 1;

    
    static char[] DecryptStringFromBytes(byte[] cipherText, byte[] Key, byte[] IV)
    {
        // Check arguments.
        if (cipherText == null || cipherText.Length <= 0)
            throw new ArgumentNullException("cipherText");
        if (Key == null || Key.Length <= 0)
            throw new ArgumentNullException("Key");
        if (IV == null || IV.Length <= 0)
            throw new ArgumentNullException("IV");

        // Declare the string used to hold
        // the decrypted text.
        string plaintext = null;

        // Create an RijndaelManaged object
        // with the specified key and IV.
        using (TripleDES tripleDESalg = TripleDESCng.Create())
        {
            tripleDESalg.Key = Key;
            tripleDESalg.IV = IV;

            // Create a decryptor to perform the stream transform.
            ICryptoTransform decryptor = tripleDESalg.CreateDecryptor(tripleDESalg.Key, tripleDESalg.IV);

            // Create the streams used for decryption.
            using (MemoryStream msDecrypt = new MemoryStream(cipherText))
            {
                using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read))
                {
                    using (StreamReader srDecrypt = new StreamReader(csDecrypt))
                    {
                        // Read the decrypted bytes from the decrypting stream
                        // and place them in a string.
                        plaintext = srDecrypt.ReadToEnd();
                    }
                }
            }

        }

        return plaintext.ToCharArray();
    }

    private static byte[] EncryptStringToBytes(string plainText, byte[] Key, byte[] IV)
    {
        // Check arguments.
        if (plainText == null || plainText.Length <= 0)
            throw new ArgumentNullException("plainText");
        if (Key == null || Key.Length <= 0)
            throw new ArgumentNullException("Key");
        if (IV == null || IV.Length <= 0)
            throw new ArgumentNullException("IV");
        byte[] encrypted;
        // Create an Rijndael object
        // with the specified key and IV.
        //using (Rijndael rijAlg = Rijndael.Create())
        using (TripleDES TripleDESalg = TripleDESCng.Create())
        {
            TripleDESalg.Key = Key;
            TripleDESalg.IV = IV;

            // Create an encryptor to perform the stream transform.
            ICryptoTransform encryptor = TripleDESalg.CreateEncryptor(TripleDESalg.Key, TripleDESalg.IV);

            // Create the streams used for encryption.
            using (MemoryStream msEncrypt = new MemoryStream())
            {
                using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write))
                {
                    using (StreamWriter swEncrypt = new StreamWriter(csEncrypt))
                    {
                        //Write all data to the stream.
                        swEncrypt.Write(plainText);
                    }
                    encrypted = msEncrypt.ToArray();
                }
            }
        }

        // Return the encrypted bytes from the memory stream.
        return encrypted;
    }

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

        // Need to retain the key and IV        
        TripleDESCryptoServiceProvider tripleDESalg = new TripleDESCryptoServiceProvider();

        // Writing
        try
        {
            FileStream fs = new FileStream(FILE_NAME, FileMode.CreateNew);
            Stopwatch sw = new Stopwatch();
            Encoding ascii = Encoding.ASCII;
            Encoding utf8 = Encoding.UTF8;

            // Write Byte-order mark preamble to file
            //Byte[] bom = utf8.GetPreamble();
            //fs.Write(bom, 0, bom.Length);

            // Create the writer for data.
            BinaryWriter w = new BinaryWriter(fs);
            string strAscii = "";

            // Create a string of ASCII characters
            for (int i = 32; i < 255; i++)
            {
                strAscii += (char)i;
            }
            // Multiply and concatenate - becomes 16 times the original length
            for (int i = 0; i < 8; i++)
            {
                strAscii += strAscii;
                DIVISOR *= 2;
            }
            
            Byte[] encBytes;
            {
                encBytes = EncryptStringToBytes(strAscii, tripleDESalg.Key, tripleDESalg.IV);
            }

            Console.WriteLine("Writing {0} bytes to file...", REPETITIONS / DIVISOR * utf8.GetBytes(strAscii).Length); // + bom.Length);
            //Console.WriteLine("DIVISOR = {0}", DIVISOR);
            
            sw.Start();
            // Write the string for COUNT times to Test.data
            for (int j = 0; j < REPETITIONS / DIVISOR; j++)
            {
                //w.Write(utf8.GetBytes(strAscii));
                w.Write(encBytes);
                Console.Write("{0:F1} %\r", (float)j / (REPETITIONS / DIVISOR) * 100.0f);
            }
            sw.Stop();
            Console.WriteLine("\nWrote {0} bytes to file...", fs.Length);
            
            w.Close();
            fs.Close();
            
            Console.WriteLine("\nElapsed time: {0} ms\n", sw.ElapsedMilliseconds);
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
            byte[] byteArray =  new byte[fs.Length];

            Console.WriteLine("Reading from file in {0}-byte chunks...", READ_BLOCK_SIZE);

            sw.Restart();
            while (fs.Position < fs.Length)
            {
                // Read data from Test.data.
                //for (int i = 32; i < 255; i++)
                //{
                    //Console.Write(r.ReadChar());
                    //r.ReadChar();			// Elapsed time: 115080 ms
                    byteArray = r.ReadBytes((int)fs.Length);

                    Console.WriteLine("byteArray.Length: {0}", byteArray.Length);
                    //foreach (byte b in byteArray)
                    //    Console.Write("{0:X2} ", b);
                    
                    //Console.WriteLine("{0:X2}", tripleDESalg.Key.Length);
                    //tripleDESalg.Key[0] = 0xee;
                    //*/Console.WriteLine("{0:X2}", tripleDESalg.Key[0]);
                    charArray = DecryptStringFromBytes(byteArray, tripleDESalg.Key, tripleDESalg.IV);
                    
                    Console.Write("{0:F1} %\r", (float)fs.Position / fs.Length * 100.0f);
                //}
            }
            sw.Stop();
            
            //Console.WriteLine("{0}", new string(charArray));
            Console.WriteLine("\nElapsed time: {0} ms\n", sw.ElapsedMilliseconds);

            r.Close();
            fs.Close();

            if (bDeleteFile)
            {
                Console.WriteLine("Deleting {0}...", FILE_NAME);
                File.Delete(FILE_NAME);
            }
        }
        catch (IOException ex)
        {
            Console.WriteLine("Exception: {0}", ex.Message);
            return;
        }
            
    }   // void Main
}   // class MyStream
