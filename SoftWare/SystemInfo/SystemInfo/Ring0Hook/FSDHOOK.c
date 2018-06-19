#include "../Driver/Driver.h"

const char* const IPR_FUNC_NAME[] =
{
	"IRP MJ CREATE",
	"IRP MJ CREATE NAMED PIPE",
	"IRP MJ CLOSE",
	"IRP MJ READ",
	"IRP MJ WRITE",
	"IRP MJ QUERY INFORMATION",
	"IRP MJ SET INFORMATION",
	"IRP MJ QUERY EA",
	"IRP MJ SET EA",
	"IRP MJ FLUSH BUFFERS",
	"IRP MJ QUERY VOLUME INFORMATION",
	"IRP MJ SET VOLUME INFORMATION",
	"IRP MJ DIRECTORY CONTROL",
	"IRP MJ FILE SYSTEM CONTROL",
	"IRP MJ DEVICE CONTROL",
	"IRP MJ INTERNAL DEVICE CONTROL",
	"IRP MJ SHUTDOWN",
	"IRP MJ LOCK CONTROL",
	"IRP MJ CLEANUP",
	"IRP MJ CREATE MAILSLOT",
	"IRP MJ QUERY SECURITY",
	"IRP MJ SET SECURITY",
	"IRP MJ POWER",
	"IRP MJ SYSTEM CONTROL",
	"IRP MJ DEVICE CHANGE",
	"IRP MJ QUERY QUOTA",
	"IRP MJ SET QUOTA",
	"IRP MJ PNP"
};

NTSTATUS GetCurrentFsdAddress(PWSTR DriverName, PDWORD64* buffer)
{
	UNICODE_STRING uniDriverName;
	PDRIVER_OBJECT DriverObject = NULL;
 	RtlInitUnicodeString(&uniDriverName, DriverName);

    NTSTATUS ntStatus = ObReferenceObjectByName(&uniDriverName,  
        OBJ_CASE_INSENSITIVE,  
        NULL,  
        0,  
        *IoDriverObjectType,  
        KernelMode,  
        NULL,  
        &DriverObject);
	if (!NT_SUCCESS(ntStatus))
		return ntStatus;
	KdPrint(("This is %wZ\n",uniDriverName));
	if (MmIsAddressValid(DriverObject))
	{
		if(DriverObject->MajorFunction)
			for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
			{
				KdPrint(("Func Name: %s, Address: 0x%llX\n",IPR_FUNC_NAME[i], (DWORD64)DriverObject->MajorFunction[i]));
				**buffer = (DWORD64)DriverObject->MajorFunction[i];
				(*buffer)++;
			}
	}
	ObDereferenceObject(DriverObject);
	return STATUS_SUCCESS;
}

NTSTATUS EnumFSDAddress(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength, size_t* Resquestlength)
{
	NTSTATUS status = STATUS_SUCCESS;
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;

	size_t OutputBufferlength = 0;
	PDWORD64 buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, IRP_MJ_MAXIMUM_FUNCTION * 2 * sizeof(DWORD64), &buffer, &OutputBufferlength);

	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*buffer = IRP_MJ_MAXIMUM_FUNCTION * 2 * sizeof(DWORD64);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = IRP_MJ_MAXIMUM_FUNCTION * 2 * sizeof(DWORD64);

	status = GetCurrentFsdAddress(L"\\FileSystem\\ntfs", &buffer);
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	status = GetCurrentFsdAddress(L"\\FileSystem\\fastfat", &buffer);
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	return STATUS_SUCCESS;
}
