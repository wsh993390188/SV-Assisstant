#pragma once
#include <netlistmgr.h>
#include <atlbase.h>

namespace Hardware
{
	namespace NIC
	{
		/// @brief Wifi质量分级
		enum class WiFiQuality
		{
			DisConnect,		  ///< 无网络
			Unknown,		  ///< 状态未知
			Weak,             ///< 极差 (-INF, -70dBm)
			Fair,             ///< 一般 [-70dBm, -60dBm)
			Good,             ///< 较好 [-60dBm, -50dBm)
			Excellent         ///< 最好 [-50dBm, +INF)
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
