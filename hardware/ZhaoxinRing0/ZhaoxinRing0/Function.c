#include "Driver.h"
#include <Ndis.h>

// #ifdef ALLOC_PRAGMA
// #pragma alloc_text (PAGE, ReadIoPort)
// #pragma alloc_text (PAGE, WriteIoPort)
// #pragma alloc_text (PAGE, ReadMsr)
// #pragma alloc_text (PAGE, WriteMsr)
// #pragma alloc_text (PAGE, ReadPmc)
// #pragma alloc_text (PAGE, ReadMemory)
// #pragma alloc_text (PAGE, WriteMemory)
// #pragma alloc_text (PAGE, ReadPciConfig)
// #pragma alloc_text (PAGE, WritePciConfig)
// #pragma alloc_text (PAGE, ReadSPDBYTE)
// #pragma alloc_text (PAGE, SMBUS_Read_Byte)
// #pragma alloc_text (PAGE, ReadITEDisk)
// /*#pragma alloc_text (PAGE, WritePciConfig)*/
// #endif

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
	KdPrint(("Entry IOCTl_READ_PORT\n"));
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	TargetPortData databuf;//读写IO口的数据buffer
	 
	//DbgBreakPoint();
	if ((!InputBufferLength))
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_READ_PORT\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_PORT\n"));
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
			KdPrint(("Leave IOCTl_READ_PORT\n"));
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, &databuf.Port_Data, sizeof(databuf.Port_Data));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, databuf.Data_size);
		KdPrint(("Leave IOCTl_READ_PORT\n"));
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
	KdPrint(("Entry IOCTl_WRITE_PORT\n"));
	//DbgBreakPoint();
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	TargetPortData databuf;//读写IO口的数据buffer
	 
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_WRITE_PORT\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_WRITE_PORT\n"));
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
		KdPrint(("Leave IOCTl_WRITE_PORT\n"));
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
	KdPrint(("Entry IOCTl_READ_MSR\n"));
	//DbgBreakPoint();
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	MSR_Request MSR_Data;//MSR buffer
	 
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_READ_MSR\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_MSR\n"));
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&MSR_Data, buffer, InputBufferLength);

		__try
		{
			MSR_Data.Value = __readmsr(MSR_Data.msr_address);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_MSR\n"));
			return STATUS_UNSUCCESSFUL;
		}

		status = WdfRequestRetrieveOutputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_MSR\n"));
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, &MSR_Data.Value, sizeof(MSR_Data.Value));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(MSR_Data.Value));
		KdPrint(("Leave IOCTl_READ_MSR\n"));
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
	KdPrint(("Entry IOCTl_WRITE_MSR\n"));
	//DbgBreakPoint();
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	MSR_Request MSR_Data;//MSR buffer
	 
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_WRITE_MSR\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_WRITE_MSR\n"));
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
			KdPrint(("Leave IOCTl_WRITE_MSR\n"));
			return STATUS_UNSUCCESSFUL;
		}
		WdfRequestComplete(Request, STATUS_SUCCESS);
		KdPrint(("Leave IOCTl_WRITE_MSR\n"));
		return STATUS_SUCCESS;
	}
}

NTSTATUS ReadMsrThread(WDFREQUEST Request, size_t OutputBufferLength, size_t InputBufferLength)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	KdPrint(("Entry IOCTl_READ_MSR\n"));
	//DbgBreakPoint();
	NTSTATUS status;
	PMSR_Request buffer = NULL;//DeviceIoControl的输入输出buffer
	unsigned __int64 output = 0;
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_READ_MSR\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{

		status = WdfRequestRetrieveInputBuffer(Request, sizeof(MSR_Request), &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_MSR\n"));
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
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_MSR\n"));
			return STATUS_UNSUCCESSFUL;
		}

		status = WdfRequestRetrieveOutputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_MSR\n"));
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, &output, sizeof(output));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(output));
		KdPrint(("Leave IOCTl_READ_MSR\n"));
		return STATUS_SUCCESS;
	}
}

NTSTATUS WriteMsrThread(WDFREQUEST Request, size_t OutputBufferLength, size_t InputBufferLength)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);
	KdPrint(("Entry IOCTl_WRITE_MSR\n"));
	NTSTATUS status;
	PMSR_Request buffer = NULL;//DeviceIoControl的输入输出buffer

	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_WRITE_MSR\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, sizeof(MSR_Request), &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_WRITE_MSR\n"));
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
			KdPrint(("Leave IOCTl_WRITE_MSR\n"));
			return STATUS_UNSUCCESSFUL;
		}
		WdfRequestComplete(Request, STATUS_SUCCESS);
		KdPrint(("Leave IOCTl_WRITE_MSR\n"));
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
	KdPrint(("Entry IOCTl_READ_PMC"));
	NTSTATUS status;
	PMSR_Request buffer = NULL;//DeviceIoControl的输入输出buffer
	MSR_Request MSR_Data;//MSR buffer
	 
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_READ_PMC\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_PMC\n"));
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
			KdPrint(("Leave IOCTl_READ_PMC\n"));
			return STATUS_UNSUCCESSFUL;
		}

		status = WdfRequestRetrieveOutputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_PMC\n"));
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, &MSR_Data.Value, sizeof(MSR_Data.Value));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(MSR_Data.Value));
		KdPrint(("Leave IOCTl_READ_PMC\n"));
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
	KdPrint(("Entry IOCTL_READ_MEMORY"));
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
		KdPrint(("Leave IOCTL_READ_MEMORY\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTL_READ_MEMORY\n"));
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&Read_mem, buffer, InputBufferLength);
		memsize = Read_mem.UnitSize * Read_mem.Count;

		status = WdfRequestRetrieveOutputBuffer(Request, 1, &outmembuf, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTL_READ_MEMORY\n"));
			return STATUS_UNSUCCESSFUL;
		}

		__try
		{
			mapped = MmMapIoSpace(Read_mem.Address, memsize, FALSE);
			if (mapped == NULL)
			{
				WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
				KdPrint(("Leave IOCTL_READ_MEMORY\n"));
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
			default:
				break;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTL_READ_MEMORY\n"));
			return STATUS_UNSUCCESSFUL;
		}
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, Read_mem.UnitSize);
		if (mapped != NULL)
		{
			MmUnmapIoSpace(mapped, memsize);
		}
		KdPrint(("Leave IOCTl_READ_PMC\n"));
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
	KdPrint(("Entry IOCTL_WRITE_MEMORY\n"));
	WRITE_MEMORY_INPUT Write_mem;
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	size_t memsize;
	PVOID mapped;
	 
	//DbgBreakPoint();
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTL_WRITE_MEMORY\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTL_WRITE_MEMORY\n"));
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
				KdPrint(("Leave IOCTL_WRITE_MEMORY\n"));
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
			default:
				break;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTL_WRITE_MEMORY\n"));
			return STATUS_UNSUCCESSFUL;
		}
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 0);
		if (mapped != NULL)
		{
			MmUnmapIoSpace(mapped, memsize);
		}
		KdPrint(("Leave IOCTL_WRITE_MEMORY\n"));
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
	KdPrint(("Entry IOCTl_READ_PCI_COnfig\n"));
	//DbgBreakPoint();
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	PCI_ADDRESS addr;
	ULONG	dwAddr;
	ULONG	dwData;
	PPCI_COMMON_CONFIG pci_config = ExAllocatePoolWithTag(
		PagedPool,
		256,
		111
	);
	RtlFillMemory(pci_config, sizeof(*pci_config), 0xFF);
	if (!InputBufferLength)
	{
		ExFreePoolWithTag(pci_config, 111);
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_READ_PCI_COnfig\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 4, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			ExFreePoolWithTag(pci_config, 111);
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_PCI_COnfig\n"));
			return STATUS_UNSUCCESSFUL;
		}

		memcpy(&addr, buffer, InputBufferLength);
		for (int i = 0; i < 0x100; i+=4)
		{
			dwAddr = MK_PCIADDR(addr.bus, addr.dev, addr.func, i);
			WRITE_PORT_ULONG((PULONG)(ULONG)PCI_CONFIG_ADDRESS, dwAddr);
			dwData = READ_PORT_ULONG((PULONG)(ULONG)PCI_CONFIG_DATA);
			memcpy((PUCHAR)pci_config + i, &dwData, 4);
			if (pci_config->VendorID == 0xFFFF)
			{
				RtlFillMemory(pci_config, sizeof(*pci_config), 0xFF);
				break;
			}
		}
		/*dwAddr = 0x80000000 | (addr.bus << 16) | (SlotNumber.u.AsULONG << 8 | addr.Offset);
		WRITE_PORT_ULONG((PULONG)(ULONG)PCI_CONFIG_ADDRESS, dwAddr);
		dwData = READ_PORT_ULONG((PULONG)(ULONG)PCI_CONFIG_DATA);	*/	
		
		status = WdfRequestRetrieveOutputBuffer(Request, 1, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			ExFreePoolWithTag(pci_config, 111);
			KdPrint(("Leave IOCTl_READ_PCI_COnfig\n"));
			return STATUS_UNSUCCESSFUL;
		}
		RtlMoveMemory(buffer, pci_config, sizeof(*pci_config));
		//RtlMoveMemory(buffer, &dwData, sizeof(dwData));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(*pci_config));
		ExFreePoolWithTag(pci_config, 111);
		KdPrint(("Leave IOCTl_READ_PCI_COnfig\n"));
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
	KdPrint(("Entry IOCTl_WRITE_PCI_COnfig\n"));
	//DbgBreakPoint();
	NTSTATUS status;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	PCI_ADDRESS addr;
	PCI_SLOT_NUMBER SlotNumber;
	 
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_WRITE_PCI_COnfig\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 1, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_WRITE_PCI_COnfig\n"));
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
			KdPrint(("Leave IOCTl_WRITE_PCI_COnfig\n"));
			return status;
		}

// 		dwAddr = 0x80000000 | (addr.bus << 16) | (SlotNumber.u.AsULONG << 8);
// 
// 		WRITE_PORT_ULONG((PULONG)(ULONG)PCI_CONFIG_ADDRESS, dwAddr | addr.Offset);
// 
// 		__try {
// 			WRITE_PORT_ULONG((PULONG)(ULONG)PCI_CONFIG_DATA, addr.Data);
// 		}
// 		__except (EXCEPTION_EXECUTE_HANDLER)
// 		{
// 			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
// 			KdPrint(("Leave IOCTl_WRITE_PCI_COnfig\n"));
// 			return STATUS_UNSUCCESSFUL;
// 		}
		WdfRequestComplete(Request, STATUS_SUCCESS);
		KdPrint(("Leave IOCTl_WRITE_PCI_COnfig\n"));
		return STATUS_SUCCESS;
	}
}

/************************************************************************/
/*                         READ SPD Module                              */
/************************************************************************/


NTSTATUS ReadSPDBYTE(
	WDFREQUEST Request,
	size_t OutputBufferLength, 
	size_t InputBufferLength
)
{
	//SPD
	//UCHAR Data = 0;
	SPD_Head spd_head;
	NTSTATUS status;
	PVOID buffer = NULL;
	PPCI_COMMON_CONFIG Data = NULL;
	UCHAR cData = 0;
	KdPrint(("Entry IOCTl_READ_SPD\n"));
	//DbgBreakPoint();
	if (!InputBufferLength)
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_READ_SPD\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_SPD\n"));
			return STATUS_UNSUCCESSFUL;
		}
		RtlZeroMemory(&spd_head, sizeof(spd_head));
		memcpy(&spd_head, buffer, InputBufferLength);

		status = WdfRequestRetrieveOutputBuffer(Request, 255, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_SPD\n"));
			return STATUS_UNSUCCESSFUL;
		}
		RtlZeroMemory(buffer, 255);
		Data = buffer;
		KdPrint(("Entry Read SPD Byte\n"));
		for (USHORT i = 0; i < 0x100; ++i)
		{
			KdPrint(("Reading SPD %d Byte\n", i));
			if (NT_SUCCESS(SMBUS_Read_Byte(spd_head.Base_Address, spd_head.Slave_Address, i, &cData)))
				memcpy((PUCHAR)Data + i, &cData, 1);
		}
		KdPrint(("Leave Read SPD Byte\n"));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(PCI_COMMON_CONFIG));
		KdPrint(("Leave IOCTl_READ_SPD\n"));
		return STATUS_SUCCESS;
	}
}

NTSTATUS __stdcall SMBUS_Read_Byte(size_t Base_Address, size_t Slave_Address, size_t Offset, OUT PUCHAR Data)
{
	NTSTATUS status = STATUS_SUCCESS;
	USHORT temp = 0;
	USHORT counter = 4;

	KdPrint(("Init SMBUS!\n"));
	do
	{
		WRITE_PORT_UCHAR((PUCHAR)(USHORT)Base_Address, 0x1E);
		temp = READ_PORT_UCHAR((PUCHAR)(USHORT)Base_Address);
		if (!(counter--))
		{
			temp = 0;
			status = STATUS_UNSUCCESSFUL;
			goto exit;
		}
	} while (temp & 0x01);

	WRITE_PORT_UCHAR((PUCHAR)(USHORT)(Base_Address + 4), Slave_Address + 1);
	WRITE_PORT_UCHAR((PUCHAR)(USHORT)(Base_Address + 3), Offset);
	WRITE_PORT_UCHAR((PUCHAR)(USHORT)(Base_Address + 2), 0x48);
	NdisMSleep(200);
	KdPrint(("Init SMBUS Over!\n"));
	counter = 4;
	while (counter--)
	{
		temp = READ_PORT_UCHAR((PUCHAR)(USHORT)Base_Address);
		if (temp & 0x1C)
		{
			temp = 0;
			status = STATUS_UNSUCCESSFUL;
			goto exit;
		}
		else
		{
			if ((temp & 0x03) == 0x02)
			{
				break;
			}
		}
	}
	if (counter)
		temp = READ_PORT_UCHAR((PUCHAR)(USHORT)(Base_Address + 5));
exit:
	RtlCopyMemory(Data, &temp, sizeof(temp));
	return status;
}



/************************************************************************/
/*						Disk Identify                                   */
/************************************************************************/
NTSTATUS ReadITEDisk(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
) 
{
	KdPrint(("Entry IOCTl_READ_Disk_IDE\n"));
	NTSTATUS status;
	ULONG temp = 0;
	PVOID buffer = NULL;//DeviceIoControl的输入输出buffer
	USHORT SATA_Port = 0;
	PVOID data = ExAllocatePoolWithTag(
		PagedPool,
		512,
		'Disk'
	);
	//DbgBreakPoint();
	if ((!OutputBufferLength))
	{
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		KdPrint(("Leave IOCTl_READ_Disk_IDE\n"));
		return STATUS_INVALID_PARAMETER;
	}
	else
	{
		status = WdfRequestRetrieveInputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_Disk_IDE\n"));
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(&SATA_Port, buffer, InputBufferLength);
		temp = READ_PORT_UCHAR((PUCHAR)(USHORT)0x3f6);
		while (temp != 0x50)
		{
			static USHORT count = 0;
			if (count++ == 0xFFFF) 
			{
				temp = 'OCQ';
				break;  
			}
		}//BUSY? 
		WRITE_PORT_UCHAR((PUCHAR)(USHORT)0x1f6, (SATA_Port%2) << 4);
		WRITE_PORT_UCHAR((PUCHAR)(USHORT)0x1f7, 0xec);
		while (READ_PORT_UCHAR((PUCHAR)(USHORT)0x3f6) != 0x58) 
		{
			static USHORT count = 0;
			if (count++ == 0xFFFF) 
			{
				temp = 'TCQ';
				break;
			}
		}//Data Ready? 
		for (int i = 0; i < 256; i++)
			*((PUSHORT)data + i) = READ_PORT_USHORT((PUSHORT)(USHORT)0x1f0);//Read the data 

		status = WdfRequestRetrieveOutputBuffer(Request, 2, &buffer, NULL);
		if (!NT_SUCCESS(status))
		{
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
			KdPrint(("Leave IOCTl_READ_Disk_IDE\n"));
			return STATUS_UNSUCCESSFUL;
		}
		memcpy(buffer, data, sizeof(data)/sizeof(USHORT));
		WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(data) / sizeof(USHORT));
		ExFreePoolWithTag(data,'Disk');
		KdPrint(("Leave IOCTl_READ_Disk_IDE\n"));
		return STATUS_SUCCESS;
	}
}
