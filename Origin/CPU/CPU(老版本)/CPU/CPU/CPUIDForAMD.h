/**

*Copyright (C), 2017-.

*@file CPUID For AMD.h，存储格式：UNICODE

*@brief 获取AMD的CPUID信息

*@author 王硕

*@version 1.0

*@date 2017.9.22

*@History: 修改历史记录列表，每行修改记录应包括修改日期、修改者及修改内容简述

*<p>时间 版本 作者 说明</p>

*/
#pragma once
#include "CPUIDinfo.h"
/**

*@class Get_Info_For_AMD 所在文件CPUID For AMD.h "~/CPUID For AMD.h"

*@author 王硕

*@brief 获取AMD的信息

*<br>

*对CPUID用AMD的手册进行解析。

*/
class Get_Info_For_AMD : virtual public Get_cpuid, private UnCopyable
{
public:
	Get_Info_For_AMD();
	~Get_Info_For_AMD();
	/**

	*@brief 返回解析后的信息

	*@author 王硕

	*@param 无

	*@return 无

	*/
	CPUID_Data Return_info() const;

	/**

	*@brief 返回原始后的信息

	*@author 王硕

	*@param 无

	*@return 无

	*/
	CPU_RES Return_CPUID() const;

	/**

	*@brief				更新CPUID的值 继承于Get_cpuid类中

	*@author			王硕

	*@param				无

	*@return			无

	*/
	void UPdate();

private:

	/**

	*@brief 解析AMD CPUID = 1的函数

	*@author 王硕

	*@param 无

	*@return

	*@TRUE 获取完毕

	*@FALSE 获取失败

	*/
	bool AMD_Get_Standard_Feature(int ECX, int EDX);

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

	*@brief 解析AMD CPUID = 0x80000001的函数

	*@author 王硕

	*@param 无

	*@return

	*@TRUE 获取完毕

	*@FALSE 获取失败

	*/
	bool AMD_Get_Extended_Feature(int ECX, int EDX);
	int find_location(vector<wstring> vecIntegerArray, wstring search_string);
	/**

	*@brief 解析AMD CPUID的函数

	*@author 王硕

	*@param 无

	*@return

	*@TRUE 获取完毕

	*@FALSE 获取失败

	*/
	bool excute_CPUID_FOR_AMD();
private:
	/**

	@brief 保存解析后的CPUID信息结构

	*/
	CPUID_Data AMD_Data;
	/**

	@brief 保存CPUID返回值的结构

	*/

	CPU_RES AMD_cpuRegister;
};