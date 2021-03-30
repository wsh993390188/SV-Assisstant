#include "stdafx.h"

#include "Type1.h"
#include "TypeImpl.h"
#include <sstream>
#include <iomanip>

namespace
{
	constexpr auto Manufacturer = 0x04;//String
	constexpr auto ProductName = 0x05;//String
	constexpr auto Version = 0x06;//String
	constexpr auto SerialNumber = 0x07;//String
	constexpr auto UUID = 0x08;//16BYTE
	constexpr auto WakeUp = 0x18;//String
	constexpr auto SKUNumber = 0x19;//String
	constexpr auto Family = 0x1A;//String
}

namespace
{
#pragma pack(push,1)
	struct UUIDStruct
	{
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned short Data4;
		unsigned char  Data5[6];
	};
#pragma pack(pop)

	std::string CreateUUID(const uint64_t& Most, const uint64_t& Least)
	{
		UUIDStruct temp;
		memcpy(&temp, &Most, sizeof(Most));
		memcpy((uint8_t*)&temp + 8, &Least, sizeof(Least));
		std::ostringstream oss;
		oss << std::uppercase << std::hex << std::setfill('0') << std::setw(8) << temp.Data1 << "-";
		oss << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << temp.Data2 << "-";
		oss << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << temp.Data3 << "-";
		oss << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << temp.Data4 << "-";
		for (int i = 0; i < 6; ++i)
			oss << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << temp.Data5[i] + 0;
		return oss.str();
	}
}

namespace
{
	class Type1Impl : public Smbios::TypeImpl
	{
	private:

		Type1Impl(const Type1Impl&) = delete;
		Type1Impl& operator=(const Type1Impl&) = delete;
	public:
		Type1Impl()
		{
			TypeVersion =
			{
				{ GetSmbiosType(Manufacturer),std::make_pair(2,0) },
				{ GetSmbiosType(ProductName),std::make_pair(2,0) },
				{ GetSmbiosType(Version),std::make_pair(2,0) },
				{ GetSmbiosType(SerialNumber),std::make_pair(2,0) },
				{ GetSmbiosType(UUID),std::make_pair(2,1) },
				{ GetSmbiosType(WakeUp),std::make_pair(2,1) },
				{ GetSmbiosType(SKUNumber),std::make_pair(2,1) },
				{ GetSmbiosType(Family),std::make_pair(2,4) }
			};
		}
		~Type1Impl() = default;
		TypeImpl& Create(const int32_t&) override
		{
			type.SwitchToTypePhyAddress(1);
			return *this;
		}

		std::string GetType()
		{
			return "Type1";
		}
	};
}

Smbios::Type1::Type1() : pImpl(std::make_unique<Type1Impl>())
{
	pImpl->Create(1);
}

Smbios::Type1::Type1(void* PhysicalAddress) : pImpl(std::make_unique<Type1Impl>())
{
	pImpl->Create(PhysicalAddress);
}

Smbios::Type1::~Type1() = default;

Smbios::Type1::Type1(Type1&&) = default;

Smbios::Type1& Smbios::Type1::operator=(Type1&&) = default;

std::string Smbios::Type1::Manufacturer()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::Manufacturer, Type1Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type1::Manufacturer(std::string & ManufacturerStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::Manufacturer, Type1Impl::GetFuncRealName(__FUNCTION__), ManufacturerStr);
}

std::string Smbios::Type1::ProductName()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::ProductName, Type1Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type1::ProductName(std::string & ProductNameStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::ProductName, Type1Impl::GetFuncRealName(__FUNCTION__), ProductNameStr);
}

std::string Smbios::Type1::Version()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::Version, Type1Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type1::Version(std::string & VersionStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::Version, Type1Impl::GetFuncRealName(__FUNCTION__), VersionStr);
}

std::string Smbios::Type1::SerialNumber()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::SerialNumber, Type1Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type1::SerialNumber(std::string & SerialNumberStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::SerialNumber, Type1Impl::GetFuncRealName(__FUNCTION__), SerialNumberStr);
}

std::string Smbios::Type1::UUID()
{
	auto UUIDMost = GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>(*pImpl, ::UUID, Type1Impl::GetFuncRealName(__FUNCTION__));
	auto UUIDLeast = GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>(*pImpl, ::UUID + 8, Type1Impl::GetFuncRealName(__FUNCTION__));
	return CreateUUID(UUIDMost, UUIDLeast);
}

bool Smbios::Type1::UUID(std::string & UUIDStr) const noexcept
{
	uint64_t UUIDLeast, UUIDMost;
	auto res = GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint64_t>(*pImpl, ::UUID, Type1Impl::GetFuncRealName(__FUNCTION__), UUIDMost);
	res &= GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint64_t>(*pImpl, ::UUID + 8, Type1Impl::GetFuncRealName(__FUNCTION__), UUIDLeast);
	if (res)
	{
		UUIDStr = CreateUUID(UUIDMost, UUIDLeast);
	}
	return res;
}

Smbios::WakeUpType Smbios::Type1::WakeUp()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, Smbios::WakeUpType>
		(*pImpl, ::WakeUp, Type1Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::WakeUpType>(ret);
	});
}

bool Smbios::Type1::WakeUp(WakeUpType & WakeUpStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::WakeUp, Type1Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::WakeUpType>(ret);
	}, WakeUpStr);
}

std::string Smbios::Type1::SKUNumber()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::SKUNumber, Type1Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type1::SKUNumber(std::string & SKUNumberStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::SKUNumber, Type1Impl::GetFuncRealName(__FUNCTION__), SKUNumberStr);
}

std::string Smbios::Type1::Family()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::Family, Type1Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type1::Family(std::string & FamilyStr) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::Family, Type1Impl::GetFuncRealName(__FUNCTION__), FamilyStr);
}