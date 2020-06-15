using System;
using System.Runtime.InteropServices;

public class FloatingPointRepresentation
{
    [StructLayout(LayoutKind.Explicit, Size = 8)]
    public unsafe struct data_representation
    {
        [FieldOffset(0)]
        public float f;
        
        [FieldOffset(0)]
        public uint i;
        
        [FieldOffset(0)]
        public double d;
        
        [FieldOffset(0)]
        public ulong l;
    }
    
    public static void Main()
    {
        Console.WriteLine("sizeof(float):  {0}\nsizeof(double): {1}\nsizeof(uint):   {2}\nsizeof(ulong):  {3}\n", sizeof(float), sizeof(double), sizeof(uint), sizeof(ulong));
        data_representation dr;
        dr.i = 0;
        dr.f = 0.1f;
        
        dr.i = 0x7ff00000;
        dr.f = float.MaxValue;
        
        Console.WriteLine("float value:  {0}", float.IsNaN(dr.f) ? float.NaN : dr.f);
        Console.WriteLine("IEEE 754:     {0:X}", dr.i);

        dr.f = float.Epsilon;
        Console.WriteLine("\nfloat.Epsilon:  {0}", dr.f);
        Console.WriteLine("IEEE 754:     {0:X}", dr.i);

        dr.l = 0;
        dr.d = 0.03125d;

        Console.WriteLine("\ndouble value: {0}", dr.d);
        Console.WriteLine("hexadecimal:  {0:X}", dr.l);

        dr.d = 123456789012.34567;
        Double additional = Double.Epsilon * 1e305;
        Console.WriteLine("\n   {0}\n + {1}\n   ----------------\n = {2}", dr.d, additional,
                                               dr.d + additional);
        Console.WriteLine("\nhexadecimal:      {0:X}", dr.l);
        dr.d = additional;
        Console.WriteLine("epsilon * 1e305:  {0:X}", dr.l);
        dr.d += 123456789012.34567;
        Console.WriteLine("\nhexadecimal:      {0:X}", dr.l);
    }
}
