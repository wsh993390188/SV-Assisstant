#include "stdafx.h"
#include "CMemory.h"
#include "..\smbios\smbioslib.h"
#include "MemoryLib.h"
#include <boost/algorithm/string.hpp>

namespace SV_ASSIST
{
	namespace Memory
	{
		const USHORT DIMMADDR[8] =
		{
			0xA0,
			0xA2,
			0xA4,
			0xA6,
			0xA8,
			0xAA,
			0xAC,
			0xAE,
		};
		const USHORT IvyBridgeDIMMSlot[8] = { 0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7 };

		class MemoryLib
		{
		public:
			static MemoryLib* Instance() 
			{
				if (!m_memory.get())
					m_memory = std::make_shared<MemoryLib>();
				return m_memory.get();
			}
			MemoryLib();
			const std::vector<std::pair<ULONG, std::pair<ULONG, DIMMType>>>& GetMemoryDeviceInfo() const { return this->memorymapdeviceID; }			
			void UpdateData();

			const std::vector<std::pair<ULONG, DDR3_INFO>>& GetMemoryDDR3Info() const { return this->Memoryddr3Info; }
			const std::vector<std::pair<ULONG, DDR4_INFO>>& GetMemoryDDR4Info() const { return this->Memoryddr4Info; }
			const std::vector<std::pair<ULONG, MemoryData>>& GetMemoryGlobalInfomation() const { return this->SPDInfomation; }
			~MemoryLib();
		private:
			bool ReadSPD(const DIMMType types, const USHORT DIMMId, PVOID64& spd, const int len, const ULONG SmbusControlBase = 0);
			void GetSPDInfo();
			bool JudgeSPDType(const USHORT DIMMId, const ULONG SwitchSmbus = 0/*Ivy Bridge Switch Smbus Controller*/);
			void ExecToSPDInfomation();
			void ExecDDR3ToSPDInfomation(const DDR3_INFO& spd, MemoryData& data);
			void ExecDDR4ToSPDInfomation(const DDR4_INFO& spd, MemoryData& data);
			const std::string SPDManufacturer(const BYTE Bank, const BYTE IDCode);
		private:
			DIMMType GlobelDIMMType;
			std::vector<std::pair<ULONG, std::pair<ULONG, DIMMType>>> memorymapdeviceID;//first: smbus base second: first:Slave Device Address; second:DIMM Module Type
			std::vector<std::pair<ULONG, DDR3_INFO>> Memoryddr3Info;
			std::vector<std::pair<ULONG, DDR4_INFO>> Memoryddr4Info;
			std::vector<std::pair<ULONG, MemoryData>> SPDInfomation;
			std::unique_ptr<IBaseMemory> m_data;
			static std::shared_ptr<MemoryLib> m_memory;
		};


		std::shared_ptr<MemoryLib> MemoryLib::m_memory = nullptr;

		MemoryLib::MemoryLib() : GlobelDIMMType(DIMM_UNKNOWN)
		{
			m_data = std::make_unique<CommonSMBUS>();
			auto smbiosmemory = SV_ASSIST::DMI_SMBIOS::GetSMBIOSinfo().MemoryDev;
			bool Success = false;
			for (size_t DIMMSlot = 0; DIMMSlot < smbiosmemory.size(); DIMMSlot++)
			{
				Success |= JudgeSPDType(DIMMADDR[DIMMSlot]);
			}
			if (!Success && memorymapdeviceID.empty())
			{
				PCI_COMMON_CONFIG pci = {};
				Ring0::ReadPci(0, 0, 0, pci);
				if (pci.VendorID == 0x8086 && pci.DeviceID == 0x3C00)
				{
					m_data = nullptr;
					m_data.reset(new IvyBridgeSMbus());
				}
				else if (pci.VendorID == 0x8086 && pci.DeviceID == 0x0E00)
				{
					m_data = nullptr;
					m_data.reset(new IvyBridgeSMbus());
				}
				for (size_t DIMMSlot = 0; DIMMSlot < smbiosmemory.size(); DIMMSlot++)
				{
					Success |= JudgeSPDType(IvyBridgeDIMMSlot[DIMMSlot]);
					Success |= JudgeSPDType(IvyBridgeDIMMSlot[DIMMSlot] ,1);
				}
			}
			Sleep(1);
			if (Success && !memorymapdeviceID.empty())
			{
				GlobelDIMMType = memorymapdeviceID.at(0).second.second;
				this->GetSPDInfo();
				this->ExecToSPDInfomation();
			}
		}

		void MemoryLib::UpdateData()
		{
			Memoryddr3Info.clear();
			Memoryddr4Info.clear();
			this->SPDInfomation.clear();
			this->GetSPDInfo();
			this->ExecToSPDInfomation();

		}

		void MemoryLib::GetSPDInfo()
		{
			PVOID64 spd = nullptr;
			size_t len = 0;
			bool firstCreate = true;
			for (const auto& i : memorymapdeviceID)
			{
				auto SmbusBase = i.first;
				auto DIMMSlot = i.second.first;
				auto DIMMType = i.second.second;
				if(firstCreate)
				{
					if (DIMMType == DDR4 || DIMMType == DDR4E || DIMMType == LPDDR4)
					{
						spd = new DDR4_INFO{};
						len = sizeof(DDR4_INFO);
					}
					else
					{
						spd = new DDR3_INFO{};
						len = sizeof(DDR3_INFO);
					}
					firstCreate = false;
				}
				else
				{
					memset(spd, 0, len);
				}
				if(this->ReadSPD(DIMMType, DIMMSlot, spd, len, SmbusBase))
					if (DIMMType == DDR4 || DIMMType == DDR4E || DIMMType == LPDDR4)
					{
						std::pair<ULONG, DDR4_INFO> temp(DIMMSlot, *(PDDR4_INFO)spd);
						Memoryddr4Info.emplace_back(temp);
					}
					else
					{
						std::pair<ULONG, DDR3_INFO> temp(DIMMSlot, *(PDDR3_INFO)spd);
						Memoryddr3Info.emplace_back(temp);
					}
			}
			if(spd)
			{
				delete spd;
				spd = nullptr;
			}
		}

		bool MemoryLib::JudgeSPDType(const USHORT DIMMId, const ULONG SwitchSmbus)
		{
			if (!m_data.get())
				return false;
			DIMMType types = DIMM_UNKNOWN;
			ULONG SmbusControlBase = SwitchSmbus;
			if (this->m_data->JudgeSPDType(types, DIMMId, SmbusControlBase))
			{
				std::pair<ULONG, DIMMType> temp1(DIMMId, types);
				std::pair<ULONG, std::pair<ULONG, DIMMType>> temp(SmbusControlBase, temp1);
				memorymapdeviceID.emplace_back(temp);
			}
			else
				return false;
			return true;
		}
#pragma region Execute SPD Infomation
		void MemoryLib::ExecToSPDInfomation()
		{
			if (GlobelDIMMType == DDR4 || GlobelDIMMType == DDR4E || GlobelDIMMType == LPDDR4)
			{
				for (const auto& i : this->Memoryddr4Info)
				{
					MemoryData temp = {};
					this->ExecDDR4ToSPDInfomation(i.second, temp);
					std::pair<ULONG, MemoryData> addinfo(i.first, temp);
					SPDInfomation.emplace_back(addinfo);
				}
			}
			else
			{
				for (const auto& i : this->Memoryddr3Info)
				{
					MemoryData temp = {};
					this->ExecDDR3ToSPDInfomation(i.second, temp);
					std::pair<ULONG, MemoryData> addinfo(i.first, temp);
					SPDInfomation.emplace_back(addinfo);
				}
			}

		}

		void MemoryLib::ExecDDR3ToSPDInfomation(const DDR3_INFO & spd, MemoryData& data)
		{
			char part_number[20] = {};
			memcpy(part_number, spd.DDR3_Detail.PartNumber, 18);
			data.PartNumber = part_number;
			boost::trim(data.PartNumber);
			memset(part_number, 0, 20);

			sprintf_s(part_number, "%02X%02X%02X%02X", spd.DDR3_Detail.SerialNumber[0], spd.DDR3_Detail.SerialNumber[1], spd.DDR3_Detail.SerialNumber[2], spd.DDR3_Detail.SerialNumber[3]);
			data.SerialNumber = part_number;
			boost::trim(data.SerialNumber);

			switch (spd.DDR3_Detail.sDRAMType)
			{
			case 0x0B:
				data.ModuleType = "DDR3 ";
				switch (spd.DDR3_Detail.sModuleType.bits.Module_Type)
				{
				case 0b0001:
					data.ModuleType += "RDIMM";
					break;
				case 0b0010:
					data.ModuleType += "UDIMM";
					break;
				case 0b0011:
					data.ModuleType += "SO-DIMM";
					break;
				case 0b0100:
					data.ModuleType += "Micro-DIMM";
					break;
				case 0b0101:
					data.ModuleType += "Mini-RDIMM";
					break;
				case 0b0110:
					data.ModuleType += "Mini-UDIMM";
					break;
				case 0b0111:
					data.ModuleType += "Mini-CDIMM";
					break;
				case 0b1000:
					data.ModuleType += "72b-SO-UDIMM";
					break;
				case 0b1001:
					data.ModuleType += "72b-SO-RDIMM";
					break;
				case 0b1010:
					data.ModuleType += "72b-SO-CDIMM";
					break;
				default:
					break;
				}
				break;
			case 0x0F:
				data.ModuleType = "LPDDR3 ";
				switch (spd.DDR3_Detail.sModuleType.bits.Module_Type)
				{
				case 0b0001:
					data.ModuleType += "RDIMM";
					break;
				case 0b0010:
					data.ModuleType += "UDIMM";
					break;
				case 0b0011:
					data.ModuleType += "SO-DIMM";
					break;
				case 0b0100:
					data.ModuleType += "Micro-DIMM";
					break;
				case 0b0101:
					data.ModuleType += "Mini-RDIMM";
					break;
				case 0b0110:
					data.ModuleType += "Mini-UDIMM";
					break;
				case 0b1000:
					data.ModuleType += "72b-SO-UDIMM";
					break;
				case 0b1001:
					data.ModuleType += "72b-SO-RDIMM";
					break;
				case 0b1010:
					data.ModuleType += "72b-SO-CDIMM";
					break;
				case 0b1100:
					data.ModuleType += "16b-SO-CDIMM";
					break;
				case 0b1101:
					data.ModuleType += "32b-SO-CDIMM";
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			data.DRAMManufacturer = SPDManufacturer(spd.DDR3_Detail.DRAMManufacturerIDLeast & 0x7F, spd.DDR3_Detail.DRAMManufacturerIDMost);
			data.ModuleManufacturer = SPDManufacturer(spd.DDR3_Detail.ModuleManufacturerIDLeast & 0x7F, spd.DDR3_Detail.ModuleManufacturerIDMost);
			int  Ranks = spd.DDR3_Detail.sModuleOrganization.bits.Number_of_Ranks, Primarybuswidth = spd.DDR3_Detail.sBusWidth.bits.Primary_bus_width,\
				SDRAMWidth = spd.DDR3_Detail.sModuleOrganization.bits.SDRAM_Depth_Width;
			double capacity = 0;
			Ranks++;
			switch (spd.DDR3_Detail.sSDRAMBank_Density.bits.Capacity)
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
			switch (SDRAMWidth)
			{
			case 0:
				SDRAMWidth = 4;
				break;
			case 0x01:
				SDRAMWidth = 8;
				break;
			case 0x02:
				SDRAMWidth = 16;
				break;
			case 0x03:
				SDRAMWidth = 32;
				break;
			default:
				break;
			}
			switch (Primarybuswidth)
			{
			case 0:
				Primarybuswidth = 8;
				break;
			case 0x01:
				Primarybuswidth = 16;
				break;
			case 0x02:
				Primarybuswidth = 32;
				break;
			case 0x03:
				Primarybuswidth = 64;
				break;
			default:
				break;
			}
			if (spd.DDR3_Detail.sBusWidth.bits.Bus_width_extension == 1)
			{
				data.SupportsECC = true;
				data.DataBits = (std::to_string(Primarybuswidth + 8) + " Bits With 8 Bits ECC");
			}
			else
			{
				data.SupportsECC = false;
				data.DataBits = (std::to_string(Primarybuswidth) + " Bits");
			}
			if (spd.DDR3_Detail.MTB_Dividend == 1 && spd.DDR3_Detail.MTB_Divisor == 8)
			{
				if (spd.DDR3_Detail.sTCKmin == 10)
					data.MaxBandWidth = "DDR3-1600 (800MHz)";
				else if (spd.DDR3_Detail.sTCKmin == 12)
					data.MaxBandWidth = "DDR3-1333 (666.67MHz)";
				else if (spd.DDR3_Detail.sTCKmin == 15)
					data.MaxBandWidth = "DDR3-1066 (533MHz)";
				else if (spd.DDR3_Detail.sTCKmin == 20)
					data.MaxBandWidth = "DDR3-800 (400MHz)";
			}
			double DDRSize = capacity / 8.0 * Primarybuswidth / SDRAMWidth * Ranks;
			data.ModuleSize = (size_t)DDRSize;
			int year = spd.DDR3_Detail.ManufacturingDate[0], week = spd.DDR3_Detail.ManufacturingDate[1];
			memset(part_number, 0, 20);
			if(week != 0 && year != 0)
			{
				sprintf_s(part_number, "%02X\\%02X", week, year);
				data.ProductDate = part_number;
			}

			{
				int Banks = pow(2, spd.DDR3_Detail.sSDRAMBank_Density.bits.Bank + 3);
				data.Ranks_Banks = boost::str(boost::format("%1% Ranks %2% Banks") % Ranks % Banks);
			}
			if (!spd.DDR3_Detail.sModuleNominalVoltage.bits.VDD_1_5)
				data.Voltages += " 1.5 V";
			if (spd.DDR3_Detail.sModuleNominalVoltage.bits.VDD_1_35)
				data.Voltages += " 1.35 V";
			if (spd.DDR3_Detail.sModuleNominalVoltage.bits.VDD_1_2X)
				data.Voltages += " 1.2X V";
		}

		void MemoryLib::ExecDDR4ToSPDInfomation(const DDR4_INFO & spd, MemoryData& data)
		{
			char part_number[20] = {};
			memcpy(part_number, spd.DDR4_Detail.PartNumber, 18);
			data.PartNumber = part_number;
			boost::trim(data.PartNumber);
			memset(part_number, 0, 20);

			sprintf_s(part_number, "%02X%02X%02X%02X", spd.DDR4_Detail.SerialNumber[0], spd.DDR4_Detail.SerialNumber[1], spd.DDR4_Detail.SerialNumber[2], spd.DDR4_Detail.SerialNumber[3]);
			data.SerialNumber = part_number;
			boost::trim(data.SerialNumber);

			int Primarybuswidth = spd.DDR4_Detail.sBusWidth.bits.Primary_bus_width, SDRAMWidth = spd.DDR4_Detail.sModuleOrganization.bits.SDRAM_Depth_Width,\
				Ranks = spd.DDR4_Detail.sModuleOrganization.bits.Number_of_Ranks_per_DIMM;
			double capacity = 0;
			Ranks++;
			switch (spd.DDR4_Detail.sSDRAMBank_Density.bits.Capacity)
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
			switch (SDRAMWidth)
			{
			case 0:
				SDRAMWidth = 4;
				break;
			case 0x01:
				SDRAMWidth = 8;
				break;
			case 0x02:
				SDRAMWidth = 16;
				break;
			case 0x03:
				SDRAMWidth = 32;
				break;
			default:
				break;
			}
			switch (Primarybuswidth)
			{
			case 0:
				Primarybuswidth = 8;
				break;
			case 0x01:
				Primarybuswidth = 16;
				break;
			case 0x02:
				Primarybuswidth = 32;
				break;
			case 0x03:
				Primarybuswidth = 64;
				break;
			default:
				break;
			}
			if (spd.DDR4_Detail.sPackageType.bits.SignalLoading == 2)
				Ranks = Ranks * spd.DDR4_Detail.sPackageType.bits.DieCount;
			double DDRSize = capacity / 8.0 * Primarybuswidth / SDRAMWidth * Ranks;
			data.ModuleSize = (size_t)DDRSize;
			switch (spd.DDR4_Detail.sDRAMType)
			{
			case 0x0C:
				data.ModuleType = "DDR4 ";
				break;
			case 0x0E:
				data.ModuleType = "DDR4E ";
				break;
			case 0x10:
				data.ModuleType = "LPDDR4 ";
				break;
			default:
				break;
			}
			switch (spd.DDR4_Detail.sModuleType.bits.Module_Type)
			{
			case 0b0001:
				data.ModuleType += "RDIMM";
				break;
			case 0b0010:
				data.ModuleType += "UDIMM";
				break;
			case 0b0011:
				data.ModuleType += "SO-DIMM";
				break;
			case 0b0100:
				data.ModuleType += "Micro-DIMM";
				break;
			case 0b0101:
				data.ModuleType += "Mini-RDIMM";
				break;
			case 0b0110:
				data.ModuleType += "Mini-UDIMM";
				break;
			case 0b1000:
				data.ModuleType += "72b-SO-UDIMM";
				break;
			case 0b1001:
				data.ModuleType += "72b-SO-RDIMM";
				break;
			case 0b1010:
				data.ModuleType += "72b-SO-CDIMM";
				break;
			case 0b1100:
				data.ModuleType += "16b-SO-CDIMM";
				break;
			case 0b1101:
				data.ModuleType += "32b-SO-CDIMM";
				break;
			default:
				break;
			}
			data.Voltages = "1.2 V";
			data.DRAMManufacturer = SPDManufacturer(spd.DDR4_Detail.DRAMManufacturerIDLeast & 0x7F, spd.DDR4_Detail.DRAMManufacturerIDMost);
			data.ModuleManufacturer = SPDManufacturer(spd.DDR4_Detail.ModuleManufacturerIDLeast & 0x7F, spd.DDR4_Detail.ModuleManufacturerIDMost);
			int year = spd.DDR4_Detail.ManufacturingDate[0], week = spd.DDR4_Detail.ManufacturingDate[1];
			memset(part_number, 0, 20);
			if (week != 0 && year != 0)
			{
				sprintf_s(part_number, "%02X\\%02X", week, year);
				data.ProductDate = part_number;
			}
			if (spd.DDR4_Detail.sTCKmin == 10 && spd.DDR4_Detail.sTCKmin125 == 0)
				data.MaxBandWidth = "DDR4-1600 (800MHz)";
			else if (spd.DDR4_Detail.sTCKmin == 9 && spd.DDR4_Detail.sTCKmin125 == 0xCA)
				data.MaxBandWidth = "DDR4-1866 (933MHz)";
			else if (spd.DDR4_Detail.sTCKmin == 8 && spd.DDR4_Detail.sTCKmin125 == 0xC1)
				data.MaxBandWidth = "DDR4-2133 (1066.67MHz)";
			else if (spd.DDR4_Detail.sTCKmin == 7 && spd.DDR4_Detail.sTCKmin125 == 0xD6)
				data.MaxBandWidth = "DDR4-2400 (1200MHz)";
			else if (spd.DDR4_Detail.sTCKmin == 6 && spd.DDR4_Detail.sTCKmin125 == 0)
				data.MaxBandWidth = "DDR4-2666 (1333.33MHz)";
			else if (spd.DDR4_Detail.sTCKmin == 5 && spd.DDR4_Detail.sTCKmin125 == 0)
				data.MaxBandWidth = "DDR4-3200 (1600MHz)";
			{
				int BankGroup = 1;
				switch (spd.DDR4_Detail.sSDRAMBank_Density.bits.BankGroupBits)
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
				int BankAddressBits = 0;
				switch (spd.DDR4_Detail.sSDRAMBank_Density.bits.BankAddressBits)
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
				data.Ranks_Banks = boost::str(boost::format("%1% Ranks %2% Banks") % Ranks % (BankAddressBits * BankGroup));
			}

			if (spd.DDR4_Detail.sBusWidth.bits.Bus_width_extension == 1)
				data.SupportsECC = true;
			else
				data.SupportsECC = false;
		}

		const std::string MemoryLib::SPDManufacturer(const BYTE Bank, const BYTE IDCode)
		{
			std::string temp = "Unknown Manufacturer";
			switch (Bank + 1)
			{
			case 1:
				switch (IDCode)
				{
				case 1:
					temp = "AMD";
					break;
				case 2:
					temp = "AMI";
					break;
				case 0x83:
					temp = "Fairchild";
					break;
				case 4:
					temp = "Fujitsu";
					break;
				case 0x85:
					temp = "GTE";
					break;
				case 0x86:
					temp = "Harris";
					break;
				case 7:
					temp = "Hitachi";
					break;
				case 8:
					temp = "Inmos";
					break;
				case 0x89:
					temp = "Intel";
					break;
				case 0x8A:
					temp = "I.T.T.";
					break;
				case 0x0B:
					temp = "Intersil";
					break;
				case 0x8C:
					temp = "Monolithic Memories";
					break;
				case 0x0D:
					temp = "Mostek";
					break;
				case 0x0E:
					temp = "Freescale (Motorola)";
					break;
				case 0x8F:
					temp = "National";
					break;
				case 0x10:
					temp = "NEC";
					break;
				case 0x91:
					temp = "RCA";
					break;
				case 0x92:
					temp = "Raytheon";
					break;
				case 0x13:
					temp = "Conexant (Rockwell)";
					break;
				case 0x94:
					temp = "Seeq";
					break;
				case 0x15:
					temp = "NXP (Philips)";
					break;
				case 0x16:
					temp = "Synertek";
					break;
				case 0x97:
					temp = "Texas Instruments";
					break;
				case 0x98:
					temp = "Toshiba";
					break;
				case 0x19:
					temp = "Xicor";
					break;
				case 0x1A:
					temp = "Zilog";
					break;
				case 0x9B:
					temp = "Eurotechnique";
					break;
				case 0x1C:
					temp = "Mitsubishi";
					break;
				case 0x9D:
					temp = "Lucent (AT&T)";
					break;
				case 0x9E:
					temp = "Exel";
					break;
				case 0x1F:
					temp = "Atmel";
					break;
				case 0x20:
					temp = "STMicroelectronics";
					break;
				case 0xA1:
					temp = "Lattice Semi.";
					break;
				case 0xA2:
					temp = "NCR";
					break;
				case 0x23:
					temp = "Wafer Scale Integration";
					break;
				case 0xA4:
					temp = "IBM";
					break;
				case 0x25:
					temp = "Tristar";
					break;
				case 0x26:
					temp = "Visic";
					break;
				case 0xA7:
					temp = "Intl. CMOS Technology";
					break;
				case 0xA8:
					temp = "SSSI";
					break;
				case 0x29:
					temp = "MicrochipTechnology";
					break;
				case 0x2A:
					temp = "Ricoh Ltd.";
					break;
				case 0xAB:
					temp = "VLSI";
					break;
				case 0x2C:
					temp = "Micron Technology";
					break;
				case 0xAD:
					temp = "SK Hynix";
					break;
				case 0xAE:
					temp = "OKI Semiconductor";
					break;
				case 0x2F:
					temp = "ACTEL";
					break;
				case 0xB0:
					temp = "Sharp";
					break;
				case 0x31:
					temp = "Catalyst";
					break;
				case 0x32:
					temp = "Panasonic";
					break;
				case 0xB3:
					temp = "IDT";
					break;
				case 0x34:
					temp = "Cypress";
					break;
				case 0xB5:
					temp = "DEC";
					break;
				case 0xB6:
					temp = "LSI Logic";
					break;
				case 0x37:
					temp = "Zarlink (Plessey)";
					break;
				case 0x38:
					temp = "UTMC";
					break;
				case 0xB9:
					temp = "Thinking Machine";
					break;
				case 0xBA:
					temp = "Thomson CSF";
					break;
				case 0x3B:
					temp = "Integrated CMOS (Vertex)";
					break;
				case 0xBC:
					temp = "Honeywell";
					break;
				case 0x3D:
					temp = "Tektronix";
					break;
				case 0x3E:
					temp = "Oracle Corporation";
					break;
				case 0xBF:
					temp = "Silicon Storage Technology";
					break;
				case 0x40:
					temp = "ProMos/Mosel Vitelic";
					break;
				case 0xC1:
					temp = "Infineon (Siemens)";
					break;
				case 0xC2:
					temp = "Macronix";
					break;
				case 0x43:
					temp = "Xerox";
					break;
				case 0xC4:
					temp = "Plus Logic";
					break;
				case 0x45:
					temp = "SanDisk Corporation";
					break;
				case 0x46:
					temp = "Elan Circuit Tech.";
					break;
				case 0xC7:
					temp = "European Silicon Str.";
					break;
				case 0xC8:
					temp = "Apple Computer";
					break;
				case 0x49:
					temp = "Xilinx";
					break;
				case 0x4A:
					temp = "Compaq";
					break;
				case 0xCB:
					temp = "Protocol Engines";
					break;
				case 0x4C:
					temp = "SCI";
					break;
				case 0xCD:
					temp = "Seiko Instruments";
					break;
				case 0xCE:
					temp = "Samsung";
					break;
				case 0x4F:
					temp = "I3 Design System";
					break;
				case 0xD0:
					temp = "Klic";
					break;
				case 0x51:
					temp = "Crosspoint Solutions";
					break;
				case 0x52:
					temp = "Alliance Semiconductor";
					break;
				case 0xD3:
					temp = "Tandem";
					break;
				case 0x54:
					temp = "Hewlett-Packard";
					break;
				case 0xD5:
					temp = "Integrated Silicon Solutions";
					break;
				case 0xD6:
					temp = "Brooktree";
					break;
				case 0x57:
					temp = "New Media";
					break;
				case 0x58:
					temp = "MHS Electronic";
					break;
				case 0xD9:
					temp = "Performance Semi.";
					break;
				case 0xDA:
					temp = "Winbond Electronic";
					break;
				case 0x5B:
					temp = "Kawasaki Steel";
					break;
				case 0xDC:
					temp = "Bright Micro";
					break;
				case 0x5D:
					temp = "TECMAR";
					break;
				case 0x5E:
					temp = "Exar";
					break;
				case 0xDF:
					temp = "PCMCIA";
					break;
				case 0xE0:
					temp = "LG Semi (Goldstar)";
					break;
				case 0x61:
					temp = "Northern Telecom";
					break;
				case 0x62:
					temp = "Sanyo";
					break;
				case 0xE3:
					temp = "Array Microsystems";
					break;
				case 0x64:
					temp = "Crystal Semiconductor";
					break;
				case 0xE5:
					temp = "Analog Devices";
					break;
				case 0xE6:
					temp = "PMC-Sierra";
					break;
				case 0X67:
					temp = "Asparix";
					break;
				case 0x68:
					temp = "Convex Computer";
					break;
				case 0xE9:
					temp = "Quality Semiconductor";
					break;
				case 0xEA:
					temp = "Nimbus Technology";
					break;
				case 0x6B:
					temp = "Transwitch";
					break;
				case 0xEC:
					temp = "Micronas (ITT Intermetall)";
					break;
				case 0x6D:
					temp = "Cannon";
					break;
				case 0x6E:
					temp = "Altera";
					break;
				case 0xEF:
					temp = "NEXCOM";
					break;
				case 0x70:
					temp = "Qualcomm";
					break;
				case 0xF1:
					temp = "Sony";
					break;
				case 0xF2:
					temp = "Cray Research";
					break;
				case 0x73:
					temp = "AMS(Austria Micro)";
					break;
				case 0xF4:
					temp = "Vitesse";
					break;
				case 0x75:
					temp = "Aster Electronics";
					break;
				case 0x76:
					temp = "Bay Networks (Synoptic)";
					break;
				case 0xF7:
					temp = "Zentrum/ZMD";
					break;
				case 0xF8:
					temp = "TRW";
					break;
				case 0x79:
					temp = "Thesys";
					break;
				case 0x7A:
					temp = "Solbourne Computer";
					break;
				case 0xFB:
					temp = "Allied-Signal";
					break;
				case 0x7C:
					temp = "Dialog Semiconductor";
					break;
				case 0xFD:
					temp = "Media Vision";
					break;
				case 0xFE:
					temp = "Numonyx Corporation";
					break;				
				default:
					break;
				}
				break;
			case 2:
					switch (IDCode)
					{
					case 1:
						temp = "Cirrus Logic";
						break;
					case 2:
						temp = "National Instruments";
						break;
					case 0x83:
						temp = "ILC Data Device";
						break;
					case 4:
						temp = "Alcatel Mietec";
						break;
					case 0x85:
						temp = "Micro Linear";
						break;
					case 0x86:
						temp = "Univ. of NC";
						break;
					case 7:
						temp = "JTAG Technologies";
						break;
					case 8:
						temp = "BAE Systems (Loral)";
						break;
					case 0x89:
						temp = "Nchip";
						break;
					case 0x8A:
						temp = "Galileo Tech";
						break;
					case 0x0B:
						temp = "Bestlink Systems";
						break;
					case 0x8C:
						temp = "Graychip";
						break;
					case 0x0D:
						temp = "GENNUM";
						break;
					case 0x0E:
						temp = "VideoLogic";
						break;
					case 0x8F:
						temp = "Robert Bosch";
						break;
					case 0x10:
						temp = "Chip Express";
						break;
					case 0x91:
						temp = "DATARAM";
						break;
					case 0x92:
						temp = "United Microelectronics Corp.";
						break;
					case 0x13:
						temp = "TCSI";
						break;
					case 0x94:
						temp = "Smart Modular";
						break;
					case 0x15:
						temp = "Hughes Aircraft";
						break;
					case 0x16:
						temp = "Lanstar Semiconductor";
						break;
					case 0x97:
						temp = "Qlogic";
						break;
					case 0x98:
						temp = "Kingston";
						break;
					case 0x19:
						temp = "Music Semi";
						break;
					case 0x1A:
						temp = "Ericsson Components";
						break;
					case 0x9B:
						temp = "SpaSE";
						break;
					case 0x1C:
						temp = "Eon Silicon Devices";
						break;
					case 0x9D:
						temp = "Integrated Silicon Solution (ISSI)";
						break;
					case 0x9E:
						temp = "DoD";
						break;
					case 0x1F:
						temp = "Integ. Memories Tech.";
						break;
					case 0x20:
						temp = "Corollary Inc.";
						break;
					case 0xA1:
						temp = "Dallas Semiconductor";
						break;
					case 0xA2:
						temp = "Omnivision";
						break;
					case 0x23:
						temp = "EIV(Switzerland)";
						break;
					case 0xA4:
						temp = "Novatel Wireless";
						break;
					case 0x25:
						temp = "Zarlink (Mitel)";
						break;
					case 0x26:
						temp = "Clearpoint";
						break;
					case 0xA7:
						temp = "Cabletron";
						break;
					case 0xA8:
						temp = "STEC (Silicon Tech)";
						break;
					case 0x29:
						temp = "Vanguard";
						break;
					case 0x2A:
						temp = "Hagiwara Sys-Com";
						break;
					case 0xAB:
						temp = "Vantis";
						break;
					case 0x2C:
						temp = "Celestica";
						break;
					case 0xAD:
						temp = "Century";
						break;
					case 0xAE:
						temp = "Hal Computers";
						break;
					case 0x2F:
						temp = "Rohm Company Ltd.";
						break;
					case 0xB0:
						temp = "Juniper Networks";
						break;
					case 0x31:
						temp = "Libit Signal Processing";
						break;
					case 0x32:
						temp = "Mushkin Enhanced Memory";
						break;
					case 0xB3:
						temp = "Tundra Semiconductor";
						break;
					case 0x34:
						temp = "Adaptec Inc.";
						break;
					case 0xB5:
						temp = "LightSpeed Semi.";
						break;
					case 0xB6:
						temp = "ZSP Corp.";
						break;
					case 0x37:
						temp = "AMIC Technology";
						break;
					case 0x38:
						temp = "Adobe Systems";
						break;
					case 0xB9:
						temp = "Dynachip";
						break;
					case 0xBA:
						temp = "PNY Technologies, Inc.";
						break;
					case 0x3B:
						temp = "Newport Digital";
						break;
					case 0xBC:
						temp = "MMC Networks";
						break;
					case 0x3D:
						temp = "T Square";
						break;
					case 0x3E:
						temp = "Seiko Epson";
						break;
					case 0xBF:
						temp = "Broadcom";
						break;
					case 0x40:
						temp = "Viking Components";
						break;
					case 0xC1:
						temp = "V3 Semiconductor";
						break;
					case 0xC2:
						temp = "Flextronics (Orbit Semiconductor)";
						break;
					case 0x43:
						temp = "Suwa Electronics";
						break;
					case 0xC4:
						temp = "Transmeta";
						break;
					case 0x45:
						temp = "Micron CMS";
						break;
					case 0x46:
						temp = "American Computer & Digital Components Inc.";
						break;
					case 0xC7:
						temp = "Enhance 3000 Inc.";
						break;
					case 0xC8:
						temp = "Tower Semiconductor";
						break;
					case 0x49:
						temp = "CPU Design";
						break;
					case 0x4A:
						temp = "Price Point";
						break;
					case 0xCB:
						temp = "Maxim Integrated Product";
						break;
					case 0x4C:
						temp = "Tellabs";
						break;
					case 0xCD:
						temp = "Centaur Technology";
						break;
					case 0xCE:
						temp = "Unigen Corporation";
						break;
					case 0x4F:
						temp = "Transcend Information";
						break;
					case 0xD0:
						temp = "Memory Card Technology";
						break;
					case 0x51:
						temp = "CKD Corporation Ltd.";
						break;
					case 0x52:
						temp = "Capital Instruments, Inc.";
						break;
					case 0xD3:
						temp = "Aica Kogyo, Ltd.";
						break;
					case 0x54:
						temp = "Linvex Technology";
						break;
					case 0xD5:
						temp = "MSC Vertriebs GmbH";
						break;
					case 0xD6:
						temp = "AKM Company, Ltd.";
						break;
					case 0x57:
						temp = "Dynamem, Inc.";
						break;
					case 0x58:
						temp = "NERA ASA";
						break;
					case 0xD9:
						temp = "GSI Technology";
						break;
					case 0xDA:
						temp = "Dane-Elec (C Memory)";
						break;
					case 0x5B:
						temp = "Acorn Computers";
						break;
					case 0xDC:
						temp = "Lara Technology";
						break;
					case 0x5D:
						temp = "Oak TechnologY Inc.";
						break;
					case 0x5E:
						temp = "Itec Memory";
						break;
					case 0xDF:
						temp = "Tanisys Technology";
						break;
					case 0xE0:
						temp = "Truevision";
						break;
					case 0x61:
						temp = "Wintec Industries";
						break;
					case 0x62:
						temp = "Super PC Memory";
						break;
					case 0xE3:
						temp = "MGV Memory";
						break;
					case 0x64:
						temp = "Galvantech";
						break;
					case 0xE5:
						temp = "Gadzoox Networks";
						break;
					case 0xE6:
						temp = "Multi Dimensional Cons.";
						break;
					case 0X67:
						temp = "GateField";
						break;
					case 0x68:
						temp = "Integrated Memory System";
						break;
					case 0xE9:
						temp = "Triscend";
						break;
					case 0xEA:
						temp = "XaQti";
						break;
					case 0x6B:
						temp = "Goldenram";
						break;
					case 0xEC:
						temp = "Clear Logic";
						break;
					case 0x6D:
						temp = "Cimaron Communications";
						break;
					case 0x6E:
						temp = "Nippon Steel Semi. Corp.";
						break;
					case 0xEF:
						temp = "Advantage Memory";
						break;
					case 0x70:
						temp = "AMCC";
						break;
					case 0xF1:
						temp = "LeCroy";
						break;
					case 0xF2:
						temp = "Yamaha Corporation";
						break;
					case 0x73:
						temp = "Digital Microwave";
						break;
					case 0xF4:
						temp = "NetLogic Microsystems";
						break;
					case 0x75:
						temp = "MIMOS Semiconductor";
						break;
					case 0x76:
						temp = "Advanced Fibre";
						break;
					case 0xF7:
						temp = "BF Goodrich Data.";
						break;
					case 0xF8:
						temp = "Epigram";
						break;
					case 0x79:
						temp = "Acbel Polytech Inc.";
						break;
					case 0x7A:
						temp = "Apacer Technology";
						break;
					case 0xFB:
						temp = "Admor Memory";
						break;
					case 0x7C:
						temp = "FOXCONN";
						break;
					case 0xFD:
						temp = "Quadratics Superconductor";
						break;
					case 0xFE:
						temp = "3COM";
						break;
					default:
						break;
					}
					break;
			case 3:
					switch (IDCode)
					{
					case 1:
						temp = "Camintonn Corporation";
						break;
					case 2:
						temp = "ISOA Incorporated";
						break;
					case 0x83:
						temp = "Agate Semiconductor";
						break;
					case 4:
						temp = "ADMtek Incorporated";
						break;
					case 0x85:
						temp = "HYPERTEC";
						break;
					case 0x86:
						temp = "Adhoc Technologies";
						break;
					case 7:
						temp = "MOSAID Technologies";
						break;
					case 8:
						temp = "Ardent Technologies";
						break;
					case 0x89:
						temp = "Switchcore";
						break;
					case 0x8A:
						temp = "Cisco Systems, Inc.";
						break;
					case 0x0B:
						temp = "Allayer Technologies";
						break;
					case 0x8C:
						temp = "WorkX AG (Wichman)";
						break;
					case 0x0D:
						temp = "Oasis Semiconductor";
						break;
					case 0x0E:
						temp = "Novanet Semiconductor";
						break;
					case 0x8F:
						temp = "E-M Solutions";
						break;
					case 0x10:
						temp = "Power General";
						break;
					case 0x91:
						temp = "Advanced Hardware Arch.";
						break;
					case 0x92:
						temp = "Inova Semiconductors GmbH";
						break;
					case 0x13:
						temp = "Telocity";
						break;
					case 0x94:
						temp = "Delkin Devices";
						break;
					case 0x15:
						temp = "Symagery Microsystems";
						break;
					case 0x16:
						temp = "C-Port Corporation";
						break;
					case 0x97:
						temp = "SiberCore Technologies";
						break;
					case 0x98:
						temp = "Southland Microsystems";
						break;
					case 0x19:
						temp = "Malleable Technologies";
						break;
					case 0x1A:
						temp = "Kendin Communications";
						break;
					case 0x9B:
						temp = "Great Technology Microcomputer";
						break;
					case 0x1C:
						temp = "Sanmina Corporation";
						break;
					case 0x9D:
						temp = "HADCO Corporation";
						break;
					case 0x9E:
						temp = "Corsair";
						break;
					case 0x1F:
						temp = "Actrans System Inc.";
						break;
					case 0x20:
						temp = "ALPHA Technologies";
						break;
					case 0xA1:
						temp = "Silicon Laboratories, Inc. (Cygnal)";
						break;
					case 0xA2:
						temp = "Artesyn Technologies";
						break;
					case 0x23:
						temp = "Align Manufacturing";
						break;
					case 0xA4:
						temp = "Peregrine Semiconductor";
						break;
					case 0x25:
						temp = "Chameleon Systems";
						break;
					case 0x26:
						temp = "Aplus Flash Technology";
						break;
					case 0xA7:
						temp = "MIPS Technologies";
						break;
					case 0xA8:
						temp = "Chrysalis ITS";
						break;
					case 0x29:
						temp = "ADTEC Corporation";
						break;
					case 0x2A:
						temp = "Kentron Technologies";
						break;
					case 0xAB:
						temp = "Win Technologies";
						break;
					case 0x2C:
						temp = "Tezzaron Semiconductor";
						break;
					case 0xAD:
						temp = "Extreme Packet Devices";
						break;
					case 0xAE:
						temp = "RF Micro Devices";
						break;
					case 0x2F:
						temp = "Siemens AG";
						break;
					case 0xB0:
						temp = "Sarnoff Corporation";
						break;
					case 0x31:
						temp = "Itautec SA";
						break;
					case 0x32:
						temp = "Radiata Inc.";
						break;
					case 0xB3:
						temp = "Benchmark Elect. (AVEX)";
						break;
					case 0x34:
						temp = "Legend";
						break;
					case 0xB5:
						temp = "SpecTek Incorporated";
						break;
					case 0xB6:
						temp = "Hi/fn";
						break;
					case 0x37:
						temp = "Enikia Incorporated";
						break;
					case 0x38:
						temp = "SwitchOn Networks";
						break;
					case 0xB9:
						temp = "AANetcom Incorporated";
						break;
					case 0xBA:
						temp = "Micro Memory Bank";
						break;
					case 0x3B:
						temp = "ESS Technology";
						break;
					case 0xBC:
						temp = "Virata Corporation";
						break;
					case 0x3D:
						temp = "Excess Bandwidth";
						break;
					case 0x3E:
						temp = "West Bay Semiconductor";
						break;
					case 0xBF:
						temp = "DSP Group";
						break;
					case 0x40:
						temp = "Newport Communications";
						break;
					case 0xC1:
						temp = "Chip2Chip Incorporated";
						break;
					case 0xC2:
						temp = "Phobos Corporation";
						break;
					case 0x43:
						temp = "Intellitech Corporation";
						break;
					case 0xC4:
						temp = "Nordic VLSI ASA";
						break;
					case 0x45:
						temp = "Ishoni Networks";
						break;
					case 0x46:
						temp = "Silicon Spice";
						break;
					case 0xC7:
						temp = "Alchemy Semiconductor";
						break;
					case 0xC8:
						temp = "Agilent Technologies";
						break;
					case 0x49:
						temp = "Centillium Communications";
						break;
					case 0x4A:
						temp = "W.L. Gore";
						break;
					case 0xCB:
						temp = "HanBit Electronics";
						break;
					case 0x4C:
						temp = "GlobeSpan";
						break;
					case 0xCD:
						temp = "Element 14";
						break;
					case 0xCE:
						temp = "Pycon";
						break;
					case 0x4F:
						temp = "Saifun Semiconductors";
						break;
					case 0xD0:
						temp = "Sibyte, Incorporated";
						break;
					case 0x51:
						temp = "MetaLink Technologies";
						break;
					case 0x52:
						temp = "Feiya Technology";
						break;
					case 0xD3:
						temp = "I&C Technology";
						break;
					case 0x54:
						temp = "Shikatronics";
						break;
					case 0xD5:
						temp = "Elektrobit";
						break;
					case 0xD6:
						temp = "Megic";
						break;
					case 0x57:
						temp = "Com-Tier";
						break;
					case 0x58:
						temp = "Malaysia Micro Solutions";
						break;
					case 0xD9:
						temp = "Hyperchip";
						break;
					case 0xDA:
						temp = "Gemstone Communications";
						break;
					case 0x5B:
						temp = "Anadigm (Anadyne)";
						break;
					case 0xDC:
						temp = "3ParData";
						break;
					case 0x5D:
						temp = "Mellanox Technologies";
						break;
					case 0x5E:
						temp = "Tenx Technologies";
						break;
					case 0xDF:
						temp = "Helix AG";
						break;
					case 0xE0:
						temp = "Domosys";
						break;
					case 0x61:
						temp = "Skyup Technology";
						break;
					case 0x62:
						temp = "HiNT Corporation";
						break;
					case 0xE3:
						temp = "Chiaro";
						break;
					case 0x64:
						temp = "MDT Technologies GmbH";
						break;
					case 0xE5:
						temp = "Exbit Technology A/S";
						break;
					case 0xE6:
						temp = "Integrated Technology Express";
						break;
					case 0X67:
						temp = "AVED Memory";
						break;
					case 0x68:
						temp = "Legerity";
						break;
					case 0xE9:
						temp = "Jasmine Networks";
						break;
					case 0xEA:
						temp = "Caspian Networks";
						break;
					case 0x6B:
						temp = "nCUBE";
						break;
					case 0xEC:
						temp = "Silicon Access Networks";
						break;
					case 0x6D:
						temp = "FDK Corporation";
						break;
					case 0x6E:
						temp = "High Bandwidth Access";
						break;
					case 0xEF:
						temp = "MultiLink Technology";
						break;
					case 0x70:
						temp = "BRECIS";
						break;
					case 0xF1:
						temp = "World Wide Packets";
						break;
					case 0xF2:
						temp = "APW";
						break;
					case 0x73:
						temp = "Chicory Systems";
						break;
					case 0xF4:
						temp = "Xstream Logic";
						break;
					case 0x75:
						temp = "Fast-Chip";
						break;
					case 0x76:
						temp = "Zucotto Wireless";
						break;
					case 0xF7:
						temp = "Realchip";
						break;
					case 0xF8:
						temp = "Galaxy Power";
						break;
					case 0x79:
						temp = "eSilicon";
						break;
					case 0x7A:
						temp = "Morphics Technology";
						break;
					case 0xFB:
						temp = "Accelerant Networks";
						break;
					case 0x7C:
						temp = "Silicon Wave";
						break;
					case 0xFD:
						temp = "SandCraft";
						break;
					case 0xFE:
						temp = "Elpida";
						break;
					default:
						break;
					}
				break;
			case 4:
					switch (IDCode)
					{
					case 1:
						temp = "Solectron";
						break;
					case 2:
						temp = "Optosys Technologies";
						break;
					case 0x83:
						temp = "Buffalo (Formerly Melco)";
						break;
					case 4:
						temp = "TriMedia Technologies";
						break;
					case 0x85:
						temp = "Cyan Technologies";
						break;
					case 0x86:
						temp = "Global Locate";
						break;
					case 7:
						temp = "Optillion";
						break;
					case 8:
						temp = "Terago Communications";
						break;
					case 0x89:
						temp = "Ikanos Communications";
						break;
					case 0x8A:
						temp = "Princeton Technology";
						break;
					case 0x0B:
						temp = "Nanya Technology";
						break;
					case 0x8C:
						temp = "Elite Flash Storage";
						break;
					case 0x0D:
						temp = "Mysticom";
						break;
					case 0x0E:
						temp = "LightSand Communications";
						break;
					case 0x8F:
						temp = "ATI Technologies";
						break;
					case 0x10:
						temp = "Agere Systems";
						break;
					case 0x91:
						temp = "NeoMagic";
						break;
					case 0x92:
						temp = "AuroraNetics";
						break;
					case 0x13:
						temp = "Golden Empire";
						break;
					case 0x94:
						temp = "Mushkin";
						break;
					case 0x15:
						temp = "Tioga Technologies";
						break;
					case 0x16:
						temp = "Netlist";
						break;
					case 0x97:
						temp = "TeraLogic";
						break;
					case 0x98:
						temp = "Cicada Semiconductor";
						break;
					case 0x19:
						temp = "Centon Electronics";
						break;
					case 0x1A:
						temp = "Tyco Electronics";
						break;
					case 0x9B:
						temp = "Magis Works";
						break;
					case 0x1C:
						temp = "Zettacom";
						break;
					case 0x9D:
						temp = "Cogency Semiconductor";
						break;
					case 0x9E:
						temp = "Chipcon AS";
						break;
					case 0x1F:
						temp = "Aspex Technology";
						break;
					case 0x20:
						temp = "F5 Networks";
						break;
					case 0xA1:
						temp = "Programmable Silicon Solutions";
						break;
					case 0xA2:
						temp = "ChipWrights";
						break;
					case 0x23:
						temp = "Acorn Networks";
						break;
					case 0xA4:
						temp = "Quicklogic";
						break;
					case 0x25:
						temp = "Kingmax Semiconductor";
						break;
					case 0x26:
						temp = "BOPS";
						break;
					case 0xA7:
						temp = "Flasys";
						break;
					case 0xA8:
						temp = "BitBlitz Communications";
						break;
					case 0x29:
						temp = "eMemory Technology";
						break;
					case 0x2A:
						temp = "Procket Networks";
						break;
					case 0xAB:
						temp = "Purple Ray";
						break;
					case 0x2C:
						temp = "Trebia Networks";
						break;
					case 0xAD:
						temp = "Delta Electronics";
						break;
					case 0xAE:
						temp = "Onex Communications";
						break;
					case 0x2F:
						temp = "Ample Communications";
						break;
					case 0xB0:
						temp = "Memory Experts Intl";
						break;
					case 0x31:
						temp = "Astute Networks";
						break;
					case 0x32:
						temp = "Azanda Network Devices";
						break;
					case 0xB3:
						temp = "Dibcom";
						break;
					case 0x34:
						temp = "Tekmos";
						break;
					case 0xB5:
						temp = "API NetWorks";
						break;
					case 0xB6:
						temp = "Bay Microsystems";
						break;
					case 0x37:
						temp = "Firecron Ltd";
						break;
					case 0x38:
						temp = "Resonext Communications";
						break;
					case 0xB9:
						temp = "Tachys Technologies";
						break;
					case 0xBA:
						temp = "Equator Technology";
						break;
					case 0x3B:
						temp = "Concept Computer";
						break;
					case 0xBC:
						temp = "SILCOM";
						break;
					case 0x3D:
						temp = "3Dlabs";
						break;
					case 0x3E:
						temp = "c¡¯t Magazine";
						break;
					case 0xBF:
						temp = "Sanera Systems";
						break;
					case 0x40:
						temp = "Silicon Packets";
						break;
					case 0xC1:
						temp = "Viasystems Group";
						break;
					case 0xC2:
						temp = "Simtek";
						break;
					case 0x43:
						temp = "Semicon Devices Singapore";
						break;
					case 0xC4:
						temp = "Satron Handelsges";
						break;
					case 0x45:
						temp = "Improv Systems";
						break;
					case 0x46:
						temp = "INDUSYS GmbH";
						break;
					case 0xC7:
						temp = "Corrent";
						break;
					case 0xC8:
						temp = "Infrant Technologies";
						break;
					case 0x49:
						temp = "Ritek Corp";
						break;
					case 0x4A:
						temp = "empowerTel Networks";
						break;
					case 0xCB:
						temp = "Hypertec";
						break;
					case 0x4C:
						temp = "Cavium Networks";
						break;
					case 0xCD:
						temp = "PLX Technology";
						break;
					case 0xCE:
						temp = "Massana Design";
						break;
					case 0x4F:
						temp = "Intrinsity";
						break;
					case 0xD0:
						temp = "Valence Semiconductor";
						break;
					case 0x51:
						temp = "Terawave Communications";
						break;
					case 0x52:
						temp = "IceFyre Semiconductor";
						break;
					case 0xD3:
						temp = "Primarion";
						break;
					case 0x54:
						temp = "Picochip Designs Ltd";
						break;
					case 0xD5:
						temp = "Silverback Systems";
						break;
					case 0xD6:
						temp = "Jade Star Technologies";
						break;
					case 0x57:
						temp = "Pijnenburg Securealink";
						break;
					case 0x58:
						temp = "takeMS - Ultron AG";
						break;
					case 0xD9:
						temp = "Cambridge Silicon Radio";
						break;
					case 0xDA:
						temp = "Swissbit";
						break;
					case 0x5B:
						temp = "Nazomi Communications";
						break;
					case 0xDC:
						temp = "eWave System";
						break;
					case 0x5D:
						temp = "Rockwell Collins";
						break;
					case 0x5E:
						temp = "Picocel Co. Ltd. (Paion)";
						break;
					case 0xDF:
						temp = "Alphamosaic Ltd.";
						break;
					case 0xE0:
						temp = "Sandburst";
						break;
					case 0x61:
						temp = "SiCon Video";
						break;
					case 0x62:
						temp = "NanoAmp Solutions";
						break;
					case 0xE3:
						temp = "Ericsson Technology";
						break;
					case 0x64:
						temp = "PrairieComm";
						break;
					case 0xE5:
						temp = "Mitac International";
						break;
					case 0xE6:
						temp = "Layer N Networks";
						break;
					case 0X67:
						temp = "MtekVision (Atsana)";
						break;
					case 0x68:
						temp = "Allegro Networks";
						break;
					case 0xE9:
						temp = "Marvell Semiconductors";
						break;
					case 0xEA:
						temp = "Netergy Microelectronic";
						break;
					case 0x6B:
						temp = "NVIDIA";
						break;
					case 0xEC:
						temp = "Internet Machines";
						break;
					case 0x6D:
						temp = "Memorysolution GmbH";
						break;
					case 0x6E:
						temp = "Litchfield Communication";
						break;
					case 0xEF:
						temp = "Accton Technology";
						break;
					case 0x70:
						temp = "Teradiant Networks";
						break;
					case 0xF1:
						temp = "Scaleo Chip";
						break;
					case 0xF2:
						temp = "Cortina Systems";
						break;
					case 0x73:
						temp = "RAM Components";
						break;
					case 0xF4:
						temp = "Raqia Networks";
						break;
					case 0x75:
						temp = "ClearSpeed";
						break;
					case 0x76:
						temp = "Matsushita Battery";
						break;
					case 0xF7:
						temp = "Xelerated";
						break;
					case 0xF8:
						temp = "SimpleTech";
						break;
					case 0x79:
						temp = "Utron Technology";
						break;
					case 0x7A:
						temp = "Astec International";
						break;
					case 0xFB:
						temp = "AVM gmbH";
						break;
					case 0x7C:
						temp = "Redux Communications";
						break;
					case 0xFD:
						temp = "Dot Hill Systems";
						break;
					case 0xFE:
						temp = "TeraChip";
						break;
					default:
						break;
					}
				break;
			case 5:
					switch (IDCode)
					{
					case 1:
						temp = "T-RAM Incorporated";
						break;
					case 2:
						temp = "Innovics Wireless";
						break;
					case 0x83:
						temp = "Teknovus";
						break;
					case 4:
						temp = "KeyEye Communications";
						break;
					case 0x85:
						temp = "Runcom Technologies";
						break;
					case 0x86:
						temp = "RedSwitch";
						break;
					case 7:
						temp = "Dotcast";
						break;
					case 8:
						temp = "Silicon Mountain Memory";
						break;
					case 0x89:
						temp = "Signia Technologies";
						break;
					case 0x8A:
						temp = "Pixim";
						break;
					case 0x0B:
						temp = "Galazar Networks";
						break;
					case 0x8C:
						temp = "White Electronic Designs";
						break;
					case 0x0D:
						temp = "Patriot Scientific";
						break;
					case 0x0E:
						temp = "Neoaxiom Corporation";
						break;
					case 0x8F:
						temp = "3Y Power Technology";
						break;
					case 0x10:
						temp = "Scaleo Chip";
						break;
					case 0x91:
						temp = "Potentia Power Systems";
						break;
					case 0x92:
						temp = "C-guys Incorporated";
						break;
					case 0x13:
						temp = "Digital Communications Technology Incorporated";
						break;
					case 0x94:
						temp = "Silicon-Based Technology";
						break;
					case 0x15:
						temp = "Fulcrum Microsystems";
						break;
					case 0x16:
						temp = "Positivo Informatica Ltd.";
						break;
					case 0x97:
						temp = "XIOtech Corporation";
						break;
					case 0x98:
						temp = "PortalPlayer";
						break;
					case 0x19:
						temp = "Zhiying Software";
						break;
					case 0x1A:
						temp = "ParkerVision, Inc.";
						break;
					case 0x9B:
						temp = "Phonex Broadband";
						break;
					case 0x1C:
						temp = "Skyworks Solutions";
						break;
					case 0x9D:
						temp = "Entropic Communications";
						break;
					case 0x9E:
						temp = "I¡¯M Intelligent Memory Ltd.";
						break;
					case 0x1F:
						temp = "Zensys A/S";
						break;
					case 0x20:
						temp = "Legend Silicon Corp";
						break;
					case 0xA1:
						temp = "Sci-worx GmbH";
						break;
					case 0xA2:
						temp = "SMSC (Standard Microsystems)";
						break;
					case 0x23:
						temp = "Renesas Electronics";
						break;
					case 0xA4:
						temp = "Raza Microelectronics";
						break;
					case 0x25:
						temp = "Phyworks";
						break;
					case 0x26:
						temp = "MediaTek";
						break;
					case 0xA7:
						temp = "Non-cents Productions";
						break;
					case 0xA8:
						temp = "US Modular";
						break;
					case 0x29:
						temp = "Wintegra Ltd.";
						break;
					case 0x2A:
						temp = "Mathstar";
						break;
					case 0xAB:
						temp = "StarCore";
						break;
					case 0x2C:
						temp = "Oplus Technologies";
						break;
					case 0xAD:
						temp = "Mindspeed";
						break;
					case 0xAE:
						temp = "Just Young Computer";
						break;
					case 0x2F:
						temp = "Radia Communications";
						break;
					case 0xB0:
						temp = "OCZ";
						break;
					case 0x31:
						temp = "Emuzed";
						break;
					case 0x32:
						temp = "LOGIC Devices";
						break;
					case 0xB3:
						temp = "Inphi Corporation";
						break;
					case 0x34:
						temp = "Quake Technologies";
						break;
					case 0xB5:
						temp = "Vixel";
						break;
					case 0xB6:
						temp = "SolusTek";
						break;
					case 0x37:
						temp = "Kongsberg Maritime";
						break;
					case 0x38:
						temp = "Faraday Technology";
						break;
					case 0xB9:
						temp = "Altium Ltd.";
						break;
					case 0xBA:
						temp = "Insyte";
						break;
					case 0x3B:
						temp = "ARM Ltd.";
						break;
					case 0xBC:
						temp = "DigiVision";
						break;
					case 0x3D:
						temp = "Vativ Technologies";
						break;
					case 0x3E:
						temp = "Endicott Interconnect Technologies";
						break;
					case 0xBF:
						temp = "Pericom";
						break;
					case 0x40:
						temp = "Bandspeed";
						break;
					case 0xC1:
						temp = "LeWiz Communications";
						break;
					case 0xC2:
						temp = "CPU Technology";
						break;
					case 0x43:
						temp = "Ramaxel Technology";
						break;
					case 0xC4:
						temp = "DSP Group";
						break;
					case 0x45:
						temp = "Axis Communications";
						break;
					case 0x46:
						temp = "Legacy Electronics";
						break;
					case 0xC7:
						temp = "Chrontel";
						break;
					case 0xC8:
						temp = "Powerchip Semiconductor";
						break;
					case 0x49:
						temp = "MobilEye Technologies";
						break;
					case 0x4A:
						temp = "Excel Semiconductor";
						break;
					case 0xCB:
						temp = "A-DATA Technology";
						break;
					case 0x4C:
						temp = "VirtualDigm";
						break;
					case 0xCD:
						temp = "G Skill Intl";
						break;
					case 0xCE:
						temp = "Quanta Computer";
						break;
					case 0x4F:
						temp = "Yield Microelectronics";
						break;
					case 0xD0:
						temp = "Afa Technologies";
						break;
					case 0x51:
						temp = "KINGBOX Technology Co. Ltd.";
						break;
					case 0x52:
						temp = "Ceva";
						break;
					case 0xD3:
						temp = "iStor Networks";
						break;
					case 0x54:
						temp = "Advance Modules";
						break;
					case 0xD5:
						temp = "Microsoft";
						break;
					case 0xD6:
						temp = "Open-Silicon";
						break;
					case 0x57:
						temp = "Goal Semiconductor";
						break;
					case 0x58:
						temp = "ARC International";
						break;
					case 0xD9:
						temp = "Simmtec";
						break;
					case 0xDA:
						temp = "Metanoia";
						break;
					case 0x5B:
						temp = "Key Stream";
						break;
					case 0xDC:
						temp = "Lowrance Electronics";
						break;
					case 0x5D:
						temp = "Adimos";
						break;
					case 0x5E:
						temp = "SiGe Semiconductor";
						break;
					case 0xDF:
						temp = "Fodus Communications";
						break;
					case 0xE0:
						temp = "Credence Systems Corp.";
						break;
					case 0x61:
						temp = "Genesis Microchip Inc.";
						break;
					case 0x62:
						temp = "Vihana, Inc.";
						break;
					case 0xE3:
						temp = "WIS Technologies";
						break;
					case 0x64:
						temp = "GateChange Technologies";
						break;
					case 0xE5:
						temp = "High Density Devices AS";
						break;
					case 0xE6:
						temp = "Synopsys";
						break;
					case 0X67:
						temp = "Gigaram";
						break;
					case 0x68:
						temp = "Enigma Semiconductor Inc.";
						break;
					case 0xE9:
						temp = "Century Micro Inc.";
						break;
					case 0xEA:
						temp = "Icera Semiconductor";
						break;
					case 0x6B:
						temp = "Mediaworks Integrated Systems";
						break;
					case 0xEC:
						temp = "O¡¯Neil Product Development";
						break;
					case 0x6D:
						temp = "Supreme Top Technology Ltd.";
						break;
					case 0x6E:
						temp = "MicroDisplay Corporation";
						break;
					case 0xEF:
						temp = "Team Group Inc.";
						break;
					case 0x70:
						temp = "Sinett Corporation";
						break;
					case 0xF1:
						temp = "Toshiba Corporation";
						break;
					case 0xF2:
						temp = "Tensilica";
						break;
					case 0x73:
						temp = "SiRF Technology";
						break;
					case 0xF4:
						temp = "Bacoc Inc.";
						break;
					case 0x75:
						temp = "SMaL Camera Technologies";
						break;
					case 0x76:
						temp = "Thomson SC";
						break;
					case 0xF7:
						temp = "Airgo Networks";
						break;
					case 0xF8:
						temp = "Wisair Ltd.";
						break;
					case 0x79:
						temp = "SigmaTel";
						break;
					case 0x7A:
						temp = "Arkados";
						break;
					case 0xFB:
						temp = "Compete IT gmbH Co. KG";
						break;
					case 0x7C:
						temp = "Eudar Technology Inc.";
						break;
					case 0xFD:
						temp = "Focus Enhancements";
						break;
					case 0xFE:
						temp = "Xyratex";
						break;
					default:
						break;
					}
				break;
			case 6:
					switch (IDCode)
					{
					case 1:
						temp = "Specular Networks";
						break;
					case 2:
						temp = "Patriot Memory (PDP Systems)";
						break;
					case 0x83:
						temp = "U-Chip Technology Corp.";
						break;
					case 4:
						temp = "Silicon Optix";
						break;
					case 0x85:
						temp = "Greenfield Networks";
						break;
					case 0x86:
						temp = "CompuRAM GmbH";
						break;
					case 7:
						temp = "Stargen, Inc.";
						break;
					case 8:
						temp = "NetCell Corporation";
						break;
					case 0x89:
						temp = "Excalibrus Technologies Ltd.";
						break;
					case 0x8A:
						temp = "SCM Microsystems";
						break;
					case 0x0B:
						temp = "Xsigo Systems, Inc.";
						break;
					case 0x8C:
						temp = "CHIPS & Systems Inc";
						break;
					case 0x0D:
						temp = "Tier 1 Multichip Solutions";
						break;
					case 0x0E:
						temp = "CWRL Labs";
						break;
					case 0x8F:
						temp = "Teradici";
						break;
					case 0x10:
						temp = "Gigaram, Inc.";
						break;
					case 0x91:
						temp = "g2 Microsystems";
						break;
					case 0x92:
						temp = "PowerFlash Semiconductor";
						break;
					case 0x13:
						temp = "P.A. Semi, Inc.";
						break;
					case 0x94:
						temp = "NovaTech Solutions, S.A.";
						break;
					case 0x15:
						temp = "c2 Microsystems, Inc.";
						break;
					case 0x16:
						temp = "Level5 Networks";
						break;
					case 0x97:
						temp = "COS Memory AG";
						break;
					case 0x98:
						temp = "Innovasic Semiconductor";
						break;
					case 0x19:
						temp = "02IC Co. Ltd";
						break;
					case 0x1A:
						temp = "Tabula, Inc.";
						break;
					case 0x9B:
						temp = "Crucial Technology";
						break;
					case 0x1C:
						temp = "Chelsio Communications";
						break;
					case 0x9D:
						temp = "Solarflare Communications";
						break;
					case 0x9E:
						temp = "Xambala Inc.";
						break;
					case 0x1F:
						temp = "EADS Astrium";
						break;
					case 0x20:
						temp = "Terra Semiconductor, Inc.";
						break;
					case 0xA1:
						temp = "Imaging Works, Inc.";
						break;
					case 0xA2:
						temp = "Astute Networks, Inc.";
						break;
					case 0x23:
						temp = "Tzero";
						break;
					case 0xA4:
						temp = "Emulex";
						break;
					case 0x25:
						temp = "Power-One";
						break;
					case 0x26:
						temp = "Pulse~LINK Inc.";
						break;
					case 0xA7:
						temp = "Hon Hai Precision Industry";
						break;
					case 0xA8:
						temp = "White Rock Networks Inc.";
						break;
					case 0x29:
						temp = "Telegent Systems USA, Inc.";
						break;
					case 0x2A:
						temp = "Atrua Technologies, Inc.";
						break;
					case 0xAB:
						temp = "Acbel Polytech Inc.";
						break;
					case 0x2C:
						temp = "eRide Inc.";
						break;
					case 0xAD:
						temp = "ULi Electronics Inc.";
						break;
					case 0xAE:
						temp = "Magnum Semiconductor Inc.";
						break;
					case 0x2F:
						temp = "neoOne Technology, Inc.";
						break;
					case 0xB0:
						temp = "Connex Technology, Inc.";
						break;
					case 0x31:
						temp = "Stream Processors, Inc.";
						break;
					case 0x32:
						temp = "Focus Enhancements";
						break;
					case 0xB3:
						temp = "Telecis Wireless, Inc.";
						break;
					case 0x34:
						temp = "uNav Microelectronics";
						break;
					case 0xB5:
						temp = "Tarari, Inc.";
						break;
					case 0xB6:
						temp = "Ambric, Inc.";
						break;
					case 0x37:
						temp = "Newport Media, Inc.";
						break;
					case 0x38:
						temp = "VMTS";
						break;
					case 0xB9:
						temp = "Enuclia Semiconductor, Inc.";
						break;
					case 0xBA:
						temp = "Virtium Technology Inc.";
						break;
					case 0x3B:
						temp = "Solid State System Co., Ltd.";
						break;
					case 0xBC:
						temp = "Kian Tech LLC";
						break;
					case 0x3D:
						temp = "Artimi";
						break;
					case 0x3E:
						temp = "Power Quotient International";
						break;
					case 0xBF:
						temp = "Avago Technologies";
						break;
					case 0x40:
						temp = "ADTechnology";
						break;
					case 0xC1:
						temp = "Sigma Designs";
						break;
					case 0xC2:
						temp = "SiCortex, Inc.";
						break;
					case 0x43:
						temp = "Ventura Technology Group";
						break;
					case 0xC4:
						temp = "eASIC";
						break;
					case 0x45:
						temp = "M.H.S. SAS";
						break;
					case 0x46:
						temp = "Micro Star International";
						break;
					case 0xC7:
						temp = "Rapport Inc.";
						break;
					case 0xC8:
						temp = "Makway International";
						break;
					case 0x49:
						temp = "Broad Reach Engineering Co.";
						break;
					case 0x4A:
						temp = "Semiconductor Mfg Intl Corp";
						break;
					case 0xCB:
						temp = "SiConnect";
						break;
					case 0x4C:
						temp = "FCI USA Inc.";
						break;
					case 0xCD:
						temp = "Validity Sensors";
						break;
					case 0xCE:
						temp = "Coney Technology Co. Ltd.";
						break;
					case 0x4F:
						temp = "Spans Logic";
						break;
					case 0xD0:
						temp = "Neterion Inc.";
						break;
					case 0x51:
						temp = "Qimonda";
						break;
					case 0x52:
						temp = "New Japan Radio Co. Ltd.";
						break;
					case 0xD3:
						temp = "Velogix";
						break;
					case 0x54:
						temp = "Montalvo Systems";
						break;
					case 0xD5:
						temp = "iVivity Inc.";
						break;
					case 0xD6:
						temp = "Walton Chaintech";
						break;
					case 0x57:
						temp = "AENEON";
						break;
					case 0x58:
						temp = "Lorom Industrial Co. Ltd.";
						break;
					case 0xD9:
						temp = "Radiospire Networks";
						break;
					case 0xDA:
						temp = "Sensio Technologies, Inc.";
						break;
					case 0x5B:
						temp = "Nethra Imaging";
						break;
					case 0xDC:
						temp = "Hexon Technology Pte Ltd";
						break;
					case 0x5D:
						temp = "CompuStocx(CSX)";
						break;
					case 0x5E:
						temp = "Methode Electronics, Inc.";
						break;
					case 0xDF:
						temp = "Connect One Ltd.";
						break;
					case 0xE0:
						temp = "Opulan Technologies";
						break;
					case 0x61:
						temp = "Septentrio NV";
						break;
					case 0x62:
						temp = "Goldenmars Technology Inc.";
						break;
					case 0xE3:
						temp = "Kreton Corporation";
						break;
					case 0x64:
						temp = "Cochlear Ltd.";
						break;
					case 0xE5:
						temp = "Altair Semiconductor";
						break;
					case 0xE6:
						temp = "NetEffect, Inc.";
						break;
					case 0X67:
						temp = "Spansion, Inc.";
						break;
					case 0x68:
						temp = "Taiwan Semiconductor Mfg";
						break;
					case 0xE9:
						temp = "Emphany Systems Inc.";
						break;
					case 0xEA:
						temp = "ApaceWave Technologies";
						break;
					case 0x6B:
						temp = "Mobilygen Corporation";
						break;
					case 0xEC:
						temp = "Tego";
						break;
					case 0x6D:
						temp = "Cswitch Corporation";
						break;
					case 0x6E:
						temp = "Haier (Beijing) IC Design Co.";
						break;
					case 0xEF:
						temp = "MetaRAM";
						break;
					case 0x70:
						temp = "Axel Electronics Co. Ltd.";
						break;
					case 0xF1:
						temp = "Tilera Corporation";
						break;
					case 0xF2:
						temp = "Aquantia";
						break;
					case 0x73:
						temp = "Vivace Semiconductor";
						break;
					case 0xF4:
						temp = "Redpine Signals";
						break;
					case 0x75:
						temp = "Octalica";
						break;
					case 0x76:
						temp = "InterDigital Communications";
						break;
					case 0xF7:
						temp = "Avant Technology";
						break;
					case 0xF8:
						temp = "Asrock, Inc.";
						break;
					case 0x79:
						temp = "Availink";
						break;
					case 0x7A:
						temp = "Quartics, Inc.";
						break;
					case 0xFB:
						temp = "Element CXI";
						break;
					case 0x7C:
						temp = "Innovaciones Microelectronicas";
						break;
					case 0xFD:
						temp = "VeriSilicon Microelectronics";
						break;
					case 0xFE:
						temp = "W5 Networks";
						break;
					default:
						break;
					}
				break;
			case 7:
					switch (IDCode)
					{
					case 1:
						temp = "MOVEKING";
						break;
					case 2:
						temp = "Mavrix Technology, Inc.";
						break;
					case 0x83:
						temp = "CellGuide Ltd.";
						break;
					case 4:
						temp = "Faraday Technology";
						break;
					case 0x85:
						temp = "Diablo Technologies, Inc.";
						break;
					case 0x86:
						temp = "Jennic";
						break;
					case 7:
						temp = "Octasic";
						break;
					case 8:
						temp = "Molex Incorporated";
						break;
					case 0x89:
						temp = "3Leaf Networks";
						break;
					case 0x8A:
						temp = "Bright Micron Technology";
						break;
					case 0x0B:
						temp = "Netxen";
						break;
					case 0x8C:
						temp = "NextWave Broadband Inc.";
						break;
					case 0x0D:
						temp = "DisplayLink";
						break;
					case 0x0E:
						temp = "ZMOS Technology";
						break;
					case 0x8F:
						temp = "Tec-Hill";
						break;
					case 0x10:
						temp = "Multigig, Inc.";
						break;
					case 0x91:
						temp = "Amimon";
						break;
					case 0x92:
						temp = "Euphonic Technologies, Inc.";
						break;
					case 0x13:
						temp = "BRN Phoenix";
						break;
					case 0x94:
						temp = "InSilica";
						break;
					case 0x15:
						temp = "Ember Corporation";
						break;
					case 0x16:
						temp = "Avexir Technologies Corporation";
						break;
					case 0x97:
						temp = "Echelon Corporation";
						break;
					case 0x98:
						temp = "Edgewater Computer Systems";
						break;
					case 0x19:
						temp = "XMOS Semiconductor Ltd.";
						break;
					case 0x1A:
						temp = "GENUSION, Inc.";
						break;
					case 0x9B:
						temp = "Memory Corp NV";
						break;
					case 0x1C:
						temp = "SiliconBlue Technologies";
						break;
					case 0x9D:
						temp = "Rambus Inc.";
						break;
					case 0x9E:
						temp = "Andes Technology Corporation";
						break;
					case 0x1F:
						temp = "Coronis Systems";
						break;
					case 0x20:
						temp = "Achronix Semiconductor";
						break;
					case 0xA1:
						temp = "Siano Mobile Silicon Ltd.";
						break;
					case 0xA2:
						temp = "Semtech Corporation";
						break;
					case 0x23:
						temp = "Pixelworks Inc.";
						break;
					case 0xA4:
						temp = "Gaisler Research AB";
						break;
					case 0x25:
						temp = "Teranetics";
						break;
					case 0x26:
						temp = "Toppan Printing Co. Ltd.";
						break;
					case 0xA7:
						temp = "Kingxcon";
						break;
					case 0xA8:
						temp = "Silicon Integrated Systems";
						break;
					case 0x29:
						temp = "I-O Data Device, Inc.";
						break;
					case 0x2A:
						temp = "NDS Americas Inc.";
						break;
					case 0xAB:
						temp = "Solomon Systech Limited";
						break;
					case 0x2C:
						temp = "On Demand Microelectronics";
						break;
					case 0xAD:
						temp = "Amicus Wireless Inc.";
						break;
					case 0xAE:
						temp = "SMARDTV SNC";
						break;
					case 0x2F:
						temp = "Comsys Communication Ltd.";
						break;
					case 0xB0:
						temp = "Movidia Ltd.";
						break;
					case 0x31:
						temp = "Javad GNSS, Inc.";
						break;
					case 0x32:
						temp = "Montage Technology Group";
						break;
					case 0xB3:
						temp = "Trident Microsystems";
						break;
					case 0x34:
						temp = "Super Talent";
						break;
					case 0xB5:
						temp = "Optichron, Inc.";
						break;
					case 0xB6:
						temp = "Future Waves UK Ltd.";
						break;
					case 0x37:
						temp = "SiBEAM, Inc.";
						break;
					case 0x38:
						temp = "Inicore,Inc.";
						break;
					case 0xB9:
						temp = "Virident Systems";
						break;
					case 0xBA:
						temp = "M2000, Inc.";
						break;
					case 0x3B:
						temp = "ZeroG Wireless, Inc.";
						break;
					case 0xBC:
						temp = "Gingle Technology Co. Ltd.";
						break;
					case 0x3D:
						temp = "Space Micro Inc.";
						break;
					case 0x3E:
						temp = "Wilocity";
						break;
					case 0xBF:
						temp = "Novafora, Inc.";
						break;
					case 0x40:
						temp = "iKoa Corporation";
						break;
					case 0xC1:
						temp = "ASint Technology";
						break;
					case 0xC2:
						temp = "Ramtron";
						break;
					case 0x43:
						temp = "Plato Networks Inc.";
						break;
					case 0xC4:
						temp = "IPtronics AS";
						break;
					case 0x45:
						temp = "Infinite-Memories";
						break;
					case 0x46:
						temp = "Parade Technologies Inc.";
						break;
					case 0xC7:
						temp = "Dune Networks";
						break;
					case 0xC8:
						temp = "GigaDevice Semiconductor";
						break;
					case 0x49:
						temp = "Modu Ltd.";
						break;
					case 0x4A:
						temp = "CEITEC";
						break;
					case 0xCB:
						temp = "Northrop Grumman";
						break;
					case 0x4C:
						temp = "XRONET Corporation";
						break;
					case 0xCD:
						temp = "Sicon Semiconductor AB";
						break;
					case 0xCE:
						temp = "Atla Electronics Co. Ltd.";
						break;
					case 0x4F:
						temp = "TOPRAM Technology";
						break;
					case 0xD0:
						temp = "Silego Technology Inc.";
						break;
					case 0x51:
						temp = "Kinglife";
						break;
					case 0x52:
						temp = "Ability Industries Ltd.";
						break;
					case 0xD3:
						temp = "Silicon Power Computer & Communications";
						break;
					case 0x54:
						temp = "Augusta Technology, Inc.";
						break;
					case 0xD5:
						temp = "Nantronics Semiconductors";
						break;
					case 0xD6:
						temp = "Hilscher Gesellschaft";
						break;
					case 0x57:
						temp = "Quixant Ltd.";
						break;
					case 0x58:
						temp = "Percello Ltd.";
						break;
					case 0xD9:
						temp = "NextIO Inc.";
						break;
					case 0xDA:
						temp = "Scanimetrics Inc.";
						break;
					case 0x5B:
						temp = "FS-Semi Company Ltd.";
						break;
					case 0xDC:
						temp = "Infinera Corporation";
						break;
					case 0x5D:
						temp = "SandForce Inc.";
						break;
					case 0x5E:
						temp = "Lexar Media";
						break;
					case 0xDF:
						temp = "Teradyne Inc.";
						break;
					case 0xE0:
						temp = "Memory Exchange Corp.";
						break;
					case 0x61:
						temp = "Suzhou Smartek Electronics";
						break;
					case 0x62:
						temp = "Avantium Corporation";
						break;
					case 0xE3:
						temp = "ATP Electronics Inc.";
						break;
					case 0x64:
						temp = "Valens Semiconductor Ltd";
						break;
					case 0xE5:
						temp = "Agate Logic, Inc.";
						break;
					case 0xE6:
						temp = "Netronome";
						break;
					case 0X67:
						temp = "Zenverge, Inc.";
						break;
					case 0x68:
						temp = "N-trig Ltd";
						break;
					case 0xE9:
						temp = "SanMax Technologies Inc.";
						break;
					case 0xEA:
						temp = "Contour Semiconductor Inc.";
						break;
					case 0x6B:
						temp = "TwinMOS";
						break;
					case 0xEC:
						temp = "Silicon Systems, Inc.";
						break;
					case 0x6D:
						temp = "V-Color Technology Inc.";
						break;
					case 0x6E:
						temp = "Certicom Corporation";
						break;
					case 0xEF:
						temp = "JSC ICC Milandr";
						break;
					case 0x70:
						temp = "PhotoFast Global Inc.";
						break;
					case 0xF1:
						temp = "InnoDisk Corporation";
						break;
					case 0xF2:
						temp = "Muscle Power";
						break;
					case 0x73:
						temp = "Energy Micro";
						break;
					case 0xF4:
						temp = "Innofidei";
						break;
					case 0x75:
						temp = "CopperGate Communications";
						break;
					case 0x76:
						temp = "Holtek Semiconductor Inc.";
						break;
					case 0xF7:
						temp = "Myson Century, Inc.";
						break;
					case 0xF8:
						temp = "FIDELIX";
						break;
					case 0x79:
						temp = "Red Digital Cinema";
						break;
					case 0x7A:
						temp = "Densbits Technology";
						break;
					case 0xFB:
						temp = "Zempro";
						break;
					case 0x7C:
						temp = "MoSys";
						break;
					case 0xFD:
						temp = "Provigent";
						break;
					case 0xFE:
						temp = "Triad Semiconductor, Inc.";
						break;
					default:
						break;
					}
				break;
			case 8:
					switch (IDCode)
					{
					case 1:
						temp = "Siklu Communication Ltd.";
						break;
					case 2:
						temp = "A Force Manufacturing Ltd.";
						break;
					case 0x83:
						temp = "Strontium";
						break;
					case 4:
						temp = "ALi Corp (Abilis Systems)";
						break;
					case 0x85:
						temp = "Siglead, Inc.";
						break;
					case 0x86:
						temp = "Ubicom, Inc.";
						break;
					case 7:
						temp = "Unifosa Corporation";
						break;
					case 8:
						temp = "Stretch, Inc.";
						break;
					case 0x89:
						temp = "Lantiq Deutschland GmbH";
						break;
					case 0x8A:
						temp = "Visipro.";
						break;
					case 0x0B:
						temp = "EKMemory";
						break;
					case 0x8C:
						temp = "Microelectronics Institute ZTE";
						break;
					case 0x0D:
						temp = "u-blox AG";
						break;
					case 0x0E:
						temp = "Carry Technology Co. Ltd.";
						break;
					case 0x8F:
						temp = "Nokia";
						break;
					case 0x10:
						temp = "King Tiger Technology";
						break;
					case 0x91:
						temp = "Sierra Wireless";
						break;
					case 0x92:
						temp = "HT Micron";
						break;
					case 0x13:
						temp = "Albatron Technology Co. Ltd.";
						break;
					case 0x94:
						temp = "Leica Geosystems AG";
						break;
					case 0x15:
						temp = "BroadLight";
						break;
					case 0x16:
						temp = "AEXEA";
						break;
					case 0x97:
						temp = "ClariPhy Communications, Inc.";
						break;
					case 0x98:
						temp = "Green Plug";
						break;
					case 0x19:
						temp = "Design Art Networks";
						break;
					case 0x1A:
						temp = "Mach Xtreme Technology Ltd.";
						break;
					case 0x9B:
						temp = "ATO Solutions Co. Ltd.";
						break;
					case 0x1C:
						temp = "Ramsta";
						break;
					case 0x9D:
						temp = "Greenliant Systems, Ltd.";
						break;
					case 0x9E:
						temp = "Teikon";
						break;
					case 0x1F:
						temp = "Antec Hadron";
						break;
					case 0x20:
						temp = "NavCom Technology, Inc.";
						break;
					case 0xA1:
						temp = "Shanghai Fudan Microelectronics";
						break;
					case 0xA2:
						temp = "Calxeda, Inc.";
						break;
					case 0x23:
						temp = "JSC EDC Electronics";
						break;
					case 0xA4:
						temp = "Kandit Technology Co. Ltd.";
						break;
					case 0x25:
						temp = "Ramos Technology";
						break;
					case 0x26:
						temp = "Goldenmars Technology";
						break;
					case 0xA7:
						temp = "XeL Technology Inc.";
						break;
					case 0xA8:
						temp = "Newzone Corporation";
						break;
					case 0x29:
						temp = "ShenZhen MercyPower Tech";
						break;
					case 0x2A:
						temp = "Nanjing Yihuo Technology";
						break;
					case 0xAB:
						temp = "Nethra Imaging Inc.";
						break;
					case 0x2C:
						temp = "SiTel Semiconductor BV";
						break;
					case 0xAD:
						temp = "SolidGear Corporation";
						break;
					case 0xAE:
						temp = "Topower Computer Ind Co Ltd.";
						break;
					case 0x2F:
						temp = "Wilocity";
						break;
					case 0xB0:
						temp = "Profichip GmbH";
						break;
					case 0x31:
						temp = "Gerad Technologies";
						break;
					case 0x32:
						temp = "Ritek Corporation";
						break;
					case 0xB3:
						temp = "Gomos Technology Limited";
						break;
					case 0x34:
						temp = "Memoright Corporation";
						break;
					case 0xB5:
						temp = "D-Broad, Inc.";
						break;
					case 0xB6:
						temp = "HiSilicon Technologies";
						break;
					case 0x37:
						temp = "Syndiant Inc.";
						break;
					case 0x38:
						temp = "Enverv Inc.";
						break;
					case 0xB9:
						temp = "Cognex";
						break;
					case 0xBA:
						temp = "Xinnova Technology Inc.";
						break;
					case 0x3B:
						temp = "Ultron AG";
						break;
					case 0xBC:
						temp = "Concord Idea Corporation";
						break;
					case 0x3D:
						temp = "AIM Corporation";
						break;
					case 0x3E:
						temp = "Lifetime Memory Products";
						break;
					case 0xBF:
						temp = "Ramsway";
						break;
					case 0x40:
						temp = "Recore Systems B.V.";
						break;
					case 0xC1:
						temp = "Haotian Jinshibo Science Tech";
						break;
					case 0xC2:
						temp = "Being Advanced Memory";
						break;
					case 0x43:
						temp = "Adesto Technologies";
						break;
					case 0xC4:
						temp = "Giantec Semiconductor, Inc.";
						break;
					case 0x45:
						temp = "HMD Electronics AG";
						break;
					case 0x46:
						temp = "Gloway International (HK).";
						break;
					case 0xC7:
						temp = "Kingcore";
						break;
					case 0xC8:
						temp = "Anucell Technology Holding";
						break;
					case 0x49:
						temp = "Accord Software & Systems Pvt. Ltd.";
						break;
					case 0x4A:
						temp = "Active-Semi Inc.";
						break;
					case 0xCB:
						temp = "Denso Corporation";
						break;
					case 0x4C:
						temp = "TLSI Inc.";
						break;
					case 0xCD:
						temp = "Qidan";
						break;
					case 0xCE:
						temp = "Mustang";
						break;
					case 0x4F:
						temp = "Orca Systems";
						break;
					case 0xD0:
						temp = "Passif Semiconductor";
						break;
					case 0x51:
						temp = "GigaDevice Semiconductor (Beijing) Inc.";
						break;
					case 0x52:
						temp = "Memphis Electronic";
						break;
					case 0xD3:
						temp = "Beckhoff Automation GmbH";
						break;
					case 0x54:
						temp = "Harmony Semiconductor Corp";
						break;
					case 0xD5:
						temp = "Air Computers SRL";
						break;
					case 0xD6:
						temp = "TMT Memory";
						break;
					case 0x57:
						temp = "Eorex Corporation";
						break;
					case 0x58:
						temp = "Xingtera";
						break;
					case 0xD9:
						temp = "Netsol";
						break;
					case 0xDA:
						temp = "Bestdon Technology Co. Ltd.";
						break;
					case 0x5B:
						temp = "Baysand Inc.";
						break;
					case 0xDC:
						temp = "Uroad Technology Co. Ltd.";
						break;
					case 0x5D:
						temp = "Wilk Elektronik S.A.";
						break;
					case 0x5E:
						temp = "AAI";
						break;
					case 0xDF:
						temp = "Harman";
						break;
					case 0xE0:
						temp = "Berg Microelectronics Inc.";
						break;
					case 0x61:
						temp = "ASSIA, Inc.";
						break;
					case 0x62:
						temp = "Visiontek Products LLC";
						break;
					case 0xE3:
						temp = "OCMEMORY";
						break;
					case 0x64:
						temp = "Welink Solution Inc.";
						break;
					case 0xE5:
						temp = "Shark Gaming";
						break;
					case 0xE6:
						temp = "Avalanche Technology";
						break;
					case 0X67:
						temp = "R&D Center ELVEES OJSC";
						break;
					case 0x68:
						temp = "KingboMars Technology Co. Ltd.";
						break;
					case 0xE9:
						temp = "High Bridge Solutions Industria Eletronica";
						break;
					case 0xEA:
						temp = "Transcend Technology Co. Ltd.";
						break;
					case 0x6B:
						temp = "Everspin Technologies";
						break;
					case 0xEC:
						temp = "Hon-Hai Precision";
						break;
					case 0x6D:
						temp = "Smart Storage Systems";
						break;
					case 0x6E:
						temp = "Toumaz Group";
						break;
					case 0xEF:
						temp = "Zentel Electronics Corporation";
						break;
					case 0x70:
						temp = "Panram International Corporation";
						break;
					case 0xF1:
						temp = "Silicon Space Technology";
						break;
					case 0xF2:
						temp = "LITE-ON IT Corporation";
						break;
					case 0x73:
						temp = "Inuitive";
						break;
					case 0xF4:
						temp = "HMicro";
						break;
					case 0x75:
						temp = "BittWare, Inc.";
						break;
					case 0x76:
						temp = "GLOBALFOUNDRIES";
						break;
					case 0xF7:
						temp = "ACPI Digital Co. Ltd.";
						break;
					case 0xF8:
						temp = "Annapurna Labs";
						break;
					case 0x79:
						temp = "AcSiP Technology Corporation";
						break;
					case 0x7A:
						temp = "Idea! Electronic Systems";
						break;
					case 0xFB:
						temp = "Gowe Technology Co. Ltd.";
						break;
					case 0x7C:
						temp = "Hermes Testing Solutions, Inc.";
						break;
					case 0xFD:
						temp = "Positivo BGH";
						break;
					case 0xFE:
						temp = "Intelligence Silicon Technology";
						break;
					default:
						break;
					}
				break;
			case 9:
					switch (IDCode)
					{
					case 1:
						temp = "3D PLUS";
						break;
					case 2:
						temp = "Diehl Aerospace";
						break;
					case 0x83:
						temp = "Fairchild";
						break;
					case 4:
						temp = "Mercury Systems";
						break;
					case 0x85:
						temp = "Sonics, Inc.";
						break;
					case 0x86:
						temp = "GE Intelligent Platforms GmbH & Co.";
						break;
					case 7:
						temp = "Shenzhen Jinge Information Co. Ltd.";
						break;
					case 8:
						temp = "SCWW";
						break;
					case 0x89:
						temp = "Silicon Motion Inc.";
						break;
					case 0x8A:
						temp = "Anurag";
						break;
					case 0x0B:
						temp = "King Kong";
						break;
					case 0x8C:
						temp = "FROM30 Co. Ltd.";
						break;
					case 0x0D:
						temp = "Gowin Semiconductor Corp";
						break;
					case 0x0E:
						temp = "Fremont Micro Devices Ltd.";
						break;
					case 0x8F:
						temp = "Ericsson Modems";
						break;
					case 0x10:
						temp = "Exelis";
						break;
					case 0x91:
						temp = "Satixfy Ltd.";
						break;
					case 0x92:
						temp = "Galaxy Microsystems Ltd.";
						break;
					case 0x13:
						temp = "Gloway International Co. Ltd.";
						break;
					case 0x94:
						temp = "Lab";
						break;
					case 0x15:
						temp = "Smart Energy Instruments";
						break;
					case 0x16:
						temp = "Approved Memory Corporation";
						break;
					case 0x97:
						temp = "Axell Corporation";
						break;
					case 0x98:
						temp = "Essencore Limited";
						break;
					case 0x19:
						temp = "Phytium";
						break;
					case 0x1A:
						temp = "Xi¡¯an UnilC Semiconductors Co., Ltd.";
						break;
					case 0x9B:
						temp = "Ambiq Micro";
						break;
					case 0x1C:
						temp = "eveRAM Technology, Inc.";
						break;
					case 0x9D:
						temp = "Infomax";
						break;
					case 0x9E:
						temp = "Butterfly Network, Inc.";
						break;
					case 0x1F:
						temp = "Shenzhen City Gcai Electronics";
						break;
					case 0x20:
						temp = "Stack Devices Corporation";
						break;
					case 0xA1:
						temp = "ADK Media Group";
						break;
					case 0xA2:
						temp = "TSP Global Co., Ltd.";
						break;
					case 0x23:
						temp = "HighX";
						break;
					case 0xA4:
						temp = "Shenzhen Elicks Technology";
						break;
					case 0x25:
						temp = "ISSI/Chingis";
						break;
					case 0x26:
						temp = "Google,Inc.";
						break;
					case 0xA7:
						temp = "Dasima International Development";
						break;
					case 0xA8:
						temp = "Leahkinn Technology Limited";
						break;
					case 0x29:
						temp = "HIMA Paul Hildebrandt GmbH Co KG";
						break;
					case 0x2A:
						temp = "Keysight Technologies";
						break;
					case 0xAB:
						temp = "Techcomp International (Fastable)";
						break;
					case 0x2C:
						temp = "Ancore Technology Corporation";
						break;
					case 0xAD:
						temp = "Nuvoton";
						break;
					case 0xAE:
						temp = "Korea Uhbele International Group Ltd.";
						break;
					case 0x2F:
						temp = "Ikegami Tsushinki Co Ltd.";
						break;
					case 0xB0:
						temp = "RelChip, Inc.";
						break;
					case 0x31:
						temp = "Baikal Electronics";
						break;
					case 0x32:
						temp = "Nemostech Inc.";
						break;
					case 0xB3:
						temp = "Memorysolution GmbH";
						break;
					case 0x34:
						temp = "Silicon Integrated Systems Corporation";
						break;
					case 0xB5:
						temp = "Xiede";
						break;
					case 0xB6:
						temp = "Multilaser Components";
						break;
					case 0x37:
						temp = "Flash Chi";
						break;
					case 0x38:
						temp = "Jone";
						break;
					case 0xB9:
						temp = "GCT Semiconductor Inc.";
						break;
					case 0xBA:
						temp = "Hong Kong Zetta Device Technology";
						break;
					case 0x3B:
						temp = "Unimemory Technology(s) Pte Ltd.";
						break;
					case 0xBC:
						temp = "Cuso";
						break;
					case 0x3D:
						temp = "Kuso";
						break;
					case 0x3E:
						temp = "Uniquify Inc.";
						break;
					case 0xBF:
						temp = "Skymedi Corporation";
						break;
					case 0x40:
						temp = "Core Chance Co. Ltd.";
						break;
					case 0xC1:
						temp = "Tekism Co. Ltd.";
						break;
					case 0xC2:
						temp = "Seagate Technology PLC";
						break;
					case 0x43:
						temp = "Hong Kong Gaia Group Co. Limited";
						break;
					case 0xC4:
						temp = "Gigacom Semiconductor LLC";
						break;
					case 0x45:
						temp = "V2 Technologies";
						break;
					case 0x46:
						temp = "TLi";
						break;
					case 0xC7:
						temp = "Neotion";
						break;
					case 0xC8:
						temp = "Lenovo";
						break;
					case 0x49:
						temp = "Shenzhen Zhongteng Electronic Corp. Ltd.";
						break;
					case 0x4A:
						temp = "Compound Photonics";
						break;
					case 0xCB:
						temp = "in2H2 inc";
						break;
					case 0x4C:
						temp = "Shenzhen Pango Microsystems Co. Ltd";
						break;
					case 0xCD:
						temp = "Vasekey";
						break;
					case 0xCE:
						temp = "Cal-Comp Industria de Semicondutores";
						break;
					case 0x4F:
						temp = "Eyenix Co., Ltd.";
						break;
					case 0xD0:
						temp = "Heoriady";
						break;
					case 0x51:
						temp = "Accelerated Memory Production Inc.";
						break;
					case 0x52:
						temp = "INVECAS, Inc.";
						break;
					case 0xD3:
						temp = "AP Memory";
						break;
					case 0x54:
						temp = "Douqi Technology";
						break;
					case 0xD5:
						temp = "Etron Technology, Inc.";
						break;
					case 0xD6:
						temp = "Indie Semiconductor";
						break;
					case 0x57:
						temp = "Socionext Inc.";
						break;
					case 0x58:
						temp = "HGST";
						break;
					case 0xD9:
						temp = "EVGA";
						break;
					case 0xDA:
						temp = "Audience Inc.";
						break;
					case 0x5B:
						temp = "EpicGear";
						break;
					case 0xDC:
						temp = "Vitesse Enterprise Co.";
						break;
					case 0x5D:
						temp = "Foxtronn International Corporation";
						break;
					case 0x5E:
						temp = "Bretelon Inc.";
						break;
					case 0xDF:
						temp = "Graphcore";
						break;
					case 0xE0:
						temp = "Eoplex Inc.";
						break;
					case 0x61:
						temp = "MaxLinear, Inc.";
						break;
					case 0x62:
						temp = "ETA Devices";
						break;
					case 0xE3:
						temp = "LOKI";
						break;
					case 0x64:
						temp = "IMS Electronics Co., Ltd.";
						break;
					case 0xE5:
						temp = "Dosilicon Co., Ltd.";
						break;
					case 0xE6:
						temp = "Dolphin Integration";
						break;
					case 0X67:
						temp = "Shenzhen Mic Electronics";
						break;
					case 0x68:
						temp = "Boya Microelectronics Inc.";
						break;
					case 0xE9:
						temp = "Geniachip (Roche)";
						break;
					case 0xEA:
						temp = "Axign";
						break;
					case 0x6B:
						temp = "Kingred Electronic Technology Ltd.";
						break;
					case 0xEC:
						temp = "Chao Yue Zhuo Computer Business Dept.";
						break;
					case 0x6D:
						temp = "Guangzhou Si Nuo Electronic Technology.";
						break;
					case 0x6E:
						temp = "Crocus Technology Inc.";
						break;
					case 0xEF:
						temp = "Creative Chips GmbH";
						break;
					case 0x70:
						temp = "GE Aviation Systems LLC.";
						break;
					case 0xF1:
						temp = "Asgard";
						break;
					case 0xF2:
						temp = "Good Wealth Technology Ltd.";
						break;
					case 0x73:
						temp = "TriCor Technologies";
						break;
					case 0xF4:
						temp = "Nova-Systems GmbH";
						break;
					case 0x75:
						temp = "JUHOR";
						break;
					case 0x76:
						temp = "Zhuhai Douke Commerce Co. Ltd.";
						break;
					case 0xF7:
						temp = "DSL Memory";
						break;
					case 0xF8:
						temp = "Anvo-Systems Dresden GmbH";
						break;
					case 0x79:
						temp = "Realtek";
						break;
					case 0x7A:
						temp = "AltoBeam";
						break;
					case 0xFB:
						temp = "Wave Computing";
						break;
					case 0x7C:
						temp = "Beijing TrustNet Technology Co Ltd";
						break;
					case 0xFD:
						temp = "Innovium, Inc.";
						break;
					case 0xFE:
						temp = "Starsway Technology Limited";
						break;
					default:
						break;
					}
				break;
			case 10:
					switch (IDCode)
					{
					case 1:
						temp = "Weltronics Co. LTD";
						break;
					case 2:
						temp = "VMware, Inc.";
						break;
					case 0x83:
						temp = "Hewlett Packard Enterprise";
						break;
					case 4:
						temp = "INTENSO";
						break;
					case 0x85:
						temp = "Puya Semiconductor";
						break;
					case 0x86:
						temp = "MEMORFI";
						break;
					case 7:
						temp = "MSC Technologies GmbH";
						break;
					case 8:
						temp = "Txrui";
						break;
					case 0x89:
						temp = "SiFive, Inc.";
						break;
					case 0x8A:
						temp = "Spreadtrum Communications";
						break;
					case 0x0B:
						temp = "XTX Technology Limited";
						break;
					case 0x8C:
						temp = "UMAX Technology";
						break;
					case 0x0D:
						temp = "Shenzhen Yong Sheng Technology";
						break;
					case 0x0E:
						temp = "SNOAMOO (Shenzhen Kai Zhuo Yue)";
						break;
					case 0x8F:
						temp = "Daten Tecnologia LTDA";
						break;
					case 0x10:
						temp = "Shenzhen XinRuiYan Electronics";
						break;
					case 0x91:
						temp = "Eta Compute";
						break;
					case 0x92:
						temp = "Energous";
						break;
					case 0x13:
						temp = "Raspberry Pi Trading Ltd.";
						break;
					case 0x94:
						temp = "Shenzhen Chixingzhe Tech Co. Ltd.";
						break;
					case 0x15:
						temp = "Silicon Mobility";
						break;
					case 0x16:
						temp = "IQ-Analog Corporation";
						break;
					case 0x97:
						temp = "Uhnder, Inc.";
						break;
					case 0x98:
						temp = "Impinj";
						break;
					case 0x19:
						temp = "DEPO Computers";
						break;
					case 0x1A:
						temp = "Nespeed Sysems";
						break;
					case 0x9B:
						temp = "Yangtze Memory Technologies Co. Ltd";
						break;
					case 0x1C:
						temp = "MemxPro Inc.";
						break;
					case 0x9D:
						temp = "Tammuz Co., Ltd.";
						break;
					case 0x9E:
						temp = "Allwinner Technology";
						break;
					case 0x1F:
						temp = "Shenzhen City Futian District Qing Xuan Tong Computer Trading Firm";
						break;
					case 0x20:
						temp = "XMC";
						break;
					case 0xA1:
						temp = "Teclast";
						break;
					case 0xA2:
						temp = "Maxsun";
						break;
					case 0x23:
						temp = "Haiguang Integrated Circuit Design";
						break;
					case 0xA4:
						temp = "RamCENTER Technology";
						break;
					case 0x25:
						temp = "Phison Electronics Corporation";
						break;
					case 0x26:
						temp = "Guizhou Huaxintong Semi-Conductor";
						break;
					case 0xA7:
						temp = "Network Intelligence";
						break;
					case 0xA8:
						temp = "Continental Technology (Holdings)";
						break;
					case 0x29:
						temp = "Guangzhou Huayan Suning Electronic";
						break;
					case 0x2A:
						temp = "Guangzhou Zhouji Electronic Co., Ltd.";
						break;
					case 0xAB:
						temp = "Shenzhen Giant Hui Kang Tech Co. Ltd";
						break;
					case 0x2C:
						temp = "Shenzhen Yilong Innovative Co. Ltd.";
						break;
					case 0xAD:
						temp = "Neo Forza";
						break;
					case 0xAE:
						temp = "Lyontek Inc.";
						break;
					case 0x2F:
						temp = "Shanghai Kuxin Microelectronics Ltd.";
						break;
					case 0xB0:
						temp = "Shenzhen Larix Technology Co. Ltd.";
						break;
					case 0x31:
						temp = "Qbit Semiconductor Ltd.";
						break;
					case 0x32:
						temp = "Insignis Technology Corporation";
						break;
					case 0xB3:
						temp = "Lanson Memory Co. Ltd.";
						break;
					case 0x34:
						temp = "Shenzhen Superway Electronics Co Ltd";
						break;
					case 0xB5:
						temp = "Canaan-Creative Co. Ltd.";
						break;
					case 0xB6:
						temp = "Black Diamond Memory";
						break;
					case 0x37:
						temp = "Shenzhen City Parker Baking Electronics";
						break;
					case 0x38:
						temp = "Shenzhen Baihong Technology Co. Ltd.";
						break;
					case 0xB9:
						temp = "GEO Semiconductors";
						break;
					case 0xBA:
						temp = "DTG NEO Scientific Ltd.";
						break;
					case 0x3B:
						temp = "Artery Technology Co. Ltd.";
						break;
					case 0xBC:
						temp = "Jinyu";
						break;
					case 0x3D:
						temp = "ShenzhenYing Chi Technology Development";
						break;
					case 0x3E:
						temp = "Shenzhen Pengcheng Xin Technology";
						break;
					case 0xBF:
						temp = "Pegasus Semiconductor (Shanghai) Co.";
						break;
					case 0x40:
						temp = "Mythic, Inc.";
						break;
					case 0xC1:
						temp = "Elmos Semiconductor AG";
						break;
					case 0xC2:
						temp = "Kllisre";
						break;
					case 0x43:
						temp = "Shenzhen Yuan Chuang Wei Tech";
						break;
					case 0xC4:
						temp = "Shenzhen Xingmem Technology Corp.";
						break;					
					default:
						break;
					}
				break;
			default:	break;
			}
			return temp;
		}
#pragma endregion
		bool MemoryLib::ReadSPD(const DIMMType types, const USHORT DIMMId, PVOID64& spd, const int len, const ULONG SmbusControlBase)
		{
			if (!m_data.get())
				return false;
			return this->m_data->ReadSPD(types, DIMMId, spd, len, SmbusControlBase);
		}


		MemoryLib::~MemoryLib()
		{
		}

#pragma region Export Functions
		const std::vector<std::pair<ULONG, std::pair<ULONG, DIMMType>>>& GetMemoryDeviceInfo()
		{
			return MemoryLib::Instance()->GetMemoryDeviceInfo();
		}

		const std::vector<std::pair<ULONG, DDR3_INFO>>& GetMemoryDDR3SPDInfo()
		{
			return MemoryLib::Instance()->GetMemoryDDR3Info();
		}

		const std::vector<std::pair<ULONG, DDR4_INFO>>& GetMemoryDDR4SPDInfo()
		{
			return MemoryLib::Instance()->GetMemoryDDR4Info();
		}

		const std::vector<std::pair<ULONG, MemoryData>>& GetMemoryGlobalInfomation()
		{
			return MemoryLib::Instance()->GetMemoryGlobalInfomation();
		}

		void UpdateData()
		{
			MemoryLib::Instance()->UpdateData();
		}
#pragma endregion


	}
}


