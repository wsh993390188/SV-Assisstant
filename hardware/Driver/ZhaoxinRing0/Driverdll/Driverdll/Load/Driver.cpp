//-----------------------------------------------------------------------------
//     Author : hiyohiyo
//       Mail : hiyohiyo@crystalmark.info
//        Web : http://openlibsys.org/
//    License : The modified BSD license
//
//                          Copyright 2007 OpenLibSys.org. All rights reserved.
//-----------------------------------------------------------------------------
//#include "stdafx.h"
#include <DriverSpecs.h>
_Analysis_mode_(_Analysis_code_type_user_code_)

#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include <wdfinstaller.h>

#include "Driver.h"

#define WHILE(a) \
__pragma(warning(suppress:4127)) while(a)

#define ARRAY_SIZE(x)        (sizeof(x) /sizeof(x[0]))
#define WDF_SECTION_NAME L"ZhaoxinRing0.NT.Wdf"
#define SYSTEM32_DRIVERS _T("\\System32\\Drivers\\")
#define MAX_VERSION_SIZE 6
//-----------------------------------------------------------------------------
//
// Global
//
//-----------------------------------------------------------------------------

HANDLE gHandle = INVALID_HANDLE_VALUE;
TCHAR gDriverPath[MAX_PATH];

//-----------------------------------------------------------------------------
//
// Prototypes
//
//-----------------------------------------------------------------------------

static BOOL InstallDriver(SC_HANDLE hSCManager, LPCTSTR DriverId, LPCTSTR DriverPath);
static BOOL RemoveDriver(SC_HANDLE hSCManager, LPCTSTR DriverId);
static BOOL StartDriver(SC_HANDLE hSCManager, LPCTSTR DriverId);
static BOOL StopDriver(SC_HANDLE hSCManager, LPCTSTR DriverId);
static BOOL SystemInstallDriver(SC_HANDLE hSCManager, LPCTSTR DriverId, LPCTSTR DriverPath);
static BOOL IsSystemInstallDriver(SC_HANDLE hSCManager, LPCTSTR DriverId, LPCTSTR DriverPath);

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
PFN_WDFPREDEVICEINSTALLEX pfnWdfPreDeviceInstallEx;
PFN_WDFPOSTDEVICEINSTALL   pfnWdfPostDeviceInstall;
PFN_WDFPREDEVICEREMOVE     pfnWdfPreDeviceRemove;
PFN_WDFPOSTDEVICEREMOVE   pfnWdfPostDeviceRemove;

LONG
GetPathToInf(
	_Out_writes_(InfFilePathSize) PWCHAR InfFilePath,
	IN ULONG InfFilePathSize
)
{
	LONG    error = ERROR_SUCCESS;

	if (GetCurrentDirectoryW(InfFilePathSize, InfFilePath) == 0) {
		error = GetLastError();
		printf("InstallDriver failed!  Error = %d \n", error);
		return error;
	}
	if (FAILED(StringCchCatW(InfFilePath,
		InfFilePathSize,
		L"\\" DRIVER_INF_NAME))) {
		error = ERROR_BUFFER_OVERFLOW;
		return error;
	}
	return error;

}

//
// Caller must free returned pathname string.
//
TCHAR*
BuildDriversDirPath(
	_In_ TCHAR* DriverName
)
{
	size_t  remain;
	size_t  len;
	TCHAR*   dir;

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
		(remain - len) < sizeof(SYSTEM32_DRIVERS)) {
		free(dir);
		return NULL;
	}
	remain -= len;

	//
	// Build dir to have "%windir%\System32\Drivers\<DriverName>".
	//
	if (FAILED(StringCchCat(dir, remain, SYSTEM32_DRIVERS))) {
		free(dir);
		return NULL;
	}

	remain -= sizeof(SYSTEM32_DRIVERS);
	len += sizeof(SYSTEM32_DRIVERS);
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
	TCHAR*  driversDir;

	//
	// Setup path name to driver file.
	//
	driverLocLen =
		GetCurrentDirectory(BufferLength, DriverLocation);

	if (!driverLocLen) {

		printf("GetCurrentDirectory failed!  Error = %d \n",
			GetLastError());

		return FALSE;
	}

	if (FAILED(StringCchCat(DriverLocation, BufferLength, "\\" DRIVER_NAME))) {
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
		printf("Driver: %ls.SYS is not in the %ls directory. \n",
			DRIVER_NAME, DriverLocation);
		return FALSE;
	}

	//
	// Build %windir%\System32\Drivers\<DRIVER_NAME> path.
	// Copy the driver to %windir%\system32\drivers
	//
	driversDir = BuildDriversDirPath(DRIVER_NAME);

	if (!driversDir) {
		printf("BuildDriversDirPath failed!\n");
		return FALSE;
	}

	ok = CopyFile(DriverLocation, driversDir, FALSE);

	if (!ok) {
		printf("CopyFile failed: error(%d) - \"%ls\"\n",
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

HMODULE
LoadWdfCoInstaller(
	VOID
)
{
	HMODULE library = NULL;
	DWORD   error = ERROR_SUCCESS;
	TCHAR   szCurDir[MAX_PATH];
	TCHAR   tempCoinstallerName[MAX_PATH];
	TCHAR*  coinstallerVersion;

	do {

		if (GetCurrentDirectory(MAX_PATH, szCurDir) == 0) {

			printf("GetCurrentDirectory failed!  Error = %d \n", GetLastError());
			break;
		}

		coinstallerVersion = _T("01011");
		if (FAILED(StringCchPrintf(tempCoinstallerName,
			MAX_PATH,
			_T("WdfCoInstaller%s.dll"),
			coinstallerVersion))) {
			break;
		}
		if (FAILED(StringCchCat(szCurDir, MAX_PATH, _T("\\")))) {
			break;
		}
		if (FAILED(StringCchCat(szCurDir, MAX_PATH, tempCoinstallerName))) {
			break;
		}

		library = LoadLibrary(tempCoinstallerName);

		if (library == NULL) {
			error = GetLastError();
			printf("LoadLibrary(%ls) failed: %d\n", tempCoinstallerName, error);
			break;
		}

		pfnWdfPreDeviceInstallEx =
			(PFN_WDFPREDEVICEINSTALLEX)GetProcAddress(library, "WdfPreDeviceInstallEx");

		if (pfnWdfPreDeviceInstallEx == NULL) {
			error = GetLastError();
			printf("GetProcAddress(\"WdfPreDeviceInstallEx\") failed: %d\n", error);
			return NULL;
		}

		pfnWdfPostDeviceInstall =
			(PFN_WDFPOSTDEVICEINSTALL)GetProcAddress(library, "WdfPostDeviceInstall");

		if (pfnWdfPostDeviceInstall == NULL) {
			error = GetLastError();
			printf("GetProcAddress(\"WdfPostDeviceInstall\") failed: %d\n", error);
			return NULL;
		}

		pfnWdfPreDeviceRemove =
			(PFN_WDFPREDEVICEREMOVE)GetProcAddress(library, "WdfPreDeviceRemove");

		if (pfnWdfPreDeviceRemove == NULL) {
			error = GetLastError();
			printf("GetProcAddress(\"WdfPreDeviceRemove\") failed: %d\n", error);
			return NULL;
		}

		pfnWdfPostDeviceRemove =
			(PFN_WDFPREDEVICEREMOVE)GetProcAddress(library, "WdfPostDeviceRemove");

		if (pfnWdfPostDeviceRemove == NULL) {
			error = GetLastError();
			printf("GetProcAddress(\"WdfPostDeviceRemove\") failed: %d\n", error);
			return NULL;
		}

	} WHILE(0);

	if (error != ERROR_SUCCESS) {
		if (library) {
			FreeLibrary(library);
		}
		library = NULL;
	}

	return library;
}


VOID
UnloadWdfCoInstaller(
	HMODULE Library
)
{
	if (Library) {
		FreeLibrary(Library);
	}
}

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
	TCHAR dir[MAX_PATH];
	TCHAR *ptr;
	TCHAR gDriverFileName[MAX_PATH];

	_tcscpy_s(gDriverFileName, MAX_PATH, DRIVER_NAME);

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
	DWORD		error = NO_ERROR;

	if(DriverId == NULL || DriverPath == NULL)
	{
		return FALSE;
	}
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if(hSCManager == NULL)
	{
        return FALSE;
    }

    switch(Function)
	{
	case OLS_DRIVER_INSTALL:
		if(InstallDriver(hSCManager, DriverId, DriverPath))
		{
			rCode = StartDriver(hSCManager, DriverId);
		}
		break;
	case OLS_DRIVER_REMOVE:
		if(! IsSystemInstallDriver(hSCManager, DriverId, DriverPath))
		{
			StopDriver(hSCManager, DriverId);
			rCode = RemoveDriver(hSCManager, DriverId);
		}
		break;
	case OLS_DRIVER_SYSTEM_INSTALL:
		if(IsSystemInstallDriver(hSCManager, DriverId, DriverPath))
		{
			rCode = TRUE;
		}
		else
		{
			if(! OpenDriver())
			{
				StopDriver(hSCManager, DriverId);
				RemoveDriver(hSCManager, DriverId);
				if(InstallDriver(hSCManager, DriverId, DriverPath))
				{
					StartDriver(hSCManager, DriverId);
				}
				OpenDriver();
			}
			rCode = SystemInstallDriver(hSCManager, DriverId, DriverPath);
		}
		break;
	case OLS_DRIVER_SYSTEM_UNINSTALL:
		if(! IsSystemInstallDriver(hSCManager, DriverId, DriverPath))
		{
			rCode = TRUE;
		}
		else
		{
			if(gHandle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(gHandle);
				gHandle = INVALID_HANDLE_VALUE;
			}

			if(StopDriver(hSCManager, DriverId))
			{
				rCode = RemoveDriver(hSCManager, DriverId);
			}
		}
		break;
	default:
		rCode = FALSE;
		break;
    }

	if(hSCManager != NULL)
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
	WDF_COINSTALLER_INSTALL_OPTIONS clientOptions;
	WCHAR      infPath[MAX_PATH];

	WDF_COINSTALLER_INSTALL_OPTIONS_INIT(&clientOptions);

	//
	// NOTE: This creates an entry for a standalone driver. If this
	//       is modified for use with a driver that requires a Tag,
	//       Group, and/or Dependencies, it may be necessary to
	//       query the registry for existing driver information
	//       (in order to determine a unique Tag, etc.).
	//
	//
	// PRE-INSTALL for WDF support
	//
	error = GetPathToInf(infPath, ARRAY_SIZE(infPath));
	if (error != ERROR_SUCCESS) {
		return  FALSE;
	}
	error = pfnWdfPreDeviceInstallEx(infPath, WDF_SECTION_NAME, &clientOptions);

	if (error != ERROR_SUCCESS) {
		if (error == ERROR_SUCCESS_REBOOT_REQUIRED) {
			printf("System needs to be rebooted, before the driver installation can proceed.\n");
		}

		return  FALSE;
	}

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

    if(hService == NULL)
	{
		error = GetLastError();
		if(error == ERROR_SERVICE_EXISTS)
		{
			rCode = TRUE;
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

	if(hService != NULL)
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

	DWORD       err;
	WCHAR      infPath[MAX_PATH];

	err = GetPathToInf(infPath, ARRAY_SIZE(infPath));
	if (err != ERROR_SUCCESS) {
		return  FALSE;
	}

	//
	// PRE-REMOVE of WDF support
	//
	err = pfnWdfPreDeviceRemove(infPath, WDF_SECTION_NAME);

	if (err != ERROR_SUCCESS) {
		return  FALSE;
	}

	//
	// Open the handle to the existing service.
	//

    hService = OpenService(hSCManager, DriverId, SERVICE_ALL_ACCESS);
    if(hService == NULL)
	{
		rCode = TRUE;
	}
	else
	{
		rCode = DeleteService(hService);
		CloseServiceHandle(hService);
	}

	//
	// POST-REMOVE of WDF support
	//
	err = pfnWdfPostDeviceRemove(infPath, WDF_SECTION_NAME);

	if (err != ERROR_SUCCESS) {
		rCode = FALSE;
	}

	return rCode;
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
    if(hService != NULL)
	{
		if(! StartService(hService, 0, NULL))
		{
			error = GetLastError();
			if(error == ERROR_SERVICE_ALREADY_RUNNING)
			{
				rCode = TRUE;
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

    if(hService != NULL)
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
	SC_HANDLE				hService = NULL;
	BOOL					rCode = FALSE;
	DWORD					dwSize;
	LPQUERY_SERVICE_CONFIG	lpServiceConfig;

    hService = OpenService(hSCManager, DriverId, SERVICE_ALL_ACCESS);

	if(hService != NULL)
	{
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		lpServiceConfig = (LPQUERY_SERVICE_CONFIG)HeapAlloc(GetProcessHeap(), 
															HEAP_ZERO_MEMORY, dwSize);
		QueryServiceConfig(hService, lpServiceConfig, dwSize, &dwSize);

		if(lpServiceConfig->dwStartType == SERVICE_AUTO_START)
		{
			rCode = TRUE;
		}

		CloseServiceHandle(hService);

		HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, lpServiceConfig);
	}
	
	return rCode;
}