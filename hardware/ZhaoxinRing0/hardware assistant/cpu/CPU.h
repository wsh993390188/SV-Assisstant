#pragma once

/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取CPU的信息
**************************************************/

#include "defination.h"

namespace SV_ASSIST
{
	namespace CPU
	{
		/**************************************************************
		*@Function				Updatedata
		*@brief					更新CPU信息
		*@author				王硕
		*@param
		*@return
			*@null			
		****************************************************************/
		void Updatedata();
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
		*@Function				GetCPUSpecification
		*@brief					获取CPU的名字(Brand Name)
		*@author				王硕
		*@param
		*@
		*@return
			*@string			CPU名字
		****************************************************************/
		const std::string& GetCPUSpecification();
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

		/**************************************************************
		*@Function				GetCurrentClockSpeed
		*@brief					获取处理器最近的频率 
		*@author				王硕
		*@param
			*@null
		*@return
			*@vector<double>&	处理器频率数组				
		****************************************************************/
		const std::vector<double>& GetCurrentClockSpeed();

		//可能会废弃，改用MSR形式
		/**************************************************************
		*@Function				GetExtClock
		*@brief					获取处理器的Bus Speed 
		*@author				王硕
		*@param
			*@null
		*@return
			*@double			处理器外频				
		****************************************************************/
		const double GetExtClock();

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
			*@null
		*@return
			*@Cache_info		一、二、三级Cache(若没3级需要自己判断)		
		****************************************************************/
		const Cache_info* GetCache();

		/**************************************************************
		*@Function				GetSouthbridgeName
		*@brief					获取南桥的代号
		*@author				王硕
		*@param
			*@null
		*@return
			*@string			Intel未知代号返回Unknown，其他未知代号返回空
		****************************************************************/
		const std::string& GetSouthbridgeName();

		//以下仅支持Inte部分cpu
		/**************************************************************
		*@Function				GetCodeName
		*@brief					获取处理器的代号
		*@author				王硕
		*@param
			*@null
		*@return
			*@string				Intel未知代号返回Unknown，其他未知代号返回空
		****************************************************************/
		const std::string& GetCodeName();

		/**************************************************************
		*@Function				GetTechnology
		*@brief					获取处理器的工艺
		*@author				王硕
		*@param
			*@null
		*@return
			*@UINT				未知返回0
		****************************************************************/
		const unsigned int GetTechnology();

		/**************************************************************
		*@Function				GetMaxTDP
		*@brief					获取处理器的最大功率
		*@author				王硕
		*@param
		*@null
		*@return
			*@UINT				未知返回0
		****************************************************************/
		const unsigned int GetMaxTDP();

		/**************************************************************
		*@Function				GetPackageTemperature
		*@brief					获取处理器的封装温度
		*@author				王硕
		*@param
			*@null
		*@return
			*@double			未知返回INFINITY
		****************************************************************/
		const double GetPackageTemperature();

		/**************************************************************
		*@Function				GetTemperature
		*@brief					获取处理器的每个核的温度
		*@author				王硕
		*@param
			*@null
		*@return
			*@double			未知返回INFINITY
		****************************************************************/
		const std::vector<double>& GetTemperature();

		/**************************************************************
		*@Function				GetVoltage
		*@brief					获取处理器的每个核的电压
		*@author				王硕
		*@param
			*@null
		*@return
			*@double			未知返回INFINITY
		****************************************************************/
		const std::vector<double>& GetVoltage();


		void GetCpuid(int(&cpuid)[4], const int eax, const int ecx);
	}
}
