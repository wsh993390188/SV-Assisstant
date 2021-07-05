#pragma once
#include "XMLConfig.h"
namespace Hardware
{
	namespace Utils
	{
		/// @brief ͨ��PCI DB��ȡ��Ϣ
		class PCIUtils : Hardware::XMLConfig
		{
			/*
			first : [vendorID] -> vencor name
			second : [vendorID][deviceID] -> device name
			*/
			typedef std::pair< std::map<int, std::string>, std::map< int, std::map<int, std::string> > > PCIDB;

			/// @brief ����PCI��Ϣ��
			PCIUtils();

			/// @brief ����
			PCIDB pciDB;
		public:
			/// @brief ����ģʽ
			/// @return @ref PCIUtils
			static PCIUtils& Instance();

			/// @brief ��ȡVID��Ӧ����
			/// @param VendorID VID
			/// @return ��Ӧ����
			std::string GetVendorName(const int VendorID);

			std::string GetDeviceName(const int VendorID, const int DeviceID);
		};
	}
}
