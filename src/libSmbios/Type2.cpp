#include "stdafx.h"

#include "Type2.h"
#include "TypeImpl.h"

namespace
{
	constexpr int Manufacturer = 0x04;
	constexpr int ProductName = 0x05;
	constexpr int Version = 0x06;
	constexpr int SerialNumber = 0x07;
	constexpr int AssetTag = 0x08;//String
	constexpr int FeatureFlags = 0x09;//BTYE£¨bit£©
	constexpr int LocationInChassis = 0x0A;//String
	constexpr int ChassisHandle = 0x0B;//WORD
	constexpr int BoardType = 0x0D;//BYTE(ENUM)
	constexpr int NumObjHandles = 0x0E;//BYTE
	constexpr int ObjHandles = 0x0F;//n WORDs
}

namespace
{
	const std::string BoardTypeStrs[] =
	{
		{},
		"Unknown",
		"Other",
		"Server Blade",
		"Connectivity Switch",
		"System Management Module",
		"Processor Module",
		"I / O Module",
		"Memory Module",
		"Daughter board",
		"Motherboard(includes processor, memory,and I / O)",
		"Processor / Memory Module",
		"Processor / IO Module",
		"Interconnect board"
	};
	class Type2Impl : public Smbios::TypeImpl
	{
	public:
		Type2Impl()
		{
			TypeVersion =
			{
				{ GetSmbiosType(Manufacturer),std::make_pair(0,0) },
				{ GetSmbiosType(Product),std::make_pair(0,0) },
				{ GetSmbiosType(Version),std::make_pair(0,0) },
				{ GetSmbiosType(SerialNumber),std::make_pair(0,0) },
				{ GetSmbiosType(AssetTag),std::make_pair(0,0) },
				{ GetSmbiosType(FeatureFlags),std::make_pair(0,0) },
				{ GetSmbiosType(LocationinChassis),std::make_pair(0,0) },
				{ GetSmbiosType(ChassisHandle),std::make_pair(0,0) },
				{ GetSmbiosType(Board),std::make_pair(0,0) },
				{ GetSmbiosType(NumberofContainedObjectHandles),std::make_pair(0,0) },
				{ GetSmbiosType(ContainedObjectHandles),std::make_pair(0,0) },
			};
		}
		~Type2Impl() = default;
		Type2Impl(const Type2Impl&) = delete;
		Type2Impl& operator=(const Type2Impl&) = delete;
	public:
		TypeImpl& Create(const int32_t&) override
		{
			type.SwitchToTypePhyAddress(2);
			return *this;
		}

		std::string GetType()
		{
			return "Type2";
		}
	};
}
Smbios::Type2::Type2() : pImpl(std::make_unique<Type2Impl>())
{
	pImpl->Create(2);
}

Smbios::Type2::Type2(void* PhysicalAddress) : pImpl(std::make_unique<Type2Impl>())
{
	pImpl->Create(PhysicalAddress);
}

Smbios::Type2::~Type2() = default;

Smbios::Type2::Type2(Type2&&) = default;

Smbios::Type2& Smbios::Type2::operator=(Type2&&) = default;

std::string Smbios::Type2::Manufacturer()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::Manufacturer, Type2Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type2::Manufacturer(std::string & ManufacturerStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::Manufacturer, Type2Impl::GetFuncRealName(__FUNCTION__), ManufacturerStr);
}

std::string Smbios::Type2::Product()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::ProductName, Type2Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type2::Product(std::string & ProductStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::ProductName, Type2Impl::GetFuncRealName(__FUNCTION__), ProductStr);
}

std::string Smbios::Type2::Version()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::Version, Type2Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type2::Version(std::string & VersionStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::Version, Type2Impl::GetFuncRealName(__FUNCTION__), VersionStr);
}

std::string Smbios::Type2::SerialNumber()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::SerialNumber, Type2Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type2::SerialNumber(std::string & SerialNumberStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::SerialNumber, Type2Impl::GetFuncRealName(__FUNCTION__), SerialNumberStr);
}

std::string Smbios::Type2::AssetTag()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::AssetTag, Type2Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type2::AssetTag(std::string & AssetTagStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::AssetTag, Type2Impl::GetFuncRealName(__FUNCTION__), AssetTagStr);
}

uint8_t Smbios::Type2::FeatureFlags()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>(*pImpl, ::FeatureFlags, Type2Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type2::FeatureFlags(uint8_t & Feature) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>(*pImpl, ::FeatureFlags, Type2Impl::GetFuncRealName(__FUNCTION__), Feature);
}

std::string Smbios::Type2::LocationinChassis()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::LocationInChassis, Type2Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type2::LocationinChassis(std::string & LocationStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::LocationInChassis, Type2Impl::GetFuncRealName(__FUNCTION__), LocationStr);
}

uint16_t Smbios::Type2::ChassisHandle()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>(*pImpl, ::ChassisHandle, Type2Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type2::ChassisHandle(uint16_t & Handle) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>(*pImpl, ::ChassisHandle, Type2Impl::GetFuncRealName(__FUNCTION__), Handle);
}

std::string Smbios::Type2::Board()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>(*pImpl, ::BoardType, Type2Impl::GetFuncRealName(__FUNCTION__),
		[](const uint8_t& ret) -> std::string {
		if (ret < _countof(BoardTypeStrs))
			return BoardTypeStrs[ret];
		else
			return {};
	});
}

bool Smbios::Type2::Board(std::string & BoardType) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t, std::string>
		(*pImpl, ::BoardType, Type2Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) ->std::string {
		if (ret < _countof(BoardTypeStrs))
		{
			return BoardTypeStrs[ret];
		}
		return {};
	}, BoardType);
}

uint8_t Smbios::Type2::NumberofContainedObjectHandles()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>(*pImpl, ::NumObjHandles, Type2Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type2::NumberofContainedObjectHandles(uint8_t & Handles) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>(*pImpl, ::NumObjHandles, Type2Impl::GetFuncRealName(__FUNCTION__), Handles);
}

std::vector<uint16_t> Smbios::Type2::ContainedObjectHandles()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::vector<uint16_t>>
		(*pImpl, ::NumObjHandles, Type2Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& Number)
	{
		std::vector<uint16_t> ret;
		for (auto i = 0; i < 2 * Number - 1; i += 2)
		{
			auto TempHandles = GetSmbiosInformation<std::remove_reference_t<uint16_t>>(*pImpl, ::ObjHandles + i);
			ret.emplace_back(TempHandles);
		}
		return ret;
	});
}

bool Smbios::Type2::ContainedObjectHandles(std::vector<uint16_t> & ObjHandles) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t, std::vector<uint16_t>>
		(*pImpl, ::NumObjHandles, Type2Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& Number)
	{
		std::vector<uint16_t> ret;
		for (auto i = 0; i < 2 * Number - 1; i += 2)
		{
			ret.emplace_back(GetSmbiosInformation<uint16_t>(*pImpl, ::ObjHandles + i));
		}
		return ret;
	}, ObjHandles);
}