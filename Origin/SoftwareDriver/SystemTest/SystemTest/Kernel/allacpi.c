#include "../Driver/driver.h"
#include <Aux_klib.h>

NTSTATUS EnumAllACPITable(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength)
{
	NTSTATUS status = AuxKlibInitialize();
	if (!NT_SUCCESS(status))
		return status;
	PVOID pBuffer = NULL;
	ULONG BUFSIZE = 0, dataSize = 0;
	status = AuxKlibEnumerateSystemFirmwareTables('ACPI', pBuffer, BUFSIZE, &dataSize);
	if (status == STATUS_BUFFER_TOO_SMALL)
	{
		pBuffer = ExAllocatePool(PagedPool, dataSize);
		BUFSIZE = dataSize;
	}
	if (!pBuffer)
		return STATUS_NO_MEMORY;
	status = AuxKlibEnumerateSystemFirmwareTables('ACPI', pBuffer, BUFSIZE, &dataSize);

	ExFreePool(pBuffer);
	pBuffer = NULL;
	return status;
}
