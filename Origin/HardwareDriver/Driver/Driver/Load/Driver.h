//-----------------------------------------------------------------------------
//     Author : hiyohiyo
//       Mail : hiyohiyo@crystalmark.info
//        Web : http://openlibsys.org/
//    License : The modified BSD license
//
//                          Copyright 2007 OpenLibSys.org. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

#define OLS_DRIVER_INSTALL			1
#define OLS_DRIVER_REMOVE			2
#define OLS_DRIVER_SYSTEM_INSTALL	3
#define	OLS_DRIVER_SYSTEM_UNINSTALL	4

#define DRIVER_NAME					_T("CharTest.sys")
#define DRIVER_ID					_T("CharTest")
#define DRIVER_INF_NAME				L"CharTest.inf"

HMODULE
LoadWdfCoInstaller(
	VOID
);

VOID
UnloadWdfCoInstaller(
	HMODULE Library
);

BOOLEAN
SetupDriverName(
	_Inout_updates_all_(BufferLength) PCHAR DriverLocation,
	_In_ ULONG BufferLength
);

BOOL Initialize();
BOOL ManageDriver(LPCTSTR DriverId, LPCTSTR DriverPath, USHORT Function);

