#include "stdafx.h"
#include "Type15.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto LogAreaLength = 0x04;
	constexpr auto LogHeaderStartOffset = 0x06;
	constexpr auto LogDataStartOffset = 0x08;
	constexpr auto AccessMethod = 0x0A;
	constexpr auto LogStatus = 0x0B;
	constexpr auto LogChangeToken = 0x0C;
	constexpr auto AccessMethodAddress = 0x10;
	constexpr auto LogHeaderFormat = 0x14;
	constexpr auto NumofSupportedLogTypes = 0x15;
	constexpr auto LengthLogTypeDescriptor = 0x16;
	constexpr auto ListofSupportedEventLogTypeDescriptors = 0x17;

	const std::string AccesssMethodStrs[] =
	{
		{"Indexed I/O: 1 8-bit index port, 1 8-bit data port"},
		{"Indexed I/O: 2 8-bit index ports, 1 8-bit data port"},
		{"Indexed I/O: 1 16-bit index port, 1 8-bit data port"},
		{"Memory-mapped physical 32-bit address"},
		{"Available through General-Purpose NonVolatile Data functions"}
	};

	const std::string LogHeaderFormatStrs[] =
	{
		{"No header"},
		{"Type 1 log header"}
	};

	const std::string EventLogTypesStrs[] =
	{
		"Reversed",
		"Single - bit ECC memory error",
		"Multi - bit ECC memory error",
		"Parity memory error",
		"Bus time - out",
		"I / O Channel Check",
		"Software NMI",
		"POST Memory Resize",
		"POST Error",
		"PCI Parity Error",
		"PCI System Error",
		"CPU Failure",
		"EISA FailSafe Timer time - out",
		"Correctable memory log disabled",
		"Logging disabled for a specific Event Type ¡ª too many errors of the same type received in a short amount of time",
		"Reserved",
		"System Limit Exceeded(for example, voltage or temperature threshold exceeded)",
		"Asynchronous hardware timer expired and issued a system reset",
		"System configuration information",
		"Hard - disk information",
		"System reconfigured",
		"Uncorrectable CPU - complex error",
		"Log Area Reset / Cleared",
		"System boot"
	};

	const std::string VariableDataFormatTypesStrs[] =
	{
		"None",
		"Handle",
		"Multiple-Event",
		"Multiple-Event Handle",
		"POST Results Bitmap",
		"System Management Type",
		"Multiple-Event System Management Type"
	};

#pragma pack(push,1)
	struct Uint16toUint8Helper
	{
		union
		{
			struct
			{
				uint8_t Low;
				uint8_t High;
			};
			uint16_t Number;
		};
		Uint16toUint8Helper(const uint16_t& Num) : Number(Num) {}
	};
#pragma pack(pop)
}

namespace
{
	class Type15Impl : public Smbios::TypeImpl
	{
	public:
		Type15Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(LogAreaLength),std::make_pair(2,0)},
				{GetSmbiosType(LogHeaderStartOffset),std::make_pair(2,0)},
				{GetSmbiosType(LogDataStartOffset),std::make_pair(2,0)},
				{GetSmbiosType(AccessMethod),std::make_pair(2,0)},
				{GetSmbiosType(LogStatus),std::make_pair(2,0)},
				{GetSmbiosType(LogChangeToken),std::make_pair(2,0)},
				{GetSmbiosType(AccessMethodAddress),std::make_pair(2,0)},
				{GetSmbiosType(LogHeaderFormat),std::make_pair(2,1)},
				{GetSmbiosType(NumofSupportedLogTypes),std::make_pair(2,1)},
				{GetSmbiosType(LengthLogTypeDescriptor),std::make_pair(2,1)},
				{GetSmbiosType(ListofSupportedEventLogTypeDescriptors),std::make_pair(2,1)}
			};
		}
		~Type15Impl() = default;
		Type15Impl(const Type15Impl&) = delete;
		Type15Impl& operator=(const Type15Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum)
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type15";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(15, TableNum);
		}
	};
}

Smbios::Type15::Type15(const int32_t& TableNum) : pImpl(std::make_unique<Type15Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type15::Type15(void* PhysicalAddress) : pImpl(std::make_unique<Type15Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type15::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type15::~Type15() = default;

Smbios::Type15::Type15(Type15&&) = default;

Smbios::Type15& Smbios::Type15::operator=(Type15&&) = default;

uint16_t Smbios::Type15::LogAreaLength() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::LogAreaLength, Type15Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type15::LogAreaLength(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LogAreaLength, Type15Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type15::LogHeaderStartOffset() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::LogHeaderStartOffset, Type15Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type15::LogHeaderStartOffset(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LogHeaderStartOffset, Type15Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type15::LogDataStartOffset() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::LogDataStartOffset, Type15Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type15::LogDataStartOffset(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LogDataStartOffset, Type15Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type15::AccessMethod() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::AccessMethod, Type15Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(AccesssMethodStrs))
		{
			return AccesssMethodStrs[num];
		}
		else if (num < 0x80)
		{
			return std::string("Available for future assignment by this specification");
		}
		else
		{
			return std::string("BIOS Vendor/OEM-specific");
		}
	});
}

bool Smbios::Type15::AccessMethod(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::AccessMethod, Type15Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(AccesssMethodStrs))
		{
			return AccesssMethodStrs[num];
		}
		else if (num < 0x80)
		{
			return std::string("Available for future assignment by this specification");
		}
		else
		{
			return std::string("BIOS Vendor/OEM-specific");
		}
	}, Str);
}

Smbios::LogStatusField Smbios::Type15::LogStatus() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::LogStatusField>
		(*pImpl, ::LogStatus, Type15Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		return static_cast<Smbios::LogStatusField>(num);
	});
}

bool Smbios::Type15::LogStatus(Smbios::LogStatusField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::LogStatus, Type15Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		return static_cast<Smbios::LogStatusField>(num);
	}, Str);
}

uint32_t Smbios::Type15::LogChangeToken() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::LogChangeToken, Type15Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type15::LogChangeToken(uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LogChangeToken, Type15Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::AccessMethodAddressField Smbios::Type15::AccessMethodAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t, Smbios::AccessMethodAddressField>
		(*pImpl, ::AccessMethodAddress, Type15Impl::GetFuncRealName(__FUNCTION__),
			[](const uint32_t& num)
	{
		return static_cast<Smbios::AccessMethodAddressField>(num);
	});
}

bool Smbios::Type15::AccessMethodAddress(Smbios::AccessMethodAddressField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint32_t>
		(*pImpl, ::AccessMethodAddress, Type15Impl::GetFuncRealName(__FUNCTION__),
			[](const uint32_t& num)
	{
		return static_cast<Smbios::AccessMethodAddressField>(num);
	}, Str);
}

std::string Smbios::Type15::LogHeaderFormat() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::LogHeaderFormat, Type15Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(LogHeaderFormatStrs))
		{
			return LogHeaderFormatStrs[num];
		}
		else if (num < 0x80)
		{
			return std::string("Available for future assignment by this specification");
		}
		else
		{
			return std::string("BIOS vendor or OEM-specific format");
		}
	});
}

bool Smbios::Type15::LogHeaderFormat(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::LogHeaderFormat, Type15Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(LogHeaderFormatStrs))
		{
			return LogHeaderFormatStrs[num];
		}
		else if (num < 0x80)
		{
			return std::string("Available for future assignment by this specification");
		}
		else
		{
			return std::string("BIOS vendor or OEM-specific format");
		}
	}, Str);
}

std::uint8_t Smbios::Type15::NumofSupportedLogTypes() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::NumofSupportedLogTypes, Type15Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type15::NumofSupportedLogTypes(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NumofSupportedLogTypes, Type15Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type15::LengthLogTypeDescriptor() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::LengthLogTypeDescriptor, Type15Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type15::LengthLogTypeDescriptor(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LengthLogTypeDescriptor, Type15Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::vector<Smbios::EventLogType> Smbios::Type15::ListofSupportedEventLogTypeDescriptors() const
{
	auto Number = NumofSupportedLogTypes();
	auto Length = LengthLogTypeDescriptor(); // Must Be 2
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::vector<Smbios::EventLogType>>
		(*pImpl, ::ListofSupportedEventLogTypeDescriptors, Type15Impl::GetFuncRealName(__FUNCTION__),
			[Number, Length, this](const uint8_t)
	{
		std::vector<Smbios::EventLogType> ret;
		for (uint8_t index = 0; index < Number; ++index)
		{
			Smbios::EventLogType temp;
			Uint16toUint8Helper Helper = GetSmbiosInformation<uint16_t>(*pImpl, ::ListofSupportedEventLogTypeDescriptors + (index * Length));
			if (Helper.Low < _countof(EventLogTypesStrs))
			{
				temp.LogType = EventLogTypesStrs[Helper.Low];
			}
			else if (Helper.Low < 0x80)
			{
				temp.LogType = "Unused, available for assignment by this specification";
			}
			else if (Helper.Low < 0xFF)
			{
				temp.LogType = "Available for system- and OEM-specific assignments";
			}
			else
			{
				temp.LogType = "End of log";
			}

			if (Helper.High < _countof(VariableDataFormatTypesStrs))
			{
				temp.DataFormatType = VariableDataFormatTypesStrs[Helper.High];
			}
			else if (Helper.High < 0x80)
			{
				temp.DataFormatType = "Unused, available for assignment by this specification";
			}
			else
			{
				temp.DataFormatType = "Available for system- and OEM-specific assignments";
			}
			ret.emplace_back(std::move(temp));
		}
		return ret;
	});
}

bool Smbios::Type15::ListofSupportedEventLogTypeDescriptors(std::vector<Smbios::EventLogType> & Str) const noexcept
{
	uint8_t Number{}, Length{};
	if (!NumofSupportedLogTypes(Number) || !LengthLogTypeDescriptor(Length))// Must Be 2
	{
		return false;
	}

	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::ListofSupportedEventLogTypeDescriptors, Type15Impl::GetFuncRealName(__FUNCTION__),
			[Number, Length, this](const uint8_t)
	{
		std::vector<Smbios::EventLogType> ret;
		for (uint8_t Index = 0; Index < Number; ++Index)
		{
			Smbios::EventLogType temp;
			Uint16toUint8Helper Helper = GetSmbiosInformation<uint16_t>(*pImpl, ::ListofSupportedEventLogTypeDescriptors + (Index * Length));
			if (Helper.Low < _countof(EventLogTypesStrs))
			{
				temp.LogType = EventLogTypesStrs[Helper.Low];
			}
			else if (Helper.Low < 0x80)
			{
				temp.LogType = "Unused, available for assignment by this specification";
			}
			else if (Helper.Low < 0xFF)
			{
				temp.LogType = "Available for system- and OEM-specific assignments";
			}
			else
			{
				temp.LogType = "End of log";
			}

			if (Helper.High < _countof(VariableDataFormatTypesStrs))
			{
				temp.DataFormatType = VariableDataFormatTypesStrs[Helper.High];
			}
			else if (Helper.High < 0x80)
			{
				temp.DataFormatType = "Unused, available for assignment by this specification";
			}
			else
			{
				temp.DataFormatType = "Available for system- and OEM-specific assignments";
			}
			ret.emplace_back(std::move(temp));
		}
		return ret;
	}, Str);
}