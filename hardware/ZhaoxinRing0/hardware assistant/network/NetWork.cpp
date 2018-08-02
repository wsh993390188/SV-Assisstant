#include "stdafx.h"
#include "NetWork.h"

namespace SV_ASSIST
{
	namespace Net
	{
		NetWork::NetWork() : NNproperties(nullptr), hr(E_FAIL)
		{
#ifdef ZX_OutputLog
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "***** Net info *****");
#endif
			this->Exec();
#ifdef ZX_OutputLog
			if (SUCCEEDED(hr))
			{
				Logger::Instance()->OutputLogInfo(el::Level::Debug, "Initialize Net COM Success");
				Logger::Instance()->OutputLogInfo(el::Level::Debug, "Enuming Net Device");
				DWORD count = 0;
				for (auto const i : NetInfo)
				{
					auto ft = boost::wformat(L"Device #%d: %s") % ++count %i.DeviceName.c_str();
					Logger::Instance()->OutputLogInfo(el::Level::Debug, ft.str());
				}
			}
			else
			{
				Logger::Instance()->OutputLogInfo(el::Level::Debug, "Initialize Net COM Failed");
				std::string r = boost::str(boost::format("Failed Code: 0x%lX") % hr);
				Logger::Instance()->OutputLogInfo(el::Level::Debug, r);
			}

			Logger::Instance()->OutputLogInfo(el::Level::Debug, "***** End Net info *****\n");
#endif
		} 


		NetWork::~NetWork()
		{
			pEnum = nullptr;
			pConnection = nullptr;
			pManager = nullptr;
			CoUninitialize();
		}

		std::wstring NetWork::GetDeviceState(IN DWORD state) 
		{
			switch (state)
			{
			case NCS_DISCONNECTED:	return L"DISCONNECTED";
			case NCS_CONNECTING:	return L"CONNECTING";
			case NCS_CONNECTED:	return L"CONNECTED";
			case NCS_DISCONNECTING:	return L"DISCONNECTING";
			case NCS_HARDWARE_NOT_PRESENT:	return L"HARDWARE_NOT_PRESENT";
			case NCS_HARDWARE_DISABLED:	return L"HARDWARE_DISABLED";
			case NCS_MEDIA_DISCONNECTED:	return L"MEDIA_DISCONNECTED";
			case NCS_AUTHENTICATION_SUCCEEDED:	return L"AUTHENTICATION_SUCCEEDED";
			case NCS_AUTHENTICATING:	return L"AUTHENTICATING";
			case NCS_AUTHENTICATION_FAILED:	return L"AUTHENTICATION_FAILED";
			case NCS_INVALID_ADDRESS:	return L"INVALID_ADDRESS";
			case NCS_CREDENTIALS_REQUIRED:	return L"CREDENTIALS_REQUIRED";
			case NCS_ACTION_REQUIRED_RETRY:	return L"ACTION_REQUIRED_RETRY";
			case NCS_ACTION_REQUIRED:	return L"ACTION_REQUIRED";
			case NCS_CONNECT_FAILED:	return L"CONNECT_FAILED";
			case NCS_HARDWARE_MALFUNCTION:	return L"HARDWARE_MALFUNCTION";
			}
			return L"UNKNOWN";
		}

		std::wstring NetWork::GetDeviceMediaType(IN DWORD state)
		{
			switch (state)
			{
			case NCM_NONE:	return L"NONE";
			case NCM_DIRECT:	return L"DIRECT";
			case NCM_ISDN:	return L"ISDN";
			case NCM_LAN:	return L"LAN";
			case NCM_PHONE:	return L"PHONE";
			case NCM_TUNNEL:	return L"TUNNEL";
			case NCM_PPPOE:	return L"PPPOE";
			case NCM_BRIDGE:	return L"BRIDGE";
			case NCM_SHAREDACCESSHOST_LAN:	return L"SHAREDACCESSHOST_LAN";
			case NCM_SHAREDACCESSHOST_RAS:	return L"SHAREDACCESSHOST_RAS";
			}
			return L"UNKNOWN";
		}

		void NetWork::Exec()
		{
			hr = CoInitialize(NULL);
			hr = CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&pManager);
			if (pManager == NULL)
			{
				OutputDebugString(L"产生异常，创建失败");
				return ;
			}
			pManager->EnumConnections(NCME_DEFAULT, &pEnum);
			while (pEnum->Next(1, &pConnection, &celt) == S_OK)
			{
				pConnection->GetProperties(&NNproperties);
				auto s = GetDeviceState(NNproperties->Status);
				auto m = GetDeviceMediaType(NNproperties->MediaType);
				NetStruct temp = { NNproperties->pszwDeviceName, s, m, NNproperties->pszwName };
				NetInfo.emplace_back(temp);
				pConnection = nullptr;
			}
			pEnum = nullptr;
			pConnection = nullptr;
			pManager = nullptr;
			CoUninitialize();
		}

		BOOL NetWork::EnumWlan()
		{
			// Declare and initialize variables.
			HANDLE hClient = NULL;
			DWORD dwMaxClient = 2;   //
			DWORD dwCurVersion = 0;
			DWORD dwResult = 0;
			int iRet = 0;
			WCHAR GuidString[40] = { 0 };
			int i;
			/* variables used for WlanEnumInterfaces  */
			PWLAN_INTERFACE_INFO_LIST pIfList = nullptr;
			PWLAN_INTERFACE_INFO pIfInfo = nullptr;
			dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
			if (dwResult != ERROR_SUCCESS) {
				std::wcerr << L"WlanOpenHandle failed with error:" << dwResult << std::endl;
				// FormatMessage can be used to find out why the function failed
				return FALSE;
			}
			dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
			if (dwResult != ERROR_SUCCESS) {
				std::wcerr << L"WlanEnumInterfaces failed with error:" << dwResult << std::endl;
				// FormatMessage can be used to find out why the function failed
				if (pIfList != NULL) {
					WlanFreeMemory(pIfList);
					pIfList = NULL;
				}
				return FALSE;
			}
			else {
				for (i = 0; i < (int)pIfList->dwNumberOfItems; i++) {

					pIfInfo = (WLAN_INTERFACE_INFO *)&pIfList->InterfaceInfo[i];
					wprintf(L"  Interface Description[%d]: %ws", i,
						pIfInfo->strInterfaceDescription);
					std::wstring s, m = L"Wlan";
					switch (pIfInfo->isState) {
					case wlan_interface_state_not_ready:
						s = L"NotReady";
						break;
					case wlan_interface_state_connected:
						s = L"Connected";
						break;
					case wlan_interface_state_ad_hoc_network_formed:
						s = L"ad hoc network";
						break;
					case wlan_interface_state_disconnecting:
						s = L"Disconnecting";
						wprintf(L"Disconnecting\n");
						break;
					case wlan_interface_state_disconnected:
						s = L"Not connected";
						break;
					case wlan_interface_state_associating:
						s = L"Attempting to associate with a network";
						break;
					case wlan_interface_state_discovering:
						s = L"Discovering settings";
						break;
					case wlan_interface_state_authenticating:
						s = L"Authenticating";
						break;
					default:
						s = L"Unknown";
						break;
					}
					NetStruct temp = { pIfInfo->strInterfaceDescription, s, m, pIfInfo->strInterfaceDescription };
				}
			}
			if (pIfList != NULL) {
				WlanFreeMemory(pIfList);
				pIfList = NULL;
			}
			return TRUE;
		}
		void NetWork::Update()
		{
			pEnum = nullptr;
			pConnection = nullptr;
			pManager = nullptr;
			this->NetInfo.clear();
			this->Exec();
		}

		const std::vector<NetStruct> &NetWork::GetData()
		{
			return this->NetInfo;
		}
	}
}
