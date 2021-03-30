#include "stdafx.h"
#include "Type5.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto ErrorDetectingMethod = 0x04;
	constexpr auto ErrorCorrectingCapability = 0x05;
	constexpr auto SupportedInterleave = 0x06;
	constexpr auto CurrentInterleave = 0x07;
	constexpr auto MaximumMemoryMoudulesize = 0x08;
	constexpr auto SupportedSpeeds = 0x09;
	constexpr auto SupportedMemoryTypes = 0x0B;
	constexpr auto MemoryModuleVoltage = 0x0D;
	constexpr auto NumofMemorySlots = 0x0E;
	constexpr auto MemoryModuleConfigurationHandles = 0x0F;

	const std::string ErrorDetectedMethodStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("None"),
		("8-bit Parity"),
		("32-bit ECC"),
		("64-bit ECC"),
		("128-bit ECC"),
		("CRC")
	};

	const std::string InterleaveSupportFieldStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("One-Way Interleave"),
		("Two-Way Interleave"),
		("Four-Way Interleave"),
		("Eight-Way Interleave"),
		("Sixteen-Way Interleave")
	};

	class Type5Impl : public Smbios::TypeImpl
	{
		Type5Impl(const Type5Impl&) = delete;
		Type5Impl& operator=(const Type5Impl&) = delete;
	public:
		Type5Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(ErrorDetectingMethod),std::make_pair(2,0)},
				{GetSmbiosType(ErrorCorrectingCapability),std::make_pair(2,0)},
				{GetSmbiosType(SupportedInterleave),std::make_pair(2,0)},
				{GetSmbiosType(CurrentInterleave),std::make_pair(2,0)},
				{GetSmbiosType(MaximumMemoryMoudulesize),std::make_pair(2,0)},
				{GetSmbiosType(SupportedSpeeds),std::make_pair(2,0)},
				{GetSmbiosType(SupportedMemoryTypes),std::make_pair(2,0)},
				{GetSmbiosType(MemoryModuleVoltage),std::make_pair(2,0)},
				{GetSmbiosType(NumofMemorySlots),std::make_pair(2,0)},
				{GetSmbiosType(MemoryModuleConfigurationHandles),std::make_pair(2,0)},
				{GetSmbiosType(EnabledErrorCorrectingCapabilities),std::make_pair(2,1)}
			};
		}
		~Type5Impl() = default;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type5";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(5, TableNum);
		}
	};
}

Smbios::Type5::Type5(const int32_t& TableNum) : pImpl(std::make_unique<Type5Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type5::Type5(void* PhysicalAddress) : pImpl(std::make_unique<Type5Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type5::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type5::~Type5() = default;

Smbios::Type5::Type5(Type5&&) = default;

Smbios::Type5& Smbios::Type5::operator=(Type5&&) = default;

std::string Smbios::Type5::ErrorDetectingMethod()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>(*pImpl, ::ErrorDetectingMethod, Type5Impl::GetFuncRealName(__FUNCTION__),
		[](const uint8_t& ret) ->std::string {
		if (ret < _countof(ErrorDetectedMethodStrs))
		{
			return ErrorDetectedMethodStrs[ret];
		}
		return {};
	});
}

bool Smbios::Type5::ErrorDetectingMethod(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t, std::string>
		(*pImpl, ::ErrorDetectingMethod, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) ->std::string {
		if (ret < _countof(ErrorDetectedMethodStrs))
		{
			return ErrorDetectedMethodStrs[ret];
		}
		return {};
	}, Str);
}

Smbios::ErrorCapability Smbios::Type5::ErrorCorrectingCapability()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, Smbios::ErrorCapability>
		(*pImpl, ::ErrorCorrectingCapability, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) {
		return static_cast<Smbios::ErrorCapability>(ret);
	});
}

bool Smbios::Type5::ErrorCorrectingCapability(Smbios::ErrorCapability & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t, Smbios::ErrorCapability>
		(*pImpl, ::ErrorCorrectingCapability, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) {
		return static_cast<Smbios::ErrorCapability>(ret);
	}, Str);
}

std::string Smbios::Type5::SupportedInterleave()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::SupportedInterleave, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) {
		if (ret < _countof(InterleaveSupportFieldStrs))
		{
			return InterleaveSupportFieldStrs[ret];
		}
		return std::string();
	});
}

bool Smbios::Type5::SupportedInterleave(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t, std::string>
		(*pImpl, ::SupportedInterleave, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) ->std::string {
		if (ret < _countof(InterleaveSupportFieldStrs))
		{
			return InterleaveSupportFieldStrs[ret];
		}
		return {};
	}, Str);
}

std::string Smbios::Type5::CurrentInterleave()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::CurrentInterleave, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) {
		if (ret < _countof(InterleaveSupportFieldStrs))
		{
			return InterleaveSupportFieldStrs[ret];
		}
		return std::string();
	});
}

bool Smbios::Type5::CurrentInterleave(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t, std::string>
		(*pImpl, ::CurrentInterleave, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) ->std::string {
		if (ret < _countof(InterleaveSupportFieldStrs))
		{
			return InterleaveSupportFieldStrs[ret];
		}
		return {};
	}, Str);
}

uint8_t Smbios::Type5::MaximumMemoryMoudulesize()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::MaximumMemoryMoudulesize, Type5Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type5::MaximumMemoryMoudulesize(uint8_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MaximumMemoryMoudulesize, Type5Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::MemorySpeeds Smbios::Type5::SupportedSpeeds()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, Smbios::MemorySpeeds>
		(*pImpl, ::SupportedSpeeds, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret) {
		return static_cast<Smbios::MemorySpeeds>(ret);
	});
}

bool Smbios::Type5::SupportedSpeeds(Smbios::MemorySpeeds & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>
		(*pImpl, ::SupportedSpeeds, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret) {
		return static_cast<Smbios::MemorySpeeds>(ret);
	}, Str);
}

Smbios::MemoryTypes Smbios::Type5::SupportedMemoryTypes()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, Smbios::MemoryTypes>
		(*pImpl, ::SupportedMemoryTypes, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret) {
		return static_cast<Smbios::MemoryTypes>(ret);
	});
}

bool Smbios::Type5::SupportedMemoryTypes(Smbios::MemoryTypes & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>
		(*pImpl, ::SupportedMemoryTypes, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret) {
		return static_cast<Smbios::MemoryTypes>(ret);
	}, Str);
}

Smbios::MemoryVoltages Smbios::Type5::MemoryModuleVoltage()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, Smbios::MemoryVoltages>
		(*pImpl, ::MemoryModuleVoltage, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) {
		return static_cast<Smbios::MemoryVoltages>(ret);
	});
}

bool Smbios::Type5::MemoryModuleVoltage(Smbios::MemoryVoltages & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::MemoryModuleVoltage, Type5Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret) {
		return static_cast<Smbios::MemoryVoltages>(ret);
	}, Str);
}

uint8_t Smbios::Type5::NumofMemorySlots()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::NumofMemorySlots, Type5Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type5::NumofMemorySlots(uint8_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NumofMemorySlots, Type5Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::vector<uint16_t> Smbios::Type5::MemoryModuleConfigurationHandles()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::vector<uint16_t>>
		(*pImpl, ::NumofMemorySlots, Type5Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& Number)
	{
		std::vector<uint16_t> ret;
		for (auto i = 0; i < 2 * Number - 1; i += 2)
		{
			ret.emplace_back(GetSmbiosInformation<uint16_t>(*pImpl, ::MemoryModuleConfigurationHandles + i));
		}
		return ret;
	});
}

bool Smbios::Type5::MemoryModuleConfigurationHandles(std::vector<uint16_t> & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t, std::vector<uint16_t>>
		(*pImpl, ::NumofMemorySlots, Type5Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& Number)
	{
		std::vector<uint16_t> ret;
		for (auto i = 0; i < 2 * Number - 1; i += 2)
		{
			ret.emplace_back(GetSmbiosInformation<uint16_t>(*pImpl, ::MemoryModuleConfigurationHandles + i));
		}
		return ret;
	}, Str);
}

uint8_t Smbios::Type5::EnabledErrorCorrectingCapabilities()
{
	auto Number = NumofMemorySlots();
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::MemoryModuleConfigurationHandles + 2 * Number, Type5Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type5::EnabledErrorCorrectingCapabilities(uint8_t & Str)
{
	uint8_t Number{};
	if (!NumofMemorySlots(Number))
	{
		return false;
	}
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemoryModuleConfigurationHandles + 2 * Number, Type5Impl::GetFuncRealName(__FUNCTION__), Str);
}