#include "stdafx.h"
#include "Type44.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Length = 0x01;
	constexpr auto ReferencedHandle = 0x04;
	constexpr auto ProcessorSpecificBlock = 0x06;

	constexpr auto BlockLength = 0x00;
	constexpr auto ProcessorType = 0x01;
	constexpr auto ProcessorSpecificData = 0x02;

	const std::string ProcessorArchitectureTypesStrs[] =
	{
		u8"Reserved",
		u8"IA32(x86)",
		u8"x64(x86 - 64, Intel64, AMD64, EM64T)",
		u8"Intel® Itanium® architecture",
		u8"32 - bit ARM(Aarch32)",
		u8"64 - bit ARM(Aarch64)",
		u8"32 - bit RISC - V(RV32)",
		u8"64 - bit RISC - V(RV64)",
		u8"128 - bit RISC - V(RV128)"
	};
}

namespace
{
	class Type44Impl : public Smbios::TypeImpl
	{
	public:
		Type44Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(ReferencedHandle),std::make_pair(0,0)},
				{GetSmbiosType(ProcessorSpecificBlock),std::make_pair(0,0)}
			};
		}
		~Type44Impl() = default;
		Type44Impl(const Type44Impl&) = delete;
		Type44Impl& operator=(const Type44Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type44";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(44, TableNum);
		}
	};
}

Smbios::Type44::Type44(const int32_t& TableNum) : pImpl(std::make_unique<Type44Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type44::Type44(void* PhysicalAddress) : pImpl(std::make_unique<Type44Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type44::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type44::~Type44() = default;

Smbios::Type44::Type44(Type44&&) = default;

Smbios::Type44& Smbios::Type44::operator=(Type44&&) = default;

uint16_t Smbios::Type44::ReferencedHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::ReferencedHandle, Type44Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type44::ReferencedHandle(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ReferencedHandle, Type44Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::vector < Smbios::ProcessorSpecificField> Smbios::Type44::ProcessorSpecificBlock() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::vector < Smbios::ProcessorSpecificField>>
		(*pImpl, ::Length, Type44Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector < Smbios::ProcessorSpecificField> ret;
		for (auto begin = ::ProcessorSpecificBlock; begin < length;)
		{
			auto BlockLength = GetSmbiosInformation<uint8_t>(*pImpl, begin + ::BlockLength);
			if (BlockLength == 0)
				break;
			Smbios::ProcessorSpecificField temp;
			auto TypeNum = GetSmbiosInformation<uint8_t>(*pImpl, begin + ::ProcessorType);
			if (TypeNum < _countof(ProcessorArchitectureTypesStrs))
			{
				temp.Type = ProcessorArchitectureTypesStrs[TypeNum];
			}
			else
			{
				temp.Type = {};
			}

			for (auto i = ::ProcessorSpecificData; i < BlockLength; ++i)
			{
				temp.Values.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, begin + ::ProcessorSpecificData + i));
			}
			begin += BlockLength;
			ret.emplace_back(std::move(temp));
		}
		return ret;
	});
}

bool Smbios::Type44::ProcessorSpecificBlock(std::vector < Smbios::ProcessorSpecificField> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::Length, Type44Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector < Smbios::ProcessorSpecificField> ret;
		for (auto begin = ::ProcessorSpecificBlock; begin < length;)
		{
			auto BlockLength = GetSmbiosInformation<uint8_t>(*pImpl, begin + ::BlockLength);
			if (BlockLength == 0)
				break;
			Smbios::ProcessorSpecificField temp;
			auto TypeNum = GetSmbiosInformation<uint8_t>(*pImpl, begin + ::ProcessorType);
			if (TypeNum < _countof(ProcessorArchitectureTypesStrs))
			{
				temp.Type = ProcessorArchitectureTypesStrs[TypeNum];
			}
			else
			{
				temp.Type = {};
			}

			for (auto i = ::ProcessorSpecificData; i < BlockLength; ++i)
			{
				temp.Values.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, begin + ::ProcessorSpecificData + i));
			}
			begin += BlockLength;
			ret.emplace_back(std::move(temp));
		}
		return ret;
	}, Str);
}