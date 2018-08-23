#pragma once
/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取USB的信息
**************************************************/
#include "UsbDefination.h"
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
		const std::vector<USB_ROOT_HUB>& GetAllUsbinfo();

		/**************************************************************
		*@Function				GetAllUsbDeviceInfo
		*@brief					获取Usb设备的信息
		*@author				王硕
		*@param
			*@null
		*@return
			*@vector<DEVICE_INFO_NODE>		Usb设备信息
		****************************************************************/
		const std::vector<DEVICE_INFO_NODE>& GetAllUsbDeviceInfo();

		/**************************************************************
		*@Function				GetAllUsbHubInfo
		*@brief					获取Usb集线器的信息
		*@author				王硕
		*@param
			*@null
		*@return
			*@vector<DEVICE_INFO_NODE>		Usb集线器信息
		****************************************************************/
		const std::vector<DEVICE_INFO_NODE>& GetAllUsbHubInfo();

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

