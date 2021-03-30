#pragma once
#include <netlistmgr.h>
#include <atlbase.h>

namespace Hardware
{
	namespace NIC
	{
		/// @brief Wifi�����ּ�
		enum class WiFiQuality
		{
			DisConnect,		  ///< ������
			Unknown,		  ///< ״̬δ֪
			Weak,             ///< ���� (-INF, -70dBm)
			Fair,             ///< һ�� [-70dBm, -60dBm)
			Good,             ///< �Ϻ� [-60dBm, -50dBm)
			Excellent         ///< ��� [-50dBm, +INF)
		};

		class NetworkMonitor final
		{
		public:
			static NetworkMonitor& Instance();
			std::string QueryWiFiSignalQuality(const GUID& guid);
		private:
			NetworkMonitor();
			~NetworkMonitor();
			WiFiQuality GetWiFiSignalQuality(const GUID& guid);
			std::string GetWiFiSignalQuality(const Hardware::NIC::WiFiQuality wlanQuality)const;
		private:
			HANDLE _wlanHandle;
		};
	}
}
