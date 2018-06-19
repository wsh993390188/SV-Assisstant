#pragma once

/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取Embedded Controller的信息
**************************************************/

#include "../../../../ACPI/ACPI/ACPI.h"

#define MK_PCIEADDR(bus, dev, func, off) (ULONG)(((bus & 0xFF) << 20) | (dev & 0x1F) << 15 | (func & 0x07) << 12 | off)

/**************************************************************
*@Function				Initialize
*@brief					加载ACPI dll
*@author				王硕
*@param
	*@hMoudle			IN Dll载入模块的线性地址
*@return
	*@BOOL				载入是否成功
****************************************************************/
BOOL Initialize(HMODULE& hMoudle);

/**************************************************************
*@Function				Free
*@brief					释放dll
*@author				王硕
*@param
	*@hMoudle			IN Dll载入模块的线性地址
*@return
	*@BOOL				释放是否成功
****************************************************************/
void Free(HMODULE& hMoudle);

/**************************************************************
*@Function				GetPCIEBaseAddress
*@brief					获取PCIE在Memory映射中的地址
*@author				王硕
*@param
	*@PCIE_BaseAddress	OUT 输出PCIE Configuration Space 在内存中的基地址
*@return
	*@BOOL				获取PCIE基地址是否成功
****************************************************************/
BOOL GetPCIEBaseAddress(_Out_ ULONGLONG& PCIE_BaseAddress);

/**************************************************************
*@Function				GetPCIEConfigSpace
*@brief					获取PCIE Configuration Space
*@author				王硕
*@param
	*@PCIE_BaseAddress	IN PCIE内存地址
	*@Data				INOut PCIE配置空间数组
	*@DataSize			INOut PCIE配置空间大小
*@return
	*@BOOL				获取Configuration Space是否成功
****************************************************************/
BOOL GetPCIEConfigSpace(const ULONGLONG PCIE_BaseAddress, PVOID* Data, size_t DataSize);