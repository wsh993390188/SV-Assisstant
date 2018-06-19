#include <vld.h>
#include "net ipconfig.h"
BOOL GetAdaptersInfo_WinApi(vector<MAPSTRING >& ipconfig)
{
	ULONG Buflen = sizeof(IP_ADAPTER_INFO);
	MAPSTRING ipconfigtmp;
	int ret = 0;
	PIP_ADAPTER_INFO ip_info = (PIP_ADAPTER_INFO)HeapAlloc(GetProcessHeap(), 0, sizeof(IP_ADAPTER_INFO));
	PIP_ADAPTER_INFO pAdapter = NULL;
	if (ip_info == NULL)
	{
		return FALSE;
	}
	ret = GetAdaptersInfo(ip_info, &Buflen);
	if (ret == ERROR_BUFFER_OVERFLOW)
	{
		HeapFree(GetProcessHeap(), 0, ip_info);
		ip_info = (PIP_ADAPTER_INFO)HeapAlloc(GetProcessHeap(), 0, Buflen);
		if (ip_info == NULL)
		{
			return FALSE;
		}
	}
	ret = GetAdaptersInfo(ip_info, &Buflen);
	if (ret == NO_ERROR)
	{
		PCHAR strbuf = new char[512];
		struct tm newtime;
		char buffer[32];
		errno_t error;
		//对数据进行操作
		pAdapter = ip_info;
		while (pAdapter) {
			sprintf_s(strbuf, 512,"%d", pAdapter->ComboIndex);
			ipconfigtmp.insert(MAPSTRING::value_type(_T("ComboIndex"), strbuf));
			sprintf_s(strbuf, 512, "%s", pAdapter->AdapterName);
			ipconfigtmp.insert(MAPSTRING::value_type(_T("Adapter Name"), strbuf));
			sprintf_s(strbuf, 512, "%s", pAdapter->Description);
			ipconfigtmp.insert(MAPSTRING::value_type(_T("Adapter Desc"), strbuf));
			ZeroMemory(strbuf, 512);
			for (USHORT i = 0; i < pAdapter->AddressLength; i++) {
				if (i == (pAdapter->AddressLength - 1))
					sprintf_s(strbuf + (i * 3), sizeof(strbuf), "%.2X", (int)pAdapter->Address[i]);
				else
					sprintf_s(strbuf + (i * 3), sizeof(strbuf), "%.2X-", (int)pAdapter->Address[i]);
			}
			ipconfigtmp.insert(MAPSTRING::value_type(_T("MAC Addr"), strbuf));
			sprintf_s(strbuf, 512, "%d", pAdapter->Index);
			ipconfigtmp.insert(MAPSTRING::value_type(_T("Index"), strbuf));
			switch (pAdapter->Type) {
			case MIB_IF_TYPE_OTHER:
				sprintf_s(strbuf, 512, "Other");
				break;
			case MIB_IF_TYPE_ETHERNET:
				sprintf_s(strbuf, 512, "Ethernet");
				break;
			case MIB_IF_TYPE_TOKENRING:
				sprintf_s(strbuf, 512, "Token Ring");
				break;
			case MIB_IF_TYPE_FDDI:
				sprintf_s(strbuf, 512, "FDDI");
				break;
			case MIB_IF_TYPE_PPP:
				sprintf_s(strbuf, 512, "PPP");
				break;
			case MIB_IF_TYPE_LOOPBACK:
				sprintf_s(strbuf, 512, "Lookback");
				break;
			case MIB_IF_TYPE_SLIP:
				sprintf_s(strbuf, 512, "Slip");
				break;
			case IF_TYPE_IEEE80211:
				sprintf_s(strbuf, 512, "IEEE 802.11 wireless");
				break;
			default:
				sprintf_s(strbuf, 512, "Unknown type %ld", pAdapter->Type);
				break;
			}
			ipconfigtmp.insert(MAPSTRING::value_type(_T("Type"), strbuf));
			sprintf_s(strbuf, 512, "%s",
				pAdapter->IpAddressList.IpAddress.String);
			ipconfigtmp.insert(MAPSTRING::value_type(_T("IP Addr"), strbuf));
			sprintf_s(strbuf, 512, "%s", pAdapter->IpAddressList.IpMask.String);
			ipconfigtmp.insert(MAPSTRING::value_type(_T("IP Mask"), strbuf));
			sprintf_s(strbuf, 512, "%s", pAdapter->GatewayList.IpAddress.String);
			ipconfigtmp.insert(MAPSTRING::value_type(_T("GateWay"), strbuf));

			if (pAdapter->DhcpEnabled) {
				sprintf_s(strbuf, 512, "Yes");
				ipconfigtmp.insert(MAPSTRING::value_type(_T("DHCP Enable"), strbuf));
				sprintf_s(strbuf, 512, "DHCP Server: %s",
					pAdapter->DhcpServer.IpAddress.String);
				ipconfigtmp.insert(MAPSTRING::value_type(_T("DHCP Server"), strbuf));
				/* Display local time */
				error = _localtime32_s(&newtime, (__time32_t*)&pAdapter->LeaseObtained);
				if (error)
					sprintf_s(strbuf, 512, "Invalid Argument");
				else {
					// Convert to an ASCII representation 
					error = asctime_s(buffer, 32, &newtime);
					if (error)
						sprintf_s(strbuf, 512, "Invalid Argument");
					else
						/* asctime_s returns the string terminated by \n\0 */
						sprintf_s(strbuf, 512, "%s", buffer);
				}
				ipconfigtmp.insert(MAPSTRING::value_type(_T("Lease Obtained"), strbuf));

				error = _localtime32_s(&newtime, (__time32_t*)&pAdapter->LeaseExpires);
				if (error)
					sprintf_s(strbuf, 512, "Invalid Argument");
				else {
					// Convert to an ASCII representation 
					error = asctime_s(buffer, 32, &newtime);
					if (error)
						sprintf_s(strbuf, 512, "Invalid Argument");
					else
						/* asctime_s returns the string terminated by \n\0 */
						sprintf_s(strbuf, 512, "%s", buffer);
				}
				ipconfigtmp.insert(MAPSTRING::value_type(_T("Lease Expires"), strbuf));
			}
			else
				sprintf_s(strbuf, 512, "No");
			ipconfigtmp.insert(MAPSTRING::value_type(_T("DHCP Enable"), strbuf));

			if (pAdapter->HaveWins) {
				sprintf_s(strbuf, 512, "Yes");
				ipconfigtmp.insert(MAPSTRING::value_type(_T("Have Wins"), strbuf));
				sprintf_s(strbuf, 512, "%s",
					pAdapter->PrimaryWinsServer.IpAddress.String);
				ipconfigtmp.insert(MAPSTRING::value_type(_T("Primary Server"), strbuf));
				sprintf_s(strbuf, 512, "%s",
					pAdapter->SecondaryWinsServer.IpAddress.String);
				ipconfigtmp.insert(MAPSTRING::value_type(_T("Secondary Server"), strbuf));
			}
			else
			{
				sprintf_s(strbuf, 512, "No");
				ipconfigtmp.insert(MAPSTRING::value_type(_T("Have Wins"), strbuf));
			}
			ipconfig.emplace_back(ipconfigtmp);
			ipconfigtmp.clear();
			pAdapter = pAdapter->Next;
		}
		if (strbuf)
		{
			delete[] strbuf;
			strbuf = NULL;
		}
	}
	if (ip_info)
	{
		HeapFree(GetProcessHeap(), 0, ip_info);
	}
	if (ipconfig.empty())
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

int main()
{
	vector<MAPSTRING > ipconfig;
	GetAdaptersInfo_WinApi(ipconfig);
	return 0;
}