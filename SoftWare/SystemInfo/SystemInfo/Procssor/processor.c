#include "../Driver/Driver.h"

NTSTATUS EnumProcessList(
	_In_ WDFREQUEST Request,
	_In_ size_t InputBufferLength,
	_In_ size_t OutputBufferLength
)
{
	if (!OutputBufferLength || !InputBufferLength)
	{
		return STATUS_INVALID_PARAMETER;
	}
	NTSTATUS status = STATUS_SUCCESS;
	PM_EPROCESS InputBuffer = NULL;//DeviceIoControl的输入buffer
	size_t InBufferlength = 0;
	status = WdfRequestRetrieveInputBuffer(Request, sizeof(M_EPROCESS), &InputBuffer, &InBufferlength);
	if (!NT_SUCCESS(status))
	{
		return STATUS_UNSUCCESSFUL;
	}

	DWORD64   dwPidOffset = GetPlantformDependentInfo(PROCESS_ID_OFFSET);
	DWORD64   dwPNameOffset = GetPlantformDependentInfo(FILE_NAME_OFFSET);
	DWORD64   dwPLinkOffset = GetPlantformDependentInfo(PROCESS_LINK_OFFSET);
	DWORD64	  dwfullNameOffset = GetPlantformDependentInfo(FULL_FILE_NAME_OFFSET);
	DWORD64	  dwInheritedidOffset = GetPlantformDependentInfo(InheritedFromUniqueProcessId_OFFSET);

	LIST_ENTRY*     ActiveProcessLinks = NULL;
	KdPrint(("PidOff=0x%X NameOff=0x%X LinkOff=0x%X\n", dwPidOffset, dwPNameOffset, dwPLinkOffset));

	DWORD64 EProcess;
	if (InputBuffer->EProcess_ADDR == 0)
	{
		EProcess = (DWORD64)PsGetCurrentProcess();
	}
	else
	{
		EProcess = InputBuffer->EProcess_ADDR;
		ActiveProcessLinks = (LIST_ENTRY*)((DWORD64)EProcess + dwPLinkOffset);
		EProcess = (DWORD64)((DWORD64)ActiveProcessLinks->Flink - dwPLinkOffset);
	}

	PM_EPROCESS OutputBuffer = NULL;//DeviceIoControl的输出buffer
	size_t OutputBufferlength = 0;
	status = WdfRequestRetrieveOutputBuffer(Request, sizeof(M_EPROCESS), &OutputBuffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		return STATUS_UNSUCCESSFUL;
	}
	RtlZeroMemory(OutputBuffer, OutputBufferlength);
	status = STATUS_SUCCESS;

	__try {
		DWORD64   dwProcessId = *((DWORD64*)(EProcess + dwPidOffset));
		PUCHAR  pImageFileName = (PUCHAR)(EProcess + dwPNameOffset);
		DWORD64   dwInheritedFromUniqueProcessId = *((DWORD64*)(EProcess + dwInheritedidOffset));
		PSE_AUDIT_PROCESS_CREATION_INFO	fullprocessorNamePtr = (PSE_AUDIT_PROCESS_CREATION_INFO)(EProcess + dwfullNameOffset);
		KdPrint(("[Pid=%8d] EProcess=0x%016X Name:%s ", dwProcessId, EProcess, pImageFileName));
		KdPrint(("Owner Processor ID : %8d\n", dwInheritedFromUniqueProcessId));
		if (fullprocessorNamePtr->ImageFileName != NULL)
		{
			RtlCopyMemory(OutputBuffer->FullName, fullprocessorNamePtr->ImageFileName->Name.Buffer, fullprocessorNamePtr->ImageFileName->Name.Length);
			KdPrint(("Full Process Name: %wZ\n", fullprocessorNamePtr->ImageFileName->Name));
		}
		OutputBuffer->EProcess_ADDR = EProcess;
		OutputBuffer->ParentProcessID = dwInheritedFromUniqueProcessId;
		OutputBuffer->UniqueProcessId = dwProcessId;
	}
	__except (1) {
		KdPrint(("EnumProcessList exception !"));
	}
	return status;
}

DWORD GetAllProcessorNum()
{
	DWORD   dwCount = 0;
	DWORD64   dwPLinkOffset = GetPlantformDependentInfo(PROCESS_LINK_OFFSET);
	LIST_ENTRY*     ActiveProcessLinks = NULL;
	DWORD64 EProcess = (DWORD64)PsGetCurrentProcess();
	DWORD64 FirstEProcess = EProcess;
	__try {
		while (EProcess != 0)
		{
			dwCount++;
			ActiveProcessLinks = (LIST_ENTRY*)((DWORD64)EProcess + dwPLinkOffset);
			//KdPrint(("ActiveProcessLinks->Flink =0x%016X\n", (DWORD64)ActiveProcessLinks->Flink));
			EProcess = (DWORD64)((DWORD64)ActiveProcessLinks->Flink - dwPLinkOffset);

			if (EProcess == FirstEProcess)
				break;
		}
		KdPrint(("ProcessNum = %d\n", dwCount));
	}
	__except (1) {
		KdPrint(("GetAllProcessorNum exception !"));
	}

	return dwCount;
}
