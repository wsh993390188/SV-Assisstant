#pragma once
/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取DMI的信息
**************************************************/
namespace SV_ASSIST
{
	namespace DMI_SMBIOS
	{
		//也可以导出map树

		/**************************************************************
		*@Function				Updatesmbios
		*@brief					更新Smbios信息
		*@author				王硕
		*@param
			*@null
		*@return
			*@null		
		*@Other					存储为文件，文件中大类[],类中信息无特殊标示
		****************************************************************/
		void Updatesmbios();
	}
}
