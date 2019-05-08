///////////////////////////////////////////////////////////////////////////////
//
// CrashDemo.cpp 
//
// Author: Oleg Starodumov (www.debuginfo.com)
//
// This application simulates an unhandled exception and creates a minidump, 
// with the help of a custom filter for unhandled exceptions 
//
//


///////////////////////////////////////////////////////////////////////////////
// Include files 
//

#include <windows.h>
#include <tchar.h>
#include <dbghelp.h>

#include <process.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
// Directives
//

// Link with DbgHelp.lib 
#pragma comment ( lib, "dbghelp.lib" )


///////////////////////////////////////////////////////////////////////////////
// Helper macros 
//

// _beginthreadex wrapper macro (source: J.Richter, "Advanced Windows")

typedef unsigned (__stdcall *PTHREAD_START) (void*);

#define BEGINTHREADEX(lpsa, cbStack, lpStartAddr,\
	lpvThreadParm, fdwCreate, lpIDThread)          \
		((HANDLE)_beginthreadex(                     \
				(void*)(lpsa),                           \
				(unsigned)(cbStack),                     \
				(PTHREAD_START)(lpStartAddr),            \
				(void*)(lpvThreadParm),                  \
				(unsigned)(fdwCreate),                   \
				(unsigned*)(lpIDThread)))


///////////////////////////////////////////////////////////////////////////////
// Function declarations 
//

DWORD WINAPI WorkerThread( LPVOID lpParam );
void TestFunc( int* pParam );

LONG __stdcall MyCustomFilter( EXCEPTION_POINTERS* pep ); 
void CreateMiniDump( EXCEPTION_POINTERS* pep ); 


///////////////////////////////////////////////////////////////////////////////
// main() 
//

int _tmain( int argc, TCHAR* argv[] )
{
	// Register the custom filter for unhandled exceptions 

	SetUnhandledExceptionFilter( MyCustomFilter ); 


	// Start the worker thread 

	_tprintf( _T("Starting the worker thread...\n") );

	HANDLE  hThread   = NULL;
	DWORD   ThreadId  = 0;

	hThread = BEGINTHREADEX(0, 0, WorkerThread, 0, 0, &ThreadId );

	if( hThread == NULL )
	{
		_tprintf( _T("Cannot start thread. Error: %u\n"), GetLastError() );
		return 0;
	}

	
	// Worker thread started 

	_tprintf( _T("Worker thread started.\n") );

	Sleep( 60 * 60 * 1000 );


	// Complete 

	_tprintf( _T("Test complete.\n") );

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// Test functions 
//

DWORD WINAPI WorkerThread( LPVOID lpParam )
{
	_tprintf( _T("Worker thread [%u] started.\n"), GetCurrentThreadId() );

	Sleep( 10 * 1000 );

	int* TempPtr = (int*)lpParam;

	TestFunc( TempPtr );

	return 0;
}

void TestFunc( int* pParam )
{
	_tprintf( _T("TestFunc()\n") );

	*pParam = 0;
}


///////////////////////////////////////////////////////////////////////////////
// Custom filter for unhandled exceptions and its helper function 
//

LONG __stdcall MyCustomFilter( EXCEPTION_POINTERS* pep ) 
{
	CreateMiniDump( pep ); 

	return EXCEPTION_EXECUTE_HANDLER; 
}

void CreateMiniDump( EXCEPTION_POINTERS* pep ) 
{
	// Open the file 

	HANDLE hFile = CreateFile( _T("MiniDump.dmp"), GENERIC_READ | GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); 

	if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
	{
		// Create the minidump 

		MINIDUMP_EXCEPTION_INFORMATION mdei; 

		mdei.ThreadId           = GetCurrentThreadId(); 
		mdei.ExceptionPointers  = pep; 
		mdei.ClientPointers     = FALSE; 

		//https://msdn.microsoft.com/en-us/library/windows/desktop/ms680519(v=vs.85).aspx
		MINIDUMP_TYPE mdt       = MiniDumpWithThreadInfo;                                   //MiniDumpNormal; 

		BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
			hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0 ); 

		if( !rv ) 
			_tprintf( _T("MiniDumpWriteDump failed. Error: %u \n"), GetLastError() ); 
		else 
			_tprintf( _T("Minidump created.\n") ); 

		// Close the file 

		CloseHandle( hFile ); 

	}
	else 
	{
		_tprintf( _T("CreateFile failed. Error: %u \n"), GetLastError() ); 
	}

}


