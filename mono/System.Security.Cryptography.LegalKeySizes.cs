using System;
using System.Security.Cryptography;

using static System.Console;

class Program
{
    static void Main(string[] args)
    {
        AesManaged aes = new AesManaged();
        WriteLine("AesManaged: ");
        KeySizes[] ks = aes.LegalKeySizes;
        foreach (KeySizes k in ks)
        {
            WriteLine("\tLegal min key size = " + k.MinSize);
            WriteLine("\tLegal max key size = " + k.MaxSize);
            WriteLine();
        }
        ks = aes.LegalBlockSizes;
        foreach (KeySizes k in ks)
        {
            WriteLine("\tLegal min block size = " + k.MinSize);
            WriteLine("\tLegal max block size = " + k.MaxSize);
            WriteLine();
        }

        //------------------------------------------------------------
        DESCryptoServiceProvider des = new DESCryptoServiceProvider();
        WriteLine("DESCryptoServiceProvider: ");
        ks = des.LegalKeySizes;
        foreach (KeySizes k in ks)
        {
            WriteLine("\tLegal min key size = " + k.MinSize);
            WriteLine("\tLegal max key size = " + k.MaxSize);
            WriteLine();
        }
        ks = des.LegalBlockSizes;
        foreach (KeySizes k in ks)
        {
            WriteLine("\tLegal min block size = " + k.MinSize);
            WriteLine("\tLegal max block size = " + k.MaxSize);
            WriteLine();
        }

        //------------------------------------------------------------
        RC2CryptoServiceProvider rc2 = new RC2CryptoServiceProvider();
        WriteLine("RC2CryptoServiceProvider: ");
        ks = rc2.LegalKeySizes;
        foreach (KeySizes k in ks)
        {
            WriteLine("\tLegal min key size = " + k.MinSize);
            WriteLine("\tLegal max key size = " + k.MaxSize);
            WriteLine();
        }
        ks = rc2.LegalBlockSizes;
        foreach (KeySizes k in ks)
        {
            WriteLine("\tLegal min block size = " + k.MinSize);
            WriteLine("\tLegal max block size = " + k.MaxSize);
            WriteLine();
        }

        //------------------------------------------------------------
        RijndaelManaged rij = new RijndaelManaged();
        WriteLine("RijndaelManaged: ");
        ks = rij.LegalKeySizes;
        foreach (KeySizes k in ks)
        {
            WriteLine("\tLegal min key size = " + k.MinSize);
            WriteLine("\tLegal max key size = " + k.MaxSize);
            WriteLine();
        }
        ks = rij.LegalBlockSizes;
        foreach (KeySizes k in ks)
        {
            WriteLine("\tLegal min block size = " + k.MinSize);
            WriteLine("\tLegal max block size = " + k.MaxSize);
            WriteLine();
        }

        //------------------------------------------------------------
        TripleDESCryptoServiceProvider tsp = new TripleDESCryptoServiceProvider();
        WriteLine("TripleDESCryptoServiceProvider: ");
        ks = tsp.LegalKeySizes;
        foreach (KeySizes k in ks)
        {
            WriteLine("\tLegal min key size = " + k.MinSize);
            WriteLine("\tLegal max key size = " + k.MaxSize);
            WriteLine();
        }
        ks = tsp.LegalBlockSizes;
        foreach (KeySizes k in ks)
        {
            WriteLine("\tLegal min block size = " + k.MinSize);
            WriteLine("\tLegal max block size = " + k.MaxSize);
            WriteLine();
        }
        
    }
}

//This sample produces the following output:
//AesManaged
//        Legal min key size = 128
//        Legal max key size = 256
//        Legal min block size = 128
//        Legal max block size = 128
//DESCryptoServiceProvider
//        Legal min key size = 64
//        Legal max key size = 64
//        Legal min block size = 64
//        Legal max block size = 64
//RC2CryptoServiceProvider
//        Legal min key size = 40
//        Legal max key size = 128
//        Legal min block size = 64
//        Legal max block size = 64
//RijndaelManaged
//        Legal min key size = 128
//        Legal max key size = 256
//        Legal min block size = 128
//        Legal max block size = 256
//TripleDESCryptoServiceProvider
//        Legal min key size = 128
//        Legal max key size = 192
//        Legal min block size = 64
//        Legal max block size = 64
