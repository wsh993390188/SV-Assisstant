#include "stdafx.h"
#include "PCIUtils.h"
#include "resource.h"
#include <sstream>
extern HMODULE g_hModule;

Hardware::Utils::PCIUtils::PCIUtils()
{
	std::string content;
	if (GetConfigXmlFromResource(content, IDR_PCIIDS2, L"PCIIDS"))
	{
		std::istringstream iss(content);
		std::string line, item;
		int vendorID = -1;

		while (std::getline(iss, line)) {
			// Ignore any line starting with #
			if (line.size() == 0 || line[0] == '#' || line[0] == '\r')
				continue;

			if (line[0] == '\t' && line[1] == '\t')
			{
				// subvendor subdevice  subsystem_name
				continue;
			}

			if (line[0] == '\t')
			{
				int deviceID = stoi(line.substr(1, 4), 0, 16);
				//std::cout << vendorID << ";" << vendorName << ";" << deviceID << ";" << line.substr(7) << "\n";
				pciDB.second[vendorID][deviceID] = line.substr(7);
				continue;
			}
			// vendor
			vendorID = stoi(line.substr(0, 4), 0, 16);
			pciDB.first[vendorID] = line.substr(6);
		}
	}
}

Hardware::Utils::PCIUtils& Hardware::Utils::PCIUtils::Instance()
{
	static PCIUtils inst;
	return inst;
}

std::string Hardware::Utils::PCIUtils::GetVendorName(const int VendorID)
{
	return (pciDB.first.count(VendorID) > 0) ? pciDB.first.at(VendorID).c_str() : "unknown vendor";
}

std::string Hardware::Utils::PCIUtils::GetDeviceName(const int VendorID, const int DeviceID)
{
	return (pciDB.second.count(VendorID) > 0 && pciDB.second.at(VendorID).count(DeviceID) > 0) ? pciDB.second.at(VendorID).at(DeviceID).c_str() : "unknown device";
}