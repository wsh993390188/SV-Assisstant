#include "stdafx.h"
#include <map>
#include "LenovoDisplay.h"
#include "LibSMBIOS.h"

using namespace Smbios;

namespace
{
	class LenovoDisplayImpl
	{
	public:
		static LenovoDisplayImpl& Instance()
		{
			static LenovoDisplayImpl inst;
			return inst;
		}

		bool IsLenovoMachine()
		{
			return Utils::StringIsEqualsNoCase(GetBiosVendor(), ("LENOVO"));
		}

		bool IsThink()
		{
			try
			{
				auto Name = GetType(Smbios::SmbiosHeaderType::Type1()).Version();
				Utils::trim(Name);
				if (Name.find("ThinkPad") == 0 || Name.find("ThinkStation") == 0 || (Name.find("ThinkCentre") == 0 &&
					(Name[12] == 'A' || Name[12] == 'E' || Name[12] == 'N' || Name[12] == 'S' || Name[12] == 'T' || Name[12] == 'W')))
					return true;
				return false;
			}
			catch (const std::exception&)
			{
				return false;
			}
		}

		std::wstring GetSN()
		{
			try
			{
				if (!IsLenovoMachine())
				{
					return Utils::utf8ToWstring(GetType(Smbios::SmbiosHeaderType::Type1()).SerialNumber());
				}
				else
				{
					if (IsThink())
					{
						return Utils::utf8ToWstring(GetType(Smbios::SmbiosHeaderType::Type1()).ProductName() + GetType(Smbios::SmbiosHeaderType::Type1()).SerialNumber());
					}
					else
					{
						auto ProductSN = GetType(Smbios::SmbiosHeaderType::Type1()).SerialNumber();
						if (!isLenovoSNFormat(ProductSN))
						{
							auto BaseBoardSN = GetType(Smbios::SmbiosHeaderType::Type2()).SerialNumber();
							if (isLenovoSNFormat(BaseBoardSN))
								return Utils::utf8ToWstring(BaseBoardSN);
						}
						return Utils::utf8ToWstring(ProductSN);
					}
				}
			}
			catch (const std::exception&)
			{
				return {};
			}
		}

		std::wstring GetMachineType()
		{
			if (!IsLenovoMachine())
			{
				try
				{
					return Utils::utf8ToWstring(GetType(Smbios::SmbiosHeaderType::Type1()).ProductName());
				}
				catch (const std::exception&)
				{
					return {};
				}
			}
			else
			{
				try
				{
					auto itr = BiosVersionToMachineType.find(GetType(Smbios::SmbiosHeaderType::Type0()).BiosVersion());
					if (itr != BiosVersionToMachineType.end())
						return itr->second;
				}
				catch (const std::exception&)
				{
					return {};
				}

				try
				{
					auto MachineType = GetType(Smbios::SmbiosHeaderType::Type1()).ProductName();
					if (MachineType.find("Lenovo Product") == decltype(MachineType)::npos || MachineType.find("INVALID") == decltype(MachineType)::npos)
					{
						for (const auto& prop : LenovoMachineTypeChac)
						{
							if (MachineType.find(prop) != decltype(MachineType)::npos)
							{
								return Utils::utf8ToWstring(MachineType);
							}
						}
					}

					return Utils::utf8ToWstring(GetType(Smbios::SmbiosHeaderType::Type1()).Version());
				}
				catch (const std::exception&)
				{
					return {};
				}
			}
		}

		std::wstring GetMT()
		{
			if (IsLenovoMachine())
			{
				try
				{
					return Utils::utf8ToWstring(GetType(Smbios::SmbiosHeaderType::Type1()).ProductName().substr(0, 4));
				}
				catch (const std::exception&)
				{
				}
			}
			return {};
		}

		std::wstring GetMTM()
		{
			if (IsLenovoMachine())
			{
				try
				{
					return Utils::utf8ToWstring(GetType(Smbios::SmbiosHeaderType::Type200()).at(0).MTM());
				}
				catch (const std::exception&)
				{
				}
			}
			return {};
		}
	private:
		bool isLenovoSNFormat(const std::string& strSn)
		{
			auto len = strSn.length();
			if (!(len == 10 || len == 8)) {
				return false;
			}
			return true;
		}
	private:
		std::map<std::string, std::wstring> BiosVersionToMachineType =
		{
			{"51KT54AUS",L"QiTian M6900" },
			{"9GKT32AUS", L"QiTian M7300"},
			{"2TKT00AUS",L"KaiTian S4700"},
			{"2OKT41AUS", L"KaiTian B6600"},
			{"90KT22CUS", L"M7150"},
		};

		std::vector<std::string> LenovoMachineTypeChac =
		{
			{"MIIX"},
			{"ZHAOYANG"},
			{"IDEA"},
			{"YOGA"},
			{"THINK"},
			{"YANGTIAN"},
			{"YT"},
			{"QITIAN"},
			{"KAITIAN"},
		};
	};
}

std::wstring Smbios::LenovoDisplay::GetSNFromSMbios()
{
	auto SN = LenovoDisplayImpl::Instance().GetSN();
	Utils::trim(SN);
	return SN;
}

std::wstring Smbios::LenovoDisplay::GetMachineTypeFromSMbios()
{
	return LenovoDisplayImpl::Instance().GetMachineType();
}

std::wstring Smbios::LenovoDisplay::GetMachineType()
{
	return LenovoDisplayImpl::Instance().GetMT();
}

std::wstring Smbios::LenovoDisplay::GetMachineTypeModel()
{
	return LenovoDisplayImpl::Instance().GetMTM();
}

std::wstring Smbios::LenovoDisplay::GetMachineUUID()
{
	try
	{
		return Utils::utf8ToWstring(GetType(Smbios::SmbiosHeaderType::Type1()).UUID());
	}
	catch (const std::exception&)
	{
		return{};
	}
}

bool Smbios::LenovoDisplay::IsLenovo()
{
	return LenovoDisplayImpl::Instance().IsLenovoMachine();
}

bool Smbios::LenovoDisplay::IsThink()
{
	return LenovoDisplayImpl::Instance().IsThink();
}