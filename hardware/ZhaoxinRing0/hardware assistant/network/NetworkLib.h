#pragma once
/*************************************************
Copyright: 2017 Zhaoxin BJ-SV
Author:王硕
Date:2017-12-13
Description: 获取Net Card的信息
**************************************************/
#include <vector>
#include <string>


namespace SV_ASSIST
{
	namespace Net
	{
		struct DHCPStruct 
		{
			bool IsEnable;
			std::wstring DHCPService;
			std::wstring LeaseObtained;
			std::wstring LeaseExpires;
		};

		struct WinsStruct
		{
			bool IsEnable;
			std::wstring PrimaryWinsServer;
			std::wstring SecondaryWinsServer;
		};

		struct NetStruct
		{
			std::wstring DeviceName;
			std::wstring state;
			std::wstring MediaType;
			std::wstring Name;
			std::wstring MACAddrss;
			std::wstring NetWorkType;
			std::wstring IPAddress;
			std::wstring IPMask;
			std::wstring GateWay;
			DHCPStruct	DHCP;
			WinsStruct	Wins;
		};
		/**************************************************************
		*@Function				Update
		*@brief					更新网卡信息
		*@author				王硕
		*@param
			*@null
		*@return
			*@null
		****************************************************************/
		void Update();
		/**************************************************************
		*@Function				GetData
		*@brief					获取网卡信息
		*@author				王硕
		*@param
			*@null
		*@return
			*@std::vector<std::wstring>	网卡名		
		****************************************************************/
		const std::vector<NetStruct>& GetData();
	}
}
