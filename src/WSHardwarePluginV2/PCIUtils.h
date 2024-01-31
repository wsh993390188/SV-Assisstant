#pragma once
#include "XMLConfig.h"
namespace Hardware
{
	namespace Utils
	{
		/// @brief 通过PCI DB获取信息
		class PCIUtils : Hardware::XMLConfig
		{
			/*
			first : [vendorID] -> vencor name
			second : [vendorID][deviceID] -> device name
			*/
			typedef std::pair< std::map<int, std::string>, std::map< int, std::map<int, std::string> > > PCIDB;

			/// @brief 构造PCI信息库
			PCIUtils();

			/// @brief 对象
			PCIDB pciDB;
		public:
			/// @brief 单例模式
			/// @return @ref PCIUtils
			static PCIUtils& Instance();

			/// @brief 获取VID对应名称
			/// @param VendorID VID
			/// @return 对应名称
			std::string GetVendorName(const int VendorID);

			std::string GetDeviceName(const int VendorID, const int DeviceID);
		};
	}
}
