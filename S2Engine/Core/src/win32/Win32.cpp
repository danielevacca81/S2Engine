// Win32.cpp
// 
#include "Win32.h"

#include <winsdkver.h>
#include <windows.h>
#include <shlwapi.h>
#include <mapi.h>
#include <psapi.h>

#include <sstream>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "shlwapi.lib")


// ------------------------------------------------------------------------------------------------
std::string Win32::productName( const std::string &fileName )
{
	const std::wstring wFileName(fileName.begin(),fileName.end() );

	// get info size
	DWORD handle;
	DWORD len = GetFileVersionInfoSize( wFileName.c_str() , &handle );
	if( !len )
		return "";

	// get info
	TCHAR *data  = new TCHAR[len];
	{
		BOOL  ok = GetFileVersionInfo( wFileName.c_str(), 0, len, data );
		if( !ok|| len==0 )
		{
			delete[] data;
			return "";
		}
	}

	
	// get version fields
	LPVOID prodName = NULL;
	UINT sz;
	{
		BOOL ok = VerQueryValue( data , TEXT("\\StringFileInfo\\040904b0\\ProductName") , &prodName, &sz );
		if( !ok|| sz==0 )
		{
			delete[] data;
			return "";
		}
	}

	const std::wstring wProductName( (LPCTSTR)prodName );
	const std::string productName( wProductName.begin(), wProductName.end() );
	delete [] data;

	return productName;
}

// ------------------------------------------------------------------------------------------------
std::string Win32::executableFileName()
{
	TCHAR fileName[MAX_PATH];

	// get executable filename
	if( !GetModuleFileName( 0, fileName, MAX_PATH ) )
		return "";

	const std::wstring wFilename(fileName);

	const std::string s(wFilename.begin(),wFilename.end() );

	return s;
}

// ------------------------------------------------------------------------------------------------
std::string Win32::fileVersion( const std::string &fileName )
{
	const std::wstring wFileName(fileName.begin(),fileName.end() );

	// get info size
	DWORD handle;
	DWORD len = GetFileVersionInfoSize( wFileName.c_str() , &handle );
	if( !len )
		return "";

	// get info
	TCHAR *data  = new TCHAR[len];
	{
		BOOL  ok = GetFileVersionInfo( wFileName.c_str(), 0, len, data );
		if( !ok|| len==0 )
		{
			delete[] data;
			return "";
		}
	}

	// get version fields
	VS_FIXEDFILEINFO *lpFfi;
	{
		UINT sz;
		BOOL ok = VerQueryValue( data , TEXT("\\") , (void**)&lpFfi , &sz);
		if( !ok|| sz==0 )
		{
			delete[] data;
			return "";
		}
	}


	DWORD dwFileVersionMS = lpFfi->dwFileVersionMS;
	DWORD dwFileVersionLS = lpFfi->dwFileVersionLS;

	unsigned int majorVersion    = HIWORD(dwFileVersionMS);
	unsigned int minorVersion    = LOWORD(dwFileVersionMS);
	unsigned int buildNumber     = HIWORD(dwFileVersionLS);
	unsigned int revisionNumber  = LOWORD(dwFileVersionLS);
	delete [] data;

	std::stringstream ss;
	ss << majorVersion   << "."
	   << minorVersion   << "."
	   << buildNumber    << "."
	   << revisionNumber ;

	return ss.str();
}

// ------------------------------------------------------------------------------------------------
std::string Win32::systemName()
{
	OSVERSIONINFO osvi;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	SYSTEM_INFO siSysInfo;

	// Copy the hardware information to the SYSTEM_INFO structure. 
	GetSystemInfo(&siSysInfo); 

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	/* 
	* Windows 8.1	            6.3*
	* Windows Server 2012 R2	6.3*
	* Windows 8	                6.2
	* Windows Server 2012	    6.2
	* Windows 7	                6.1
	* Windows Server 2008 R2	6.1
	* Windows Server 2008	    6.0
	* Windows Vista	            6.0
	* Windows Server 2003 R2	5.2
	* Windows Server 2003	    5.2
	* Windows XP 64-Bit Edition	5.2
	* Windows XP	            5.1
	* Windows 2000	            5.0
	*/

	const std::string baseName("Windows");
	
	std::string osVersion;
	if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3 )	osVersion = IsOS(OS_SERVER) ? "Server 2012 R2" : "8.1";
	if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2 )	osVersion = IsOS(OS_SERVER) ? "Server 2012"    : "8";
	if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1 )	osVersion = IsOS(OS_SERVER) ? "Server 2008 R2" : "7";
	if( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )	osVersion = IsOS(OS_SERVER) ? "Server 2008"    : "Vista";

	if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
	{
		if     ( GetSystemMetrics(SM_SERVERR2) != 0 )	osVersion = "Server 2003 R2";
		else if( GetSystemMetrics(SM_SERVERR2) == 0 )   osVersion = "Server 2003";
		else if( IsOS(OS_SERVER) )                      osVersion = "Home Server 2003";
		else if( IsOS(OS_SERVER) && 
			   (siSysInfo.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64) ) osVersion = "XP 64-Bit Edition";
	}

	if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )	osVersion = "XP";
	if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )	osVersion = "2000";

	std::string bits;
	if( IsOS( OS_WOW6432 ) ) bits = "64-bit";
	else                     bits = "32-bit";


	return baseName + " " + osVersion + " " + bits;// Service Pack: + " - " + osvi.szCSDVersion;
}

// ------------------------------------------------------------------------------------------------
bool Win32::sendMail( const std::vector<std::string> &recipient, const std::string &subject, const std::string &body, const std::string &fileName )
{
	HINSTANCE hMAPI = ::LoadLibrary(L"MAPI32.DLL");
	if (!hMAPI)
		return false;

	HWND hWndParent = HWND_DESKTOP;

	ULONG (PASCAL *SendMail)(ULONG, ULONG_PTR, MapiMessage*, FLAGS, ULONG);
	(FARPROC&)SendMail = GetProcAddress(hMAPI, "MAPISendMail");

	if (!SendMail)
		return false;

	if( recipient.empty() )
		return false;

	std::vector<MapiRecipDesc> recips;
	for( size_t i=0; i<recipient.size(); ++i )
	{
		MapiRecipDesc recipDesc;
		::ZeroMemory(&recipDesc, sizeof(recipDesc));
		recipDesc.ulRecipClass = MAPI_TO;
		recipDesc.lpszName     = (LPSTR)recipient[i].c_str();

		recips.push_back(recipDesc);
	}

	MapiMessage message;
	::ZeroMemory(&message, sizeof(message));
	message.lpszSubject = (LPSTR)subject.c_str();
	message.lpszNoteText= (LPSTR)body.c_str();
	message.nRecipCount = recips.size();
	message.lpRecips    = &recips[0];

	MapiFileDesc fileDesc;
	if( !fileName.empty() )
	{
		::ZeroMemory(&fileDesc, sizeof(fileDesc));
		fileDesc.nPosition    = (ULONG)-1;
		fileDesc.lpszPathName = (LPSTR)fileName.c_str();
		fileDesc.lpszFileName = (LPSTR)fileName.c_str();

		message.nFileCount  = 1;
		message.lpFiles     = &fileDesc;
	}


	int nError = SendMail(0, (ULONG_PTR)hWndParent, &message, MAPI_LOGON_UI|MAPI_DIALOG, 0);

	if (nError != SUCCESS_SUCCESS && nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
		return false;

	return true;
}

// ------------------------------------------------------------------------------------------------
//                                      CRASH REPORT                                               
// ------------------------------------------------------------------------------------------------
#include <Dbghelp.h>
#include <tchar.h>
// minidump function pointer
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess,
	                                     DWORD dwPid, HANDLE hFile, 
										 MINIDUMP_TYPE DumpType,
										 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
										 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
										 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

// ------------------------------------------------------------------------------------------------
static void createMinidump( EXCEPTION_POINTERS* apExceptionInfo )
{
	// load dbghelp
	HMODULE mhLib = ::LoadLibraryA("dbghelp.dll");
	if( !mhLib )
		return;

	// get procedure address
	MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress(mhLib, "MiniDumpWriteDump");
	if( !pDump )
		return;
	
	// create filename
	char name[MAX_PATH];
	{
		char *nameEnd = name + GetModuleFileNameA(GetModuleHandleA(0), name, MAX_PATH);
		SYSTEMTIME t;
		GetSystemTime(&t);
		wsprintfA(nameEnd - strlen(".exe"),
			"_%4d%02d%02d_%02d%02d%02d.dmp",
			t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	}

	// open file for writing
	HANDLE  hFile = ::CreateFileA( name, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	if(hFile == INVALID_HANDLE_VALUE)
		return;

	// fill minidump info
	MINIDUMP_EXCEPTION_INFORMATION ExInfo;
	ExInfo.ThreadId = ::GetCurrentThreadId();
	ExInfo.ExceptionPointers = apExceptionInfo;
	ExInfo.ClientPointers = FALSE;

	// dump
	BOOL dumped = pDump( GetCurrentProcess(),
		                 GetCurrentProcessId(), 
						 hFile, 
						 MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory),
						 apExceptionInfo ? &ExInfo : 0,
						 0,
						 0);
	// close file
	::CloseHandle(hFile);
}


// ------------------------------------------------------------------------------------------------
static LONG WINAPI unhandledExceptionHandler( _EXCEPTION_POINTERS* apExceptionInfo)
{
	createMinidump(apExceptionInfo);
	return EXCEPTION_CONTINUE_SEARCH;
}

// ------------------------------------------------------------------------------------------------
void Win32::enableCrashReport()
{
	::SetUnhandledExceptionFilter(unhandledExceptionHandler);
}

// ------------------------------------------------------------------------------------------------
// * Returns the peak (maximum so far) resident set size (physical
// * memory use) measured in bytes, or zero if the value cannot be
// * determined on this OS.
size_t Win32::peakMemoryUsage()
{
	/* Windows -------------------------------------------------- */
	PROCESS_MEMORY_COUNTERS info;
	GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
	return (size_t)info.PeakWorkingSetSize;
}


// ------------------------------------------------------------------------------------------------
// * Returns the current resident set size (physical memory use) measured
// * in bytes, or zero if the value cannot be determined on this OS.
size_t Win32::currentMemoryUsage()
{
	/* Windows -------------------------------------------------- */
	PROCESS_MEMORY_COUNTERS info;
	GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
	return (size_t)info.WorkingSetSize;
}
