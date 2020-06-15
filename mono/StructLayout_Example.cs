using System;
using System.Runtime.InteropServices;

namespace InteropSample
{

    [StructLayout(LayoutKind.Explicit, Size=16, CharSet=CharSet.Ansi)]
    public class MySystemTime
    {
        [FieldOffset(0)]public ushort wYear;
        [FieldOffset(2)]public ushort wMonth;
        [FieldOffset(4)]public ushort wDayOfWeek;
        [FieldOffset(6)]public ushort wDay;
        [FieldOffset(8)]public ushort wHour;
        [FieldOffset(10)]public ushort wMinute;
        [FieldOffset(12)]public ushort wSecond;
        [FieldOffset(14)]public ushort wMilliseconds;
    }

    internal static class PInvoke
    {
        [DllImport("kernel32.dll")]
        internal static extern void GetSystemTime([MarshalAs(UnmanagedType.LPStruct)]MySystemTime st);

        // Import user32.dll (containing the function we need) and define
        // the method corresponding to the native function.
        [DllImport("user32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        internal static extern int MessageBox(IntPtr hWnd, string lpText, string lpCaption, uint uType);
        
        [DllImport("user32.dll")]
        internal static extern int SetCursorPos(int X,int Y);
        
        [DllImport("user32.dll")]
        internal static extern int LockWorkStation();
    };

    class TestApplication
    {
        public static void Main()
        {
            try
            {
                MySystemTime sysTime = new MySystemTime();
                PInvoke.GetSystemTime(sysTime);
                string strFormat = String.Format("The System (UTC) time is {0:00}/{1:00}/{2} {3:00}:{4:00}:{5:00}", sysTime.wDay,
                                                sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
                Random rnd = new Random();

                PInvoke.MessageBox(IntPtr.Zero, strFormat, "Attention!", 0);
                //PInvoke.LockWorkStation();
                while (sysTime.wSecond != 0)
                {
                    strFormat = String.Format("The System (UTC) time is {0:00}/{1:00}/{2} {3:00}:{4:00}:{5:00}", sysTime.wDay,
                    sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

                    Console.WriteLine(strFormat);
                    PInvoke.SetCursorPos(sysTime.wSecond * 15, sysTime.wMinute * rnd.Next(20));
                    PInvoke.GetSystemTime(sysTime);
                    System.Threading.Thread.Sleep(1000);
                }
            }
            catch(TypeLoadException e)
            {
                Console.WriteLine("TypeLoadException : " + e.Message);
            }
            catch(Exception e)
            {
                Console.WriteLine("Exception : " + e.Message);
            }
        }
    }
}
