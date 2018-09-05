#pragma once
#include "cpulibdefination.h"
namespace SV_ASSIST
{
	namespace CPU
	{
		/**************************************************************
		*@Function				GetCPUName
		*@brief					获取CPU的名字(Brand Name)
		*@author				王硕
		*@param
		*@
		*@return
		*@string			CPU名字
		****************************************************************/
		const std::string& GetCPUName();
		/**************************************************************
		*@Function				GetProcessorID
		*@brief					获取处理器的ID
		*@author				王硕
		*@param
		*@
		*@return
		*@string			CPU ID
		****************************************************************/
		const std::string& GetProcessorID();
		/**************************************************************
		*@Function				GetSocketDesignation
		*@brief					获取处理器的封装形式
		*@author				王硕
		*@param
		*@x
		*@return
		*@string
		****************************************************************/
		const std::string& GetSocketDesignation();

		//可能会废弃 改用MSR形式
		/**************************************************************
		*@Function				GetCurrentClockSpeed
		*@brief					获取处理器最近的频率（WMI 可能不准确）
		*@author				王硕
		*@param
		*@null
		*@return
		*@UINT				处理器频率
		****************************************************************/
		const unsigned int GetCurrentClockSpeed();

		//可能会废弃，改用MSR形式
		/**************************************************************
		*@Function				GetExtClock
		*@brief					获取处理器的Bus Speed
		*@author				王硕
		*@param
		*@null
		*@return
		*@uint				处理器外频
		****************************************************************/
		const unsigned int GetExtClock();

		/**************************************************************
		*@Function				GetCore
		*@brief					获取处理器的物理核心数
		*@author				王硕
		*@param
		*@null
		*@return
		*@uint				物理核心数
		****************************************************************/
		const unsigned int GetCore();

		/**************************************************************
		*@Function				GetThread
		*@brief					获取处理器的线程数
		*@author				王硕
		*@param
		*@null
		*@return
		*@uint				处理器线程数
		****************************************************************/
		const unsigned int GetThread();

		//采用MSR形式 未做
		/**************************************************************
		*@Function				GetRevision
		*@brief					获取处理器的修订版本
		*@author				王硕
		*@param
		*@null
		*@return
		*@uint				处理器的修订版本
		****************************************************************/
		const unsigned int GetRevision(); //未做

		//可能会废弃 采用msr形式
		/**************************************************************
		*@Function				GetMaxClockSpeed
		*@brief					获取处理器的最大时钟
		*@author				王硕
		*@param
		*@null
		*@return
		*@uint				最大时钟
		****************************************************************/
		const unsigned int GetMaxClockSpeed();


		/**************************************************************
		*@Function				GetUpgradeMethod
		*@brief					获取处理器的封装形式
		*@author				王硕
		*@param
		*@null
		*@return
		*@uint
		****************************************************************/
		const unsigned int GetUpgradeMethod();

		/**************************************************************
		*@Function				GetManufacturer
		*@brief					获取处理器的厂商ID
		*@author				王硕
		*@param
		*@null
		*@return
		*@string			处理器厂商
		****************************************************************/
		const std::string& GetManufacturer();

		/**************************************************************
		*@Function				GetFamily
		*@brief					获取处理器的Family
		*@author				王硕
		*@param
		*@null
		*@return
		*@short				处理器家族
		****************************************************************/
		const short GetFamily();

		/**************************************************************
		*@Function				GetModel
		*@brief					获取处理器的Model
		*@author				王硕
		*@param
		*@
		*@return
		*@short				Model
		****************************************************************/
		const short GetModel();
		/**************************************************************
		*@Function				GetSocketDesignation
		*@brief					获取处理器的封装形式
		*@author				王硕
		*@param
		*@x					OUT 自己分配好的数组大小空间一般来讲至少要求16字节
		*@return
		*@Null
		****************************************************************/
		const short GetStepping();
		/**************************************************************
		*@Function				GetExtFamily
		*@brief					获取处理器的Ext.Family
		*@author				王硕
		*@param
		*@null
		*@return
		*@short				Ext.Family
		****************************************************************/
		const short GetExtFamily();
		/**************************************************************
		*@Function				GetExtModel
		*@brief					获取处理器的Ext.Model
		*@author				王硕
		*@param
		*@null
		*@return
		*@short				Ext.Model
		****************************************************************/
		const short GetExtModel();
		/**************************************************************
		*@Function				GetFeature
		*@brief					获取处理器的特性
		*@author				王硕
		*@param
		*@null
		*@return
		*@CPUFeature		处理器支持的特性
		****************************************************************/
		const CPUFeature& GetFeature();

		/**************************************************************
		*@Function				GetCache
		*@brief					获取处理器的Cache
		*@author				王硕
		*@param
		*@
		*@return
		*@Cache_info		一、二、三级Cache(若没3级需要自己判断)
		****************************************************************/
		const Cache_info* GetCache();
	}
}

