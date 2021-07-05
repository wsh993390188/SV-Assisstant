#include "stdafx.h"
#include "ParserSPD.h"
#include "JEDECConfig.h"

namespace
{
	size_t ipow(size_t base, size_t exp)
	{
		size_t result = 1;
		while (exp)
		{
			if (exp & 1)
			{
				result *= base;
			}
			exp >>= 1;
			base *= base;
		}
		return result;
	}

	template<class T>
	void ParserPartNumber(const T* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		OutputData.PartNumber.resize(18, 0);
		memcpy_s(OutputData.PartNumber.data(), OutputData.PartNumber.size(), SpdInfo->PartNumber, 18);
		Hardware::Utils::trim(OutputData.PartNumber);
	}

	template<class T>
	void ParserSerialNumber(const T* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		OutputData.SerialNumber.resize(9, 0);
		sprintf_s(OutputData.SerialNumber.data(), OutputData.SerialNumber.size(), "%02X%02X%02X%02X", SpdInfo->SerialNumber[0], SpdInfo->SerialNumber[1], SpdInfo->SerialNumber[2], SpdInfo->SerialNumber[3]);
		Hardware::Utils::trim(OutputData.SerialNumber);
	}

	template<class T>
	void ParseDeviceType(const T* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		switch (SpdInfo->sDRAMType)
		{
		case 0x0B:
			OutputData.DRAMType = "DDR3";
			break;
		case 0x0C:
			OutputData.DRAMType = "DDR4";
			break;
		case 0x0E:
			OutputData.DRAMType = "DDR4E";
			break;
		case 0x0F:
			OutputData.DRAMType = "LPDDR3";
			break;
		case 0x10:
			OutputData.DRAMType = "LPDDR4";
			break;
		case 0x11:
			OutputData.DRAMType = "LPDDR4X";
			break;
		case 0x12:
			OutputData.DRAMType = "DDR5";
			break;
		case 0x13:
			OutputData.DRAMType = "LPDDR5";
			break;
		default:
			break;
		}
	}

	template<class T>
	void ParseModuleType(const T* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		switch (SpdInfo->sModuleType.bits.Module_Type)
		{
		case 0b0001:
			OutputData.ModuleType = "RDIMM";
			break;
		case 0b0010:
			OutputData.ModuleType = "UDIMM";
			break;
		case 0b0011:
			OutputData.ModuleType = "SO-DIMM";
			break;
		case 0b0100:
			OutputData.ModuleType = "Micro-DIMM";
			break;
		case 0b0101:
			OutputData.ModuleType = "Mini-RDIMM";
			break;
		case 0b0110:
			OutputData.ModuleType = "Mini-UDIMM";
			break;
		case 0b1000:
			OutputData.ModuleType = "72b-SO-UDIMM";
			break;
		case 0b1001:
			OutputData.ModuleType = "72b-SO-RDIMM";
			break;
		case 0b1010:
			OutputData.ModuleType = "72b-SO-CDIMM";
			break;
		case 0b1100:
			OutputData.ModuleType = "16b-SO-CDIMM";
			break;
		case 0b1101:
			OutputData.ModuleType = "32b-SO-CDIMM";
			break;
		default:
			break;
		}
	}

	std::string ParseManufacture(const size_t BankCode, const size_t PageCode)
	{
		return Hardware::Memory::JEDECConfig::Instance().GetManufacture(BankCode, PageCode);
	}

	template<class T>
	void ParseDRAMManufacture(const T* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		auto TempStr = ParseManufacture(SpdInfo->DRAMManufacturerIDLeast & 0x7F, SpdInfo->DRAMManufacturerIDMost);
		if (!TempStr.empty())
			OutputData.DRAMManufacturer = TempStr;
	}

	template<class T>
	void ParseModuleManufacture(const T* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		auto TempStr = ParseManufacture(SpdInfo->ModuleManufacturerIDLeast & 0x7F, SpdInfo->ModuleManufacturerIDMost);
		if (!TempStr.empty())
			OutputData.ModuleManufacturer = TempStr;
	}

	template<class T>
	void ParseProduceDate(const T* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		int year = SpdInfo->ManufacturingDate[0], week = SpdInfo->ManufacturingDate[1];
		if (week != 0 && year != 0)
		{
			OutputData.ProductDate.resize(6, 0);
			sprintf_s(OutputData.ProductDate.data(), OutputData.ProductDate.size(), "%02X\\%02X", week, year);
		}
	}

	template<class T>
	void ParsePackage(const T* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		// Todo
	}

	template<class T>
	void ParseDataWidth(const T* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		auto DataWidth = 8 * ipow(2, SpdInfo->sBusWidth.bits.Primary_bus_width);
		auto ECC = SpdInfo->sBusWidth.bits.Bus_width_extension ? 8 : 0;
		OutputData.DataBits = std::to_string(DataWidth) + " bits";
	}

	template<class T>
	double GetCapacity(const T* SpdInfo)
	{
		double capacity = 0;
		switch (SpdInfo->sSDRAMBank_Density.bits.Capacity)
		{
		case 0:
			capacity = 0.25;
			break;
		case 0x01:
			capacity = 0.5;
			break;
		case 0x02:
			capacity = 1.0;
			break;
		case 0x03:
			capacity = 2.0;
			break;
		case 0x04:
			capacity = 4.0;
			break;
		case 0x05:
			capacity = 8.0;
			break;
		case 0x06:
			capacity = 16.0;
			break;
		case 0x07:
			capacity = 32.0;
			break;
		case 0x08:
			capacity = 12.0;
			break;
		case 0x09:
			capacity = 24.0;
			break;
		default:
			break;
		}
		return capacity;
	}

	template<class T>
	uint32_t GetRanks(const T* SpdInfo)
	{
		return SpdInfo->sModuleOrganization.bits.Number_of_Ranks + 1;
	}

	template<class T>
	uint32_t GetPrimarybuswidth(const T* SpdInfo)
	{
		return 8 * ipow(2, SpdInfo->sBusWidth.bits.Primary_bus_width);
	}

	template<class T>
	uint32_t GetDRAMWidth(const T* SpdInfo)
	{
		return 4 * ipow(2, SpdInfo->sModuleOrganization.bits.SDRAM_Depth_Width);
	}

	template<class T>
	uint32_t GetDDR3Banks(const T* SpdInfo)
	{
		return 1 << (SpdInfo->sSDRAMBank_Density.bits.Bank + 3);
	}

	template<class T>
	uint32_t GetDDR4Banks(const T* SpdInfo)
	{
		uint32_t BankGroup = 1;
		switch (SpdInfo->sSDRAMBank_Density.bits.BankGroupBits)
		{
		case 1:
			BankGroup = 2;
			break;
		case 2:
			BankGroup = 4;
			break;
		default:
			break;
		}
		uint32_t BankAddressBits = 0;
		switch (SpdInfo->sSDRAMBank_Density.bits.BankAddressBits)
		{
		case 0:
			BankAddressBits = 4;
			break;
		case 1:
			BankAddressBits = 8;
			break;
		default:
			break;
		}
		return BankGroup * BankAddressBits;
	}

	void CalcDIMMRB(const uint32_t Ranks, const uint32_t Banks, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		OutputData.Ranks_Banks = std::to_string(Ranks) + " Ranks " + std::to_string(Banks) + " Banks";
	}

	void CalcDIMMCapacity(const double capacity, const uint32_t Ranks, const uint32_t SDRAMWidth, const uint32_t Primarybuswidth
		, Hardware::Memory::MemoryCommonInformation& OutputData)
	{
		OutputData.ModuleSize = capacity / 8.0 * Primarybuswidth / SDRAMWidth * Ranks;
	}
}

#pragma region DDR4
void Hardware::Memory::ParserDDR4SPD::ParserFirstSPD(const DDR4_INFO_FIRST_128* SpdInfo, MemoryCommonInformation& OutputData)
{
	ParseDeviceType(SpdInfo, OutputData);
	ParseModuleType(SpdInfo, OutputData);
	ParseDataWidth(SpdInfo, OutputData);
	CalcDIMMCapacity(GetCapacity(SpdInfo), GetRanks(SpdInfo), GetDRAMWidth(SpdInfo), GetPrimarybuswidth(SpdInfo), OutputData);
	CalcDIMMRB(GetRanks(SpdInfo), GetDDR4Banks(SpdInfo), OutputData);
	CalcDIMMFrequency(SpdInfo, OutputData);
	CalcDDR4TimeBase(SpdInfo, OutputData);
}

void Hardware::Memory::ParserDDR4SPD::ParserSecondSPD(const DDR4_INFO_SECOND_128* SpdInfo, MemoryCommonInformation& OutputData)
{
}

void Hardware::Memory::ParserDDR4SPD::ParserThirdSPD(const DDR4_INFO_THIRD_128* SpdInfo, MemoryCommonInformation& OutputData)
{
	ParserPartNumber(SpdInfo, OutputData);
	ParserSerialNumber(SpdInfo, OutputData);
	ParseModuleManufacture(SpdInfo, OutputData);
	ParseDRAMManufacture(SpdInfo, OutputData);
	ParseProduceDate(SpdInfo, OutputData);
}

void Hardware::Memory::ParserDDR4SPD::ParserFourthSPD(const DDR4_INFO_FOURTH_128* SpdInfo, MemoryCommonInformation& OutputData)
{
}

void Hardware::Memory::ParserDDR4SPD::CalcDIMMFrequency(const DDR4_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
{
	auto TB = GetTimeBase<int8_t>(SpdInfo->stCKminMTB, SpdInfo->stCKminFTB);
	OutputData.MemoryFrequency = 1000.0 / TB;
}

void Hardware::Memory::ParserDDR4SPD::CalcDDR4TimeBase(const DDR4_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
{
#define CalcTimeBaseImpl(X) TimeType.Value = GetTimeBase<int8_t>(SpdInfo->s##X##MTB, SpdInfo->s##X##FTB)

	{
		MemoryTimingType TimeType;
		CalcTimeBaseImpl(tCKmin);
		TimeType.Name = "Minimum Clock Cycle Time (tCK min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		CalcTimeBaseImpl(tCKmax);
		TimeType.Name = "Maximum Clock Cycle Time (tCK max)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		CalcTimeBaseImpl(tAAmin);
		TimeType.Name = "CAS# Latency Time (tAA min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		CalcTimeBaseImpl(tRCDmin);
		TimeType.Name = "RAS# to CAS# Delay Time (tRCD min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		CalcTimeBaseImpl(tRPmin);
		TimeType.Name = "Row Precharge Delay Time (tRP min)";
		OutputData.Times.emplace_back(TimeType);
	}
	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase<int16_t>(SpdInfo->sUpper_TRAS_TRCmin.bits.tRASminMostMTB << 8 | SpdInfo->stRASminLeastMTB, 0);
		TimeType.Name = "Active to Precharge Delay Time (tRAS min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase<int16_t>(SpdInfo->sUpper_TRAS_TRCmin.bits.tRCminMostMTB << 8 | SpdInfo->stRCminLeastMTB, SpdInfo->stRCminFTB);
		TimeType.Name = "Act to Act/Refresh Delay Time (tRC min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase<int16_t>(SpdInfo->stRFC1minMostMTB << 8 | SpdInfo->stRFC1minLeastMTB, 0);
		TimeType.Name = "Normal Refresh Recovery Delay Time (tRFC1 min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase<int16_t>(SpdInfo->stRFC2minMostMTB << 8 | SpdInfo->stRFC2minLeastMTB, 0);
		TimeType.Name = "2x mode Refresh Recovery Delay Time (tRFC2 min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase<int16_t>(SpdInfo->stRFC4minMostMTB << 8 | SpdInfo->stRFC4minLeastMTB, 0);
		TimeType.Name = "4x mode Refresh Recovery Delay Time (tRFC4 min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		CalcTimeBaseImpl(tRRD_Smin);
		TimeType.Name = "Short Row Active to Row Active Delay (tRRD_S min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		CalcTimeBaseImpl(tRRD_Lmin);
		TimeType.Name = "Long Row Active to Row Active Delay (tRRD_L min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase<int16_t>(SpdInfo->sTWRminMost.bits.tWRminMostMTB << 8 | SpdInfo->sTWRminMTB, 0);
		TimeType.Name = "Write Recovery Time (tWR min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase<int16_t>(SpdInfo->sUpper_TWTR_L_TWTR_Smin.bits.tWTR_SminMostMTB << 8 | SpdInfo->stWTR_SminMTB, 0);
		TimeType.Name = "Short Write to Read Command Delay (tWTR_S min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase<int16_t>(SpdInfo->sUpper_TWTR_L_TWTR_Smin.bits.tWTR_LminMostMTB << 8 | SpdInfo->stWTR_LminMTB, 0);
		TimeType.Name = "Long Write to Read Command Delay (tWTR_L min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		CalcTimeBaseImpl(tCCD_Lmin);
		TimeType.Name = "Long CAS to CAS Delay Time (tCCD_L min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase<int16_t>(SpdInfo->sTFAWminMost.bits.tFAWminMostMTB << 8 | SpdInfo->stFAWminLeastMTB, 0);
		TimeType.Name = "Four Active Windows Delay (tFAW min)";
		OutputData.Times.emplace_back(TimeType);
	}

#undef CalcTimeBaseImpl
}
#pragma endregion

#pragma region DDR3
void Hardware::Memory::ParserDDR3SPD::ParserFirstSPD(const DDR3_INFO_FIRST_128* SpdInfo, MemoryCommonInformation& OutputData)
{
	ParserSerialNumber(SpdInfo, OutputData);
	ParseDeviceType(SpdInfo, OutputData);
	ParseModuleType(SpdInfo, OutputData);
	ParseModuleManufacture(SpdInfo, OutputData);
	ParseProduceDate(SpdInfo, OutputData);
	ParseDataWidth(SpdInfo, OutputData);
	CalcDIMMCapacity(GetCapacity(SpdInfo), GetRanks(SpdInfo), GetDRAMWidth(SpdInfo), GetPrimarybuswidth(SpdInfo), OutputData);
	CalcDIMMRB(GetRanks(SpdInfo), GetDDR3Banks(SpdInfo), OutputData);
	CalcDIMMFrequency(SpdInfo, OutputData);
	CalcDDR3TimeBase(SpdInfo, OutputData);
}

void Hardware::Memory::ParserDDR3SPD::ParserSecondSPD(const DDR3_INFO_SECOND_128* SpdInfo, MemoryCommonInformation& OutputData)
{
	ParserPartNumber(SpdInfo, OutputData);
	ParseDRAMManufacture(SpdInfo, OutputData);
}

void Hardware::Memory::ParserDDR3SPD::CalcDDR3TimeBase(const Hardware::Memory::DDR3_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
{
#define GetTimeBase(txxMTB, txxFTB) (int16_t)(txxMTB) * MTB + (int16_t)(txxFTB) * FTB
	auto MTB = SpdInfo->MTB_Dividend * 1.0 / SpdInfo->MTB_Divisor;
	auto FTB = SpdInfo->sFTB.bits.FTB_Dividend / (SpdInfo->sFTB.bits.FTB_Divisor * 1000.0);
	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->stCKminMTB, 0);
		TimeType.Name = "Minimum Clock Cycle Time (tCK min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->stCKminMTB, 0);
		TimeType.Name = "CAS# Latency Time (tAA min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->stCKminMTB, 0);
		TimeType.Name = "Write Recovery Time (tWR min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->stRCDminMTB, 0);
		TimeType.Name = "RAS# to CAS# Delay Time (tRCD min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->stRRDminMTB, 0);
		TimeType.Name = "Row Active to Row Active Delay Time (tRRD min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->stRPminMTB, 0);
		TimeType.Name = "Row Precharge Delay Time (tRP min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->sUpper_TRAS_TRCmin.bits.tRASminMostMTB << 8 | SpdInfo->stRASminLeastMTB, 0);
		TimeType.Name = "Active to Precharge Delay Time (tRAS min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->sUpper_TRAS_TRCmin.bits.tRCminMostMTB << 8 | SpdInfo->stRCminLeastMTB, 0);
		TimeType.Name = "Active to Active/Refresh Delay Time (tRC min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->stRFCminMostMTB << 8 | SpdInfo->stRFCminLeastMTB, 0);
		TimeType.Name = "Refresh Recovery Delay Time (tRFC min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->stWTRminMTB, 0);
		TimeType.Name = "Write to Read Command Delay Time (tWTR min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->stRTPminMTB, 0);
		TimeType.Name = "Read to Precharge Command Delay Time (tRTP min)";
		OutputData.Times.emplace_back(TimeType);
	}

	{
		MemoryTimingType TimeType;
		TimeType.Value = GetTimeBase(SpdInfo->sTFAWminMost.bits.tFAWminMostMTB << 8 | SpdInfo->stFAWminLeastMTB, 0);
		TimeType.Name = "Four Activate Window Delay Time (tFAW min)";
		OutputData.Times.emplace_back(TimeType);
	}
#undef GetTimeBase
}
void Hardware::Memory::ParserDDR3SPD::CalcDIMMFrequency(const DDR3_INFO_FIRST_128* SpdInfo, Hardware::Memory::MemoryCommonInformation& OutputData)
{
	auto MTB = SpdInfo->MTB_Dividend * 1.0 / SpdInfo->MTB_Divisor;
	auto TB = (int8_t)SpdInfo->stCKminMTB * MTB;
	OutputData.MemoryFrequency = 1000.0 / TB;
}
#pragma endregion