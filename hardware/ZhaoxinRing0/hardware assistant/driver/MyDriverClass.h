#pragma once
/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-10-27
Description:从驱动中读取IO Memory PCI MSR SuperIO SPD等等信息
**************************************************/
#include <stdlib.h>
#include <conio.h>
#include <winioctl.h>

#include <algorithm>
#include "../../ZhaoxinRing0/ioctl.h"
#include "../../ZhaoxinRing0/Public.h"
#include "Load/Driver.h"


using namespace std;

//-----------------------------------------------------------------------------
//
// Global
//
//-----------------------------------------------------------------------------

extern HANDLE gHandle;
extern TCHAR gDriverPath[MAX_PATH];

#define		EC_SC					0x66
#define		EC_DATA					0x62
#define		RD_EC					0x80
#define		WR_EC					0x81
#define		BE_EC					0x82
#define		BD_EC					0x83
#define		QR_EC					0x84

class CRing0
{
public:
	CRing0();
	~CRing0();
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
	BOOL RdMsr(IN DWORD Index, OUT DWORD64& Data);

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
	BOOL WrMsr(IN DWORD Index, IN DWORD64 Data);

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
	BOOL RdMsrTx(IN DWORD Index, IN DWORD threadAffinityMask, OUT DWORD64& Data);

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
	BOOL WrMsrTx(IN DWORD Index, IN DWORD threadAffinityMask, IN DWORD64 Data);

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
	BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD& IO_Data);

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
	BOOL RdMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data);

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

	BOOL WrMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data);
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

	/*********************************************************
	*@Function					Pci_All_Config_Space
	*@brief						返回pci Config Space 全部信息
	*@author					王硕
	*@param
	*@return
		Pci_All_Config_Space	PCI Config Space 全部信息
	*********************************************************/
	const Pci_All_Config_Space& ReturnPCIConfigSpace();

	/**************************************************
	*@Function					GetECData
	*@brief						获取EC的信息
	*@author					王硕
	*@param
	*@Embedded_Controller_Data	OUT EC 256字节的信息
	*@return
	*@BOOL						读取是否成功
	***************************************************/
	BOOL GetECData(Embedded_Controller_Data& EC_Data);

	/***************************************************
	*@Function					SetECData
	*@brief						设置EC的信息
	*@author					王硕
	*@param
	*@EC_Addr					IN 要设置的EC的地址
	*@EC_Write_Data				IN 要设置的EC的数据
	*@return
	*@BOOL						设置是否成功
	****************************************************/
	BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data);

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
	explicit CRing0(const CRing0& x);
	CRing0& operator=(const CRing0& x);

public:
	USHORT  VendorID;		//设备的主Vendor
protected:
	HANDLE hdevice;			//设备句柄
	TCHAR*  G_DevicePath;	//设备符号链接
private:
	Pci_All_Config_Space Pci_Config_space; //PCI配置空间的数据
};