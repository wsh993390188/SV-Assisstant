#include "stdafx.h"
#include "TypeImpl.h"
#include "RawSmbios.h"

Smbios::EntryPoint::EntryPoint() : TypePhyAddress{}, SmbiosMajorVersion{}, SmbiosMinorVersion{}
{
	auto addr = static_cast<RawSMBIOSData*>(RawSmbios::Instance()->GetPhyAdderss());
	if (addr)
	{
		SmbiosMajorVersion = addr->SMBIOSMajorVersion;
		SmbiosMinorVersion = addr->SMBIOSMinorVersion;
		TypePhyAddress = addr->SMBIOSTableData;
	}
}

std::string Smbios::EntryPoint::DMIToString(uint8_t Targetstring) const
{
	if (Targetstring == 0)
	{
		return  std::string("NULL");
	}

	auto data = reinterpret_cast<char*>(TypePhyAddress);

	data += reinterpret_cast<DMI_Header*>(TypePhyAddress)->length;

	while (Targetstring > 1 && *data)
	{
		data += strlen(data);
		Targetstring--;
		++data;
	}

	if (!*data)
		return std::string("BAD_INDEX");

	return std::string(data);
}

void Smbios::EntryPoint::SwitchToTypePhyAddress(uint8_t TypeNum, int8_t TargetTable)
{
	if (!TypePhyAddress)
		return;
	auto test = reinterpret_cast<DMI_Header*>(TypePhyAddress);
	while (test->type != SmbiosEndofTable)
	{
		if (test->type == TypeNum)
		{
			if (TargetTable == 0)
				break;
			--TargetTable;
		}

		TypePhyAddress += test->length;//跳过结构段
		while ((*reinterpret_cast<uint16_t*>(TypePhyAddress)) != 0) // 跳过字符段
		{
			++TypePhyAddress;
		}
		TypePhyAddress += 2; //跳过SMBIOS段结束符 0000
		test = reinterpret_cast<DMI_Header*>(TypePhyAddress);
	}

	if (test->type == SmbiosEndofTable)
	{
		TypePhyAddress = nullptr;
	}
}

void Smbios::EntryPoint::ResetPhyAddress()
{
	auto addr = static_cast<RawSMBIOSData*>(RawSmbios::Instance()->GetPhyAdderss());
	if (addr)
	{
		TypePhyAddress = addr->SMBIOSTableData;
	}
}

Smbios::TypeImpl::TypeImpl()
{
}

Smbios::TypeImpl::~TypeImpl()
{
}
