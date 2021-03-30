//-----------------------------------------------------------------------------
//     Author : hiyohiyo
//       Mail : hiyohiyo@crystalmark.info
//        Web : http://openlibsys.org/
//    License : The modified BSD license
//
//                          Copyright 2007 OpenLibSys.org. All rights reserved.
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "DriverLoad.h"
#include <strsafe.h>
#include <string.h>
#include <tchar.h>

//-----------------------------------------------------------------------------
//
// Global
//
//-----------------------------------------------------------------------------

HANDLE gHandle = INVALID_HANDLE_VALUE;
TCHAR gDriverPath[MAX_PATH] = {};
#define SYSTEM32_DRIVERS _T("\\System32\\Drivers\\")
#define SYSTEM_TEMP _T("\\Temp\\")

#define OLS_DRIVER_INSTALL			1
#define OLS_DRIVER_REMOVE			2
#define OLS_DRIVER_SYSTEM_INSTALL	3
#define	OLS_DRIVER_SYSTEM_UNINSTALL	4
#define OLS_DRIVER_EXIT				5

#define DRIVER_ID					_T("{8EB9AAE8-AC93-4AB3-ACC1-275CB500E346}")

#define DRIVER_INF_NAME				L"WSHardwarePluginSys.inf"

//-----------------------------------------------------------------------------
//
// Prototypes
//
//-----------------------------------------------------------------------------

static BOOL InstallDriver(SC_HANDLE hSCManager, LPCTSTR DriverId, LPCTSTR DriverPath);
static BOOL RemoveDriver(SC_HANDLE hSCManager, LPCTSTR DriverId);
static BOOL RemoveFile(LPCTSTR DriverId);
static BOOL StartDriver(SC_HANDLE hSCManager, LPCTSTR DriverId);
static BOOL StopDriver(SC_HANDLE hSCManager, LPCTSTR DriverId);
static BOOL SystemInstallDriver(SC_HANDLE hSCManager, LPCTSTR DriverId, LPCTSTR DriverPath);
static BOOL IsSystemInstallDriver(SC_HANDLE hSCManager, LPCTSTR DriverId, LPCTSTR DriverPath);

BOOL Is64BitOS()
{
#ifdef _WIN64
	return TRUE;
#else
	GETSYSTEMWOW64DIRECTORY getSystemWow64Directory;
	HMODULE hKernel32;
	TCHAR Wow64Directory[32767];

	hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
	if (hKernel32 == NULL)
	{
		//
		// This shouldn't happen, but if we can't get
		// kernel32's module handle then assume we are
		// on x86. We won't ever install 32-bit drivers
		// on 64-bit machines, we just want to catch it
		// up front to give users a better error message.
		//
		return FALSE;
	}

	getSystemWow64Directory = (GETSYSTEMWOW64DIRECTORY)GetProcAddress(hKernel32, "GetSystemWow64DirectoryW");

	if (getSystemWow64Directory == NULL)
	{
		//
		// This most likely means we are running
		// on Windows 2000, which didn't have this API
		// and didn't have a 64-bit counterpart.
		//
		return FALSE;
	}

	if ((getSystemWow64Directory(Wow64Directory, _countof(Wow64Directory)) == 0) &&
		(GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)) {
		return FALSE;
	}

	//
	// GetSystemWow64Directory succeeded
	// so we are on a 64-bit OS.
	//
	return TRUE;
#endif
}

LPCTSTR GetDriverName()
{
	// TODO X64 X86Çø·Ö
	if (Is64BitOS())
		return _T("WSHardwarePluginSys.sys");
	else
		return _T("WSHardwarePluginSys.sys");
}

BOOL ManageDriver(LPCTSTR DriverId, LPCTSTR DriverPath, USHORT Function);

BOOLEAN
SetupDriverName(
	_Inout_updates_all_(BufferLength) TCHAR* DriverLocation,
	_In_ ULONG BufferLength
);

BOOL Initialize();

//
// Caller must free returned pathname string.
//
TCHAR*
BuildDriversDirPath(
	_In_ const TCHAR* const DriverName
)
{
	size_t  remain;
	size_t  len;
	TCHAR* dir;

	if (!DriverName || wcslen(DriverName) == 0) {
		return NULL;
	}

	remain = MAX_PATH;

	//
	// Allocate string space
	//
	dir = (TCHAR*)malloc(remain + 1);

	if (!dir) {
		return NULL;
	}

	//
	// Get the base windows directory path.
	//
	len = GetWindowsDirectory(dir, (UINT)remain);

	if (len == 0 ||
		(remain - len) < sizeof(SYSTEM_TEMP)) {
		free(dir);
		return NULL;
	}
	remain -= len;

	//
	// Build dir to have "%windir%\System32\Drivers\<DriverName>".
	//
	if (FAILED(StringCchCat(dir, remain, SYSTEM_TEMP))) {
		free(dir);
		return NULL;
	}

	remain -= sizeof(SYSTEM_TEMP);
	len += sizeof(SYSTEM_TEMP);
	len += wcslen(DriverName);

	if (remain < len) {
		free(dir);
		return NULL;
	}

	if (FAILED(StringCchCat(dir, remain, DriverName))) {
		free(dir);
		return NULL;
	}

	dir[len] = '\0';  // keeps prefast happy

	return dir;
}

BOOLEAN
SetupDriverName(
	_Inout_updates_all_(BufferLength) TCHAR* DriverLocation,
	_In_ ULONG BufferLength
)
{
	HANDLE fileHandle;
	DWORD  driverLocLen = 0;
	BOOL   ok;
	TCHAR* driversDir;

	//
	// Setup path name to driver file.
	//
	driverLocLen =
		GetCurrentDirectory(BufferLength, DriverLocation);

	if (!driverLocLen) {
		LOG_ERROR(_T("GetCurrentDirectory failed!  Error = %d \n"),
			GetLastError());

		return FALSE;
	}

	std::wstring DriverName = L"\\";
	DriverName.append(GetDriverName());

	if (FAILED(StringCchCat(DriverLocation, BufferLength, DriverName.c_str()))) {
		return FALSE;
	}

	//
	// Insure driver file is in the specified directory.
	//
	fileHandle = CreateFile(DriverLocation,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (fileHandle == INVALID_HANDLE_VALUE) {
		//
		// Indicate failure.
		//
		LOG_ERROR(_T("Driver: %ls is not in the %ls directory. \n"),
			GetDriverName(), DriverLocation);
		return FALSE;
	}

	//
	// Build %windir%\System32\Drivers\<GetDriverName()> path.
	// Copy the driver to %windir%\system32\drivers
	//
	driversDir = BuildDriversDirPath(GetDriverName());

	if (!driversDir) {
		LOG_ERROR(_T("BuildDriversDirPath failed!\n"));
		return FALSE;
	}

	ok = CopyFile(DriverLocation, driversDir, FALSE);

	if (!ok) {
		LOG_ERROR(_T("CopyFile failed: error(%d) - \"%ls\"\n"),
			GetLastError(), driversDir);
		free(driversDir);
		return FALSE;
	}

	if (FAILED(StringCchCopy(DriverLocation, BufferLength, driversDir))) {
		free(driversDir);
		return FALSE;
	}

	free(driversDir);

	//
	// Close open file handle.
	//
	if (fileHandle) {
		CloseHandle(fileHandle);
	}

	//
	// Indicate success.
	//
	return TRUE;
}   // SetupDriverName

BOOL StartRing0()
{
	BOOL status = FALSE;
	for (int i = 0; i < 4; i++)
	{
		status = Initialize();
		if (status == TRUE)
		{
			break;
		}
		Sleep(100 * i);
	}
	return status;
}

VOID ExitRing0()
{
	CloseHandle(gHandle);
	gHandle = INVALID_HANDLE_VALUE;
	ManageDriver(DRIVER_ID, gDriverPath, OLS_DRIVER_REMOVE);
}

HANDLE& GetGlobalHandle()
{
	return gHandle;
}

BOOL OpenDriver()
{
	gHandle = CreateFile(
		_T("\\\\.\\") DRIVER_ID,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (gHandle == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR(_T("OpenDriver Failed! Error Code: %d"), GetLastError());
		return FALSE;
	}
	return TRUE;
}
//-----------------------------------------------------------------------------
//
// Manage Driver
//
//-----------------------------------------------------------------------------

BOOL Initialize()
{
	TCHAR dir[MAX_PATH] = {};
	TCHAR* ptr;
	TCHAR gDriverFileName[MAX_PATH] = {};

	_tcscpy_s(gDriverFileName, MAX_PATH, GetDriverName());

	GetModuleFileName(NULL, dir, MAX_PATH);
	if ((ptr = _tcsrchr(dir, '\\')) != NULL)
	{
		*ptr = '\0';
	}
	wsprintf(gDriverPath, _T("%s\\%s"), dir, gDriverFileName);

	if (OpenDriver())
	{
		return TRUE;
	}

	if (!SetupDriverName(gDriverPath, MAX_PATH))
		return FALSE;

	ManageDriver(DRIVER_ID, gDriverPath, OLS_DRIVER_REMOVE);
	if (!ManageDriver(DRIVER_ID, gDriverPath, OLS_DRIVER_INSTALL))
	{
		ManageDriver(DRIVER_ID, gDriverPath, OLS_DRIVER_REMOVE);
		return FALSE;
	}

	if (OpenDriver())
	{
		return TRUE;
	}
	return FALSE;
}

BOOL ManageDriver(LPCTSTR DriverId, LPCTSTR DriverPath, USHORT Function)
{
	SC_HANDLE	hSCManager = NULL;
	BOOL		rCode = FALSE;

	if (DriverId == NULL || DriverPath == NULL)
	{
		return FALSE;
	}
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCManager == NULL)
	{
		return FALSE;
	}

	switch (Function)
	{
	case OLS_DRIVER_INSTALL:
		if (InstallDriver(hSCManager, DriverId, DriverPath))
		{
			rCode = StartDriver(hSCManager, DriverId);
		}
		break;
	case OLS_DRIVER_REMOVE:
		if (!IsSystemInstallDriver(hSCManager, DriverId, DriverPath))
		{
			StopDriver(hSCManager, DriverId);
			rCode = RemoveDriver(hSCManager, DriverId);
		}
		break;
	case OLS_DRIVER_EXIT:
		if (!IsSystemInstallDriver(hSCManager, DriverId, DriverPath))
		{
			StopDriver(hSCManager, DriverId);
			rCode = RemoveDriver(hSCManager, DriverId);
		}
		//		RemoveFile(DriverPath);
		break;
	case OLS_DRIVER_SYSTEM_INSTALL:
		if (IsSystemInstallDriver(hSCManager, DriverId, DriverPath))
		{
			rCode = TRUE;
		}
		else
		{
			if (!OpenDriver())
			{
				StopDriver(hSCManager, DriverId);
				RemoveDriver(hSCManager, DriverId);
				if (InstallDriver(hSCManager, DriverId, DriverPath))
				{
					StartDriver(hSCManager, DriverId);
				}
				OpenDriver();
			}
			rCode = SystemInstallDriver(hSCManager, DriverId, DriverPath);
		}
		break;
	case OLS_DRIVER_SYSTEM_UNINSTALL:
		if (!IsSystemInstallDriver(hSCManager, DriverId, DriverPath))
		{
			rCode = TRUE;
		}
		else
		{
			if (gHandle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(gHandle);
				gHandle = INVALID_HANDLE_VALUE;
			}

			if (StopDriver(hSCManager, DriverId))
			{
				rCode = RemoveDriver(hSCManager, DriverId);
			}
		}
		break;
	default:
		rCode = FALSE;
		break;
	}

	if (hSCManager != NULL)
	{
		CloseServiceHandle(hSCManager);
	}

	return rCode;
}

//-----------------------------------------------------------------------------
//
// Install Driver
//
//-----------------------------------------------------------------------------

BOOL InstallDriver(SC_HANDLE hSCManager, LPCTSTR DriverId, LPCTSTR DriverPath)
{
	SC_HANDLE	hService = NULL;
	BOOL        rCode = FALSE;
	DWORD		error = NO_ERROR;

	hService = CreateService(hSCManager,
		DriverId,
		DriverId,
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		DriverPath,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	if (hService == NULL)
	{
		error = GetLastError();
		if (error == ERROR_SERVICE_EXISTS)
		{
			rCode = TRUE;
		}
		else
		{
			LOG_ERROR(_T("CreateService Failed! Error Code: %d"), error);
		}
	}
	else
	{
		rCode = TRUE;
		CloseServiceHandle(hService);
	}

	return rCode;
}

//-----------------------------------------------------------------------------
//
// System Install Driver
//
//-----------------------------------------------------------------------------

BOOL SystemInstallDriver(SC_HANDLE hSCManager, LPCTSTR DriverId, LPCTSTR DriverPath)
{
	SC_HANDLE	hService = NULL;
	BOOL		rCode = FALSE;

	hService = OpenService(hSCManager, DriverId, SERVICE_ALL_ACCESS);

	if (hService != NULL)
	{
		rCode = ChangeServiceConfig(hService,
			SERVICE_KERNEL_DRIVER,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			DriverPath,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
		);
		CloseServiceHandle(hService);
	}

	return rCode;
}

//-----------------------------------------------------------------------------
//
// Remove Driver
//
//-----------------------------------------------------------------------------

BOOL RemoveDriver(SC_HANDLE hSCManager, LPCTSTR DriverId)
{
	SC_HANDLE   hService = NULL;
	BOOL        rCode = FALSE;

	hService = OpenService(hSCManager, DriverId, SERVICE_ALL_ACCESS);
	if (hService == NULL)
	{
		rCode = TRUE;
	}
	else
	{
		rCode = DeleteService(hService);
		CloseServiceHandle(hService);
	}

	return rCode;
}

BOOL RemoveFile(LPCTSTR DriverId)
{
	LOG_ERROR(_T("Remove sys: %ls"), DriverId);
	return _wremove(DriverId);
}

//-----------------------------------------------------------------------------
//
// Start Driver
//
//-----------------------------------------------------------------------------

BOOL StartDriver(SC_HANDLE hSCManager, LPCTSTR DriverId)
{
	SC_HANDLE	hService = NULL;
	BOOL		rCode = FALSE;
	DWORD		error = NO_ERROR;

	hService = OpenService(hSCManager, DriverId, SERVICE_ALL_ACCESS);

	if (hService != NULL)
	{
		if (!StartService(hService, 0, NULL))
		{
			error = GetLastError();
			if (error == ERROR_SERVICE_ALREADY_RUNNING)
			{
				rCode = TRUE;
			}
			else
			{
				LOG_ERROR(_T("StartService Failed! Error Code: %d"), error);
			}
		}
		else
		{
			rCode = TRUE;
		}
		CloseServiceHandle(hService);
	}

	return rCode;
}

//-----------------------------------------------------------------------------
//
// Stop Driver
//
//-----------------------------------------------------------------------------

BOOL StopDriver(SC_HANDLE hSCManager, LPCTSTR DriverId)
{
	SC_HANDLE		hService = NULL;
	BOOL			rCode = FALSE;
	SERVICE_STATUS	serviceStatus;
	DWORD		error = NO_ERROR;

	hService = OpenService(hSCManager, DriverId, SERVICE_ALL_ACCESS);

	if (hService != NULL)
	{
		rCode = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);
		error = GetLastError();
		CloseServiceHandle(hService);
	}

	return rCode;
}

//-----------------------------------------------------------------------------
//
// IsSystemInstallDriver
//
//-----------------------------------------------------------------------------

BOOL IsSystemInstallDriver(SC_HANDLE hSCManager, LPCTSTR DriverId, LPCTSTR DriverPath)
{
	UNREFERENCED_PARAMETER(DriverPath);
	SC_HANDLE				hService = NULL;
	BOOL					rCode = FALSE;
	DWORD					dwSize;
	LPQUERY_SERVICE_CONFIG	lpServiceConfig;

	hService = OpenService(hSCManager, DriverId, SERVICE_ALL_ACCESS);

	if (hService != NULL)
	{
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		lpServiceConfig = (LPQUERY_SERVICE_CONFIG)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY, dwSize);
		QueryServiceConfig(hService, lpServiceConfig, dwSize, &dwSize);

		if (lpServiceConfig->dwStartType == SERVICE_AUTO_START)
		{
			rCode = TRUE;
		}

		CloseServiceHandle(hService);

		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, lpServiceConfig);
	}

	return rCode;
}