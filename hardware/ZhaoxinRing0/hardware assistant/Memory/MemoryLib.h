#pragma once
#include "memorydefination.h"

namespace SV_ASSIST
{
	namespace Memory
	{

		/***************************************************
		*@Function					GetMemoryDeviceInfo
		*@brief						内存的插槽及类型
		*@author					王硕
		*@param
		*@return
			*@info					pair.first: SmbusBase pair.second.first: DIMM Slot pair.second.second: DIMM Module
		****************************************************/
		const std::vector<std::pair<ULONG, std::pair<ULONG, DIMMType>>>& GetMemoryDeviceInfo();

		/***************************************************
		*@Function					GetMemoryDDR3Info
		*@brief						获取DDR3 SPD的信息
		*@author					王硕
		*@param
		*@return
			*@info					pair.first: DIMM Slot pair.second SPD Infomation
		****************************************************/
		const std::vector<std::pair<ULONG, DDR3_INFO>>&  GetMemoryDDR3SPDInfo();

		/***************************************************
		*@Function					GetMemoryDDR4Info
		*@brief						获取DDR4 SPD的信息
		*@author					王硕
		*@param
		*@return
			*@info					pair.first: DIMM Slot pair.second SPD Memory Map
		****************************************************/
		const std::vector<std::pair<ULONG, DDR4_INFO>>&  GetMemoryDDR4SPDInfo();

		/***************************************************
		*@Function					GetMemoryGlobalInfomation
		*@brief						获取解析后的内存信息
		*@author					王硕
		*@param
		*@return
			*@info					pair.first: DIMM Slot pair.second SPD Infomation
		****************************************************/
		const std::vector<std::pair<ULONG, MemoryData>>& GetMemoryGlobalInfomation();

		void UpdateData();
	}
}

