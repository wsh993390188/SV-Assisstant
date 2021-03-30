#pragma once
#include "memorydefination.h"
namespace Hardware
{
	namespace Memory
	{
		const std::string SPDManufacturer(const BYTE Bank, const BYTE IDCode);
		template<class T>
		class ExecMemory
		{
		public:
			explicit ExecMemory(T& spd) :spd(spd) {}
			virtual void Execute(MemoryCommonInformation& data)
			{
				char part_number[20] = {};
				memcpy(part_number, spd.DIMM_Detail.PartNumber, 18);
				data.PartNumber = part_number;
				Utils::trim(data.PartNumber);
				memset(part_number, 0, 20);

				sprintf_s(part_number, "%02X%02X%02X%02X", spd.DIMM_Detail.SerialNumber[0], spd.DIMM_Detail.SerialNumber[1], spd.DIMM_Detail.SerialNumber[2], spd.DIMM_Detail.SerialNumber[3]);
				data.SerialNumber = part_number;
				Utils::trim(data.SerialNumber);

				switch (spd.DIMM_Detail.sDRAMType)
				{
				case 0x0B:
					data.ModuleType = "DDR3 ";
					break;
				case 0x0C:
					data.ModuleType = "DDR4 ";
					break;
				case 0x0E:
					data.ModuleType = "DDR4E ";
					break;
				case 0x0F:
					data.ModuleType = "LPDDR3 ";
					break;
				case 0x10:
					data.ModuleType = "LPDDR4 ";
					break;
				default:
					break;
				}
				switch (spd.DIMM_Detail.sModuleType.bits.Module_Type)
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

				data.DRAMManufacturer = SPDManufacturer(spd.DIMM_Detail.DRAMManufacturerIDLeast & 0x7F, spd.DIMM_Detail.DRAMManufacturerIDMost);
				data.ModuleManufacturer = SPDManufacturer(spd.DIMM_Detail.ModuleManufacturerIDLeast & 0x7F, spd.DIMM_Detail.ModuleManufacturerIDMost);

				int year = spd.DIMM_Detail.ManufacturingDate[0], week = spd.DIMM_Detail.ManufacturingDate[1];
				memset(part_number, 0, 20);
				if (week != 0 && year != 0)
				{
					sprintf_s(part_number, "%02X\\%02X", week, year);
					data.ProductDate = part_number;
				}

				// 				if (spd.DIMM_Detail.sBusWidth.bits.Bus_width_extension == 1)
				// 					data.SupportsECC = true;
				// 				else
				// 					data.SupportsECC = false;
			}
		protected:
			std::add_lvalue_reference_t<T> spd;
		};

		class ExecMemoryDDR3 final : public ExecMemory<DDR3_INFO>
		{
		public:
			template<class T>
			explicit ExecMemoryDDR3(T& spd) : ExecMemory(spd) {}
			void Execute(MemoryCommonInformation& data) override;
		};

		class ExecMemoryDDR4 final : public ExecMemory<DDR4_INFO>
		{
		public:
			template<class T>
			explicit ExecMemoryDDR4(T& spd) : ExecMemory(spd) {}
			void Execute(MemoryCommonInformation& data) override;
		};
	}
}
