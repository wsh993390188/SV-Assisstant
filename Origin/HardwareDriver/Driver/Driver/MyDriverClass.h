#pragma once
/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-10-27
Description:从驱动中读取IO Memory PCI MSR SuperIO SPD等等信息
**************************************************/
#include <windows.h>
#include <setupapi.h>
#include <stdlib.h>
#include <conio.h>
#include <winioctl.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include "../../CharTest/ioctl.h"
#include "../../CharTest/Public.h"
#include "Load/Driver.h"
#include "SuperIo.h"
using namespace std;

//-----------------------------------------------------------------------------
//
// Global
//
//-----------------------------------------------------------------------------

extern HANDLE gHandle;
extern TCHAR gDriverPath[MAX_PATH];

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")
typedef struct _Pci_All_Config_Space
{
	std::vector<PCHAR> Label;
	std::vector<PCI_COMMON_CONFIG> Pci_Config_Space;

}Pci_All_Config_Space, *PPci_All_Config_Space;
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
class CRing0
{
public:
	CRing0();
	virtual ~CRing0();
	/**************************************************
	*@Function				RdMsr
	*@brief					获取MSR的值
	*@author				王硕
	*@param
		*@Index				IN 输入MSR的索引
		*@threadAffinityMask设置线程所运行的核心
		*@Data				OUT 输出MSR的值，64位
	*@return
		*@BOOL				是否成功
	***************************************************/
	BOOL RdMsr(IN DWORD Index, OUT DWORD64& Data, IN DWORD_PTR threadAffinityMask);

	/**************************************************
	*@Function				WrMsr
	*@brief					写MSR的值
	*@author				王硕
	*@param
		*@Index				IN 输入MSR的索引
		*@threadAffinityMask设置线程所运行的核心
		*@Data				IN 需要写入MSR的值，64位
	*@return
		*@BOOL				是否成功
	***************************************************/
	BOOL WrMsr(IN DWORD Index, IN DWORD64 Data, IN DWORD_PTR threadAffinityMask);

	/**********************************************************************
	*@Function				RdIOPort
	*@brief					读取IO Port的值
	*@author				王硕
	*@param
		*@IO_Port_Addr		IN IO Port的地址
		*@IO_DataSize		IN IO 输出数据的大小 1、2、4（8、16、32bits）
		*@IO_Data			OUT 输出的IO Port中的值
	*@return
	*@BOOL				是否成功
	**************************************************************************/
	BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD64& IO_Data);

	/**********************************************************************
	*@Function				WrIOPort
	*@brief				写入IO Port的值
	*@author			王硕
	*@param
		*@IO_Data			IN 输入IO Port的值
		*@IO_DataSize		IN IO 输出数据的大小 1、2、4（8、16、32bits）
	*@IO_Port_Addr		IN 输入IO Port的地址
	*@return
		*@BOOL				是否成功
	**************************************************************************/
	BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data);

	/**************************************************************************
	*@Function				RdMemory
	*@brief					读取内存的值
	*@author				王硕
	*@param
		*@IO_Port_Addr		IN 输入Memory的地址
		*@IO_DataSize		IN Memory 输出数据的大小 1、2、4（8、16、32bits）
		*@IO_Data			OUT 输出的Memory中的值
	*@return
		*@BOOL				是否成功
	***************************************************************************/
	BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data);

	/***************************************************************************
	*@Function				WrMemory
	*@brief					写入内存的值
	*@author				王硕
	*@param
		*@IO_Data			IN 输入Memory的值
		*@IO_DataSize		IN Memory 输出数据的大小 1、2、4（8、16、32bits）
		*@IO_Port_Addr		IN 输入Memory的地址
	*@return
		*@BOOL				是否成功
	******************************************************************************/

	BOOL WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data);
	/*************************************************
	*@Function				ReadPci
	*@brief					读PCI配置空间的数据
	*@author				王硕
	*@param
		*@bus				IN 输入bus的值
		*@dev				IN 输入dev的值
		*@func				IN 输入func的值
		*@pci_config		OUT 输出Pci配置空间的数据
	*@return
		*@BOOL				是否成功
	**************************************************/

	BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG& pci_config);

	/*************************************************
	*@Function				WritePci
	*@brief					写PCI配置空间的数据
	*@author				王硕
	*@param
		*@bus				IN 输入bus的值
		*@dev				IN 输入dev的值
		*@func				IN 输入func的值
		*@offset			IN 输入要写入偏移量的值
		*@Data				IN 输入要写入数据的值
	*@return
		*@BOOL				是否成功
	**************************************************/
	BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data);

	/*************************************************
	*@Function				ReadAllPci
	*@brief					读全部PCI配置空间的数据
	*@author				王硕
	*@param
	*@return
	*@存储位置			类中的Pci_Config_space变量
	**************************************************/
	BOOL ReadAllPci();

	/*************************************************
	*@Function				ReadSPD
	*@brief					读SPD的数据
	*@author				王硕
	*@param
		*@Base_Address		IN 输入SMBUS的地址
		*@Slave_Address		IN 输入从设备的地址
		*@offset			IN 输入要读取的偏移量的值
	*@return			
	**************************************************/
	BOOL ReadSPD(USHORT Base_Address, USHORT Slave_Address, UCHAR offset);

	/*************************************************
	*@Function				ReadITE
	*@brief					读ITE芯片中的数据
	*@author				王硕
	*@param
	*@return
	**************************************************/
	BOOL ReadITE();

	/*********************************************************
	*@Function					Pci_All_Config_Space
	*@brief						返回pci Config Space 全部信息
	*@author					王硕
	*@param
	*@return
		Pci_All_Config_Space	PCI Config Space 全部信息
	*********************************************************/
	const Pci_All_Config_Space ReturnPCIConfigSpace();

private:
	/*****************************************************************
	*@Function				CRing0_Initialize
	*@brief					对CRing0驱动进行初始化
	*@author				王硕
	*@param					无
	*@return
		*@TRUE				初始化完毕
		*@Others			获取失败，返回的是错误代码，GetLastError()。
	******************************************************************/
	BOOL CRing0_Initialize();

	/****************************************************
	*@Function				CRing0_Destory
	*@brief					对CRing0驱动进行退出时操作
	*@author				王硕
	*@param					无
	*@return
		*@TRUE				设备句柄关闭完毕
	****************************************************/
	BOOL CRing0_Destory();

	/****************************************************
	*@Function				GetDevicePath
	*@brief					通过GUID来获取设备的符号链接
	*@author				王硕
	*@param					无
	*@return
		*@PCHAR				设备的符号链接名
	****************************************************/
	PCHAR GetDevicePath(IN LPGUID InterfaceGuid);

	/****************************************************
	*@Function				GetDevicePath
	*@brief					获取数组长度
	*@author				王硕
	*@param			
		*@Array				输入静态数组
	*@return
		*@int				返回数组长度
	****************************************************/
	template<typename T>
	int GetStaticArraylength(IN T& Array);

	/*************************************************************
	*@Function					find_location
	*@brief						寻找字符串在矢量中的位置
	*@author					王硕
	*@param
		*@vecIntegerArray		输入矢量
		*@search_string			输入要寻找的字符串
	*@return
		*@INT64					返回字符串在矢量中位置 -1代表未找到
	*************************************************************/
	template<class T>
	INT64 find_location(vector<T> vecIntegerArray, string search_string);

public:
	USHORT  VendorID;		//设备的主Vendor
protected:
	DDR3_INFO Data;			//SPD一个字节数据的存储对象
	HANDLE hdevice;			//设备句柄
	PCHAR  G_DevicePath;	//设备符号链接
	HMODULE  library;		//wdfCoInstall dll
private:
	Pci_All_Config_Space Pci_Config_space; //PCI配置空间的数据
	const USHORT SuperIO_Ctrl_Port;		//SuperIO的控制端口
	const USHORT SuperIO_Data_Port;		//SuperIO的数据端口
	vector<Super_IO> SuperIO;			//SuperIO中的数据矢量组
};