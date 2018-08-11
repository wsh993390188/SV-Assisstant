#pragma once
/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取GPU的信息
**************************************************/
namespace SV_ASSIST
{
	namespace GPU
	{
		enum FanTypes
		{
			Percent = 0,
			RPM = 1
		};

		/**************************************************************
		*@Function				GPUName
		*@brief					获取显卡的名字
		*@author				王硕
		*@param
		*@null
		*@return
		*@string			显卡名字
		****************************************************************/
		const std::string GPUName();
		/**************************************************************
		*@Function				GPUBIOSVersion
		*@brief					获取显卡BIOS信息
		*@author				王硕
		*@param
		*@null
		*@return
		*@string			显卡BIOS信息
		****************************************************************/
		const std::string GPUBIOSVersion();

		/**************************************************************
		*@Function				GPUtemperature
		*@brief					获取显卡温度
		*@author				王硕
		*@param
		*@null
		*@return
		*@double			显卡温度
		****************************************************************/
		double GPUtemperature();
		/**************************************************************
		*@Function				GPUFans
		*@brief					获取显卡风扇转速
		*@author				王硕
		*@param
		*@FanTypes			IN_OUT	AMD：输入RPMorPer 判断输出类型决定返回值类型，Nvidia：此值无效，返回风扇的RPM
		*@return
		*@int				风扇转速
		****************************************************************/
		int GPUFans(FanTypes & type);

		/**************************************************************
		*@Function				GPUCurrentClock
		*@brief					获取显卡频率
		*@author				王硕
		*@param
		*@null
		*@return
		*@double			显卡频率 MHz
		****************************************************************/
		double GPUCurrentClock();

		/**************************************************************
		*@Function				GPUMemoryClock
		*@brief					获取显存频率
		*@author				王硕
		*@param
		*@null
		*@return
		*@double			显存频率 MHz
		****************************************************************/
		double GPUMemoryClock();

		/**************************************************************
		*@Function				GPUPhysicalMemory
		*@brief					获取显存大小(N卡独有 A卡待研究)
		*@author				王硕
		*@param
		*@null
		*@return
		*@double			显存大小
		****************************************************************/
		double GPUPhysicalMemory();

		/**************************************************************
		*@Function				GPUVirtualMemory
		*@brief					获取虚拟显存大小(N卡独有)
		*@author				王硕
		*@param
		*@null
		*@return
		*@double			虚拟显存大小
		****************************************************************/
		double GPUVirtualMemory();

		/**************************************************************
		*@Function				GetGPUAvailableMemory
		*@brief					获取可用显存
		*@author				王硕
		*@param
		*@null
		*@return
		*@double			可用显存大小
		****************************************************************/
		double GetGPUAvailableMemory();

		/**************************************************************
		*@Function				GetGPUUsasge
		*@brief					获取显卡使用率
		*@author				王硕
		*@param
		*@null
		*@return
		*@int				显卡使用率(%)
		****************************************************************/
		int GetGPUUsasge();
		/**************************************************************
		*@Function				GetGPUMemoryControlUsasge
		*@brief					获取显存使用率(N卡独有)
		*@author				王硕
		*@param
		*@null
		*@return
		*@int				显存使用率
		****************************************************************/
		int GetGPUMemoryControlUsasge();
		/**************************************************************
		*@Function				GetGPUVideoEngineUsasge
		*@brief					获取显卡Frame Buffer使用率(N卡独有)
		*@author				王硕
		*@param
		*@null
		*@return
		*@int				使用率
		****************************************************************/
		int GetGPUVideoEngineUsasge();
		/**************************************************************
		*@Function				UpdateDate
		*@brief					更新显卡信息
		*@author				王硕
		*@param
		*@null
		*@return
		*@null
		****************************************************************/
		void UpdateDate();
	}
}



