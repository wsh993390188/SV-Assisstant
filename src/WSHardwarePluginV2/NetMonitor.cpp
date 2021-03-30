#include "stdafx.h"
#include "NetMonitor.h"
#include <wlanapi.h>

Hardware::NIC::NetworkMonitor::NetworkMonitor() : _wlanHandle(nullptr)
{
}

Hardware::NIC::NetworkMonitor::~NetworkMonitor()
{
	if (_wlanHandle)
		WlanCloseHandle(_wlanHandle, nullptr);
}

Hardware::NIC::NetworkMonitor& Hardware::NIC::NetworkMonitor::Instance()
{
	static NetworkMonitor inst;
	return inst;
}

std::string Hardware::NIC::NetworkMonitor::QueryWiFiSignalQuality(const GUID& guid)
{
	return GetWiFiSignalQuality(GetWiFiSignalQuality(guid));
}

std::string Hardware::NIC::NetworkMonitor::GetWiFiSignalQuality(const Hardware::NIC::WiFiQuality wlanQuality) const
{
	std::string ret;
	switch (wlanQuality)
	{
	case WiFiQuality::Weak:
		ret = "Weak";
		break;
	case WiFiQuality::Fair:
		ret = "Fair";
		break;
	case WiFiQuality::Good:
		ret = "Good";
		break;
	case WiFiQuality::Excellent:
		ret = "Excellent";
		break;
	case WiFiQuality::DisConnect:
		ret = "Disconnected";
		break;
	default:
		ret = "Unknown";
		break;
	}
	return ret;
}

Hardware::NIC::WiFiQuality Hardware::NIC::NetworkMonitor::GetWiFiSignalQuality(const GUID& guid)
{
	WiFiQuality result = WiFiQuality::Unknown;
	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;

	PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
	PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;

	int iRSSI = 0;

	if (_wlanHandle == NULL)
	{
		dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &_wlanHandle);
		if (dwResult != ERROR_SUCCESS)
		{
			Utils::ErrorCode("WlanOpenHandle failed with error: ", dwResult);
			return result;
		}
	}

	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);

	dwResult = WlanQueryInterface(_wlanHandle,
		&guid,
		wlan_intf_opcode_current_connection,
		NULL,
		&connectInfoSize,
		(PVOID*)&pConnectInfo,
		nullptr);

	if (dwResult != ERROR_SUCCESS)
	{
		Utils::ErrorCode("WlanQueryInterface failed with error: ", dwResult);
		return result;
	}

	if (pConnectInfo->isState == wlan_interface_state_connected)
	{
		dwResult = WlanGetAvailableNetworkList(_wlanHandle,
			&guid,
			0,
			NULL,
			&pBssList);

		if (dwResult != ERROR_SUCCESS)
		{
			Utils::ErrorCode("WlanGetAvailableNetworkList failed with error: ", dwResult);
			return result;
		}

		for (auto j = 0; j < pBssList->dwNumberOfItems; j++)
		{
			pBssEntry = (WLAN_AVAILABLE_NETWORK*)&pBssList->Network[j];

			if (pBssEntry->bNetworkConnectable && (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED))
			{
				if (pBssEntry->wlanSignalQuality == 0)
					iRSSI = -100;
				else if (pBssEntry->wlanSignalQuality == 100)
					iRSSI = -50;
				else
					iRSSI = -100 + (pBssEntry->wlanSignalQuality / 2);
				spdlog::trace("Signal Quality:\t {} (RSSI: {} dBm)\n", pBssEntry->wlanSignalQuality, iRSSI);
				result = iRSSI < -70 ? WiFiQuality::Weak :
					iRSSI < -60 ? WiFiQuality::Fair :
					iRSSI < -50 ? WiFiQuality::Good : WiFiQuality::Excellent;
			}
		}
	}
	else if (pConnectInfo->isState == wlan_interface_state_disconnected || pConnectInfo->isState == wlan_interface_state_disconnecting)
	{
		result = WiFiQuality::DisConnect;
	}

	if (pBssList != NULL)
	{
		WlanFreeMemory(pBssList);
		pBssList = NULL;
	}

	if (pConnectInfo != NULL)
	{
		WlanFreeMemory(pConnectInfo);
		pConnectInfo = NULL;
	}
	return result;
}