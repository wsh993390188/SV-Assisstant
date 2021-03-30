#include "stdafx.h"

#include "Type3.h"
#include "TypeImpl.h"
namespace
{
	constexpr auto Manufacturer = 0x04;
	constexpr auto Type = 0x05;
	constexpr auto Version = 0x06;
	constexpr auto SerialNumber = 0x07;
	constexpr auto AssetTag = 0x08;
	constexpr auto BootUpState = 0x09;
	constexpr auto PowerSupplyStatus = 0x0A;
	constexpr auto ThermalState = 0x0B;
	constexpr auto SecurityStatus = 0x0C;
	constexpr auto OEMdefined = 0x0D;
	constexpr auto Height = 0x11;
	constexpr auto NumofPowerCords = 0x12;
	constexpr auto ContainedElementCount = 0x13;
	constexpr auto ContainedElementLength = 0x14;
	constexpr auto ContainedElement = 0x15;

	const std::string SystemEnclosureorChassisTypes[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Desktop"),
		("Low Profile Desktop"),
		("Pizza Box"),
		("Mini Tower"),
		("Tower"),
		("Portable"),
		("Laptop"),
		("Notebook"),
		("Hand held"),
		("Docking Station"),
		("All In one"),
		("Sub Notebook"),
		("Space saving"),
		("Lunch Box"),
		("Main Server Chassis"),
		("Expansion Chassis"),
		("SubChassis"),
		("BUS Expansion Chassis"),
		("Peripheral Chassis"),
		("RAID Chassis"),
		("Rack Mount Chassis"),
		("Sealed case PC"),
		("Multi system Chassis"),
		("Compact PCI"),
		("Advanced TCA"),
		("Blade"),
		("Blade Enclosure"),
		("Tablet"),
		("Convertible"),
		("Detachable"),
		("IoT GateWay"),
		("Embedded PC"),
		("mini PC"),
		("Stick PC"),
	};
}

namespace
{
	class Type3Impl : public Smbios::TypeImpl
	{
		Type3Impl(const Type3Impl&) = delete;
		Type3Impl& operator=(const Type3Impl&) = delete;
	public:
		Type3Impl()
		{
			TypeVersion = {
				{GetSmbiosType(Manufacturer), std::make_pair(2, 0)},
				{ GetSmbiosType(Type),std::make_pair(2,0) },
				{ GetSmbiosType(Version),std::make_pair(2,0) },
				{ GetSmbiosType(SerialNumber),std::make_pair(2,0) },
				{ GetSmbiosType(AssetTag),std::make_pair(2,0) },
				{ GetSmbiosType(BootUpState),std::make_pair(2,1) },
				{ GetSmbiosType(PowerSupplyState),std::make_pair(2,1) },
				{ GetSmbiosType(ThermalState),std::make_pair(2,1) },
				{ GetSmbiosType(SecurityState),std::make_pair(2,1) },
				{ GetSmbiosType(OEMDefined),std::make_pair(2,3) },
				{ GetSmbiosType(Height),std::make_pair(2,3) },
				{ GetSmbiosType(NumberofPowerCords),std::make_pair(2,3) },
				{ GetSmbiosType(ContainedElementCount),std::make_pair(2,3) },
				{ GetSmbiosType(ContainedElementRecordLength),std::make_pair(2,3) },
				{ GetSmbiosType(ContainedElements),std::make_pair(2,3) },
				{ GetSmbiosType(SKUNumber),std::make_pair(2,7) }
			};
		}
		~Type3Impl() = default;
		TypeImpl& Create(const int32_t&) override
		{
			type.SwitchToTypePhyAddress(3);
			return *this;
		}

		std::string GetType()
		{
			return "Type3";
		}
	};
}

using namespace Smbios;

Smbios::Type3::Type3() : pImpl(std::make_unique<Type3Impl>())
{
	pImpl->Create(3);
}

Smbios::Type3::Type3(void* PhysicalAddress) : pImpl(std::make_unique<Type3Impl>())
{
	pImpl->Create(PhysicalAddress);
}

Smbios::Type3::~Type3() = default;

Smbios::Type3::Type3(Type3&&) = default;

Type3& Smbios::Type3::operator=(Type3&&) = default;

std::string Smbios::Type3::Manufacturer()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::Manufacturer, Type3Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type3::Manufacturer(std::string & ManufacturerStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::Manufacturer, Type3Impl::GetFuncRealName(__FUNCTION__), ManufacturerStr);
}

std::string Smbios::Type3::Type()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>(*pImpl, ::Type, Type3Impl::GetFuncRealName(__FUNCTION__),
		[](const uint8_t& ret) -> std::string {
		if (ret < _countof(SystemEnclosureorChassisTypes))
			return SystemEnclosureorChassisTypes[ret];
		else
			return {};
	});
}
bool Smbios::Type3::Type(std::string & TypeStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t, std::string>
		(*pImpl, ::Type, Type3Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) ->std::string {
		if (ret < _countof(SystemEnclosureorChassisTypes))
		{
			return SystemEnclosureorChassisTypes[ret];
		}
		return {};
	}, TypeStr);
}

std::string Smbios::Type3::Version()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::Version, Type3Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type3::Version(std::string & VersionStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::Version, Type3Impl::GetFuncRealName(__FUNCTION__), VersionStr);
}

std::string Smbios::Type3::SerialNumber()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::SerialNumber, Type3Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type3::SerialNumber(std::string & SerialNumberStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::SerialNumber, Type3Impl::GetFuncRealName(__FUNCTION__), SerialNumberStr);
}

std::string Smbios::Type3::AssetTag()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::AssetTag, Type3Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type3::AssetTag(std::string & AssetTagStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::AssetTag, Type3Impl::GetFuncRealName(__FUNCTION__), AssetTagStr);
}

Type3StatusType Smbios::Type3::BootUpState()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, Smbios::Type3StatusType>
		(*pImpl, ::BootUpState, Type3Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		return static_cast<Smbios::Type3StatusType>(ret);
	});
}

bool Smbios::Type3::BootUpState(Type3StatusType & Status) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::BootUpState, Type3Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::Type3StatusType>(ret);
	}, Status);
}

Type3StatusType Smbios::Type3::PowerSupplyState()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, Smbios::Type3StatusType>
		(*pImpl, ::PowerSupplyStatus, Type3Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		return static_cast<Smbios::Type3StatusType>(ret);
	});
}

bool Smbios::Type3::PowerSupplyState(Type3StatusType & Status) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::PowerSupplyStatus, Type3Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::Type3StatusType>(ret);
	}, Status);
}

Type3StatusType Smbios::Type3::ThermalState()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, Smbios::Type3StatusType>
		(*pImpl, ::ThermalState, Type3Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		return static_cast<Smbios::Type3StatusType>(ret);
	});
}

bool Smbios::Type3::ThermalState(Type3StatusType & Status) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::ThermalState, Type3Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::Type3StatusType>(ret);
	}, Status);
}

Type3SecurityStatus Smbios::Type3::SecurityState()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, Smbios::Type3SecurityStatus>
		(*pImpl, ::SecurityStatus, Type3Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		return static_cast<Smbios::Type3SecurityStatus>(ret);
	});
}

bool Smbios::Type3::SecurityState(Type3SecurityStatus & Status) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::SecurityStatus, Type3Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::Type3SecurityStatus>(ret);
	}, Status);
}

uint32_t Smbios::Type3::OEMDefined()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t>(*pImpl, ::OEMdefined, Type3Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type3::OEMDefined(uint32_t & Defined) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMdefined, Type3Impl::GetFuncRealName(__FUNCTION__), Defined);
}

uint8_t Smbios::Type3::Height()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>(*pImpl, ::Height, Type3Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type3::Height(uint8_t & Height) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Height, Type3Impl::GetFuncRealName(__FUNCTION__), Height);
}

uint8_t Smbios::Type3::NumberofPowerCords()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>(*pImpl, ::NumofPowerCords, Type3Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type3::NumberofPowerCords(uint8_t & PowerCord) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NumofPowerCords, Type3Impl::GetFuncRealName(__FUNCTION__), PowerCord);
}

uint8_t Smbios::Type3::ContainedElementCount()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>(*pImpl, ::ContainedElementCount, Type3Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type3::ContainedElementCount(uint8_t & Count) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ContainedElementCount, Type3Impl::GetFuncRealName(__FUNCTION__), Count);
}

uint8_t Smbios::Type3::ContainedElementRecordLength()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>(*pImpl, ::ContainedElementLength, Type3Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type3::ContainedElementRecordLength(uint8_t & Length) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ContainedElementLength, Type3Impl::GetFuncRealName(__FUNCTION__), Length);
}

std::vector<uint8_t> Smbios::Type3::ContainedElements()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::vector<uint8_t>>
		(*pImpl, ::ContainedElementCount, Type3Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& n)
	{
		std::vector<uint8_t> ret;
		auto m = ContainedElementRecordLength();
		for (auto i = 0; i < n * m; ++i)
		{
			ret.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, ::ContainedElement + i));
		}
		return ret;
	});
}

bool Smbios::Type3::ContainedElements(std::vector<uint8_t> & Elements) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::ContainedElementCount, Type3Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& n)
	{
		std::vector<uint8_t> ret;
		uint8_t m = 0;
		if (ContainedElementRecordLength(m))
		{
			for (auto i = 0; i < n * m; ++i)
			{
				ret.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, ::ContainedElement + i));
			}
		}
		return ret;
	}, Elements);
}

std::string Smbios::Type3::SKUNumber()
{
	std::vector<uint8_t> res;
	auto n = ContainedElementCount();
	auto m = ContainedElementRecordLength();
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::ContainedElement + n * m, Type3Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type3::SKUNumber(std::string & SKU) const noexcept
{
	uint8_t n, m = 0;
	if (ContainedElementCount(n) && ContainedElementRecordLength(m))
	{
		return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl,
			::ContainedElement + n * m, Type3Impl::GetFuncRealName(__FUNCTION__), SKU);
	}
	else
	{
		SKU = {};
		return false;
	}
}