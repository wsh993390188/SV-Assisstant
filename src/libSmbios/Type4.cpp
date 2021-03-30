#include "stdafx.h"

#include "Type4.h"
#include "TypeImpl.h"

#include <sstream>
#include <iomanip>

namespace
{
	constexpr int SocketDesignation = 0x04;
	constexpr int ProcessorType = 0x05;
	constexpr int ProcessorFamily = 0x06;
	constexpr int ProcessorManufacturer = 0x07;
	constexpr int ProcessorID = 0x08;
	constexpr int ProcessorVersion = 0x10;
	constexpr int Voltage = 0x11;
	constexpr int ExternalClock = 0x12;
	constexpr int MaxSpeed = 0x14;
	constexpr int CurrentSpeed = 0x16;
	constexpr int Status = 0x18;
	constexpr int ProcessorUpgrade = 0x19;
	constexpr int L1CacheHandle = 0x1A;
	constexpr int L2CacheHandle = 0x1C;
	constexpr int L3CacheHandle = 0x1E;
	constexpr int SerialNumber = 0x20;
	constexpr int AssetTag = 0x21;
	constexpr int PartNumber = 0x22;
	constexpr int CoreCount = 0x23;
	constexpr int CoreEnable = 0x24;
	constexpr int ThreadCount = 0x25;
	constexpr int ProcessorCharacteristics = 0x26;
	constexpr int ProcessorFamily2 = 0x28;
	constexpr int CoreCount2 = 0x2A;
	constexpr int CoreEnable2 = 0x2C;
	constexpr int ThreadCount2 = 0x2E;

	const std::string ProcessorTypeStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Central Processor"),
		("Math Processor"),
		("DSP Processor"),
		("Video Processor")
	};
	const std::string ProcessorFamilyStrs[] =
	{
		{},
		(u8"Other"),
		(u8"Unknown"),
		(u8"8086"),
		(u8"80286"),
		(u8"Intel386™ processor"),
		(u8"Intel486™ processor"),
		(u8"8087"),
		(u8"80287"),
		(u8"80387"),
		(u8"80487"),
		(u8"Intel® Pentium® processor"),
		(u8"Pentium® Pro processor"),
		(u8"Pentium® II processor"),
		(u8"Pentium® processor with MMX™ technology"),
		(u8"Intel® Celeron® processor"),
		(u8"Pentium® II Xeon™ processor"),
		(u8"Pentium® III processor"),
		(u8"M1 Family"),
		(u8"M2 Family"),
		(u8"Intel® Celeron® M processor"),
		(u8"Intel® Pentium® 4 HT processor"),
		(u8""),
		(u8""),
		(u8"AMD Duron™ Processor Family"),
		(u8"K5 Family"),
		(u8"K6 Family"),
		(u8"K6-2"),
		(u8"K6-3"),
		(u8"AMD Athlon™ Processor Family"),
		(u8"AMD29000 Family"),
		(u8"K6-2+"),
		(u8"Power PC Family"),
		(u8"Power PC 601"),
		(u8"Power PC 603"),
		(u8"Power PC 603+"),
		(u8"Power PC 604"),
		(u8"Power PC 620"),
		(u8"Power PC x704"),
		(u8"Power PC 750"),
		(u8"Intel® Core™ Duo processor"),
		(u8"Intel® Core™ Duo mobile processor"),
		(u8"Intel® Core™ Solo mobile processor"),
		(u8"Intel® Atom™ processor"),
		(u8"Intel® Core™ M processor"),
		(u8"Intel(R) Core(TM) m3 processor"),
		(u8"Intel(R) Core(TM) m5 processor"),
		(u8"Intel(R) Core(TM) m7 processor"),
		(u8"Alpha Family"),
		(u8"Alpha 21064"),
		(u8"Alpha 21066"),
		(u8"Alpha 21164"),
		(u8"Alpha 21164PC"),
		(u8"Alpha 21164a"),
		(u8"Alpha 21264"),
		(u8"Alpha 21364"),
		(u8"AMD Turion™ II Ultra Dual-Core Mobile M Processor Family"),
		(u8"AMD Turion™ II Dual-Core Mobile M Processor Family"),
		(u8"AMD Athlon™ II Dual-Core M Processor Family"),
		(u8"AMD Opteron™ 6100 Series Processor"),
		(u8"AMD Opteron™ 4100 Series Processor"),
		(u8"AMD Opteron™ 6200 Series Processor"),
		(u8"AMD Opteron™ 4200 Series Processor"),
		(u8"AMD FX™ Series Processor"),
		(u8"MIPS Family"),
		(u8"MIPS R4000"),
		(u8"MIPS R4200"),
		(u8"MIPS R4400"),
		(u8"MIPS R4600"),
		(u8"MIPS R10000"),
		(u8"AMD C-Series Processor"),
		(u8"AMD E-Series Processor"),
		(u8"AMD A-Series Processor"),
		(u8"AMD G-Series Processor"),
		(u8"AMD Z-Series Processor"),
		(u8"AMD R-Series Processor"),
		(u8"AMD Opteron™ 4300 Series Processor"),
		(u8"AMD Opteron™ 6300 Series Processor"),
		(u8"AMD Opteron™ 3300 Series Processor"),
		(u8"AMD FirePro™ Series Processor"),
		(u8"SPARC Family"),
		(u8"SuperSPARC"),
		(u8"microSPARC II"),
		(u8"microSPARC IIep"),
		(u8"UltraSPARC"),
		(u8"UltraSPARC II"),
		(u8"UltraSPARC Iii"),
		(u8"UltraSPARC III"),
		(u8"UltraSPARC IIIi"),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8"68040 Family"),
		(u8"68xxx"),
		(u8"68000"),
		(u8"68010"),
		(u8"68020"),
		(u8"68030"),
		(u8"AMD Athlon(TM) X4 Quad-Core Processor Family"),
		(u8"AMD Opteron(TM) X1000 Series Processor"),
		(u8"AMD Opteron(TM) X2000 Series APU"),
		(u8"AMD Opteron(TM) A-Series Processor"),
		(u8"AMD Opteron(TM) X3000 Series APU"),
		(u8"AMD Zen Processor Family"),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8"Hobbit Family"),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8"Crusoe™ TM5000 Family"),
		(u8"Crusoe™ TM3000 Family"),
		(u8"Efficeon™ TM8000 Family"),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8"Weitek"),
		(u8""),
		(u8"Itanium™ processor"),
		(u8"AMD Athlon™ 64 Processor Family"),
		(u8"AMD Opteron™ Processor Family"),
		(u8"AMD Sempron™ Processor Family"),
		(u8"AMD Turion™ 64 Mobile Technology"),
		(u8"Dual-Core AMD Opteron™ Processor Family"),
		(u8"AMD Athlon™ 64 X2 Dual-Core Processor Family"),
		(u8"AMD Turion™ 64 X2 Mobile Technology"),
		(u8"Quad-Core AMD Opteron™ Processor Family"),
		(u8"Third-Generation AMD Opteron™ Processor Family"),
		(u8"AMD Phenom™ FX Quad-Core Processor Family"),
		(u8"AMD Phenom™ X4 Quad-Core Processor Family"),
		(u8"AMD Phenom™ X2 Quad-Core Processor Family"),
		(u8"AMD Athlon™ X2 Dual-Core Processor Family"),
		(u8"PA-RISC Family"),
		(u8"PA-RISC 8500"),
		(u8"PA-RISC 8000"),
		(u8"PA-RISC 7300LC"),
		(u8"PA-RISC 7200"),
		(u8"PA-RISC 7100LC"),
		(u8"PA-RISC 7100"),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8"V30 Family"),
		(u8"Quad-Core Intel® Xeon® processor 3200 Series"),
		(u8"Dual-Core Intel® Xeon® processor 3000 Series"),
		(u8"Quad-Core Intel® Xeon® processor 5300 Series"),
		(u8"Dual-Core Intel® Xeon® processor 5100 Series"),
		(u8"Dual-Core Intel® Xeon® processor 5000 Series"),
		(u8"Dual-Core Intel® Xeon® processor LV"),
		(u8"Dual-Core Intel® Xeon® processor ULV"),
		(u8"Dual-Core Intel® Xeon® processor 7100 Series"),
		(u8"Quad-Core Intel® Xeon® processor 5400 Series"),
		(u8"Quad-Core Intel® Xeon® processor"),
		(u8"Dual-Core Intel® Xeon® processor 5200 Series"),
		(u8"Dual-Core Intel® Xeon® processor 7200 Series"),
		(u8"Quad-Core Intel® Xeon® processor 7300 Series"),
		(u8"Quad-Core Intel® Xeon® processor 7400 Series"),
		(u8"Multi-Core Intel® Xeon® processor 7400 Series"),
		(u8"Pentium® III Xeon™ processor"),
		(u8"Pentium® III Processor with Intel® SpeedStep™ Technology"),
		(u8"Pentium® 4 Processor"),
		(u8"Intel® Xeon® processor"),
		(u8"AS400 Family"),
		(u8"Intel® Xeon™ processor MP"),
		(u8"AMD Athlon™ XP Processor Family"),
		(u8"AMD Athlon™ MP Processor Family"),
		(u8"Intel® Itanium® 2 processor"),
		(u8"Intel® Pentium® M processor"),
		(u8"Intel® Celeron® D processor"),
		(u8"Intel® Pentium® D processor"),
		(u8"Intel® Pentium® Processor Extreme Edition"),
		(u8"Intel® Core™ Solo Processor"),
		(u8"Reserved"),
		(u8"Intel® Core™ 2 Duo Processor"),
		(u8"Intel® Core™ 2 Solo processor"),
		(u8"Intel® Core™ 2 Extreme processor"),
		(u8"Intel® Core™ 2 Quad processor"),
		(u8"Intel® Core™ 2 Extreme mobile processor"),
		(u8"Intel® Core™ 2 Duo mobile processor"),
		(u8"Intel® Core™ 2 Solo mobile processor"),
		(u8"Intel® Core™ i7 processor"),
		(u8"Dual-Core Intel® Celeron® processor"),
		(u8"IBM390 Family"),
		(u8"G4"),
		(u8"G5"),
		(u8"ESA/390 G6"),
		(u8"z/Architecture base"),
		(u8"Intel® Core™ i5 processor"),
		(u8"Intel® Core™ i3 processor"),
		(u8""),
		(u8""),
		(u8""),
		(u8"VIA C7™-M Processor Family"),
		(u8"VIA C7™-D Processor Family"),
		(u8"VIA C7™ Processor Family"),
		(u8"VIA Eden™ Processor Family"),
		(u8"Multi-Core Intel® Xeon® processor"),
		(u8"Dual-Core Intel® Xeon® processor 3xxx Series"),
		(u8"Quad-Core Intel® Xeon® processor 3xxx Series"),
		(u8"VIA Nano™ Processor Family"),
		(u8"Dual-Core Intel® Xeon® processor 5xxx Series"),
		(u8"Quad-Core Intel® Xeon® processor 5xxx Series"),
		(u8""),
		(u8"Dual-Core Intel® Xeon® processor 7xxx Series"),
		(u8"Quad-Core Intel® Xeon® processor 7xxx Series"),
		(u8"Multi-Core Intel® Xeon® processor 7xxx Series"),
		(u8"Multi-Core Intel® Xeon® processor 3400 Series"),
		(u8""),
		(u8""),
		(u8""),
		(u8"AMD Opteron™ 3000 Series Processor"),
		(u8"AMD Sempron™ II Processor"),
		(u8"Embedded AMD Opteron™ Quad-Core Processor Family"),
		(u8"AMD Phenom™ Triple-Core Processor Family"),
		(u8"AMD Turion™ Ultra Dual-Core Mobile Processor Family"),
		(u8"AMD Turion™ Dual-Core Mobile Processor Family"),
		(u8"AMD Athlon™ Dual-Core Processor Family"),
		(u8"AMD Sempron™ SI Processor Family"),
		(u8"AMD Phenom™ II Processor Family"),
		(u8"AMD Athlon™ II Processor Family"),
		(u8"Six-Core AMD Opteron™ Processor Family"),
		(u8"AMD Sempron™ M Processor Family"),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8""),
		(u8"i860"),
		(u8"i960"),
		(u8""),
		(u8""),
		(u8"Indicator to obtain the processor family from the Processor Family 2 field"),
		(u8"Reserved"),
	};

	const std::string ProcessorSocketStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Daughter Board"),
		("ZIF Socket"),
		("Replaceable Piggy Back"),
		("None"),
		("LIF Socket"),
		("Slot 1"),
		("Slot 2"),
		("370-pin socket"),
		("Slot A"),
		("Slot M"),
		("Slot 423"),
		("Socket A (Socket 462)"),
		("Socket 478"),
		("Socket 754"),
		("Socket 940"),
		("Socket 939"),
		("Socket mPGA604"),
		("Socket LGA771"),
		("Socket LGA775"),
		("Socket S1"),
		("Socket AM2"),
		("Socket F (1207)"),
		("Socket LGA1366"),
		("Socket G34"),
		("Socket AM3"),
		("Socket C32"),
		("Socket LGA1156"),
		("Socket LGA1567"),
		("Socket PGA988A"),
		("Socket BGA1288"),
		("Socket rPGA988B"),
		("Socket BGA1023"),
		("Socket BGA1224"),
		("Socket LGA1155"),
		("Socket LGA1356"),
		("Socket LGA2011"),
		("Socket FS1"),
		("Socket FS2"),
		("Socket FM1"),
		("Socket FM2"),
		("Socket LGA2011-3"),
		("Socket LGA1356-3"),
		("Socket LGA1150"),
		("Socket BGA1168"),
		("Socket BGA1234"),
		("Socket BGA1364"),
		("Socket AM4"),
		("Socket LGA1151"),
		("Socket BGA1356"),
		("Socket BGA1440"),
		("Socket BGA1515"),
		("Socket LGA3647-1"),
		("Socket SP3"),
		("Socket SP3r2"),
		"Socket LGA2066",
		"Socket BGA1392",
		"Socket BGA1510",
		"Socket BGA1528" /* 0x3C */
	};
}

namespace
{
	class Type4Impl : public Smbios::TypeImpl
	{
		Type4Impl(const Type4Impl&) = delete;
		Type4Impl& operator=(const Type4Impl&) = delete;
	public:
		Type4Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(SocketDesignation),std::make_pair(2,0)},
				{GetSmbiosType(Manufacturer),std::make_pair(2,0)},
				{GetSmbiosType(Version),std::make_pair(2,0)},
				{GetSmbiosType(ProcessorType),std::make_pair(2,0)},
				{GetSmbiosType(ProcessorID),std::make_pair(2,0)},
				{GetSmbiosType(ProcessorFamily),std::make_pair(2,0)},
				{GetSmbiosType(ProcessorVersion),std::make_pair(2,0)},
				{GetSmbiosType(Voltage),std::make_pair(2,0)},
				{GetSmbiosType(ExtClock),std::make_pair(2,0)},
				{GetSmbiosType(MaxSpeed),std::make_pair(2,0)},
				{GetSmbiosType(CurrentSpeed),std::make_pair(2,0)},
				{GetSmbiosType(Status),std::make_pair(2,0)},
				{GetSmbiosType(ProcessorUpgrade),std::make_pair(2,0)},
				{GetSmbiosType(L1CacheHandle),std::make_pair(2,1)},
				{GetSmbiosType(L2CacheHandle),std::make_pair(2,1)},
				{GetSmbiosType(L3CacheHandle),std::make_pair(2,1)},
				{GetSmbiosType(SerialNumber),std::make_pair(2,3)},
				{GetSmbiosType(PartNumber),std::make_pair(2,3)},
				{GetSmbiosType(AssetTag),std::make_pair(2,3)},
				{GetSmbiosType(CoreCount),std::make_pair(2,5)},
				{GetSmbiosType(CoreEnable),std::make_pair(2,5)},
				{GetSmbiosType(ThreadCount),std::make_pair(2,5)},
				{GetSmbiosType(ProcessorCharacteristics),std::make_pair(2,5)},
				{GetSmbiosType(ProcessorFamily2),std::make_pair(2,6)},
				{GetSmbiosType(CoreCount2),std::make_pair(2,6)},
				{GetSmbiosType(CoreEnable2),std::make_pair(2,6)},
				{GetSmbiosType(ThreadCount2),std::make_pair(2,6)}
			};
		}
		~Type4Impl() = default;
		TypeImpl& Create(const int32_t& TableNum)
		{
			SwitchTable(TableNum);
			return *this;
		}

		std::string GetType()
		{
			return "Type4";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(4, TableNum);
		}
	};
}

Smbios::Type4::Type4(const int32_t& TableNum) : pImpl(std::make_unique<Type4Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type4::Type4(void* PhysicalAddress) : pImpl(std::make_unique<Type4Impl>())
{
	pImpl->Create(PhysicalAddress);
}

Smbios::Type4::~Type4() = default;

bool Smbios::Type4::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type4::Type4(Type4&&) = default;

Smbios::Type4& Smbios::Type4::operator=(Type4&&) = default;

std::string Smbios::Type4::SocketDesignation()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::SocketDesignation, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::SocketDesignation(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::SocketDesignation, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type4::ProcessorType()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::ProcessorType, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::ProcessorType(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::ProcessorType, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type4::ProcessorFamily()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>(*pImpl, ::ProcessorFamily, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint8_t& ret) -> std::string {
		if (ret < _countof(ProcessorFamilyStrs))
		{
			return ProcessorFamilyStrs[ret];
		}
		else
		{
			return {};
		}
	});
}

bool Smbios::Type4::ProcessorFamily(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>(*pImpl, ::ProcessorFamily, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint8_t& ret) -> std::string {
		if (ret < _countof(ProcessorFamilyStrs))
		{
			return ProcessorFamilyStrs[ret];
		}
		else
		{
			return {};
		}
	}, Str);
}

std::string Smbios::Type4::Manufacturer()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::ProcessorManufacturer, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::Manufacturer(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::ProcessorManufacturer, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type4::ProcessorID()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint64_t, std::string>(*pImpl, ::ProcessorID, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint64_t& ret) -> std::string
	{
		std::ostringstream oss;
		oss << std::hex << std::setw(16) << std::setfill('0') << ret;
		return oss.str();
	});
}

bool Smbios::Type4::ProcessorID(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint64_t>(*pImpl, ::ProcessorID, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint64_t& ret) -> std::string
	{
		std::ostringstream oss;
		oss << std::hex << std::setw(16) << std::setfill('0') << ret;
		return oss.str();
	}, Str);
}

std::string Smbios::Type4::ProcessorVersion()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::ProcessorVersion, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::ProcessorVersion(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::ProcessorVersion, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type4::Voltage()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>(*pImpl, ::Voltage, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint8_t& numberic) -> std::string
	{
		if (numberic & 0x80)
			return std::to_string(double((numberic & 0x7F) / 10.0)) + "V";
		else
		{
			if (numberic & 0x01)
			{
				return "5V";
			}
			else if (numberic & 0x02)
				return "3.3V";
			else if (numberic & 0x4)
				return "2.9V";
			return {};
		}
	});
}

bool Smbios::Type4::Voltage(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>(*pImpl, ::Voltage, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint8_t& numberic) -> std::string
	{
		if (numberic & 0x80)
			return std::to_string(double((numberic & 0x7F) / 10.0)) + "V";
		else
		{
			if (numberic & 0x01)
			{
				return "5V";
			}
			else if (numberic & 0x02)
				return "3.3V";
			else if (numberic & 0x4)
				return "2.9V";
			return {};
		}
	}, Str);
}

std::string Smbios::Type4::ExtClock()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, std::string>(*pImpl, ::ExternalClock, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& numberic) -> std::string
	{
		if (numberic)
			return std::to_string(numberic) + "Mhz";
		else
		{
			return {};
		}
	});
}

bool Smbios::Type4::ExtClock(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t, std::string>(*pImpl, ::ExternalClock, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& numberic) -> std::string
	{
		if (numberic)
			return std::to_string(numberic) + "Mhz";
		else
		{
			return {};
		}
	}, Str);
}

std::string Smbios::Type4::MaxSpeed()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, std::string>(*pImpl, ::MaxSpeed, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& numberic) -> std::string
	{
		if (numberic)
			return std::to_string(numberic) + "Mhz";
		else
		{
			return {};
		}
	});
}

bool Smbios::Type4::MaxSpeed(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t, std::string>(*pImpl, ::MaxSpeed, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& numberic) -> std::string
	{
		if (numberic)
			return std::to_string(numberic) + "Mhz";
		else
		{
			return {};
		}
	}, Str);
}

std::string Smbios::Type4::CurrentSpeed()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, std::string>(*pImpl, ::CurrentSpeed, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& numberic) -> std::string
	{
		if (numberic)
			return std::to_string(numberic) + "Mhz";
		else
		{
			return {};
		}
	});
}

bool Smbios::Type4::CurrentSpeed(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t, std::string>(*pImpl, ::CurrentSpeed, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& numberic) -> std::string
	{
		if (numberic)
			return std::to_string(numberic) + "Mhz";
		else
		{
			return {};
		}
	}, Str);
}

uint8_t Smbios::Type4::Status()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>(*pImpl, ::Status, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::Status(uint8_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::Status, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type4::ProcessorUpgrade()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>(*pImpl, ::ProcessorUpgrade, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint8_t& ret) -> std::string {
		if (ret < _countof(ProcessorSocketStrs))
		{
			return ProcessorSocketStrs[ret];
		}
		else
		{
			return {};
		}
	});
}

bool Smbios::Type4::ProcessorUpgrade(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>(*pImpl, ::ProcessorUpgrade, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint8_t& ret) -> std::string {
		if (ret < _countof(ProcessorSocketStrs))
		{
			return ProcessorSocketStrs[ret];
		}
		else
		{
			return {};
		}
	}, Str);
}

uint16_t Smbios::Type4::L1CacheHandle()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>(*pImpl, ::L1CacheHandle, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::L1CacheHandle(std::uint16_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::L1CacheHandle, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type4::L2CacheHandle()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>(*pImpl, ::L2CacheHandle, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::L2CacheHandle(std::uint16_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::L2CacheHandle, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type4::L3CacheHandle()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>(*pImpl, ::L3CacheHandle, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::L3CacheHandle(std::uint16_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::L3CacheHandle, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type4::SerialNumber()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::SerialNumber, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::SerialNumber(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::SerialNumber, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type4::PartNumber()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::PartNumber, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::PartNumber(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::PartNumber, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type4::AssetTag()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::AssetTag, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::AssetTag(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::AssetTag, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type4::CoreCount()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>(*pImpl, ::CoreCount, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::CoreCount(std::uint8_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::CoreCount, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type4::CoreEnable()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>(*pImpl, ::CoreEnable, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::CoreEnable(std::uint8_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::CoreEnable, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type4::ThreadCount()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>(*pImpl, ::ThreadCount, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::ThreadCount(std::uint8_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::ThreadCount, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type4::ProcessorCharacteristics()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>(*pImpl, ::ProcessorCharacteristics, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::ProcessorCharacteristics(std::uint16_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::ProcessorCharacteristics, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}
std::string Smbios::Type4::ProcessorFamily2()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, std::string>(*pImpl, ::ProcessorFamily2, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& ret) -> std::string {
		if (ret < _countof(ProcessorFamilyStrs))
		{
			return ProcessorFamilyStrs[ret];
		}
		else
		{
			return {};
		}
	});
}

bool Smbios::Type4::ProcessorFamily2(std::string & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>(*pImpl, ::ProcessorFamily2, Type4Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& ret) -> std::string {
		if (ret < _countof(ProcessorFamilyStrs))
		{
			return ProcessorFamilyStrs[ret];
		}
		else
		{
			return {};
		}
	}, Str);
}

uint16_t Smbios::Type4::CoreCount2()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>(*pImpl, ::CoreCount2, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::CoreCount2(std::uint16_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::CoreCount2, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type4::CoreEnable2()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>(*pImpl, ::CoreEnable2, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::CoreEnable2(std::uint16_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::CoreEnable2, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type4::ThreadCount2()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>(*pImpl, ::ThreadCount2, Type4Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type4::ThreadCount2(std::uint16_t & Str)
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::ThreadCount2, Type4Impl::GetFuncRealName(__FUNCTION__), Str);
}