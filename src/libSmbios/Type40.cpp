#include "stdafx.h"
#include "Type40.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto NumofAdditionInfEntries = 0x04;
	constexpr auto AdditionInfEntries = 0x05;

	constexpr auto EntryLength = 0;
	constexpr auto ReferencedHandle = 1;
	constexpr auto ReferencedOffset = 3;
	constexpr auto String = 4;
	constexpr auto Values = 5;
}

namespace
{
	class Type40Impl : public Smbios::TypeImpl
	{
	public:
		Type40Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(NumofAdditionInfEntries),std::make_pair(0,0)},
				{GetSmbiosType(AdditionInfEntries),std::make_pair(0,0)}
			};
		}
		~Type40Impl() = default;
		Type40Impl(const Type40Impl&) = delete;
		Type40Impl& operator=(const Type40Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type40";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(40, TableNum);
		}
	};
}

Smbios::Type40::Type40(const int32_t& TableNum) : pImpl(std::make_unique<Type40Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type40::Type40(void* PhysicalAddress) : pImpl(std::make_unique<Type40Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type40::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type40::~Type40() = default;

Smbios::Type40::Type40(Type40&&) = default;

Smbios::Type40& Smbios::Type40::operator=(Type40&&) = default;

std::uint8_t Smbios::Type40::NumofAdditionInfEntries() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::NumofAdditionInfEntries, Type40Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type40::NumofAdditionInfEntries(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NumofAdditionInfEntries, Type40Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::vector<Smbios::AdditionalInformationEntry> Smbios::Type40::AdditionInfEntries() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::vector<AdditionalInformationEntry>>
		(*pImpl, ::NumofAdditionInfEntries, Type40Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector<Smbios::AdditionalInformationEntry> ret;
		uint32_t BeginOffset = ::AdditionInfEntries;
		for (auto index = 0; index < length; ++index)
		{
			Smbios::AdditionalInformationEntry temp;
			auto EntryLength = GetSmbiosInformation<uint8_t>(*pImpl, BeginOffset + ::EntryLength);
			temp.ReferencedHandle = GetSmbiosInformation<uint16_t>(*pImpl, BeginOffset + ::ReferencedHandle);
			temp.ReferencedOffset = GetSmbiosInformation<uint8_t>(*pImpl, BeginOffset + ::ReferencedOffset);
			temp.Desc = GetSmbiosInformation<std::string>(*pImpl, BeginOffset + ::String);
			for (auto begin = ::Values; begin < EntryLength; ++begin)
				temp.Values.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, BeginOffset + begin));
			BeginOffset += EntryLength;
		}
		return ret;
	});
}

bool Smbios::Type40::AdditionInfEntries(std::vector<Smbios::AdditionalInformationEntry> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::NumofAdditionInfEntries, Type40Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector<Smbios::AdditionalInformationEntry> ret;
		uint32_t BeginOffset = ::AdditionInfEntries;
		for (auto index = 0; index < length; ++index)
		{
			Smbios::AdditionalInformationEntry temp;
			auto EntryLength = GetSmbiosInformation<uint8_t>(*pImpl, BeginOffset + ::EntryLength);
			temp.ReferencedHandle = GetSmbiosInformation<uint16_t>(*pImpl, BeginOffset + ::ReferencedHandle);
			temp.ReferencedOffset = GetSmbiosInformation<uint8_t>(*pImpl, BeginOffset + ::ReferencedOffset);
			temp.Desc = GetSmbiosInformation<std::string>(*pImpl, BeginOffset + ::String);
			for (auto begin = ::Values; begin < EntryLength; ++begin)
				temp.Values.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, BeginOffset + begin));
			BeginOffset += EntryLength;
		}
		return ret;
	}, Str);
}