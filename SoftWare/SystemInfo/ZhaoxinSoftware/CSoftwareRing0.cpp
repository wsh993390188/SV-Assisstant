#include "stdafx.h"
#include "CSoftwareRing0.h"



CSoftwareRing0::CSoftwareRing0() : hdevice(INVALID_HANDLE_VALUE), OutBuffersize(0)
{
	this->SoftwareRing0_Initialize();
	SYSTEM_INFO qwe = {};
	GetSystemInfo(&qwe);
	dwNumberOfProcessors = qwe.dwNumberOfProcessors;
}


CSoftwareRing0::~CSoftwareRing0()
{
	this->Destory();
}

void CSoftwareRing0::Exec()
{

	this->m_EnumDrivermoduleFunc();
	this->m_EnumProcessFunc();
	this->m_EnumPowerSettingCallbackFunc();
	this->m_EnumFsNotifyChangeCallbackFunc();
	this->m_EnumPlugPlayCallbackFunc();
	this->m_EnumLastShutdownCallbackFunc();
	this->m_EnumShutdownCallbackFunc();
	this->m_EnumBugReasonCallbackFunc();
	this->m_EnumBugCallbackFunc();
	this->m_EnumCmpCallbackFunc();
	this->m_EnumLoadImageCallbackFunc();
	this->m_EnumProcessorCallBackFunc();
	this->m_EnumThreadCallbackFunc();
	this->m_EnumFilterDriverFunc();
	this->m_EnumDPCTimerFunc();
	this->m_GETSCSIHOOKFunc();
	this->m_GETACPIHOOKFunc();
	this->m_GETATAPIHOOKFunc();
	this->m_GETDISKHOOKFunc();
	this->m_GETPARTMGRHOOKFunc();
	this->m_GETI8042PRTHOOKFunc();
	this->m_GETKBDHOOKFunc();
	this->m_GETMOUSEHOOKFunc();
	this->m_GETFSDHOOKFunc();
	this->m_GETNSIPROXYHOOKFunc();
	this->m_GETTCPIPHOOKFunc();
	this->m_GETTDXHOOKFunc();
	this->m_GETSSDTABLEFunc();
	this->m_GETSSDTSHADOWTABLEFunc();
	for (DWORD i = 0; i < dwNumberOfProcessors; i++)
	{
		m_GETGPTABLEFunc(i);
		m_GETIDTABLEFunc(i);
	}
}

BOOL CSoftwareRing0::SoftwareRing0_Initialize()
{
	BOOL status = FALSE;
	//
	// Load WdfCoInstaller.dll.
	//
	library = LoadWdfCoInstaller();

	if (library == NULL) {
		// 		printf("The WdfCoInstaller%s.dll library needs to be "
		// 			"in same directory as nonpnpapp.exe\n", coinstallerVersion);
		return FALSE;
	}

	for (int i = 0; i < 4; i++)
	{
		status = Initialize();
		if (status == TRUE)
		{
			hdevice = gHandle;
			break;
		}
		Sleep(100 * i);
	}
	return status;
}

void CSoftwareRing0::Destory()
{
	if (hdevice != INVALID_HANDLE_VALUE || gHandle != INVALID_HANDLE_VALUE) {
		//
		// Close the handle to the device before unloading the driver.
		//

		CloseHandle(gHandle);
		gHandle = INVALID_HANDLE_VALUE;
		hdevice = INVALID_HANDLE_VALUE;

		//
		// Unload the driver.  Ignore any errors.
		//

		ManageDriver(DRIVER_ID, gDriverPath, OLS_DRIVER_REMOVE);
	}

	//
	// Unload WdfCoInstaller.dll
	//
	if (library) {
		UnloadWdfCoInstaller(library);
	}
}

void CSoftwareRing0::UpdateData()
{
	m_Processor.clear();
	Driver_Module.clear();
	m_PowerSetting.clear();
	m_FsNotifyChange.clear();
	m_LastShutdown.clear();
	m_Shutdown.clear();
	m_BugReason.clear();
	m_Bug.clear();
	m_Cmp.clear();
	m_LoadImage.clear();
	m_ProcessorCallback.clear();
	m_ThreadCallback.clear();
	m_Filter.clear();
	m_DpcTimer.clear();
	m_GdtTable.clear();
	m_IdtTable.clear();
	m_SCSI.clear();
	m_ACPI.clear();
	m_ATAPI.clear();
	m_DISK.clear();
	m_PARTMGR.clear();
	m_I8042PRT.clear();
	m_KBD.clear();
	m_MOUSE.clear();
	m_FSD.clear();
	m_TDX.clear();
	m_TCPIP.clear();
	m_NSIPROXY.clear();
	m_SSDT.clear();
	m_SHADOWSSDT.clear();
	this->Exec();
}

BOOL CSoftwareRing0::m_EnumProcessFunc()
{
	BOOL status = TRUE;
	DWORD ProcessNum = 0;
	if (!DeviceIoControl(hdevice, GetProcessNum, nullptr, 0, &ProcessNum, sizeof(DWORD), nullptr, nullptr))
	{
		return GetLastError();
	}

	M_EPROCESS process = {};
	DWORD nOutdata = 0;
	for (DWORD i = 0; i < ProcessNum; i++)
	{
		M_EPROCESS currPro = {};
		if(this->m_Processor.size())
			currPro = this->m_Processor.back();
		if (DeviceIoControl(hdevice,
			EnumProc,
			&currPro,
			sizeof(M_EPROCESS),
			&process,
			sizeof(M_EPROCESS),
			&nOutdata,
			nullptr))
			this->m_Processor.push_back(process);
		else
		{
			status = GetLastError();
			break;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumDrivermoduleFunc()
{
	BOOL status = TRUE;
	DWORD ModuleNum = 0;
	if (!DeviceIoControl(hdevice, GetModuleNum, nullptr, 0, &ModuleNum, sizeof(DWORD), nullptr, nullptr))
	{
		return GetLastError();
	}

	DRIVER_MODULE module = {};
	for (DWORD i = 0; i < ModuleNum; i++)
	{
		DRIVER_MODULE CurMod = {};
		if (this->Driver_Module.size())
			CurMod = this->Driver_Module.back();
		if (DeviceIoControl(hdevice,
			EnumModule,
			&CurMod,
			sizeof(DRIVER_MODULE),
			&module,
			sizeof(DRIVER_MODULE),
			nullptr,
			nullptr))
			Driver_Module.emplace_back(module);
		else
		{
			status = GetLastError();
			break;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumPowerSettingCallbackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, PowerSettingCallback, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(PLUGPLAY_STRUCT);
		PPLUGPLAY_STRUCT temp = (PPLUGPLAY_STRUCT)OutBuffer.get();
		while (Count--)
		{
			this->m_PowerSetting.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumFsNotifyChangeCallbackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, FsNotifyChangeCallback, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(PLUGPLAY_STRUCT);
		PPLUGPLAY_STRUCT temp = (PPLUGPLAY_STRUCT)OutBuffer.get();
		while (Count--)
		{
			this->m_FsNotifyChange.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumPlugPlayCallbackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, PlugPlayCallback, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumLastShutdownCallbackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, LastShutdownCallback, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(SHUTDOWN_STRUCT);
		PSHUTDOWN_STRUCT temp = (PSHUTDOWN_STRUCT)OutBuffer.get();
		while (Count--)
		{
			this->m_LastShutdown.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumShutdownCallbackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, ShutdownCallback, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(SHUTDOWN_STRUCT);
		PSHUTDOWN_STRUCT temp = (PSHUTDOWN_STRUCT)OutBuffer.get();
		while (Count--)
		{
			this->m_Shutdown.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumBugReasonCallbackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, BugReasonCallback, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(BUG_STRUCT);
		PBUG_STRUCT temp = (PBUG_STRUCT)OutBuffer.get();
		while (Count--)
		{
			this->m_BugReason.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumBugCallbackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, BugCallback, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(BUG_STRUCT);
		PBUG_STRUCT temp = (PBUG_STRUCT)OutBuffer.get();
		while (Count--)
		{
			this->m_Bug.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumCmpCallbackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, CmpCallback, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_Cmp.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumLoadImageCallbackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, LoadImageCallback, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_LoadImage.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumThreadCallbackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, ThreadCallback, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_ThreadCallback.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumProcessorCallBackFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, ProcessorCallBack, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_ProcessorCallback.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumFilterDriverFunc()
{
	OutBuffer.reset(new BYTE[30 * sizeof(FILTERSYS_INFO)]);
	OutBuffersize = 30 * sizeof(FILTERSYS_INFO);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, EnumFilterDriver, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(FILTERSYS_INFO);
		PFILTERSYS_INFO temp = (PFILTERSYS_INFO)OutBuffer.get();
		while (Count--)
		{
			this->m_Filter.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_EnumDPCTimerFunc()
{
	OutBuffer.reset(new BYTE[256 * sizeof(DPCTIMER_STRUCT)]);
	OutBuffersize = 256 * sizeof(DPCTIMER_STRUCT);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, EnumDPCTimer, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DPCTIMER_STRUCT);
		PDPCTIMER_STRUCT temp = (PDPCTIMER_STRUCT)OutBuffer.get();
		while (Count--)
		{
			this->m_DpcTimer.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETGPTABLEFunc(UINT num)
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, GETGPTABLE, &num, sizeof(int), OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(GPTABLE_STRUCT);
		PGPTABLE_STRUCT temp = (PGPTABLE_STRUCT)OutBuffer.get();
		while (Count--)
		{
			this->m_GdtTable.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETIDTABLEFunc(UINT num)
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, GETIDTABLE, &num, sizeof(int), OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(IDTABLE_STRUCT);
		PIDTABLE_STRUCT temp = (PIDTABLE_STRUCT)OutBuffer.get();
		while (Count--)
		{
			this->m_IdtTable.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETSCSIHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETSCSIHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_SCSI.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETACPIHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETACPIHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_ACPI.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETATAPIHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETATAPIHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_ATAPI.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETDISKHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETDISKHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_DISK.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETPARTMGRHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETPARTMGRHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	else
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_PARTMGR.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETI8042PRTHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETI8042PRTHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	else
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_I8042PRT.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETKBDHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETKBDHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	else
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_KBD.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETMOUSEHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETMOUSEHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	else
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_MOUSE.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETFSDHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * 2 * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * 2 * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETFSDHOOKLIST, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	else
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_FSD.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETTCPIPHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETTCPIPHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	else
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_TCPIP.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETNSIPROXYHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETNSIPROXYHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	else
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_NSIPROXY.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETTDXHOOKFunc()
{
	OutBuffer.reset(new BYTE[0x1C * sizeof(DWORD64)]);
	OutBuffersize = 0x1C * sizeof(DWORD64);
	DWORD buffersize = 0, status = TRUE;
	if (!DeviceIoControl(hdevice, GETTDXHOOK, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize, nullptr))
	{
		status = GetLastError();
	}
	else
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_TDX.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETSSDTABLEFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, GETSSDTABLE, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if(status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_SSDT.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}

BOOL CSoftwareRing0::m_GETSSDTSHADOWTABLEFunc()
{
	SafeFree(OutBuffer, OutBuffersize);
	DWORD buffersize = 0, status = TRUE;
	while (!SV_GetDriverInfo(hdevice, GETSSDTSHADOWTABLE, nullptr, 0, OutBuffer.get(), OutBuffersize, &buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (OutBuffer)
			{
				OutBuffer.release();
				OutBuffer = nullptr;
			}
			OutBuffer.reset(new BYTE[buffersize]);
			OutBuffersize = buffersize;
		}
		else
		{
			status = GetLastError();
			break;
		}
	}
	if (status == TRUE)
	{
		DWORD Count = OutBuffersize / sizeof(DWORD64);
		PDWORD64 temp = (PDWORD64)OutBuffer.get();
		while (Count--)
		{
			this->m_SHADOWSSDT.emplace_back(*temp);
			temp++;
		}
	}
	return status;
}



BOOL CSoftwareRing0::SV_GetDriverInfo(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned)
{
	if (!DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, nullptr))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			DWORD ttt = 0;;
			DWORD Num;
			DWORD tempreturns = 0;
			if (DeviceIoControl(hDevice, dwIoControlCode, &ttt, sizeof(DWORD), &Num, sizeof(DWORD), &tempreturns, nullptr))
			{
				if (Num)
				{
					SetLastError(ERROR_INSUFFICIENT_BUFFER);
					*lpBytesReturned = Num;
				}
				else
					return TRUE;
			}
			else
				SetLastError(ERROR_INVALID_PARAMETER);
			return FALSE;
		}
		else
			return FALSE;
	}
	return TRUE;
}
