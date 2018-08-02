#include "EDIDdefination.h"

/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取EDID的信息
**************************************************/

namespace SV_ASSIST
{
	namespace Display
	{
		/**************************************************************
		*@Function				GetEDID
		*@brief					获得EDID的值
		*@author				王硕
		*@param
		*@return
			*@EDID				EDID未解析的值
		****************************************************************/
		const std::vector<EDID>& GetEDID();

		/**************************************************************
		*@Function				Exec
		*@brief					执行获取EDID值
		*@author				王硕
		*@param
			*@null
		*@return
			*@null	
		*@Other					
								生成文件\\HardWare\\EDID\\edid%d.txt	
		****************************************************************/
		void Exec();

		/**************************************************************
		*@Function				UpdateData
		*@brief					更新EDID值
		*@author				王硕
		*@param
			*@null
		*@return
			*@null				更新EDID 调用Exec前需调用本函数
		****************************************************************/

		void UpdateData();
	}
}



