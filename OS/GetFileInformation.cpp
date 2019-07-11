#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

//declare a BY_HANDLE_FILE_INFORMATION structure
BY_HANDLE_FILE_INFORMATION fileinfo;

int main()
{
    // clear everything in the structure, this is optional
    ZeroMemory(&fileinfo, sizeof(BY_HANDLE_FILE_INFORMATION));

    // obtain a handle to the file, in this case the file
    // must be in the same directory as your application
    HANDLE myfile = NULL;
    myfile = CreateFileA("listener.exe",0x00,0x00,NULL,
                         OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    FILETIME ft;
    SYSTEMTIME st;
    BOOL f;

    SYSTEMTIME stUTC, stLocal;
    DWORD dwRet, dwSize;

    TCHAR lpszString[MAX_PATH];

    // if we managed to obtain the desired handle
    if(myfile!=INVALID_HANDLE_VALUE)
    {
        //try to fill the structure with info regarding the file
        if(GetFileInformationByHandle(myfile, &fileinfo))
        {
            // Ex: show the serial number of the volume the file belongs to
            cout << endl << "Volume Serial: " << hex << fileinfo.dwVolumeSerialNumber << endl;

            FileTimeToSystemTime(&fileinfo.ftLastWriteTime, &stUTC);
            SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
        
            // Build a string showing the date and time.
            dwRet = sprintf(lpszString,
                TEXT("%02d/%02d/%d  %02d:%02d:%02d"),
                stLocal.wDay, stLocal.wMonth, stLocal.wYear,
                stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
        
            cout << endl << "Last Modified: " << lpszString << endl;
        }
        // you should close the handle once finished
        CloseHandle(myfile);
    }
    system("pause");
    return 0;
}
