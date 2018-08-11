#pragma once
/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取DMI的信息
**************************************************/
#include "smbiosdefination.h "
namespace SV_ASSIST
{
	namespace DMI_SMBIOS
	{
		//也可以导出map树

		/**************************************************************
		*@Function				OutputFile
		*@brief					Smbios信息输出到文件中
		*@author				王硕
		*@param
			*@null
		*@return
			*@null
		*@Other					存储为文件，文件中大类[],类中信息无特殊标示
		****************************************************************/
		void OutputFile();

		/**************************************************************
		*@Function				GetSMBIOSinfo
		*@brief					获取Smbios信息，不需要更新信息
		*@author				王硕
		*@param
			*@null
		*@return
			*@SMBIOS_Struct		包含SMBIOS信息的结构
		****************************************************************/
		const SMBIOS_Struct& GetSMBIOSinfo();
	}
}
