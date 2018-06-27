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
		*@brief					???EDID?????
		*@author				???
		*@param
			*@Num				????????????EDID
		*@return
			*@EDID				EDID?????
		****************************************************************/
		const EDID& GetEDID(const UINT Num);

		/**************************************************************
		*@Function				Exec
		*@brief					???EDID???
		*@author				???
		*@param
			*@null
		*@return
			*@null	
		*@Other					
								??????????????浽?????	
		****************************************************************/
		
		void Exec();

		/**************************************************************
		*@Function				GetEDIDNum
		*@brief					??????????????
		*@author				???
		*@param
			*@null
		*@return
			*@DWORD				?????????
		****************************************************************/

		const DWORD GetEDIDNum();
	}
}



