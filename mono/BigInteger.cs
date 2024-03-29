//************************************************************************************
// BigInteger Class Version 1.03
//
// Copyright (c) 2002 Chew Keong TAN
// All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, provided that the above
// copyright notice(s) and this permission notice appear in all copies of
// the Software and that both the above copyright notice(s) and this
// permission notice appear in supporting documentation.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
// OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
// INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
// FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
// NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
// WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
//
// Disclaimer
// ----------
// Although reasonable care has been taken to ensure the correctness of this
// implementation, this code should never be used in any application without
// proper verification and testing.  I disclaim all liability and responsibility
// to any person or entity with respect to any loss or damage caused, or alleged
// to be caused, directly or indirectly, by the use of this BigInteger class.
//
// Comments, bugs and suggestions to
// (http://www.codeproject.com/csharp/biginteger.asp)
//
//
// Overloaded Operators +, -, *, /, %, >>, <<, ==, !=, >, <, >=, <=, &, |, ^, ++, --, ~
//
// Features
// --------
// 1) Arithmetic operations involving large signed integers (2's complement).
// 2) Primality test using Fermat little theorm, Rabin Miller's method,
//    Solovay Strassen's method and Lucas strong pseudoprime.
// 3) Modulo exponential with Barrett's reduction.
// 4) Inverse modulo.
// 5) Pseudo prime generation.
// 6) Co-prime generation.
//
//
// Known Problem
// -------------
// This pseudoprime passes my implementation of
// primality test but failed in JDK's isProbablePrime test.
//
//       byte[] pseudoPrime1 = { (byte)0x00,
//             (byte)0x85, (byte)0x84, (byte)0x64, (byte)0xFD, (byte)0x70, (byte)0x6A,
//             (byte)0x9F, (byte)0xF0, (byte)0x94, (byte)0x0C, (byte)0x3E, (byte)0x2C,
//             (byte)0x74, (byte)0x34, (byte)0x05, (byte)0xC9, (byte)0x55, (byte)0xB3,
//             (byte)0x85, (byte)0x32, (byte)0x98, (byte)0x71, (byte)0xF9, (byte)0x41,
//             (byte)0x21, (byte)0x5F, (byte)0x02, (byte)0x9E, (byte)0xEA, (byte)0x56,
//             (byte)0x8D, (byte)0x8C, (byte)0x44, (byte)0xCC, (byte)0xEE, (byte)0xEE,
//             (byte)0x3D, (byte)0x2C, (byte)0x9D, (byte)0x2C, (byte)0x12, (byte)0x41,
//             (byte)0x1E, (byte)0xF1, (byte)0xC5, (byte)0x32, (byte)0xC3, (byte)0xAA,
//             (byte)0x31, (byte)0x4A, (byte)0x52, (byte)0xD8, (byte)0xE8, (byte)0xAF,
//             (byte)0x42, (byte)0xF4, (byte)0x72, (byte)0xA1, (byte)0x2A, (byte)0x0D,
//             (byte)0x97, (byte)0xB1, (byte)0x31, (byte)0xB3,
//       };
//
//
// Change Log
// ----------
// 1) September 23, 2002 (Version 1.03)
//    - Fixed operator- to give correct data length.
//    - Added Lucas sequence generation.
//    - Added Strong Lucas Primality test.
//    - Added integer square root method.
//    - Added setBit/unsetBit methods.
//    - New isProbablePrime() method which do not require the
//      confident parameter.
//
// 2) August 29, 2002 (Version 1.02)
//    - Fixed bug in the exponentiation of negative numbers.
//    - Faster modular exponentiation using Barrett reduction.
//    - Added getBytes() method.
//    - Fixed bug in ToHexString method.
//    - Added overloading of ^ operator.
//    - Faster computation of Jacobi symbol.
//
// 3) August 19, 2002 (Version 1.01)
//    - Big integer is stored and manipulated as unsigned integers (4 bytes) instead of
//      individual bytes this gives significant performance improvement.
//    - Updated Fermat's Little Theorem test to use a^(p-1) mod p = 1
//    - Added isProbablePrime method.
//    - Updated documentation.
//
// 4) August 9, 2002 (Version 1.0)
//    - Initial Release.
//
//
// References
// [1] D. E. Knuth, "Seminumerical Algorithms", The Art of Computer Programming Vol. 2,
//     3rd Edition, Addison-Wesley, 1998.
//
// [2] K. H. Rosen, "Elementary Number Theory and Its Applications", 3rd Ed,
//     Addison-Wesley, 1993.
//
// [3] B. Schneier, "Applied Cryptography", 2nd Ed, John Wiley & Sons, 1996.
//
// [4] A. Menezes, P. van Oorschot, and S. Vanstone, "Handbook of Applied Cryptography",
//     CRC Press, 1996, www.cacr.math.uwaterloo.ca/hac
//
// [5] A. Bosselaers, R. Govaerts, and J. Vandewalle, "Comparison of Three Modular
//     Reduction Functions," Proc. CRYPTO'93, pp.175-186.
//
// [6] R. Baillie and S. S. Wagstaff Jr, "Lucas Pseudoprimes", Mathematics of Computation,
//     Vol. 35, No. 152, Oct 1980, pp. 1391-1417.
//
// [7] H. C. Williams, "�douard Lucas and Primality Testing", Canadian Mathematical
//     Society Series of Monographs and Advance Texts, vol. 22, John Wiley & Sons, New York,
//     NY, 1998.
//
// [8] P. Ribenboim, "The new book of prime number records", 3rd edition, Springer-Verlag,
//     New York, NY, 1995.
//
// [9] M. Joye and J.-J. Quisquater, "Efficient computation of full Lucas sequences",
//     Electronics Letters, 32(6), 1996, pp 537-538.
//
//************************************************************************************

using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;


public class BigInteger
{
    // maximum length of the BigInteger in uint (4 bytes)
    // change this to suit the required level of precision.

    private const int maxLength = 320;          // 100 is sufficient for 1560-bits, 130 needed for 2048-bits

    // Number of rounds to perform tests
    private const int ROUNDS = 1000;

    // default confidence level to test primes
    private const int confidence = 15;
    
    // primes smaller than 2000 to test the generated prime number

    public static readonly int[] primesBelow2000 = 
    {
           2,    3,    5,    7,   11,   13,   17,   19,   23,   29,   31,   37,   41,   43,   47,  
          53,   59,   61,   67,   71,   73,   79,   83,   89,   97,
         101,  103,  107,  109,  113,  127,  131,  137,  139,  149,  151,  157,  163,  167,  173,  179,  181,  191,  193,  197,  199,
         211,  223,  227,  229,  233,  239,  241,  251,  257,  263,  269,  271,  277,  281,  283,  293,
         307,  311,  313,  317,  331,  337,  347,  349,  353,  359,  367,  373,  379,  383,  389,  397,
         401,  409,  419,  421,  431,  433,  439,  443,  449,  457,  461,  463,  467,  479,  487,  491,  499,
         503,  509,  521,  523,  541,  547,  557,  563,  569,  571,  577,  587,  593,  599,
         601,  607,  613,  617,  619,  631,  641,  643,  647,  653,  659,  661,  673,  677,  683,  691,
         701,  709,  719,  727,  733,  739,  743,  751,  757,  761,  769,  773,  787,  797,
         809,  811,  821,  823,  827,  829,  839,  853,  857,  859,  863,  877,  881,  883,  887,
         907,  911,  919,  929,  937,  941,  947,  953,  967,  971,  977,  983,  991,  997,
        1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097,
        1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193,
        1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297,
        1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399,
        1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499,
        1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597,
        1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699,
        1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789,
        1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889,
        1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999 
    };


    private uint[] data = null;             // stores bytes from the Big Integer
    public int dataLength;                  // number of actual chars used


    //***********************************************************************
    // Constructor (Default value for BigInteger is 0
    //***********************************************************************

    public BigInteger()
    {
        data = new uint[maxLength];
        dataLength = 1;
    }


    //***********************************************************************
    // Constructor (Default value provided by long)
    //***********************************************************************

    public BigInteger(long value)
    {
        data = new uint[maxLength];
        long tempVal = value;

        // copy bytes from long to BigInteger without any assumption of
        // the length of the long datatype

        dataLength = 0;
        while(value != 0 && dataLength < maxLength)
        {
            data[dataLength] = (uint)(value & 0xFFFFFFFF);
            // Console.WriteLine("data[]:  {0}\nvalue: 0x{1:x8}\ndataLength: {2}\n", data[dataLength], value, dataLength);
            // if ((value & 0xFFFFFFFF) == 4294967289)
                // Console.ReadLine();
            value >>= 32;
            dataLength++;
        }

        if(tempVal > 0)         // overflow check for +ve value
        {
            if(value != 0 || (data[maxLength-1] & 0x80000000) != 0)
                throw new OverflowException("Positive overflow in constructor.");
        }
        else if(tempVal < 0)    // underflow check for -ve value
        {
            if(value != -1 || (data[dataLength-1] & 0x80000000) == 0)
                throw new OverflowException("Negative underflow in constructor.");
        }

        if(dataLength == 0)
            dataLength = 1;
    }


    //***********************************************************************
    // Constructor (Default value provided by ulong)
    //***********************************************************************

    public BigInteger(ulong value)
    {
        data = new uint[maxLength];

        // copy bytes from ulong to BigInteger without any assumption of
        // the length of the ulong datatype

        dataLength = 0;
        while(value != 0 && dataLength < maxLength)
        {
            data[dataLength] = (uint)(value & 0xFFFFFFFF);
            value >>= 32;
            dataLength++;
        }

        if(value != 0 || (data[maxLength-1] & 0x80000000) != 0)
            throw new OverflowException("Positive overflow in constructor.");

        if(dataLength == 0)
            dataLength = 1;
    }



    //***********************************************************************
    // Constructor (Default value provided by BigInteger)
    //***********************************************************************

    public BigInteger(BigInteger bi)
    {
        data = new uint[maxLength];

        dataLength = bi.dataLength;
        
        //for(int i = 0; i < dataLength; i++)
        //    data[i] = bi.data[i];
        bi.data.CopyTo(data, 0);
    }


    //***********************************************************************
    // Constructor (Default value provided by a string of digits of the
    //              specified base)
    //
    // Example (base 10)
    // -----------------
    // To initialize "a" with the default value of 1234 in base 10
    //      BigInteger a = new BigInteger("1234", 10)
    //
    // To initialize "a" with the default value of -1234
    //      BigInteger a = new BigInteger("-1234", 10)
    //
    // Example (base 16)
    // -----------------
    // To initialize "a" with the default value of 0x1D4F in base 16
    //      BigInteger a = new BigInteger("1D4F", 16)
    //
    // To initialize "a" with the default value of -0x1D4F
    //      BigInteger a = new BigInteger("-1D4F", 16)
    //
    // Note that string values are specified in the <sign><magnitude>
    // format.
    //
    //***********************************************************************

    public BigInteger(string value, int radix)
    {
        BigInteger multiplier = new BigInteger(1);
        BigInteger result = new BigInteger();
        value = (value.ToUpper()).Trim();
        int limit = 0;

        if(value[0] == '-')
            limit = 1;

        for(int i = value.Length - 1; i >= limit ; i--)
        {
            int posVal = (int)value[i];

            if(posVal >= '0' && posVal <= '9')
                posVal -= '0';
            else if(posVal >= 'A' && posVal <= 'Z')
                posVal = (posVal - 'A') + 10;
            else
                posVal = 9999999;       // arbitrary large


            if(posVal >= radix)
                throw new ArgumentException("Invalid string in constructor.");
            else
            {
                if(value[0] == '-')
                    posVal = -posVal;

                    result = result + (multiplier * posVal);

                    if((i - 1) >= limit)
                        multiplier = multiplier * radix;
            }
        }

        if(value[0] == '-')     // negative values
        {
            if((result.data[maxLength-1] & 0x80000000) == 0)
                throw new OverflowException("Negative underflow in constructor.");
        }
        else    // positive values
        {
            if((result.data[maxLength-1] & 0x80000000) != 0)
                throw new OverflowException("Positive overflow in constructor.");
        }

        data = new uint[maxLength];
        for(int i = 0; i < result.dataLength; i++)
            data[i] = result.data[i];

        dataLength = result.dataLength;
    }


    //***********************************************************************
    // Constructor (Default value provided by an array of bytes)
    //
    // The lowest index of the input byte array (i.e [0]) should contain the
    // most significant byte of the number, and the highest index should
    // contain the least significant byte.
    //
    // E.g.
    // To initialize "a" with the default value of 0x1D4F in base 16
    //      byte[] temp = { 0x1D, 0x4F };
    //      BigInteger a = new BigInteger(temp)
    //
    // Note that this method of initialization does not allow the
    // sign to be specified.
    //
    //***********************************************************************

    public BigInteger(byte[] inData)
    {
        dataLength = inData.Length >> 2;

        int leftOver = inData.Length & 0x3;
        if(leftOver != 0)         // length not multiples of 4
            dataLength++;


        if(dataLength > maxLength)
            throw new OverflowException("Byte overflow in constructor.");

        data = new uint[maxLength];

        for(int i = inData.Length - 1, j = 0; i >= 3; i -= 4, j++)
        {
            data[j] = (uint)((inData[i-3] << 24) + (inData[i-2] << 16) +
                             (inData[i-1] <<  8) + inData[i]);
        }

        if(leftOver == 1)
            data[dataLength-1] = (uint)inData[0];
        else if(leftOver == 2)
            data[dataLength-1] = (uint)((inData[0] << 8) + inData[1]);
        else if(leftOver == 3)
            data[dataLength-1] = (uint)((inData[0] << 16) + (inData[1] << 8) + inData[2]);


        while(dataLength > 1 && data[dataLength-1] == 0)
            dataLength--;

        //Console.WriteLine("Len = " + dataLength);
    }


    //***********************************************************************
    // Constructor (Default value provided by an array of bytes of the
    // specified length.)
    //***********************************************************************

    public BigInteger(byte[] inData, int inLen)
    {
        dataLength = inLen >> 2;

        int leftOver = inLen & 0x3;
        if(leftOver != 0)         // length not multiples of 4
            dataLength++;

        if(dataLength > maxLength || inLen > inData.Length)
            throw new OverflowException("Byte overflow in constructor.");


        data = new uint[maxLength];

        for(int i = inLen - 1, j = 0; i >= 3; i -= 4, j++)
        {
            data[j] = (uint)((inData[i-3] << 24) + (inData[i-2] << 16) +
                             (inData[i-1] <<  8) + inData[i]);
        }

        if(leftOver == 1)
            data[dataLength-1] = (uint)inData[0];
        else if(leftOver == 2)
            data[dataLength-1] = (uint)((inData[0] << 8) + inData[1]);
        else if(leftOver == 3)
            data[dataLength-1] = (uint)((inData[0] << 16) + (inData[1] << 8) + inData[2]);


        if(dataLength == 0)
            dataLength = 1;

        while(dataLength > 1 && data[dataLength-1] == 0)
            dataLength--;

        //Console.WriteLine("Len = " + dataLength);
    }


    //***********************************************************************
    // Constructor (Default value provided by an array of unsigned integers)
    //*********************************************************************

    public BigInteger(uint[] inData)
    {
        dataLength = inData.Length;

        if(dataLength > maxLength)
            throw new OverflowException("Byte overflow in constructor.");

        data = new uint[maxLength];

        for(int i = dataLength - 1, j = 0; i >= 0; i--, j++)
            data[j] = inData[i];

        while(dataLength > 1 && data[dataLength-1] == 0)
            dataLength--;

        //Console.WriteLine("Len = " + dataLength);
    }


    //***********************************************************************
    // Overloading of the typecast operator.
    // For BigInteger bi = 10;
    //***********************************************************************

    public static implicit operator BigInteger(long value)
    {
        return (new BigInteger(value));
    }

    public static implicit operator BigInteger(ulong value)
    {
        return (new BigInteger(value));
    }

    public static implicit operator BigInteger(int value)
    {
        return (new BigInteger((long)value));
    }

    public static implicit operator BigInteger(uint value)
    {
        return (new BigInteger((ulong)value));
    }


    //***********************************************************************
    // Overloading of addition operator
    //***********************************************************************

    public static BigInteger operator +(BigInteger bi1, BigInteger bi2)
    {
        BigInteger result = new BigInteger();

        result.dataLength = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;

        long carry = 0;
        for(int i = 0; i < result.dataLength; i++)
        {
                long sum = (long)bi1.data[i] + (long)bi2.data[i] + carry;
            carry  = sum >> 32;
            result.data[i] = (uint)(sum & 0xFFFFFFFF);
        }

        if(carry != 0 && result.dataLength < maxLength)
        {
            result.data[result.dataLength] = (uint)(carry);
            result.dataLength++;
        }

        while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
            result.dataLength--;


        // overflow check
        int lastPos = maxLength - 1;
        if((bi1.data[lastPos] & 0x80000000) == (bi2.data[lastPos] & 0x80000000) &&
           (result.data[lastPos] & 0x80000000) != (bi1.data[lastPos] & 0x80000000))
        {
            throw new OverflowException("Addition overflow.");
        }

        return result;
    }


    //***********************************************************************
    // Overloading of the unary ++ operator
    //***********************************************************************

    public static BigInteger operator ++(BigInteger bi1)
    {
        BigInteger result = new BigInteger(bi1);

        long val, carry = 1;
        int index = 0;

        while(carry != 0 && index < maxLength)
        {
            val = (long)(result.data[index]);
            val++;

            result.data[index] = (uint)(val & 0xFFFFFFFF);
            carry = val >> 32;

            index++;
        }

        if(index > result.dataLength)
            result.dataLength = index;
        else
        {
            while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
                result.dataLength--;
        }

        // overflow check
        int lastPos = maxLength - 1;

        // overflow if initial value was +ve but ++ caused a sign
        // change to negative.

        if((bi1.data[lastPos] & 0x80000000) == 0 &&
           (result.data[lastPos] & 0x80000000) != (bi1.data[lastPos] & 0x80000000))
        {
            throw new OverflowException("Overflow in increment operator++.");
        }
        return result;
    }


    //***********************************************************************
    // Overloading of subtraction operator
    //***********************************************************************

    public static BigInteger operator -(BigInteger bi1, BigInteger bi2)
    {
        BigInteger result = new BigInteger();

        result.dataLength = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;

        long carryIn = 0;
        for(int i = 0; i < result.dataLength; i++)
        {
            long diff;

            diff = (long)bi1.data[i] - (long)bi2.data[i] - carryIn;
            result.data[i] = (uint)(diff & 0xFFFFFFFF);

            if(diff < 0)
                carryIn = 1;
            else
                carryIn = 0;
        }

        // roll over to negative
        if(carryIn != 0)
        {
            for(int i = result.dataLength; i < maxLength; i++)
                result.data[i] = 0xFFFFFFFF;
            result.dataLength = maxLength;
        }

        // fixed in v1.03 to give correct datalength for a - (-b)
        while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
            result.dataLength--;

        // overflow check

        int lastPos = maxLength - 1;
        if ((bi1.data[lastPos] & 0x80000000)    != (bi2.data[lastPos] & 0x80000000) &&
            (result.data[lastPos] & 0x80000000) != (bi1.data[lastPos] & 0x80000000))
        {
            throw new OverflowException("Subtraction overflow.");
        }

        return result;
    }


    //***********************************************************************
    // Overloading of the unary -- operator
    //***********************************************************************

    public static BigInteger operator --(BigInteger bi1)
    {
            BigInteger result = new BigInteger(bi1);

            long val;
            bool carryIn = true;
            int index = 0;

            while(carryIn && index < maxLength)
            {
                val = (long)(result.data[index]);
                val--;

                result.data[index] = (uint)(val & 0xFFFFFFFF);

                if(val >= 0)
                        carryIn = false;

                index++;
            }

            if(index > result.dataLength)
                result.dataLength = index;

            while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
                result.dataLength--;

            // overflow check
            int lastPos = maxLength - 1;

            // overflow if initial value was -ve but -- caused a sign
            // change to positive.

            if((bi1.data[lastPos] & 0x80000000) != 0 &&
               (result.data[lastPos] & 0x80000000) != (bi1.data[lastPos] & 0x80000000))
            {
                throw new OverflowException("Underflow in decrement operator--.");
            }

            return result;
    }


    //***********************************************************************
    // Overloading of multiplication operator
    //***********************************************************************

    public static BigInteger operator *(BigInteger bi1, BigInteger bi2)
    {
        int lastPos = maxLength-1;
        bool bi1Neg = false, bi2Neg = false;

        // take the absolute value of the inputs
        try
        {
            if((bi1.data[lastPos] & 0x80000000) != 0)     // bi1 negative
            {
                bi1Neg = true;
                bi1 = -bi1;
            }
            if((bi2.data[lastPos] & 0x80000000) != 0)     // bi2 negative
            {
                bi2Neg = true; 
                bi2 = -bi2;
            }
        }
        catch(Exception) {
            throw new ArithmeticException("Negation error.");
        }

        BigInteger result = new BigInteger();

        // multiply the absolute values
        try
        {
            for(int i = 0; i < bi1.dataLength; i++)
            {
                if(bi1.data[i] == 0)
                    continue;

                ulong mcarry = 0;
                for(int j = 0, k = i; j < bi2.dataLength; j++, k++)
                {
                    // k = i + j
                    ulong val = ((ulong)bi1.data[i] * (ulong)bi2.data[j]) +
                                 (ulong)result.data[k] + mcarry;

                    result.data[k] = (uint)(val & 0xFFFFFFFF);
                    mcarry = (val >> 32);
                }

                if(mcarry != 0)
                    result.data[i+bi2.dataLength] = (uint)mcarry;
            }
        }
        catch(Exception)
        {
            throw(new OverflowException("Multiplication overflow."));
        }


        result.dataLength = bi1.dataLength + bi2.dataLength;
        if(result.dataLength > maxLength)
            result.dataLength = maxLength;

        while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
            result.dataLength--;

        // overflow check (result is -ve)
        if((result.data[lastPos] & 0x80000000) != 0)
        {
            if(bi1Neg != bi2Neg && result.data[lastPos] == 0x80000000)    // different sign
            {
                // handle the special case where multiplication produces
                // a max negative number in 2's complement.

                if(result.dataLength == 1)
                    return result;
                else
                {
                    bool isMaxNeg = true;
                    for(int i = 0; i < result.dataLength - 1 && isMaxNeg; i++)
                    {
                        if(result.data[i] != 0)
                                isMaxNeg = false;
                    }

                    if(isMaxNeg)
                        return result;
                }
            }

            throw new OverflowException("Multiplication overflow.");
        }

        // if input has different signs, then result is -ve
        if(bi1Neg != bi2Neg)
            return -result;

        return result;
    }



    //***********************************************************************
    // Overloading of unary << operators
    //***********************************************************************

    public static BigInteger operator <<(BigInteger bi1, int shiftVal)
    {
        BigInteger result = new BigInteger(bi1);
        result.dataLength = shiftLeft(result.data, shiftVal);

        return result;
    }


    // least significant bits at lower part of buffer

    private static int shiftLeft(uint[] buffer, int shiftVal)
    {
        int shiftAmount = 32;
        int bufLen = buffer.Length;

        while(bufLen > 1 && buffer[bufLen-1] == 0)
            bufLen--;

        for(int count = shiftVal; count > 0;)
        {
            if(count < shiftAmount)
                shiftAmount = count;

            //Console.WriteLine("shiftAmount = {0}", shiftAmount);

            ulong carry = 0;
            for(int i = 0; i < bufLen; i++)
            {
                ulong val = ((ulong)buffer[i]) << shiftAmount;
                val |= carry;

                buffer[i] = (uint)(val & 0xFFFFFFFF);
                carry = val >> 32;
            }

            if(carry != 0)
            {
                if(bufLen + 1 <= buffer.Length)
                {
                        buffer[bufLen] = (uint)carry;
                        bufLen++;
                }
            }
            count -= shiftAmount;
        }
        return bufLen;
    }


    //***********************************************************************
    // Overloading of unary >> operators
    //***********************************************************************

    public static BigInteger operator >>(BigInteger bi1, int shiftVal)
    {
        BigInteger result = new BigInteger(bi1);
        result.dataLength = shiftRight(result.data, shiftVal);


        if((bi1.data[maxLength-1] & 0x80000000) != 0) // negative
        {
            for(int i = maxLength - 1; i >= result.dataLength; i--)
                result.data[i] = 0xFFFFFFFF;

            uint mask = 0x80000000;
            for(int i = 0; i < 32; i++)
            {
                if((result.data[result.dataLength-1] & mask) != 0)
                    break;

                result.data[result.dataLength-1] |= mask;
                mask >>= 1;
            }
            result.dataLength = maxLength;
        }

        return result;
    }


    private static int shiftRight(uint[] buffer, int shiftVal)
    {
        int shiftAmount = 32;
        int invShift = 0;
        int bufLen = buffer.Length;

        while(bufLen > 1 && buffer[bufLen-1] == 0)
            bufLen--;

        //Console.WriteLine("bufLen = " + bufLen + " buffer.Length = " + buffer.Length);

        for(int count = shiftVal; count > 0;)
        {
            if(count < shiftAmount)
            {
                shiftAmount = count;
                invShift = 32 - shiftAmount;
            }

            //Console.WriteLine("shiftAmount = {0}", shiftAmount);

            ulong carry = 0;
            for(int i = bufLen - 1; i >= 0; i--)
            {
                ulong val = ((ulong)buffer[i]) >> shiftAmount;
                val |= carry;

                carry = ((ulong)buffer[i]) << invShift;
                buffer[i] = (uint)(val);
            }

            count -= shiftAmount;
        }

        while(bufLen > 1 && buffer[bufLen-1] == 0)
            bufLen--;

        return bufLen;
    }


    //***********************************************************************
    // Overloading of the NOT operator (1's complement)
    //***********************************************************************

    public static BigInteger operator ~(BigInteger bi1)
    {
        BigInteger result = new BigInteger(bi1);

        for(int i = 0; i < maxLength; i++)
            result.data[i] = (uint)(~(bi1.data[i]));

        result.dataLength = maxLength;

        while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
            result.dataLength--;

        return result;
    }


    //***********************************************************************
    // Overloading of the NEGATE operator (2's complement)
    //***********************************************************************

    public static BigInteger operator -(BigInteger bi1)
    {
        // handle neg of zero separately since it'll cause an overflow
        // if we proceed.

        if(bi1.dataLength == 1 && bi1.data[0] == 0)
            return (new BigInteger());

        BigInteger result = new BigInteger(bi1);

        // 1's complement
        for(int i = 0; i < maxLength; i++)
            result.data[i] = (uint)(~(bi1.data[i]));

        // add one to result of 1's complement
        long val, carry = 1;
        int index = 0;

        while(carry != 0 && index < maxLength)
        {
            val = (long)(result.data[index]);
            val++;

            result.data[index] = (uint)(val & 0xFFFFFFFF);
            carry = val >> 32;

            index++;
        }

        if((bi1.data[maxLength-1] & 0x80000000) == (result.data[maxLength-1] & 0x80000000))
            throw new OverflowException("Overflow in negation.");

        result.dataLength = maxLength;

        while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
            result.dataLength--;
        return result;
    }


    //***********************************************************************
    // Overloading of equality operator
    //***********************************************************************

    public static bool operator ==(BigInteger bi1, BigInteger bi2)
    {
        return bi1.Equals(bi2);
    }


    public static bool operator !=(BigInteger bi1, BigInteger bi2)
    {
        return !(bi1.Equals(bi2));
    }


    public override bool Equals(object o)
    {
        BigInteger bi = (BigInteger)o;

        if(this.dataLength != bi.dataLength)
            return false;

        for(int i = 0; i < this.dataLength; i++)
        {
            if(this.data[i] != bi.data[i])
                return false;
        }
        return true;
    }


    public override int GetHashCode()
    {
        return this.ToByteArray().GetHashCode();
    }


    //***********************************************************************
    // Overloading of inequality operator
    //***********************************************************************

    public static bool operator >(BigInteger bi1, BigInteger bi2)
    {
        int pos = maxLength - 1;

        // bi1 is negative, bi2 is positive
        if((bi1.data[pos] & 0x80000000) != 0 && (bi2.data[pos] & 0x80000000) == 0)
            return false;

            // bi1 is positive, bi2 is negative
        else if((bi1.data[pos] & 0x80000000) == 0 && (bi2.data[pos] & 0x80000000) != 0)
            return true;

            // same sign
        int len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
        for(pos = len - 1; pos >= 0 && bi1.data[pos] == bi2.data[pos]; pos--);

        if(pos >= 0)
        {
            if(bi1.data[pos] > bi2.data[pos])
                return true;
            return false;
        }
        return false;
    }


    public static bool operator <(BigInteger bi1, BigInteger bi2)
    {
        int pos = maxLength - 1;

        // bi1 is negative, bi2 is positive
        if((bi1.data[pos] & 0x80000000) != 0 && (bi2.data[pos] & 0x80000000) == 0)
            return true;

            // bi1 is positive, bi2 is negative
        else if((bi1.data[pos] & 0x80000000) == 0 && (bi2.data[pos] & 0x80000000) != 0)
            return false;

        // same sign
        int len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
        for(pos = len - 1; pos >= 0 && bi1.data[pos] == bi2.data[pos]; pos--);

        if(pos >= 0)
        {
            if(bi1.data[pos] < bi2.data[pos])
                return true;
            return false;
        }
        return false;
    }


    public static bool operator >=(BigInteger bi1, BigInteger bi2)
    {
        return (bi1 == bi2 || bi1 > bi2);
    }


    public static bool operator <=(BigInteger bi1, BigInteger bi2)
    {
        return (bi1 == bi2 || bi1 < bi2);
    }


    //***********************************************************************
    // Private function that supports the division of two numbers with
    // a divisor that has more than 1 digit.
    //
    // Algorithm taken from [1]
    //***********************************************************************

    private static void multiByteDivide(BigInteger bi1, BigInteger bi2,
                                        BigInteger outQuotient, BigInteger outRemainder)
    {
        uint[] result = new uint[maxLength];

        int remainderLen = bi1.dataLength + 1;
        uint[] remainder = new uint[remainderLen];

        uint mask = 0x80000000;
        uint val = bi2.data[bi2.dataLength - 1];
        int shift = 0, resultPos = 0;

        while(mask != 0 && (val & mask) == 0)
        {
            shift++; mask >>= 1;
        }

        //Console.WriteLine("shift = {0}", shift);
        //Console.WriteLine("Before bi1 Len = {0}, bi2 Len = {1}", bi1.dataLength, bi2.dataLength);

        for(int i = 0; i < bi1.dataLength; i++)
            remainder[i] = bi1.data[i];
        shiftLeft(remainder, shift);
        bi2 = bi2 << shift;

        /*
        Console.WriteLine("bi1 Len = {0}, bi2 Len = {1}", bi1.dataLength, bi2.dataLength);
        Console.WriteLine("dividend = " + bi1 + "\ndivisor = " + bi2);
        for(int q = remainderLen - 1; q >= 0; q--)
                Console.Write("{0:x2}", remainder[q]);
        Console.WriteLine();
        */

        int j = remainderLen - bi2.dataLength;
        int pos = remainderLen - 1;

        ulong firstDivisorByte = bi2.data[bi2.dataLength-1];
        ulong secondDivisorByte = bi2.data[bi2.dataLength-2];

        int divisorLen = bi2.dataLength + 1;
        uint[] dividendPart = new uint[divisorLen];

        while(j > 0)
        {
            ulong dividend = ((ulong)remainder[pos] << 32) + (ulong)remainder[pos-1];
            //Console.WriteLine("dividend = {0}", dividend);

            ulong q_hat = dividend / firstDivisorByte;
            ulong r_hat = dividend % firstDivisorByte;

            //Console.WriteLine("q_hat = {0:X}, r_hat = {1:X}", q_hat, r_hat);

            bool done = false;
            while(!done)
            {
                done = true;

                if(q_hat == 0x100000000 ||
                   (q_hat * secondDivisorByte) > ((r_hat << 32) + remainder[pos-2]))
                {
                    q_hat--;
                    r_hat += firstDivisorByte;

                    if(r_hat < 0x100000000)
                            done = false;
                }
            }

            for(int h = 0; h < divisorLen; h++)
                dividendPart[h] = remainder[pos-h];

            BigInteger kk = new BigInteger(dividendPart);
            BigInteger ss = bi2 * (long)q_hat;

            //Console.WriteLine("ss before = " + ss);
            while(ss > kk)
            {
                q_hat--;
                ss -= bi2;
                //Console.WriteLine(ss);
            }
            BigInteger yy = kk - ss;

            //Console.WriteLine("ss = " + ss);
            //Console.WriteLine("kk = " + kk);
            //Console.WriteLine("yy = " + yy);

            for(int h = 0; h < divisorLen; h++)
                remainder[pos-h] = yy.data[bi2.dataLength-h];

            /*
            Console.WriteLine("dividend = ");
            for(int q = remainderLen - 1; q >= 0; q--)
                    Console.Write("{0:x2}", remainder[q]);
            Console.WriteLine("\n************ q_hat = {0:X}\n", q_hat);
            */

            result[resultPos++] = (uint)q_hat;

            pos--;
            j--;
        }

        outQuotient.dataLength = resultPos;
        int y = 0;
        for(int x = outQuotient.dataLength - 1; x >= 0; x--, y++)
            outQuotient.data[y] = result[x];
        for(; y < maxLength; y++)
            outQuotient.data[y] = 0;

        while(outQuotient.dataLength > 1 && outQuotient.data[outQuotient.dataLength-1] == 0)
            outQuotient.dataLength--;

        if(outQuotient.dataLength == 0)
            outQuotient.dataLength = 1;

        outRemainder.dataLength = shiftRight(remainder, shift);

        for(y = 0; y < outRemainder.dataLength; y++)
            outRemainder.data[y] = remainder[y];
        for(; y < maxLength; y++)
            outRemainder.data[y] = 0;
    }


    //***********************************************************************
    // Private function that supports the division of two numbers with
    // a divisor that has only 1 digit.
    //***********************************************************************

    private static void singleByteDivide(BigInteger bi1, BigInteger bi2,
                                         BigInteger outQuotient, BigInteger outRemainder)
    {
        uint[] result = new uint[maxLength];
        int resultPos = 0;

        // copy dividend to reminder
        for(int i = 0; i < maxLength; i++)
            outRemainder.data[i] = bi1.data[i];
        outRemainder.dataLength = bi1.dataLength;

        while(outRemainder.dataLength > 1 && outRemainder.data[outRemainder.dataLength-1] == 0)
            outRemainder.dataLength--;

        ulong divisor = (ulong)bi2.data[0];
        int pos = outRemainder.dataLength - 1;
        ulong dividend = (ulong)outRemainder.data[pos];

        //Console.WriteLine("divisor = " + divisor + " dividend = " + dividend);
        //Console.WriteLine("divisor = " + bi2 + "\ndividend = " + bi1);

        if(dividend >= divisor)
        {
            ulong quotient = dividend / divisor;
            result[resultPos++] = (uint)quotient;

            outRemainder.data[pos] = (uint)(dividend % divisor);
        }
        pos--;

        while(pos >= 0)
        {
            //Console.WriteLine(pos);

            dividend = ((ulong)outRemainder.data[pos+1] << 32) + (ulong)outRemainder.data[pos];
            ulong quotient = dividend / divisor;
            result[resultPos++] = (uint)quotient;

            outRemainder.data[pos+1] = 0;
            outRemainder.data[pos--] = (uint)(dividend % divisor);
            //Console.WriteLine(">>>> " + bi1);
        }

        outQuotient.dataLength = resultPos;
        int j = 0;
        for(int i = outQuotient.dataLength - 1; i >= 0; i--, j++)
            outQuotient.data[j] = result[i];
        for(; j < maxLength; j++)
            outQuotient.data[j] = 0;

        while(outQuotient.dataLength > 1 && outQuotient.data[outQuotient.dataLength-1] == 0)
            outQuotient.dataLength--;

        if(outQuotient.dataLength == 0)
            outQuotient.dataLength = 1;

        while(outRemainder.dataLength > 1 && outRemainder.data[outRemainder.dataLength-1] == 0)
            outRemainder.dataLength--;
    }


    //***********************************************************************
    // Overloading of division operator
    //***********************************************************************

    public static BigInteger operator /(BigInteger bi1, BigInteger bi2)
    {
        BigInteger quotient = new BigInteger();
        BigInteger remainder = new BigInteger();

        int lastPos = maxLength-1;
        bool divisorNeg = false, dividendNeg = false;

        if((bi1.data[lastPos] & 0x80000000) != 0)     // bi1 negative
        {
            bi1 = -bi1;
            dividendNeg = true;
        }
        if((bi2.data[lastPos] & 0x80000000) != 0)     // bi2 negative
        {
            bi2 = -bi2;
            divisorNeg = true;
        }

        if(bi1 < bi2)
        {
            return quotient;
        }

        else
        {
            if(bi2.dataLength == 1)
                    singleByteDivide(bi1, bi2, quotient, remainder);
            else
                    multiByteDivide(bi1, bi2, quotient, remainder);

            if(dividendNeg != divisorNeg)
                    return -quotient;

            return quotient;
        }
    }


    //***********************************************************************
    // Overloading of modulus operator
    //***********************************************************************

    public static BigInteger operator %(BigInteger bi1, BigInteger bi2)
    {
        BigInteger quotient = new BigInteger();
        BigInteger remainder = new BigInteger(bi1);

        int lastPos = maxLength-1;
        bool dividendNeg = false;

        if((bi1.data[lastPos] & 0x80000000) != 0)     // bi1 negative
        {
            bi1 = -bi1;
            dividendNeg = true;
        }
        if((bi2.data[lastPos] & 0x80000000) != 0)     // bi2 negative
            bi2 = -bi2;

        if(bi1 < bi2)
        {
            return remainder;
        }

        else
        {
            if(bi2.dataLength == 1)
                singleByteDivide(bi1, bi2, quotient, remainder);
            else
                multiByteDivide(bi1, bi2, quotient, remainder);

            if(dividendNeg)
                return -remainder;

            return remainder;
        }
    }


    //***********************************************************************
    // Overloading of bitwise AND operator
    //***********************************************************************

    public static BigInteger operator &(BigInteger bi1, BigInteger bi2)
    {
        BigInteger result = new BigInteger();

        int len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;

        for(int i = 0; i < len; i++)
        {
            uint sum = (uint)(bi1.data[i] & bi2.data[i]);
            result.data[i] = sum;
        }

        result.dataLength = maxLength;

        while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
            result.dataLength--;

        return result;
    }


    //***********************************************************************
    // Overloading of bitwise OR operator
    //***********************************************************************

    public static BigInteger operator |(BigInteger bi1, BigInteger bi2)
    {
        BigInteger result = new BigInteger();

        int len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;

        for(int i = 0; i < len; i++)
        {
            uint sum = (uint)(bi1.data[i] | bi2.data[i]);
            result.data[i] = sum;
        }

        result.dataLength = maxLength;

        while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
            result.dataLength--;

        return result;
    }


    //***********************************************************************
    // Overloading of bitwise XOR operator
    //***********************************************************************

    public static BigInteger operator ^(BigInteger bi1, BigInteger bi2)
    {
        BigInteger result = new BigInteger();

        int len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;

        for(int i = 0; i < len; i++)
        {
            uint sum = (uint)(bi1.data[i] ^ bi2.data[i]);
            result.data[i] = sum;
        }

        result.dataLength = maxLength;

        while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
            result.dataLength--;

        return result;
    }


    //***********************************************************************
    // Returns max(this, bi)
    //***********************************************************************

    public BigInteger max(BigInteger bi)
    {
        if(this > bi)
            return (new BigInteger(this));
        else
            return (new BigInteger(bi));
    }


    //***********************************************************************
    // Returns min(this, bi)
    //***********************************************************************

    public BigInteger min(BigInteger bi)
    {
        if(this < bi)
            return (new BigInteger(this));
        else
            return (new BigInteger(bi));

    }


    //***********************************************************************
    // Returns the absolute value
    //***********************************************************************

    public BigInteger abs()
    {
        if((this.data[maxLength - 1] & 0x80000000) != 0)
            return (-this);
        else
            return (new BigInteger(this));
    }


    //***********************************************************************
    // Returns a string representing the BigInteger in base 10.
    //***********************************************************************

    public override string ToString()
    {
        return ToString(10);
    }


    //***********************************************************************
    // Returns a string representing the BigInteger in sign-and-magnitude
    // format in the specified radix.
    //
    // Example
    // -------
    // If the value of BigInteger is -255 in base 10, then
    // ToString(16) returns "-FF"
    //
    //***********************************************************************

    public string ToString(int radix)
    {
        if(radix < 2 || radix > 36)
            throw new ArgumentException("Radix must be >= 2 and <= 36");

        string charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        string result = "";

        BigInteger a = this;

        bool negative = false;
        if((a.data[maxLength-1] & 0x80000000) != 0)
        {
            negative = true;
            try
            {
                    a = -a;
            }
            catch(Exception) {}
        }

        BigInteger quotient = new BigInteger();
        BigInteger remainder = new BigInteger();
        BigInteger biRadix = new BigInteger(radix);

        if(a.dataLength == 1 && a.data[0] == 0)
                result = "0";
        else
        {
            while(a.dataLength > 1 || (a.dataLength == 1 && a.data[0] != 0))
            {
                singleByteDivide(a, biRadix, quotient, remainder);

                if(remainder.data[0] < 10)
                    result = remainder.data[0] + result;
                else
                    result = charSet[(int)remainder.data[0] - 10] + result;

                a = quotient;
            }
            if(negative)
                result = "-" + result;
        }

        return result;
    }


    //***********************************************************************
    // Returns a hex string showing the contents of the BigInteger
    //
    // Examples
    // -------
    // 1) If the value of BigInteger is 255 in base 10, then
    //    ToHexString() returns "FF"
    //
    // 2) If the value of BigInteger is -255 in base 10, then
    //    ToHexString() returns ".....FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF01",
    //    which is the 2's complement representation of -255.
    //
    //***********************************************************************

    public string ToHexString()
    {
        string result = data[dataLength - 1].ToString("X");

        for(int i = dataLength - 2; i >= 0; i--)
        {
            result += data[i].ToString("X8");
        }

        return result;
    }


    //***********************************************************************
    // Returns a byte array representing of the contents of the BigInteger
    // by reversing the order returned by calling getBytes()
    // The lowest index contains the LSB.
    // 
    //***********************************************************************

    public byte[] ToByteArray()
    {
        // Stack<byte> reversed_bytes = new Stack<byte>();
        byte[] reversed_bytes = getBytes();
		// Output byte [] structure suitable for BigInteger.cs
		Array.Reverse(reversed_bytes);
        //for (int i = getBytes().Length - 1; i >= 0; i--)
        /*foreach (byte b in getBytes())
        {
            reversed_bytes.Push(b);
            // Console.Write("{0:x2}  ", b);
        }*/
        return reversed_bytes;
    }
    

    //***********************************************************************
    // Modulo Exponentiation
    //***********************************************************************

    public BigInteger modPow(BigInteger exp, BigInteger n)
    {
        if((exp.data[maxLength-1] & 0x80000000) != 0)
            throw new ArgumentException("Positive exponents only.");

        BigInteger resultNum = 1;
        BigInteger tempNum;
        bool thisNegative = false;

        if((this.data[maxLength-1] & 0x80000000) != 0)   // negative this
        {
            tempNum = -this % n;
            thisNegative = true;
        }
        else
            tempNum = this % n;  // ensures (tempNum * tempNum) < b^(2k)

        if((n.data[maxLength-1] & 0x80000000) != 0)   // negative n
                n = -n;

        // calculate constant = b^(2k) / m
        BigInteger constant = new BigInteger();

        int i = n.dataLength << 1;
        //Console.WriteLine("n.dataLength << 1: {0}", i);
        constant.data[i] = 0x00000001;
        constant.dataLength = i + 1;

        constant = constant / n;
        int totalBits = exp.bitCount();
        int count = 0;

        // perform squaring and multiply exponentiation
        for(int pos = 0; pos < exp.dataLength; pos++)
        {
            uint mask = 0x01;
            //Console.WriteLine("pos = " + pos);

            for(int index = 0; index < 32; index++)
            {
                if((exp.data[pos] & mask) != 0)
                    resultNum = BarrettReduction(resultNum * tempNum, n, constant);

                mask <<= 1;

                tempNum = BarrettReduction(tempNum * tempNum, n, constant);


                if(tempNum.dataLength == 1 && tempNum.data[0] == 1)
                {
                    if(thisNegative && (exp.data[0] & 0x1) != 0)    //odd exp
                        return -resultNum;
                    return resultNum;
                }
                count++;
                if(count == totalBits)
                    break;
            }
        }

        if(thisNegative && (exp.data[0] & 0x1) != 0)    //odd exp
            return -resultNum;

        return resultNum;
    }

    //***********************************************************************
    // Naive method of exponentiation
    //***********************************************************************
    
    public BigInteger Pow(int exp)
    {
        if (exp < 0)
            throw new ArgumentException("Positive exponents only.");
        
        BigInteger result = new BigInteger(1);

        if (exp == 0)
            return result;
        if (exp == 1)
            return this;

        // initialize result = this but only when special case of powers of two
        // if (exp % 2 == 0)
        // result = this;
        // else
            // exp--;
        string strHexPowerOf2 = String.Format("{0:X}", exp);
        
        Regex re = new Regex("^[1248]{1}0?$");
        if (re.IsMatch(strHexPowerOf2))
        {
            // Console.WriteLine("\nFound match: {0}", strHexPowerOf2);
            result = this;
            
            while (exp > 1)
            {
                result *= result;
                exp >>= 1;
            }
        }
        else
        {
                // Console.WriteLine("exp: {0}", exp);
            // while ((exp & 0x80000000) == 0)
                // exp <<= 1;
            // for ( ; exp > 0; exp--)
                // result *= this;
            
            // return result;
            
            // Console.WriteLine("\nexp: 0x{0:X}", exp);
            // Console.WriteLine("{0}", (exp & 0x80000000) == 0x80000000);
            string strBinary = "";
            while (exp > 0)
            {
                if ((exp & 1) == 1)
                    strBinary = "1" + strBinary;
                else
                    strBinary = "0" + strBinary;
                exp >>= 1;
            }
            // Console.WriteLine("\nstrBinary: {0}", strBinary);
            
            // int i = 0;
            while (strBinary.Length > 0)
            {
                // Console.WriteLine("\nstep: {0}\t{1}", i, result);
                if (strBinary[0] == '1')
                {
                    // Console.Write("multiplying...");
                    result *= this;
                }
                
                strBinary = strBinary.Substring(1);
                if (strBinary.Length > 0)
                {
                    // Console.Write("squaring...");
                    result *= result;
                }
                // Console.WriteLine("exp: {0}", exp);
                // Console.WriteLine("\nstep: {0}\t{1}", i++, result);
            } 
        }
        
        return result;
    }

    //***********************************************************************
    // Fast calculation of modular reduction using Barrett's reduction.
    // Requires x < b^(2k), where b is the base.  In this case, base is
    // 2^32 (uint).
    //
    // Reference [4]
    //***********************************************************************

    private BigInteger BarrettReduction(BigInteger x, BigInteger n, BigInteger constant)
    {
        int k = n.dataLength,
            kPlusOne = k+1,
            kMinusOne = k-1;

        BigInteger q1 = new BigInteger();

        // q1 = x / b^(k-1)
        for(int i = kMinusOne, j = 0; i < x.dataLength; i++, j++)
            q1.data[j] = x.data[i];

        q1.dataLength = x.dataLength - kMinusOne;
        if(q1.dataLength <= 0)
            q1.dataLength = 1;


        BigInteger q2 = q1 * constant;
        BigInteger q3 = new BigInteger();

        // q3 = q2 / b^(k+1)
        for(int i = kPlusOne, j = 0; i < q2.dataLength; i++, j++)
            q3.data[j] = q2.data[i];

        q3.dataLength = q2.dataLength - kPlusOne;
        if(q3.dataLength <= 0)
            q3.dataLength = 1;


        // r1 = x mod b^(k+1)
        // i.e. keep the lowest (k+1) words
        BigInteger r1 = new BigInteger();
        int lengthToCopy = (x.dataLength > kPlusOne) ? kPlusOne : x.dataLength;
        for(int i = 0; i < lengthToCopy; i++)
            r1.data[i] = x.data[i];
        r1.dataLength = lengthToCopy;


        // r2 = (q3 * n) mod b^(k+1)
        // partial multiplication of q3 and n

        BigInteger r2 = new BigInteger();
        for(int i = 0; i < q3.dataLength; i++)
        {
            if(q3.data[i] == 0)     continue;

            ulong mcarry = 0;
            int t = i;
            for(int j = 0; j < n.dataLength && t < kPlusOne; j++, t++)
            {
                // t = i + j
                ulong val = ((ulong)q3.data[i] * (ulong)n.data[j]) +
                             (ulong)r2.data[t] + mcarry;

                r2.data[t] = (uint)(val & 0xFFFFFFFF);
                mcarry = (val >> 32);
            }

            if(t < kPlusOne)
                r2.data[t] = (uint)mcarry;
        }
        r2.dataLength = kPlusOne;
        while(r2.dataLength > 1 && r2.data[r2.dataLength-1] == 0)
            r2.dataLength--;

        r1 -= r2;
        if((r1.data[maxLength-1] & 0x80000000) != 0)        // negative
        {
            BigInteger val = new BigInteger();
            val.data[kPlusOne] = 0x00000001;
            val.dataLength = kPlusOne + 1;
            r1 += val;
        }

        while(r1 >= n)
            r1 -= n;

        return r1;
    }


    //***********************************************************************
    // Returns gcd(this, bi)
    //***********************************************************************

    public BigInteger gcd(BigInteger bi)
    {
        BigInteger x;
        BigInteger y;

        if((data[maxLength-1] & 0x80000000) != 0)     // negative
                x = -this;
        else
                x = this;

        if((bi.data[maxLength-1] & 0x80000000) != 0)     // negative
                y = -bi;
        else
                y = bi;

        BigInteger g = y;

        while(x.dataLength > 1 || (x.dataLength == 1 && x.data[0] != 0))
        {
            g = x;
            x = y % x;
            y = g;
        }

        return g;
    }


    //***********************************************************************
    // Populates "this" with the specified amount of random bits
    //***********************************************************************

    public void genRandomBits(int bits, Random rand)
    {
        int dwords = bits >> 5;
        int remBits = bits & 0x1F;

        if(remBits != 0)
            dwords++;

        if(dwords > maxLength)
            throw new ArgumentException("Number of required bits > maxLength.");

        for(int i = 0; i < dwords; i++)
            data[i] = (uint)(rand.Next() | (rand.Next() & 1 << 32));
            //data[i] = (uint)(rand.NextDouble() * 0x100000000);

        for(int i = dwords; i < maxLength; i++)
            data[i] = 0;

        if(remBits != 0)
        {
            uint mask = (uint)(0x01 << (remBits-1));
            data[dwords-1] |= mask;

            mask = (uint)(0xFFFFFFFF >> (32 - remBits));
            data[dwords-1] &= mask;
        }
        else
            data[dwords-1] |= 0x80000000;

        dataLength = dwords;

        if(dataLength == 0)
            dataLength = 1;
    }


    //***********************************************************************
    // Returns the position of the most significant bit in the BigInteger.
    //
    // Eg.  The result is 0, if the value of BigInteger is 0...0000 0000
    //      The result is 1, if the value of BigInteger is 0...0000 0001
    //      The result is 2, if the value of BigInteger is 0...0000 0010
    //      The result is 2, if the value of BigInteger is 0...0000 0011
    //
    //***********************************************************************

    public int bitCount()
    {
        while(dataLength > 1 && data[dataLength-1] == 0)
            dataLength--;

        uint value = data[dataLength - 1];
        uint mask = 0x80000000;
        int bits = 32;

        while(bits > 0 && (value & mask) == 0)
        {
            bits--;
            mask >>= 1;
        }
        bits += ((dataLength - 1) << 5);

        return bits;
    }


    //***********************************************************************
    // Probabilistic prime test based on Fermat's little theorem
    //
    // for any a < p (p does not divide a) if
    //      a^(p-1) mod p != 1 then p is not prime.
    //
    // Otherwise, p is probably prime (pseudoprime to the chosen base).
    //
    // Returns
    // -------
    // True if "this" is a pseudoprime to randomly chosen
    // bases.  The number of chosen bases is given by the "confidence"
    // parameter.
    //
    // False if "this" is definitely NOT prime.
    //
    // Note - this method is fast but fails for Carmichael numbers except
    // when the randomly chosen base is a factor of the number.
    //
    //***********************************************************************

    public bool FermatLittleTest(int confidence)
    {
        BigInteger thisVal;
        if((this.data[maxLength-1] & 0x80000000) != 0)        // negative
            thisVal = -this;
        else
            thisVal = this;

        if(thisVal.dataLength == 1)
        {
            // test small numbers
            if(thisVal.data[0] == 0 || thisVal.data[0] == 1)
                return false;
            else if(thisVal.data[0] == 2 || thisVal.data[0] == 3)
                return true;
        }

        if((thisVal.data[0] & 0x1) == 0)     // even numbers
            return false;

        int bits = thisVal.bitCount();
        BigInteger a = new BigInteger();
        BigInteger p_sub1 = thisVal - new BigInteger(1);
        Random rand = new Random();

        for(int round = 0; round < confidence; round++)
        {
/*            bool done = false;

             while(!done)        // generate a < n
            {
                int testBits = 0;

                // make sure "a" has at least 2 bits
                while(testBits < 2)
                    testBits = (int)(rand.Next() % bits);

                a.genRandomBits(testBits, rand);

                int byteLen = a.dataLength;

                // make sure "a" is not 0
                if(byteLen > 1 || (byteLen == 1 && a.data[0] != 1))
                    done = true;
            }
 */
			a = primesBelow2000[round];
			
            // check whether a factor exists (fix for version 1.03)
            BigInteger gcdTest = a.gcd(thisVal);
            if(gcdTest.dataLength == 1 && gcdTest.data[0] != 1)
                return false;

            // calculate a^(p-1) mod p
            BigInteger expResult = a.modPow(p_sub1, thisVal);

            int resultLen = expResult.dataLength;

            // is NOT prime is a^(p-1) mod p != 1

            if(resultLen > 1 || (resultLen == 1 && expResult.data[0] != 1))
            {
                //Console.WriteLine("a = " + a.ToString());
                return false;
            }
        }

        return true;
    }


    //***********************************************************************
    // Probabilistic prime test based on Rabin-Miller's
    //
    // for any p > 0 with p - 1 = 2^s * t
    //
    // p is probably prime (strong pseudoprime) if for any a < p,
    // 1) a^t mod p = 1 or
    // 2) a^((2^j)*t) mod p = p-1 for some 0 <= j <= s-1
    //
    // Otherwise, p is composite.
    //
    // Returns
    // -------
    // True if "this" is a strong pseudoprime to randomly chosen
    // bases.  The number of chosen bases is given by the "confidence"
    // parameter.
    //
    // False if "this" is definitely NOT prime.
    //
    //***********************************************************************

    public bool RabinMillerTest(int confidence)
    {
        BigInteger thisVal;
        if((this.data[maxLength-1] & 0x80000000) != 0)        // negative
            thisVal = -this;
        else
            thisVal = this;

        if(thisVal.dataLength == 1)
        {
            // test small numbers
            if(thisVal.data[0] == 0 || thisVal.data[0] == 1)
                return false;
            else if(thisVal.data[0] == 2 || thisVal.data[0] == 3)
                return true;
        }

        if((thisVal.data[0] & 0x1) == 0)     // even numbers
                return false;


        // calculate values of s and t
        BigInteger p_sub1 = thisVal - (new BigInteger(1));
        int s = 0;

        for(int index = 0; index < p_sub1.dataLength; index++)
        {
            uint mask = 0x01;

            for(int i = 0; i < 32; i++)
            {
                if((p_sub1.data[index] & mask) != 0)
                {
                    index = p_sub1.dataLength;      // to break the outer loop
                    break;
                }
                mask <<= 1;
                s++;
            }
        }

        BigInteger t = p_sub1 >> s;

        int bits = thisVal.bitCount();
        BigInteger a = new BigInteger();
        Random rand = new Random();

        for(int round = 0; round < confidence; round++)
        {
            bool done = false;

            while(!done)        // generate a < n
            {
                int testBits = 0;

                // make sure "a" has at least 2 bits
                while(testBits < 2)
                    testBits = (int)(rand.Next() % bits);

                a.genRandomBits(testBits, rand);

                int byteLen = a.dataLength;

                // make sure "a" is not 0
                if(byteLen > 1 || (byteLen == 1 && a.data[0] != 1))
                    done = true;
            }

            // check whether a factor exists (fix for version 1.03)
            BigInteger gcdTest = a.gcd(thisVal);
            if(gcdTest.dataLength == 1 && gcdTest.data[0] != 1)
                return false;

            BigInteger b = a.modPow(t, thisVal);

            /*
            Console.WriteLine("a = " + a.ToString(10));
            Console.WriteLine("b = " + b.ToString(10));
            Console.WriteLine("t = " + t.ToString(10));
            Console.WriteLine("s = " + s);
            */

            bool result = false;

            if(b.dataLength == 1 && b.data[0] == 1)         // a^t mod p = 1
                result = true;

            for(int j = 0; result == false && j < s; j++)
            {
                if(b == p_sub1)         // a^((2^j)*t) mod p = p-1 for some 0 <= j <= s-1
                {
                    result = true;
                    break;
                }

                b = (b * b) % thisVal;
            }

            if(result == false)
                return false;
        }
        return true;
    }


    //***********************************************************************
    // Probabilistic prime test based on Solovay-Strassen (Euler Criterion)
    //
    // p is probably prime if for any a < p (a is not multiple of p),
    // a^((p-1)/2) mod p = J(a, p)
    //
    // where J is the Jacobi symbol.
    //
    // Otherwise, p is composite.
    //
    // Returns
    // -------
    // True if "this" is a Euler pseudoprime to randomly chosen
    // bases.  The number of chosen bases is given by the "confidence"
    // parameter.
    //
    // False if "this" is definitely NOT prime.
    //
    //***********************************************************************

    public bool SolovayStrassenTest(int confidence)
    {
        BigInteger thisVal;
        if((this.data[maxLength-1] & 0x80000000) != 0)        // negative
            thisVal = -this;
        else
            thisVal = this;

        if(thisVal.dataLength == 1)
        {
                // test small numbers
                if(thisVal.data[0] == 0 || thisVal.data[0] == 1)
                    return false;
                else if(thisVal.data[0] == 2 || thisVal.data[0] == 3)
                    return true;
        }

        if((thisVal.data[0] & 0x1) == 0)     // even numbers
                return false;


        int bits = thisVal.bitCount();
        BigInteger a = new BigInteger();
        BigInteger p_sub1 = thisVal - 1;
        BigInteger p_sub1_shift = p_sub1 >> 1;

        Random rand = new Random();

        for(int round = 0; round < confidence; round++)
        {
            bool done = false;

            while(!done)        // generate a < n
            {
                int testBits = 0;

                // make sure "a" has at least 2 bits
                while(testBits < 2)
                    testBits = (int)(rand.Next() % bits);

                a.genRandomBits(testBits, rand);

                int byteLen = a.dataLength;

                            // make sure "a" is not 0
                if(byteLen > 1 || (byteLen == 1 && a.data[0] != 1))
                    done = true;
            }

            // check whether a factor exists (fix for version 1.03)
            BigInteger gcdTest = a.gcd(thisVal);
            if(gcdTest.dataLength == 1 && gcdTest.data[0] != 1)
                return false;

            // calculate a^((p-1)/2) mod p

            BigInteger expResult = a.modPow(p_sub1_shift, thisVal);
            if(expResult == p_sub1)
                expResult = -1;

            // calculate Jacobi symbol
            BigInteger jacob = Jacobi(a, thisVal);

            //Console.WriteLine("a = " + a.ToString(10) + " b = " + thisVal.ToString(10));
            //Console.WriteLine("expResult = " + expResult.ToString(10) + " Jacob = " + jacob.ToString(10));

            // if they are different then it is not prime
            if(expResult != jacob)
                return false;
        }

        return true;
    }


    //***********************************************************************
    // Implementation of the Lucas Strong Pseudo Prime test.
    //
    // Let n be an odd number with gcd(n,D) = 1, and n - J(D, n) = 2^s * d
    // with d odd and s >= 0.
    //
    // If Ud mod n = 0 or V2^r*d mod n = 0 for some 0 <= r < s, then n
    // is a strong Lucas pseudoprime with parameters (P, Q).  We select
    // P and Q based on Selfridge.
    //
    // Returns True if number is a strong Lucus pseudo prime.
    // Otherwise, returns False indicating that number is composite.
    //***********************************************************************

    public bool LucasStrongTest()
    {
        BigInteger thisVal;
        if((this.data[maxLength-1] & 0x80000000) != 0)        // negative
            thisVal = -this;
        else
            thisVal = this;

        if(thisVal.dataLength == 1)
        {
            // test small numbers
            if(thisVal.data[0] == 0 || thisVal.data[0] == 1)
                return false;
            else if(thisVal.data[0] == 2 || thisVal.data[0] == 3)
                return true;
        }

        if((thisVal.data[0] & 0x1) == 0)     // even numbers
            return false;

        return LucasStrongTestHelper(thisVal);
    }


    private bool LucasStrongTestHelper(BigInteger thisVal)
    {
        // Do the test (selects D based on Selfridge)
        // Let D be the first element of the sequence
        // 5, -7, 9, -11, 13, ... for which J(D,n) = -1
        // Let P = 1, Q = (1-D) / 4

        long D = 5, sign = -1, dCount = 0;
        bool done = false;

        //Console.WriteLine("LucasStrongTestHelper...");
        while(!done)
        {
            int Jresult = BigInteger.Jacobi(D, thisVal);        // Parameters are initialized as (BigInteger(long), BigInteger thisVal)

            if(Jresult == -1)
            {
                done = true;    //  Jacobi(D, this) = -1
            }
            else
            {
                // Console.WriteLine("Is it here?  Math.Abs(D)= {0}", Math.Abs(D));
                if(Jresult == 0 && Math.Abs(D) < thisVal)       // divisor found
                    return false;

                if(dCount == 20)
                {
                    // check for (perfect) square
                    BigInteger root = thisVal.sqrt();
                    if(root * root == thisVal)
                        return false;
                }

                // Console.WriteLine(D);
                D = (Math.Abs(D) + 2) * sign;
                // Console.WriteLine("Is it here?  D= {0}", D);
                sign = -sign;
            }
            dCount++;
            // Console.WriteLine("Is it here?  dCount= {0}\ndone: {1}", dCount, done);
        }

        long Q = (1 - D) >> 2;

        /*
        Console.WriteLine("D = " + D);
        Console.WriteLine("Q = " + Q);
        Console.WriteLine("(n,D) = " + thisVal.gcd(D));
        Console.WriteLine("(n,Q) = " + thisVal.gcd(Q));
        Console.WriteLine("J(D|n) = " + BigInteger.Jacobi(D, thisVal));
        */

        BigInteger p_add1 = thisVal + 1;
        int s = 0;

        for(int index = 0; index < p_add1.dataLength; index++)
        {
            uint mask = 0x01;

            for(int i = 0; i < 32; i++)
            {
                if((p_add1.data[index] & mask) != 0)
                {
                    index = p_add1.dataLength;      // to break the outer loop
                    break;
                }
                mask <<= 1;
                s++;
            }
        }

        BigInteger t = p_add1 >> s;

        // calculate constant = b^(2k) / m
        // for Barrett Reduction
        BigInteger constant = new BigInteger();

        int nLen = thisVal.dataLength << 1;
        constant.data[nLen] = 0x00000001;
        constant.dataLength = nLen + 1;

        constant = constant / thisVal;

        // Console.WriteLine("LucasSequenceHelper...");
        BigInteger[] lucas = LucasSequenceHelper(1, Q, t, thisVal, constant, 0);
        bool isPrime = false;

        if((lucas[0].dataLength == 1 && lucas[0].data[0] == 0) ||
           (lucas[1].dataLength == 1 && lucas[1].data[0] == 0))
        {
            // u(t) = 0 or V(t) = 0
            isPrime = true;
        }

        for(int i = 1; i < s; i++)
        {
            if(!isPrime)
            {
                // doubling of index
                lucas[1] = thisVal.BarrettReduction(lucas[1] * lucas[1], thisVal, constant);
                lucas[1] = (lucas[1] - (lucas[2] << 1)) % thisVal;

                // lucas[1] = ((lucas[1] * lucas[1]) - (lucas[2] << 1)) % thisVal;

                if((lucas[1].dataLength == 1 && lucas[1].data[0] == 0))
                    isPrime = true;
            }

            lucas[2] = thisVal.BarrettReduction(lucas[2] * lucas[2], thisVal, constant);     // Q^k
        }


        if(isPrime)     // additional checks for composite numbers
        {
            // If n is prime and gcd(n, Q) == 1, then
            // Q^((n+1)/2) = Q * Q^((n-1)/2) is congruent to (Q * J(Q, n)) mod n

            BigInteger g = thisVal.gcd(Q);
            if(g.dataLength == 1 && g.data[0] == 1)         // gcd(this, Q) == 1
            {
                if((lucas[2].data[maxLength-1] & 0x80000000) != 0)
                    lucas[2] += thisVal;

                BigInteger temp = (Q * BigInteger.Jacobi(Q, thisVal)) % thisVal;
                if((temp.data[maxLength-1] & 0x80000000) != 0)
                    temp += thisVal;

                if(lucas[2] != temp)
                    isPrime = false;
            }
        }

        return isPrime;
    }


    //***********************************************************************
    // Determines whether a number is probably prime, using the Rabin-Miller's
    // test.  Before applying the test, the number is tested for divisibility
    // by primes < 2000
    //
    // Returns true if number is probably prime.
    //***********************************************************************

    public bool isProbablePrime(int confidence)
    {
        BigInteger thisVal;
        if((this.data[maxLength-1] & 0x80000000) != 0)        // negative
            thisVal = -this;
        else
            thisVal = this;


        // test for divisibility by primes < 2000
        for(int p = 0; p < primesBelow2000.Length; p++)
        {
            BigInteger divisor = primesBelow2000[p];

            if(divisor >= thisVal)
                break;

            BigInteger resultNum = thisVal % divisor;
            if(resultNum.IntValue() == 0)
            {
                /*
                Console.WriteLine("Not prime!  Divisible by {0}\n",
                                  primesBelow2000[p]);
                */
                return false;
            }
        }

        if(thisVal.RabinMillerTest(confidence))
            return true;
        else
        {
            //Console.WriteLine("Not prime!  Failed primality test\n");
            return false;
        }
    }


    //***********************************************************************
    // Determines whether this BigInteger is probably prime using a
    // combination of base 2 strong pseudoprime test and Lucas strong
    // pseudoprime test.
    //
    // The sequence of the primality test is as follows,
    //
    // 1) Trial divisions are carried out using prime numbers below 2000.
    //    if any of the primes divides this BigInteger, then it is not prime.
    //
    // 2) Perform base 2 strong pseudoprime test.  If this BigInteger is a
    //    base 2 strong pseudoprime, proceed on to the next step.
    //
    // 3) Perform strong Lucas pseudoprime test.
    //
    // Returns True if this BigInteger is both a base 2 strong pseudoprime
    // and a strong Lucas pseudoprime.
    //
    // For a detailed discussion of this primality test, see [6].
    //
    //***********************************************************************

    public bool isProbablePrime()
    {
        BigInteger thisVal;
        if((this.data[maxLength-1] & 0x80000000) != 0)        // negative
        {
            thisVal = -this;
            Console.WriteLine("-ve");
        }
        else
            thisVal = this;

        if(thisVal.dataLength == 1)
        {
            // test small numbers
            if(thisVal.data[0] == 0 || thisVal.data[0] == 1)
                return false;
            else if(thisVal.data[0] == 2 || thisVal.data[0] == 3)
                return true;
        }

        if((thisVal.data[0] & 0x1) == 0)     // even numbers
            return false;


        // test for divisibility by primes < 2000
        for(int p = 0; p < primesBelow2000.Length; p++)
        {
            BigInteger divisor = primesBelow2000[p];

            if(divisor >= thisVal)
                break;

            BigInteger resultNum = thisVal % divisor;
            if(resultNum.IntValue() == 0)
            {
                //Console.WriteLine("Not prime!  Divisible by {0}\n",
                //                  primesBelow2000[p]);

                return false;
            }
        }

        // Perform BASE 2 Rabin-Miller Test

        // calculate values of s and t
        BigInteger p_sub1 = thisVal - new BigInteger(1);
        int s = 0;

        for(int index = 0; index < p_sub1.dataLength; index++)
        {
            uint mask = 0x01;

            for(int i = 0; i < 32; i++)
            {
                if((p_sub1.data[index] & mask) != 0)
                {
                    index = p_sub1.dataLength;      // to break the outer loop
                    break;
                }
                mask <<= 1;
                s++;
            }
        }

        BigInteger t = p_sub1 >> s;

        int bits = thisVal.bitCount();
        BigInteger a = 2;

        // b = a^t mod p
        BigInteger b = a.modPow(t, thisVal);
        bool result = false;

        if(b.dataLength == 1 && b.data[0] == 1)         // a^t mod p = 1
            result = true;

        for(int j = 0; result == false && j < s; j++)
        {
            if(b == p_sub1)         // a^((2^j)*t) mod p = p-1 for some 0 <= j <= s-1
            {
                result = true;
                break;
            }

            b = (b * b) % thisVal;
        }

        // if number is strong pseudoprime to base 2, then do a strong lucas test
        if(result)
        {
            // Console.WriteLine("result: {0}", result);
            result = LucasStrongTestHelper(thisVal);
        }

        return result;
    }



    //***********************************************************************
    // Returns the lowest 4 bytes of the BigInteger as an int.
    //***********************************************************************

    public int IntValue()
    {
        return (int)data[0];
    }


    //***********************************************************************
    // Returns the lowest 8 bytes of the BigInteger as a long.
    //***********************************************************************

    public long LongValue()
    {
        long val = 0;

        val = (long)data[0];
        try
        {   
            // exception if maxLength = 1
            val |= (long)data[1] << 32;
        }
        catch(Exception)
        {
            if((data[0] & 0x80000000) != 0) // negative
                val = (int)data[0];
        }

        return val;
    }


    //***********************************************************************
    // Computes the Jacobi Symbol for a and b.
    // Algorithm adapted from [3] and [4] with some optimizations
    //***********************************************************************

    public static int Jacobi(BigInteger a, BigInteger b)
    {
        // Console.WriteLine("a = {0}", a);
        // Jacobi defined only for odd integers
        if((b.data[0] & 0x1) == 0 && b != 2)
            throw new ArgumentException("Jacobi defined only for odd integers.");

        if(a >= b)
            a %= b;
        if(a.dataLength == 1 && a.data[0] == 0)
            return 0;  // a == 0
        if(a.dataLength == 1 && a.data[0] == 1)
            return 1;  // a == 1

        if(a < 0)
        {
            if( (((b-1).data[0]) & 0x2) == 0)       //if( (((b-1) >> 1).data[0] & 0x1) == 0)
                return Jacobi(-a, b);
            else
                return -Jacobi(-a, b);
        }

        int e = 0;
        for(int index = 0; index < a.dataLength; index++)
        {
            uint mask = 0x01;

            for(int i = 0; i < 32; i++)
            {
                if((a.data[index] & mask) != 0)
                {
                    index = a.dataLength;      // to break the outer loop
                    break;
                }
                mask <<= 1;
                e++;
            }
        }

        BigInteger a1 = a >> e;

        int s = 1;
        if((e & 0x1) != 0 && ((b.data[0] & 0x7) == 3 || (b.data[0] & 0x7) == 5))
            s = -1;

        if((b.data[0] & 0x3) == 3 && (a1.data[0] & 0x3) == 3)
            s = -s;

        if(a1.dataLength == 1 && a1.data[0] == 1)
            return s;
        else
            return (s * Jacobi(b % a1, a1));
    }



    //***********************************************************************
    // Generates a positive BigInteger that is probably prime.
    //***********************************************************************

    public static BigInteger genPseudoPrime(int bits, int confidence, Random rand)
    {
        BigInteger result = new BigInteger();
        bool done = false;

        while(!done)
        {
            result.genRandomBits(bits, rand);
            result.data[0] |= 0x01;     // make it odd

            Console.Write('.');
            //Console.Out.Flush();
            // prime test
            done = result.isProbablePrime(confidence);
        }
        Console.WriteLine();
        return result;
    }


    //***********************************************************************
    // Generates a random number with the specified number of bits such
    // that gcd(number, this) = 1
    //***********************************************************************

    public BigInteger genCoPrime(int bits, Random rand)
    {
        bool done = false;
        BigInteger result = new BigInteger();

        while(!done)
        {
            result.genRandomBits(bits, rand);
            //Console.WriteLine(result.ToString(16));

            // gcd test
            BigInteger g = result.gcd(this);
            if(g.dataLength == 1 && g.data[0] == 1)
                done = true;
        }

        return result;
    }


    //***********************************************************************
    // Returns the modulo inverse of this.  Throws ArithmeticException if
    // the inverse does not exist.  (i.e. gcd(this, modulus) != 1)
    //***********************************************************************

    public BigInteger modInverse(BigInteger modulus)
    {
        BigInteger[] p = { 0, 1 };
        BigInteger[] q = new BigInteger[2];    // quotients
        BigInteger[] r = { 0, 0 };             // remainders

        int step = 0;

        BigInteger a = modulus;
        BigInteger b = this;

        while(b.dataLength > 1 || (b.dataLength == 1 && b.data[0] != 0))
        {
            BigInteger quotient = new BigInteger();
            BigInteger remainder = new BigInteger();

            if(step > 1)
            {
                BigInteger pval = (p[0] - (p[1] * q[0])) % modulus;
                p[0] = p[1];
                p[1] = pval;
            }

            if(b.dataLength == 1)
                singleByteDivide(a, b, quotient, remainder);
            else
                multiByteDivide(a, b, quotient, remainder);

            /*
            Console.WriteLine(quotient.dataLength);
            Console.WriteLine("{0} = {1}({2}) + {3}  p = {4}", a.ToString(10),
                              b.ToString(10), quotient.ToString(10), remainder.ToString(10),
                              p[1].ToString(10));
            */

            q[0] = q[1];
            r[0] = r[1];
            q[1] = quotient; r[1] = remainder;

            a = b;
            b = remainder;

            step++;
        }

        if(r[0].dataLength > 1 || (r[0].dataLength == 1 && r[0].data[0] != 1))
            throw new ArithmeticException("No modular inverse exists!");

        BigInteger result = ((p[0] - (p[1] * q[0])) % modulus);

        if((result.data[maxLength - 1] & 0x80000000) != 0)
            result += modulus;  // get the least positive modulus

        return result;
    }


    //***********************************************************************
    // Returns the value of the BigInteger as a byte array.  The lowest
    // index contains the MSB.
    //***********************************************************************

    public byte[] getBytes()
    {
        int numBits = bitCount();

        int numBytes = numBits >> 3;
        if((numBits & 0x7) != 0)
            numBytes++;

        byte[] result = new byte[numBytes];

        //Console.WriteLine(result.Length);

        int pos = 0;
        uint tempVal, val = data[dataLength - 1];

        if((tempVal = (val >> 24 & 0xFF)) != 0)
            result[pos++] = (byte)tempVal;
        if((tempVal = (val >> 16 & 0xFF)) != 0)
            result[pos++] = (byte)tempVal;
        if((tempVal = (val >> 8 & 0xFF)) != 0)
            result[pos++] = (byte)tempVal;
        if((tempVal = (val & 0xFF)) != 0)
            result[pos++] = (byte)tempVal;

        for(int i = dataLength - 2; i >= 0; i--, pos += 4)
        {
            val = data[i];
            result[pos+3] = (byte)(val & 0xFF);
            val >>= 8;
            result[pos+2] = (byte)(val & 0xFF);
            val >>= 8;
            result[pos+1] = (byte)(val & 0xFF);
            val >>= 8;
            result[pos] = (byte)(val & 0xFF);
        }

        return result;
    }


    //***********************************************************************
    // Sets the value of the specified bit to 1
    // The Least Significant Bit position is 0.
    //***********************************************************************

    public void setBit(uint bitNum)
    {
        uint bytePos = bitNum >> 5;             // divide by 32
        byte bitPos = (byte)(bitNum & 0x1F);    // get the lowest 5 bits

        uint mask = (uint)1 << bitPos;
        this.data[bytePos] |= mask;

        if(bytePos >= this.dataLength)
            this.dataLength = (int)bytePos + 1;
    }


    //***********************************************************************
    // Sets the value of the specified bit to 0
    // The Least Significant Bit position is 0.
    //***********************************************************************

    public void unsetBit(uint bitNum)
    {
        uint bytePos = bitNum >> 5;

        if(bytePos < this.dataLength)
        {
            byte bitPos = (byte)(bitNum & 0x1F);

            uint mask = (uint)1 << bitPos;
            uint mask2 = 0xFFFFFFFF ^ mask;

            this.data[bytePos] &= mask2;

            if(this.dataLength > 1 && this.data[this.dataLength - 1] == 0)
                this.dataLength--;
        }
    }


    //***********************************************************************
    // Returns a value that is equivalent to the integer square root
    // of the BigInteger.
    //
    // The integer square root of "this" is defined as the largest integer n
    // such that (n * n) <= this
    //
    //***********************************************************************

    public BigInteger sqrt()
    {
        uint numBits = (uint)this.bitCount();

        if((numBits & 0x1) != 0)        // odd number of bits
            numBits = (numBits >> 1) + 1;
        else
            numBits = (numBits >> 1);

        uint bytePos = numBits >> 5;
        byte bitPos = (byte)(numBits & 0x1F);

        uint mask;

        BigInteger result = new BigInteger();
        if(bitPos == 0)
            mask = 0x80000000;
        else
        {
            mask = (uint)1 << bitPos;
            bytePos++;
        }
        result.dataLength = (int)bytePos;

        int j = 0;
        for(int i = (int)bytePos - 1; i >= 0; i--)
        {
            while(mask != 0)
            {
                // guess
                result.data[i] ^= mask;

                // undo the guess if its square is larger than this
                if((result * result) > this)
                    result.data[i] ^= mask;

                mask >>= 1;
                j++;
            }
            mask = 0x80000000;
            j++;
        }
        // Console.WriteLine("iterations: {0}", j);
        return result;
    }


    BigInteger nthRoot(int n) {
        if (this < new BigInteger(0) || n <= 0) {
            throw new ArgumentException("Illegal argument.");
        }
 
        int n1 = n - 1;
        BigInteger n2 = new BigInteger(n);
        BigInteger n3 = new BigInteger(n1);
        BigInteger c = new BigInteger(1);
        BigInteger d = (n3 + this) / n2;
        BigInteger e = (n3 * d + this / d.Pow(n1)) / n2;
        BigInteger f = new BigInteger();

        int i = 0;
        while (!c.Equals(d) && !c.Equals(e)) {
            c = d;
            d = e;
            f = e.Pow(n1);
            // Console.WriteLine("{0}\n{1}\n", f, n2);
            // Console.ReadLine();
            e = (n3 * e + this / e.Pow(n1)) / n2;
            i++;
        }
        //Console.WriteLine("iterations: {0}", i);
        if (d < e) {
            return d;
        }
        return e;
    }
    
    //***********************************************************************
    // Returns the k_th number in the Lucas Sequence reduced modulo n.
    //
    // Uses index doubling to speed up the process.  For example, to calculate V(k),
    // we maintain two numbers in the sequence V(n) and V(n+1).
    //
    // To obtain V(2n), we use the identity
    //      V(2n) = (V(n) * V(n)) - (2 * Q^n)
    // To obtain V(2n+1), we first write it as
    //      V(2n+1) = V((n+1) + n)
    // and use the identity
    //      V(m+n) = V(m) * V(n) - Q * V(m-n)
    // Hence,
    //      V((n+1) + n) = V(n+1) * V(n) - Q^n * V((n+1) - n)
    //                   = V(n+1) * V(n) - Q^n * V(1)
    //                   = V(n+1) * V(n) - Q^n * P
    //
    // We use k in its binary expansion and perform index doubling for each
    // bit position.  For each bit position that is set, we perform an
    // index doubling followed by an index addition.  This means that for V(n),
    // we need to update it to V(2n+1).  For V(n+1), we need to update it to
    // V((2n+1)+1) = V(2*(n+1))
    //
    // This function returns
    // [0] = U(k)
    // [1] = V(k)
    // [2] = Q^n
    //
    // Where U(0) = 0 % n, U(1) = 1 % n
    //       V(0) = 2 % n, V(1) = P % n
    //***********************************************************************

    public static BigInteger[] LucasSequence(BigInteger P, BigInteger Q,
                                             BigInteger k, BigInteger n)
    {
        if(k.dataLength == 1 && k.data[0] == 0)
        {
            BigInteger[] result = new BigInteger[3];

            result[0] = 0; result[1] = 2 % n; result[2] = 1 % n;
            return result;
        }

        // calculate constant = b^(2k) / m
        // for Barrett Reduction
        BigInteger constant = new BigInteger();

        int nLen = n.dataLength << 1;
        constant.data[nLen] = 0x00000001;
        constant.dataLength = nLen + 1;

        constant = constant / n;

        // calculate values of s and t
        int s = 0;

        for(int index = 0; index < k.dataLength; index++)
        {
            uint mask = 0x01;

            for(int i = 0; i < 32; i++)
            {
                if((k.data[index] & mask) != 0)
                {
                    index = k.dataLength;      // to break the outer loop
                    break;
                }
                mask <<= 1;
                s++;
            }
        }

        BigInteger t = k >> s;

        //Console.WriteLine("s = " + s + " t = " + t);
        return LucasSequenceHelper(P, Q, t, n, constant, s);
    }


    //***********************************************************************
    // Performs the calculation of the kth term in the Lucas Sequence.
    // For details of the algorithm, see reference [9].
    //
    // k must be odd.  i.e LSB == 1
    //***********************************************************************

    private static BigInteger[] LucasSequenceHelper(BigInteger P, BigInteger Q,
                                                    BigInteger k, BigInteger n,
                                                    BigInteger constant, int s)
    {
        BigInteger[] result = new BigInteger[3];

        if((k.data[0] & 0x00000001) == 0)
            throw new ArgumentException("Argument k must be odd.");

        int numbits = k.bitCount();
        uint mask = (uint)0x1 << ((numbits & 0x1F) - 1);

        // v = v0, v1 = v1, u1 = u1, Q_k = Q^0

        BigInteger v = 2 % n, Q_k = 1 % n,
               v1 = P % n, u1 = Q_k;
        bool flag = true;

        for(int i = k.dataLength - 1; i >= 0 ; i--)     // iterate on the binary expansion of k
        {
            //Console.WriteLine("round");
            while(mask != 0)
            {
                if(i == 0 && mask == 0x00000001)        // last bit
                    break;

                if((k.data[i] & mask) != 0)             // bit is set
                {
                    // index doubling with addition

                    u1 = (u1 * v1) % n;

                    v = ((v * v1) - (P * Q_k)) % n;
                    v1 = n.BarrettReduction(v1 * v1, n, constant);
                    v1 = (v1 - ((Q_k * Q) << 1)) % n;

                    if(flag)
                        flag = false;
                    else
                        Q_k = n.BarrettReduction(Q_k * Q_k, n, constant);

                    Q_k = (Q_k * Q) % n;
                }
                else
                {
                    // index doubling
                    u1 = ((u1 * v) - Q_k) % n;

                    v1 = ((v * v1) - (P * Q_k)) % n;
                    v = n.BarrettReduction(v * v, n, constant);
                    v = (v - (Q_k << 1)) % n;

                    if(flag)
                    {
                        Q_k = Q % n;
                        flag = false;
                    }
                    else
                        Q_k = n.BarrettReduction(Q_k * Q_k, n, constant);
                }

                mask >>= 1;
            }
            mask = 0x80000000;
        }

        // at this point u1 = u(n+1) and v = v(n)
        // since the last bit always 1, we need to transform u1 to u(2n+1) and v to v(2n+1)

        u1 = ((u1 * v) - Q_k) % n;
        v = ((v * v1) - (P * Q_k)) % n;
        if(flag)
            flag = false;
        else
            Q_k = n.BarrettReduction(Q_k * Q_k, n, constant);

        Q_k = (Q_k * Q) % n;


        for(int i = 0; i < s; i++)
        {
            // index doubling
            u1 = (u1 * v) % n;
            v = ((v * v) - (Q_k << 1)) % n;

            if(flag)
            {
                Q_k = Q % n;
                flag = false;
            }
            else
                Q_k = n.BarrettReduction(Q_k * Q_k, n, constant);
        }

        result[0] = u1;
        result[1] = v;
        result[2] = Q_k;

        // Console.WriteLine("Exiting LucasSequenceHelper.");
        return result;
    }


    //***********************************************************************
    // Tests the multiplicative inverse function
    //***********************************************************************

    public static void ModInverseTest(int rounds)
    {
        Random rand = new Random();
        
        for(int count = 0; count < rounds; count++)
        {
            Console.Write("Round [{0,8}]: ", count);

            BigInteger modulus = genPseudoPrime(128, confidence, rand);
            
            foreach (int p in primesBelow2000)
            {
                BigInteger bi_p = new BigInteger(p);
                BigInteger bi_inv = bi_p.modInverse(modulus);
                
                var congruent_to_1 = bi_p * bi_inv % modulus;
                
                // Console.WriteLine("{0} * {1} % {2} ≡ {3}", bi_p, bi_inv, modulus, congruent_to_1);
                
                if (congruent_to_1 != 1)
                    throw new ArithmeticException( String.Format("{0}*{1} mod {2} was not congruent to 1.", bi_p, bi_inv, modulus) );
            }
            Console.WriteLine(" <PASSED>.");
            
        }
    }
    

    //***********************************************************************
    // Tests the correct implementation of the /, %, * and + operators
    //***********************************************************************

    public static void MulDivTest(int rounds)
    {
        Random rand = new Random();
        byte[] val1 = new byte[516];
        byte[] val2 = new byte[516];

        for(int count = 0; count < rounds; count++)
        {
            // generate 2 numbers of random length
            int t1 = 0;
            // while(t1 == 0)
                t1 = (int)rand.Next(513) + 4;

            int t2 = 0;
            // while(t2 == 0)
                t2 = (int)rand.Next(513) + 4;

            bool done = false;
            while(!done)
            {
                for(int i = 0; i < t1; i++)
                {
                    if(i < t1)
                        val1[i] = (byte)rand.Next();
                    else
                        val1[i] = 0;

                    if(val1[i] != 0)
                        done = true;
                }
                // Console.WriteLine($"1. {done}");
            }

            done = false;
            while(!done)
            {
                for(int i = 0; i < t2; i++)
                {
                    if(i < t2)
                        val2[i] = (byte)rand.Next();
                    else
                        val2[i] = 0;

                    if(val2[i] != 0)
                        done = true;
                }
                // Console.WriteLine($"2. {done}");
            }

            while(val1[0] == 0)
                val1[0] = (byte)rand.Next();
            
            while(val2[0] == 0)
                val2[0] = (byte)rand.Next();

            Console.Write("Round [{0,8}]: ", count);
            BigInteger bn1 = new BigInteger(val1, t1);
            BigInteger bn2 = new BigInteger(val2, t2);


            // Determine the quotient and remainder by dividing
            // the first number by the second.

            BigInteger bn3 = bn1 / bn2;
            BigInteger bn4 = bn1 % bn2;

            // Recalculate the number
            BigInteger bn5 = (bn3 * bn2) + bn4;

            // Make sure they're the same
            if(bn5 != bn1)
            {
                Console.WriteLine($"\n{bn1}\n{bn5}\n{bn1.bitCount()}\n{bn2.bitCount()}\n");
                Console.WriteLine("Error at " + count);
                Console.WriteLine(bn1 + "\n");
                Console.WriteLine(bn2 + "\n");
                Console.WriteLine(bn3 + "\n");
                Console.WriteLine(bn4 + "\n");
                Console.WriteLine(bn5 + "\n");
                return;
            }
            Console.WriteLine(" <PASSED>.");
        }
    }

    
    //***********************************************************************
    // Tests the correctness of raising x to the power n
    // - using list of primes < 2000 and modPow()
    //***********************************************************************
    public static void PowTest(int rounds)
    {
        BigInteger x;
        for(int count = 3; count < rounds; count++)
        {
            Console.Write("Round: {0}", count);
            
            foreach(int p in primesBelow2000)
            // foreach(int p in new int[] {2, 3, 5, 7, 11, 13, 17} )
            {
                BigInteger bigInt_p = new BigInteger(p);
                x = bigInt_p.Pow(count);

                // Console.WriteLine("\t{0}^{1} = {2}", p, count, x);
                
                if (count == 0 && x != new BigInteger(1))
                    throw new ArithmeticException("x.Pow(0) was not equal to 1.");
                if (count == 1 && x != bigInt_p)
                    throw new ArithmeticException("x.Pow(1) was not equal to x.");
                if (count > 0 && x % p != 0)
                    throw new ArithmeticException( String.Format("x mod {0} was not congruent to zero.", p) );
                if ( p > 2 && (x % (p-1) != 1) )
                    throw new ArithmeticException( String.Format("p^{0} mod (p-1) was not congruent to one.", count) );
                if ( x.gcd(new BigInteger(2017)) != 1 ) 
                    throw new ArithmeticException("gcd(x, 2017) has common denominator > 1.");
                
            }
            Console.WriteLine(" <PASSED>.");
            // Console.ReadLine();
            
        }
    }


    //***********************************************************************
    // Tests the correct implementation of the modulo exponential function
    // using RSA encryption and decryption (using pre-computed encryption and
    // decryption keys).
    //***********************************************************************

    public static void RSATest(int rounds)
    {
        const int LEN = 127;
        Random rand = new Random();
        byte[] val = new byte[LEN];

        // private and public key
        BigInteger bi_e = new BigInteger("a932b948feed4fb2b692609bd22164fc9edb59fae7880cc1eaff7b3c9626b7e5b241c27a974833b2622ebe09beb451917663d47232488f23a117fc97720f1e7", 16);
        BigInteger bi_d = new BigInteger("4adf2f7a89da93248509347d2ae506d683dd3a16357e859a980c4f77a4e2f7a01fae289f13a851df6e9db5adaa60bfd2b162bbbe31f7c8f828261a6839311929d2cef4f864dde65e556ce43c89bbbf9f1ac5511315847ce9cc8dc92470a747b8792d6a83b0092d2e5ebaf852c85cacf34278efa99160f2f8aa7ee7214de07b7", 16);
        
        //BigInteger bi_e = new BigInteger("10001", 16);
        BigInteger bi_n = new BigInteger("e8e77781f36a7b3188d711c2190b560f205a52391b3479cdb99fa010745cbeba5f2adc08e1de6bf38398a0487c4a73610d94ec36f17f3f46ad75e17bc1adfec99839589f45f95ccc94cb2a5c500b477eb3323d8cfab0c8458c96f0147a45d27e45a4d11d54d77684f65d48f15fafcc1ba208e71e921b9bd9017c16a5231af7f", 16);
        
        BigInteger modInv = bi_d.modInverse(bi_n) + 1;
        //Console.WriteLine("modInv + 1 =\n" + modInv.ToString());
        BigInteger dQ = bi_e.modInverse(modInv);
        //Console.WriteLine("dQ =\n" + dQ.ToString());
        
        Console.WriteLine("encrypting exponent =\n" + bi_e.ToString(10));
        Console.WriteLine("\ndecrypting exponent =\n" + bi_d.ToString(10));
        Console.WriteLine("\nN =\n" + bi_n.ToString(10) + "\n");

        for(int count = 0; count < rounds; count++)
        {
            // generate data of random length
            int t1 = 0;
            while(t1 == 0)
                t1 = (int)(rand.Next() % LEN);

            bool done = false;
            while(!done)
            {
                for(int i = 0; i < LEN; i++)
                {
                    if(i < t1)
                        val[i] = (byte)rand.Next();
                    else
                        val[i] = 0;

                    if(val[i] != 0)
                    {
                        // Console.WriteLine("done.");
                        done = true;
                    }
                }
            }

            while(val[0] == 0)
                val[0] = (byte)rand.Next();

            Console.Write("Round = " + count);

            // encrypt and decrypt data            
            BigInteger bi_data = new BigInteger(val);
            BigInteger bi_encrypted = bi_data.modPow(bi_e, bi_n);
            BigInteger bi_decrypted = bi_encrypted.modPow(bi_d, bi_n);

            // compare
            if(bi_decrypted != bi_data)
            {
                Console.WriteLine("\nError at round " + count);
                Console.WriteLine("{0}\n{1}\n", bi_data.ToString(), bi_decrypted.ToString());
                return;
            }
            Console.WriteLine(" <PASSED>.");
        }

    }


    //***********************************************************************
    // Tests the correct implementation of the modulo exponential and
    // inverse modulo functions using RSA encryption and decryption.  The two
    // pseudoprimes p and q are fixed, but the two RSA keys are generated
    // for each round of testing.
    //***********************************************************************

    public static void RSATest2(int rounds)
    {
        const int LEN = 128;
        Random rand = new Random();
        byte[] val = new byte[LEN];

        byte[] pseudoPrime1 = {
            0x85, 0x84, 0x64, 0xFD, 0x70, 0x6A, 0x9F, 0xF0, 0x94, 0x0C, 0x3E, 0x2C, 0x74, 0x34, 0x05, 0xC9, 
            0x55, 0xB3, 0x85, 0x32, 0x98, 0x71, 0xF9, 0x41, 0x21, 0x5F, 0x02, 0x9E, 0xEA, 0x56, 0x8D, 0x8C, 
            0x44, 0xCC, 0xEE, 0xEE, 0x3D, 0x2C, 0x9D, 0x2C, 0x12, 0x41, 0x1E, 0xF1, 0xC5, 0x32, 0xC3, 0xAA,
            0x31, 0x4A, 0x52, 0xD8, 0xE8, 0xAF, 0x42, 0xF4, 0x72, 0xA1, 0x2A, 0x0D, 0x97, 0xB1, 0x31, 0xB3,
        };

        byte[] pseudoPrime2 = {
            0x99, 0x98, 0xCA, 0xB8, 0x5E, 0xD7, 0xE5, 0xDC, 0x28, 0x5C, 0x6F, 0x0E, 0x15, 0x09, 0x59, 0x6E, 
            0x84, 0xF3, 0x81, 0xCD, 0xDE, 0x42, 0xDC, 0x93, 0xC2, 0x7A, 0x62, 0xAC, 0x6C, 0xAF, 0xDE, 0x74, 
            0xE3, 0xCB, 0x60, 0x20, 0x38, 0x9C, 0x21, 0xC3, 0xDC, 0xC8, 0xA2, 0x4D, 0xC6, 0x2A, 0x35, 0x7F,
            0xF3, 0xA9, 0xE8, 0x1D, 0x7B, 0x2C, 0x78, 0xFA, 0xB8, 0x02, 0x55, 0x80, 0x9B, 0xC2, 0xA5, 0xCB,
        };


        BigInteger bi_p = new BigInteger(pseudoPrime1);
        BigInteger bi_q = new BigInteger(pseudoPrime2);
        BigInteger bi_pq = (bi_p-1)*(bi_q-1);
        BigInteger bi_n = bi_p * bi_q;

        for(int count = 0; count < rounds; count++)
        {
            // generate private and public key
            BigInteger bi_e = bi_pq.genCoPrime(1024, rand);
            BigInteger bi_d = bi_e.modInverse(bi_pq);

            // Console.WriteLine("\ne =\n" + bi_e.ToString(10));
            // Console.WriteLine("\nd =\n" + bi_d.ToString(10));
            // Console.WriteLine("\nn =\n" + bi_n.ToString(10) + "\n");

            // generate data of random length
            int t1 = 0;
            while(t1 == 0)
                t1 = (int)(rand.Next() % LEN);

            bool done = false;
            while(!done)
            {
                for(int i = 0; i < 64; i++)
                {
                    if(i < t1)
                        val[i] = (byte)rand.Next();
                    else
                        val[i] = 0;

                    if(val[i] != 0)
                        done = true;
                }
            }

            while(val[0] == 0)
                val[0] = (byte)rand.Next();

            Console.Write("Round = " + count);

            // encrypt and decrypt data
            BigInteger bi_data = new BigInteger(val, t1);
            BigInteger bi_encrypted = bi_data.modPow(bi_e, bi_n);
            BigInteger bi_decrypted = bi_encrypted.modPow(bi_d, bi_n);

            // compare
            if(bi_decrypted != bi_data)
            {
                Console.WriteLine("\nError at round " + count);
                Console.WriteLine(bi_data + "\n");
                return;
            }
            Console.WriteLine(" <PASSED>.");
        }

    }


    //***********************************************************************
    // Tests the correct implementation of sqrt() method.
    //***********************************************************************

    public static void SqrtTest(int rounds)
    {
        Random rand = new Random();
        for(int count = 0; count < rounds; count++)
        {
            // generate data of random length
            int t1 = 0;
            while(t1 == 0)
                t1 = (int)(rand.Next() & 0x3ff);

            Console.Write("Round = {0,8}\trandBits = {1}\t", count, t1);

            BigInteger a = new BigInteger();
            a.genRandomBits(t1, rand);

            BigInteger b = a.sqrt();
            BigInteger c = (b+1)*(b+1);

            // check that b is the largest integer such that b*b <= a
            if (c <= a)
            {
                Console.WriteLine("\nError at round " + count);
                Console.WriteLine(a + "\n");
                return;
            }
            Console.WriteLine(" <PASSED>.");
        }
    }


    public static void SqrtTest2(int rounds)
    {
        BigInteger BigTwo = new BigInteger(256);
        
        for (int i = 0; i < 500; i++)
            BigTwo *= 100;
        
        Console.WriteLine(BigTwo.sqrt());
        
        for(int count = 0; count < rounds; count++)
        {
            BigInteger y = new BigInteger(count + 1);
            for (int i = 0; i < 250; i++)
                y *= 10000;

            BigInteger x = y.sqrt();
            BigInteger z = (x+1) * (x+1);

            Console.Write("Round: {0}", count);

            // check that x is the largest integer such that x*x <= z
            if (z <= y)
            {
                Console.WriteLine("\nError at round " + count);
                Console.WriteLine(x + "\n");
                return;
            }
            // Console.WriteLine("\nz = {0}\nx = {1}", z, x);
            Console.WriteLine(" <PASSED>.");
        }
    }


    public static string ByteArrayStruct(byte[] byte_array)
    {
        // Output byte [] structure suitable (little-endian) for RSABigInt/Program.cs
        string str_bytes = "new byte [] {\n    ";
        int c = 0;
        foreach (byte b in byte_array)
        {
            str_bytes += String.Format($"0x{b:x2}, ");
            c++;
            if (c % 8 == 0)
                str_bytes += "\n    ";
        }
        str_bytes += "0x00 };";

        return str_bytes;
    }

    public static void Print_Jacobi_Table(int a, int n)
    {
        Console.WriteLine("\nJacobi symbol for a|p");
        Console.Write("  a ");
        for (int i = 1; i <= a; i++)
            Console.Write($"{i,4}");
        Console.Write("p   " + new String('-', 116));
        
        for (int j = 0; j < n; j++)
        {
            Console.Write($"{primesBelow2000[j],3} ");
            for (uint i = 1; i <= a; i++)
                Console.Write( "{0,4}", Jacobi(new BigInteger(i), new BigInteger(primesBelow2000[j])) );
        }
        Console.WriteLine("\n");
    }

	// The following pseudoPrimes are all in big-endian format
    private static byte[] pseudoPrime1 => 
        new byte[] { 
            0x00,
            0x85, 0x84, 0x64, 0xFD, 0x70, 0x6A, 0x9F, 0xF0, 0x94, 0x0C, 0x3E, 0x2C, 0x74, 0x34, 0x05, 0xC9, 
            0x55, 0xB3, 0x85, 0x32, 0x98, 0x71, 0xF9, 0x41, 0x21, 0x5F, 0x02, 0x9E, 0xEA, 0x56, 0x8D, 0x8C, 
            0x44, 0xCC, 0xEE, 0xEE, 0x3D, 0x2C, 0x9D, 0x2C, 0x12, 0x41, 0x1E, 0xF1, 0xC5, 0x32, 0xC3, 0xAA,
            0x31, 0x4A, 0x52, 0xD8, 0xE8, 0xAF, 0x42, 0xF4, 0x72, 0xA1, 0x2A, 0x0D, 0x97, 0xB1, 0x31, 0xB3
        };
    
    private static byte[] pseudoPrime2 =>
        new byte[] { 
            0x00,
            0x99, 0x98, 0xCA, 0xB8, 0x5E, 0xD7, 0xE5, 0xDC, 0x28, 0x5C, 0x6F, 0x0E, 0x15, 0x09, 0x59, 0x6E, 
            0x84, 0xF3, 0x81, 0xCD, 0xDE, 0x42, 0xDC, 0x93, 0xC2, 0x7A, 0x62, 0xAC, 0x6C, 0xAF, 0xDE, 0x74, 
            0xE3, 0xCB, 0x60, 0x20, 0x38, 0x9C, 0x21, 0xC3, 0xDC, 0xC8, 0xA2, 0x4D, 0xC6, 0x2A, 0x35, 0x7F,
            0xF3, 0xA9, 0xE8, 0x1D, 0x7B, 0x2C, 0x78, 0xFA, 0xB8, 0x02, 0x55, 0x80, 0x9B, 0xC2, 0xA5, 0xCB
        };
    
    private static byte[] pseudoPrime3 =>
        new byte[] { 
            0x00, 
            0x6c, 0xbd, 0xd3, 0x6a, 0xf4, 0x9c, 0xc9, 0x9b, 0xab, 0xd0, 0x98, 0xfa, 0x36, 0x06, 0xae, 0xdf, 
            0x87, 0x1e, 0x71, 0x6f, 0x4a, 0xe1, 0xee, 0xf7, 0xda, 0x68, 0xd5, 0x04, 0xf3, 0xb9, 0xb1, 0x82, 
            0x84, 0x60, 0xf6, 0x35, 0x90, 0x72, 0x07, 0xd7, 0x68, 0x77, 0xe5, 0xd9, 0xe0, 0x3a, 0x8f, 0xf0, 
            0x20, 0x00, 0xf3, 0x23, 0x58, 0x8f, 0x3c, 0xb3, 0x91, 0x3a, 0xb2, 0x7f
        };
    
    private static byte[] pseudoPrime4 =>
        new byte[] { 
            0x00, 
            0x53, 0xc0, 0xcd, 0x2a, 0x75, 0xce, 0xd0, 0x84, 0xb1, 0x11, 0x9f, 0xa8, 0xa5, 0x5a, 0xe3, 0x3c, 
            0x21, 0x95, 0xbd, 0xd1, 0x51, 0x40, 0x50, 0x89, 0x94, 0x79, 0x8d, 0x8e, 0xef, 0x13, 0xad, 0xaa, 
            0xa7, 0xaa, 0xb4, 0x55, 0x45, 0xe5, 0xa9, 0xbd, 0xae, 0x6f, 0x17, 0x57, 0x8e, 0xd0, 0xc8, 0xbe, 
            0x49, 0xe6, 0x7d, 0xab, 0x54, 0x8d, 0x7f, 0xf0, 0xaf, 0x6a, 0x64, 0x0d 
        };
    
    private static byte[] pseudoPrime5 =>
        new byte[] { 
            0x00,
            0x5b, 0x15, 0x92, 0x27, 0x43, 0xfd, 0x2b, 0x31, 0x88, 0x1a, 0x34, 0x99, 0xf6, 0x88, 0xc6, 0xcc, 
            0xea, 0xd3, 0x0a, 0x23, 0x79, 0x54, 0xa2, 0x38, 0xae, 0x33, 0x69, 0x01, 0xc1, 0x38, 0xfb, 0xd2, 
            0xe7, 0xdc, 0xb3, 0x49, 0x52, 0x39, 0x84, 0xa2, 0x56, 0x03, 0x4e, 0x95, 0x3f, 0x21, 0xbd, 0xd7, 
            0x79, 0x30, 0xfa, 0xb2, 0x8a, 0xe5, 0x14, 0x18, 0x2f, 0x17, 0xa7, 0x7f 
        };

    private static byte[] pseudoPrime6 =>
        new byte[] { 
            0x00,
            0x03, 0x14, 0x8b, 0x80, 0xa7, 0xf8, 0xf4, 0x73, 0x9f, 0x07, 0x07, 0xf7, 0x54, 0xf5, 0xf5, 0x5f, 
            0x65, 0x5d, 0xe9, 0x83, 0x3c, 0x79, 0x8c, 0xcb, 0xde, 0xfb, 0x99, 0x66, 0x10, 0x7f, 0x37, 0xa9, 
            0x07, 0x21, 0x58, 0xa9, 0x33, 0x04, 0x06, 0xa7, 0xdc, 0x7a, 0xcd, 0xb2, 0xce, 0xb0, 0xa9, 0xcf, 
            0x0e, 0x49, 0xed, 0x2e, 0x5f, 0x4d, 0x3d, 0x9e, 0xb3, 0xb9, 0x18, 0x9f  
        };
    
	private static byte[] pseudoPrime7 =>
		new byte [] {
			0x90, 0x97, 0x7c, 0xab, 0x61, 0xb2, 0xa5, 0xd1, 0x17, 0x83, 0xe2, 0x9d, 0x04, 0xe2, 0xf8, 0xff, 
			0x59, 0x88, 0x3d, 0x45, 0x1f, 0x75, 0x4e, 0xd6, 0x4a, 0x31, 0x40, 0x51, 0x52, 0x10, 0x47, 0xd3, 
			0x5d, 0x39, 0xf5, 0xfd, 0x29, 0x6b, 0xdd, 0xf3, 0x35, 0xbe, 0x46, 0x0a, 0x28, 0x61, 0xa5, 0x91, 
			0x51, 0xab, 0x59, 0xc6, 0x64, 0x69, 0x87, 0x4f, 0x32, 0x46, 0xf2, 0x48, 0x44, 0x4d, 0x8a, 0xed, 
			0x5d, 0xe6, 0x07, 0x13, 0x09, 0xf4, 0xc4, 0x81, 0x3e, 0x8b, 0x60, 0x35, 0x55, 0xe7, 0x9c, 0xe5, 
			0x74, 0x2d, 0x2a, 0x7d, 0x56, 0x86, 0x01, 0x81, 0x70, 0x1a, 0xf3, 0x6f, 0x6f, 0x75, 0x59, 0x9d, 
			0x17, 0xe8, 0x47, 0xdb, 0x11, 0x55, 0x33, 0x9b, 0x71, 0xb2, 0x87, 0x83, 0x7b, 0x73, 0xf1, 0xe6, 
			0x52, 0xeb, 0xb6, 0x82, 0x16, 0xf1, 0xa0, 0xab, 0x6d, 0xaf, 0xd1, 0x91, 0x40, 0xa7, 0xfa, 0x15, 
			0x19, 0x7e, 0x24, 0xe7, 0x08, 0x87, 0xb0, 0xc7, 0x0e, 0xad, 0x7a, 0x09, 0x45, 0xe4, 0x67, 0x39, 
			0x48, 0x2c, 0x28, 0x11, 0x19, 0x8c, 0x0e, 0x59, 0x7c, 0x7e, 0xf9, 0x85, 0x5b, 0x3e, 0x0e, 0x82, 
			0x7d, 0x91, 0xcc, 0x3b, 0x24, 0xd5, 0xa5, 0x5c, 0x57, 0x1a, 0xc0, 0x26, 0x25, 0x83, 0x42, 0x29, 
			0x21, 0x37, 0xcb, 0x91, 0x5d, 0x25, 0x4a, 0xdb, 0x79, 0x39, 0xb4, 0x24, 0x3f, 0x5c, 0x7d, 0x1d, 
			0x56, 0xd8, 0x21, 0x85, 0x01, 0x4f, 0xb8, 0x6c, 0x5f, 0xc3, 0xc6, 0x45, 0x72, 0x69, 0x26, 0xef, 
			0x40, 0x43, 0x98, 0xf9, 0x23, 0x04, 0xd8, 0x47, 0x32, 0x19, 0x05, 0x35, 0x33, 0x2e, 0x7f, 0xb5, 
			0x24, 0xad, 0xcd, 0x75, 0x0b, 0x7f, 0x44, 0x77, 0x06, 0xc9, 0x5c, 0x53, 0x03, 0x13, 0x39, 0xcd, 
			0x5a, 0xe9, 0x44, 0x37, 0x26, 0xad, 0xbf, 0xc9, 0x26, 0xb6, 0x7c, 0xe6, 0x56, 0xff, 0x1d, 0x58, 
			0x32, 0x54, 0x2f, 0x25, 0x4d, 0xdd, 0xf5, 0x69, 0x2b, 0x5e, 0x5d, 0x71, 0x78, 0xc8, 0x4f, 0x41, 
			0x7f, 0x25, 0xa7, 0x8d, 0x0a, 0x6f, 0x07, 0xf7, 0x78, 0x99, 0x8d, 0x26, 0x4e, 0xb9, 0x2e, 0xe7, 
			0x19, 0xb8, 0xdf, 0x81, 0x0a, 0x97, 0xc7, 0xa1, 0x09, 0x44, 0xc5, 0x4d, 0x59, 0x5e, 0x4c, 0x65, 
			0x02, 0xb2, 0x74, 0x42, 0x4b, 0x44, 0xdd, 0xe8, 0x76, 0x18, 0x36, 0x8d, 0x68, 0x5c, 0xee, 0xe7, 
			0x42, 0x9d, 0xed, 0xb3, 0x17, 0xa2, 0x50, 0x0d, 0x64, 0x91, 0x48, 0xd3, 0x33, 0xec, 0xc9, 0x1e, 
			0x60, 0x64, 0x26, 0x4d, 0x1e, 0xbc, 0xfa, 0xee, 0x3c, 0x36, 0x32, 0x87, 0x47, 0x7b, 0xf3, 0x2f, 
			0x40, 0xd7, 0x8c, 0x95, 0x72, 0xa5, 0x6f, 0x75, 0x50, 0xda, 0xa1, 0x21, 0x06, 0xcf, 0x65, 0xc6, 
			0x4c, 0x77, 0x47, 0xab, 0x7c, 0x94, 0xd8, 0x71, 0x2f, 0x31, 0x71, 0x0b, 0x1d, 0xd5, 0x1a, 0x77, 
			0x0b, 0xc6, 0xaa, 0x68, 0x5f, 0xf3, 0xa8, 0x01, 0x26, 0x9a, 0xa8, 0x7f, 0x64, 0xab, 0x81, 0xb0, 
			0x12, 0xef, 0x33, 0x8f, 0x69, 0xd5, 0xdc, 0x89, 0x3b, 0xa7, 0x2b, 0x73, 0x57, 0x25, 0x55, 0x2b, 
			0x0b, 0xf9, 0x3c, 0x6b, 0x31, 0xb8, 0x8b, 0x17, 0x0f, 0x71, 0x70, 0xfa, 0x11, 0x57, 0xdb, 0x6c, 
			0x5b, 0xb3, 0xe5, 0xc4, 0x10, 0x62, 0xef, 0x99, 0x59, 0x45, 0xae, 0x2d, 0x0a, 0xb7, 0xfd, 0xd5, 
			0x24, 0xd8, 0x34, 0xd7, 0x05, 0xfd, 0x3c, 0xca, 0x05, 0x0c, 0xfa, 0xc2, 0x20, 0xa4, 0x71, 0x13, 
			0x30, 0xb3, 0x47, 0x5b, 0x03, 0x37, 0x05, 0x1c, 0x3c, 0x2e, 0xa0, 0xc0, 0x08, 0x33, 0x77, 0xbe, 
			0x7b, 0xac, 0x08, 0xbf, 0x26, 0x27, 0x23, 0x3b, 0x23, 0x66, 0x78, 0x67, 0x78, 0x46, 0xc8, 0xdc, 
			0x5a, 0x00, 0xc2, 0x2f, 0x13, 0x8a, 0xaf, 0xf1, 0x02, 0x20, 0xd3, 0x28, 0x0e, 0x75, 0x2f, 0x67,
		};

	private static List<int> pseudoPrimes =>
		new List<int>() {
			1941454657, 
			1942608529, 
			1943951041, 
			1944125633, 
			1947867361, 
			1949584501, 
			1949646601, 
			1950276565, 
			1951469101, 
			1952968753, 
			1953988345, 
			1954174465, 
			1955324449, 
			1958102641, 
			1958613601, 
			1962804565, 
			1966146451, 
			1970097001, 
			1973398141, 
			1974420721, 
			1976295241, 
			1977147019, 
			1977257441, 
			1984089601, 
			1984685185, 
			1988071801, 
			1989985453, 
			1991063449, 
			1992841201, 
			1995830761, 
			1999004365, 
			1999743661
		};
		
    public static void Main(string[] args)
    {
        // Known problem -> these two pseudoprimes passes my implementation of
        // primality test but failed in JDK's isProbablePrime test.

        
        Console.Write("List of primes < 1000\n" + new String('-', 120));
        int limit = 89, count = 0;
        for(uint i = 0; i < 1000; i++)
        {
            // if( (count+1) % 20 == 0)
            if (i >= limit)
            {
                Console.WriteLine();
                limit += 101;
            }

            BigInteger p = new BigInteger(i);

            if(p.isProbablePrime())
            // if(p.LucasStrongTest())
            // if(p.FermatLittleTest(confidence))
            {
                if (i < 1000)
                    Console.Write("{0,3:G}, ", i);
                else
                    Console.Write("{0,4:G}, ", i);
                // Console.WriteLine("\n{0}", p.Pow(10));
                // Console.ReadLine();
                count++;
            }
        }
        Console.WriteLine("\nCount = {0}\n{1}", count, new String('-', 120) );
        
        // BigInteger c = new BigInteger(5777);        // false LucasStrongTest prime!
        // Console.WriteLine("LucasStrongTest: {0}", c.LucasStrongTest());
        
        //
        System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
        int dwStart = Environment.TickCount;
        
        sw.Start();
        
		foreach (var pseudoPrime in pseudoPrimes)
		{
			BigInteger bigInt1 = new BigInteger(pseudoPrime);
			{
				Console.WriteLine("\n\nPrimality testing for: \t" + bigInt1.ToString() + "\n");
				Console.WriteLine("SolovayStrassenTest({0})  {1}", confidence, bigInt1.SolovayStrassenTest(confidence));
				Console.WriteLine("RabinMillerTest({0}) \t {1}", confidence, bigInt1.RabinMillerTest(confidence));
				Console.WriteLine("FermatLittleTest({0}) \t {1}", confidence, bigInt1.FermatLittleTest(confidence));
				Console.WriteLine("LucasStrongTest() \t {0}", bigInt1.LucasStrongTest());
				Console.WriteLine("isProbablePrime() \t {0}\n", bigInt1.isProbablePrime());
			}
		}
        
        int randPrimeBits = 2048;
        Console.Write("\n\nGenerating {0}-bits random pseudoprime.", randPrimeBits);
        Random rand = new Random();
        BigInteger prime = BigInteger.genPseudoPrime(randPrimeBits, confidence, rand);
        Console.WriteLine("\nprime.GetHashCode(): 0x{0:X8}", prime.GetHashCode());
        Console.WriteLine("\n" + ByteArrayStruct(prime.ToByteArray()) + "\n");
               
        
        //Print_Jacobi_Table(29, 31);
        
        // Console.WriteLine("\nModInverseTest(ROUNDS={0})", ROUNDS);
        // BigInteger.ModInverseTest(ROUNDS);
        
        Console.WriteLine("\nPowTest(ROUNDS={0})", ROUNDS);
        BigInteger.PowTest(ROUNDS);
        
        // Console.WriteLine("\nSqrtTest(ROUNDS={0})", ROUNDS);
        // BigInteger.SqrtTest(ROUNDS);
		
        // Console.WriteLine("\nSqrtTest2(ROUNDS={0})", ROUNDS);
        // BigInteger.SqrtTest2(ROUNDS);

        // Console.WriteLine("\nMulDivTest(ROUNDS={0})", ROUNDS);
        // BigInteger.MulDivTest(ROUNDS);

        // Console.WriteLine("\nRSATest(ROUNDS={0})", ROUNDS);
        // BigInteger.RSATest(ROUNDS);

        // Console.WriteLine("\nRSATest2(ROUNDS={0})", ROUNDS);
        // BigInteger.RSATest2(ROUNDS);
        
        sw.Stop();
        Console.WriteLine("\nTickCount {0} secs.", (float)(Environment.TickCount - dwStart) / 1000.0f);
        Console.WriteLine("Stopwatch {0:F2} secs.", sw.Elapsed.TotalSeconds);
    }

}
