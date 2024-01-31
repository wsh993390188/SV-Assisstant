#include "stdafx.h"
#include <bitset>
#include "AmdCPU.h"
#include "CPUWMI.h"
#include "PCIHandle.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

#pragma region CPUID Offset
//L1 Cache and TLB Information CPUID = 0x80000005
//EAX
#define INSTRUCTION_TLB_SIZE				0x000000FF
#define INSTRUCTION_TLB_ASSOC				0x0000FF00
#define DATA_TLB_SIZE						0x00FF0000
#define DATA_TLB_ASSOC						0xFF000000
//EBX
#define KB4_INSTRUCTION_TLB_SIZE			0x000000FF
#define KB4_INSTRUCTION_TLB_ASSOC			0x0000FF00
#define KB4_DATA_TLB_SIZE					0x00FF0000
#define KB4_DATA_TLB_ASSOC					0xFF000000
//ECX
#define L1_DATA_CACHE_SIZE					0xFF000000
#define L1_DATA_CACHE_ASSOC					0x00FF0000
#define L1_DATA_CACHE_LINE_PER_TAG			0x0000FF00
#define L1_DATA_CACHE_LINE_SIZE				0x000000FF
//EDX
#define L1_INSTRUCTION_CACHE_SIZE			0xFF000000
#define L1_INSTRUCTION_CACHE_ASSOC			0x00FF0000
#define L1_INSTRUCTION_CACHE_LINE_PER_TAG	0x0000FF00
#define L1_INSTRUCTION_CACHE_LINE_SIZE		0x000000FF

//L2 Cache and TLB and L3 Cache Information CPUID = 0x80000006
//EAX
#define L2_INSTRUCTION_TLB_ASSOC			0x0000F000
#define L2_INSTRUCTION_TLB_SIZE				0x00000FFF
#define L2_DATA_TLB_SIZE					0x0FFF0000
#define L2_DATA_TLB_ASSOC					0xF0000000
//EBX
#define KB4_L2_INSTRUCTION_TLB_SIZE			0x00000FFF
#define KB4_L2_INSTRUCTION_TLB_ASSOC		0x0000F000
#define KB4_L2_DATA_TLB_SIZE				0x0FFF0000
#define KB4_L2_DATA_TLB_ASSOC				0xF0000000
//ECX
#define L2_DATA_CACHE_SIZE					0xFFFF0000
#define L2_DATA_CACHE_ASSOC					0x0000F000
#define L2_DATA_CACHE_LINE_PER_TAG			0x00000F00
#define L2_DATA_CACHE_LINE_SIZE				0x000000FF
//EDX
#define L3_INSTRUCTION_CACHE_SIZE			0xFFFC0000
#define L3_INSTRUCTION_CACHE_ASSOC			0x0000F000
#define L3_INSTRUCTION_CACHE_LINE_PER_TAG	0x00000F00
#define L3_INSTRUCTION_CACHE_LINE_SIZE		0x000000FF
#pragma endregion

namespace
{
	// Todo AMD 动态信息
	unsigned int AMD_L2_L3_Way_Associativity(unsigned int value)
	{
		switch (value) {
		case 0x6:
			return (8);
		case 0x8:
		case 0x9:
			return (16);
		case 0xa:
			return (32);
		case 0xb:
			return (48);
		case 0xc:
			return (64);
		case 0xd:
			return (96);
		case 0xe:
			return (128);
		default:
			return (value);
		}
	}

	constexpr auto MSR_FAMILY_17H_P_STATE = 0xC0010293;
	constexpr auto MSR_FAMILY_10H_P_STATE = 0xC0010071;

	bool CheckAMDPStatusDeviceIsExist(const uint32_t Family, const uint32_t Model)
	{
		constexpr auto FAMILY_10H_MISCELLANEOUS_CONTROL_DEVICE_ID = 0x1203;
		constexpr auto FAMILY_11H_MISCELLANEOUS_CONTROL_DEVICE_ID = 0x1303;
		constexpr auto FAMILY_12H_MISCELLANEOUS_CONTROL_DEVICE_ID = 0x1703;
		constexpr auto FAMILY_14H_MISCELLANEOUS_CONTROL_DEVICE_ID = 0x1703;
		constexpr auto FAMILY_15H_MODEL_00_MISC_CONTROL_DEVICE_ID = 0x1603;
		constexpr auto FAMILY_15H_MODEL_10_MISC_CONTROL_DEVICE_ID = 0x1403;
		constexpr auto FAMILY_15H_MODEL_30_MISC_CONTROL_DEVICE_ID = 0x141D;
		constexpr auto FAMILY_16H_MODEL_00_MISC_CONTROL_DEVICE_ID = 0x1533;
		constexpr auto FAMILY_16H_MODEL_30_MISC_CONTROL_DEVICE_ID = 0x1583;
		DWORD miscellaneousControlDeviceId = 0;
		switch (Family) {
		case 0x10: miscellaneousControlDeviceId =
			FAMILY_10H_MISCELLANEOUS_CONTROL_DEVICE_ID; break;
		case 0x11: miscellaneousControlDeviceId =
			FAMILY_11H_MISCELLANEOUS_CONTROL_DEVICE_ID; break;
		case 0x12: miscellaneousControlDeviceId =
			FAMILY_12H_MISCELLANEOUS_CONTROL_DEVICE_ID; break;
		case 0x14: miscellaneousControlDeviceId =
			FAMILY_14H_MISCELLANEOUS_CONTROL_DEVICE_ID; break;
		case 0x15:
			switch (Model & 0xF0) {
			case 0x00: miscellaneousControlDeviceId =
				FAMILY_15H_MODEL_00_MISC_CONTROL_DEVICE_ID; break;
			case 0x10: miscellaneousControlDeviceId =
				FAMILY_15H_MODEL_10_MISC_CONTROL_DEVICE_ID; break;
			case 0x30: miscellaneousControlDeviceId =
				FAMILY_15H_MODEL_30_MISC_CONTROL_DEVICE_ID; break;
			default: miscellaneousControlDeviceId = 0; break;
			} break;
		case 0x16:
			switch (Model & 0xF0) {
			case 0x00: miscellaneousControlDeviceId =
				FAMILY_16H_MODEL_00_MISC_CONTROL_DEVICE_ID; break;
			case 0x30: miscellaneousControlDeviceId =
				FAMILY_16H_MODEL_30_MISC_CONTROL_DEVICE_ID; break;
			default: miscellaneousControlDeviceId = 0; break;
			} break;
		default:
			break;
		}

		if (miscellaneousControlDeviceId != 0)
		{
			Hardware::Utils::Ring0::SafePCIHandle Pci;
			DWORD DeviceId;
			if (Pci.ReadWORD(0, 0x18, 3, 2, DeviceId))
			{
				if (DeviceId == miscellaneousControlDeviceId)
					return true;
			}
		}
		return false;
	}

	bool ReadSmnRegister(const uint32_t Smnaddress, DWORD& value)
	{
		Hardware::Utils::Ring0::SafePCIHandle Pci;
		if (Pci.WriteDWORD(0, 0, 0, 0x60, Smnaddress))
		{
			if (Pci.ReadDWORD(0, 0, 0, 0x64, value))
			{
				return true;
			}
		}
		value = 0;
		return false;
	}

	double AMD_Family17H_DealTemperature(const uint32_t, const uint32_t)
	{
		constexpr auto FAMILY_17H_M01H_THM_TCON_TEMP = 0x00059800;
		constexpr auto FAMILY_17H_M01H_THM_TCON_TEMP_RANGE_SEL = 0x80000;
		DWORD value = 0;
		double temperature = 0.0;
		if (ReadSmnRegister(FAMILY_17H_M01H_THM_TCON_TEMP, value)) {
			// 			temperature = Hardware::Utils::extract_bits_ui(value, 24, 31);
			// 			temperature += (Hardware::Utils::extract_bits_ui(value, 21, 23) * 0.125);
			temperature = ((value >> 21) & 0x7FF) / 8.0f;
			if ((value & FAMILY_17H_M01H_THM_TCON_TEMP_RANGE_SEL) != 0)
				temperature -= 49;
		}
		return temperature;
	}

	double AMD_Family15H_DealTemperature(const uint32_t Family, const uint32_t Model)
	{
		constexpr auto REPORTED_TEMPERATURE_CONTROL_REGISTER = 0xA4;
		Hardware::Utils::Ring0::SafePCIHandle Pci;
		DWORD value{};
		double coreTemperature = 0.0;
		if (Pci.ReadDWORD(0, 0x18, 3, REPORTED_TEMPERATURE_CONTROL_REGISTER, value)) {
			if (Family == 0x15 && (value & 0x30000) == 0x30000) {
				if ((Model & 0xF0) == 0x00) {
					coreTemperature = (((value >> 21) & 0x7FC) / 8.0f) - 49;
				}
				else {
					coreTemperature = (((value >> 21) & 0x7FF) / 8.0f) - 49;
				}
			}
			else if (Family == 0x16 &&
				((value & 0x30000) == 0x30000 || (value & 0x80000) == 0x80000)) {
				coreTemperature = (((value >> 21) & 0x7FF) / 8.0f) - 49;
			}
			else {
				coreTemperature = ((value >> 21) & 0x7FF) / 8.0f;
			}
		}
		return coreTemperature;
	}

	double AMD_Family17H_DealFrequency(const uint32_t value, const uint32_t)
	{
		double CpuFID = (value & 0xFF);
		double CpuDfsId = ((DWORD)value & 0x3F00) >> 8;
		switch ((ULONG)CpuDfsId)
		{
		case 0x08:
			break;
		case 0x09:
			CpuDfsId = CpuDfsId / 1.125;
			break;
		default:
			break;
		}
		if (CpuDfsId < 7)
			return 0.0;
		else
			return CpuFID / CpuDfsId * 2.0;
	}

	double AMD_Family17H_DealVoltage(const uint32_t value, const uint32_t)
	{
		double CpuVID = (value & 0x3FC000) >> 14;
		CpuVID = 1.55 - 0.00625 * CpuVID;
		return CpuVID;
	}

	double AMD_Family15H_DealFrequency(const uint32_t value, const uint32_t family)
	{
		constexpr auto CLOCK_POWER_TIMING_CONTROL_0_REGISTER = 0xD4;
		switch (family) {
		case 0x10:
		case 0x11:
		case 0x15:
		case 0x16: {
			// 8:6 CpuDid: current core divisor ID
			// 5:0 CpuFid: current core frequency ID
			auto cpuDid = (value >> 6) & 7;
			auto cpuFid = value & 0x1F;
			return 0.5 * (cpuFid + 0x10) / (1 << (int)cpuDid);
		}
		case 0x12: {
			// 8:4 CpuFid: current CPU core frequency ID
			// 3:0 CpuDid: current CPU core divisor ID
			auto cpuFid = (value >> 4) & 0x1F;
			auto cpuDid = value & 0xF;
			double divisor;
			switch (cpuDid) {
			case 0: divisor = 1; break;
			case 1: divisor = 1.5; break;
			case 2: divisor = 2; break;
			case 3: divisor = 3; break;
			case 4: divisor = 4; break;
			case 5: divisor = 6; break;
			case 6: divisor = 8; break;
			case 7: divisor = 12; break;
			case 8: divisor = 16; break;
			default: divisor = 1; break;
			}
			return (cpuFid + 0x10) / divisor;
		}
		case 0x14: {
			Hardware::Utils::Ring0::SafePCIHandle Pci;
			// 8:4: current CPU core divisor ID most significant digit
			// 3:0: current CPU core divisor ID least significant digit
			auto divisorIdMSD = (value >> 4) & 0x1F;
			auto divisorIdLSD = value & 0xF;
			DWORD PciValue = 0;
			if (Pci.ReadByte(0, 0x18, 3, CLOCK_POWER_TIMING_CONTROL_0_REGISTER, PciValue))
			{
				auto frequencyId = PciValue & 0x1F;
				return (frequencyId + 0x10) /
					(divisorIdMSD + (divisorIdLSD * 0.25) + 1);
			}
			else
				return 1.0;
		}
		default:
			return 1.0;
		}
	}

	double AMD_Family15H_DealVoltage(const uint32_t value, const uint32_t)
	{
		double CpuVID = Hardware::Utils::extract_bits_ui(value, 9, 15);
		CpuVID = 1.55 - 0.00625 * CpuVID;
		return CpuVID;
	}
}

Hardware::CPU::AmdCPU::AmdCPU() : GenericCPU()
{
	InitializeSocketFromWMI();
}

Hardware::Data::ErrorType Hardware::CPU::AmdCPU::Initialize(std::string& response)
{
	GetInfoFromCPUID();
	AddDynamicInfo();
	response = BuildInitializeJson();
	return Data::ErrorType::SUCCESS;
}

Hardware::Data::ErrorType Hardware::CPU::AmdCPU::Update(const std::string& Args, std::string& response)
{
	auto UpdateArrs = PaserUpdateJson(Args);
	bool UpdateSoc = false;
	Json::Value root;
	for (const auto& ele : UpdateArrs)
	{
		auto soc = m_SystemRoot.find(ele.SocketId);
		if (soc != m_SystemRoot.end())
		{
			if (!UpdateSoc)
			{
				soc->second.Update();
				auto tempJson = soc->second.BuildJson();
				if (!tempJson.isNull())
					root["Socket"].append(tempJson);
				UpdateSoc = true;
			}

			auto TempThreadPtr = soc->second.findThreadByOSID(ele.ThreadId);
			if (!TempThreadPtr.expired())
			{
				auto Ptr = TempThreadPtr.lock();
				Ptr->Update();
				auto tempJson = Ptr->BuildJson();
				if (!tempJson.isNull())
					root["Thread"].append(tempJson);
			}
		}
	}
	if (!root.isNull())
		response = Json::FastWriter().write(root);
	return Data::ErrorType::SUCCESS;
}

bool Hardware::CPU::AmdCPU::InitializeSocketFromAPIC()
{
	// TODO 找个AMD平台测试APIC
	uint32_t num_Thread = 0;
	uint32_t num_core = 0;
	std::unique_ptr<char[]>slpi(new char[sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)]);
	DWORD len = (DWORD)sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
	BOOL res = GetLogicalProcessorInformationEx(RelationAll, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(slpi.get()), &len);

	while (res == FALSE)
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			slpi.reset(new char[len]);
			res = GetLogicalProcessorInformationEx(RelationAll, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(slpi.get()), &len);
		}
		else
		{
			spdlog::debug("Error in Windows function 'GetLogicalProcessorInformationEx': {}", GetLastError());
			return false;
		}
	}

	char* base_slpi = slpi.get();
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX pi = NULL;

	for (; base_slpi < slpi.get() + len; base_slpi += (DWORD)pi->Size)
	{
		pi = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(base_slpi);
		if (pi->Relationship == RelationProcessorCore)
		{
			num_Thread += (pi->Processor.Flags == LTP_PC_SMT) ? 2 : 1;
			++num_core;
		}
	}

	if (num_Thread != GetActiveProcessorCount(ALL_PROCESSOR_GROUPS))
	{
		spdlog::debug("Error in processor group size counting: {} != {}", num_Thread, GetActiveProcessorCount(ALL_PROCESSOR_GROUPS));
		spdlog::debug("Make sure your binary is compiled for 64-bit: using 'x64' platform configuration.");
		return false;
	}
	int OsId = 0;
	Socket soc(0, 0);
	for (decltype(num_core) TileId = 0; TileId < num_core; ++TileId)
	{
		TopologyEntry temp{};
		Core core(TileId, TileId, 0);
		temp.socket = 0;
		temp.core_id = TileId;
		temp.tile_id = TileId;
		for (decltype(num_core) ThreadId = 0; ThreadId < num_Thread / num_core; ++ThreadId)
		{
			temp.os_id = OsId;
			temp.thread_id = ThreadId;
			core.AddThread(ThreadId, OsId++);
			topology.emplace_back(temp);
		}
		soc.AddCore(std::move(core));
	}
	this->AddSocket(std::move(soc));
	return true;
}

bool Hardware::CPU::AmdCPU::InitializeSocketFromWMI()
{
	CPU_WMI wmi;
	if (FAILED(wmi.ExcuteFun()))
	{
		return false;
	}

	int OsId = 0;
	auto ThreadPerCore = wmi.Thread / wmi.Core;
	Socket soc(0, 0);
	for (decltype(wmi.Core) TileId = 0; TileId < wmi.Core; ++TileId)
	{
		TopologyEntry temp{};
		Core core(TileId, TileId, 0);
		temp.socket = 0;
		temp.core_id = TileId;
		temp.tile_id = TileId;
		for (decltype(ThreadPerCore) ThreadId = 0; ThreadId < ThreadPerCore; ++ThreadId)
		{
			temp.os_id = OsId;
			temp.thread_id = ThreadId;
			core.AddThread(ThreadId, OsId++);
			topology.emplace_back(temp);
		}
		soc.AddCore(std::move(core));
	}
	this->AddSocket(std::move(soc));
	return true;
}

void Hardware::CPU::AmdCPU::GetInfoFromCPUID()
{
	for (auto& soc : m_SystemRoot)
	{
		GetInfoFromCPUID1_80000001(soc.second);
		GetInfoFromCPUID80000005_6(soc.second);
		GetInfoFromCPUID80000002(soc.second);
		GetInfoFromCPUDB(soc.second);
	}
}

void Hardware::CPU::AmdCPU::GetInfoFromCPUID1_80000001(Socket& soc)
{
	Utils::CPUID_INFO cpuid_args;
	Utils::GetCpuid(1, cpuid_args);
	soc.m_Data.Family = (cpuid_args.array[0] & CPUID_Family) >> 8;
	soc.m_Data.Model = (cpuid_args.array[0] & CPUID_Model) >> 4;
	soc.m_Data.Stepping = (cpuid_args.array[0] & CPUID_Stepping) >> 0;
	soc.m_Data.ExtFamily = ((cpuid_args.array[0] & CPUID_Extended_Family) >> 20) + soc.m_Data.Family;
	soc.m_Data.ExtModel = (((cpuid_args.array[0] & CPUID_Extended_Model) >> 16) << 4) + soc.m_Data.Model;
	std::bitset<32> ecx = cpuid_args.reg.ecx;
	std::bitset<32> edx = cpuid_args.reg.ebx;

	if (edx[23])
		soc.m_Data.Instructions.append("MMX ");
	if (edx[25])
		soc.m_Data.Instructions.append("SSE ");
	if (edx[26])
		soc.m_Data.Instructions.append("SSE2 ");
	if (ecx[0])
		soc.m_Data.Instructions.append("SSE3 ");
	if (ecx[9])
		soc.m_Data.Instructions.append("SSSE3 ");
	if (ecx[19])
		soc.m_Data.Instructions.append("SSE4.1 ");
	if (ecx[20])
		soc.m_Data.Instructions.append("SSE4.2 ");
	if (ecx[28])
		soc.m_Data.Instructions.append("AVX ");
	if (ecx[25])
		soc.m_Data.Instructions.append("AES ");
	if (ecx[12])
		soc.m_Data.Instructions.append("FMA ");
	if (ecx[1])
		soc.m_Data.Instructions.append("PCLMULQDQ ");

	Utils::GetCpuid(0x80000001, cpuid_args);
	ecx = cpuid_args.reg.ecx;
	edx = cpuid_args.reg.edx;

	if (ecx[6])
		soc.m_Data.Instructions.append("SSE4.A ");
	if (ecx[16])
		soc.m_Data.Instructions.append("FMA4 ");
	if (edx[31])
		soc.m_Data.Instructions.append("3DNow! ");
	if (edx[30])
		soc.m_Data.Instructions.append("3DNow!Ext ");
}

void Hardware::CPU::AmdCPU::GetInfoFromCPUID80000005_6(Socket& soc)
{
	int l1D, l1T, l2, l3;
	UpdateCacheNumber(l1D, l1T, l2, l3);
	Utils::CPUID_INFO cpuid_args;
	Utils::GetCpuid(0x80000005, cpuid_args);
	Cache_info temp;
	temp.Cache_level = 1;
	temp.Cache_type = 'D';
	temp.Cache_line_size = ((cpuid_args.array[2] & L1_DATA_CACHE_LINE_SIZE) >> 0);
	temp.Cache_Ways = ((cpuid_args.array[2] & L1_DATA_CACHE_ASSOC) >> 16);
	temp.Cache_Size = ((cpuid_args.array[2] & L1_DATA_CACHE_SIZE) >> 24);
	temp.NumberOfCache = l1D;
	if (temp.Cache_Size)
		soc.m_Data.Cache.emplace_back(temp);

	temp.Cache_level = 1;
	temp.Cache_type = 'T';
	temp.Cache_line_size = ((cpuid_args.array[3] & L1_DATA_CACHE_LINE_SIZE) >> 0);
	temp.Cache_Ways = ((cpuid_args.array[3] & L1_DATA_CACHE_ASSOC) >> 16);
	temp.Cache_Size = ((cpuid_args.array[3] & L1_DATA_CACHE_SIZE) >> 24);
	temp.NumberOfCache = l1T;
	if (temp.Cache_Size)
		soc.m_Data.Cache.emplace_back(temp);

	Utils::GetCpuid(0x80000006, cpuid_args);
	temp.Cache_level = 2;
	temp.Cache_type = 'D';
	temp.Cache_line_size = ((cpuid_args.array[2] & L2_DATA_CACHE_LINE_SIZE) >> 0);
	temp.Cache_Ways = AMD_L2_L3_Way_Associativity((cpuid_args.array[2] & L2_DATA_CACHE_ASSOC) >> 12);
	temp.Cache_Size = ((cpuid_args.array[2] & L2_DATA_CACHE_SIZE) >> 16);
	temp.NumberOfCache = l2;
	if (temp.Cache_Size)
		soc.m_Data.Cache.emplace_back(temp);

	temp.Cache_level = 3;
	temp.Cache_type = 'D';
	temp.Cache_line_size = ((cpuid_args.array[3] & L3_INSTRUCTION_CACHE_LINE_SIZE) >> 0);
	temp.Cache_Ways = AMD_L2_L3_Way_Associativity((cpuid_args.array[3] & L3_INSTRUCTION_CACHE_ASSOC) >> 12);
	temp.Cache_Size = ((cpuid_args.array[3] & L3_INSTRUCTION_CACHE_SIZE) >> 18) * 512;
	temp.NumberOfCache = l3;
	if (temp.Cache_Size)
		soc.m_Data.Cache.emplace_back(temp);
}

void Hardware::CPU::AmdCPU::GetInfoFromCPUID80000002(Socket& soc)
{
	char brand[0x40]{};
	Utils::CPUID_INFO cpuid_args;
	Utils::GetCpuid(0x80000002, cpuid_args);
	memcpy(brand, cpuid_args.array, _countof(cpuid_args.array) * sizeof(int));
	Utils::GetCpuid(0x80000003, cpuid_args);
	memcpy(brand + 16, cpuid_args.array, _countof(cpuid_args.array) * sizeof(int));
	Utils::GetCpuid(0x80000004, cpuid_args);
	memcpy(brand + 32, cpuid_args.array, _countof(cpuid_args.array) * sizeof(int));
	soc.m_Data.Name = brand;
	Utils::trim(soc.m_Data.Name);
}

void Hardware::CPU::AmdCPU::GetInfoFromCPUDB(Socket& soc)
{
	//Todo AMD ParserCPU Name方式不太确定 暂留空
}

void Hardware::CPU::AmdCPU::AddDynamicInfo()
{
	AddDynamicInfoForSocket();
	AddDynamicInfoForHyperThead();
}

void Hardware::CPU::AmdCPU::AddDynamicInfoForSocket()
{
	for (auto& Soc : m_SystemRoot)
	{
		AddPackageTemperature(Soc.second);
	}
}

void Hardware::CPU::AmdCPU::AddDynamicInfoForHyperThead()
{
	for (const auto& topologyEle : topology)
	{
		for (auto& Soc : m_SystemRoot)
		{
			if (Soc.second.ApicId() == topologyEle.socket)
			{
				auto TargetThread = Soc.second.findThreadByOSID(topologyEle.os_id);
				{
					AddCoreVoltage(TargetThread, Soc.second.m_Data.Family, Soc.second.m_Data.Model);
					// TODO PStatus Frequency Affirm
					AddCoreFrequency(TargetThread, Soc.second.m_Data.Family, Soc.second.m_Data.Model);
				}
			}
		}
	}
}

void Hardware::CPU::AmdCPU::AddPackageTemperature(Socket& soc)
{
	switch (soc.m_Data.Family)
	{
	case 0x17:
	case 0x19:
		soc.AddDecorator(std::make_unique<AMDTemperature>(0, "PackageTemperature", AMD_Family17H_DealTemperature, soc.m_Data.Family, soc.m_Data.Model));
		break;
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
		if (CheckAMDPStatusDeviceIsExist(soc.m_Data.Family, soc.m_Data.Model))
			soc.AddDecorator(std::make_unique<AMDTemperature>(0, "PackageTemperature", AMD_Family15H_DealTemperature, soc.m_Data.Family, soc.m_Data.Model));
		break;
	default:
		break;
	}
}

void Hardware::CPU::AmdCPU::AddCoreVoltage(std::weak_ptr<HyperThread> thread, const uint32_t Family, const uint32_t Model)
{
	if (!thread.expired())
	{
		auto Ptr = thread.lock();
		auto TempMsrPtr = Ptr->GetMsr();
		if (!TempMsrPtr.expired())
		{
			auto Msr = TempMsrPtr.lock();
			Utils::CvtDs value{};
			switch (Family)
			{
			case 0x17:
			case 0x19:

				if (Msr->read(MSR_FAMILY_17H_P_STATE, value.ui64) && value.ui32.Eax)
				{
					Ptr->AddDecorator(std::make_unique<AMDVoltage>(MSR_FAMILY_17H_P_STATE, "CoreVoltage", Family, AMD_Family17H_DealVoltage));
				}
				break;
			case 0x10:
			case 0x11:
			case 0x12:
			case 0x13:
			case 0x15:
			case 0x16:
				if (Msr->read(MSR_FAMILY_10H_P_STATE, value.ui64) && value.ui32.Eax)
				{
					Ptr->AddDecorator(std::make_unique<AMDVoltage>(MSR_FAMILY_10H_P_STATE, "CoreVoltage", Family, AMD_Family15H_DealVoltage));
				}
				break;

			default:
				break;
			}
		}
	}
}

void Hardware::CPU::AmdCPU::AddCoreFrequency(std::weak_ptr<HyperThread> thread, const uint32_t Family, const uint32_t Model)
{
	CPU_WMI wmi;
	wmi.ExcuteFun();

	if (!thread.expired())
	{
		auto Ptr = thread.lock();
		auto TempMsrPtr = Ptr->GetMsr();
		if (!TempMsrPtr.expired())
		{
			auto Msr = TempMsrPtr.lock();
			Utils::CvtDs value;
			switch (Family)
			{
			case 0x17:
			case 0x19:
				if (Msr->read(MSR_FAMILY_10H_P_STATE, value.ui64) && value.ui32.Eax)
				{
					Ptr->AddDecorator(std::make_unique<AMDFrequency>(MSR_FAMILY_17H_P_STATE, "CoreFrequency", Family, AMD_Family17H_DealFrequency, wmi.MaxClockSpeed));
				}
				break;
			case 0x10:
			case 0x11:
			case 0x12:
			case 0x13:
			case 0x15:
			case 0x16:
				if (Msr->read(MSR_FAMILY_10H_P_STATE, value.ui64) && value.ui32.Eax)
				{
					Ptr->AddDecorator(std::make_unique<AMDFrequency>(MSR_FAMILY_10H_P_STATE, "CoreFrequency", Family, AMD_Family15H_DealFrequency, wmi.MaxClockSpeed));
				}
				break;
			case 0x14:
				if (Msr->read(MSR_FAMILY_10H_P_STATE, value.ui64) && value.ui32.Eax)
				{
					if (CheckAMDPStatusDeviceIsExist(Family, Model))
						Ptr->AddDecorator(std::make_unique<AMDFrequency>(MSR_FAMILY_10H_P_STATE, "CoreFrequency", Family, AMD_Family15H_DealFrequency, wmi.MaxClockSpeed));
				}
				break;
			default:
				break;
			}
		}
	}
}

Hardware::CPU::AMDTemperature::AMDTemperature(const uint64_t MsrRegistry, const std::string& Name, std::function<double(const uint32_t, const uint32_t)> DealTemperatureFn, const uint32_t Family, const uint32_t Model)
	: CPUDecorator(MsrRegistry, Name), m_DealTemperatureFn(DealTemperatureFn), m_Family(Family), m_Model(Model)
{
	m_CurrentTemperature = std::numeric_limits<double>::infinity();
}

const std::string Hardware::CPU::AMDTemperature::GetDecoratorValue() const
{
	if (m_CurrentTemperature != std::numeric_limits<double>::infinity())
	{
		if (IsUpdate)
		{
			IsUpdate = false;
			return std::to_string(m_CurrentTemperature) + "degree";
		}
	}
	return {};
}

void Hardware::CPU::AMDTemperature::Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr)
{
	auto temp = m_DealTemperatureFn(m_Family, m_Model);
	if (temp - 0.0 < 1e-6)
	{
		return;
	}
	m_CurrentTemperature = temp;
	IsUpdate = true;
}

Hardware::CPU::AMDFrequency::AMDFrequency(const uint64_t MsrRegistry, const std::string& Name, const uint32_t Family,
	std::function<double(const uint32_t, const uint32_t)> DealValueFn, const uint32_t MaxClockSpeed) :
	CPUDecorator(MsrRegistry, Name), m_DealValueFn(DealValueFn), m_Family(Family), m_TscFrequencyMhz(MaxClockSpeed)
{
	m_CurrentFrequency = std::numeric_limits<double>::infinity();
}

const std::string Hardware::CPU::AMDFrequency::GetDecoratorValue() const
{
	if (m_CurrentFrequency != std::numeric_limits<double>::infinity())
	{
		if (IsUpdate)
		{
			IsUpdate = false;
			return Utils::to_string_with_precision(m_CurrentFrequency) + "Mhz";
		}
	}
	return {};
}

void Hardware::CPU::AMDFrequency::Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr)
{
	if (!Msr.expired())
	{
		auto MsrPtr = Msr.lock();
		Utils::CvtDs value{};
		auto res = MsrPtr->read(m_MsrRegistry, value.ui64);
		if (res)
		{
			auto temp = m_DealValueFn(value.ui32.Eax, m_Family);
			if (temp - 0.0 < 1e-6)
				return;
			m_CurrentFrequency = temp * GetBusSpeedByTsc(MsrPtr->getCoreId());
			IsUpdate = true;
		}
	}
}

double Hardware::CPU::AMDFrequency::GetBusSpeedByTsc(const int32_t& coreid)
{
	int i = 10;
	double res = 0.0;
	Utils::Ring0::SafeTscIntervalHandle TscReader(coreid);
	double value;
	while (i--)
	{
		TscReader.read(value);
		res += (value / 1000000.0);
	}
	return res / m_TscFrequencyMhz * 10.0;
}

Hardware::CPU::AMDVoltage::AMDVoltage(const uint64_t MsrRegistry, const std::string& Name, const uint32_t Family, std::function<double(const uint32_t, const uint32_t)> DealValueFn) :
	CPUDecorator(MsrRegistry, Name), m_DealValueFn(DealValueFn), m_Family(Family)
{
	m_CurrentVoltage = std::numeric_limits<double>::infinity();
}

const std::string Hardware::CPU::AMDVoltage::GetDecoratorValue() const
{
	if (m_CurrentVoltage != std::numeric_limits<double>::infinity())
	{
		if (IsUpdate)
		{
			IsUpdate = false;
			return Utils::to_string_with_precision(m_CurrentVoltage) + "V";
		}
	}
	return {};
}

void Hardware::CPU::AMDVoltage::Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr)
{
	if (!Msr.expired())
	{
		auto MsrPtr = Msr.lock();
		Utils::CvtDs value{};
		auto res = MsrPtr->read(m_MsrRegistry, value.ui64);
		if (res && value.ui64)
		{
			auto temp = m_DealValueFn(value.ui32.Eax, m_Family);
			if (temp - 0.0 < 1e-6)
				return;
			m_CurrentVoltage = temp;
			IsUpdate = true;
		}
	}
}
