#include "Driver.h"
#include <Ndis.h>
#include "Function.tmh"

/************************************************************************/
/*						R/W Io Port			                            */
/************************************************************************/
NTSTATUS
ReadIoPort(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	TargetPortData databuf;//读写IO口的数据buffer

	if ((!InputBufferLength))
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&databuf, buffer, InputBufferLength);

		switch (databuf.Data_size)
		{
		case 1:
			databuf.Port_Data = (ULONG)READ_PORT_UCHAR((PUCHAR)(USHORT)databuf.Port_Addr);
			break;
		case 2:
			databuf.Port_Data = (ULONG)READ_PORT_USHORT((PUSHORT)(USHORT)databuf.Port_Addr);
			break;
		case 4:
			databuf.Port_Data = READ_PORT_ULONG((PULONG)(USHORT)databuf.Port_Addr);
			break;
		default:
			break;
		}

		status = WdfRequestRetrieveOutputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, &databuf.Port_Data, sizeof(databuf.Port_Data));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, databuf.Data_size);
		return STATUS_SUCCESS;
	}
}

NTSTATUS WriteIoPort(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	//DbgBreakPoint();
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	TargetPortData databuf;//读写IO口的数据buffer

	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&databuf, buffer, InputBufferLength);
		switch (databuf.Data_size)
		{
		case 1:
			WRITE_PORT_UCHAR((PUCHAR)(USHORT)databuf.Port_Addr, (UCHAR)databuf.Port_Data);
			break;
		case 2:
			WRITE_PORT_USHORT((PUSHORT)(USHORT)databuf.Port_Addr, (USHORT)databuf.Port_Data);
			break;
		case 4:
			WRITE_PORT_ULONG((PULONG)(USHORT)databuf.Port_Addr, (ULONG)databuf.Port_Data);
			break;
		default:
			break;
		}
		WdfRequestComplete(Request, STATUS_SUCCESS);
		return STATUS_SUCCESS;
	}
}

/************************************************************************/
/*                          R/W MSR                                     */
/************************************************************************/

NTSTATUS ReadMsr(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	MSR_Request MSR_Data;//MSR buffer

	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&MSR_Data, buffer, InputBufferLength);

		__try
		{
			MSR_Data.Value = __readmsr(MSR_Data.msr_address);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			MSR_Data.Value = -1;
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		status = WdfRequestRetrieveOutputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, &MSR_Data.Value, sizeof(MSR_Data.Value));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(MSR_Data.Value));
		return STATUS_SUCCESS;
	}
}

NTSTATUS WriteMsr(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	MSR_Request MSR_Data;//MSR buffer

	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(&MSR_Data, buffer, InputBufferLength);
		__try
		{
			__writemsr(MSR_Data.msr_address, MSR_Data.Value);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		WdfRequestComplete(Request, STATUS_SUCCESS);
		return STATUS_SUCCESS;
	}
}

NTSTATUS ReadMsrThread(WDFREQUEST Request, size_t OutputBufferLength, size_t InputBufferLength)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	NTSTATUS status;
	PMSR_Request buffer = NULL;//DeviceIoControl的输入输出buffer
	unsigned __int64 output = 0;
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, sizeof(MSR_Request), &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		PROCESSOR_NUMBER ProcNumber;
		GROUP_AFFINITY old_affinity, new_affinity;
		RtlZeroMemory(&ProcNumber, sizeof(PROCESSOR_NUMBER));
		RtlZeroMemory(&new_affinity, sizeof(GROUP_AFFINITY));
		RtlZeroMemory(&old_affinity, sizeof(GROUP_AFFINITY));

		__try
		{
			memset(&new_affinity, 0, sizeof(GROUP_AFFINITY));
			memset(&old_affinity, 0, sizeof(GROUP_AFFINITY));
			KeGetProcessorNumberFromIndex(buffer->core_id, &ProcNumber);
			new_affinity.Group = ProcNumber.Group;
			new_affinity.Mask = 1ULL << (ProcNumber.Number);
			KeSetSystemGroupAffinityThread(&new_affinity, &old_affinity);
			output = __readmsr(buffer->msr_address);
			KeRevertToUserGroupAffinityThread(&old_affinity);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			output = -1;
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		status = WdfRequestRetrieveOutputBuffer(Request, 8, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, &output, sizeof(output));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(output));
		return STATUS_SUCCESS;
	}
}

NTSTATUS WriteMsrThread(WDFREQUEST Request, size_t OutputBufferLength, size_t InputBufferLength)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	NTSTATUS status;
	PMSR_Request buffer = NULL;//DeviceIoControl的输入输出buffer

	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, sizeof(MSR_Request), &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		PROCESSOR_NUMBER ProcNumber;
		GROUP_AFFINITY old_affinity, new_affinity;
		RtlZeroMemory(&ProcNumber, sizeof(PROCESSOR_NUMBER));
		RtlZeroMemory(&new_affinity, sizeof(GROUP_AFFINITY));
		RtlZeroMemory(&old_affinity, sizeof(GROUP_AFFINITY));

		__try
		{
			memset(&new_affinity, 0, sizeof(GROUP_AFFINITY));
			memset(&old_affinity, 0, sizeof(GROUP_AFFINITY));
			KeGetProcessorNumberFromIndex(buffer->core_id, &ProcNumber);
			new_affinity.Group = ProcNumber.Group;
			new_affinity.Mask = 1ULL << (ProcNumber.Number);
			KeSetSystemGroupAffinityThread(&new_affinity, &old_affinity);
			__writemsr(buffer->msr_address, buffer->Value);
			KeRevertToUserGroupAffinityThread(&old_affinity);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		WdfRequestComplete(Request, STATUS_SUCCESS);
		return STATUS_SUCCESS;
	}
}

/************************************************************************/
/*                            Read TSC                                  */
/************************************************************************/

NTSTATUS ReadTsc(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	MSR_Request MSR_Data;//MSR buffer

	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&MSR_Data, buffer, InputBufferLength);

		__try
		{
			MSR_Data.Value = __rdtsc();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			MSR_Data.Value = -1;
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		status = WdfRequestRetrieveOutputBuffer(Request, 8, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, &MSR_Data.Value, sizeof(MSR_Data.Value));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(MSR_Data.Value));
		return STATUS_SUCCESS;
	}
}

NTSTATUS ReadTscThread(WDFREQUEST Request, size_t OutputBufferLength, size_t InputBufferLength)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	MSR_Request MSR_Data;//MSR buffer

	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&MSR_Data, buffer, InputBufferLength);

		__try
		{
			MSR_Data.Value = __rdtsc();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			MSR_Data.Value = -1;
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		status = WdfRequestRetrieveOutputBuffer(Request, 8, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, &MSR_Data.Value, sizeof(MSR_Data.Value));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(MSR_Data.Value));
		return STATUS_SUCCESS;
	}
}

/************************************************************************/
/*                            Read PMC                                  */
/************************************************************************/

NTSTATUS ReadPmc(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	NTSTATUS status;
	PMSR_Request buffer = NULL;//DeviceIoControl的输入输出buffer
	MSR_Request MSR_Data;//MSR buffer

	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&MSR_Data, buffer, InputBufferLength);
		__try
		{
			MSR_Data.Value = __readpmc((ULONG)MSR_Data.msr_address);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		status = WdfRequestRetrieveOutputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, &MSR_Data.Value, sizeof(MSR_Data.Value));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(MSR_Data.Value));
		return STATUS_SUCCESS;
	}
}

/************************************************************************/
/*                           R/W Memory                                 */
/************************************************************************/

NTSTATUS ReadMemory(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	READ_MEMORY_INPUT Read_mem;//Memory缓冲区
	PVOID outmembuf;
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	size_t memsize;
	PVOID mapped;

	//DbgBreakPoint();
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&Read_mem, buffer, InputBufferLength);
		memsize = Read_mem.UnitSize * Read_mem.Count;

		status = WdfRequestRetrieveOutputBuffer(Request, 1, &outmembuf, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		__try
		{
			mapped = MmMapIoSpace(Read_mem.Address, memsize, FALSE);
			if (mapped == NULL)
			{
				WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
				return STATUS_UNSUCCESSFUL;
			}
			switch (Read_mem.UnitSize)
			{
			case 1:
				READ_REGISTER_BUFFER_UCHAR(mapped, (PUCHAR)outmembuf, Read_mem.Count);
				break;
			case 2:
				READ_REGISTER_BUFFER_USHORT(mapped, (PUSHORT)outmembuf, Read_mem.Count);
				break;
			case 4:
				READ_REGISTER_BUFFER_ULONG(mapped, (PULONG)outmembuf, Read_mem.Count);
				break;
#ifdef _WIN64
			case 8:
				READ_REGISTER_BUFFER_ULONG64(mapped, (PULONG64)outmembuf, Read_mem.Count);
				break;
#endif
			default:
				break;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, Read_mem.UnitSize);
		if (mapped != NULL)
		{
			MmUnmapIoSpace(mapped, memsize);
		}
		return STATUS_SUCCESS;
	}
}
NTSTATUS WriteMemory(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	WRITE_MEMORY_INPUT Write_mem;
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	size_t memsize;
	PVOID mapped;

	//DbgBreakPoint();
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&Write_mem, buffer, InputBufferLength);
		memsize = Write_mem.UnitSize * Write_mem.Count;

		__try
		{
			mapped = MmMapIoSpace(Write_mem.Address, memsize, FALSE);
			if (mapped == NULL)
			{
				WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
				return STATUS_UNSUCCESSFUL;
			}
			switch (Write_mem.UnitSize)
			{
			case 1:
				WRITE_REGISTER_BUFFER_UCHAR(mapped, (PUCHAR)&Write_mem.Data, Write_mem.Count);
				break;
			case 2:
				WRITE_REGISTER_BUFFER_USHORT(mapped, (PUSHORT)&Write_mem.Data, Write_mem.Count);
				break;
			case 4:
				WRITE_REGISTER_BUFFER_ULONG(mapped, (PULONG)&Write_mem.Data, Write_mem.Count);
				break;
#ifdef _WIN64
			case 8:
				WRITE_REGISTER_BUFFER_ULONG64(mapped, (PULONG64)&Write_mem.Data, Write_mem.Count);;
				break;
#endif
			default:
				break;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 0);
		if (mapped != NULL)
		{
			MmUnmapIoSpace(mapped, memsize);
		}
		return STATUS_SUCCESS;
	}
}

/************************************************************************/
/*                          R/W PCI CONFIGURE                           */
/************************************************************************/

NTSTATUS ReadPciConfig(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	PCI_ADDRESS addr;
	ULONG	dwAddr;
	ULONG	dwData;
	PPCI_COMMON_CONFIG pci_config = NULL;
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(&addr, buffer, InputBufferLength);

		status = WdfRequestRetrieveOutputBuffer(Request, 1, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		RtlZeroMemory(buffer, OutputBufferLength);
		pci_config = buffer;

		for (int i = 0; i < 0x100; i += 4)
		{
			dwAddr = MK_PCIADDR(addr.bus, addr.dev, addr.func, i);
			WRITE_PORT_ULONG((PULONG)(ULONG)PCI_CONFIG_ADDRESS, dwAddr);
			dwData = READ_PORT_ULONG((PULONG)(ULONG)PCI_CONFIG_DATA);
			RtlCopyMemory((PUCHAR)pci_config + i, &dwData, 4);
			if (pci_config->VendorID == 0xFFFF)
			{
				RtlFillMemory(pci_config, sizeof(PCI_COMMON_CONFIG), 0xFF);
				break;
			}
		}

		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(PCI_COMMON_CONFIG));
		return STATUS_SUCCESS;
	}
}

NTSTATUS WritePciConfig(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	PCI_ADDRESS addr;
	PCI_SLOT_NUMBER SlotNumber;

	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 1, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&addr, buffer, InputBufferLength);
		SlotNumber.u.AsULONG = 0;
		SlotNumber.u.bits.DeviceNumber = addr.dev;
		SlotNumber.u.bits.FunctionNumber = addr.func;

		unsigned size = HalSetBusDataByOffset(PCIConfiguration, addr.bus, SlotNumber.u.AsULONG,
			&(addr.Data), addr.Offset, 4);
		if (size != 4)
		{
			status = STATUS_INVALID_PARAMETER;
			WdfRequestComplete(Request, status);
			return status;
		}

		WdfRequestComplete(Request, STATUS_SUCCESS);
		return STATUS_SUCCESS;
	}
}