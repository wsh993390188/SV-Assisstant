#pragma once
#include <taskschd.h>

enum SV_SERVICE_STATUS
{
	CONTINUE_PENDING,
	PAUSE_PENDING,
	PAUSED,
	RUNNING,
	START_PENDING,
	STOPPED,
	SERVICE_UNKNOWN
};

enum SERVICE_START_TYPE
{
	AUTO_START,
	DEMAND_START,
	DISABLED,
	UNKNOWN_START
};

struct Service_Struct
{
	SV_SERVICE_STATUS CurrentStatus;
	SERVICE_START_TYPE Startype;
	std::wstring ServiceName;
	std::wstring ServiceDisplayName;
	std::wstring ServicePath;
	std::wstring descripton;
};

struct RegKeyDetail
{
	std::wstring Type;
	std::wstring sRegKeyName;
	std::wstring sRegKeyData;
};

struct TaskScheduler_Struct
{
	std::wstring TaskName;
	TASK_STATE taskstate;
};

struct ConnInfo
{
	UCHAR	uPro;
	std::wstring uState;
	std::string	dwLocalIp;
	std::string	dwRemoteIp;
	USHORT	nLocalPort;
	USHORT	nRemotePort;
	DWORD	nPId;
	std::wstring strPath;
};