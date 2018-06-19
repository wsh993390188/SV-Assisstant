#include "../Driver/Driver.h"
#pragma intrinsic(__readmsr) 

DWORD64 MyGetKeServiceDescriptorTable64()
{
	PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
	PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
	PUCHAR i = NULL;
	UCHAR b1 = 0, b2 = 0, b3 = 0;
	DWORD templong = 0;
	DWORD64 addr = 0;
	for (i = StartSearchAddress; i < EndSearchAddress; i++)
	{
		if (MmIsAddressValid(i) && MmIsAddressValid(i + 1) && MmIsAddressValid(i + 2))
		{
			b1 = *i;
			b2 = *(i + 1);
			b3 = *(i + 2);
			if (b1 == 0x4c && b2 == 0x8d && b3 == 0x15) //4c8d15  
			{
				RtlCopyMemory(&templong, i + 3, sizeof(DWORD));
				templong += (DWORD)((DWORD64)i + 7);
				addr = ((DWORD64)i & 0xFFFFFFFF00000000);
				addr |= templong;
				break;
			}
		}
	}
	return addr;
}
DWORD64 MyGetKeShadowServiceDescriptorTable64()
{
	PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
	PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
	PUCHAR i = NULL;
	UCHAR b1 = 0, b2 = 0, b3 = 0;
	DWORD templong = 0;
	DWORD64 addr = 0;
	for (i = StartSearchAddress; i < EndSearchAddress; i++)
	{
		if (MmIsAddressValid(i) && MmIsAddressValid(i + 1) && MmIsAddressValid(i + 2))
		{
			b1 = *i;
			b2 = *(i + 1);
			b3 = *(i + 2);
			if (b1 == 0x4c && b2 == 0x8d && b3 == 0x1d) //4c8d1d  
			{
				RtlCopyMemory(&templong, i + 3, sizeof(DWORD));
				templong += (DWORD)((DWORD64)i + 7);
				addr = ((DWORD64)i & 0xFFFFFFFF00000000);
				addr |= templong;
				break;
			}
		}
	}
	return addr;
}

NTSTATUS EnumSSDTable(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength, size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return	STATUS_BUFFER_TOO_SMALL;
	LONG dwTemp = 0;
	DWORD64 qwTemp = 0;
	PSYSTEM_SERVICE_TABLE ssdt = (PSYSTEM_SERVICE_TABLE)MyGetKeServiceDescriptorTable64();

	NTSTATUS status = STATUS_SUCCESS;
	size_t OutputBufferlength = 0;
	PDWORD64 buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, ssdt->NumberOfServices * sizeof(DWORD64), &buffer, &OutputBufferlength);

	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*buffer = ssdt->NumberOfServices * sizeof(DWORD64);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;

	if (ssdt)
	{
		KdPrint(("SSDT Nums: %d",ssdt->NumberOfServices));
		for(int Index = 0; Index < ssdt->NumberOfServices; Index++)
		{
			DWORD* stb = (DWORD*)(ssdt->ServiceTableBase);
			dwTemp = stb[Index];
			dwTemp = (DWORD)(dwTemp >> 4);
			qwTemp = ((DWORD64)ssdt->ServiceTableBase + (DWORD64)dwTemp) & 0xffffffff0fffffff;
			qwTemp = qwTemp | ((DWORD64)ssdt->ServiceTableBase & 0x00000000f0000000);
			*buffer = qwTemp;
			KdPrint(("Offset Address: 0x%llX", qwTemp));
			buffer++;
		}
	}

	return STATUS_SUCCESS;
}

NTSTATUS EnumShadowSSDTable(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength, size_t* Resquestlength)
{
	if (!OutputBufferLength)
		return	STATUS_BUFFER_TOO_SMALL;

	LONG dwTemp = 0;
	DWORD64 qwTemp = 0;
	PSYSTEM_SERVICE_TABLE ssdt = (PSYSTEM_SERVICE_TABLE)MyGetKeShadowServiceDescriptorTable64();
	ssdt++;

	NTSTATUS status = STATUS_SUCCESS;
	size_t OutputBufferlength = 0;
	PDWORD64 buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, ssdt->NumberOfServices * sizeof(DWORD64), &buffer, &OutputBufferlength);

	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*buffer = ssdt->NumberOfServices * sizeof(DWORD64);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = OutputBufferlength;


	if (ssdt)
	{
		KdPrint(("SSDT Nums: %d",ssdt->NumberOfServices));
		for(int Index = 0; Index < ssdt->NumberOfServices; Index++)
		{
			DWORD* stb = (DWORD*)(ssdt->ServiceTableBase);
			dwTemp = stb[Index];
			dwTemp = (DWORD)(dwTemp >> 4);
			qwTemp = ((DWORD64)ssdt->ServiceTableBase + (DWORD64)dwTemp) & 0xffffffff0fffffff;
			qwTemp = qwTemp | ((DWORD64)ssdt->ServiceTableBase & 0x00000000f0000000);
			*buffer = qwTemp;
			buffer++;
			KdPrint(("Offset Address: 0x%llX", qwTemp));
		}
	}

	return STATUS_SUCCESS;
}
