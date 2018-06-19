#include "../../Driver/Driver.h"

NTSTATUS GetPspSetCreateProcessNotifyRoutine(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"PsSetCreateProcessNotifyRoutine");
	for (int i = 0; i<0xff; i++)
	{
		/*
		fffff802`381ba1d0 4883ec28        sub     rsp,28h
		fffff802`381ba1d4 8ac2            mov     al,dl
		fffff802`381ba1d6 33d2            xor     edx,edx
		fffff802`381ba1d8 84c0            test    al,al
		fffff802`381ba1da 0f95c2          setne   dl
		fffff802`381ba1dd e8d2000000      call    nt!PspSetCreateProcessNotifyRoutine (fffff802`381ba2b4)
		fffff802`381ba1e2 4883c428        add     rsp,28h
		*/
		if ((*(UCHAR*)Base == 0xc2) && (*(UCHAR*)(Base + 1) == 0xe8))
		{
			DWORD OffsetAddrTemp;
			DWORD64 PspSetCreateProcessNotifyRoutine_Address = 0;
			OffsetAddrTemp = *((DWORD*)(Base + 2));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 6);
			PspSetCreateProcessNotifyRoutine_Address = ((DWORD64)Base & 0xFFFFFFFF00000000);
			PspSetCreateProcessNotifyRoutine_Address |= OffsetAddrTemp;
			KdPrint(("PspSetCreateProcessNotifyRoutine: 0x%llx\n", PspSetCreateProcessNotifyRoutine_Address));
			*Address = PspSetCreateProcessNotifyRoutine_Address;
			break;
		}
		Base++;
	}

	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetProcessorCallbackAddress(
	_In_ WDFREQUEST Request,
	_In_ size_t InputBufferLength,
	_In_ size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 PspSetCreateProcessNotifyRoutine_Addr = 0;
	status = GetPspSetCreateProcessNotifyRoutine(&PspSetCreateProcessNotifyRoutine_Addr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PspSetCreateProcessNotifyRoutine Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	DWORD64 PspCreateProcessNotifyRoutine_Addr = 0;
	status = GetPspCreateProcessNotifyRoutine(PspSetCreateProcessNotifyRoutine_Addr, &PspCreateProcessNotifyRoutine_Addr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PspSetCreateProcessNotifyRoutine Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	DWORD64 PspCreateProcessNotifyRoutine_Count = 0;
	status = GetPspCreateProcessNotifyRoutineCount(PspSetCreateProcessNotifyRoutine_Addr, &PspCreateProcessNotifyRoutine_Count);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PspSetCreateProcessNotifyRoutine Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	size_t OutputBufferlength = 0;
	PDWORD64 buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, PspCreateProcessNotifyRoutine_Count * sizeof(DWORD64), &buffer, &OutputBufferlength);

	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*buffer = PSP_MAX_CREATE_PROCESS_NOTIFY * sizeof(DWORD64);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	 RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;

	// 	PspCreateProcessNotifyRoutine_Addr = 0;
	// 	if (!PspCreateProcessNotifyRoutine_Addr)
	// 		return STATUS_UNSUCCESSFUL;
	PVOID MagicPtr, NotifyAddr;

	for (DWORD i = 0; i < PSP_MAX_CREATE_PROCESS_NOTIFY; i++)
	{
		MagicPtr = (PVOID)((PUCHAR)PspCreateProcessNotifyRoutine_Addr + i * sizeof(ULONG_PTR));
		if (MagicPtr == NULL)continue;

		(DWORD64)NotifyAddr = *(PULONG_PTR)(MagicPtr);
		if (NotifyAddr == NULL)continue;

		if (MmIsAddressValid(NotifyAddr) && NotifyAddr != NULL)
		{
			(DWORD64)NotifyAddr = *(PULONG_PTR)(((ULONG_PTR)NotifyAddr & 0xfffffffffffffff0ui64) + sizeof(EX_RUNDOWN_REF));
			*buffer = (DWORD64)NotifyAddr;
			KdPrint(("LoadProcessNotify at 0x%llx\n", NotifyAddr));
			buffer++;
		}
	}

	return STATUS_SUCCESS;
}

NTSTATUS GetPspCreateProcessNotifyRoutineCount(DWORD64 PspSetCreateProcessNotifyRoutine_Addr, DWORD64 * RoutineCount)
{
	if (!PspSetCreateProcessNotifyRoutine_Addr)
		return STATUS_INVALID_PARAMETER;

	//	fffff802`381ba344 f0ff05353b2400  lock inc dword ptr [nt!PspCreateProcessNotifyRoutineCount (fffff802`383fde80)]
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	UCHAR* Base = (PUCHAR)PspSetCreateProcessNotifyRoutine_Addr;
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0xf0) && (*(UCHAR*)(Base + 1) == 0xff))
		{
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			OffsetAddr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			OffsetAddr |= OffsetAddrTemp;
			break;
		}
		Base++;
	}

	if (OffsetAddr && MmIsAddressValid((PVOID)OffsetAddr))
	{
		*RoutineCount = *(DWORD*)OffsetAddr;
	}

	if (*RoutineCount)
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetPspCreateProcessNotifyRoutine(DWORD64 PspSetCreateProcessNotifyRoutine_Addr, DWORD64 * Address)
{
	if (!PspSetCreateProcessNotifyRoutine_Addr)
		return STATUS_INVALID_PARAMETER;

	// 		nt!PspSetCreateProcessNotifyRoutine + 0x5d:
	// 			fffff802`381ba311 33db            xor     ebx, ebx
	// 			fffff802`381ba313 4c8d2d262bdfff  lea     r13, [nt!PspCreateProcessNotifyRoutine(fffff802`37face40)]
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	UCHAR* Base = (PUCHAR)PspSetCreateProcessNotifyRoutine_Addr;
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0x4c) && (*(UCHAR*)(Base + 1) == 0x8d))
		{
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			OffsetAddr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			OffsetAddr |= OffsetAddrTemp;
			*Address = OffsetAddr;
			break;
		}
		Base++;
	}
	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetPspSetCreateThreadNotifyRoutine(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"PsSetCreateThreadNotifyRoutine");
	for (int i = 0; i<0xff; i++)
	{
		/*
		nt!PsSetCreateThreadNotifyRoutine:
		fffff800`f9daefc0 4883ec28        sub     rsp,28h
		fffff800`f9daefc4 33d2            xor     edx,edx
		fffff800`f9daefc6 e80d000000      call    nt!PspSetCreateThreadNotifyRoutine (fffff800`f9daefd8)
		fffff800`f9daefcb 4883c428        add     rsp,28h
		fffff800`f9daefcf c3              ret
		*/
		if ((*(UCHAR*)Base == 0xd2) && (*(UCHAR*)(Base + 1) == 0xe8))
		{
			DWORD OffsetAddrTemp;
			DWORD64 PspSetCreateThreadNotifyRoutine_Address = 0;
			OffsetAddrTemp = *((DWORD*)(Base + 2));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 6);
			PspSetCreateThreadNotifyRoutine_Address = ((DWORD64)Base & 0xFFFFFFFF00000000);
			PspSetCreateThreadNotifyRoutine_Address |= OffsetAddrTemp;
			KdPrint(("PspSetCreateThreadNotifyRoutine: 0x%llX\n", PspSetCreateThreadNotifyRoutine_Address));
			*Address = PspSetCreateThreadNotifyRoutine_Address;
			break;
		}
		Base++;
	}

	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetThreadCallbackAddress(
	_In_ WDFREQUEST Request,
	_In_ size_t InputBufferLength,
	_In_ size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;

	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 PspSetCreateThreadNotifyRoutine_Addr = 0;
	status = GetPspSetCreateThreadNotifyRoutine(&PspSetCreateThreadNotifyRoutine_Addr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PspSetCreateThreadNotifyRoutine Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	DWORD64 PspCreateThreadNotifyRoutine_Addr = 0;
	status = GetPspCreateThreadNotifyRoutine(PspSetCreateThreadNotifyRoutine_Addr, &PspCreateThreadNotifyRoutine_Addr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PspCreateThreadNotifyRoutine Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	DWORD64 PspCreateThreadNotifyRoutine_Count = 0;
	status = GetPspCreateThreadNotifyRoutineCount(PspSetCreateThreadNotifyRoutine_Addr, &PspCreateThreadNotifyRoutine_Count);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PspCreateThreadNotifyRoutineCount Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}	

	size_t OutputBufferlength = 0;
	PDWORD64 buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, PspCreateThreadNotifyRoutine_Count * sizeof(DWORD64), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if(STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if(NT_SUCCESS(status))
			{
				*buffer = PspCreateThreadNotifyRoutine_Count * sizeof(DWORD64);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;

	PEX_CALLBACK_ROUTINE_BLOCK MagicPtr;
	PVOID NotifyAddr;

	for (DWORD i = 0; i < PspCreateThreadNotifyRoutine_Count; i++)
	{
		MagicPtr = (PEX_CALLBACK_ROUTINE_BLOCK)((PUCHAR)PspCreateThreadNotifyRoutine_Addr + i * sizeof(ULONG_PTR));
		if (MagicPtr == NULL)continue;

		(DWORD64)NotifyAddr = *(PULONG_PTR)(MagicPtr);
		if (NotifyAddr == NULL)continue;

		if (MmIsAddressValid(NotifyAddr) && NotifyAddr != NULL)
		{
			(DWORD64)NotifyAddr = *(PULONG_PTR)(((ULONG_PTR)NotifyAddr & 0xfffffffffffffff0ui64) + sizeof(EX_RUNDOWN_REF));
			*buffer = (DWORD64)NotifyAddr;
			KdPrint(("LoadThreadNotify at 0x%llx\n", NotifyAddr));
			buffer++;
		}
	}

	return STATUS_SUCCESS;
}

NTSTATUS GetPspCreateThreadNotifyRoutineCount(DWORD64 PspSetCreateThreadNotifyRoutine_Addr, DWORD64 * RoutineCount)
{
	if (!PspSetCreateThreadNotifyRoutine_Addr)
		return STATUS_INVALID_PARAMETER;
	/*
	nt!PspSetCreateThreadNotifyRoutine + 0x4e:
	fffff800`f9daf026 f0ff054b3e2400  lock inc dword ptr[nt!PspCreateThreadNotifyRoutineCount(fffff800`f9ff2e78)]
	*/
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	UCHAR* Base = (PUCHAR)PspSetCreateThreadNotifyRoutine_Addr;
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0xf0) && (*(UCHAR*)(Base + 1) == 0xff))
		{
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			OffsetAddr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			OffsetAddr |= OffsetAddrTemp;
			break;
		}
		Base++;
	}

	if (OffsetAddr && MmIsAddressValid((PVOID)OffsetAddr))
	{
		*RoutineCount = *(DWORD*)OffsetAddr;
	}

	if (*RoutineCount)
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetPspCreateThreadNotifyRoutine(DWORD64 PspSetCreateThreadNotifyRoutine_Addr, DWORD64 * Address)
{
	if (!PspSetCreateThreadNotifyRoutine_Addr)
		return STATUS_INVALID_PARAMETER;
	/*
	nt!PspSetCreateThreadNotifyRoutine+0x26:
	fffff800`f9daeffe 488d0d3b30dfff  lea     rcx,[nt!PspCreateThreadNotifyRoutine (fffff800`f9ba2040)]
	*/
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	UCHAR* Base = (PUCHAR)PspSetCreateThreadNotifyRoutine_Addr;
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0x48) && (*(UCHAR*)(Base + 1) == 0x8d))
		{
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			OffsetAddr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			OffsetAddr |= OffsetAddrTemp;
			*Address = OffsetAddr;
			break;
		}
		Base++;
	}
	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetPsSetLoadImageNotifyRoutineEx(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"PsSetLoadImageNotifyRoutine");
	for (int i = 0; i<0xff; i++)
	{
		/*
		nt!PsSetLoadImageNotifyRoutine:
		fffff800`cc639090 4883ec28        sub     rsp,28h
		fffff800`cc639094 33d2            xor     edx,edx
		fffff800`cc639096 e815000000      call    nt!PsSetLoadImageNotifyRoutineEx (fffff800`cc6390b0)
		fffff800`cc63909b 4883c428        add     rsp,28h
		*/
		if ((*(UCHAR*)Base == 0xd2) && (*(UCHAR*)(Base + 1) == 0xe8))
		{
			DWORD OffsetAddrTemp;
			DWORD64 PspSetCreateLoadImageNotifyRoutine_Address = 0;
			OffsetAddrTemp = *((DWORD*)(Base + 2));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 6);
			PspSetCreateLoadImageNotifyRoutine_Address = ((DWORD64)Base & 0xFFFFFFFF00000000);
			PspSetCreateLoadImageNotifyRoutine_Address |= OffsetAddrTemp;
			KdPrint(("PspSetLoadImageNotifyRoutine: 0x%llX\n", PspSetCreateLoadImageNotifyRoutine_Address));
			*Address = PspSetCreateLoadImageNotifyRoutine_Address;
			break;
		}
		Base++;
	}

	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetLoadImageCallbackAddress(
	WDFREQUEST Request, 
	size_t InputBufferLength, 
	size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 PspSetCreateLoadImageNotifyRoutine_Addr = 0;
	status = GetPsSetLoadImageNotifyRoutineEx(&PspSetCreateLoadImageNotifyRoutine_Addr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PsSetLoadImageNotifyRoutineEx Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	DWORD64 PspCreateLoadImageNotifyRoutine_Addr = 0;
	status = GetPspCreateLoadImageNotifyRoutine(PspSetCreateLoadImageNotifyRoutine_Addr, &PspCreateLoadImageNotifyRoutine_Addr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PspCreateLoadImageNotifyRoutine Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	DWORD64 PspCreateLoadImageNotifyRoutine_Count = 0;
	status = GetPspCreateLoadImageNotifyRoutineCount(PspSetCreateLoadImageNotifyRoutine_Addr, &PspCreateLoadImageNotifyRoutine_Count);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PspCreateThreadNotifyRoutineCount Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	size_t OutputBufferlength = 0;
	PDWORD64 buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, PspCreateLoadImageNotifyRoutine_Count * sizeof(DWORD64), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*buffer = PspCreateLoadImageNotifyRoutine_Count * sizeof(DWORD64);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;

	PEX_CALLBACK_ROUTINE_BLOCK MagicPtr;
	PVOID NotifyAddr;

	for (DWORD i = 0; i < PspCreateLoadImageNotifyRoutine_Count; i++)
	{
		MagicPtr = (PEX_CALLBACK_ROUTINE_BLOCK)((PUCHAR)PspCreateLoadImageNotifyRoutine_Addr + i * sizeof(ULONG_PTR));
		if (MagicPtr == NULL)continue;

		(DWORD64)NotifyAddr = *(PULONG_PTR)(MagicPtr);
		if (NotifyAddr == NULL)continue;

		if (MmIsAddressValid(NotifyAddr) && NotifyAddr != NULL)
		{
			(DWORD64)NotifyAddr = *(PULONG_PTR)(((ULONG_PTR)NotifyAddr & 0xfffffffffffffff0ui64) + sizeof(EX_RUNDOWN_REF));
			*buffer = (DWORD64)NotifyAddr;
			buffer++;
			KdPrint(("LoadImageNotify at 0x%llx\n", NotifyAddr));
		}
	}

	return STATUS_SUCCESS;
}

NTSTATUS GetPspCreateLoadImageNotifyRoutine(DWORD64 PspSetCreateLoadImageNotifyRoutine_Addr, DWORD64 * Address)
{
	if (!PspSetCreateLoadImageNotifyRoutine_Addr)
		return STATUS_INVALID_PARAMETER;
	/*
	nt!PsSetLoadImageNotifyRoutineEx+0x41:
	fffff800`cc6390f1 488d0d482bdfff  lea     rcx,[nt!PspLoadImageNotifyRoutine (fffff800`cc42bc40)]
	*/
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	UCHAR* Base = (PUCHAR)PspSetCreateLoadImageNotifyRoutine_Addr;
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0x48) && (*(UCHAR*)(Base + 1) == 0x8d))
		{
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			OffsetAddr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			OffsetAddr |= OffsetAddrTemp;
			*Address = OffsetAddr;
			break;
		}
		Base++;
	}
	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetPspCreateLoadImageNotifyRoutineCount(DWORD64 PspSetCreateLoadImageNotifyRoutine_Addr, DWORD64 * RoutineCount)
{
	if (!PspSetCreateLoadImageNotifyRoutine_Addr)
		return STATUS_INVALID_PARAMETER;
	/*
	nt!PsSetLoadImageNotifyRoutineEx+0x5f:
	fffff800`cc63910f f0ff05723d2400  lock inc dword ptr [nt!PspLoadImageNotifyRoutineCount (fffff800`cc87ce88)]
	*/
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	UCHAR* Base = (PUCHAR)PspSetCreateLoadImageNotifyRoutine_Addr;
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0xf0) && (*(UCHAR*)(Base + 1) == 0xff))
		{
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			OffsetAddr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			OffsetAddr |= OffsetAddrTemp;
			break;
		}
		Base++;
	}

	if (OffsetAddr && MmIsAddressValid((PVOID)OffsetAddr))
	{
		*RoutineCount = *(DWORD*)OffsetAddr;
	}

	if (*RoutineCount)
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetCmpInsertCallbackInListByAltitude(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"CmRegisterCallbackEx");
	DWORD64 CmpRegisterCallbackInternal_Address = 0;
	for (int i = 0; i<0x44; i++)
	{
		/*
		nt!CmRegisterCallbackEx+0x22:
		fffff800`cc712b62 488b442460      mov     rax,qword ptr [rsp+60h]
		fffff800`cc712b67 4c8bc2          mov     r8,rdx
		fffff800`cc712b6a 4889442428      mov     qword ptr [rsp+28h],rax
		fffff800`cc712b6f 4533c9          xor     r9d,r9d
		fffff800`cc712b72 498bd2          mov     rdx,r10
		fffff800`cc712b75 c644242000      mov     byte ptr [rsp+20h],0
		fffff800`cc712b7a e859cdf5ff      call    nt!CmpRegisterCallbackInternal (fffff800`cc66f8d8)
		*/
		if ((*(UCHAR*)Base == 0x00) && (*(UCHAR*)(Base + 1) == 0xe8))
		{
			DWORD OffsetAddrTemp;
			OffsetAddrTemp = *((DWORD*)(Base + 2));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 6);
			CmpRegisterCallbackInternal_Address = ((DWORD64)Base & 0xFFFFFFFF00000000);
			CmpRegisterCallbackInternal_Address |= OffsetAddrTemp;
			KdPrint(("CmpRegisterCallbackInternal: 0x%llX\n", CmpRegisterCallbackInternal_Address));
			break;
		}
		Base++;
	}

	if (!CmpRegisterCallbackInternal_Address) return STATUS_UNSUCCESSFUL;

	Base = (UCHAR*)CmpRegisterCallbackInternal_Address;
	for (int i = 0; i < 0xff; i++)
	{
		/*
		nt!CmpRegisterCallbackInternal+0xac:
		fffff800`e89eb984 440fb707        movzx   r8d,word ptr [rdi]
		fffff800`e89eb988 488bc8          mov     rcx,rax
		fffff800`e89eb98b 488b5708        mov     rdx,qword ptr [rdi+8]
		fffff800`e89eb98f e8ac80baff      call    nt!memcpy (fffff800`e8593a40)
		fffff800`e89eb994 408ad6          mov     dl,sil
		fffff800`e89eb997 488bcb          mov     rcx,rbx
		fffff800`e89eb99a e839000000      call    nt!CmpInsertCallbackInListByAltitude (fffff800`e89eb9d8)
		*/
		if ((*(UCHAR*)Base == 0xcb) && (*(UCHAR*)(Base + 1) == 0xe8))
		{
			DWORD OffsetAddrTemp;
			OffsetAddrTemp = *((DWORD*)(Base + 2));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 6);
			DWORD64 CmpInsertCallbackInListByAltitude_Addr = 0;
			CmpInsertCallbackInListByAltitude_Addr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			CmpInsertCallbackInListByAltitude_Addr |= OffsetAddrTemp;
			KdPrint(("CmpInsertCallbackInListByAltitude_Addr: 0x%llX\n", CmpInsertCallbackInListByAltitude_Addr));
			*Address = CmpInsertCallbackInListByAltitude_Addr;
			break;
		}
		Base++;
	}

	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetCmpRegisterCallbackAddress(
	WDFREQUEST Request, 
	size_t InputBufferLength,
	size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 CmpInsertCallbackInListByAltitude_Addr = 0;
	status = GetCmpInsertCallbackInListByAltitude(&CmpInsertCallbackInListByAltitude_Addr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get CmpInsertCallbackInListByAltitude Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	DWORD64 CallbackListHead_Addr = 0;
	status = GetCallbackListHead(CmpInsertCallbackInListByAltitude_Addr, &CallbackListHead_Addr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get CallbackListHead Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	DWORD64 CmpCallBack_Count = 0;
	status = GetCmpCallBackCount(CmpInsertCallbackInListByAltitude_Addr, &CmpCallBack_Count);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get CmpCallBackCount Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}
	size_t OutputBufferlength = 0;
	PDWORD64 buffer = NULL;
	status =  WdfRequestRetrieveOutputBuffer(Request, CmpCallBack_Count * sizeof(DWORD64), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*buffer = CmpCallBack_Count * sizeof(DWORD64);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	   RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;

	KSPIN_LOCK spin_lock; // 自旋锁
	KIRQL  irql;    // 中断级别
	KeInitializeSpinLock(&spin_lock);
	irql = KeGetCurrentIrql();
	KeAcquireSpinLock(&spin_lock, &irql);
	PLIST_ENTRY BeginPtr;
	BeginPtr = (PLIST_ENTRY)CallbackListHead_Addr;
	while (CmpCallBack_Count)
	{
		KdPrint(("vPCallbackListHead->Flink = 0x%08X\r\n", BeginPtr->Flink));
		DWORD64 vExAllocateAddr = (DWORD64)BeginPtr->Flink;
		vExAllocateAddr += 5 * sizeof(DWORD64);
		KdPrint(("RegistryCallbackFunction: 0x%llX\n", *(PDWORD64)vExAllocateAddr));
		*buffer = *(PDWORD64)vExAllocateAddr;
		buffer++;
		BeginPtr = BeginPtr->Flink;
		CmpCallBack_Count--;
	}
	KeReleaseSpinLock(&spin_lock, irql);
	return STATUS_SUCCESS;
}

NTSTATUS GetCallbackListHead(DWORD64 CmpInsertCallbackInListByAltitude_Addr, DWORD64 * Address)
{
	if (!CmpInsertCallbackInListByAltitude_Addr)
		return STATUS_INVALID_PARAMETER;
	/*
	fffff801`45476a16 488b05bb28daff  mov     rax,qword ptr [nt!CmpCallbackCookie (fffff801`452192d8)]
	fffff801`45476a1d 4c8d3d9c28daff  lea     r15,[nt!CallbackListHead (fffff801`452192c0)]
	fffff801`45476a24 48ffc0          inc     rax
	*/
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	UCHAR* Base = (PUCHAR)CmpInsertCallbackInListByAltitude_Addr;
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0x4c) && (*(UCHAR*)(Base + 1) == 0x8d) && (*(UCHAR*)(Base + 7) == 0x48))
		{
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			OffsetAddr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			OffsetAddr |= OffsetAddrTemp;
			*Address = OffsetAddr;
			break;
		}
		Base++;
	}
	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetCmpCallBackCount(DWORD64 CmpInsertCallbackInListByAltitude_Addr, DWORD64 * RoutineCount)
{
	if (!CmpInsertCallbackInListByAltitude_Addr)
		return STATUS_INVALID_PARAMETER;
	/*
	nt!CmpInsertCallbackInListByAltitude+0x7b:
	fffff800`e89eba53 48890f          mov     qword ptr [rdi],rcx
	fffff800`e89eba56 48894708        mov     qword ptr [rdi+8],rax
	fffff800`e89eba5a 48897908        mov     qword ptr [rcx+8],rdi
	fffff800`e89eba5e 488938          mov     qword ptr [rax],rdi
	fffff800`e89eba61 f0ff058853daff  lock inc dword ptr [nt!CmpCallBackCount (fffff800`e8790df0)]
	*/
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	UCHAR* Base = (PUCHAR)CmpInsertCallbackInListByAltitude_Addr;
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0xf0) && (*(UCHAR*)(Base + 1) == 0xff))
		{
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			OffsetAddr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			OffsetAddr |= OffsetAddrTemp;
			break;
		}
		Base++;
	}

	if (OffsetAddr && MmIsAddressValid((PVOID)OffsetAddr))
	{
		*RoutineCount = *(DWORD*)OffsetAddr;
	}

	if (*RoutineCount)
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetKeBugCheckCallbackAddress(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 KeBugCheckCallbackListHead_Address = 0;
	status = GetKeBugCheckCallbackListHead(&KeBugCheckCallbackListHead_Address);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get KeBugCheckCallbackListHead Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	KSPIN_LOCK spin_lock; // 自旋锁
	KIRQL  irql;    // 中断级别
	KeInitializeSpinLock(&spin_lock);
	irql = KeGetCurrentIrql();
	KeAcquireSpinLock(&spin_lock, &irql);
	PLIST_ENTRY BeginPtr;
	BeginPtr = (PLIST_ENTRY)KeBugCheckCallbackListHead_Address;
	PLIST_ENTRY temp = BeginPtr;
	size_t KeBugCheck_Count = 0;
	do
	{
		PKBUGCHECK_CALLBACK_RECORD bugCheckCallback = (PKBUGCHECK_CALLBACK_RECORD)(PVOID64)temp;
		if(bugCheckCallback->State)
			KeBugCheck_Count++;
		temp = temp->Flink;
	} while (temp != BeginPtr);

	size_t OutputBufferlength = 0;
	PBUG_STRUCT buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, KeBugCheck_Count * sizeof(BUG_STRUCT), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*(PDWORD64)buffer = KeBugCheck_Count * sizeof(BUG_STRUCT);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		KeReleaseSpinLock(&spin_lock, irql);
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;

	do
	{
		PKBUGCHECK_CALLBACK_RECORD bugCheckCallback = (PKBUGCHECK_CALLBACK_RECORD)(PVOID64)temp;
		if (bugCheckCallback->State)
		{
			KdPrint(("CallbackRoutine Entry: 0x%llX\n", (DWORD64)bugCheckCallback->CallbackRoutine));
			buffer->BUG_Address = (DWORD64)bugCheckCallback->CallbackRoutine;
			if(bugCheckCallback->Component)
			{
				KdPrint(("Component: %s\n", bugCheckCallback->Component));
				RtlCopyMemory(buffer->DriverName, bugCheckCallback->Component, strlen(bugCheckCallback->Component));
			}
			buffer++;
		}
		temp = temp->Flink;
	}while (temp != BeginPtr);

	KeReleaseSpinLock(&spin_lock, irql);
	return STATUS_SUCCESS;
}

NTSTATUS GetKeBugCheckCallbackListHead(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"KeRegisterBugCheckCallback");
	for (int i = 0; i<0xff; i++)
	{
		/*
		nt!KeRegisterBugCheckCallback+0x46:
		fffff801`44fd9706 4c8d05439f2200  lea     r8,[nt!KeBugCheckCallbackListHead (fffff801`45203650)]
		fffff801`44fd970d 488bd3          mov     rdx,rbx
		*/
		if ((*(UCHAR*)Base == 0x4c) && (*(UCHAR*)(Base + 1) == 0x8d) && (*(UCHAR*)(Base + 7) == 0x48))
		{
			DWORD64 KeBugCheckCallbackListHead_Address = 0;
			DWORD OffsetAddrTemp;
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			KeBugCheckCallbackListHead_Address = ((DWORD64)Base & 0xFFFFFFFF00000000);
			KeBugCheckCallbackListHead_Address |= OffsetAddrTemp;
			KdPrint(("Get KeBugCheckCallbackListHead: 0x%llX\n", KeBugCheckCallbackListHead_Address));
			*Address = KeBugCheckCallbackListHead_Address;
			break;
		}
		Base++;
	}

	if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetKeRegisterBugCheckReasonCallbackAddress(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 KeBugCheckReasonCallbackListHead_Address = 0;
	status = GetKeRegisterBugCheckReasonCallbackListHead(&KeBugCheckReasonCallbackListHead_Address);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get KeBugCheckReasonCallbackListHead Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	KSPIN_LOCK spin_lock; // 自旋锁
	KIRQL  irql;    // 中断级别
	KeInitializeSpinLock(&spin_lock);
	irql = KeGetCurrentIrql();
	KeAcquireSpinLock(&spin_lock, &irql);
	PLIST_ENTRY BeginPtr;
	BeginPtr = (PLIST_ENTRY)KeBugCheckReasonCallbackListHead_Address;
	PLIST_ENTRY temp = BeginPtr;
	size_t KeBugCheck_Count = 0;
	do
	{
		PKBUGCHECK_REASON_CALLBACK_RECORD bugReasonCallback = (PKBUGCHECK_REASON_CALLBACK_RECORD)(PVOID64)temp;
		if(bugReasonCallback->State)
			KeBugCheck_Count++;
		temp = temp->Flink;
	} while (temp != BeginPtr);

	size_t OutputBufferlength = 0;
	PBUG_STRUCT buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, KeBugCheck_Count * sizeof(BUG_STRUCT), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*(PDWORD64)buffer = KeBugCheck_Count * sizeof(BUG_STRUCT);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		KeReleaseSpinLock(&spin_lock, irql);
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;

	do
	{
		PKBUGCHECK_REASON_CALLBACK_RECORD bugReasonCallback = (PKBUGCHECK_REASON_CALLBACK_RECORD)(PVOID64)temp;
		if (bugReasonCallback->State)
		{
			KdPrint(("CallbackRoutine Entry: 0x%llX\n", (DWORD64)bugReasonCallback->CallbackRoutine));
			buffer->BUG_Address = (DWORD64)bugReasonCallback->CallbackRoutine;
			if (bugReasonCallback->Component)
			{
				KdPrint(("Component: %s\n", bugReasonCallback->Component));
				RtlCopyMemory(buffer->DriverName, bugReasonCallback->Component, strlen(bugReasonCallback->Component));
			}
			buffer++;
		}
		temp = temp->Flink;
	} while (temp != BeginPtr);

	KeReleaseSpinLock(&spin_lock, irql);
	return STATUS_SUCCESS;
}

NTSTATUS GetKeRegisterBugCheckReasonCallbackListHead(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"KeRegisterBugCheckReasonCallback");
	for (int i = 0; i<0xff; i++)
	{
		/*
		nt!KeRegisterBugCheckReasonCallback+0x4f:
		fffff801`44fd97ef 488d0d5a992200  lea     rcx,[nt!KeBugCheckReasonCallbackListHead (fffff801`45203150)]
		fffff801`44fd97f6 83fe06          cmp     esi,6
		*/
		if ((*(UCHAR*)Base == 0x48) && (*(UCHAR*)(Base + 1) == 0x8d) && (*(UCHAR*)(Base + 7) == 0x83))
		{
			DWORD64 KeBugCheckReasonCallbackListHead_Address = 0;
			DWORD OffsetAddrTemp;
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			KeBugCheckReasonCallbackListHead_Address = ((DWORD64)Base & 0xFFFFFFFF00000000);
			KeBugCheckReasonCallbackListHead_Address |= OffsetAddrTemp;
			KdPrint(("Get KeBugCheckReasonCallbackListHead: 0x%llX\n", KeBugCheckReasonCallbackListHead_Address));
			*Address = KeBugCheckReasonCallbackListHead_Address;
			break;
		}
		Base++;
	}

	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetIoRegisterShutdownNotificationAddress(
	WDFREQUEST Request, 
	size_t InputBufferLength, 
	size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 IopNotifyShutdownQueueHead_Address = 0;
	status = GetIopNotifyShutdownQueueHead(&IopNotifyShutdownQueueHead_Address);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get IopNotifyShutdownQueueHead Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	PSHUTDOWN_ENTRY Head = (PSHUTDOWN_ENTRY)IopNotifyShutdownQueueHead_Address;
	PSHUTDOWN_ENTRY temp = (PSHUTDOWN_ENTRY)Head->ShutdownList.Flink;
	size_t KeBugCheck_Count = 0;
	do
	{
		if(temp->DeviceObject)
			KeBugCheck_Count++;
		temp = (PSHUTDOWN_ENTRY)temp->ShutdownList.Flink;
	} while (temp != Head);

	size_t OutputBufferlength = 0;
	PSHUTDOWN_STRUCT buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, KeBugCheck_Count * sizeof(SHUTDOWN_STRUCT), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*(PDWORD64)buffer = KeBugCheck_Count * sizeof(SHUTDOWN_STRUCT);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	  RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;

	temp = (PSHUTDOWN_ENTRY)Head->ShutdownList.Flink;
	do 
	{
		if(MmIsAddressValid(temp->DeviceObject))
		{
			PDRIVER_OBJECT Driver_Object = temp->DeviceObject->DriverObject;
			if (Driver_Object)
			{
				KdPrint(("Belong to : %wZ", Driver_Object->DriverName));
				KdPrint(("ShutDown Callback: 0x%llX", (DWORD64)Driver_Object->MajorFunction[IRP_MJ_SHUTDOWN]));
				KdPrint(("Device Object: 0x%llX", (DWORD64)temp->DeviceObject));
				buffer->SHUTDOWN = (DWORD64)Driver_Object->MajorFunction[IRP_MJ_SHUTDOWN];
				buffer->DEVICE_OBJECT = (DWORD64)temp->DeviceObject;
				buffer++;
			}
		}
		temp = (PSHUTDOWN_ENTRY)temp->ShutdownList.Flink;
	} while (temp != Head);

	return STATUS_SUCCESS;
}

NTSTATUS GetIopNotifyShutdownQueueHead(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"IoRegisterShutdownNotification");
	for (int i = 0; i<0xff; i++)
	{
		/*
		nt!IoRegisterShutdownNotification+0x2a:
		fffff803`44bc9dba 488bcb          mov     rcx,rbx
		fffff803`44bc9dbd 48895810        mov     qword ptr [rax+10h],rbx
		fffff803`44bc9dc1 e85af8a8ff      call    nt!ObfReferenceObject (fffff803`44659620)
		fffff803`44bc9dc6 488bd7          mov     rdx,rdi
		fffff803`44bc9dc9 488d0de05fdcff  lea     rcx,[nt!IopNotifyShutdownQueueHead (fffff803`4498fdb0)]
		fffff803`44bc9dd0 e81f3ab8ff      call    nt!IopInterlockedInsertHeadList (fffff803`4474d7f4)
		*/
		if ((*(UCHAR*)Base == 0x48) && (*(UCHAR*)(Base + 1) == 0x8d) && (*(UCHAR*)(Base + 7) == 0xe8))
		{
			DWORD64 IopNotifyShutdownQueueHead_Address = 0;
			DWORD OffsetAddrTemp;
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			IopNotifyShutdownQueueHead_Address = ((DWORD64)Base & 0xFFFFFFFF00000000);
			IopNotifyShutdownQueueHead_Address |= OffsetAddrTemp;
			KdPrint(("Get IopNotifyShutdownQueueHead: 0x%llX\n", IopNotifyShutdownQueueHead_Address));
			*Address = IopNotifyShutdownQueueHead_Address;
			break;
		}
		Base++;
	}

	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetIoRegisterLastChanceShutdownNotificationAddress(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 IopNotifyShutdownQueueHead_Address = 0;
	status = GetIopNotifyLastChanceShutdownQueueHead(&IopNotifyShutdownQueueHead_Address);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get IoRegisterLastChanceShutdownNotification Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	PSHUTDOWN_ENTRY Head = (PSHUTDOWN_ENTRY)IopNotifyShutdownQueueHead_Address;
	PSHUTDOWN_ENTRY temp = (PSHUTDOWN_ENTRY)Head->ShutdownList.Flink;
	size_t KeBugCheck_Count = 0;
	do
	{
		PDRIVER_OBJECT Driver_Object = temp->DeviceObject->DriverObject;
		if (Driver_Object)
		{
			KeBugCheck_Count++;
		}
		temp = (PSHUTDOWN_ENTRY)temp->ShutdownList.Flink;
	} while (temp != Head);

	size_t OutputBufferlength = 0;
	PSHUTDOWN_STRUCT buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, KeBugCheck_Count * sizeof(SHUTDOWN_STRUCT), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*(PDWORD64)buffer = KeBugCheck_Count * sizeof(SHUTDOWN_STRUCT);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;
	temp = (PSHUTDOWN_ENTRY)Head->ShutdownList.Flink;
	do
	{
		PDRIVER_OBJECT Driver_Object = temp->DeviceObject->DriverObject;
		if (Driver_Object)
		{
			KdPrint(("Belong to : %wZ", Driver_Object->DriverName));
			KdPrint(("LastChance ShutDown Callback: 0x%llX", (DWORD64)Driver_Object->MajorFunction[IRP_MJ_SHUTDOWN]));
			KdPrint(("Device Object: 0x%llX", (DWORD64)temp->DeviceObject));
			buffer->SHUTDOWN = (DWORD64)Driver_Object->MajorFunction[IRP_MJ_SHUTDOWN];
			buffer->DEVICE_OBJECT = (DWORD64)temp->DeviceObject;
			buffer++;
		}
		temp = (PSHUTDOWN_ENTRY)temp->ShutdownList.Flink;
	} while (temp != Head);

	return STATUS_SUCCESS;
}

NTSTATUS GetIopNotifyLastChanceShutdownQueueHead(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"IoRegisterLastChanceShutdownNotification");
	for (int i = 0; i<0xff; i++)
	{
		/*
		nt!IoRegisterLastChanceShutdownNotification + 0x2a:
		fffff803`44bc9d3a 488bcb          mov     rcx, rbx
		fffff803`44bc9d3d e8def8a8ff      call    nt!ObfReferenceObject(fffff803`44659620)
		fffff803`44bc9d42 488bd7          mov     rdx, rdi
		fffff803`44bc9d45 48895f10        mov     qword ptr[rdi + 10h], rbx
		fffff803`44bc9d49 488d0d7060dcff  lea     rcx, [nt!IopNotifyLastChanceShutdownQueueHead(fffff803`4498fdc0)]
		fffff803`44bc9d50 e89f3ab8ff      call    nt!IopInterlockedInsertHeadList(fffff803`4474d7f4)
		*/
		if ((*(UCHAR*)Base == 0x48) && (*(UCHAR*)(Base + 1) == 0x8d) && (*(UCHAR*)(Base + 7) == 0xe8))
		{
			DWORD64 IopNotifyLastChanceShutdownQueueHead_Address = 0;
			DWORD OffsetAddrTemp;
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			IopNotifyLastChanceShutdownQueueHead_Address = ((DWORD64)Base & 0xFFFFFFFF00000000);
			IopNotifyLastChanceShutdownQueueHead_Address |= OffsetAddrTemp;
			KdPrint(("Get IoRegisterLastChanceShutdownNotification: 0x%llX\n", IopNotifyLastChanceShutdownQueueHead_Address));
			*Address = IopNotifyLastChanceShutdownQueueHead_Address;
			break;
		}
		Base++;
	}

	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetIoRegisterFsRegistrationChangeAddress(
	WDFREQUEST Request, 
	size_t InputBufferLength, 
	size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 IoRegisterFsRegistrationChange_Address = 0;
	status = GetIoRegisterFsRegistrationChange(&IoRegisterFsRegistrationChange_Address);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get IoRegisterFsRegistrationChange Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}

	DWORD64 IopFileSystemQueueHead_Addr[4] = { 0 };
	status = GetIopFileSystemQueueHead(IoRegisterFsRegistrationChange_Address, IopFileSystemQueueHead_Addr);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get IopFileSystemQueueHead Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}
	size_t KeBugCheck_Count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (MmIsAddressValid(&IopFileSystemQueueHead_Addr[i]) && IopFileSystemQueueHead_Addr[i])
		{
			PFS_CHANGE_NOTIFY_ENTRY Begin = (PFS_CHANGE_NOTIFY_ENTRY)IopFileSystemQueueHead_Addr[i];
			PFS_CHANGE_NOTIFY_ENTRY temp = (PFS_CHANGE_NOTIFY_ENTRY)Begin->FsChangeNotifyList.Flink;
			do
			{
				if(MmIsAddressValid(temp->DriverObject) && temp->FSDNotificationProc)
					KeBugCheck_Count++;
				temp = (PFS_CHANGE_NOTIFY_ENTRY)temp->FsChangeNotifyList.Flink;
			} while (temp != Begin);

		}
	}

	size_t OutputBufferlength = 0;
	PPLUGPLAY_STRUCT buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, KeBugCheck_Count * sizeof(PLUGPLAY_STRUCT), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*(PDWORD64)buffer = KeBugCheck_Count * sizeof(PLUGPLAY_STRUCT);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;

	for (int i = 0; i < 4; i++)
	{
		if (MmIsAddressValid(&IopFileSystemQueueHead_Addr[i]) && IopFileSystemQueueHead_Addr[i])
		{
			PFS_CHANGE_NOTIFY_ENTRY Begin = (PFS_CHANGE_NOTIFY_ENTRY)IopFileSystemQueueHead_Addr[i];
			PFS_CHANGE_NOTIFY_ENTRY temp = (PFS_CHANGE_NOTIFY_ENTRY)Begin->FsChangeNotifyList.Flink;
			do 
			{
				if (temp->FSDNotificationProc)
				{
					KdPrint(("FileSystem Callback: 0xllX\n", (DWORD64)temp->FSDNotificationProc));
					buffer->PlugPlay_Address = (DWORD64)temp->FSDNotificationProc;
					if (MmIsAddressValid(temp->DriverObject) && temp->DriverObject->DriverName.Length)
					{
						KdPrint(("FileSystem Driver Object: %wZ\n", temp->DriverObject->DriverName));
						RtlCopyMemory(buffer->DriverName, temp->DriverObject->DriverName.Buffer, temp->DriverObject->DriverName.MaximumLength);
					}
					buffer++;
				}
				temp = (PFS_CHANGE_NOTIFY_ENTRY)temp->FsChangeNotifyList.Flink;
			} while (temp != Begin);
		}
	}
	return STATUS_SUCCESS;
}

NTSTATUS GetIoRegisterFsRegistrationChange(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"IoRegisterFsRegistrationChange");
	for (int i = 0; i<0xff; i++)
	{
		/*
		nt!IoRegisterFsRegistrationChange:
		fffff802`088c4090 4883ec28        sub     rsp,28h
		fffff802`088c4094 4533c0          xor     r8d,r8d
		fffff802`088c4097 e82492f0ff      call    nt!IoRegisterFsRegistrationChangeMountAware (fffff802`087cd2c0)
		fffff802`088c409c 4883c428        add     rsp,28h
		*/
		if ((*(UCHAR*)Base == 0xe8) && (*(UCHAR*)(Base + 5) == 0x48))
		{
			DWORD64 IoRegisterFsRegistrationChangeMountAware_Address = 0;
			DWORD OffsetAddrTemp;
			RtlCopyMemory(&OffsetAddrTemp, Base + 1, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 5);
			IoRegisterFsRegistrationChangeMountAware_Address = ((DWORD64)Base & 0xFFFFFFFF00000000);
			IoRegisterFsRegistrationChangeMountAware_Address |= OffsetAddrTemp;
			KdPrint(("Get IoRegisterFsRegistrationChangeMountAware: 0x%llX\n", IoRegisterFsRegistrationChangeMountAware_Address));
			*Address = IoRegisterFsRegistrationChangeMountAware_Address;
			break;
		}
		Base++;
	}

	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetIopFileSystemQueueHead(DWORD64 IoRegisterFsRegistrationChange_Addr,
	DWORD64 * IopFileSystemQueueHead_Addr)
{
	if (!IoRegisterFsRegistrationChange_Addr)
		return STATUS_INVALID_PARAMETER;
	/*
	nt!IoRegisterFsRegistrationChangeMountAware+0x134:
	fffff802`087cd3f4 4533c0          xor     r8d,r8d
	fffff802`087cd3f7 488d0d8259dcff  lea     rcx,[nt!IopNetworkFileSystemQueueHead (fffff802`08592d80)]
	fffff802`087cd3fe 488bd6          mov     rdx,rsi
	*/
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	UCHAR* Base = (PUCHAR)(IoRegisterFsRegistrationChange_Addr + 0x100);
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0x48) && (*(UCHAR*)(Base + 1) == 0x8d) && (*(UCHAR*)(Base + 7) == 0x48))
		{
			for (int i = 0; i < 4; i++)
			{
				UCHAR* BaseTemp = Base + (0x12 * i);
				RtlCopyMemory(&OffsetAddrTemp, BaseTemp + 3, sizeof(DWORD));
				OffsetAddrTemp += (DWORD)((DWORD64)BaseTemp + 7);
				OffsetAddr = ((DWORD64)BaseTemp & 0xFFFFFFFF00000000);
				OffsetAddr |= OffsetAddrTemp;
				*(IopFileSystemQueueHead_Addr + i) = OffsetAddr;
			}
			break;
		}
		Base++;
	}

	
	if (*IopFileSystemQueueHead_Addr && MmIsAddressValid((PVOID)*IopFileSystemQueueHead_Addr))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetIoRegisterPlugPlayNotificationAddress(
	WDFREQUEST Request, 
	size_t InputBufferLength, 
	size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 PnpDeviceClassNotifyList_Address = 0;
	status = GetIoRegisterPnpDeviceClassNotifyList(&PnpDeviceClassNotifyList_Address);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PnpDeviceClassNotifyList Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}
	size_t KeBugCheck_Count = 0;

	for (DWORD i = 0; i < PNP_MAX_DEVICECLASS_NOTIFY; i++)
	{
		PSETUP_NOTIFY_DATA DeviceHead = (PSETUP_NOTIFY_DATA)((PLIST_ENTRY)PnpDeviceClassNotifyList_Address + i);
		PSETUP_NOTIFY_DATA temp = (PSETUP_NOTIFY_DATA)DeviceHead->ListEntry.Flink;
		while (temp != DeviceHead)
		{
			if (MmIsAddressValid(temp->DriverObject) && temp->Callback)
				KeBugCheck_Count++;
			temp = (PSETUP_NOTIFY_DATA)temp->ListEntry.Flink;
		}
	}

	size_t OutputBufferlength = 0;
	PPLUGPLAY_STRUCT buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, KeBugCheck_Count * sizeof(PLUGPLAY_STRUCT), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*(PDWORD64)buffer = KeBugCheck_Count * sizeof(PLUGPLAY_STRUCT);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = KeBugCheck_Count * sizeof(PLUGPLAY_STRUCT);

	for (DWORD i = 0; i < PNP_MAX_DEVICECLASS_NOTIFY; i++)
	{
		PSETUP_NOTIFY_DATA DeviceHead = (PSETUP_NOTIFY_DATA)((PLIST_ENTRY)PnpDeviceClassNotifyList_Address + i);
		PSETUP_NOTIFY_DATA temp = (PSETUP_NOTIFY_DATA)DeviceHead->ListEntry.Flink;
		while (temp != DeviceHead)
		{
			if (MmIsAddressValid(temp->DriverObject) && temp->Callback)
			{
				KdPrint(("Device CallBack: 0x%llX\n", (DWORD64)temp->Callback));
				buffer->PlugPlay_Address = (DWORD64)temp->Callback;
				if (temp->DriverObject->DriverName.MaximumLength)
				{
					KdPrint(("Get Driver Name: %wZ\n", temp->DriverObject->DriverName));
					RtlCopyMemory(buffer->DriverName, temp->DriverObject->DriverName.Buffer, temp->DriverObject->DriverName.MaximumLength);
				}
				buffer++;
			}
			temp = (PSETUP_NOTIFY_DATA)temp->ListEntry.Flink;
		}
	}
	return STATUS_SUCCESS;
}


NTSTATUS GetIoRegisterPnpDeviceClassNotifyList(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"IoRegisterPlugPlayNotification");
	/*
	nt!IoRegisterPlugPlayNotification+0x1db:
	fffff801`f438c6db 488d0dde80dfff  lea     rcx,[nt!PnpDeviceClassNotifyLock (fffff801`f41847c0)]
	fffff801`f438c6e2 e81910acff      call    nt!ExAcquireFastMutex (fffff801`f3e4d700)
	fffff801`f438c6e7 8b4f5c          mov     ecx,dword ptr [rdi+5Ch]
	fffff801`f438c6ea b84fecc44e      mov     eax,4EC4EC4Fh
	fffff801`f438c6ef 034f58          add     ecx,dword ptr [rdi+58h]
	fffff801`f438c6f2 034f54          add     ecx,dword ptr [rdi+54h]
	fffff801`f438c6f5 41030e          add     ecx,dword ptr [r14]
	fffff801`f438c6f8 f7e1            mul     eax,ecx
	fffff801`f438c6fa c1ea02          shr     edx,2
	fffff801`f438c6fd 6bc20d          imul    eax,edx,0Dh
	fffff801`f438c700 2bc8            sub     ecx,eax
	fffff801`f438c702 8bc1            mov     eax,ecx
	fffff801`f438c704 488d0d85602600  lea     rcx,[nt!PnpDeviceClassNotifyList (fffff801`f45f2790)]
	fffff801`f438c70b 48c1e004        shl     rax,4
	*/
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	Base = (PUCHAR)(Base + 0x1db);
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0x48) && (*(UCHAR*)(Base + 1) == 0x8d) && (*(UCHAR*)(Base + 2) == 0x0d) && (*(UCHAR*)(Base - 2) == 0x8b))
		{
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			OffsetAddr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			OffsetAddr |= OffsetAddrTemp;
			*Address = OffsetAddr;
			break;
		}
		Base++;
	}

	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS GetPoRegisterPowerSettingCallbackAddress(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength,
	_Out_opt_ size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	DWORD64 PopRegisteredPowerSettingCallbacksList = 0;
	status = GetPopRegisteredPowerSettingCallbacksList(&PopRegisteredPowerSettingCallbacksList);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Get PopRegisteredPowerSettingCallbacksList Address Failed"));
		return STATUS_UNSUCCESSFUL;
	}
	KIRQL irql;
	KeRaiseIrql(APC_LEVEL, &irql);

	PLIST_ENTRY Head = (PLIST_ENTRY)PopRegisteredPowerSettingCallbacksList;
	PLIST_ENTRY temp = (PLIST_ENTRY)Head->Flink;
	size_t KeBugCheck_Count = 0;
	do
	{
		KeBugCheck_Count++;
		temp = temp->Flink;
	} while (temp != Head);

	size_t OutputBufferlength = 0;
	PPLUGPLAY_STRUCT buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, KeBugCheck_Count * sizeof(PLUGPLAY_STRUCT), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*(PDWORD64)buffer = KeBugCheck_Count * sizeof(PLUGPLAY_STRUCT);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		KeLowerIrql(irql);
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;

	temp = (PLIST_ENTRY)Head->Flink;
	do
	{
		KdPrint(("CallBack Address: 0x%llX", *(PDWORD64)((DWORD64)temp + 0x50)));
		buffer->PlugPlay_Address = *(PDWORD64)((DWORD64)temp + 0x50);
		if (MmIsAddressValid((PDWORD64)*(PDWORD64)((DWORD64)temp + 0x60)))
		{
			PDEVICE_OBJECT dev = (PVOID)*(PDWORD64)((DWORD64)temp + 0x60);
			if (MmIsAddressValid(dev->DriverObject) && dev->DriverObject->DriverName.Length)
				RtlCopyMemory(buffer->DriverName, dev->DriverObject->DriverName.Buffer, dev->DriverObject->DriverName.MaximumLength);
		}
		buffer++;
		temp = temp->Flink;
	} while (temp != Head);

	KeLowerIrql(irql);
	return STATUS_SUCCESS;
}

NTSTATUS GetPopRegisteredPowerSettingCallbacksList(DWORD64 * Address)
{
	UCHAR* Base = (UCHAR*)GetFunctionAddr(L"PoRegisterPowerSettingCallback");
	/*
	nt!PoRegisterPowerSettingCallback+0x1cf:
	fffff800`ce4f33ef 488b05d250f0ff  mov     rax,qword ptr [nt!PopRegisteredPowerSettingCallbacks+0x8 (fffff800`ce3f84c8)]
	fffff800`ce4f33f6 488d0dc350f0ff  lea     rcx,[nt!PopRegisteredPowerSettingCallbacks (fffff800`ce3f84c0)]
	fffff800`ce4f33fd 483908          cmp     qword ptr [rax],rcx Branch
	fffff800`ce4f3400 0f8584a01a00    jne     nt!PoRegisterPowerSettingCallback+0x1aa26a (fffff800`ce69d48a)  Branch
	*/
	DWORD OffsetAddrTemp;
	DWORD64 OffsetAddr = 0;
	Base = (PUCHAR)(Base + 0x100);
	for (WORD i = 0; i < 0xff; i++)
	{
		if ((*(UCHAR*)Base == 0x48) && (*(UCHAR*)(Base + 1) == 0x8d) && (*(UCHAR*)(Base + 7) == 0x48) && (*(UCHAR*)(Base + 10) == 0x0f))
		{
			RtlCopyMemory(&OffsetAddrTemp, Base + 3, sizeof(DWORD));
			OffsetAddrTemp += (DWORD)((DWORD64)Base + 7);
			OffsetAddr = ((DWORD64)Base & 0xFFFFFFFF00000000);
			OffsetAddr |= OffsetAddrTemp;
			*Address = OffsetAddr;
			break;
		}
		Base++;
	}

	 if (*Address && MmIsAddressValid((PVOID)*Address))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}
}
