#include "stdafx.h"
#include "Type43.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto VendorID = 0x04;
	constexpr auto MajorSpecVersion = 0x08;
	constexpr auto MinorSpecVersion = 0x09;
	constexpr auto FirmwareVersion1 = 0x0A;
	constexpr auto FirmwareVersion2 = 0x0E;
	constexpr auto Description = 0x12;
	constexpr auto Characteristics = 0x13;
	constexpr auto OEMDefined = 0x1B;

	union Int32To8Helper
	{
		struct
		{
			int8_t LowInt81;
			int8_t LowInt82;
			int8_t HighInt81;
			int8_t HighInt82;
		};
		uint32_t Number;
		explicit Int32To8Helper(const uint32_t Num) : Number(Num) {}
	};
}

namespace
{
	class Type43Impl : public Smbios::TypeImpl
	{
	public:
		Type43Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(VendorID),std::make_pair(0,0)},
				{GetSmbiosType(MajorSpecVersion),std::make_pair(0,0)},
				{GetSmbiosType(MinorSpecVersion),std::make_pair(0,0)},
				{GetSmbiosType(FirmwareVersion1),std::make_pair(0,0)},
				{GetSmbiosType(FirmwareVersion2),std::make_pair(0,0)},
				{GetSmbiosType(Description),std::make_pair(0,0)},
				{GetSmbiosType(Characteristics),std::make_pair(0,0)},
				{GetSmbiosType(OEMDefined),std::make_pair(0,0)}
			};
		}
		~Type43Impl() = default;
		Type43Impl(const Type43Impl&) = delete;
		Type43Impl& operator=(const Type43Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type43";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(43, TableNum);
		}
	};
}

Smbios::Type43::Type43(const int32_t& TableNum) : pImpl(std::make_unique<Type43Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type43::Type43(void* PhysicalAddress) : pImpl(std::make_unique<Type43Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type43::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type43::~Type43() = default;

Smbios::Type43::Type43(Type43&&) = default;

Smbios::Type43& Smbios::Type43::operator=(Type43&&) = default;

std::string Smbios::Type43::VendorID() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t, std::string>
		(*pImpl, ::VendorID, Type43Impl::GetFuncRealName(__FUNCTION__),
			[](const uint32_t& num)
	{
		std::string ret;
		Int32To8Helper temp(num);
		if (temp.LowInt81 != 0)
		{
			ret.push_back(temp.LowInt81);
		}
		if (temp.LowInt82 != 0)
		{
			ret.push_back(temp.LowInt82);
		}
		if (temp.HighInt81 != 0)
		{
			ret.push_back(temp.HighInt81);
		}
		if (temp.HighInt82 != 0)
		{
			ret.push_back(temp.HighInt82);
		}
		return ret;
	});
}

bool Smbios::Type43::VendorID(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint32_t>
		(*pImpl, ::VendorID, Type43Impl::GetFuncRealName(__FUNCTION__),
			[](const uint32_t& num)
	{
		std::string ret;
		Int32To8Helper temp(num);
		if (temp.LowInt81 != 0)
		{
			ret.push_back(temp.LowInt81);
		}
		if (temp.LowInt82 != 0)
		{
			ret.push_back(temp.LowInt82);
		}
		if (temp.HighInt81 != 0)
		{
			ret.push_back(temp.HighInt81);
		}
		if (temp.HighInt82 != 0)
		{
			ret.push_back(temp.HighInt82);
		}
		return ret;
	}, Str);
}

std::uint8_t Smbios::Type43::MajorSpecVersion() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::MajorSpecVersion, Type43Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type43::MajorSpecVersion(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MajorSpecVersion, Type43Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type43::MinorSpecVersion() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::MinorSpecVersion, Type43Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type43::MinorSpecVersion(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MinorSpecVersion, Type43Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type43::FirmwareVersion1() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::FirmwareVersion1, Type43Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type43::FirmwareVersion1(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::FirmwareVersion1, Type43Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type43::FirmwareVersion2() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::FirmwareVersion2, Type43Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type43::FirmwareVersion2(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::FirmwareVersion2, Type43Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type43::Description() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Description, Type43Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type43::Description(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Description, Type43Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::TPMDeviceCharacteristicsField Smbios::Type43::Characteristics() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t, Smbios::TPMDeviceCharacteristicsField>
		(*pImpl, ::Characteristics, Type43Impl::GetFuncRealName(__FUNCTION__),
			[](const uint64_t& ret)
	{
		return static_cast<Smbios::TPMDeviceCharacteristicsField>(ret);
	});
}

bool Smbios::Type43::Characteristics(Smbios::TPMDeviceCharacteristicsField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint64_t>
		(*pImpl, ::Characteristics, Type43Impl::GetFuncRealName(__FUNCTION__),
			[](const uint64_t& ret)
	{
		return static_cast<Smbios::TPMDeviceCharacteristicsField>(ret);
	}, Str);
}

std::uint32_t Smbios::Type43::OEMDefined() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t>
		(*pImpl, ::OEMDefined, Type43Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type43::OEMDefined(uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMDefined, Type43Impl::GetFuncRealName(__FUNCTION__), Str);
}