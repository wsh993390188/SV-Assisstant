#include "../Driver/Driver.h"
#include <intrin.h>

DWORD64 KeGetCurrentPrcr()
{
	KeSetSystemAffinityThread(1);
	DWORD64 KiKPCRBase = __readmsr(0xC0000101); //gs Base
	KeRevertToUserAffinityThread();
	return KiKPCRBase;
}

KDPC* TransTimerDPCEx(PKTIMER Timer, ULONG64 KiWaitNever, ULONG64 KiWaitAlways)
{
	ULONG64  DPC = (ULONG64)Timer->Dpc;     //Time 
	DPC ^= KiWaitNever;
	DPC = _rotl64(DPC, (UCHAR)(KiWaitNever & 0xFF));
	DPC ^= (ULONG64)Timer;
	DPC = _byteswap_uint64(DPC);
	DPC ^= KiWaitAlways;
	return (KDPC*)DPC;
}

DWORD64 GetPcrbInfo(DWORD64 dwFlag)
{
	PRTL_OSVERSIONINFOW VerInfo = ExAllocatePool(
		PagedPool,
		sizeof(RTL_OSVERSIONINFOW)
	);

	if (!NT_SUCCESS(RtlGetVersion(VerInfo)))
		return 0;
	DWORD64 MajorVer = VerInfo->dwMajorVersion;
	DWORD64 MinorVer = VerInfo->dwMinorVersion;

	DWORD64 ret = 0;

	switch (dwFlag)
	{
	case TimerTable:
		ret = 0x3680;
		break;
	default:
		break;
	}
	ExFreePool(VerInfo);
	return ret;
}

BOOLEAN GetKiWaitVariableAddress(PULONG64 KiWaitNever, PULONG64 KiWaitAlways)
{
	/*
	2: kd> u kesettimer l 50
	nt!KeSetTimer:
	fffff802`e8ab3f40 4883ec38        sub     rsp,38h
	fffff802`e8ab3f44 4c89442420      mov     qword ptr [rsp+20h],r8
	fffff802`e8ab3f49 4533c9          xor     r9d,r9d
	fffff802`e8ab3f4c 4533c0          xor     r8d,r8d
	fffff802`e8ab3f4f e80c000000      call    nt!KiSetTimerEx (fffff802`e8ab3f60)
	fffff802`e8ab3f54 4883c438        add     rsp,38h
	fffff802`e8ab3f58 c3              ret
	nt!KiSetTimerEx:
	fffff802`e8ab3f60 48895c2408      mov     qword ptr [rsp+8],rbx
	fffff802`e8ab3f65 48896c2410      mov     qword ptr [rsp+10h],rbp
	fffff802`e8ab3f6a 4889742418      mov     qword ptr [rsp+18h],rsi
	fffff802`e8ab3f6f 57              push    rdi
	fffff802`e8ab3f70 4154            push    r12
	fffff802`e8ab3f72 4155            push    r13
	fffff802`e8ab3f74 4156            push    r14
	fffff802`e8ab3f76 4157            push    r15
	fffff802`e8ab3f78 4883ec50        sub     rsp,50h
	fffff802`e8ab3f7c 488b053db43500  mov     rax,qword ptr [nt!KiWaitNever (fffff802`e8e0f3c0)]
	fffff802`e8ab3f83 488bf9          mov     rdi,rcx
	fffff802`e8ab3f86 488b35f3b53500  mov     rsi,qword ptr [nt!KiWaitAlways (fffff802`e8e0f580)]
	fffff802`e8ab3f8d 450fb6f1        movzx   r14d,r9b
	*/
	ULONG64 KeSetTimer = 0;
	PUCHAR StartSearchAddress = 0;
	PUCHAR EndSearchAddress = 0;
	INT64   iOffset = 0;

	//通过函数名字获取函数地址
	KeSetTimer = (ULONG64)GetExportVariableFormNtosExportTableByVariableName(L"KeSetTimer");
	//查找的范围
	StartSearchAddress = (PUCHAR)KeSetTimer;
	EndSearchAddress = StartSearchAddress + 0x500;

	for (PUCHAR i = StartSearchAddress; i<EndSearchAddress; i++)
	{
		if (*i == 0x48 && *(i + 1) == 0x8B && *(i + 2) == 0x05)
		{
			memcpy(&iOffset, i + 3, 4);
			//两个密码的值
			*KiWaitNever = iOffset + (ULONG64)i + 7;
			i = i + 7;
			memcpy(&iOffset, i + 3, 4);
			*KiWaitAlways = iOffset + (ULONG64)i + 7;
			return TRUE;
		}
	}
	return FALSE;
}

PVOID
GetExportVariableFormNtosExportTableByVariableName(WCHAR *VariableName)
{
	UNICODE_STRING v1;
	PVOID ExportVariable = NULL;
	if (VariableName && wcslen(VariableName) > 0)
	{
		RtlInitUnicodeString(&v1, VariableName);
		//从Ntos模块的导出表中获得一个导出变量的地址
		ExportVariable = MmGetSystemRoutineAddress(&v1);
	}
	return ExportVariable;
}

NTSTATUS EnumDPCTimerInfo(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS			status = STATUS_SUCCESS;
	PLIST_ENTRY         ptrListEntry = NULL;
	PLIST_ENTRY         ptrListEntryHead = NULL;

	KIRQL oldirq = KeRaiseIrqlToDpcLevel();
	PKPCR _ckpcr = (PKPCR)KeGetCurrentPrcr();
	DWORD64 timetable_offset = GetPcrbInfo(TimerTable);
	PKTIMER_TABLE TimeTable = (PKTIMER_TABLE)((DWORD64)_ckpcr + 0x180 + timetable_offset);
	ULONG64 KiWaitNever = 0, KiWaitAlways = 0;
	if (!GetKiWaitVariableAddress(&KiWaitNever, &KiWaitAlways))
	{
		status = STATUS_UNSUCCESSFUL;
		KeLowerIrql(oldirq);
		return status;
	}
	size_t OutputBufferlength = 0;
	PDPCTIMER_STRUCT buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, KTIMER_TABLE_ENTRY_MAX * sizeof(DPCTIMER_STRUCT), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	for (int i = 0; i < KTIMER_TABLE_ENTRY_MAX; i++)
	{
		ptrListEntryHead = &TimeTable->TimerEntries[i].Entry;
		ptrListEntry = ptrListEntryHead;
		while (ptrListEntry->Flink != ptrListEntryHead)
		{
			PKTIMER ptrTimer = CONTAINING_RECORD(ptrListEntry, KTIMER, TimerListEntry);
			if (MmIsAddressValid(ptrTimer) && MmIsAddressValid(ptrTimer->Dpc))
			{
				PKDPC DecDpc = TransTimerDPCEx(ptrTimer, KiWaitNever, KiWaitAlways);
				KdPrint(("DPC Timer: 0x%p", ptrTimer->Dpc));
				KdPrint(("DPC Timer Due Time: %L", ptrTimer->DueTime));
				buffer->DueTime = ptrTimer->DueTime.QuadPart;
				KdPrint(("DPC Timer Period: %d", ptrTimer->Period));
				buffer->Period = ptrTimer->Period;
				buffer->dpc = (DWORD64)DecDpc;
				if (MmIsAddressValid(DecDpc))
				{
					KdPrint(("[dpc]dpc:0x%p,routine:0x%p\n", DecDpc, DecDpc->DeferredRoutine));
					buffer->DeferredRoutine = (DWORD64)DecDpc->DeferredRoutine;
					PDEVICE_OBJECT pDevObj = (PDEVICE_OBJECT)DecDpc->DeferredContext;
					if (MmIsAddressValid(pDevObj) && MmIsAddressValid(pDevObj->DriverObject) && !pDevObj->DriverObject->DriverName.Length)
					{
						KdPrint(("DPC Moudle: %wZ", pDevObj->DriverObject->DriverName));
						RtlCopyMemory(buffer->DriverName, pDevObj->DriverObject->DriverName.Buffer, pDevObj->DriverObject->DriverName.MaximumLength);
					}
				}
			}
			buffer++;
			ptrListEntry = ptrListEntry->Flink;
		}
	}
	KeLowerIrql(oldirq);
	return status;
}

