#pragma once
/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取GPU的信息
**************************************************/
#include "Igpu.h"
namespace SV_ASSIST
{
	namespace GPU
	{
		/**************************************************************
		*@Function				GetGpuInfo
		*@brief					获取显卡信息
		*@author				王硕
		*@param
			*@null
		*@return
			*@IgpuSensor		gpu信息数组
		****************************************************************/
		const std::vector<std::pair<GPUBaseInfo, GPUSensorInfo>>& GetGpuInfo();

		/**************************************************************
		*@Function				UpdateDate
		*@brief					更新显卡信息
		*@author				王硕
		*@param
			*@null
		*@return
			*@null								
		****************************************************************/
		void UpdateDate();
	}
}

