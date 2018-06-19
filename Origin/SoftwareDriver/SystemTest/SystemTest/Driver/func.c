#include "Driver.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, GetPlantformDependentInfo)
#pragma alloc_text (PAGE, GetFunctionAddr)
#endif

DWORD64 GetPlantformDependentInfo(DWORD64 dwFlag)
{
	PAGED_CODE();
	PRTL_OSVERSIONINFOW VerInfo = ExAllocatePool(
		PagedPool, 
		sizeof(RTL_OSVERSIONINFOW)
		);

	if (!NT_SUCCESS(RtlGetVersion(VerInfo)))
		return 0;
	DWORD64 MajorVer = VerInfo->dwMajorVersion;
	DWORD64 MinorVer = VerInfo->dwMinorVersion;
	//KdPrint(("MajorVer=0x%X MinorVer=0x%X\n", MajorVer, MinorVer));
	DWORD64 ret = 0;

	switch (dwFlag)
	{
	case EPROCESS_SIZE:
		break;
	case PEB_OFFSET:
		break;
	case FILE_NAME_OFFSET:
		ret = 0x450;
		break;
	case PROCESS_LINK_OFFSET:
		ret = 0x2e8;
		break;
	case PROCESS_ID_OFFSET:
		ret = 0x02E0;
		break;
	case EXIT_TIME_OFFSET:
		break;
	case FULL_FILE_NAME_OFFSET:
		ret = 0x468;
		break;
	case InheritedFromUniqueProcessId_OFFSET:
		ret = 0x3e0;
		break;
	default:
		break;
	}

	ExFreePool(VerInfo);
	return ret;
}

DWORD64 GetFunctionAddr(IN PCWSTR FunctionName)
{
	UNICODE_STRING UniCodeFunctionName;
	RtlInitUnicodeString(&UniCodeFunctionName, FunctionName);
	return (DWORD64)MmGetSystemRoutineAddress(&UniCodeFunctionName);
}




