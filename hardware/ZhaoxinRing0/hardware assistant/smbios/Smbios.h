#pragma once
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <vector>

#pragma warning(disable : 4200)
#pragma warning(disable : 4566) 
using namespace std;

typedef unordered_map<string, string> MapString;

/************************************************************************/
/*					 Type 0         BIOS Information					*/
/************************************************************************/
// offset
#define Type0_Type														0x00
#define Type0_Length													0x01
#define Type0_Handle													0x02
#define Type0_Vendor													0x04//String
#define Type0_BIOS_Version												0x05//String
#define Type0_BIOS_Start_ADDR											0x06//WORD
#define Type0_BIOS_Release_Date											0x08//String
#define Type0_BIOS_ROM_SIZE												0x09//BYTE
#define Type0_BIOS_Characteristics										0x0A//QWORD
#define Type0_BIOS_Characteristics_EXtension							0x12//WORD
#define Type0_BIOS_Major_Release										0x14//BYTE
#define Type0_BIOS_Minor_Release										0x15//BYTE
#define Type0_Firmware_Major_Release									0x16//BYTE
#define Type0_Firmware_Minor_Release									0x17//BYTE
#define Type0_Extended_BIOS_ROM_SIZE									0x18//WORD

//bits

/************************************************************************/
/*					 Type 1         System Information					*/
/************************************************************************/

//offset
#define Type1_Type														0x00
#define Type1_Length													0x01
#define Type1_Handle													0x02
#define Type1_Manufacturer												0x04//String
#define Type1_Product_Name												0x05//String
#define Type1_Version													0x06//String
#define Type1_Serial_Number												0x07//String 
#define Type1_UUID														0x08//16BYTE
#define Type1_Wake_up_Type												0x18//BYTE (ENUM)
#define Type1_SKU_Number												0x19//String 
#define Type1_Family													0x1A//String 

enum Wake_up_TYPE
{
	Other = 0x01,
	UUnknown = 0x02,
	APM_Timer = 0x03,
	Modern_Ring = 0x04,
	LAN_Remote = 0x05,
	Power_switch = 0x06,
	PCI_PME = 0x07,
	AC_Power_Restored = 0x08
};
//Bytes or Bit

/************************************************************************/
/*					 Type 2         Baseboard Information				*/
/************************************************************************/

#define Type2_Type														0x00
#define Type2_Length													0x01
#define Type2_Handle													0x02
#define Type2_Manufacturer												0x04
#define Type2_Product_Name												0x05
#define Type2_Version													0x06
#define Type2_Serial_Number												0x07
#define Type2_Asset_Tag													0x08//String
#define Type2_Feature_Flags												0x09//BTYE（bit）
#define Type2_Location_in_Chassis										0x0A//String
#define Type2_Chassis_Handle											0x0B//WORD
#define Type2_Board_Type												0x0D//BYTE(ENUM)
#define Type2_Num_Obj_Handles											0x0E//BYTE
#define Type2_Obj_Handles												0x0F//n WORDs

enum BaseBoard_Type
{
	BroadUnknown = 0x01,
	BroadOther = 0x02,
	Server_Blade = 0x03,
	Connectivity_Switch = 0x04,
	System_Management_Module = 0x05,
	Processor_Module = 0x06,
	IO_Module = 0x07,
	Memory_Module = 0x08,
	Daughter_board = 0x09,
	Motherboard = 0x0A,
	Processor_Memory_Module = 0x0B,
	Processor_IO_Module = 0x0C,
	Interconnect_board = 0x0D
};
//Bytes or Bit

/************************************************************************/
/*			 Type 3         System Enclosure or 933 Chassis				*/
/************************************************************************/

#define Type3_Type														0x00
#define Type3_Length													0x01
#define Type3_Handle													0x02
#define Type3_Manufacturer												0x04
#define Type3_TypeE														0x05
#define Type3_Version													0x06
#define Type3_Serial_Number												0x07
#define Type3_Asset_Tag													0x08
#define Type3_Boot_Up_State												0x09
#define Type3_Power_Supply_Status										0x0A
#define Type3_Thermal_State												0x0B
#define Type3_Security_Status											0x0C
#define Type3_OEM_defined												0x0D
#define Type3_Height													0x11
#define Type3_Num_of_Power_Cords										0x12
#define Type3_Contained_Element_Count									0x13
#define Type3_Contained_Element_Length									0x14
#define Type3_Contained_Element											0x15

//Bytes or Bit
const std::string System_Enclosure_or_Chassis_Types[] = 
{
	("Other"),
	("Unknown"),
	("Desktop"),
	("Low Profile Desktop"),
	("Pizza Box"),
	("Mini Tower"),
	("Tower"),
	("Portable"),
	("Laptop"),
	("Notebook"),
	("Hand held"),
	("Docking Station"),
	("All In one"),
	("Sub Notebook"),
	("Space saving"),
	("Lunch Box"),
	("Main Server Chassis"),
	("Expansion Chassis"),
	("Subchassis"),
	("BUS Expansion Chassis"),
	("Peripheral Chassis"),
	("RAID Chassis"),
	("Rack Mount Chassis"),
	("Sealed case PC"),
	("Multi system Chassis"),
	("Compact PCI"),
	("Advanced TCA"),
	("Blade"),
	("Blade Enclosure"),
	("Tablet"),
	("Convertible"),
	("Detachable"),
	("IoT GateWay"),
	("Embedded PC"),
	("mini PC"),
	("Stick PC"),
};

enum SEC_States
{
	SEC_States_Other = 0x01,
	SEC_States_Unknown = 0x02,
	SEC_Safe = 0x03,
	SEC_Warning = 0x04,
	SEC_Critical = 0x05,
	SEC_Non_recoverable = 0x06
};

enum SEC_Security_States
{
	SEC_Security_States_Other = 0x01,
	SEC_Security_States_Unknown = 0x02,
	SEC_States_None = 0x03,
	SEC_External_interface_locked_out = 0x04,
	SEC_External_interface_enabled = 0x05
};
/************************************************************************/
/*			 Type 4        Processor Information						*/
/************************************************************************/

#define Type4_Type														0x00
#define Type4_Length													0x01
#define Type4_Handle													0x02
#define Type4_Socket_Designation										0x04
#define Type4_Processor_Type											0x05
#define Type4_Processor_Family											0x06
#define Type4_Processor_Manufacturer									0x07
#define Type4_Processor_ID												0x08
#define Type4_Processor_Version											0x10
#define Type4_Voltage													0x11
#define Type4_External_Clock											0x12
#define Type4_Max_Speed													0x14
#define Type4_Current_Speed												0x16
#define Type4_Status													0x18
#define Type4_Processor_Upgrade											0x19
#define Type4_L1_Cache_Handle											0x1A
#define Type4_L2_Cache_Handle											0x1C
#define Type4_L3_Cache_Handle											0x1E
#define Type4_Serial_Number												0x20
#define Type4_Asset_Tag													0x21
#define Type4_Part_Number												0x22
#define Type4_Core_Count												0x23
#define Type4_Core_Enable												0x24
#define Type4_Thread_Count												0x25
#define Type4_Processor_Characteristics									0x26
#define Type4_Processor_NFamily2										0x28
#define Type4_Core_Count2												0x2A
#define Type4_Core_Enable2												0x2C
#define Type4_Thread_Count2												0x2E

//Bytes or Bit
const std::string Processor_Type[] = 
{
	("Other"),
	("Unknown"),
	("Central Processor"),
	("Math Processor"),
	("DSP Processor"),
	("Video Processor")
};

const std::string Processor_Family[] =
{
	("Other"),
	("Unknown"),
	("8086"),
	("80286"),
	("Intel386™ processor"),
	("Intel486™ processor"),
	("8087"),
	("80287"),
	("80387"),
	("80487"),
	("Intel® Pentium® processor"),
	("Pentium® Pro processor"),
	("Pentium® II processor"),
	("Pentium® processor with MMX™ technology"),
	("Intel® Celeron® processor"),
	("Pentium® II Xeon™ processor"),
	("Pentium® III processor"),
	("M1 Family"),
	("M2 Family"),
	("Intel® Celeron® M processor"),
	("Intel® Pentium® 4 HT processor"),
	(""),
	(""),
	("AMD Duron™ Processor Family"),
	("K5 Family"),
	("K6 Family"),
	("K6-2"),
	("K6-3"),
	("AMD Athlon™ Processor Family"),
	("AMD29000 Family"),
	("K6-2+"),
	("Power PC Family"),
	("Power PC 601"),
	("Power PC 603"),
	("Power PC 603+"),
	("Power PC 604"),
	("Power PC 620"),
	("Power PC x704"),
	("Power PC 750"),
	("Intel® Core™ Duo processor"),
	("Intel® Core™ Duo mobile processor"),
	("Intel® Core™ Solo mobile processor"),
	("Intel® Atom™ processor"),
	("Intel® Core™ M processor"),
	("Intel(R) Core(TM) m3 processor"),
	("Intel(R) Core(TM) m5 processor"),
	("Intel(R) Core(TM) m7 processor"),
	("Alpha Family"),
	("Alpha 21064"),
	("Alpha 21066"),
	("Alpha 21164"),
	("Alpha 21164PC"),
	("Alpha 21164a"),
	("Alpha 21264"),
	("Alpha 21364"),
	("AMD Turion™ II Ultra Dual-Core Mobile M Processor Family"),
	("AMD Turion™ II Dual-Core Mobile M Processor Family"),
	("AMD Athlon™ II Dual-Core M Processor Family"),
	("AMD Opteron™ 6100 Series Processor"),
	("AMD Opteron™ 4100 Series Processor"),
	("AMD Opteron™ 6200 Series Processor"),
	("AMD Opteron™ 4200 Series Processor"),
	("AMD FX™ Series Processor"),
	("MIPS Family"),
	("MIPS R4000"),
	("MIPS R4200"),
	("MIPS R4400"),
	("MIPS R4600"),
	("MIPS R10000"),
	("AMD C-Series Processor"),
	("AMD E-Series Processor"),
	("AMD A-Series Processor"),
	("AMD G-Series Processor"),
	("AMD Z-Series Processor"),
	("AMD R-Series Processor"),
	("AMD Opteron™ 4300 Series Processor"),
	("AMD Opteron™ 6300 Series Processor"),
	("AMD Opteron™ 3300 Series Processor"),
	("AMD FirePro™ Series Processor"),
	("SPARC Family"),
	("SuperSPARC"),
	("microSPARC II"),
	("microSPARC IIep"),
	("UltraSPARC"),
	("UltraSPARC II"),
	("UltraSPARC Iii"),
	("UltraSPARC III"),
	("UltraSPARC IIIi"),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	("68040 Family"),
	("68xxx"),
	("68000"),
	("68010"),
	("68020"),
	("68030"),
	("AMD Athlon(TM) X4 Quad-Core Processor Family"),
	("AMD Opteron(TM) X1000 Series Processor"),
	("AMD Opteron(TM) X2000 Series APU"),
	("AMD Opteron(TM) A-Series Processor"),
	("AMD Opteron(TM) X3000 Series APU"),
	("AMD Zen Processor Family"),
	(""),
	(""),
	(""),
	(""),
	("Hobbit Family"),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	("Crusoe™ TM5000 Family"),
	("Crusoe™ TM3000 Family"),
	("Efficeon™ TM8000 Family"),
	(""),
	(""),
	(""),
	(""),
	(""),
	("Weitek"),
	(""),
	("Itanium™ processor"),
	("AMD Athlon™ 64 Processor Family"),
	("AMD Opteron™ Processor Family"),
	("AMD Sempron™ Processor Family"),
	("AMD Turion™ 64 Mobile Technology"),
	("Dual-Core AMD Opteron™ Processor Family"),
	("AMD Athlon™ 64 X2 Dual-Core Processor Family"),
	("AMD Turion™ 64 X2 Mobile Technology"),
	("Quad-Core AMD Opteron™ Processor Family"),
	("Third-Generation AMD Opteron™ Processor Family"),
	("AMD Phenom™ FX Quad-Core Processor Family"),
	("AMD Phenom™ X4 Quad-Core Processor Family"),
	("AMD Phenom™ X2 Quad-Core Processor Family"),
	("AMD Athlon™ X2 Dual-Core Processor Family"),
	("PA-RISC Family"),
	("PA-RISC 8500"),
	("PA-RISC 8000"),
	("PA-RISC 7300LC"),
	("PA-RISC 7200"),
	("PA-RISC 7100LC"),
	("PA-RISC 7100"),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	("V30 Family"),
	("Quad-Core Intel® Xeon® processor 3200 Series"),
	("Dual-Core Intel® Xeon® processor 3000 Series"),
	("Quad-Core Intel® Xeon® processor 5300 Series"),
	("Dual-Core Intel® Xeon® processor 5100 Series"),
	("Dual-Core Intel® Xeon® processor 5000 Series"),
	("Dual-Core Intel® Xeon® processor LV"),
	("Dual-Core Intel® Xeon® processor ULV"),
	("Dual-Core Intel® Xeon® processor 7100 Series"),
	("Quad-Core Intel® Xeon® processor 5400 Series"),
	("Quad-Core Intel® Xeon® processor"),
	("Dual-Core Intel® Xeon® processor 5200 Series"),
	("Dual-Core Intel® Xeon® processor 7200 Series"),
	("Quad-Core Intel® Xeon® processor 7300 Series"),
	("Quad-Core Intel® Xeon® processor 7400 Series"),
	("Multi-Core Intel® Xeon® processor 7400 Series"),
	("Pentium® III Xeon™ processor"),
	("Pentium® III Processor with Intel® SpeedStep™ Technology"),
	("Pentium® 4 Processor"),
	("Intel® Xeon® processor"),
	("AS400 Family"),
	("Intel® Xeon™ processor MP"),
	("AMD Athlon™ XP Processor Family"),
	("AMD Athlon™ MP Processor Family"),
	("Intel® Itanium® 2 processor"),
	("Intel® Pentium® M processor"),
	("Intel® Celeron® D processor"),
	("Intel® Pentium® D processor"),
	("Intel® Pentium® Processor Extreme Edition"),
	("Intel® Core™ Solo Processor"),
	("Reserved"),
	("Intel® Core™ 2 Duo Processor"),
	("Intel® Core™ 2 Solo processor"),
	("Intel® Core™ 2 Extreme processor"),
	("Intel® Core™ 2 Quad processor"),
	("Intel® Core™ 2 Extreme mobile processor"),
	("Intel® Core™ 2 Duo mobile processor"),
	("Intel® Core™ 2 Solo mobile processor"),
	("Intel® Core™ i7 processor"),
	("Dual-Core Intel® Celeron® processor"),
	("IBM390 Family"),
	("G4"),
	("G5"),
	("ESA/390 G6"),
	("z/Architecture base"),
	("Intel® Core™ i5 processor"),
	("Intel® Core™ i3 processor"),
	(""),
	(""),
	(""),
	("VIA C7™-M Processor Family"),
	("VIA C7™-D Processor Family"),
	("VIA C7™ Processor Family"),
	("VIA Eden™ Processor Family"),
	("Multi-Core Intel® Xeon® processor"),
	("Dual-Core Intel® Xeon® processor 3xxx Series"),
	("Quad-Core Intel® Xeon® processor 3xxx Series"),
	("VIA Nano™ Processor Family"),
	("Dual-Core Intel® Xeon® processor 5xxx Series"),
	("Quad-Core Intel® Xeon® processor 5xxx Series"),
	(""),
	("Dual-Core Intel® Xeon® processor 7xxx Series"),
	("Quad-Core Intel® Xeon® processor 7xxx Series"),
	("Multi-Core Intel® Xeon® processor 7xxx Series"),
	("Multi-Core Intel® Xeon® processor 3400 Series"),
	(""),
	(""),
	(""),
	("AMD Opteron™ 3000 Series Processor"),
	("AMD Sempron™ II Processor"),
	("Embedded AMD Opteron™ Quad-Core Processor Family"),
	("AMD Phenom™ Triple-Core Processor Family"),
	("AMD Turion™ Ultra Dual-Core Mobile Processor Family"),
	("AMD Turion™ Dual-Core Mobile Processor Family"),
	("AMD Athlon™ Dual-Core Processor Family"),
	("AMD Sempron™ SI Processor Family"),
	("AMD Phenom™ II Processor Family"),
	("AMD Athlon™ II Processor Family"),
	("Six-Core AMD Opteron™ Processor Family"),
	("AMD Sempron™ M Processor Family"),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	("i860"),
	("i960"),
	(""),
	(""),
	("Indicator to obtain the processor family from the Processor Family 2 field"),
	("Reserved"),
};

const std::string Processor_Socket[] =
{
	("Other"),
	("Unknown"),
	("Daughter Board"),
	("ZIF Socket"),
	("Replaceable Piggy Back"),
	("None"),
	("LIF Socket"),
	("Slot 1"),
	("Slot 2"),
	("370-pin socket"),
	("Slot A"),
	("Slot M"),
	("Slot 423"),
	("Socket A (Socket 462)"),
	("Socket 478"),
	("Socket 754"),
	("Socket 940"),
	("Socket 939"),
	("Socket mPGA604"),
	("Socket LGA771"),
	("Socket LGA775"),
	("Socket S1"),
	("Socket AM2"),
	("Socket F (1207)"),
	("Socket LGA1366"),
	("Socket G34"),
	("Socket AM3"),
	("Socket C32"),
	("Socket LGA1156"),
	("Socket LGA1567"),
	("Socket PGA988A"),
	("Socket BGA1288"),
	("Socket rPGA988B"),
	("Socket BGA1023"),
	("Socket BGA1224"),
	("Socket LGA1155"),
	("Socket LGA1356"),
	("Socket LGA2011"),
	("Socket FS1"),
	("Socket FS2"),
	("Socket FM1"),
	("Socket FM2"),
	("Socket LGA2011-3"),
	("Socket LGA1356-3"),
	("Socket LGA1150"),
	("Socket BGA1168"),
	("Socket BGA1234"),
	("Socket BGA1364"),
	("Socket AM4"),
	("Socket LGA1151"),
	("Socket BGA1356"),
	("Socket BGA1440"),
	("Socket BGA1515"),
	("Socket LGA3647-1"),
	("Socket SP3"),
	("Socket SP3r2")
};
/************************************************************************/
/*			 Type 5        Memory Controller Information				*/
/************************************************************************/

#define	Type5_Type														0x00
#define Type5_Length													0x01
#define Type5_Handle													0x02
#define Type5_Error_Detecting_Method									0x04
#define Type5_Error_COrrecting_Capability								0x05
#define Type5_Supported_Interleave										0x06
#define Type5_Current_Interleave										0x07
#define Type5_Maximum_Memory_Moudule_size								0x08
#define Type5_Supported_Speeds											0x09
#define Type5_Supported_Memory_Types									0x0B
#define Type5_Memory_Module_Voltage										0x0D
#define Type5_Num_of_Memory_Slots										0x0E

//Bytes or Bit
const std::string Error_Detected_Method[] =
{
	("Other"),
	("Unknown"),
	("None"),
	("8-bit Parity"),
	("32-bit ECC"),
	("64-bit ECC"),
	("128-bit ECC"),
	("CRC")
};

const std::string Error_Correcting_Capability[] =
{
	("Other"),
	("Unknown"),
	("None"),
	("Single-Bit Error Correcting"),
	("Double-Bit Error Correcting"),
	("Error Scrubbing"),
	("Reserve")
};


const std::string Interleave_Support_field[] =
{
	("Other"),
	("Unknown"),
	("One-Way Interleave"),
	("Two-Way Interleave"),
	("Four-Way Interleave"),
	("Eight-Way Interleave"),
	("Sixteen-Way Interleave")
};

const std::string Memory_Speed_field[] =
{
	("Other"),
	("Unknown"),
	("70ns"),
	("60ns"),
	("50ns"),
	("Reserve")
};

const std::string Memory_Types[] =
{
	("Other"),
	("Unknown"),
	("Standard"),
	("Fast Page Mode"),
	("EDO"),
	("Parity"),
	("ECC"),
	("SIMM"),
	("DIMM"),
	("Burst EDO"),
	("SDRAM"),
	("Reserve")
};
/************************************************************************/
/*			 Type 6        Memory Module Information					*/
/************************************************************************/

#define Type6_Type														0x00
#define Type6_Length													0x01
#define Type6_Handle													0x02
#define Type6_Socket_Designation										0x04
#define Type6_Bank_Connections											0x05
#define Type6_Current_Speed												0x06
#define Type6_Current_Memory_Types										0x07
#define Type6_Installed_Size											0x09
#define Type6_Enabled_Size												0x0A
#define Type6_Error_Status												0x0B

//Bytes or Bit

/************************************************************************/
/*					 Type 7        Cache Information					*/
/************************************************************************/

#define Type7_Type														0x00
#define Type7_Length													0x01
#define Type7_Handle													0x02
#define Type7_Socket_Designation										0x04
#define Type7_Cache_Configuration										0x05
#define Type7_Maximum_Cache_Size										0x07
#define Type7_Installed_Size											0x09
#define Type7_Supported_SRAM_Type										0x0B
#define Type7_Current_SRAM_Type											0x0D
#define Type7_Cache_Speed												0x0F
#define Type7_Error_Correction_Type										0x10
#define Type7_System_Cache_Type											0x11
#define Type7_Associativity												0x12
#define Type7_Maximum_Cache_Size2										0x13
#define Type7_Installed_Cache_Size2										0x17

//Bytes or Bit
const std::string SRAM_Type[] =
{
	("Other"),
	("Unknown"),
	("Non-Burst"),
	("Burst"),
	("Pipeline Burst"),
	("Synchronous"),
	("Asynchronous")
};

const std::string Error_Correction_Type[] =
{
	("Other"),
	("Unknown"),
	("None"),
	("Parity"),
	("Single-bit ECC"),
	("Multi-bit ECC"),
	("Reserved")
};

const std::string System_Cache_Type[] =
{
	("Other"),
	("Unknown"),
	("Instruction"),
	("Data"),
	("Unified")
};

const std::string Cache_Associativity[] =
{
	("Other"),
	("Unknown"),
	("Direct Mapped"),
	("2-way Set-Associative"),
	("4-way Set-Associative"),
	("Fully Associative"),
	("8-way Set-Associative"),
	("16-way Set-Associative"),
	("12-way Set-Associative"),
	("24-way Set-Associative"),
	("32-way Set-Associative"),
	("48-way Set-Associative"),
	("64-way Set-Associative"),
	("20-way Set-Associative"),
	("Reserved"),
};
/************************************************************************/
/*			 Type 8       Port Connector Information					*/
/************************************************************************/

#define Type8_Type														0x00
#define Type8_Length													0x01
#define Type8_Handle													0x02
#define Type8_Internal_Reference_Designator								0x04
#define Type8_Internal_Connector_Type									0x05
#define Type8_External_Reference_Designator								0x06
#define Type8_External_Connector_Type									0x07
#define Type8_Port_Type													0x08

//Bytes or Bit
const std::string Connector_Type[] =
{
	("None"),
	("Centronics"),
	("Mini Centronics"),
	("Proprietary"),
	("DB-25 pin male"),
	("DB-25 pin female"),
	("DB-15 pin male"),
	("DB-15 pin female"),
	("DB-9 pin male"),
	("DB-9 pin female"),
	("RJ-11"),
	("RJ-45"),
	("50-pin MiniSCSI"),
	("Mini-DIN"),
	("Micro-DIN"),
	("PS/2"),
	("Infrared"),
	("HP-HIL"),
	("Access Bus (USB)"),
	("SSA SCSI"),
	("Circular DIN-8 male"),
	("Circular DIN-8 female"),
	("On Board IDE"),
	("On Board Floppy"),
	("9-pin Dual Inline (pin 10 cut)"),
	("25-pin Dual Inline (pin 26 cut)"),
	("50-pin Dual Inline"),
	("68-pin Dual Inline"),
	("On Board Sound Input from CD-ROM"),
	("Mini-Centronics Type-14"),
	("Mini-Centronics Type-26"),
	("Mini-jack (headphones)"),
	("BNC"),
	("1394"),
	("SAS/SATA Plug Receptacle"),
	("PC-98"),
	("PC-98Hireso"),
	("PC-H98"),
	("PC-98Note"),
	("PC-98Full"),
	("Other – Use Reference Designator Strings to supply information")
};

const std::string Port_Types[] =
{
	("None"),
	("Parallel Port XT/AT Compatible"),
	("Parallel Port PS/2"),
	("Parallel Port ECP"),
	("Parallel Port EPP"),
	("Parallel Port ECP/EPP"),
	("Serial Port XT/AT Compatible"),
	("Serial Port 16450 Compatible"),
	("Serial Port 16550 Compatible"),
	("Serial Port 16550A Compatible"),
	("SCSI Port"),
	("MIDI Port"),
	("Joy Stick Port"),
	("Keyboard Port"),
	("Mouse Port"),
	("SSA SCSI"),
	("USB"),
	("FireWire (IEEE P1394)"),
	("PCMCIA Type I2"),
	("PCMCIA Type II"),
	("PCMCIA Type III"),
	("Cardbus"),
	("Access Bus Port"),
	("SCSI II"),
	("SCSI Wide"),
	("PC-98"),
	("PC-98-Hireso"),
	("PC-H98"),
	("Video Port"),
	("Audio Port"),
	("Modem Port"),
	("Network Port"),
	("SATA"),
	("SAS"),
	("8251 Compatible"),
	("8251 FIFO Compatible"),
	("Other")
};
/************************************************************************/
/*					Type 9       System Slots							*/
/************************************************************************/

#define Type9_Type														0x00
#define Type9_Length													0x01
#define Type9_Handle													0x02
#define Type9_Slot_Designation											0x04
#define Type9_Slot_Type													0x05
#define Type9_Slot_Data_Bus_Width										0x06
#define Type9_Current_Usage												0x07
#define Type9_Slot_Length												0x08
#define Type9_Slot_ID													0x09
#define Type9_Slot_Characteristics1										0x0B
#define Type9_Slot_Characteristics2										0x0C
#define Type9_Segment_Group_Num											0x0D
#define Type9_Bus_Number												0x0F
#define Type9_Dev_Func_Num												0x10

//Bytes or Bit
const std::string System_Slots_Type_first[] =
{
	("Other"),
	("Unknown"),
	("ISA"),
	("MCA"),
	("EISA"),
	("PCI"),
	("PC Card (PCMCIA)"),
	("VL-VESA"),
	("Proprietary"),
	("Processor Card Slot"),
	("Proprietary Memory Card Slot"),
	("I/O Riser Card Slot"),
	("NuBus"),
	("PCI – 66MHz Capable"),
	("AGP"),
	("AGP 2X"),
	("AGP 4X"),
	("PCI-X"),
	("AGP 8X"),
	("M.2 Socket 1-DP (Mechanical Key A)"),
	("M.2 Socket 1-SD (Mechanical Key E)"),
	("M.2 Socket 2 (Mechanical Key B)"),
	("M.2 Socket 3 (Mechanical Key M)"),
	("MXM Type I"),
	("MXM Type II"),
	("MXM Type III (standard connector)"),
	("MXM Type III (HE connector)"),
	("MXM Type IV"),
	("MXM 3.0 Type A"),
	("MXM 3.0 Type B"),
	("PCI Express Gen 2 SFF-8639"),
	("PCI Express Gen 3 SFF-8639"),
	("PCI Express Mini 52-pin (CEM spec. 2.0) with bottom - side keep - outs.Use Slot Length field value 03h(short length) for \"half-Mini card\" - only support, 04h(long length) for \"full-Mini card\" or dual support."),
	("PCI Express Mini 52-pin (CEM spec. 2.0) without	bottom - side keep - outs.Use Slot Length field value 03h(short length) for \"half-Mini card\" - only support, 04h(long length) for \"full-Mini card\" or dual support."),
	("PCI Express Mini 76-pin (CEM spec. 2.0) Corresponds to Display - Mini card.")
};
const std::string System_Slots_Type_second[] =
{
	("PC-98/C20"),
	("PC-98/C24"),
	("PC-98/E"),
	("PC-98/Local Bus"),
	("PC-98/Card"),
	("PCI Express (see below)"),
	("PCI Express x1"),
	("PCI Express x2"),
	("PCI Express x4"),
	("PCI Express x8"),
	("PCI Express x16"),
	("PCI Express Gen 2 (see below)"),
	("PCI Express Gen 2 x1"),
	("PCI Express Gen 2 x2"),
	("PCI Express Gen 2 x4"),
	("PCI Express Gen 2 x8"),
	("PCI Express Gen 2 x16"),
	("PCI Express Gen 3 (see below)"),
	("PCI Express Gen 3 x1"),
	("PCI Express Gen 3 x2"),
	("PCI Express Gen 3 x4"),
	("PCI Express Gen 3 x8"),
	("PCI Express Gen 3 x16")
};

const std::string System_Slot_Data_Bus_Width[] =
{
	("Other"),
	("Unknown"),
	("8 bit"),
	("16 bit"),
	("32 bit"),
	("64 bit"),
	("128 bit"),
	("1x or x1"),
	("2x or x2"),
	("4x or x4"),
	("8x or x8"),
	("12x or x12"),
	("16x or x16"),
	("32x or x32")
};

const std::string System_Slot_Current_Usage[] =
{
	("Other"),
	("Unknown"),
	("Available"),
	("In use")
};

const std::string System_Slot_Length[] =
{
	("Other"),
	("Unknown"),
	("Short Length"),
	("Long Length"),
	("2.5\" drive form factor"),
	("3.5\" drive form factor")
};	

//bits
const std::string System_Slot_Characteristics1[] =
{
	("Characteristics unknown."),
	("Provides 5.0 volts."),
	("Provides 3.3 volts."),
	("Slot’s opening is shared with another slot (for example, PCI/EISA shared slot)."),
	("PC Card slot supports PC Card-16."),
	("PC Card slot supports CardBus."),
	("PC Card slot supports Zoom Video."),
	("PC Card slot supports Modem Ring Resume.")
};
const std::string System_Slot_Characteristics2[] =
{
	("PCI slot supports Power Management Event (PME#) signal."),
	("Slot supports hot-plug devices."),
	("PCI slot supports SMBus signal.")
};

/************************************************************************/
/*				Type 10       Board Devices Information					*/
/************************************************************************/

#define Type10_Type														0x00
#define Type10_Length													0x01
#define Type10_Handle													0x02

const std::string Onboard_Device_Types[] =
{
	("Other"),
	("Unknown"),
	("Video"),
	("SCSI Controller"),
	("Ethernet"),
	("Token Ring"),
	("Sound"),
	("PATA Controller"),
	("SATA Controller"),
	("SAS Controller")
};

/************************************************************************/
/*				Type 11       OEM Strings								*/
/************************************************************************/

#define Type11_Type														0x00
#define Type11_Length													0x01
#define Type11_Handle													0x02
#define Type11_Count													0x04

/************************************************************************/
/*				Type 12      System Configuration Options				*/
/************************************************************************/

#define Type12_Type														0x00
#define Type12_Length													0x01
#define Type12_Handle													0x02
#define Type12_Count													0x04

/************************************************************************/
/*				Type 13      BIOS Language Information					*/
/************************************************************************/

#define Type13_Type														0x00
#define Type13_Length													0x01
#define Type13_Handle													0x02
#define Type13_Installable_Languages									0x04
#define Type13_Flags													0x05
#define Type13_Current_Languages										0x015

/************************************************************************/
/*				Type 14      Group Associations							*/
/************************************************************************/

#define Type14_Type														0x00
#define Type14_Length													0x01
#define Type14_Handle													0x02
#define Type14_Group_Name												0x04
#define Type14_Item_Type												0x05
#define Type14_Item_Handle												0x06

/************************************************************************/
/*				Type 15      System Event Log							*/
/************************************************************************/

#define Type15_Type														0x00
#define Type15_Length													0x01
#define Type15_Handle													0x02
#define Type15_Log_Area_Length											0x04
#define Type15_Log_Header_Start_Offset									0x06
#define Type15_Log_Data_Start_Offset									0x08
#define Type15_Access_Method											0x0A
#define Type15_Log_Status												0x0B
#define Type15_Log_Change_Token											0x0C
#define Type15_Access_Method_Address									0x10
#define Type15_Log_Header_Format										0x14
#define Type15_Num_of_Supported_Log_Types								0x15
#define Type15_Length_Log_Type											0x16

//Bytes or Bit
/************************************************************************/
/*				Type 16      Physical Memory Array						*/
/************************************************************************/

#define Type16_Type														0x00
#define Type16_Length													0x01
#define Type16_Handle													0x02
#define Type16_Location													0x04
#define Type16_Use														0x05
#define Type16_Memory_Error_Correction									0x06
#define Type16_Maximum_Capacity											0x07
#define Type16_Memory_Error_Information_Handle							0x0B
#define Type16_Num_of_Memory_Devices									0x0D
#define Type16_Extended_Maximum_Capacity								0x0F

//Bytes or Bit
const std::string MemArray_Location[] =
{
	("Other"),
	("Unknown"),
	("System board or motherboard"),
	("ISA add-on card"),
	("EISA add-on card"),
	("PCI add-on card"),
	("MCA add-on card"),
	("PCMCIA add-on card"),
	("Proprietary add-on card"),
	("NuBus"),
	("PC-98/C20 add-on card"),
	("PC-98/C24 add-on card"),
	("PC-98/E add-on card"),
	("PC-98/Local bus add-on card")
};

const std::string MemArray_Use[] = 
{
	("Other"),
	("Unknown"),
	("System memory"),
	("Video memory"),
	("Flash memory"),
	("Non-volatile RAM"),
	("Cache memory")
};

const std::string MemArray_Error[] =
{
	("Other"),
	("Unknown"),
	("None"),
	("Parity"),
	("Single-bit ECC"),
	("Multi-bit ECC"),
	("CRC")
};
/************************************************************************/
/*				Type 17      Memory Device								*/
/************************************************************************/

#define Type17_Type														0x00
#define Type17_Length													0x01
#define Type17_Handle													0x02
#define Type17_Physical_Memory_Array_Handle								0x04
#define Type17_Memory_Error_Information_Handle							0x06
#define Type17_Memory_Total_Width										0x08
#define Type17_Data_width												0x0A
#define Type17_Size														0x0C
#define Type17_Form_Factor												0x0E
#define Type17_Device_Set												0x0F
#define Type17_Device_Locator											0x10
#define Type17_Bank_Locator												0x11
#define Type17_Memory_Type												0x12
#define Type17_Type_Detail												0x13
#define Type17_Speed													0x15
#define Type17_Manufacturer												0x17
#define Type17_Serial_Number											0x18
#define Type17_Asset_Tag												0x19
#define Type17_Part_Number												0x1A
#define Type17_Attributes												0x1B
#define Type17_Extented_Size											0x1C
#define Type17_Configured_Memory_Clock_Speed							0x20
#define Type17_Minimum_Voltage											0x22
#define Type17_Maximum_Voltage											0x24
#define Type17_Configured_Voltage										0x26

//Bytes or Bit
const std::string FormFactor[] =
{
	("Other"),
	("Unknown"),
	("SIMM"),
	("SIP"),
	("Chip"),
	("DIP"),
	("ZIP"),
	("Proprietary Card"),
	("DIMM"),
	("TSOP"),
	("Row of chips"),
	("RIMM"),
	("SODIMM"),
	("SRIMM"),
	("FB-DIMM")
};

const std::string MemDeviceType[] =
{
	("Other"),
	("Unknown"),
	("DRAM"),
	("EDRAM"),
	("VRAM"),
	("SRAM"),
	("RAM"),
	("ROM"),
	("FLASH"),
	("EEPROM"),
	("FEPROM"),
	("CDRAM"),
	("3DRAM"),
	("SDRAM"),
	("SGRAM"),
	("RDRAM"),
	("DDR"),
	("DDR2"),
	("DDR2 FB-DIMM"),
	(""),
	(""),
	(""),
	("DDR3"),
	("FBD2"),
	("DDR4"),
	("LPDDR"),
	("LPDDR2"),
	("LPDDR3"),
	("LPDDR4")
};

const std::string MemDeviceTypeDetail[] =
{
	("Reserved"),
	("Other"),
	("Unknown"),
	("Fast-paged"),
	("Static column"),
	("Pseudo-static"),
	("RAMBUS"),
	("Synchronous"),
	("CMOS"),
	("EDO"),
	("Window DRAM"),
	("Cache DRAM"),
	("Non-volatile"),
	("Registered (Buffered)"),
	("Unbuffered (Unregistered)"),
	("LRDIMM")
};
/************************************************************************/
/*				Type 18      Memory Error Information					*/
/************************************************************************/

#define Type18_Type														0x00
#define Type18_Length													0x01
#define Type18_Handle													0x02
#define Type18_Error_Type												0x04
#define Type18_Error_Granularity										0x05
#define Type18_Error_Operation											0x06
#define Type18_Vendor_Syndrome											0x07
#define Type18_Memory_Array_Error_Address								0x0B
#define Type18_Device_Error_Address										0x0F
#define Type18_Error_Resolution											0x13

//Bytes or Bit
const std::string Memory_Error_Type[] =
{
	("Other"),
	("Unknown"),
	("OK"),
	("Bad read"),
	("Parity error"),
	("Single-bit error"),
	("Double-bit error"),
	("Multi-bit error"),
	("Nibble error"),
	("Checksum error"),
	("CRC error"),
	("Corrected single-bit error"),
	("Corrected error"),
	("Uncorrectable error")
};

const std::string Memory_Error_Granularity[] =
{
	("Other"),
	("Unknown"),
	("Device level"),
	("Memory partition level")
};

const std::string Memory_Error_Operation[] =
{
	("Other"),
	("Unknown"),
	("Read"),
	("Write"),
	("Partial Write")
};
/************************************************************************/
/*				Type 19      Memory Array Mapped Address				*/
/************************************************************************/

#define Type19_Type														0x00
#define Type19_Length													0x01
#define Type19_Handle													0x02
#define Type19_Starting_Address											0x04
#define Type19_Ending_Address											0x08
#define Type19_Memory_Array_Handle										0x0C
#define Type19_Partition_Width											0x0E
#define Type19_Extented_Starting_Address								0x0F
#define Type19_Extented_Ending_Address									0x17

//Bytes or Bit

/************************************************************************/
/*				Type 20     Memory Device Mapped Address				*/
/************************************************************************/

#define Type20_Type														0x00
#define Type20_Length													0x01
#define Type20_Handle													0x02
#define Type20_Starting_Address											0x04
#define Type20_Ending_Address											0x08
#define Type20_Memory_Device_Handle										0x0C
#define Type20_Memory_Array_Mapped_Address_Handle						0x0E
#define Type20_Partition_Row__Position									0x10
#define Type20_Interleave_Position										0x11
#define Type20_Interleave_Data_Depth									0x12
#define Type20_Extented_Starting_Address								0x13
#define Type20_Extented_Ending_Address									0x1B

//Bytes or Bit

/************************************************************************/
/*				Type 21    Built-in Pointing Device						*/
/************************************************************************/

#define Type21_Type														0x00
#define Type21_Length													0x01
#define Type21_Handle													0x02
#define Type21_Types													0x04
#define Type21_Interface												0x05
#define Type21_Num_of_Buttons											0x06

//Bytes or Bit
const std::string Pointing_Device_Type[] =
{
	("Other"),
	("Unknown"),
	("Mouse"),
	("Track Ball"),
	("Track Point"),
	("Glide Point"),
	("Touch Pad"),
	("Touch Screen"),
	("Optical Sensor")
};
const std::string Pointing_Device_Interface[] =
{
	("Other"),
	("Unknown"),
	("Serial"),
	("PS/2"),
	("Infrared"),
	("HP-HIL"),
	("Bus mouse"),
	("ADB (Apple Desktop Bus)"),
	("Bus mouse DB-9"),
	("Bus mouse micro-DIN"),
	("USB")
};
/************************************************************************/
/*				Type 22		 Portable Battery							*/
/************************************************************************/

#define Type22_Type														0x00
#define Type22_Length													0x01
#define Type22_Handle													0x02
#define Type22_Location													0x04
#define Type22_Manufacturer												0x05
#define Type22_Manufacturer_Date										0x06
#define Type22_Serial_Number											0x07
#define Type22_Device_Name												0x08
#define Type22_Device_Chemistry											0x09
#define Type22_Design_Capacity											0x0A
#define Type22_Design_Voltage											0x0C
#define Type22_SBDS_Version_Number										0x0E
#define Type22_Maximum_Error_In_Battery_Data							0x0F
#define Type22_SDBS_Serial_Number										0x10
#define Type22_SDBS_Manufacturer_Date									0x12
#define Type22_SDBS_Device_Chemistry									0x14
#define Type22_Design_Capacity_Multiplier								0x15
#define Type22_OEM_specific												0x16

//Bytes or Bit
const std::string Portable_Battery_Device_Chemistry[] =
{
	("Other"),
	("Unknown"),
	("Lead Acid"),
	("Nickel Cadmium"),
	("Nickel metal hydride"),
	("Lithium-ion"),
	("Zinc air"),
	("Lithium Polymer")
};
/************************************************************************/
/*					Type 23    System Reset 							*/
/************************************************************************/

#define Type23_Type														0x00
#define Type23_Length													0x01
#define Type23_Handle													0x02
#define Type23_Capabilities												0x04
#define Type23_Reset_Count												0x05
#define Type23_Reset_Limit												0x07
#define Type23_Timer_Interval											0x09
#define Type23_Timeout													0x0B

//Bytes or Bit

/************************************************************************/
/*					Type 24    Hardware Security 						*/
/************************************************************************/

#define Type24_Type														0x00
#define Type24_Length													0x01
#define Type24_Handle													0x02
#define Type24_Hardware_Security_Settings								0x04

//Bytes or Bit

/************************************************************************/
/*					Type 25    System Power Controls					*/
/************************************************************************/

#define Type25_Type														0x00
#define Type25_Length													0x01
#define Type25_Handle													0x02
#define Type25_Next_Scheduled_Power_on_Month							0x04
#define Type25_Next_Scheduled_Power_on_Day								0x05
#define Type25_Next_Scheduled_Power_on_Hour								0x06
#define Type25_Next_Scheduled_Power_on_Minute							0x07
#define Type25_Next_Scheduled_Power_on_Second							0x08

//Bytes or Bit

/************************************************************************/
/*						Type 26    Voltage Probe						*/
/************************************************************************/

#define Type26_Type														0x00
#define Type26_Length													0x01
#define Type26_Handle													0x02
#define Type26_Description												0x04
#define Type26_Location_and_Status										0x05
#define Type26_Maximum_Value											0x06
#define Type26_Minimum_Value											0x08
#define Type26_Resolution												0x0A
#define Type26_Tolerance												0x0C
#define Type26_Accuracy													0x0E
#define Type26_OEM_defined												0x10
#define Type26_Nominal_Value											0x14

//Bytes or Bit

/************************************************************************/
/*						Type 27   Cooling Device						*/
/************************************************************************/

#define Type27_Type														0x00
#define Type27_Length													0x01
#define Type27_Handle													0x02
#define Type27_Temperature_Probe_Handle									0x04
#define Type27_Device_Type_and_Status									0x06
#define Type27_Cooling_Unit_Group										0x07
#define Type27_OEM_defined												0x08
#define Type27_Nominal_Speed											0x0C
#define Type27_Description												0x0E

//Bytes or Bit

/************************************************************************/
/*						Type 28   Temperature Probe						*/
/************************************************************************/

#define Type28_Type														0x00
#define Type28_Length													0x01
#define Type28_Handle													0x02
#define Type28_Description												0x04
#define Type28_Location_and_Status										0x05
#define Type28_Maximum_Value											0x06
#define Type28_Minimum_Value											0x08
#define Type28_Resolution												0x0A
#define Type28_Tolerance												0x0C
#define Type28_Accuracy													0x0E
#define Type28_OEM_defined												0x10
#define Type28_Nominal_Value											0x14

//Bytes or Bit

/************************************************************************/
/*				Type 29   Electrical Current Probe						*/
/************************************************************************/

#define Type29_Type														0x00
#define Type29_Length													0x01
#define Type29_Handle													0x02
#define Type29_Description												0x04
#define Type29_Location_and_Status										0x05
#define Type29_Maximum_Value											0x06
#define Type29_Minimum_Value											0x08
#define Type29_Resolution												0x0A
#define Type29_Tolerance												0x0C
#define Type29_Accuracy													0x0E
#define Type29_OEM_defined												0x10
#define Type29_Nominal_Value											0x14

//Bytes or Bit

/************************************************************************/
/*				Type 30   Out-of-Band Remote Access						*/
/************************************************************************/

#define Type30_Type														0x00
#define Type30_Length													0x01
#define Type30_Handle													0x02
#define Type30_Manufacturer_Name										0x04
#define Type30_Connections												0x05

//Bytes or Bit

/************************************************************************/
/*				Type 31   Boot Integrity	 Services					*/
/************************************************************************/



//Bytes or Bit

/************************************************************************/
/*				Type 32   System Boot Information						*/
/************************************************************************/

#define Type32_Type														0x00
#define Type32_Length													0x01
#define Type32_Handle													0x02
#define Type32_Boot_Status												0x0A

//Bytes or Bit

/************************************************************************/
/*			Type 33   64-Bit Memory Error Information					*/
/************************************************************************/

#define Type33_Type														0x00
#define Type33_Length													0x01
#define Type33_Handle													0x02
#define Type33_Error_Type												0x04
#define Type33_Error_Granularity										0x05
#define Type33_Error_Operation											0x06
#define Type33_Vendor_Syndrome											0x07
#define Type33_Memory_Array_Error_Address								0x0B
#define Type33_Device_Error_Address										0x13
#define Type33_Error_Resolution											0x1B

//Bytes or Bit
/************************************************************************/
/*					Type 34   Management Device							*/
/************************************************************************/

#define Type34_Type														0x00
#define Type34_Length													0x01
#define Type34_Handle													0x02
#define Type34_Description												0x04
#define Type34_Types													0x05
#define Type34_Address													0x06
#define Type34_Address_Type												0x0A

//Bytes or Bit
const std::string Management_Device_Type[] =
{
	("Other"),
	("Unknown"),
	("National Semiconductor LM75"),
	("National Semiconductor LM78"),
	("National Semiconductor LM79"),
	("National Semiconductor LM80"),
	("National Semiconductor LM81"),
	("Analog Devices ADM9240"),
	("Dallas Semiconductor DS1780"),
	("Maxim 1617"),
	("Genesys GL518SM"),
	("Winbond W83781D"),
	("Holtek HT82H791")
};

const std::string Management_Device_Address_Type[] =
{
	("Other"),
	("Unknown"),
	("I/O Port"),
	("Memory"),
	("SM Bus")
};

/************************************************************************/
/*					Type 35   Management Device Component				*/
/************************************************************************/

#define Type35_Type														0x00
#define Type35_Length													0x01
#define Type35_Handle													0x02
#define Type35_Description												0x04
#define Type35_Management_Device_Handle									0x05
#define Type35_Component_Handle											0x07
#define Type35_Threshold_Handle											0x09

//Bytes or Bit

/************************************************************************/
/*				Type 36   Management Device Threshold Data				*/
/************************************************************************/

#define Type36_Type														0x00
#define Type36_Length													0x01
#define Type36_Handle													0x02
#define Type36_Lower_Threshold_Noncritical								0x04
#define Type36_Upper_Threshold_Noncritical								0x06
#define Type36_Lower_Threshold_Critical									0x08
#define Type36_Upper_Threshold_Critical									0x0A
#define Type36_Lower_Threshold_Nonrecoverable							0x0C
#define Type36_Upper_Threshold_Nonrecoverable							0x0E

//Bytes or Bit

/************************************************************************/
/*					Type 37   Memory Channel							*/
/************************************************************************/

#define Type37_Type														0x00
#define Type37_Length													0x01
#define Type37_Handle													0x02
#define Type37_Channel_Type												0x04
#define Type37_Maximum_Channel_Load										0x05
#define Type37_Memory_Device_Count										0x06
#define Type37_Memory1_Device_Load										0x07
#define Type37_Memory1_Device_Handle									0x08

//Bytes or Bit

/************************************************************************/
/*					Type 38   IPMI Device Information					*/
/************************************************************************/

#define Type38_Type														0x00
#define Type38_Length													0x01
#define Type38_Handle													0x02
#define Type38_Interface_Type											0x04
#define Type38_IPMI_Spec_Rev											0x05
#define Type38_I2C_SlaveAddress											0x06
#define Type38_NV_Storage_Device_Address								0x07
#define Type38_Base_Address												0x08
#define Type38_Base_Address_Mod_Interrupt								0x10
#define Type38_Interrupt_Number											0x11

//Bytes or Bit

/************************************************************************/
/*					Type 39   System Power Supply						*/
/************************************************************************/

#define Type39_Type														0x00
#define Type39_Length													0x01
#define Type39_Handle													0x02
#define Type39_Power_Unit_Group											0x04
#define Type39_Location													0x05
#define Type39_Device_Name												0x06
#define Type39_Manufacturer												0x07
#define Type39_Serial_Number											0x08
#define Type39_Asset_Tag_Number											0x09
#define Type39_Model_Part_Number										0x0A
#define Type39_Revision_Level											0x0B
#define Type39_Max_Power_Capacity										0x0C
#define Type39_Power_Supply_Characteristics								0x0E
#define Type39_Input_Voltage_Probe_Handle								0x10
#define Type39_Cooling_Device_Handle									0x12
#define Type39_Input_Current_Probe_Handle								0x14

//Bytes or Bit

/************************************************************************/
/*					Type 40   Additional Information					*/
/************************************************************************/

#define Type40_Type														0x00
#define Type40_Length													0x01
#define Type40_Handle													0x02
#define Type40_Num_of_Addition_Inf_Entries								0x04
#define Type40_Addition_Inf_Entries										0x05

//Bytes or Bit

/************************************************************************/
/*				Type 41   Onboard Devices Extended Information			*/
/************************************************************************/

#define Type41_Type														0x00
#define Type41_Length													0x01
#define Type41_Handle													0x02
#define Type41_Reference_Designation									0x04
#define Type41_Device_Type												0x05
#define Type41_Device_Type_Instance										0x06
#define Type41_Segment_Group_Number										0x07
#define Type41_Bus_Number												0x09
#define Type41_Device_Function_Number									0x0A

//Bytes or Bit

/************************************************************************/
/*				Type 42   Management Controller Host Interface			*/
/************************************************************************/

#define Type42_Type														0x00
#define Type42_Length													0x01
#define Type42_Handle													0x02
#define Type42_Interface_Type											0x04
#define Type42_MC_Host_Interface_Data									0x05

//Bytes or Bit

/************************************************************************/
/*						Type 43   TPM Device							*/
/************************************************************************/

#define Type43_Type														0x00
#define Type43_Length													0x01
#define Type43_Handle													0x02
#define Type43_Vendor_ID												0x04
#define Type43_Major_Spec_Version										0x08
#define Type43_Minor_Spec_Version										0x09
#define Type43_Firmware_Version1										0x0A
#define Type43_Firmware_Version2										0x0E
#define Type43_Description												0x12
#define Type43_Characteristics											0x13
#define Type43_OEM_defined												0x1B

//Bytes or Bit

/************************************************************************/
/*						Type 126	 Inactive							*/
/************************************************************************/

#define Type126_Type													0x00
#define Type126_Length													0x01
#define Type126_Handle													0x02


//Bytes or Bit

/************************************************************************/
/*						Type 127	 End-of-Table 						*/
/************************************************************************/

#define Type127_Type													0x00
#define Type127_Length													0x01
#define Type127_Handle													0x02

struct DMI_Header
{
	BYTE type;
	BYTE length;
	WORD handle;
};

struct RawSMBIOSData
{
	BYTE    Used20CallingMethod;
	BYTE    SMBIOSMajorVersion;
	BYTE    SMBIOSMinorVersion;
	BYTE    DmiRevision;
	DWORD    Length;
	BYTE    SMBIOSTableData[];
};

class UnCopyable
{
protected:
	UnCopyable() {}
	~UnCopyable() {}

private:
	UnCopyable(const UnCopyable& x);
	UnCopyable& operator=(const UnCopyable& x);
};

class Smbios : private UnCopyable
{
public:
	Smbios();
	~Smbios();
	void UpdateData();
	MapString BIOSinfo;	//Type0
	MapString Sysyteminfo;//Type1
	MapString BaseBroadinfo;//Type2
	MapString SystemEorC;//Type3
	MapString Processorinfo;//Type4
	MapString MemoryCtrlinfo;//Type5
	MapString MemoryModinfo;//Type6
	vector<MapString > Cacheinfo;//Type7
	vector<MapString > Portinfo;//Type8
	vector<MapString > SystemSlotinfo;//Type9
	MapString BroadDevinfo;//Type10
	MapString OEMString;//Type11
	MapString SystemConfigOption;//Type12
	MapString BIOSLanginfo;//Type13
	MapString GroupAssociations;//Type14
	MapString SysEventLog;//Type15
	vector<MapString > PhysicalMemoryArray;//Type16
	vector<MapString > MemoryDev;//Type17
	vector<MapString > MemoryErrinfo;//Type18
	vector<MapString > MemArrayMappedAddress;//Type19
	vector<MapString > MemDevMappedAddress;//Type20
	MapString Pointing_Dev;//Type21
	MapString Portable_Battery;//Type22
	MapString System_Reset;//Type23
	MapString Hardware_Security;//Type24
	MapString System_Power_Controls;//Type25
	vector<MapString > Voltage_Probe;//Type26
	vector<MapString > CoolingDev;//Type27
	vector<MapString > TemperatureProbe;//Type28
	vector<MapString > ElectricalCurrentProbe;//Type29
	vector<MapString > BandRemoteAccess;//Type30
	vector<MapString > Systembootstatus;//Type32
	vector<MapString > MemoryError64Bit;//Type33
	vector<MapString > ManagementDevice;//Type34
	vector<MapString > ManagementDeviceComponent;//Type35
	vector<MapString > ManagementDeviceComponentThresholdData;//Type36
	vector<MapString > MemoryChannel;//Type37
	vector<MapString > IPMIDeviceinfo;//Type38
	vector<MapString > SystemPowerSupply;//Type39
	vector<MapString > Additionalinfo;//Type40
	vector<MapString > BroadDevExtendedinfo;//Type41
	vector<MapString > ManagementControllerHostInterface;//Type42
	vector<MapString > TPMDevice;//Type43
	vector<MapString > Inactive;//Type126
	vector<MapString > EndofTable;//Type127
private:
	template<typename T>
	T BigEndian_to_LittleEndian(T Data);
	const char* dmi_to_string(const DMI_Header *dmi, BYTE Source_string);
	template< class T >
	void SafeDeleteArray(T*& pVal);
	template<class T>
	void SafeDeleteData(T *& pVal);
	BOOL GetSmbiosinfo();

};

