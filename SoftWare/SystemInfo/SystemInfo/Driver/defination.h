#pragma once

typedef struct _M_EPROCESS M_EPROCESS, *PM_EPROCESS;
typedef struct _DRIVER_MODULE DRIVER_MODULE, *PDRIVER_MODULE;
typedef struct _DRIVER_ADDRESS DRIVER_ADDRESS, *PDRIVER_ADDRESS;
typedef struct _SHUTDOWN_STRUCT SHUTDOWN_STRUCT, *PSHUTDOWN_STRUCT;
typedef struct _PLUGPLAY_STRUCT PLUGPLAY_STRUCT, *PPLUGPLAY_STRUCT;
typedef struct _BUG_STRUCT BUG_STRUCT, *PBUG_STRUCT;
typedef struct _DPCTIMER_STRUCT DPCTIMER_STRUCT, *PDPCTIMER_STRUCT;

typedef enum _ADDR_TYPE
{
	EMPTY,
	DRIVEROBJ,
	LDR_TABLE
}_ADDR_TYPE, Driver_ADDR_Type;

struct _M_EPROCESS
{
	DWORD64 EProcess_ADDR;
	DWORD64 UniqueProcessId;
	DWORD64	ParentProcessID;
	WCHAR	FullName[0xff];
};

struct _DRIVER_ADDRESS
{
	Driver_ADDR_Type Type;
	union
	{
		DWORD64 LDR_Table_Address;
		DWORD64 Driver_Object_Address;
	};
};

struct _DRIVER_MODULE
{
	DRIVER_ADDRESS address;
	DWORD64 SizeOfImage;
	DWORD64	Driver_Entry;
	DWORD64	Driver_DllBase;
	WCHAR	FullDllName[0xff];
	WCHAR	ServiceName[0x30];
};

struct _SHUTDOWN_STRUCT
{
	DWORD64 SHUTDOWN;
	DWORD64 DEVICE_OBJECT;
	WCHAR	DriverName[0x30];
};

struct _BUG_STRUCT
{
	DWORD64 BUG_Address;
	UCHAR	DriverName[0x30];
};

struct _PLUGPLAY_STRUCT
{
	DWORD64 PlugPlay_Address;
	WCHAR	DriverName[0x30];
};

struct _DPCTIMER_STRUCT
{
	DWORD64 DueTime;
	ULONG Period;
	DWORD64 dpc;
	DWORD64 DeferredRoutine;
	WCHAR	DriverName[0x30];
};

//检测到的过滤驱动的信息
typedef struct _FILTERSYS_INFO {
	ULONG ObjectType;
	WCHAR SysName[64];
	WCHAR SysPath[260];
	DWORD64 AttachDevice;
	WCHAR HostSysName[64];

}FILTERSYS_INFO, *PFILTERSYS_INFO;

typedef struct _GPTABLE_STRUCT
{
	SIZE_T Index;
	SIZE_T Base;
	SIZE_T Limit;
	LONG  Type;
	char SegmentTypes[20];
	ULONG DPl;
	char size[3];
	char Granularity[3];
	char Present[3];
	char LongMode[3];
	int    Flags;
}GPTABLE_STRUCT, *PGPTABLE_STRUCT;

typedef struct _IDTABLE_STRUCT
{
	int CPUNum;
	SIZE_T InterruptNums;
	DWORD64 InterruptAddress;
}IDTABLE_STRUCT, *PIDTABLE_STRUCT;

typedef struct _SYSTEM_SERVICE_TABLE {
	PVOID   ServiceTableBase;
	PVOID   ServiceCounterTableBase;
	ULONGLONG   NumberOfServices;
	PVOID   ParamTableBase;
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;