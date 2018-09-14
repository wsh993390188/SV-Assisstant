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
		*@Function				GetMonitorName
		*@brief					获得Mointor名称
		*@author				王硕
		*@param
			nums				输入第几个显示器,由GetEDID确定最大显示器个数
		*@return
			*@string			Mointor的名称(制造商+型号)
		****************************************************************/
		const std::string GetMonitorName(int nums = 0);

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
		*@Function				outputfile
		*@brief					将EDID解析后信息输出到文件中
		*@author				王硕
		*@param
			*@null
		*@return
			*@null	
		*@Other					
								生成文件\\HardWare\\EDID\\edid%d.txt	
		****************************************************************/
		void outputfile();
	}
}


