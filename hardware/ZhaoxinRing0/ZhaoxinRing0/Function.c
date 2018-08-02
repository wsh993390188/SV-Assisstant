#include "Driver.h"
#include <Ndis.h>

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

		status = WdfRequestRetrieveOutputBuffer(Request, 4, &buffer, NULL);
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

		for (int i = 0; i < 0x100; i+=4)
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

/************************************************************************/
/*                         READ SPD Module                              */
/************************************************************************/


NTSTATUS ReadSPDBYTE(
	WDFREQUEST Request,
	size_t OutputBufferLength, 
	size_t InputBufferLength
)
{
	UNREFERENCED_PARAMETER(OutputBufferLength);

	if (Platform_id != INTEL_PLATFORM && Platform_id != AMD_PLATFORM && Platform_id != ZX_PLATFORM && Platform_id != ATI_PLATFORM && Platform_id != NVI_PLATFORM)
	{
		WdfRequestComplete(Request, STATUS_PLATFORM_MANIFEST_INVALID);
		return STATUS_PLATFORM_MANIFEST_INVALID;
	}
	//SPD
	//UCHAR Data = 0;
	SPD_Head spd_head;
	NTSTATUS status;
	PVOID buffer = NULL;
	PPCI_COMMON_CONFIG Data = NULL;
	UCHAR cData = 0;
	KdPrint(("Entry IOCTl_READ_SPD\n"));
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
		if (Platform_id == AMD_PLATFORM || Platform_id == ATI_PLATFORM)
		{
			setupFch(spd_head.Base_Address);
			setupFch(spd_head.Base_Address);
		}
		for (USHORT i = 0; i < 0x100; ++i)
		{
			KdPrint(("Reading %04X Platform SPD %d Byte\n", Platform_id, i));
			status = SMBUS_Read_Byte_Zhaoxin(spd_head.Base_Address, spd_head.Slave_Address, i, &cData);
			if (NT_SUCCESS(status))
				memcpy((PUCHAR)Data + i, &cData, 1);
			else
				break;
		}

		if (!NT_SUCCESS(status))
		{
			status = STATUS_UNSUCCESSFUL;
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
		}
		else
		{
			WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, sizeof(PCI_COMMON_CONFIG));
			status = STATUS_SUCCESS;
		}
		return status;
	}
}

NTSTATUS __stdcall SMBUS_Read_Byte_Zhaoxin(size_t Base_Address, size_t Slave_Address, size_t Offset, OUT PUCHAR Data)
{
	NTSTATUS status = STATUS_SUCCESS;
	USHORT temp = 0;
	USHORT counter = 4;

	do
	{
		WRITE_PORT_UCHAR((PUCHAR)(USHORT)Base_Address + SMBUS_STATUS_REG, 0x1E);
		temp = READ_PORT_UCHAR((PUCHAR)(USHORT)Base_Address + SMBUS_STATUS_REG);
		if (!(counter--))
		{
			temp = 0;
			status = STATUS_UNSUCCESSFUL;
			goto exit;
		}
	} while (temp & 0x01);

	WRITE_PORT_UCHAR((PUCHAR)(USHORT)(Base_Address + SMBUS_HOST_CMD_REG), Slave_Address | 1);
	WRITE_PORT_UCHAR((PUCHAR)(USHORT)(Base_Address + SMBUS_CONTROL_REG), Offset);
	WRITE_PORT_UCHAR((PUCHAR)(USHORT)Base_Address + SMBUS_STATUS_REG, 0xFE);
	WRITE_PORT_UCHAR((PUCHAR)(USHORT)(Base_Address + SMBUS_COMMAND_REG), SMBUS_READ_BYTE_COMMAND);
	NdisMSleep(200);
	counter = 4;
	while (counter--)
	{
		temp = READ_PORT_UCHAR((PUCHAR)(USHORT)Base_Address + SMBUS_STATUS_REG);
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
	{
		temp = READ_PORT_UCHAR((PUCHAR)(USHORT)(Base_Address + SMBUS_DATA0_REG));
		KdPrint(("Read Smbus Offset: %02X, Data: %02X", Offset, temp));
	}

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

void writePmReg(UINT8 reg, UINT8 data)
{
	__outbyte(PMIO_INDEX_REG, reg);
	__outbyte(PMIO_DATA_REG, data);
}

void setupFch(UINT16 SmbusBase)
{
	/* set up SMBUS - Set to SMBUS 0 & set base address */
	/* For SB800 & Hudson1 to SB900 & Hudson 2/3 */
	writePmReg(SMBUS_BAR_HIGH_BYTE, SmbusBase >> 8);
	writePmReg(SMBUS_BAR_LOW_BYTE, (SmbusBase & 0xe0) | 1);

	/* set SMBus clock to 400 KHz */
	__outbyte(SmbusBase + SMBUS_CLOCK_REG, SMBUS_FREQUENCY_CONST / 400000);
}

int do_smbus_read_byte(unsigned int smbus_base, UINT16 device,
	unsigned int address)
{
	unsigned char status;
	unsigned char byte;

	if (smbus_wait_until_ready(smbus_base) < 0)
		return SMBUS_WAIT_UNTIL_READY_TIMEOUT;
	/* Set up transaction */
	/* Disable interrupts */
	__outbyte(smbus_base + SMBUS_COMMAND_REG, __inbyte(smbus_base + SMBUS_COMMAND_REG) & ~SMBHSTCNT_INTREN);
	/* Set the device I'm talking to */
	__outbyte(smbus_base + SMBUS_HOST_CMD_REG, ((device & 0x7f) << 1) | 1);
	/* Set the command/address... */
	__outbyte(smbus_base + SMBUS_CONTROL_REG, address & 0xff);
	/* Set up for a byte data read */
	__outbyte((smbus_base + SMBUS_COMMAND_REG), (__inbyte(smbus_base + SMBUS_COMMAND_REG) & 0xe3) | I801_BYTE_DATA);
	/* Clear any lingering errors, so the transaction will run */
	__outbyte(smbus_base + SMBUS_STATUS_REG, __inbyte(smbus_base + SMBUS_STATUS_REG));

	/* Clear the data byte... */
	__outbyte(smbus_base + SMBUS_DATA0_REG, 0);

	/* Start the command */
	__outbyte(smbus_base + SMBUS_COMMAND_REG, (__inbyte(smbus_base + SMBUS_COMMAND_REG) | SMBHSTCNT_START));

	/* poll for it to start */
	if (smbus_wait_until_active(smbus_base) < 0)
		return SMBUS_WAIT_UNTIL_ACTIVE_TIMEOUT;

	/* Poll for transaction completion */
	if (smbus_wait_until_done(smbus_base) < 0)
		return SMBUS_WAIT_UNTIL_DONE_TIMEOUT;

	status = __inbyte(smbus_base + SMBUS_STATUS_REG);
	KdPrint(("Intel SPD Status:%02X", status));
	/* Ignore the "In Use" status... */
	status &= ~(SMBHSTSTS_SMBALERT_STS | SMBHSTSTS_INUSE_STS);

	/* Read results of transaction */
	byte = __inbyte(smbus_base + SMBUS_DATA0_REG);
	if (status != SMBHSTSTS_INTR)
		return SMBUS_ERROR;
	return byte;
}

void smbus_delay(void)
{
	__inbyte(0x80);
}

int smbus_wait_until_ready(UINT32 smbus_base)
{
	unsigned int loops = SMBUS_TIMEOUT;
	unsigned char byte;
	do {
		smbus_delay();
		if (--loops == 0)
			break;
		byte = __inbyte(smbus_base + SMBUS_STATUS_REG);
	} while (byte & SMBHSTSTS_HOST_BUSY);
	return loops ? 0 : -1;
}

int smbus_wait_until_done(UINT32 smbus_base)
{
	unsigned int loops = SMBUS_TIMEOUT;
	unsigned char byte;
	do {
		smbus_delay();
		if (--loops == 0)
			break;
		byte = __inbyte(smbus_base + SMBUS_STATUS_REG);
	} while ((byte & SMBHSTSTS_HOST_BUSY)
		|| (byte & ~(SMBHSTSTS_INUSE_STS | SMBHSTSTS_HOST_BUSY)) == 0);
	return loops ? 0 : -1;
}

int smbus_wait_until_active(UINT32 smbus_base)
{
	unsigned long loops;
	loops = SMBUS_TIMEOUT;
	do {
		unsigned char val;
		smbus_delay();
		val = __inbyte(smbus_base + SMBUS_STATUS_REG);
		if ((val & SMBHSTSTS_HOST_BUSY)) {
			break;
		}
	} while (--loops);
	return loops ? 0 : -1;
}

