#pragma once

#include <tuple>
#include <map>
#include <string>

#pragma pack(push, 1)
//EC Data 共256个字节 具体每一位代表什么需要相应的spec来确定
struct Embedded_Controller_Data
{
	BYTE	Data[256];
};

struct PCISPEED
{
	USHORT LinkWidth;
	USHORT LinkSpeed;
};


#ifndef _PCI_X_
#define _PCI_X_
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
	UCHAR   DeviceSpecific[192];

} PCI_COMMON_CONFIG, *PPCI_COMMON_CONFIG;

typedef struct _PCI_SLOT_NUMBER {
	union {
		struct {
			ULONG   FunctionNumber : 3;
			ULONG   DeviceNumber : 5;
			ULONG   Reserved : 24;
		} bits;
		ULONG   AsULONG;
	} u;
} PCI_SLOT_NUMBER, *PPCI_SLOT_NUMBER;
#endif // !IOCTL_H

typedef struct _Pci_All_Config_Space
{
	std::vector<std::pair<std::tuple<USHORT, USHORT, USHORT>, PCI_COMMON_CONFIG>> pciconfig;
}Pci_All_Config_Space, *PPci_All_Config_Space;

struct PCI_OPTION_ROM_HEADER
{
	BYTE								Signature[2]; // must be 0x55 0xAA
	BYTE								Length;
	DWORD								Initialization_Entry;
	BYTE								Reserved[17];
	WORD								PCI_Data_Offset;
	BYTE								Expansion_Header_Offset;
};

struct PCI_OPTION_ROM_Data_Structure
{
	BYTE								Signature[4]; // must be 'PCIR'
	WORD								VendorID;                   // (ro)
	WORD								DeviceID;                   // (ro)
	WORD								Product_Data;
	WORD								Structure_Length;
	BYTE								Structure_Revision;
	BYTE								Class_Code[3];
	WORD								Image_Length;
	WORD								Revision_Level;
	BYTE								Code_Type;
	BYTE								Indicator;
	WORD								Reserved;
};
#pragma pack(pop)