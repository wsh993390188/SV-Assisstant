#pragma once
#include <windows.h>
#include <list>
#include "Driver.h"

//-----------------------------------------------------------------------------
//
// Global
//
//-----------------------------------------------------------------------------
extern HANDLE gHandle;
extern TCHAR gDriverPath[MAX_PATH];

class CSoftwareRing0
{
public:
	CSoftwareRing0();
	~CSoftwareRing0();
public:
	void Exec();

	void UpdateData();

protected:
	BOOL m_EnumProcessFunc();

	BOOL m_EnumDrivermoduleFunc();

	BOOL m_EnumPowerSettingCallbackFunc();

	BOOL m_EnumFsNotifyChangeCallbackFunc();

	BOOL m_EnumPlugPlayCallbackFunc();

	BOOL m_EnumLastShutdownCallbackFunc();

	BOOL m_EnumShutdownCallbackFunc();

	BOOL m_EnumBugReasonCallbackFunc();

	BOOL m_EnumBugCallbackFunc();

	BOOL m_EnumCmpCallbackFunc();

	BOOL m_EnumLoadImageCallbackFunc();

	BOOL m_EnumThreadCallbackFunc();

	BOOL m_EnumProcessorCallBackFunc();

	BOOL m_EnumFilterDriverFunc();

	BOOL m_EnumDPCTimerFunc();

	BOOL m_GETGPTABLEFunc(UINT num);

	BOOL m_GETIDTABLEFunc(UINT num);

	BOOL m_GETSCSIHOOKFunc();

	BOOL m_GETACPIHOOKFunc();

	BOOL m_GETATAPIHOOKFunc();

	BOOL m_GETDISKHOOKFunc();

	BOOL m_GETPARTMGRHOOKFunc();

	BOOL m_GETI8042PRTHOOKFunc();

	BOOL m_GETKBDHOOKFunc();

	BOOL m_GETMOUSEHOOKFunc();

	BOOL m_GETFSDHOOKFunc();

	BOOL m_GETTCPIPHOOKFunc();

	BOOL m_GETNSIPROXYHOOKFunc();

	BOOL m_GETTDXHOOKFunc();

	BOOL m_GETSSDTABLEFunc();

	BOOL m_GETSSDTSHADOWTABLEFunc();
private:
	BOOL SoftwareRing0_Initialize();
	void Destory();
	void inline SafeFree(std::unique_ptr<BYTE[]>& x, DWORD &count);

	BOOL SV_GetDriverInfo(
			_In_ HANDLE hDevice,
			_In_ DWORD dwIoControlCode,
			_In_reads_bytes_opt_(nInBufferSize) LPVOID lpInBuffer,
			_In_ DWORD nInBufferSize,
			_Out_writes_bytes_to_opt_(nOutBufferSize, *lpBytesReturned) LPVOID lpOutBuffer,
			_In_ DWORD nOutBufferSize,
			_Out_opt_ LPDWORD lpBytesReturned
		);
	

private:
	HANDLE		hdevice;			//Éè±¸¾ä±ú
	HMODULE		library;		//wdfCoInstall dll
	DWORD		dwNumberOfProcessors;
	std::unique_ptr<BYTE[]> OutBuffer;
	DWORD OutBuffersize;
public:
	std::list<M_EPROCESS> m_Processor;
	std::list<DRIVER_MODULE> Driver_Module;
	std::list<PLUGPLAY_STRUCT> m_PowerSetting;
	std::list<PLUGPLAY_STRUCT> m_FsNotifyChange;
	std::list<SHUTDOWN_STRUCT> m_LastShutdown;
	std::list<SHUTDOWN_STRUCT> m_Shutdown;
	std::list<BUG_STRUCT> m_BugReason;
	std::list<BUG_STRUCT> m_Bug;
	std::list<DWORD64> m_Cmp;
	std::list<DWORD64> m_LoadImage;
	std::list<DWORD64> m_ProcessorCallback;
	std::list<DWORD64> m_ThreadCallback;
	std::list<FILTERSYS_INFO> m_Filter;
	std::list<DPCTIMER_STRUCT> m_DpcTimer;
	std::list<GPTABLE_STRUCT> m_GdtTable;
	std::list<IDTABLE_STRUCT> m_IdtTable;

	std::list<DWORD64> m_SCSI;
	std::list<DWORD64> m_ACPI;
	std::list<DWORD64> m_ATAPI;
	std::list<DWORD64> m_DISK;
	std::list<DWORD64> m_PARTMGR;
	std::list<DWORD64> m_I8042PRT;
	std::list<DWORD64> m_KBD;
	std::list<DWORD64> m_MOUSE;
	std::list<DWORD64> m_FSD;
	std::list<DWORD64> m_TCPIP;
	std::list<DWORD64> m_TDX;
	std::list<DWORD64> m_NSIPROXY;

	std::list<DWORD64> m_SSDT;
	std::list<DWORD64> m_SHADOWSSDT;
};

inline void CSoftwareRing0::SafeFree(std::unique_ptr<BYTE[]>& x, DWORD & count)
{
	x = nullptr;
	count = 0;
}
