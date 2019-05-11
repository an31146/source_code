#include <Windows.h>
#include <iostream>
#include <string.h>
using namespace std;

//declare a BY_HANDLE_FILE_INFORMATION structure
BY_HANDLE_FILE_INFORMATION fileinfo;

DWORD WINAPI ExportCallback(
            PBYTE pbData,
            PVOID pvCallbackContext,
            ULONG ulLength
);

WINBASEAPI DWORD WINAPI OpenEncryptedFileRawA(LPCSTR,ULONG,PVOID);
WINBASEAPI DWORD WINAPI OpenEncryptedFileRawW(LPCWSTR,ULONG,PVOID);

typedef DWORD(CALLBACK *PFE_EXPORT_FUNC)(PBYTE,PVOID,ULONG);   

WINBASEAPI DWORD WINAPI ReadEncryptedFileRawA(PFE_EXPORT_FUNC,PVOID,PVOID);
WINBASEAPI DWORD WINAPI ReadEncryptedFileRawW(PFE_EXPORT_FUNC,PVOID,PVOID);

#define OpenEncryptedFileRaw OpenEncryptedFileRawA
#define ReadEncryptedFileRaw ReadEncryptedFileRawA

int main()
{
    HANDLE hFile = NULL;
    
    PVOID pContext;
    PVOID pCallbackContext = NULL;
    
    // clear everything in the structure, this is optional
    ZeroMemory(&fileinfo, sizeof(BY_HANDLE_FILE_INFORMATION));

    OpenEncryptedFileRaw("C:\\Temp\\iManO2K - unprotected.zip", 1, &pContext);
    
    ReadEncryptedFileRaw(&ExportCallback, pCallbackContext, pContext);
    
    return 0;
}
