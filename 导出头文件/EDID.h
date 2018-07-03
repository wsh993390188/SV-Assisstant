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
			*@Num				第几个EDID，由GetEDIDNum确定总数
		*@return
			*@EDID				EDID未解析的值
		****************************************************************/
		const EDID& GetEDID(const UINT Num);

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
		*@Function				GetEDIDNum
		*@brief					获取EDID数量
		*@author				王硕
		*@param
			*@null
		*@return
			*@DWORD				EDID个数
		****************************************************************/

		const DWORD GetEDIDNum();

		/**************************************************************
		*@Function				UpdateData
		*@brief					更新EDID值
		*@author				王硕
		*@param
			*@null
		*@return
			*@null				生成文件
		****************************************************************/

		void UpdateData();
	}
}



