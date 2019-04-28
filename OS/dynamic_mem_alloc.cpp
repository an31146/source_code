// A short program to demonstrate dynamic memory allocation 
// using a structured exception handler. 

#include <windows.h>
#include <tchar.h>
#include <stdio.h>              // For printf
#include <stdlib.h>             // For exit

#define PAGELIMIT 294216        // Number of pages to ask for

LPTSTR lpNxtPage;               // Address of the next page to ask for
DWORD dwPages = 0;              // Count of pages gotten so far
DWORD dwPageSize;               // Page size on this computer

INT PageFaultExceptionFilter(DWORD dwCode)
{
	LPVOID lpvResult;

	// If the exception is not a page fault, exit.

	if (dwCode != EXCEPTION_ACCESS_VIOLATION)
    {
		printf("Exception code = %u.\n", dwCode);
		return EXCEPTION_EXECUTE_HANDLER;
    }

	printf("Exception is a page fault.\n");

	// If the reserved pages are used up, exit.

	if (dwPages >= PAGELIMIT)
    {
		printf("Exception: out of pages.\n");
		return EXCEPTION_EXECUTE_HANDLER;
    }

	// Otherwise, commit another page.

	lpvResult = VirtualAlloc(
                     (LPVOID) lpNxtPage, // Next page to commit
                     dwPageSize,         // Page size, in bytes
                     MEM_COMMIT,         // Allocate a committed page
                     PAGE_READWRITE);    // Read/write access
	if (lpvResult == NULL )
    {
		printf("VirtualAlloc failed.\n");
		return EXCEPTION_EXECUTE_HANDLER;
    } 
	else 
	{
		printf ("Allocating another page.\n");
    }

	// Increment the page count, and advance lpNxtPage to the next page.

	dwPages++;
	lpNxtPage = (LPTSTR) ((PCHAR) lpNxtPage + dwPageSize);


	// Continue execution where the page fault occurred.

	return EXCEPTION_CONTINUE_EXECUTION;
}

VOID ErrorExit(LPTSTR lpMsg)
{
	printf ("Error! %s with error code of %ld.\n",
			lpMsg, GetLastError ());
	exit (0);
}

int main(VOID)
{
	LPVOID lpvBase1, lpvBase2;    // Base address of the test memory
	LPTSTR lpPtr1, lpPtr2;        // Generic character pointer
	BOOL bSuccess;                // Flag
	DWORD i;                      // Generic counter
	SYSTEM_INFO sSysInfo;         // Useful information about the system
	char ch;

	GetSystemInfo(&sSysInfo);     // Initialize the structure.

	printf ("This computer has page size: %u bytes.\n", sSysInfo.dwPageSize);

	dwPageSize = sSysInfo.dwPageSize;

	// Reserve pages in the virtual address space of the process.
	lpvBase1 = VirtualAlloc(
                     NULL,                 // System selects address
                     PAGELIMIT*dwPageSize, // Size of allocation
                     MEM_COMMIT | MEM_RESERVE,          // Allocate and Commit reserved pages
                     PAGE_READWRITE);       // Protection = Read/Write

	if (lpvBase1 == NULL )
		ErrorExit(TEXT("VirtualAlloc reserve failed."));

	lpPtr1 = lpNxtPage = (LPTSTR) lpvBase1;
   
	// Use structured exception handling when accessing the pages.
	// If a page fault occurs, the exception filter is executed to
	// commit another page from the reserved block of pages.

	for (i=0; i < PAGELIMIT*dwPageSize; i++)
    {
		try
        {
			// Write to memory.
            //printf("0x%08x\r", lpPtr+i);
			lpPtr1[i] = 'a';
        }
        
		// If there's a page fault, commit another page and try again.

		//catch ( PageFaultExceptionFilter( GetExceptionCode() )
		catch (int e )
        {
            printf("0x%08x\n", e);
            PageFaultExceptionFilter( e );
        }
        
    }

    printf("Press Escape to end loop.\n\nAccessing memory: 0x");
    for (;;)
    {
        i = (rand() << 12) + (rand() & 0x7ff);
        printf("%08x\b\b\b\b\b\b\b\b", lpPtr1+i);
        ch = lpPtr1[i];
        if ( GetAsyncKeyState(VK_ESCAPE) & 0x80 == 0x80 )
            break;
    }
    
    printf("\n\nPress Enter to release memory: ");
    ch = getchar();

	// Release the block of pages when you are finished using them.
    bSuccess = VirtualFree(
                       lpvBase1,       // Base address of block
                       0,             // Bytes of committed pages, must be zero when specifying MEM_RELEASE
                       MEM_RELEASE);  // Decommit the pages

    printf ("Release %s.\n", bSuccess ? "succeeded" : "failed" );

    return 0;
}
