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

    static void ExceptionAndStackTrace(Exception ex)
    {
        Console.WriteLine("Exception: {0}\n{1}", ex.Message, ex.StackTrace);
        Environment.Exit(ex.HResult);           // Lazy-coding - should return to perform finally section
    }
    
    static string DecryptStringFromBytes(byte[] cipherText, byte[] Key, byte[] IV)
    {
        // Check arguments.
        if (cipherText == null || cipherText.Length <= 0)
            throw new ArgumentNullException("cipherText is null");
        if (Key == null || Key.Length <= 0)
            throw new ArgumentNullException("Key is null");
        if (IV == null || IV.Length <= 0)
            throw new ArgumentNullException("IV is null");

        // Declare the string used to hold
        // the decrypted text.
        string plaintext = null;

        // Create an TripleDES object
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
        return plaintext;
    }

    private static byte[] EncryptStringToBytes(string plainText, byte[] Key, byte[] IV)
    {
        // Check arguments.
        if (plainText == null || plainText.Length <= 0)
            throw new ArgumentNullException("plainText is null");
        if (Key == null || Key.Length <= 0)
            throw new ArgumentNullException("Key is null");
        if (IV == null || IV.Length <= 0)
            throw new ArgumentNullException("IV is null");

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
                ExceptionAndStackTrace(ex);
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
            ExceptionAndStackTrace(ex);
        }

        // Need to retain the key and IV        
        TripleDESCryptoServiceProvider tripleDESalg = new TripleDESCryptoServiceProvider();

        Console.Write("tripleDESalg.Key: ");
        foreach (byte b in tripleDESalg.Key)
            Console.Write("{0:X2} ", b);

        Console.Write("\ntripleDESalg.IV:  ");
        foreach (byte b in tripleDESalg.IV)
            Console.Write("{0:X2} ", b);
            
        Console.WriteLine("\n");
                    /*
                    Console.WriteLine("tripleDESalg.Key.Length {0} bytes", tripleDESalg.Key.Length);
                    foreach (byte b in tripleDESalg.Key)
                        Console.Write("{0:X2} ", b);
                    tripleDESalg.Key[0] = 0x55;
                    Console.WriteLine("{0:X2}", tripleDESalg.Key[0]);
                    */

        // Writing
        try
        {
            FileStream fs = new FileStream(FILE_NAME, FileMode.CreateNew);
            Stopwatch sw = new Stopwatch();
            Encoding ascii = Encoding.ASCII;
            Encoding utf8 = Encoding.UTF8;

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

            Console.WriteLine("Writing {0} bytes to file...", REPETITIONS / DIVISOR * encBytes.Length); // + bom.Length);
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
            ExceptionAndStackTrace(ex);
        }

        // Reading
        try
        {
            //using (MemoryStream memStream = new MemoryStream())
            Stopwatch sw = new Stopwatch();
            string plain_text = "";
            
            try
            {
            // Create the reader for data.  
                using (FileStream fs = new FileStream(FILE_NAME, FileMode.Open, FileAccess.Read))
                {
                    byte[] byteArray =  new byte[fs.Length];
                    byte[] buffer = new byte[READ_BLOCK_SIZE];
                    int offset = 0;

                    Console.WriteLine("Reading from file in {0}-byte chunks...", READ_BLOCK_SIZE);
                    sw.Restart();
                    //fs.Seek(0, SeekOrigin.Begin);
                    using (BinaryReader r = new BinaryReader(fs))
                    {
                        while (fs.Position < fs.Length)
                        {
                            buffer = r.ReadBytes(READ_BLOCK_SIZE);
                            //Console.WriteLine("offset = {1}\nbuffer.Length = {0} bytes, byteArray.Length = {2}", buffer.Length, offset, byteArray.Length);
                            Buffer.BlockCopy( buffer, 0, byteArray, offset, buffer.Length );
                            //memStream.Write(buffer, 0, buffer.Length);
                            offset += buffer.Length;
                            
                            Console.Write("{0:F1} %\r", (float)offset / fs.Length * 100.0f);
                        }
                        //}
                        sw.Stop();

                        Console.WriteLine("\nRead {0} bytes from {1}", fs.Length, FILE_NAME);
                        //Console.WriteLine("\nmemStream.Length: {0} bytes", memStream.Length);
                        Console.WriteLine("\nbyteArray.Length: {0} bytes", byteArray.Length);
                        Console.WriteLine("\nElapsed time: {0} ms\n", sw.ElapsedMilliseconds);
                    } // the problem with using using ...

                    sw.Restart();
                    Console.WriteLine("Decrypting... ");
                    
                    {
                        byte[] keyBytes = new byte[tripleDESalg.Key.Length];
                        for (int i = 0; i < keyBytes.Length; i++)
                            keyBytes[i] = (byte)(tripleDESalg.Key[i] ^ 0xff);
                        //Buffer.BlockCopy(keyBytes, 0, tripleDESalg.Key, 0, keyBytes.Length);
                            
                        Console.WriteLine();
                        plain_text = DecryptStringFromBytes(byteArray, tripleDESalg.Key, tripleDESalg.IV);
                    }
                        
                    sw.Stop();
                    Console.WriteLine("\nElapsed time: {0} ms\n", sw.ElapsedMilliseconds);

                } // using

                // Write the string to FILE_NAME.
                using (FileStream fs = new FileStream(FILE_NAME, FileMode.Create))
                {
                    //Write Byte-order mark preamble to file
                    UTF8Encoding utf8 = new UTF8Encoding(true);
                    Byte[] bom = utf8.GetPreamble();
                    fs.Write(bom, 0, bom.Length);
                }
                using (FileStream fs = new FileStream(FILE_NAME, FileMode.Append))
                {
                    using (BinaryWriter binWriter = new BinaryWriter(fs))
                    {
                        UTF8Encoding utf8 = new UTF8Encoding(true);
                        binWriter.Write(utf8.GetBytes(plain_text));
                        Console.WriteLine("Wrote {0} bytes of plain-text to {1}", fs.Length, FILE_NAME);
                    }
                }
                
            } // try
            catch (ObjectDisposedException ex)
            {
                ExceptionAndStackTrace(ex);
            }
            catch (CryptographicException ex)
            {
                ExceptionAndStackTrace(ex);
            }

            if (bDeleteFile)
            {
                Console.WriteLine("Deleting {0}...", FILE_NAME);
                File.Delete(FILE_NAME);
            }
        }
        catch (IOException ex)
        {
            ExceptionAndStackTrace(ex);
        }
            
    }   // void Main
}   // class MyStream
