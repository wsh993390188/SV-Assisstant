/*++

Module Name:

ioctl.h

Abstract:

This module defines some ioctl for applications.

Environment:

kernel

--*/

#pragma once

#ifndef IOCTL_H
#define IOCTL_H

#ifndef CTL_CODE
#pragma message("CTL_CODE undefined. Include winioctl.h or wdm.h")
#endif

#define DEVICE_NAME_STRING			L"\\Device\\ZhaoxinRing0" 
#define SYMBOLIC_NAME_STRING		L"\\DosDevices\\ZhaoxinRing0"

#define PCI_CONFIG_ADDRESS      0xCF8
#define PCI_CONFIG_DATA         0xCFC
#define PCI_TYPE0_ADDRESSES             6
#define PCI_TYPE1_ADDRESSES             2
#define PCI_TYPE2_ADDRESSES             5

#define READ_PORT \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define WRITE_PORT \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define READ_MSR \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define WRITE_MSR \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define READ_MSR_THREAD \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80B,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define WRITE_MSR_THREAD \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80C,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define READ_PMC \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define READ_MEMORY \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x806,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define WRITE_MEMORY \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x807,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define READ_SPD \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x808,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define READ_PCI_CONFIG \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x809,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define WRITE_PCI_CONFIG \
		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x80A,  METHOD_BUFFERED, FILE_ANY_ACCESS)
typedef struct _TargetPortData
{
	USHORT Port_Addr;
	ULONG Port_Data;
	UCHAR Data_size;
}TargetPortData;

typedef struct _READ_MEMORY_INPUT {
	LARGE_INTEGER Address;
	ULONG UnitSize;
	ULONG Count;
}READ_MEMORY_INPUT;

typedef struct  _WRITE_MEMORY_INPUT {
	LARGE_INTEGER Address;
	ULONG UnitSize;
	ULONG Count;
	ULONG Data;
} WRITE_MEMORY_INPUT;

typedef struct _SPD_STRUCT
{
	USHORT Base_Address;
	USHORT Slave_Address;
	UCHAR Offset;
}SPD_Head;

typedef struct _MSR_Request
{
	int core_id;
	ULONG64 msr_address;
	ULONG64 Value;     /* value to write if write requet
							 ignored if read request */
}MSR_Request, *PMSR_Request;

#ifndef _PCI_X_
#define _PCI_X_ 
//
// A PCI driver can read the complete 256 bytes of configuration
// information for any PCI device by calling:
//
//      ULONG
//      HalGetBusData (
//          _In_ BUS_DATA_TYPE        PCIConfiguration,
//          _In_ ULONG                PciBusNumber,
//          _In_ PCI_SLOT_NUMBER      VirtualSlotNumber,
//          _In_ PPCI_COMMON_CONFIG   &PCIDeviceConfig,
//          _In_ ULONG                sizeof (PCIDeviceConfig)
//      );
//
//      A return value of 0 means that the specified PCI bus does not exist.
//
//      A return value of 2, with a VendorID of PCI_INVALID_VENDORID means
//      that the PCI bus does exist, but there is no device at the specified
//      VirtualSlotNumber (PCI Device/Function number).
//
//




typedef struct _PCI_SLOT_NUMBER {
	union {
		struct {
			ULONG   DeviceNumber : 5;
			ULONG   FunctionNumber : 3;
			ULONG   Reserved : 24;
		} bits;
		ULONG   AsULONG;
	} u;
} PCI_SLOT_NUMBER, *PPCI_SLOT_NUMBER;

#define PCI_TYPE0_ADDRESSES             6
#define PCI_TYPE1_ADDRESSES             2
#define PCI_TYPE2_ADDRESSES             5

typedef struct _PCI_COMMON_HEADER {
	USHORT  VendorID;                   // (ro)
	USHORT  DeviceID;                   // (ro)
	USHORT  Command;                    // Device control
	USHORT  Status;
	UCHAR   RevisionID;                 // (ro)
	UCHAR   ProgIf;                     // (ro)
	UCHAR   SubClass;                   // (ro)
	UCHAR   BaseClass;                  // (ro)
	UCHAR   CacheLineSize;              // (ro+)
	UCHAR   LatencyTimer;               // (ro+)
	UCHAR   HeaderType;                 // (ro)
	UCHAR   BIST;                       // Built in self test

	union {
		struct _PCI_HEADER_TYPE_0 {
			ULONG   BaseAddresses[PCI_TYPE0_ADDRESSES];
			ULONG   CIS;
			USHORT  SubVendorID;
			USHORT  SubSystemID;
			ULONG   ROMBaseAddress;
			UCHAR   CapabilitiesPtr;
			UCHAR   Reserved1[3];
			ULONG   Reserved2;
			UCHAR   InterruptLine;      //
			UCHAR   InterruptPin;       // (ro)
			UCHAR   MinimumGrant;       // (ro)
			UCHAR   MaximumLatency;     // (ro)
		} type0;



		//
		// PCI to PCI Bridge
		//

		struct _PCI_HEADER_TYPE_1 {
			ULONG   BaseAddresses[PCI_TYPE1_ADDRESSES];
			UCHAR   PrimaryBus;
			UCHAR   SecondaryBus;
			UCHAR   SubordinateBus;
			UCHAR   SecondaryLatency;
			UCHAR   IOBase;
			UCHAR   IOLimit;
			USHORT  SecondaryStatus;
			USHORT  MemoryBase;
			USHORT  MemoryLimit;
			USHORT  PrefetchBase;
			USHORT  PrefetchLimit;
			ULONG   PrefetchBaseUpper32;
			ULONG   PrefetchLimitUpper32;
			USHORT  IOBaseUpper16;
			USHORT  IOLimitUpper16;
			UCHAR   CapabilitiesPtr;
			UCHAR   Reserved1[3];
			ULONG   ROMBaseAddress;
			UCHAR   InterruptLine;
			UCHAR   InterruptPin;
			USHORT  BridgeControl;
		} type1;

		//
		// PCI to CARDBUS Bridge
		//

		struct _PCI_HEADER_TYPE_2 {
			ULONG   SocketRegistersBaseAddress;
			UCHAR   CapabilitiesPtr;
			UCHAR   Reserved;
			USHORT  SecondaryStatus;
			UCHAR   PrimaryBus;
			UCHAR   SecondaryBus;
			UCHAR   SubordinateBus;
			UCHAR   SecondaryLatency;
			struct {
				ULONG   Base;
				ULONG   Limit;
			}       Range[PCI_TYPE2_ADDRESSES - 1];
			UCHAR   InterruptLine;
			UCHAR   InterruptPin;
			USHORT  BridgeControl;
		} type2;



	} u;

} PCI_COMMON_HEADER, *PPCI_COMMON_HEADER;

#ifdef __cplusplus

typedef struct _PCI_COMMON_CONFIG : PCI_COMMON_HEADER {
	UCHAR   DeviceSpecific[192];
} PCI_COMMON_CONFIG, *PPCI_COMMON_CONFIG;

#else

typedef struct _PCI_COMMON_CONFIG {
	PCI_COMMON_HEADER DUMMYSTRUCTNAME;
	UCHAR   DeviceSpecific[192];
} PCI_COMMON_CONFIG, *PPCI_COMMON_CONFIG;
#endif

#endif

typedef struct _PCI_ADDRESS
{
	USHORT bus;
	USHORT dev;
	USHORT func;
	UCHAR Offset;
	ULONG Data;
} PCI_ADDRESS;

#endif // !IOCTL_H


