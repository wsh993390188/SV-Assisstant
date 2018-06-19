#pragma once
/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取USB的信息
**************************************************/
namespace SV_ASSIST
{
	namespace USB
	{
		/**************************************************************
		*@Function				GetUsbInfo
		*@brief					获取Usb设备树的信息
		*@author				王硕
		*@param
			*@null
		*@return
			*@vector<USB_ROOT_HUB>		Usb设备树信息		
		****************************************************************/
		const vector<USB_ROOT_HUB>& GetUsbInfo();

		/**************************************************************
		*@Function				UpdateData
		*@brief					更新Usb信息
		*@author				王硕
		*@param
			*@null
		*@return
			*@null		
		****************************************************************/
		void UpdateData();
	}
}

