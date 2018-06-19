#include "../Driver/Driver.h"
//失败 不会搞啊 系统似乎把实现从符号表中剔除了 这个方式会导致某个漏洞？？？？？

const char* const MSG_HOOOKTYPES[] = {
	"WH_MSGFILTER",
	"WH_JOURNALRECORD",
	"WH_JOURNALPLAYBACK",
	"WH_KEYBOARD",
	"WH_GETMESSAGE",
	"WH_CALLWNDPROC",
	"WH_CBT",
	"WH_SYSMSGFILTER",
	"WH_MOUSE",
	"WH_HARDWARE",
	"WH_DEBUG",
	"WH_SHELL",
	"WH_FOREGROUNDIDLE",
	"WH_CALLWNDPROCRET",
	"WH_KEYBOARD_LL",
	"WH_MOUSE_LL"
};

NTSTATUS MsgHook_GetAddress(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength)
{
	NTSTATUS state = STATUS_SUCCESS;
	if (InputBufferLength == 0)
		return STATUS_INVALID_PARAMETER;
	PVOID InputBuffer = NULL;//DeviceIoControl的输入buffer
	size_t InBufferlength = 0;
	PVOID Destination = NULL;
	PEXCEPTION_POINTERS Ep = NULL;
	DWORD Error_Code = 0;
	state = WdfRequestRetrieveInputBuffer(Request, sizeof(DWORD64), &InputBuffer, &InBufferlength);
	if (!NT_SUCCESS(state))
	{
		return STATUS_UNSUCCESSFUL;
	}
	__try {
		DWORD64 rwkm_adr = *(PDWORD64)InputBuffer;
		DWORD64 rwkm_len = *(PDWORD64)(rwkm_adr + 16);
		Destination = ExAllocatePool(PagedPool, rwkm_len);
		if (!Destination)	return STATUS_NO_MEMORY;
		state = VxkCopyMemory(Destination, (PVOID)rwkm_adr, rwkm_len);
		if (NT_SUCCESS(state))
		{
			PVOID test = ExAllocatePool(PagedPool, sizeof(HOOK_INFO));
			if (!test)
			{
				state = STATUS_NO_MEMORY;
			}
			else
			{
				for (DWORD i = 0; i < rwkm_len; i++)
				{
					RtlZeroMemory(test, sizeof(HOOK_INFO));
					PHANDLEENTRY temp = (PHANDLEENTRY)((DWORD64)Destination + i * sizeof(HANDLEENTRY));
					if (temp->phead)
					{
						VxkCopyMemory(test, &temp->phead, sizeof(HOOK_INFO));
						if (MmIsAddressValid((PHOOK_INFO)test))
						{
							PHOOK_INFO Hook = test;
// 							KdPrint(("hHandle:     0x%llx\n", Hook->hHandle));
// 							KdPrint(("iHookFlags:  %d\n", Hook->iHookFlags));
// 							KdPrint(("iHookType:   %s\n", MSG_HOOOKTYPES[Hook->iHookType + 1]));
// 							KdPrint(("OffPfn:      0x%llx\n", Hook->OffPfn));
// 							KdPrint(("ETHREAD:     0x%llx\n", GetQWORD((UINT64)(Hook.Win32Thread)));
// 							KdPrint(("ProcessName: %s\n\n", GetPNbyET(GetQWORD((UINT64)(Hook.Win32Thread))));
						}
					}
				}
				ExFreePool(test);
			}
		}
		ExFreePool(Destination);
	}
	__except (Ep = GetExceptionInformation(), Error_Code = GetExceptionCode())
	{
		DbgBreakPoint();
		if (!Destination)
			ExFreePool(Destination);
	}
	return state;
}

NTSTATUS VxkCopyMemory(PVOID pDestination, PVOID pSourceAddress, SIZE_T SizeOfCopy)
{
	PMDL pMdl = NULL;
	PVOID pSafeAddress = NULL;
	pMdl = IoAllocateMdl(pSourceAddress, (ULONG)SizeOfCopy, FALSE, FALSE, NULL);
	if (!pMdl) return STATUS_UNSUCCESSFUL;
	__try
	{
		MmProbeAndLockPages(pMdl, KernelMode, IoReadAccess);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		IoFreeMdl(pMdl);
		return STATUS_UNSUCCESSFUL;
	}
	pSafeAddress = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
	if (!pSafeAddress) return STATUS_UNSUCCESSFUL;
	if (pDestination)
		RtlCopyMemory(pDestination, pSafeAddress, SizeOfCopy);
	MmUnlockPages(pMdl);
	IoFreeMdl(pMdl);
	return STATUS_SUCCESS;
}
