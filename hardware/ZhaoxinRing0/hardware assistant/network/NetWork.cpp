#include "stdafx.h"
#include <iphlpapi.h>
#include <algorithm>
#include "NetWork.h"

namespace SV_ASSIST
{
	namespace Net
	{
		NetWork::NetWork() : NNproperties(nullptr), hr(E_FAIL), PhysicalDev{}
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

		void NetWork::GetPhysicalDevice()
		{
			hr = CoInitialize(NULL);
			hr = CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&pManager);
			if (pManager == NULL)
			{
				OutputDebugString(L"产生异常，创建失败");
				return;
			}
			pManager->EnumConnections(NCME_DEFAULT, &pEnum);
			while (pEnum->Next(1, &pConnection, &celt) == S_OK)
			{
				pConnection->GetProperties(&NNproperties);
				auto s = GetDeviceState(NNproperties->Status);
				auto m = GetDeviceMediaType(NNproperties->MediaType);
				PhysicalDev.emplace_back(*NNproperties);
				pConnection = nullptr;
			}
			pEnum = nullptr;
			pConnection = nullptr;
			pManager = nullptr;
			CoUninitialize();
		}

		bool NetWork::GetCurrentDev()
		{
			this->GetPhysicalDevice();

			std::unique_ptr<char[]> pAdapterInfo = nullptr;
			PIP_ADAPTER_INFO  pAdapter = nullptr;
			DWORD dwRetVal = 0;

			/* variables used to print DHCP time info */
			struct tm newtime;
			char buffer[32] = {};

			ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
			pAdapterInfo = std::make_unique<char[]>(ulOutBufLen);
			if (pAdapterInfo == NULL) {
				OutputDebugPrintf(L"Error allocating memory needed to call GetAdaptersinfo\n");
				return false;
			}
			// Make an initial call to GetAdaptersInfo to get
			// the necessary size into the ulOutBufLen variable
			if (GetAdaptersInfo(reinterpret_cast<PIP_ADAPTER_INFO>(pAdapterInfo.get()), &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
				pAdapterInfo.reset(new char[ulOutBufLen] {});
				if (pAdapterInfo == nullptr) {
					OutputDebugPrintf(L"Error allocating memory needed to call GetAdaptersinfo\n");
					return false;
				}
			}
			if ((dwRetVal = GetAdaptersInfo(reinterpret_cast<PIP_ADAPTER_INFO>(pAdapterInfo.get()), &ulOutBufLen)) == NO_ERROR)
			{
				pAdapter = reinterpret_cast<PIP_ADAPTER_INFO>(pAdapterInfo.get());
				while (pAdapter) {
					auto results = std::find_if(PhysicalDev.begin(), PhysicalDev.end(), [&](const NETCON_PROPERTIES& a1) -> bool
					{
						GUID temp = {};
						const size_t cSize = strlen(pAdapter->AdapterName) + 1;
						std::unique_ptr<wchar_t[]> wc = std::make_unique<wchar_t[]>(cSize);
						swprintf(wc.get(), cSize, L"%hs", pAdapter->AdapterName);
						auto results = CLSIDFromString(wc.get(), &temp);
						if (results == NOERROR)
							return temp == a1.guidId;
						else
							return false;
					});
					if (results != PhysicalDev.end())
					{
						NetStruct temp = {results->pszwDeviceName,this->GetDeviceState(results->Status), this->GetDeviceMediaType(results->MediaType), results->pszwName};
						switch (pAdapter->Type) {
						case MIB_IF_TYPE_OTHER:
							temp.NetWorkType = _T("Other");
							break;
						case MIB_IF_TYPE_ETHERNET:
							temp.NetWorkType = _T("Ethernet");
							break;
						case MIB_IF_TYPE_TOKENRING:
							temp.NetWorkType = _T("Token Ring");
							break;
						case MIB_IF_TYPE_FDDI:
							temp.NetWorkType = _T("FDDI");
							break;
						case MIB_IF_TYPE_PPP:
							temp.NetWorkType = _T("PPP");
							break;
						case MIB_IF_TYPE_LOOPBACK:
							temp.NetWorkType = _T("Lookback");
							break;
						case MIB_IF_TYPE_SLIP:
							temp.NetWorkType = _T("Slip");
							break;
						case IF_TYPE_IEEE80211:
							temp.NetWorkType = _T("IEEE 802.11");
							break;
						default:
							temp.NetWorkType = _T("Unknown type") + std::to_wstring(pAdapter->Type);
							break;
						}
						if (pAdapter->AddressLength == 6)
						{
							temp.MACAddrss = boost::str(boost::wformat(L"%02X-%02X-%02X-%02X-%02X-%02X") % pAdapter->Address[0] % pAdapter->Address[1] % pAdapter->Address[2] % pAdapter->Address[3] % pAdapter->Address[4] % pAdapter->Address[5]);
						}
						temp.IPAddress = this->CharToWchar(pAdapter->IpAddressList.IpAddress.String, 4*4);
						temp.IPMask = this->CharToWchar(pAdapter->IpAddressList.IpMask.String, 4*4);
						temp.GateWay = this->CharToWchar(pAdapter->GatewayList.IpAddress.String, 4*4);
						DHCPStruct dhcp = { false,{}, {}, {} };
						WinsStruct wins = { false,{},{} };
						if (pAdapter->DhcpEnabled)
						{
							dhcp.IsEnable = true;
							dhcp.DHCPService = this->CharToWchar(pAdapter->DhcpServer.IpAddress.String, 4*4);
							if (!_localtime32_s(&newtime, (__time32_t*)&pAdapter->LeaseObtained) && !asctime_s(buffer, 32, &newtime))
							{
								dhcp.LeaseObtained = this->CharToWchar(buffer, 32);
							}
							if (!_localtime32_s(&newtime, (__time32_t*)&pAdapter->LeaseExpires) && !asctime_s(buffer, 32, &newtime))
							{
								dhcp.LeaseExpires = this->CharToWchar(buffer, 32);
							}
						}

						if (pAdapter->HaveWins)
						{
							wins.PrimaryWinsServer = this->CharToWchar(pAdapter->PrimaryWinsServer.IpAddress.String, 4 * 4);
							wins.SecondaryWinsServer = this->CharToWchar(pAdapter->SecondaryWinsServer.IpAddress.String, 4 * 4);
						}

						temp.DHCP = dhcp;
						temp.Wins = wins;
						this->NetInfo.emplace_back(temp);
					}
					pAdapter = pAdapter->Next;
				}
			}
			else {
				printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
				return false;
			}
			return true;
		}

		std::wstring NetWork::CharToWchar(const char * c, size_t m_encode)
		{
			const size_t cSize = strlen(c) + 1;
			wchar_t* wc = new wchar_t[cSize];
			::ZeroMemory(wc, cSize);
			swprintf(wc, cSize, L"%hs", c);
			std::wstring des = wc;
			delete[] wc;
			wc = nullptr;
			return des;
		}

		void NetWork::Exec()
		{
			this->GetCurrentDev();
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
			this->PhysicalDev.clear();
			this->Exec();
		}

		const std::vector<NetStruct> &NetWork::GetData()
		{
			return this->NetInfo;
		}
	}
}
