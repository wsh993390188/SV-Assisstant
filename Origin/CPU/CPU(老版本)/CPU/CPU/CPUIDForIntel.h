/**

*Copyright (C), 2017-.

*@file CPUID For Intel.h，存储格式：UNICODE

*@brief 获取Intel的CPUID信息

*@author 王硕

*@version 1.0

*@date 2017.9.21

*@History: 修改历史记录列表，每行修改记录应包括修改日期、修改者及修改内容简述

*<p>时间 版本 作者 说明</p>

*/

#pragma once
#include "CPUIDinfo.h"
/**

*@class Get_Info_For_Intel 所在文件CPUID For Intel.h "~/CPUID For Intel.h"

*@author 王硕

*@brief 获取Intel的信息

*<br>

*对CPUID用Intel的手册进行解析。

*/
class Get_Info_For_Intel : virtual public Get_cpuid, private UnCopyable
{
public:
	Get_Info_For_Intel();
	~Get_Info_For_Intel();
	/**

	*@brief 返回CPUID的信息

	*@author 王硕

	*@param

	*@CPUID_Index		CPUID的索引

	*@EAX				返回EAX的值

	*@EBX				返回EBX的值

	*@ECX				返回ECX的值

	*@EDX				返回EDX的值

	*@return 无

	*/

	CPU_RES Return_CPUID() const;//IN ULONGLONG CPUID_Index, OUT int& EAX, OUT int& EBX, OUT int& ECX, OUT int& EDX

	/**

	*@brief 返回解析后的信息

	*@author 王硕

	*@param 无

	*@return 无

	*/

	CPUID_Data Return_info() const;

	/**

	*@brief				更新CPUID的值 继承于Get_cpuid类中

	*@author			王硕

	*@param				无

	*@return			无

	*/
	void UPdate();

private:

	/**

	*@brief 解析Intel CPUID的函数

	*@author 王硕

	*@param 无

	*@return 无

	*/
	void excute_CPUID_FOR_Intel();
	/**

	*@brief 获取全部的CPUID

	*@author 王硕

	*@param 无

	*@return

	*@TRUE 获取完毕

	*@FALSE 获取失败

	*/
	bool Get_ALL_CPUID();

	/**

	*@brief				查找CPUID具体存放的数组元素下标

	*@author			王硕

	*@param

	*@vecIntegerArray	要查询的字符串组

	*@search_string		要查询的字符串

	*@return

	*@>0				返回的当前寻找的位置信息

	*@-1				获取失败，没有这个信息

	*/
	int find_location(vector<wstring> vecIntegerArray, wstring search_string);

	/**

	*@brief				获取CPUID=1时支持的指令集

	*@author			王硕

	*@param

	*@ECX				要查询ECX的值

	*@EDX				要查询EDX的值

	*@return

	*@TRUE				获取完毕

	*@FALSE				获取失败

	*/
	bool GET_FEATURE_INFO(IN int ECX, IN int EDX);

	/**

	*@brief				获取CPU的架构等信息

	*@author			王硕

	*@param

	*@DF_DM				输入DF_DM以确定CPU的架构

	*@return

	*@TRUE				获取完毕

	*@FALSE				获取失败

	*/
	BOOL DF_DM_info(IN UINT DF_DM);

	/**

	*@brief				获取CPUID=2时Cache的描述符

	*@author			王硕

	*@param

	*@cacheinfo			解析后的Cache8位代码

	*@return

	*@TRUE				获取完毕

	*@FALSE				获取失败

	*/

	void cache_Descriptors(IN unsigned char cacheinfo, vector<wstring>& Cache_Temp);
	/**

	*@brief				获取CPUID=6时支持的指令集

	*@author			王硕

	*@param

	*@ECX				要查询ECX的值

	*@EBX				要查询EBX的值

	*@EAX				要查询EAX的值

	*@return

	*@TRUE				获取完毕

	*@FALSE				获取失败

	*/

	bool GET_Power_Mgmt_INFO(IN int EAX, IN int EBX, IN int ECX);

	/**

	*@brief				获取CPUID=7时支持的扩展指令集

	*@author			王硕

	*@param

	*@ECX				要查询ECX的值

	*@EDX				要查询EDX的值

	*@return

	*@TRUE				获取完毕

	*@FALSE				获取失败

	*/
	bool GET_EXTENED_FEATURE_INFO(IN int ECX, IN int EDX);

	/**

	*@brief				获取CPUID=0A时性能检测的架构

	*@author			王硕

	*@param

	*@EAX				要查询EAX的值

	*@EBX				要查询EBX的值

	*@EDX				要查询EDX的值

	*@return

	*@TRUE				获取完毕

	*@FALSE				获取失败

	*/
	bool GET_ARCHITECTURAL_PERFORMANCE_MONITORING(IN int EAX, IN int EBX, IN int EDX);

	/**

	*@brief				获取CodeName

	*@author			王硕

	*@param

	*@CPUName			输入CodeName的值

	*@return

	*@TRUE				获取完毕

	*@FALSE				获取失败

	*/
	bool GET_CODENAME(const string& CPUName);
	/**

	*@brief				获取Core下的CodeName

	*@author			王硕

	*@param

	*@model				输入CPU的型号

	*@return

	*@string			CPU架构代号

	*/
	string Core_CodeName(const string & model);

	void Information_from_WMI();

	void Information_from_CPUID0();

	void Information_from_CPUID1();

	void Information_from_CPUID2();

	void Information_from_CPUID3();

	void Information_from_CPUID4();

	void Information_from_CPUID5();

	void Information_from_CPUID6();

	void Information_from_CPUID7();

	void Information_from_CPUIDA();

	void Information_from_CPUIDB();

	void Information_from_CPUIDC();

	void Information_from_CPUIDD();

public:
protected:
private:
	/**

	@brief 保存CPUID返回值的结构

	*/

	CPU_RES Intel_cpuRegister;
	/**

	@brief 保存解析后的CPUID信息结构

	*/
	CPUID_Data Intel_Data;

	int mposition;

	enum Microarchitecture
	{
		Unknown = 0,
		Core,
		Xeon,
		Itanium,
	};
};
