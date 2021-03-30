#include "stdafx.h"
#include "OEMControl.h"

#include "Type1.h"

using namespace Smbios;

OEMControl& Smbios::OEMControl::Instance()
{
	static OEMControl inst;
	return inst;
}

Smbios::OEMControl::OEMControl() : dmi_vendor(OemVendor::VENDOR_UNKNOWN)
{
	Type1 type;
	std::string Vendor;
	auto res = type.Manufacturer(Vendor);
	if (res)
	{
		if (Vendor.compare("Acer") == 0)
			dmi_vendor = OemVendor::VENDOR_ACER;
		else if (Vendor.compare("HP") == 0 || Vendor.compare("Hewlett-Packard") == 0)
			dmi_vendor = OemVendor::VENDOR_HP;
		else if (Vendor.compare("HPE") == 0 || Vendor.compare("Hewlett Packard Enterprise") == 0)
			dmi_vendor = OemVendor::VENDOR_HP;
		else if (Vendor.compare("IBM") == 0)
			dmi_vendor = OemVendor::VENDOR_LENOVO;
		else if (Vendor.compare("LENOVO") == 0)
			dmi_vendor = OemVendor::VENDOR_LENOVO;
	}
}

const OEMControl::OemVendor Smbios::OEMControl::GetOemVendor() const noexcept
{
	return dmi_vendor;
}