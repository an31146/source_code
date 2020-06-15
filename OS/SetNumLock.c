#include <stdio.h>
#include <windows.h>
#include <sas.h>

void SetNumLock( BOOL bState )
{
    BYTE keyState[256];

    GetKeyboardState((LPBYTE)&keyState);
    if( (bState && !(keyState[VK_NUMLOCK] & 1)) ||
        (!bState && (keyState[VK_NUMLOCK] & 1)) )
    {
    // Simulate a key press
     keybd_event( VK_NUMLOCK,
                  0x45,
                  KEYEVENTF_EXTENDEDKEY | 0,
                  0 );

    // Simulate a key release
     keybd_event( VK_NUMLOCK,
                  0x45,
                  KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                  0);
    }
}

void SetCapsLock( BOOL bState )
{
    BYTE keyState[256];

    GetKeyboardState((LPBYTE)&keyState);
    if( (bState && !(keyState[VK_CAPITAL] & 1)) ||
        (!bState && (keyState[VK_CAPITAL] & 1)) )
    {
    // Simulate a key press
     keybd_event( VK_CAPITAL,
                  0x45,
                  KEYEVENTF_EXTENDEDKEY | 0,
                  0 );

    // Simulate a key release
     keybd_event( VK_CAPITAL,
                  0x45,
                  KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                  0);
    }
}

void SendCtrlAltDel()
{
    keybd_event( VK_CONTROL,
                  0x1D, KEYEVENTF_EXTENDEDKEY, 0 );
    // keybd_event( VK_LSHIFT,
    keybd_event( VK_MENU,
                  0x38, KEYEVENTF_EXTENDEDKEY, 0 );
    // Sleep(10);
    keybd_event( VK_DELETE,
                  0x53, 0, 0 );
    // keybd_event( 'L',
                  // 0x4C, 0, 0 );
    
    Sleep(10);              
    
    keybd_event( VK_CONTROL,
                  0x1D, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
    // keybd_event( VK_LSHIFT,
    keybd_event( VK_MENU,
                  0x38, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
    // keybd_event( 'L',
                  // 0x4C, KEYEVENTF_KEYUP, 0 );
    keybd_event( VK_DELETE,
                  0x53, KEYEVENTF_KEYUP, 0 );
}

int main(int argc, char *argv[])
{
    int dwFlag = 0;
    
    if (argc == 2)
        dwFlag = atoi(argv[1]);
    
    if ( (dwFlag & 1) == 1)
        SetNumLock( TRUE );
    else 
        SetNumLock( FALSE );
    
    if ( (dwFlag & 2) == 2)
        SetCapsLock( TRUE );
    else 
        SetCapsLock( FALSE );
    
    printf("%d\n", MapVirtualKeyA(VK_CONTROL, MAPVK_VK_TO_VSC));
    printf("%d\n", MapVirtualKeyA(VK_MENU, MAPVK_VK_TO_VSC));
    printf("%d\n", MapVirtualKeyA(VK_DELETE, MAPVK_VK_TO_VSC));
    
    
    if ( (dwFlag & 4) == 4)
        SendSAS( TRUE );
    
    return EXIT_SUCCESS;
}
