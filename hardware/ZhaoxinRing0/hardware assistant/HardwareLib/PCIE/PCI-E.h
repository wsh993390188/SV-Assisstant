#pragma once

/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取PCIE CFG的信息
**************************************************/
namespace SV_ASSIST
{
	namespace PCIE
	{
		/**************************************************************
		*@Function				GetPCIEConfigSpace
		*@brief					获取PCIE Configuration Space
		*@author				王硕
		*@param
			*@PCIE_BaseAddress	IN PCIE内存地址
			*@Data				INOut PCIE配置空间数组
			*@DataSize			INOut PCIE配置空间大小4096
		*@return
			*@BOOL				获取Configuration Space是否成功
		****************************************************************/
		BOOL GetPCIEConfigSpace(const ULONGLONG PCIE_BaseAddress, ULONG bus, ULONG dev, ULONG func, PVOID& Data, size_t DataSize);

		/***************************************************
		*@Function					GetPCIVendorName
		*@brief						获取Pci厂商名
		*@author					王硕
		*@param
			*@VenderID				厂商ID号
			*@VenderName			输出厂商名
		*@return
			*null
		****************************************************/
		BOOL GetPCIVendorName(const USHORT VenderID, std::string & VenderName);

		/***************************************************
		*@Function					GetPCIDeviceName
		*@brief						获取Pci厂商名&设备名
		*@author					王硕
		*@param
			*@VenderID				厂商ID号
			*@DeviceID				设备ID号
			*@VenderName			输出厂商名
			*@DeviceName			输出设备名
		*@return
			*null
		****************************************************/
		BOOL GetPCIDeviceName(const USHORT VenderID, const USHORT DeviceID, std::string& VenderName, std::string& DeviceName);

		/***************************************************
		*@Function					ExecPCIClassCode
		*@brief						解析Pci设备的classcode
		*@author					王硕
		*@param
			*@data					256Bytes的pci配置空间大小
			*@DeviceType			输出解析后的class code代表值
		*@return
			*null
		****************************************************/
		BOOL ExecPCIClassCode(const PCI_COMMON_CONFIG &data, std::string& DeviceType);

		/***************************************************
		*@Function					GetPciSpeed
		*@brief						获取Pci设备速度(Gen123 x16-2)
		*@author					王硕
		*@param
			*@data					pci设备配置空间信息
			*@MAXSpeed				最大运行速度
			*@CurrentSpeed			当前运行速度
		*@return
			*null
		****************************************************/
		BOOL GetPciSpeed(const PCI_COMMON_CONFIG &data, PCISPEED& MAXSpeed, PCISPEED& CurrentSpeed);

		/***************************************************
		*@Function					PCIstringToBDF
		*@brief						pci字符转换成bus dev func
		*@author					王硕
		*@param
			*@pcistring				输入的pci字符串
			*@bus					返回bus号
			*@dev					返回dev号
			*@func					返回func号
		*@return
			*@BOOL					是否成功
		****************************************************/
		const BOOL PCIstringToBDF(const std::wstring& pcistring, ULONG &bus, ULONG &dev, ULONG &func);

		/***************************************************
		*@Function					GetPCIOptionROM
		*@brief						获取PCI Option ROM的信息
		*@author					王硕
		*@param
		*@return
			*@						Option ROM的信息
		****************************************************/
		const std::vector<std::pair<PCI_OPTION_ROM_HEADER, PCI_OPTION_ROM_Data_Structure>>& GetPCIOptionROM();

		/***************************************************
		*@Function					UpdateData
		*@brief						更新PCI Option ROM的信息
		*@author					王硕
		*@param
		*@return
		****************************************************/
		void UpdateData();
	}
}
