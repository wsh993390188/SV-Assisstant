#include "stdafx.h"
#include "Type9.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto SlotDesignation = 0x04;
	constexpr auto SlotType = 0x05;
	constexpr auto SlotDataBusWidth = 0x06;
	constexpr auto CurrentUsage = 0x07;
	constexpr auto SlotLength = 0x08;
	constexpr auto SlotID = 0x09;
	constexpr auto SlotCharacteristics1 = 0x0B;
	constexpr auto SlotCharacteristics2 = 0x0C;
	constexpr auto SegmentGroupNum = 0x0D;
	constexpr auto BusNumber = 0x0F;
	constexpr auto DevFuncNum = 0x10;
	constexpr auto DataBusWidth = 0x11;
	constexpr auto PeerGroupingCount = 0x12;
	constexpr auto PeerGroups = 0x13;

	//0-30h
	const std::string SystemSlotsTypefirst[] =
	{
		{},
		("Other"),
		("Unknown"),
		("ISA"),
		("MCA"),
		("EISA"),
		("PCI"),
		("PC Card (PCMCIA)"),
		("VL-VESA"),
		("Proprietary"),
		("Processor Card Slot"),
		("Proprietary Memory Card Slot"),
		("I/O Riser Card Slot"),
		("NuBus"),
		("PCI ¨C 66MHz Capable"),
		("AGP"),
		("AGP 2X"),
		("AGP 4X"),
		("PCI-X"),
		("AGP 8X"),
		("M.2 Socket 1-DP (Mechanical Key A)"),
		("M.2 Socket 1-SD (Mechanical Key E)"),
		("M.2 Socket 2 (Mechanical Key B)"),
		("M.2 Socket 3 (Mechanical Key M)"),
		("MXM Type I"),
		("MXM Type II"),
		("MXM Type III (standard connector)"),
		("MXM Type III (HE connector)"),
		("MXM Type IV"),
		("MXM 3.0 Type A"),
		("MXM 3.0 Type B"),
		("PCI Express Gen 2 SFF-8639"),
		("PCI Express Gen 3 SFF-8639"),
		("PCI Express Mini 52-pin (CEM spec. 2.0) with bottom - side keep - outs.Use Slot Length field value 03h(short length) for \"half-Mini card\" - only support, 04h(long length) for \"full-Mini card\" or dual support."),
		("PCI Express Mini 52-pin (CEM spec. 2.0) without	bottom - side keep - outs.Use Slot Length field value 03h(short length) for \"half-Mini card\" - only support, 04h(long length) for \"full-Mini card\" or dual support."),
		("PCI Express Mini 76-pin (CEM spec. 2.0) Corresponds to Display - Mini card.")
	};

	//A0-BD
	const std::string SystemSlotsTypesecond[] =
	{
		("PC-98/C20"),
		("PC-98/C24"),
		("PC-98/E"),
		("PC-98/Local Bus"),
		("PC-98/Card"),
		("PCI Express (see below)"),
		("PCI Express x1"),
		("PCI Express x2"),
		("PCI Express x4"),
		("PCI Express x8"),
		("PCI Express x16"),
		("PCI Express Gen 2 (see below)"),
		("PCI Express Gen 2 x1"),
		("PCI Express Gen 2 x2"),
		("PCI Express Gen 2 x4"),
		("PCI Express Gen 2 x8"),
		("PCI Express Gen 2 x16"),
		("PCI Express Gen 3 (see below)"),
		("PCI Express Gen 3 x1"),
		("PCI Express Gen 3 x2"),
		("PCI Express Gen 3 x4"),
		("PCI Express Gen 3 x8"),
		("PCI Express Gen 3 x16")
	};

	const std::string SystemSlotDataBusWidth[] =
	{
		{},
		("Other"),
		("Unknown"),
		("8 bit"),
		("16 bit"),
		("32 bit"),
		("64 bit"),
		("128 bit"),
		("1x or x1"),
		("2x or x2"),
		("4x or x4"),
		("8x or x8"),
		("12x or x12"),
		("16x or x16"),
		("32x or x32")
	};

	const std::string SystemSlotCurrentUsage[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Available"),
		("In use"),
		"Unavailable"
	};

	const std::string SystemSlotLength[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Short Length"),
		("Long Length"),
		("2.5\" drive form factor"),
		("3.5\" drive form factor")
	};
#pragma pack(push,1)
	struct NumbericConvertHelp
	{
		union
		{
			struct
			{
				uint8_t LowNunber8_1;
				uint8_t LowNunber8_2;
				uint8_t LowNunber8_3;
				uint8_t LowNunber8_4;
				uint8_t HighNunber8_1;
				uint8_t HighNunber8_2;
				uint8_t HighNunber8_3;
				uint8_t HighNunber8_4;
			};

			struct
			{
				uint16_t LowNunber16_1;
				uint16_t LowNunber16_2;
				uint16_t HighNunber16_1;
				uint16_t HighNunber16_2;
			};

			struct
			{
				uint32_t LowNumber32;
				uint32_t HighNumber32;
			};
			uint64_t Number;
		};
	};
#pragma pack(pop)
}

namespace
{
	class Type9Impl : public Smbios::TypeImpl
	{
	public:
		Type9Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(SlotDesignation),std::make_pair(2,0)},
				{GetSmbiosType(SlotType),std::make_pair(2,0)},
				{GetSmbiosType(SlotDataBusWidth),std::make_pair(2,0)},
				{GetSmbiosType(CurrentUsage),std::make_pair(2,0)},
				{GetSmbiosType(SlotLength),std::make_pair(2,0)},
				{GetSmbiosType(SlotID),std::make_pair(2,0)},
				{GetSmbiosType(SlotCharacteristics1),std::make_pair(2,0)},
				{GetSmbiosType(SlotCharacteristics2),std::make_pair(2,1)},
				{GetSmbiosType(SegmentGroupNum),std::make_pair(2,6)},
				{GetSmbiosType(BusNumber),std::make_pair(2,6)},
				{GetSmbiosType(DevFuncNum),std::make_pair(2,6)},
				{GetSmbiosType(DataBusWidth),std::make_pair(3,2)},
				{GetSmbiosType(PeerGroupingCount),std::make_pair(3,2)},
				{GetSmbiosType(PeerGroups),std::make_pair(3,2)}
			};
		}
		~Type9Impl() = default;
		Type9Impl(const Type9Impl&) = delete;
		Type9Impl& operator=(const Type9Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}

		std::string GetType()
		{
			return "Type9";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(9, TableNum);
		}
	};
}

Smbios::Type9::Type9(const int32_t& TableNum) : pImpl(std::make_unique<Type9Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type9::Type9(void* PhysicalAddress) : pImpl(std::make_unique<Type9Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type9::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type9::~Type9() = default;

Smbios::Type9::Type9(Type9&&) = default;

Smbios::Type9& Smbios::Type9::operator=(Type9&&) = default;

std::string Smbios::Type9::SlotDesignation() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::SlotDesignation, Type9Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type9::SlotDesignation(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SlotDesignation, Type9Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type9::SlotType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::SlotType, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(SystemSlotsTypefirst))
		{
			return SystemSlotsTypefirst[ret];
		}

		if (ret >= 0xA0)
		{
			if (ret - 0xA0 < _countof(SystemSlotsTypesecond))
				return SystemSlotsTypesecond[ret - 0xA0];
		}
		return std::string();
	});
}

bool Smbios::Type9::SlotType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::SlotType, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(SystemSlotsTypefirst))
		{
			return SystemSlotsTypefirst[ret];
		}

		if (ret >= 0xA0)
		{
			if (ret - 0xA0 < _countof(SystemSlotsTypesecond))
				return SystemSlotsTypesecond[ret - 0xA0];
		}
		return std::string();
	}, Str);
}

std::string Smbios::Type9::SlotDataBusWidth() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::SlotDataBusWidth, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(SystemSlotDataBusWidth))
		{
			return SystemSlotDataBusWidth[ret];
		}
		return std::string();
	});
}

bool Smbios::Type9::SlotDataBusWidth(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::SlotDataBusWidth, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(SystemSlotDataBusWidth))
		{
			return SystemSlotDataBusWidth[ret];
		}
		return std::string();
	}, Str);
}

std::string Smbios::Type9::CurrentUsage() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::CurrentUsage, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(SystemSlotCurrentUsage))
		{
			return SystemSlotCurrentUsage[ret];
		}
		return std::string();
	});
}

bool Smbios::Type9::CurrentUsage(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::CurrentUsage, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(SystemSlotCurrentUsage))
		{
			return SystemSlotCurrentUsage[ret];
		}
		return std::string();
	}, Str);
}

std::string Smbios::Type9::SlotLength() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::SlotLength, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(SystemSlotLength))
		{
			return SystemSlotLength[ret];
		}
		return std::string();
	});
}

bool Smbios::Type9::SlotLength(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::SlotLength, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(SystemSlotLength))
		{
			return SystemSlotLength[ret];
		}
		return std::string();
	}, Str);
}

std::uint16_t Smbios::Type9::SlotID() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::SlotID, Type9Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type9::SlotID(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SlotID, Type9Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::SlotCharacteristics1Field Smbios::Type9::SlotCharacteristics1() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::SlotCharacteristics1Field>
		(*pImpl, ::SlotCharacteristics1, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::SlotCharacteristics1Field>(ret);
	});
}

bool Smbios::Type9::SlotCharacteristics1(Smbios::SlotCharacteristics1Field & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::SlotCharacteristics1, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::SlotCharacteristics1Field>(ret);
	}, Str);
}

Smbios::SlotCharacteristics2Field Smbios::Type9::SlotCharacteristics2() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::SlotCharacteristics2Field>
		(*pImpl, ::SlotCharacteristics2, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::SlotCharacteristics2Field>(ret);
	});
}

bool Smbios::Type9::SlotCharacteristics2(Smbios::SlotCharacteristics2Field & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::SlotCharacteristics2, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::SlotCharacteristics2Field>(ret);
	}, Str);
}

std::uint16_t Smbios::Type9::SegmentGroupNum() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::SegmentGroupNum, Type9Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type9::SegmentGroupNum(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SegmentGroupNum, Type9Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type9::BusNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::BusNumber, Type9Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type9::BusNumber(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::BusNumber, Type9Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::DeviceFuncField Smbios::Type9::DevFuncNum() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::DeviceFuncField>
		(*pImpl, ::DevFuncNum, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::DeviceFuncField>(ret);
	});
}

bool Smbios::Type9::DevFuncNum(Smbios::DeviceFuncField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::DevFuncNum, Type9Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::DeviceFuncField>(ret);
	}, Str);
}

std::uint8_t Smbios::Type9::DataBusWidth() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::DataBusWidth, Type9Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type9::DataBusWidth(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DataBusWidth, Type9Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type9::PeerGroupingCount() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::PeerGroupingCount, Type9Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type9::PeerGroupingCount(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::PeerGroupingCount, Type9Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::vector<uint64_t> Smbios::Type9::PeerGroups() const
{
	auto Number = PeerGroupingCount();
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::vector<uint64_t> >
		(*pImpl, ::PeerGroups, Type9Impl::GetFuncRealName(__FUNCTION__),
			[this, Number](const uint8_t&)
	{
		std::vector<uint64_t> ret;
		for (auto i = 0; i < Number; i += 5)
		{
			NumbericConvertHelp NumberHelp;
			NumberHelp.LowNumber32 = GetSmbiosInformation<uint32_t>(*pImpl, ::PeerGroups + i);
			NumberHelp.HighNunber8_1 = GetSmbiosInformation<uint8_t>(*pImpl, ::PeerGroups + i + 4);
			ret.emplace_back(NumberHelp.Number);
		}
		return ret;
	});
}

bool Smbios::Type9::PeerGroups(std::vector<uint64_t> & Str) const noexcept
{
	uint8_t Number{};
	if (!PeerGroupingCount(Number))
	{
		return false;
	}
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::PeerGroups, Type9Impl::GetFuncRealName(__FUNCTION__),
			[this, Number](const uint8_t&)
	{
		std::vector<uint64_t> ret;
		for (auto i = 0; i < Number; i += 5)
		{
			NumbericConvertHelp NumberHelp;
			NumberHelp.LowNumber32 = GetSmbiosInformation<uint32_t>(*pImpl, ::PeerGroups + i);
			NumberHelp.HighNunber8_1 = GetSmbiosInformation<uint8_t>(*pImpl, ::PeerGroups + i + 4);
			ret.emplace_back(NumberHelp.Number);
		}
		return ret;
	}, Str);
}