#pragma once
#include "PCI//PCI defination.h"
typedef struct _DDR3_INFO
{
	union
	{
		struct
		{
			BYTE Byte_Used : 4; // SPD Bytes Used
			BYTE Byte_Total : 3; // SPD Bytes Total
			BYTE CRC_Coverage : 1; //CRC Coverage
		}bits;
		BYTE SPD_Bytes_info; //BYTE0 Value
	}sSPDinfo;// Byte 0: Number of Bytes Used / Number of Bytes in SPD Device / CRC Coverage
	BYTE sSPDRevision; // Byte 1: SPD Revision
	BYTE sDRAMType; // Byte 2: Key Byte / DRAM Device Type
	union
	{
		struct
		{
			BYTE Module_Type : 4; // Module Type
			BYTE Reserve : 4; // Reserve
		}bits;
		BYTE ModuleType; // BYTE3 Value
	}sModuleTpye;// Byte 3: Key Byte / Module Type
	union
	{
		struct
		{
			BYTE Capacity : 4; // Total SDRAM capacity, in megabits
			BYTE Bank : 3; // Bank Address Bits
			BYTE Reserved : 1; // Reserved
		}bits;
		BYTE SDRAMBank_Density; //BYTE4 Value
	}sSDRAMBank_Density;// Byte 4 : SDRAM Density and Banks
	union
	{
		struct
		{
			BYTE Column_Address : 3; // Column Address Bits
			BYTE Row_Address : 3; // Row Address Bits
			BYTE Reserved : 2; // Reserved
		}bits;
		BYTE SDRAMAddr; //BYTE5 Value
	}sSDRAMAddr;// Byte 5: SDRAM Addressing
	union
	{
		struct
		{
			BYTE VDD_1_5 : 1; // 1.5 V operable 0 = 1.5 V operable 1 = NOT 1.5 V operable
			BYTE VDD_1_35 : 1; // 1.35 V operable 0 = NOT 1.35 V operable 1 = 1.35 V operable
			BYTE VDD_1_2X : 1; // 1.2X V operable 0 = NOT 1.2X V operable 1 = 1.2X V operable
			BYTE Reserved : 5; // Reserved
		}bits;
		BYTE VDD; // BYTE6 Value
	}sModuleNominalVoltage;// Byte 6: Module Nominal Voltage, VDD
	union
	{
		struct
		{
			BYTE SDRAM_Depth_Width : 3; // SDRAM Device Width
			BYTE Number_of_Ranks : 3; // Number of Ranks
			BYTE Reserved : 2; // Reserved
		}bits;
		BYTE ModuleOrganization; //BYTE7 Value
	}sModuleOrganization;// Byte 7: Module Organization
	union
	{
		struct
		{
			BYTE Primary_bus_width : 3; // Primary bus width, in bits
			BYTE Bus_width_extension : 2; // Bus width extension, in bits
			BYTE Reserved : 3; // Reserved
		}bits;
		BYTE BusWidth; //BYTE8 Value
	}sBusWidth;// Byte 8: Module Memory Bus Width
	union
	{
		struct
		{
			BYTE FTB_Divisor : 4; // Fine Timebase (FTB) Divisor
			BYTE FTB_Dividend : 4; // Values defined from 1 to 15
		}bits;
		BYTE FTB; //BYTE9 Value
	}sFTB;// Byte 9: Fine Timebase (FTB) Dividend / Divisor
	BYTE MTB_Dividend; // Byte 10: Medium Timebase (MTB) Dividend
	BYTE MTB_Divisor; // Byte 11: Medium Timebase (MTB) Divisor
	BYTE sTCKmin;// Byte 12: SDRAM Minimum Cycle Time (tCKmin) Results(ns) = Tckmin(MTB units) * Timebase(ns) 
	BYTE Reserved13; // Byte 13: Reserved
	union
	{
		struct
		{
			BYTE CL4 : 1; // CL = 4
			BYTE CL5 : 1; // CL = 5
			BYTE CL6 : 1; // CL = 6
			BYTE CL7 : 1; // CL = 7
			BYTE CL8 : 1; // CL = 8
			BYTE CL9 : 1; // CL = 9
			BYTE CL10 : 1; // CL = 10
			BYTE CL11 : 1; // CL = 11
		}bits;
		BYTE LeastSignificant; //BYTE14 Value
	}sCASLeastSignificant;// Byte 14: CAS Latencies Supported, Least Significant Byte
	union
	{
		struct
		{
			BYTE CL12 : 1; // CL = 12
			BYTE CL13 : 1; // CL = 13
			BYTE CL14 : 1; // CL = 14
			BYTE CL15 : 1; // CL = 15
			BYTE CL16 : 1; // CL = 16
			BYTE CL17 : 1; // CL = 17
			BYTE CL18 : 1; // CL = 18
			BYTE Reserved : 1; // Reserved
		}bits;
		BYTE MostSignificant; //BYTE15 Value
	}sCASMostSignificant;// Byte 15: CAS Latencies Supported, Most Significant Byte
	BYTE sTAAmin; // Byte 16: Minimum CAS Latency Time (tAAmin) tAAmin Result(ns) = tAAmin(MTB units) * Timebase(ns)
	BYTE sTWRmin; // Byte 17: Minimum Write Recovery Time (tWRmin) tWRmin Result(ns) = tWRmin(MTB units) * Timebase(ns)
	BYTE sTRCDmin; // Byte 18: Minimum RAS# to CAS# Delay Time (tRCDmin) tRCDmin Result(ns) = tRCDmin(MTB units) * Timebase(ns)
	BYTE sTRDDmin; // Byte 19: Minimum Row Active to Row Active Delay Time (tRRDmin) tRRDmin Result(ns) = tRRDmin(MTB units) * Timebase(ns)
	BYTE sTRPmin; // Byte 20: Minimum Row Precharge Delay Time (tRPmin) tRPmin Result(ns) = tRPmin(MTB units) * Timebase(ns)
	union
	{
		struct
		{
			BYTE tRCmin_Most : 4; // tRC Most Significant Nibble
			BYTE tRASmin_Most : 4; // tRAS Most Significant Nibble
		}bits;
		BYTE Upper_TRAS_TRCmin; //BYTE21 Value
	}sUpper_TRAS_TRCmin; // Byte 21: Upper Nibbles for tRAS and tRC
	BYTE sTRASminLeast; // Byte 22: Minimum Active to Precharge Delay Time (tRASmin), Least Significant Byte
	BYTE sTRCminLeast; // Byte 23: Minimum Active to Active/Refresh Delay Time (tRCmin), Least Significant Byte
	BYTE sTRFCminLeast; // Byte 24: Minimum Refresh Recovery Delay Time (tRFCmin), Least Significant Byte
	BYTE sTRFCminMost; // Byte 25: Minimum Refresh Recovery Delay Time (tRFCmin), Most Significant Byte
	BYTE sTWTRmin; // Byte 26: Minimum Internal Write to Read Command Delay Time (tWTRmin)
	BYTE sTRTPmin; // Byte 27: Minimum Internal Read to Precharge Command Delay Time (tRTPmin)
	union
	{
		struct
		{
			BYTE tFAWmin_Most : 4; // tRC Most Significant Nibble
			BYTE Reserved : 4; // Reserved
		}bits;
		BYTE TFAWminMost; //BYTE28 Value
	}sTFAWminMost; // Byte 28: Upper Nibble for tFAW
	BYTE sTFAWminMeast; // Byte 29: Minimum Four Activate Window Delay Time (tFAWmin), Least Significant Byte
	union
	{
		struct
		{
			BYTE RZQ_6 : 1; // RZQ / 6 0 = Not Supported 1 = Supported
			BYTE RZQ_7 : 1; // RZQ / 7 0 = Not Supported 1 = Supported
			BYTE Reserved : 5; // Reserved
			BYTE DLL_Off_Mode : 1; // DLL-Off Mode Support 0 = Not Supported 1 = Supported
		}bits;
		BYTE SDRAMOptional; //BYTE30 Value
	}sSDRAMOptional; // Byte 30: SDRAM Optional Features
	union
	{
		struct
		{
			BYTE ExtendedTemRange : 1; // Extended Temperature Range 1 = Normal and	extended operating temperature range 0 - 95 °C supported 0 = Normal operating temperature range	0 - 85 °C supported
			BYTE ExtendedTemRefreshRate : 1; // Extended Temperature Refresh Rate 1 = Extended operating temperature range from 85 - 95 °C supported with standard 1X refresh rate	0 = Use in extended	operating temperature range from 85 - 95 °C requires 2X refresh	rate
			BYTE ASR : 1; //Auto Self Refresh 1 = ASR is supported and the SDRAM will determine the proper refresh rate for any supported	temperature 0 = ASR is not supported
			BYTE ODTS : 1; // On-die Thermal Sensor(ODTS) Readout 1 = On - die thermal sensor readout is supported 0 = On - die thermal sensor readout is not supported(pending ballot of ODTS)
			BYTE Reserved : 3; // Reserved
			BYTE PASR : 1; // Partial Array Self Refresh 1 = Supported 0 = Not supported
		}bits;
		BYTE ThermalRefresh; //BYTE31 Value
	}sThermalRefresh;// Byte 31: SDRAM Thermal and Refresh Options
	union
	{
		struct
		{
			BYTE ThermalSensorAccuracy : 7; // 0 = Undefined All others settings to be defined.
			BYTE ThermalSensor : 1; // 0 = Thermal sensor not incorporated onto this assembly 1 = Thermal sensor incorporated onto this assembly
		}bits;
		BYTE ThermalSensor; //BYTE32 Value Note : Thermal sensor compliant with TSE2002 specifications.
	}sThermalSensor; // Byte 32: Module Thermal Sensor
	union
	{
		struct
		{
			BYTE NonStandardDescription : 7; // Non-Standard Device Description 0 = Undefined All others settings to be defined.
			BYTE SDRAMDeviceType : 1; //0 = Standard Monolithic DRAM Device 1 = Non-Standard Device (This includes Dual Die, Quad Die, Multi-Die and Physical stacked devices - anything that is outside the standard monolithic device.)	
		}bits;
		BYTE SDRAMDeviceType; //BYTE33 Value
	}sSDRAMDeviceType; // Byte 33: SDRAM Device Type
	BYTE Reserved34[26]; // Byte 34 ~ 59: Reserved, General Section
						 // Module-Specific Section: Bytes 60 ~ 116
	BYTE Module_Specific[57];
	// Unique Module ID: Bytes 117 ~ 125
	BYTE ModuleManufacturerIDLeast; // Byte 117: Module Manufacturer ID Code, Least Significant Byte
	BYTE ModuleManufacturerIDMost; // Byte 118: Module Manufacturer ID Code, Most Significant Byte
	BYTE ManufacturingLocation; // Byte 119: Module Manufacturing Location
	BYTE ManufacturingDate[2]; // Bytes 120 ~ 121: Module Manufacturing Date
	BYTE SerialNumber[4]; // Bytes 122 ~ 125: Module Serial Number
	BYTE CRC[2]; // Bytes 126 ~ 127: SPD Cyclical Redundancy Code (CRC)
	BYTE PartNumber[18]; // Bytes 128 ~ 145: Module Part Number
	BYTE RevisionCode[2]; // Bytes 146 ~ 147: Module Revision Code
	BYTE DRAMManufacturerIDLeast; // Byte 148: DRAM Manufacturer ID Code, Least Significant Byte
	BYTE DRAMManufacturerIDMost; // Byte 149: DRAM Manufacturer ID Code, Most Significant Byte
	BYTE ManufacturerSpecificData[26]; // Bytes 150 ~ 175: Manufacturer’s Specific Data
	BYTE CustomerUse[80]; // Open for Customer Use
} DDR3_INFO, *PDDR3_INFO;

//EC Data 共256个字节 具体每一位代表什么需要相应的spec来确定
struct Embedded_Controller_Data
{
	BYTE	Data[256];
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