﻿using System;
using System.IO;
using System.Security.Cryptography;

using static System.Console;

class AesExample
{
    static void HexDump(byte[] bytes)
    {
        int byte_count = 0;
        string char_string = "";
        foreach (byte b in bytes)
        {
            if (byte_count % 16 == 0)
                Console.Write("{0:X8}:  ", byte_count);
            Write("{0:X2} ", b);
            if (b > 31 && b < 127)
                char_string += (char)b;
            else
                char_string += '.';
            if (++byte_count % 8 == 0)
            {
                Write(' ');
                char_string += "  ";
            }
            if (byte_count % 16 == 0)
            {
                WriteLine(char_string);
                char_string = "";
            }
        }
    }

    public static void Main(string[] args)
    {
        string original = "Aes Class\nDefinition\nNamespace:\nSystem.Security.Cryptography\nAssemblies:\nSystem.Security.Cryptography.Algorithms.dll, mscorlib.dll," +
                          "netstandard.dll, System.Core.dll\n" +
                          "Represents the abstract base class from which all implementations of the Advanced Encryption Standard (AES) must inherit.";
        string fileName = "";
        
        if (args.Length == 1)
            fileName = args[0];

        StreamReader sr = null;;
        try
        {
            sr = new StreamReader(fileName);

            // Create a new instance of the Aes
            // class.  This generates a new key and initialization 
            // vector (IV).
            using (Aes myAes = AesCng.Create())
            {
                WriteLine("myAes.Key:\n-------------");
                HexDump(myAes.Key);
                WriteLine("\nmyAes.IV:\n-------------");
                HexDump(myAes.IV);
                WriteLine("\n");
                
                original = "";
                while (!sr.EndOfStream) {
                    original += sr.ReadLine();
                    original += "\n";
                } 

                // Encrypt the string to an array of bytes.
                byte[] encrypted = EncryptStringToBytes_Aes(original, myAes.Key, myAes.IV);
                WriteLine("Encrypted bytes:\n-------------");
                HexDump(encrypted);
                WriteLine();

                // Decrypt the bytes to a string.
                string roundtrip = DecryptStringFromBytes_Aes(encrypted, myAes.Key, myAes.IV);

                //Display the original data and the decrypted data.
                //WriteLine("\nOriginal text\n-------------\n{0}\n", original);
                WriteLine("Decrypted, round Trip:\n----------------------\n{0}\n", roundtrip);
            }
        }
        catch (IOException ex)
        {
            Console.WriteLine("\nCaught exception: {0}", ex.Message);
            return;
        }
        finally
        {
            if (sr != null)
                sr.Close();
        }
    }
    
    static byte[] EncryptStringToBytes_Aes(string plainText, byte[] Key, byte[] IV)
    {
        // Check arguments.
        if (plainText == null || plainText.Length <= 0)
            throw new ArgumentNullException("plainText");
        if (Key == null || Key.Length <= 0)
            throw new ArgumentNullException("Key");
        if (IV == null || IV.Length <= 0)
            throw new ArgumentNullException("IV");
        byte[] encrypted;
        
        // Create an Aes object
        // with the specified key and IV.
        using (Aes aesAlg = Aes.Create())
        {
            aesAlg.Key = Key;
            aesAlg.IV = IV;

            // Create an encryptor to perform the stream transform.
            ICryptoTransform encryptor = aesAlg.CreateEncryptor(aesAlg.Key, aesAlg.IV);

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

    static string DecryptStringFromBytes_Aes(byte[] cipherText, byte[] Key, byte[] IV)
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

        // Create an Aes object
        // with the specified key and IV.
        using (Aes aesAlg = Aes.Create())
        {
            aesAlg.Key = Key;
            aesAlg.IV = IV;

            // Create a decryptor to perform the stream transform.
            ICryptoTransform decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);

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
}
