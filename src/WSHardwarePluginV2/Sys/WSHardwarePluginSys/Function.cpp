#include "Driver.h"
#include <intrin.h>
#include <ndis.h>
#include "Function.tmh"

inline namespace
{
	NTSTATUS MapPhysicalMemoryToLinearSpace(PVOID pPhysAddress,
		SIZE_T PhysMemSizeInBytes,
		PVOID* ppPhysMemLin,
		HANDLE* pPhysicalMemoryHandle,
		PVOID* ppPhysSection)
	{
		UNICODE_STRING     PhysicalMemoryUnicodeString;
		OBJECT_ATTRIBUTES  ObjectAttributes;
		PHYSICAL_ADDRESS   ViewBase;
		NTSTATUS           ntStatus;
		PHYSICAL_ADDRESS   pStartPhysAddress;
		PHYSICAL_ADDRESS   pEndPhysAddress;
		unsigned char* pbPhysMemLin = NULL;

		TraceEvents(TRACE_LEVEL_INFORMATION, DBG_OPERATOR, ("Entering %!FUNC!"));

		RtlInitUnicodeString(&PhysicalMemoryUnicodeString, L"\\Device\\PhysicalMemory");

		InitializeObjectAttributes(&ObjectAttributes,
			&PhysicalMemoryUnicodeString,
			OBJ_CASE_INSENSITIVE,
			(HANDLE)NULL,
			(PSECURITY_DESCRIPTOR)NULL);

		*pPhysicalMemoryHandle = NULL;
		*ppPhysSection = NULL;

		ntStatus = ZwOpenSection(pPhysicalMemoryHandle, SECTION_ALL_ACCESS, &ObjectAttributes);

		if (NT_SUCCESS(ntStatus))
		{
			ntStatus = ObReferenceObjectByHandle(*pPhysicalMemoryHandle,
				SECTION_ALL_ACCESS,
				(POBJECT_TYPE)NULL,
				KernelMode,
				ppPhysSection,
				(POBJECT_HANDLE_INFORMATION)NULL);

			if (NT_SUCCESS(ntStatus))
			{
				pStartPhysAddress.QuadPart = (ULONGLONG)(ULONG_PTR)pPhysAddress;

				pEndPhysAddress.QuadPart = pStartPhysAddress.QuadPart + PhysMemSizeInBytes;

				// Let ZwMapViewOfSection pick a linear address

				PhysMemSizeInBytes = (SIZE_T)pEndPhysAddress.QuadPart - (SIZE_T)pStartPhysAddress.QuadPart;

				ViewBase = pStartPhysAddress;

				ntStatus = ZwMapViewOfSection(*pPhysicalMemoryHandle,
					(HANDLE)-1,
					reinterpret_cast<PVOID*>(&pbPhysMemLin),
					0L,
					PhysMemSizeInBytes,
					&ViewBase,
					&PhysMemSizeInBytes,
					ViewShare,
					0,
					PAGE_READWRITE | PAGE_NOCACHE);

				// If the physical memory is already mapped with a different caching attribute, try again
				if (ntStatus == STATUS_CONFLICTING_ADDRESSES)
				{
					ntStatus = ZwMapViewOfSection(*pPhysicalMemoryHandle,
						(HANDLE)-1,
						reinterpret_cast<PVOID*>(&pbPhysMemLin),
						0L,
						PhysMemSizeInBytes,
						&ViewBase,
						&PhysMemSizeInBytes,
						ViewShare,
						0,
						PAGE_READWRITE);
				}

				if (!NT_SUCCESS(ntStatus))
					TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, ("ZwMapViewOfSection failed %!STATUS!"), ntStatus);
				else
				{
					pbPhysMemLin += pStartPhysAddress.QuadPart - ViewBase.QuadPart;
					*ppPhysMemLin = pbPhysMemLin;
				}
			}
			else
				TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, ("ObReferenceObjectByHandle failed %!STATUS!"), ntStatus);
		}
		else
			TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, ("ZwOpenSection failed %!STATUS!"), ntStatus);

		if (!NT_SUCCESS(ntStatus))
			ZwClose(*pPhysicalMemoryHandle);

		TraceEvents(TRACE_LEVEL_INFORMATION, DBG_OPERATOR, ("Leaving %!FUNC!"));
		return ntStatus;
	}

	NTSTATUS UnmapPhysicalMemory(HANDLE PhysicalMemoryHandle, PVOID pPhysMemLin, PVOID pPhysSection)
	{
		NTSTATUS ntStatus;

		TraceEvents(TRACE_LEVEL_INFORMATION, DBG_OPERATOR, ("Entering %!FUNC!"));

		ntStatus = ZwUnmapViewOfSection((HANDLE)-1, pPhysMemLin);

		if (!NT_SUCCESS(ntStatus))
			TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, ("ZwUnmapViewOfSection failed %!STATUS!"), ntStatus);

		if (pPhysSection)
			ObDereferenceObject(pPhysSection);

		ZwClose(PhysicalMemoryHandle);

		TraceEvents(TRACE_LEVEL_INFORMATION, DBG_OPERATOR, ("Leaving %!FUNC!"));

		return ntStatus;
	}
}

NTSTATUS ExcuteOperator(_In_ WDFREQUEST Request, _In_ size_t InputBufferLength, _In_ size_t OutputBufferLength)
{
	NTSTATUS status = STATUS_INVALID_INFO_CLASS;

	if (!InputBufferLength)
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! Input Buffer Is Null");
		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		return STATUS_INVALID_PARAMETER;
	}

	Ring0Operator* buffer = nullptr;//DeviceIoControl的输入输出buffer
	status = WdfRequestRetrieveInputBuffer(Request, 2, reinterpret_cast<PVOID*>(&buffer), NULL);
	if (!NT_SUCCESS(status))
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! WdfDeviceInitAssignName failed %!STATUS!", status);
		WdfRequestComplete(Request, status);
		return status;
	}

	switch (buffer->OperatorType)
	{
	case Ring0OperatorType::RDIO:
		status = ReadIo(Request, OutputBufferLength, buffer);
		break;
	case Ring0OperatorType::WRIO:
		status = WriteIo(buffer);
		break;
	case Ring0OperatorType::RDMEMORY:
		status = ReadMemory(Request, OutputBufferLength, buffer);
		break;
	case Ring0OperatorType::WRMEMORY:
		status = WriteMemory(buffer);
		break;
	case Ring0OperatorType::RDMSR:
		status = ReadMsr(Request, OutputBufferLength, buffer);
		break;
	case Ring0OperatorType::WRMSR:
		status = WriteMsr(buffer);
		break;
	case Ring0OperatorType::RDPCI:
		status = ReadPCI(Request, OutputBufferLength, buffer);
		break;
	case Ring0OperatorType::WRPCI:
		status = WritePCI(buffer);
		break;
	case Ring0OperatorType::RDTSC:
		status = ReadTSC(Request, OutputBufferLength, buffer);
		break;
	default:
		status = STATUS_INVALID_INFO_CLASS;
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! OperatorType was Unknown, Type: %d", (ULONG)(buffer->OperatorType));
	}

	if (!NT_SUCCESS(status))
	{
		WdfRequestCompleteWithInformation(Request, status, 0);
	}
	else
	{
		WdfRequestCompleteWithInformation(Request, status, OutputBufferLength);
	}

	TraceEvents(TRACE_LEVEL_INFORMATION, DBG_OPERATOR, "%!FUNC! Operator Type(%d) Status %!STATUS!", (ULONG)(buffer->OperatorType), status);

	return status;
}

NTSTATUS ReadIo(_In_ WDFREQUEST Request, _In_ size_t OutputBufferLength, _In_ const Ring0Operator* Operator)
{
	if (!OutputBufferLength)
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! Output Buffer Is Null");
		return STATUS_BUFFER_TOO_SMALL;
	}
	Ring0Output* buffer = nullptr;
	auto status = WdfRequestRetrieveOutputBuffer(Request, sizeof(Ring0Output), reinterpret_cast<PVOID*> (&buffer), NULL);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	switch (Operator->io.UnitSize)
	{
	case 1:
		buffer->Value = (ULONG)READ_PORT_UCHAR((PUCHAR)(USHORT)Operator->io.Address);
		break;
	case 2:
		buffer->Value = (ULONG)READ_PORT_USHORT((PUSHORT)(USHORT)Operator->io.Address);
		break;
	case 4:
		buffer->Value = READ_PORT_ULONG((PULONG)(USHORT)Operator->io.Address);
		break;
	default:
		status = STATUS_INVALID_PARAMETER;
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! Io Unit Size is illegal, UnitSize: %d", Operator->io.UnitSize);
		break;
	}

	if (!NT_SUCCESS(status))
	{
		return status;
	}

	return STATUS_SUCCESS;
}

NTSTATUS WriteIo(_In_ const Ring0Operator* Operator)
{
	NTSTATUS status = STATUS_SUCCESS;
	switch (Operator->io.UnitSize)
	{
	case 1:
		WRITE_PORT_UCHAR((PUCHAR)(USHORT)Operator->io.Address, (UCHAR)Operator->io.Value);
		break;
	case 2:
		WRITE_PORT_USHORT((PUSHORT)(USHORT)Operator->io.Address, (USHORT)Operator->io.Value);
		break;
	case 4:
		WRITE_PORT_ULONG((PULONG)(USHORT)Operator->io.Address, (ULONG)Operator->io.Value);
		break;
	default:
		// TODO 不是识别的值是否返回错误，还是像现在这样
		break;
	}
	return status;
}

NTSTATUS ReadMemory(_In_ WDFREQUEST Request, _In_ size_t OutputBufferLength, _In_ const Ring0Operator* Operator)
{
	if (!OutputBufferLength)
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! Output Buffer Is Null");
		return STATUS_BUFFER_TOO_SMALL;
	}
	Ring0Output* buffer = nullptr;
	auto status = WdfRequestRetrieveOutputBuffer(Request, sizeof(Ring0Output), reinterpret_cast<PVOID*> (&buffer), NULL);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	//ULONG size = Operator->memory.UnitSize;

	// #ifdef _WIN64
	// 	if (0xFFFF080000000000ull < static_cast<ULONG64>(Operator->memory.Address.QuadPart)
	// 		|| (static_cast<ULONG64>(Operator->memory.Address.QuadPart) + size - 1) > 0x0000FFFFFFFFFFFFull)
	// 	{
	// 		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! Cant Read Kernel Space Memory, Base:%llX", Operator->memory.Address.QuadPart);
	// 		return STATUS_INVALID_PARAMETER;
	// 	}
	// #else
	// 	if (0x000C0000ul < static_cast<ULONG64>(Operator->memory.Address.QuadPart)
	// 		|| (static_cast<ULONG64>(Operator->memory.Address.QuadPart) + size - 1) > 0x000FFFFFul)
	// 	{
	// 		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC!  Cant Read Kernel Space Memory, Base:%llX", Operator->memory.Address.QuadPart);
	// 		return STATUS_INVALID_PARAMETER;
	// 	}
	// #endif
	WinIoPhysStruct WinIoMemory{};
	WinIoMemory.pvPhysAddress = Operator->memory.Address.QuadPart;
	WinIoMemory.dwPhysMemSizeInBytes = Operator->memory.UnitSize;

	status = MapPhysicalMemoryToLinearSpace(
		reinterpret_cast<PVOID>(WinIoMemory.pvPhysAddress),
		static_cast<SIZE_T>(WinIoMemory.dwPhysMemSizeInBytes),
		reinterpret_cast<PVOID*>(&WinIoMemory.pvPhysMemLin),
		reinterpret_cast<HANDLE*>(&WinIoMemory.PhysicalMemoryHandle),
		reinterpret_cast<PVOID*>(&WinIoMemory.pvPhysSection));
	if (!NT_SUCCESS(status))
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "MapPhysicalMemoryToLinearSpace failed %!STATUS!", status);
		return status;
	}
	__try
	{
		switch (WinIoMemory.dwPhysMemSizeInBytes)
		{
		case 1:
			//	ProbeForRead(reinterpret_cast<void*>(WinIoMemory.pvPhysMemLin), WinIoMemory.dwPhysMemSizeInBytes, __alignof(PUINT8));
			buffer->Value = *reinterpret_cast<PUINT8>(WinIoMemory.pvPhysMemLin);
			break;
		case 2:
			//	ProbeForRead(reinterpret_cast<void*>(WinIoMemory.pvPhysMemLin), WinIoMemory.dwPhysMemSizeInBytes, __alignof(PUINT16));
			buffer->Value = *reinterpret_cast<PUINT16>(WinIoMemory.pvPhysMemLin);
			break;
		case 4:
			//	ProbeForRead(reinterpret_cast<void*>(WinIoMemory.pvPhysMemLin), WinIoMemory.dwPhysMemSizeInBytes, __alignof(PUINT32));
			buffer->Value = *reinterpret_cast<PDWORD32>(WinIoMemory.pvPhysMemLin);
			break;
		case 8:
			//		ProbeForRead(reinterpret_cast<void*>(WinIoMemory.pvPhysMemLin), WinIoMemory.dwPhysMemSizeInBytes, __alignof(PUINT64));
			buffer->Value = *reinterpret_cast<PDWORD64>(WinIoMemory.pvPhysMemLin);
			break;
		default:
			break;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "Error %d, ProbeForRead failed Cant Read Memory %lld, Length %lld", GetExceptionCode(), WinIoMemory.pvPhysMemLin, WinIoMemory.dwPhysMemSizeInBytes);
	}

	status = UnmapPhysicalMemory((HANDLE)WinIoMemory.PhysicalMemoryHandle, (PVOID)WinIoMemory.pvPhysMemLin, (PVOID)WinIoMemory.pvPhysSection);
	if (!NT_SUCCESS(status))
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "UnmapPhysicalMemory failed %!STATUS!, But Operator Sucess", status);
	}

	return STATUS_SUCCESS;
}

NTSTATUS WriteMemory(_In_ const Ring0Operator* Operator)
{
	ULONG size = Operator->memory.UnitSize;

#ifdef _WIN64
	if (0xFFFF080000000000ull < static_cast<ULONG64>(Operator->memory.Address.QuadPart)
		|| (static_cast<ULONG64>(Operator->memory.Address.QuadPart) + size - 1) > 0x0000FFFFFFFFFFFFull)
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! Cant Write Kernel Space Memory");
		return STATUS_INVALID_PARAMETER;
	}
#else
	if (0x000C0000ull < static_cast<ULONG64>(Operator->memory.Address.QuadPart)
		|| (static_cast<ULONG64>(Operator->memory.Address.QuadPart) + size - 1) > 0x000FFFFFull)
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC!  Cant Write Kernel Space Memory");
		return STATUS_INVALID_PARAMETER;
	}
#endif
	WinIoPhysStruct WinIoMemory{};
	WinIoMemory.pvPhysAddress = Operator->memory.Address.QuadPart;
	WinIoMemory.dwPhysMemSizeInBytes = Operator->memory.UnitSize;

	auto status = MapPhysicalMemoryToLinearSpace(
		reinterpret_cast<PVOID>(WinIoMemory.pvPhysAddress),
		static_cast<SIZE_T>(WinIoMemory.dwPhysMemSizeInBytes),
		reinterpret_cast<PVOID*>(&WinIoMemory.pvPhysMemLin),
		reinterpret_cast<HANDLE*>(&WinIoMemory.PhysicalMemoryHandle),
		reinterpret_cast<PVOID*>(&WinIoMemory.pvPhysSection));
	if (!NT_SUCCESS(status))
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "MapPhysicalMemoryToLinearSpace failed %!STATUS!", status);
		return status;
	}
	__try
	{
		switch (WinIoMemory.dwPhysMemSizeInBytes)
		{
		case 1:
			//			ProbeForWrite(reinterpret_cast<void*>(WinIoMemory.pvPhysMemLin), WinIoMemory.dwPhysMemSizeInBytes, __alignof(PUINT16));
			*reinterpret_cast<PUINT8>(WinIoMemory.pvPhysMemLin) = static_cast<UINT8>(Operator->memory.Value);
			break;
		case 2:
			//			ProbeForWrite(reinterpret_cast<void*>(WinIoMemory.pvPhysMemLin), WinIoMemory.dwPhysMemSizeInBytes, __alignof(PUINT16));
			*reinterpret_cast<PUINT16>(WinIoMemory.pvPhysMemLin) = static_cast<UINT16>(Operator->memory.Value);
			break;
		case 4:
			//			ProbeForWrite(reinterpret_cast<void*>(WinIoMemory.pvPhysMemLin), WinIoMemory.dwPhysMemSizeInBytes, __alignof(PUINT16));
			*reinterpret_cast<PUINT32>(WinIoMemory.pvPhysMemLin) = static_cast<UINT32>(Operator->memory.Value);
			break;
		case 8:
			//			ProbeForWrite(reinterpret_cast<void*>(WinIoMemory.pvPhysMemLin), WinIoMemory.dwPhysMemSizeInBytes, __alignof(PUINT16));
			*reinterpret_cast<PUINT64>(WinIoMemory.pvPhysMemLin) = static_cast<UINT64>(Operator->memory.Value);
			break;
		default:
			break;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "Error %d, ProbeForWrite failed Cant Write Memory %lld, Length %lld", GetExceptionCode(), WinIoMemory.pvPhysMemLin, WinIoMemory.dwPhysMemSizeInBytes);
	}

	status = UnmapPhysicalMemory((HANDLE)WinIoMemory.PhysicalMemoryHandle, (PVOID)WinIoMemory.pvPhysMemLin, (PVOID)WinIoMemory.pvPhysSection);
	if (!NT_SUCCESS(status))
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "UnmapPhysicalMemory failed %!STATUS!, But Operator Sucess", status);
	}
	return STATUS_SUCCESS;
}

NTSTATUS ReadMsr(_In_ WDFREQUEST Request, _In_ size_t OutputBufferLength, _In_ const Ring0Operator* Operator)
{
	if (!OutputBufferLength)
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! Output Buffer Is Null");
		return STATUS_BUFFER_TOO_SMALL;
	}
	Ring0Output* buffer = nullptr;
	auto status = WdfRequestRetrieveOutputBuffer(Request, sizeof(Ring0Output), reinterpret_cast<PVOID*> (&buffer), NULL);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	__try
	{
		buffer->Value = __readmsr(Operator->msr.Address);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return STATUS_UNSUCCESSFUL;
	}

	return status;
}

NTSTATUS WriteMsr(_In_ const Ring0Operator* Operator)
{
	__try
	{
		__writemsr(Operator->msr.Address, Operator->msr.Value);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return STATUS_UNSUCCESSFUL;
	}
	return STATUS_SUCCESS;
}

NTSTATUS ReadPCI(_In_ WDFREQUEST Request, _In_ size_t OutputBufferLength, _In_ const Ring0Operator* Operator)
{
	if (!OutputBufferLength)
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! Output Buffer Is Null");
		return STATUS_BUFFER_TOO_SMALL;
	}
	Ring0Output* buffer = nullptr;
	auto status = WdfRequestRetrieveOutputBuffer(Request, sizeof(Ring0Output), reinterpret_cast<PVOID*> (&buffer), NULL);
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	PCI_SLOT_NUMBER slot;
	ULONG busNumber = PciGetBus(Operator->pci.PciAddress);

	slot.u.AsULONG = 0;
	slot.u.bits.DeviceNumber = PciGetDev(Operator->pci.PciAddress);
	slot.u.bits.FunctionNumber = PciGetFunc(Operator->pci.PciAddress);
	auto error = HalGetBusDataByOffset(PCIConfiguration, busNumber, slot.u.AsULONG,
		reinterpret_cast<PVOID>(&buffer->Value), Operator->pci.PciOffset, Operator->pci.UnitSize);

	if (error == 0)
	{
		TraceEvents(TRACE_LEVEL_WARNING, DBG_OPERATOR, "%!FUNC! Bus not Exist Bus: %x, dev: %x func: %x", busNumber, slot.u.bits.DeviceNumber, slot.u.bits.FunctionNumber);
		return STATUS_NO_SUCH_DEVICE;
	}
	else if (Operator->pci.UnitSize != 2 && error == 2)
	{
		TraceEvents(TRACE_LEVEL_WARNING, DBG_OPERATOR, "%!FUNC! No Device");
		return STATUS_NO_SUCH_DEVICE;
	}
	else if (Operator->pci.UnitSize != error)
	{
		TraceEvents(TRACE_LEVEL_WARNING, DBG_OPERATOR, "%!FUNC! Read Cfg Error");
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}

NTSTATUS WritePCI(_In_ const Ring0Operator* Operator)
{
	PCI_SLOT_NUMBER slot;

	ULONG busNumber = PciGetBus(Operator->pci.PciAddress);

	slot.u.AsULONG = 0;
	slot.u.bits.DeviceNumber = PciGetDev(Operator->pci.PciAddress);
	slot.u.bits.FunctionNumber = PciGetFunc(Operator->pci.PciAddress);
	auto error = HalSetBusDataByOffset(PCIConfiguration, busNumber, slot.u.AsULONG,
		reinterpret_cast<PVOID>(&const_cast<Ring0Operator*>(Operator)->pci.Value), Operator->pci.PciOffset, Operator->pci.UnitSize);

	if (error != Operator->pci.UnitSize)
	{
		TraceEvents(TRACE_LEVEL_WARNING, DBG_OPERATOR, "%!FUNC! Write Cfg Error, Write length %d != Request length %d", error, Operator->pci.UnitSize);
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}

NTSTATUS ReadTSC(_In_ WDFREQUEST Request, _In_ size_t OutputBufferLength, _In_ const Ring0Operator* Operator)
{
	UNREFERENCED_PARAMETER(Operator);
	if (!OutputBufferLength)
	{
		TraceEvents(TRACE_LEVEL_ERROR, DBG_OPERATOR, "%!FUNC! Output Buffer Is Null");
		return STATUS_BUFFER_TOO_SMALL;
	}
	Ring0Output* buffer = nullptr;
	auto status = WdfRequestRetrieveOutputBuffer(Request, sizeof(Ring0Output), reinterpret_cast<PVOID*> (&buffer), NULL);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	LARGE_INTEGER nBeginTime, nEndTime, Freq;

	unsigned int dummy = 0;
	auto tsc1 = __rdtscp(&dummy);
	nBeginTime = KeQueryPerformanceCounter(&Freq);
	NdisMSleep(1);
	auto tsc2 = __rdtscp(&dummy);
	nEndTime = KeQueryPerformanceCounter(nullptr);
	buffer->Value = (tsc2 - tsc1) / (nEndTime.QuadPart - nBeginTime.QuadPart) * Freq.QuadPart;
	return status;
}