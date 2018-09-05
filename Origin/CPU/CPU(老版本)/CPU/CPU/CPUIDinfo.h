/**

*Copyright (C), 2017-.

*@file CPUIDinfo.h，存储格式：UNICODE

*@brief 获取CPUID信息

*@author 王硕

*@version 1.0

*@date 2017.9.21

*@History: 修改历史记录列表，每行修改记录应包括修改日期、修改者及修改内容简述

*<p>时间 版本 作者 说明</p>

*/
#pragma once

#include "definition.h"
#include<sstream>
/**

*@class Get_cpuid 所在文件：CPUIDinfo.h "~/CPUIDinfo.h"

*@author 王硕

*@brief 获取CPUID值

*/

class Get_cpuid
{


protected:
/**

*@brief 获取CPUID的值

*@param

*@param[OUT] buffer				EAX、EBX、ECX、EDX中的值

*@param[IN] function_id			主叶值，存放到EAX中，默认为0

*@param[IN] subfunction_id		子叶值，存放到ECX中，默认为0

*@return

*@GET_CPUID_SUCCESS		获取CPUID值成功

*@ILLEGAL_OPERAND		输入了非法的操作数

*@FAILED_TO_GET_INFO	获取CPUID值失败

*/
	ERROR_CODE Read_CPUID(OUT int buffer[4], IN size_t function_id = 0, IN size_t subfunction_id = 0);


public:
	Get_cpuid();
	virtual ~Get_cpuid();
/**

*@brief 判断CPU的品牌

*@param 无

*@return

*@Intel 是因特尔的CPU

*@AMD 是AMD的CPU

*@ZhaoXin 是兆芯的CPU

*@Others 是其他品牌的的CPU，三大厂商之外

*@FAILED 获取CPUID的品牌失败

*/
	CPU_Brands Judge_CPU_Brands();
/**

*@brief				更新CPUID的值

*@author			王硕

*@param				无

*@return			无

*/
	virtual void UPdate() = 0;
/**

*@brief				返回解析后CPUID的值

*@author			王硕

*@param				无

*@return			无

*/
	virtual CPUID_Data Return_info() const = 0;

/**

*@brief				返回原始后CPUID的值

*@author			王硕

*@param				无

*@return			无

*/
	virtual CPU_RES Return_CPUID() const = 0;
protected:
/**

@brief 保存CPUID的临时结构

*/
	int buffer[4];
private:
	
};


