#pragma once

/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取PCIE CFG的信息
**************************************************/

/**************************************************************
*@Function				GetPCIEConfigSpace
*@brief					获取PCIE Configuration Space
*@author				王硕
*@param
	*@PCIE_BaseAddress	IN PCIE内存地址
	*@bus				bus
	*@dev				device
	*@func				function
	*@Data				INOut PCIE配置空间数组
	*@DataSize			INOut PCIE配置空间大小4096
*@return
	*@BOOL				获取Configuration Space是否成功(根据GetAllpciinfo来确定)
****************************************************************/
BOOL GetPCIEConfigSpace(const ULONGLONG PCIE_BaseAddress, ULONG bus, ULONG dev, ULONG func, PVOID& Data, size_t DataSize);