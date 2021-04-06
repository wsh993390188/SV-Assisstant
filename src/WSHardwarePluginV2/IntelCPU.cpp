#include "stdafx.h"
#include <array>
#include <bitset>
#include <assert.h>
#include "IntelCPU.h"
#include "CPUWMI.h"
#include "IntelFamily.hpp"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

using namespace Hardware::CPU::Intel;
#define CACHE_TYPE							0x0000001F
#define CACHE_LEVEL							0x000000E0
#define CACHE_MAX_ADDR_LOGICAL				0x03FFC000
#define CACHE_MAX_ADDR_PHYSICAL				0xFC000000
//EBX
#define CACHE_WAYS							0xFFC00000
#define CACHE_PARTITIONS					0x003FF000
#define CACHE_LINE_SIZE						0x00000FFF
//EDX
#define CACHE_INVALIDATE					0x00000000
#define CACHE_INCLUSIVENESS					0x00000001
#define CACHE_COMPLEX_INDEXING				0x00000002

Hardware::CPU::IntelCPU::IntelCPU() noexcept : GenericCPU()
{
	auto res = InitializeSocket();
	if (!res)
	{
		m_SystemRoot.clear();
		InitializeSocketFromWMI();
	}
}

Hardware::CPU::IntelCPU::~IntelCPU() noexcept
{
}

Hardware::Data::ErrorType Hardware::CPU::IntelCPU::Initialize(std::string& response)
{
	spdlog::info("Start initialize Intel cpu");
	this->GetInfoFromCPUID();
	this->AddDynamicInfoForSocket();
	this->AddDynamicInfoForHyperThead();
	response = BuildInitializeJson();
	spdlog::info("End initialize Intel cpu");
	return Data::ErrorType::SUCCESS;
}

Hardware::Data::ErrorType Hardware::CPU::IntelCPU::Update(const std::string& Args, std::string& response)
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

			auto Thread = soc->second.findThreadByOSID(ele.ThreadId);
			if (!Thread.expired())
			{
				auto Ptr = Thread.lock();
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

bool Hardware::CPU::IntelCPU::InitializeSocket()
{
	spdlog::info("Initialize Intel CPU Information");
	using socketIdMap_type = std::map<uint32_t, uint32_t>;
	socketIdMap_type socketIdMap;

	Utils::CPUID_INFO cpuid_args;
	Utils::GetCpuid(0, cpuid_args);
	auto nIds = cpuid_args.reg.eax;

	// init constants for CPU topology leaf 0xB
	// adapted from Topology Enumeration Reference code for Intel 64 Architecture
	// https://software.intel.com/en-us/articles/intel-64-architecture-processor-topology-enumeration
	int wasCoreReported = 0, wasThreadReported = 0;
	int subleaf = 0, levelType, levelShift;
	uint32_t smtMaskWidth = 0;
	uint32_t corePlusSMTMaskWidth = 0;
	uint32_t coreMaskWidth = 0;
	uint32_t num_cores = 0;

	// Intel没加这个判断CPUID的保护，是不是可以去掉了
	if (nIds >= 0xB)
	{
		Utils::ThreadGroupTempAffinity aff0(0);
		do
		{
			Utils::GetCpuid(0xb, subleaf, cpuid_args);
			if (cpuid_args.array[1] == 0)
			{ // if EBX ==0 then this subleaf is not valid, we can exit the loop
				break;
			}
			levelType = Utils::extract_bits_ui(cpuid_args.array[2], 8, 15);
			levelShift = Utils::extract_bits_ui(cpuid_args.array[0], 0, 4);
			switch (levelType)
			{
			case 1: //level type is SMT, so levelShift is the SMT_Mask_Width
				smtMaskWidth = levelShift;
				wasThreadReported = 1;
				break;
			case 2: //level type is Core, so levelShift is the CorePlusSMT_Mask_Width
				corePlusSMTMaskWidth = levelShift;
				wasCoreReported = 1;
				break;
			default:
				break;
			}
			subleaf++;
		} while (1);
	}
	else
	{
		spdlog::error("Intel CPUID not support 0xb leaf function, max leafs support: {}", nIds);
	}

	if (wasThreadReported && wasCoreReported)
	{
		coreMaskWidth = corePlusSMTMaskWidth - smtMaskWidth;
	}
	else if (!wasCoreReported && wasThreadReported)
	{
		coreMaskWidth = smtMaskWidth;
	}
	else
	{
		spdlog::error("ERROR: Major problem? No leaf 0 under cpuid function 11.\n");
		return false;
	}

	uint32_t l2CacheMaskShift = 0;
	uint32_t l2CacheMaskWidth;

	Utils::GetCpuid(0x4, 2, cpuid_args); // get ID for L2 cache
	l2CacheMaskWidth = 1 + Utils::extract_bits_ui(cpuid_args.array[0], 14, 25); // number of APIC IDs sharing L2 cache
	for (; l2CacheMaskWidth > 1; l2CacheMaskWidth >>= 1)
	{
		l2CacheMaskShift++;
	}

	auto populateEntry = [&smtMaskWidth, &coreMaskWidth, &l2CacheMaskShift](TopologyEntry& entry, const int apic_id)
	{
		entry.thread_id = Utils::extract_bits_ui(apic_id, 0, smtMaskWidth - 1);
		entry.core_id = Utils::extract_bits_ui(apic_id, smtMaskWidth, smtMaskWidth + coreMaskWidth - 1);
		entry.socket = Utils::extract_bits_ui(apic_id, smtMaskWidth + coreMaskWidth, 31);
		entry.tile_id = Utils::extract_bits_ui(apic_id, l2CacheMaskShift, 31);
	};

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
			num_cores += (pi->Processor.Flags == LTP_PC_SMT) ? 2 : 1;
		}
	}

	if (num_cores != GetActiveProcessorCount(ALL_PROCESSOR_GROUPS))
	{
		spdlog::debug("Error in processor group size counting: {} != {}", num_cores, GetActiveProcessorCount(ALL_PROCESSOR_GROUPS));
		spdlog::debug("Make sure your binary is compiled for 64-bit: using 'x64' platform configuration.");
		return false;
	}

	for (int i = 0; i < (int)num_cores; i++)
	{
		Utils::ThreadGroupTempAffinity affinity(i);
		int apic_id = 0;
		if (nIds >= 0xB)
		{
			Utils::GetCpuid(0xb, 0x0, cpuid_args);
			apic_id = cpuid_args.array[3];
		}
		TopologyEntry entry;
		entry.os_id = i;

		populateEntry(entry, apic_id);
		topology.emplace_back(entry);
		socketIdMap[entry.socket] = 0;
	}

	uint32_t sid = 0;
	for (auto s = socketIdMap.begin(); s != socketIdMap.end(); ++s)
	{
		s->second = sid++;
		// first is apic id, second is logical socket id
		this->AddSocket(s->first, s->second);
	}

	for (decltype(num_cores) cid = 0; cid < num_cores; ++cid)
	{
		for (auto& Soc : m_SystemRoot)
		{
			if (Soc.second.ApicId() == topology[cid].socket)
			{
				auto core = Soc.second.findCoreByTileID(topology[cid].tile_id);
				if (core.expired())
				{
					Core temp(topology[cid].core_id, topology[cid].tile_id, topology[cid].socket);
					temp.AddThread(topology[cid].thread_id, topology[cid].os_id);
					Soc.second.AddCore(std::move(temp));
				}
				else
				{
					core.lock()->AddThread(topology[cid].thread_id, topology[cid].os_id);
				}
				break;
			}
		}
	}
	return true;
}

bool Hardware::CPU::IntelCPU::InitializeSocketFromWMI()
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

std::unique_ptr<Hardware::XMLConfig::QueryInfo> Hardware::CPU::IntelCPU::ParserCPUBrandName(Socket& soc)
{
	// 不确定如何操作
	try
	{
		Hardware::XMLConfig::QueryInfo QueryInfo;
		QueryInfo.Manufacture = "Intel";
		if (soc.m_Data.Name.find("Core") != std::string::npos)
		{
			if (soc.m_Data.Name.find("i9") != std::string::npos)
			{
				QueryInfo.Family = "Core i9";
			}
			else if (soc.m_Data.Name.find("i7") != std::string::npos)
			{
				QueryInfo.Family = "Core i7";
			}
			else if (soc.m_Data.Name.find("i5") != std::string::npos)
			{
				QueryInfo.Family = "Core i5";
			}
			else if (soc.m_Data.Name.find("i3") != std::string::npos)
			{
				QueryInfo.Family = "Core i3";
			}
			else
			{
				spdlog::error("{} Unknown Model, need to update code", soc.m_Data.Name.c_str());
				return nullptr;
			}

			size_t last = 0;
			size_t Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			auto TempModel = soc.m_Data.Name.substr(last, Index - last);
			Index = TempModel.find_first_of("-");
			QueryInfo.Model = TempModel.substr(Index + 1);
		}
		else if (soc.m_Data.Name.find("Pentium Silver") != std::string::npos)
		{
			QueryInfo.Family = "Pentium Silver";
			size_t last = 0;
			size_t Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			QueryInfo.Model = soc.m_Data.Name.substr(last, Index - last);
		}
		else if (soc.m_Data.Name.find("Pentium Gold") != std::string::npos)
		{
			QueryInfo.Family = "Pentium Gold";
			size_t last = 0;
			size_t Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			QueryInfo.Model = soc.m_Data.Name.substr(last, Index - last);
		}
		else if (soc.m_Data.Name.find("Pentium") != std::string::npos)
		{
			QueryInfo.Family = "Pentium";
			size_t last = 0;
			size_t Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			QueryInfo.Model = soc.m_Data.Name.substr(last, Index - last);
		}
		else if (soc.m_Data.Name.find("Celeron") != std::string::npos)
		{
			QueryInfo.Family = "Celeron";
			size_t last = 0;
			size_t Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			QueryInfo.Model = soc.m_Data.Name.substr(last, Index - last);
		}
		else if (soc.m_Data.Name.find("Atom") != std::string::npos)
		{
			QueryInfo.Family = "Atom";
			size_t last = 0;
			size_t Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			last = Index + 1;
			Index = soc.m_Data.Name.find_first_of(" ", last);
			QueryInfo.Model = soc.m_Data.Name.substr(last, Index - last);
		}
		else if (soc.m_Data.Name.find("Xeon") != std::string::npos)
		{
			if (soc.m_Data.Name.find("E-") != std::string::npos
				|| soc.m_Data.Name.find("E3-") != std::string::npos
				|| soc.m_Data.Name.find("E5-") != std::string::npos
				|| soc.m_Data.Name.find("E7-") != std::string::npos
				|| soc.m_Data.Name.find("E9-") != std::string::npos)
			{
				if (soc.m_Data.Name.find("E-") != std::string::npos)
					QueryInfo.Family = "Xeon E";
				else if (soc.m_Data.Name.find("E3-") != std::string::npos)
					QueryInfo.Family = "Xeon E3";
				else if (soc.m_Data.Name.find("E5-") != std::string::npos)
					QueryInfo.Family = "Xeon E5";
				else if (soc.m_Data.Name.find("E7-") != std::string::npos)
					QueryInfo.Family = "Xeon E7";
				else if (soc.m_Data.Name.find("E9-") != std::string::npos)
					QueryInfo.Family = "Xeon E9";

				size_t last = 0, temp = 0;
				size_t Index = soc.m_Data.Name.find_first_of(" ", last);
				last = Index + 1;
				Index = soc.m_Data.Name.find_first_of(" ", last);
				last = Index + 1;
				Index = soc.m_Data.Name.find_first_of(" ", last);
				last = Index + 1;
				Index = soc.m_Data.Name.find_first_of(" ", last);
				temp = Index;
				auto TempModel = soc.m_Data.Name.substr(last, Index - last);
				Index = TempModel.find_first_of("-");
				QueryInfo.Model = TempModel.substr(Index + 1);
			}
			else
			{
				if (soc.m_Data.Name.find("Silver") != std::string::npos)
					QueryInfo.Family = "Xeon Silver";
				else if (soc.m_Data.Name.find("Gold") != std::string::npos)
					QueryInfo.Family = "Xeon Gold";
				else if (soc.m_Data.Name.find("Bronze") != std::string::npos)
					QueryInfo.Family = "Xeon Bronze";
				else if (soc.m_Data.Name.find("Platinum") != std::string::npos)
					QueryInfo.Family = "Xeon Platinum";
				else if (soc.m_Data.Name.find("MP") != std::string::npos)
					QueryInfo.Family = "Xeon MP";
				else if (soc.m_Data.Name.find("Phi") != std::string::npos)
					QueryInfo.Family = "Xeon Phi";
				else
				{
					spdlog::error("{} Unknown Model, need to update code", soc.m_Data.Name.c_str());
					return nullptr;
				}
				size_t last = 0;
				size_t Index = soc.m_Data.Name.find_first_of(" ", last);
				last = Index + 1;
				Index = soc.m_Data.Name.find_first_of(" ", last);
				last = Index + 1;
				Index = soc.m_Data.Name.find_first_of(" ", last);
				last = Index + 1;
				Index = soc.m_Data.Name.find_first_of(" ", last);
				QueryInfo.Model = soc.m_Data.Name.substr(last, Index - last);
			}
		}
		else
		{
			return nullptr;
		}
		return std::make_unique<Hardware::XMLConfig::QueryInfo>(QueryInfo);
	}
	catch (const std::exception& e)
	{
		spdlog::error("parser cpu brand error {}", e.what());
	}
	return nullptr;
}

#pragma region CPUID相关

void Hardware::CPU::IntelCPU::GetInfoFromCPUID()
{
	// TODO Dual Socket Switch By NUMA
	for (auto& soc : m_SystemRoot)
	{
		GetInfoFromCPUID1_7(soc.second);
		GetInfoFromCPUID4(soc.second);
		GetInfoFromCPUID80000002(soc.second);
		GetInfoFromCPUDB(soc.second);
	}
}

void Hardware::CPU::IntelCPU::GetInfoFromCPUID1_7(Socket& soc)
{
	Utils::CPUID_INFO cpuid_args;
	Utils::GetCpuid(1, cpuid_args);
	soc.m_Data.Family = (cpuid_args.array[0] & CPUID_Family) >> 8;
	soc.m_Data.Model = (cpuid_args.array[0] & CPUID_Model) >> 4;
	soc.m_Data.Stepping = (cpuid_args.array[0] & CPUID_Stepping) >> 0;
	soc.m_Data.ExtFamily = ((cpuid_args.array[0] & CPUID_Extended_Family) >> 20) + soc.m_Data.Family;
	soc.m_Data.ExtModel = (((cpuid_args.array[0] & CPUID_Extended_Model) >> 16) << 4) + soc.m_Data.Model;
	std::bitset<32> f1_ecx = cpuid_args.reg.ecx;
	std::bitset<32> f1_edx = cpuid_args.reg.ebx;

	if (f1_edx[23])
		soc.m_Data.Instructions.append("MMX ");
	if (f1_edx[25])
		soc.m_Data.Instructions.append("SSE ");
	if (f1_edx[26])
		soc.m_Data.Instructions.append("SSE2 ");
	if (f1_ecx[0])
		soc.m_Data.Instructions.append("SSE3 ");
	if (f1_ecx[9])
		soc.m_Data.Instructions.append("SSSE3 ");
	if (f1_ecx[19])
		soc.m_Data.Instructions.append("SSE4.1 ");
	if (f1_ecx[20])
		soc.m_Data.Instructions.append("SSE4.2 ");
	if (f1_ecx[28])
		soc.m_Data.Instructions.append("AVX ");
	if (f1_ecx[25])
		soc.m_Data.Instructions.append("AES ");
	if (f1_ecx[12])
		soc.m_Data.Instructions.append("FMA ");
	if (f1_ecx[5])
		soc.m_Data.Instructions.append("VT-x ");
	if (f1_ecx[1])
		soc.m_Data.Instructions.append("PCLMULQDQ ");

	if (soc.m_Data.Family != 0x0F)		soc.m_Data.DisplayFamily = soc.m_Data.Family;
	else 	soc.m_Data.DisplayFamily = soc.m_Data.ExtFamily;
	if (soc.m_Data.Family == 0x06 || soc.m_Data.Family == 0x0F)	soc.m_Data.DisplayModel = soc.m_Data.ExtModel;
	else 	soc.m_Data.DisplayModel = soc.m_Data.Model;

	DisplayFamily = soc.m_Data.DisplayFamily;
	DisplayModel = soc.m_Data.DisplayModel;
	spdlog::info("Family:{:x} Model:{:x} Stepping: {}", soc.m_Data.DisplayFamily, soc.m_Data.DisplayModel, soc.m_Data.Stepping);

	Utils::GetCpuid(7, cpuid_args);
	std::bitset<32> f7_ecx = cpuid_args.reg.ecx;
	std::bitset<32> f7_ebx = cpuid_args.reg.edx;
	if (f7_ebx[5])
		soc.m_Data.Instructions.append("AVX2 ");
}

void Hardware::CPU::IntelCPU::GetInfoFromCPUID4(Socket& soc)
{
	int l1D, l1T, l2, l3;
	UpdateCacheNumber(l1D, l1T, l2, l3);
	Utils::CPUID_INFO cpuid_args;
	bool finish = false;
	for (int i = 0; ; i++)
	{
		Utils::GetCpuid(4, i, cpuid_args);
		Cache_info temp{};
		switch (cpuid_args.array[0] & 0x1F)
		{
		case 1:
			temp.Cache_type = 'D';
			break;
		case 2:
			temp.Cache_type = 'T';
			break;
		case 3:
			temp.Cache_type = 'U';
			break;
		default:
			finish = true;
			break;
		}
		if (finish)
			break;
		temp.Cache_level = (cpuid_args.array[0] & CACHE_LEVEL) >> 5;
		temp.Cache_Ways = ((cpuid_args.array[1] & CACHE_WAYS) >> 22) + 1;
		temp.Cache_partitions = ((cpuid_args.array[1] & CACHE_PARTITIONS) >> 12) + 1;
		temp.Cache_line_size = (cpuid_args.array[1] & CACHE_LINE_SIZE) + 1;
		temp.Cache_Set = cpuid_args.array[2] + 1;
		temp.Cache_Size = temp.Cache_Ways * temp.Cache_partitions * temp.Cache_line_size * temp.Cache_Set;
		if (temp.Cache_level == 1 && temp.Cache_type == 'D')
			temp.NumberOfCache = l1D;
		else if (temp.Cache_level == 1 && temp.Cache_type == 'T')
		{
			temp.NumberOfCache = l1T;
		}
		else if (temp.Cache_level == 2)
		{
			temp.NumberOfCache = l2;
		}
		else if (temp.Cache_level == 3)
		{
			temp.NumberOfCache = l3;
		}
		soc.m_Data.Cache.emplace_back(std::move(temp));
	}
}

void Hardware::CPU::IntelCPU::GetInfoFromCPUID80000002(Socket& soc)
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

void Hardware::CPU::IntelCPU::GetInfoFromCPUDB(Socket& soc)
{
	auto ParserInformation = ParserCPUBrandName(soc);
	if (!ParserInformation)
		return;
	GetCodeName(soc);
	GetTechnology(soc);
	GetSpecPower(soc);
	GetSocketPackageFromSmbios(soc);
	auto RealInformation = CPUDB::Instance().FindElements(*ParserInformation);
	if (!RealInformation)
		return;
	else
	{
		if (RealInformation->CodeName.Priority || soc.m_Data.CodeName.empty())
		{
			soc.m_Data.CodeName = RealInformation->CodeName.Element;
		}
		if (RealInformation->Technology.Priority || soc.m_Data.Technology.empty())
		{
			soc.m_Data.Technology = RealInformation->Technology.Element;
		}
		if (RealInformation->MaxTDP.Priority || soc.m_Data.MaxTDP.empty())
		{
			soc.m_Data.MaxTDP = RealInformation->MaxTDP.Element;
		}
		if (RealInformation->SocketPackage.Priority || soc.m_Data.SocketPackage.empty())
		{
			soc.m_Data.SocketPackage = RealInformation->SocketPackage.Element;
		}
	}
}

void Hardware::CPU::IntelCPU::GetCodeName(Socket& soc)
{
	switch (soc.m_Data.DisplayFamily) {
	case 0x06: {
		switch (soc.m_Data.DisplayModel) {
		case 0x0F: // Intel Core 2 (65nm)
			soc.m_Data.CodeName = "Core";
			break;
		case 0x17: // Intel Core 2 (45nm)
			soc.m_Data.CodeName = "Core";
			break;
		case 0x1C: // Intel Atom (45nm)
			soc.m_Data.CodeName = "Atom";
			break;
		case 0x1F: // Intel Core i5, i7
			soc.m_Data.CodeName = "Nehalem";
			break;
		case 0x1A: // Intel Core i7 LGA1366 (45nm)
		case 0x1E: // Intel Core i5, i7 LGA1156 (45nm)
		case 0x2E: // Intel Xeon Processor 7500 series (45nm)
			soc.m_Data.CodeName = "Nehalem";
			break;
		case 0x25: // Intel Core i3, i5, i7 LGA1156 (32nm)
		case 0x2C: // Intel Core i7 LGA1366 (32nm) 6 Core
		case 0x2F: // Intel Xeon Processor (32nm)
			soc.m_Data.CodeName = "Nehalem";
			break;
		case 0x2A: // Intel Core i5, i7 2xxx LGA1155 (32nm)
		case 0x2D: // Next Generation Intel Xeon, i7 3xxx LGA2011 (32nm)
			soc.m_Data.CodeName = "SandyBridge";
			break;
		case 0x3A: // Intel Core i5, i7 3xxx LGA1155 (22nm)
		case 0x3E: // Intel Core i7 4xxx LGA2011 (22nm)
			soc.m_Data.CodeName = "IvyBridge";
			break;
		case 0x3C: // Intel Core i5, i7 4xxx LGA1150 (22nm)
		case 0x3F: // Intel Xeon E5-2600/1600 v3, Core i7-59xx
				   // LGA2011-v3, Haswell-E (22nm)
		case 0x45: // Intel Core i5, i7 4xxxU (22nm)
		case 0x46:
			soc.m_Data.CodeName = "Haswell";
			break;
		case 0x3D: // Intel Core M-5xxx (14nm)
		case 0x47: // Intel i5, i7 5xxx, Xeon E3-1200 v4 (14nm)
			soc.m_Data.CodeName = "Broadwell";
			break;
		case 0x4F: // Intel Xeon E5-26xx v4
		case 0x56: // Intel Xeon D-15xx
			soc.m_Data.CodeName = "Broadwell";
			break;
		case 0x36: // Intel Atom S1xxx, D2xxx, N2xxx (32nm)
			soc.m_Data.CodeName = "Atom";
			break;
		case 0x37: // Intel Atom E3xxx, Z3xxx (22nm)
		case 0x4D: // Intel Atom C2xxx (22nm)
			soc.m_Data.CodeName = "Silvermont";
			break;
		case 0x4A:
		case 0x5A:
		case 0x5D:
			soc.m_Data.CodeName = "Silvermont";
			break;
		case 0x4E:
		case 0x5E: // Intel Core i5, i7 6xxxx LGA1151 (14nm)
		case 0x55: // Intel Core i7, i9 7xxxx LGA2066 (14nm)
			soc.m_Data.CodeName = "Skylake";
			break;
		case 0x4C:
			soc.m_Data.CodeName = "Airmont";
			break;
		case 0x8E:
		case 0x9E: // Intel Core i5, i7 7xxxx (14nm)
			soc.m_Data.CodeName = "KabyLake";
			break;
		case 0x5C: // Intel Atom processors (Apollo Lake) (14nm)
		case 0x5F: // Intel Atom processors (Denverton) (14nm)
			soc.m_Data.CodeName = "Goldmont";
			break;
		case 0x7A: // Intel Atom processors (14nm)
			soc.m_Data.CodeName = "GoldmontPlus";
			break;
		case 0x66: // Intel Core i3 8121U (10nm)
			soc.m_Data.CodeName = "CannonLake";
			break;
		case 0x7D: // Intel Core i3, i5, i7 10xxGx (10nm)
		case 0x7E:
		case 0x6A: // Intel Xeon (10nm)
		case 0x6C:
			soc.m_Data.CodeName = "IceLake";
			break;
		case 0xA5:
		case 0xA6: // Intel Core i3, i5, i7 10xxxU (14nm)
			soc.m_Data.CodeName = "CometLake";
			break;
		case 0x86: // Intel Atom processors
			soc.m_Data.CodeName = "Tremont";
			break;
		case 0x8C: // Intel processors (10nm++)
		case 0x8D:
			soc.m_Data.CodeName = "TigerLake";
			break;
		default:
			soc.m_Data.CodeName = "Unknown";
			break;
		}
	} break;
	case 0x0F: {
		switch (soc.m_Data.DisplayModel) {
		case 0x00: // Pentium 4 (180nm)
		case 0x01: // Pentium 4 (130nm)
		case 0x02: // Pentium 4 (130nm)
		case 0x03: // Pentium 4, Celeron D (90nm)
		case 0x04: // Pentium 4, Pentium D, Celeron D (90nm)
		case 0x06: // Pentium 4, Pentium D, Celeron D (65nm)
			soc.m_Data.CodeName = "NetBurst";
			break;
		default:
			soc.m_Data.CodeName = "Unknown";
			break;
		}
	} break;
	default:
		soc.m_Data.CodeName = "Unknown";
		break;
	}
}

void Hardware::CPU::IntelCPU::GetTechnology(Socket& soc)
{
	switch (soc.m_Data.DisplayFamily) {
	case 0x06: {
		switch (soc.m_Data.DisplayModel) {
		case 0x0F: // Intel Core 2 (65nm)
			soc.m_Data.Technology = "65";
			break;
		case 0x17: // Intel Core 2 (45nm)
			soc.m_Data.Technology = "45";
			break;
		case 0x1C: // Intel Atom (45nm)
			soc.m_Data.Technology = "45";
			break;
		case 0x1F: // Intel Core i5, i7
			break;
		case 0x1A: // Intel Core i7 LGA1366 (45nm)
		case 0x1E: // Intel Core i5, i7 LGA1156 (45nm)
		case 0x2E: // Intel Xeon Processor 7500 series (45nm)
			soc.m_Data.Technology = "45";
			break;
		case 0x25: // Intel Core i3, i5, i7 LGA1156 (32nm)
		case 0x2C: // Intel Core i7 LGA1366 (32nm) 6 Core
		case 0x2F: // Intel Xeon Processor (32nm)
			soc.m_Data.Technology = "32";
			break;
		case 0x2A: // Intel Core i5, i7 2xxx LGA1155 (32nm)
		case 0x2D: // Next Generation Intel Xeon, i7 3xxx LGA2011 (32nm)
			soc.m_Data.Technology = "32";
			break;
		case 0x3A: // Intel Core i5, i7 3xxx LGA1155 (22nm)
		case 0x3E: // Intel Core i7 4xxx LGA2011 (22nm)
			soc.m_Data.Technology = "22";
			break;
		case 0x3C: // Intel Core i5, i7 4xxx LGA1150 (22nm)
		case 0x3F: // Intel Xeon E5-2600/1600 v3, Core i7-59xx
				   // LGA2011-v3, Haswell-E (22nm)
		case 0x45: // Intel Core i5, i7 4xxxU (22nm)
		case 0x46:
			soc.m_Data.Technology = "22";
			break;
		case 0x3D: // Intel Core M-5xxx (14nm)
		case 0x47: // Intel i5, i7 5xxx, Xeon E3-1200 v4 (14nm)
			soc.m_Data.Technology = "14";
			break;
		case 0x4F: // Intel Xeon E5-26xx v4
		case 0x56: // Intel Xeon D-15xx
			break;
		case 0x36: // Intel Atom S1xxx, D2xxx, N2xxx (32nm)
			soc.m_Data.Technology = "32";
			break;
		case 0x37: // Intel Atom E3xxx, Z3xxx (22nm)
		case 0x4D: // Intel Atom C2xxx (22nm)
			soc.m_Data.Technology = "22";
			break;
		case 0x4A:
		case 0x5A:
		case 0x5D:
			break;
		case 0x4E:
		case 0x5E: // Intel Core i5, i7 6xxxx LGA1151 (14nm)
		case 0x55: // Intel Core i7, i9 7xxxx LGA2066 (14nm)
			soc.m_Data.Technology = "14";
			break;
		case 0x4C:
			break;
		case 0x8E:
		case 0x9E: // Intel Core i5, i7 7xxxx (14nm)
			soc.m_Data.Technology = "14";
			break;
		case 0x5C: // Intel Atom processors
			break;
		default:
			break;
		}
	} break;
	case 0x0F: {
		switch (soc.m_Data.DisplayModel) {
		case 0x00: // Pentium 4 (180nm)
			soc.m_Data.Technology = "180";
			break;
		case 0x01: // Pentium 4 (130nm)
		case 0x02: // Pentium 4 (130nm)
			soc.m_Data.Technology = "130";
			break;
		case 0x03: // Pentium 4, Celeron D (90nm)
		case 0x04: // Pentium 4, Pentium D, Celeron D (90nm)
			soc.m_Data.Technology = "90";
			break;
		case 0x06: // Pentium 4, Pentium D, Celeron D (65nm)
			soc.m_Data.Technology = "65";
			break;
		default:
			break;
		}
	} break;
	default:
		break;
	}
}
#pragma endregion

#pragma region 动态信息相关
void Hardware::CPU::IntelCPU::GetSpecPower(Socket& soc)
{
	constexpr auto MSR_RAPL_POWER_UNIT = 0x606;
	constexpr auto MSR_PKG_POWER_INFO = 0x614;
	auto thread = soc.findThreadByOSID(0);
	if (!thread.expired())
	{
		auto Ptr = thread.lock();
		auto TempMsrHandle = Ptr->GetMsr();
		if (!TempMsrHandle.expired())
		{
			auto Msr = TempMsrHandle.lock();
			Utils::CvtDs cvtds{};
			auto res = Msr->read(MSR_RAPL_POWER_UNIT, cvtds.ui64);
			auto energyUnitMultiplier = 0;
			if (res)
			{
				energyUnitMultiplier = 1 << Utils::extract_bits_ui(cvtds.ui32.Eax, 0, 3);
				res = Msr->read(MSR_PKG_POWER_INFO, cvtds.ui64);
				if (res)
				{
					soc.m_Data.MaxTDP = std::to_string(Utils::extract_bits_ui(cvtds.ui32.Eax, 0, 14) / energyUnitMultiplier);
				}
			}
		}
		else
		{
			spdlog::error("Intel CPU Can't Support core {} power msr: {}", Ptr->ThreadID(), MSR_RAPL_POWER_UNIT);
		}
	}
}

void Hardware::CPU::IntelCPU::GetSocketPackageFromSmbios(Socket& soc)
{
	auto ProcessorInfos = Smbios::GetType(Smbios::SmbiosHeaderType::Type4());
	if (!ProcessorInfos.empty())
	{
		soc.m_Data.SocketPackage = ProcessorInfos[0].ProcessorUpgrade();
	}
}

uint64_t Hardware::CPU::IntelCPU::GetTjMax(const Socket& soc, const std::shared_ptr<Hardware::Utils::Ring0::SafeMsrHandle>& msr)
{
	uint64_t tjMax = 0;
	switch (soc.m_Data.Family) {
	case 0x06: {
		switch (soc.m_Data.Model) {
		case 0x0F: // Intel Core 2 (65nm)
			switch (soc.m_Data.Stepping) {
			case 0x06: //
				switch (soc.Cores().size()) {
				case 2:
					tjMax = (80 + 10); break;
				case 4:
					tjMax = (90 + 10); break;
				default:
					tjMax = (85 + 10); break;
				}
				tjMax = (80 + 10); break;
			case 0x0B: // G0
				tjMax = (90 + 10); break;
			case 0x0D: // M0
				tjMax = (85 + 10); break;
			default:
				tjMax = (85 + 10); break;
			} break;
		case 0x17: // Intel Core 2 (45nm)
			tjMax = 100; break;
		case 0x1C: // Intel Atom (45nm)
			switch (soc.m_Data.Stepping) {
			case 0x02: // C0
				tjMax = 90; break;
			case 0x0A: // A0, B0
				tjMax = 100; break;
			default:
				tjMax = 90;
				break;
			} break;
		case 0x1A: // Intel Core i7 LGA1366 (45nm)
		case 0x1E: // Intel Core i5, i7 LGA1156 (45nm)
		case 0x1F: // Intel Core i5, i7
		case 0x25: // Intel Core i3, i5, i7 LGA1156 (32nm)
		case 0x2C: // Intel Core i7 LGA1366 (32nm) 6 Core
		case 0x2E: // Intel Xeon Processor 7500 series (45nm)
		case 0x2F: // Intel Xeon Processor (32nm)
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x2A: // Intel Core i5, i7 2xxx LGA1155 (32nm)
		case 0x2D: // Next Generation Intel Xeon, i7 3xxx LGA2011 (32nm)
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x3A: // Intel Core i5, i7 3xxx LGA1155 (22nm)
		case 0x3E: // Intel Core i7 4xxx LGA2011 (22nm)
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x3C: // Intel Core i5, i7 4xxx LGA1150 (22nm)
		case 0x3F: // Intel Xeon E5-2600/1600 v3, Core i7-59xx
				   // LGA2011-v3, Haswell-E (22nm)
		case 0x45: // Intel Core i5, i7 4xxxU (22nm)
		case 0x46:
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x3D: // Intel Core M-5xxx (14nm)
		case 0x47: // Intel i5, i7 5xxx, Xeon E3-1200 v4 (14nm)
		case 0x4F: // Intel Xeon E5-26xx v4
		case 0x56: // Intel Xeon D-15xx
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x36: // Intel Atom S1xxx, D2xxx, N2xxx (32nm)
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x37: // Intel Atom E3xxx, Z3xxx (22nm)
		case 0x4A:
		case 0x4D: // Intel Atom C2xxx (22nm)
		case 0x5A:
		case 0x5D:
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x4E:
		case 0x5E: // Intel Core i5, i7 6xxxx LGA1151 (14nm)
		case 0x55: // Intel Core i7, i9 7xxxx LGA2066 (14nm)
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x4C:
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x8E:
		case 0x9E: // Intel Core i5, i7 7xxxx (14nm)
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x5C: // Intel Atom processors (Apollo Lake) (14nm)
		case 0x5F: // Intel Atom processors (Denverton) (14nm)
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x7A: // Intel Atom processors (14nm)
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x66: // Intel Core i3 8121U (10nm)
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x7D: // Intel Core i3, i5, i7 10xxGx (10nm)
		case 0x7E:
		case 0x6A: // Intel Xeon (10nm)
		case 0x6C:
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0xA5:
		case 0xA6: // Intel Core i3, i5, i7 10xxxU (14nm)
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x86: // Intel Atom processors
			tjMax = GetTjMaxFromMsr(msr);
			break;
		case 0x8C: // Intel processors (10nm++)
		case 0x8D:
			tjMax = GetTjMaxFromMsr(msr);
			break;
		default:
			tjMax = 100;
			break;
		}
	} break;
	case 0x0F: {
		switch (soc.m_Data.Model) {
		case 0x00: // Pentium 4 (180nm)
		case 0x01: // Pentium 4 (130nm)
		case 0x02: // Pentium 4 (130nm)
		case 0x03: // Pentium 4, Celeron D (90nm)
		case 0x04: // Pentium 4, Pentium D, Celeron D (90nm)
		case 0x06: // Pentium 4, Pentium D, Celeron D (65nm)
			tjMax = 100;
			break;
		default:
			tjMax = 100;
			break;
		}
	} break;
	default:
		tjMax = 100;
		break;
	}

	return tjMax;
}

uint64_t Hardware::CPU::IntelCPU::GetTjMaxFromMsr(const std::shared_ptr<Hardware::Utils::Ring0::SafeMsrHandle>& msr)
{
	uint64_t PackageTjmax = 100;
	constexpr auto IA32_TEMPERATURE_TARGET = 0x1A2;
	Utils::CvtDs value;
	auto res = msr->read(IA32_TEMPERATURE_TARGET, value.ui64);
	if (res)
	{
		PackageTjmax = Utils::extract_bits_ui(value.ui32.Eax, 16, 23);
	}
	return PackageTjmax;
}

void Hardware::CPU::IntelCPU::AddDynamicInfo()
{
	AddDynamicInfoForSocket();
	AddDynamicInfoForHyperThead();
}

void Hardware::CPU::IntelCPU::AddDynamicInfoForSocket()
{
	constexpr std::array < std::pair<uint64_t, const char*>, 4> MstEle =
	{ {
		{0x611, "PackagePower"},
		{0x639, "CoresPower"},
		{0x619, "DRAMPower"},
		{0x641, "GTPower"},
	} };
	for (auto& Soc : m_SystemRoot)
	{
		AddPackageTemperature(Soc.second);
		for (const auto& ele : MstEle)
		{
			AddPower(Soc.second, ele.first, ele.second);
		}
	}
}

void Hardware::CPU::IntelCPU::AddDynamicInfoForHyperThead()
{
	for (const auto& topologyEle : topology)
	{
		for (auto& Soc : m_SystemRoot)
		{
			if (Soc.second.ApicId() == topologyEle.socket)
			{
				auto TargetThread = Soc.second.findThreadByOSID(topologyEle.os_id);
				{
					AddCoreTemperature(Soc.second, TargetThread);
					AddCoreVoltage(TargetThread);
					AddCoreFrequency(TargetThread);
				}
			}
		}
	}
}

void Hardware::CPU::IntelCPU::AddPackageTemperature(Socket& soc)
{
	constexpr auto IA32_PACKAGE_THERM_STATUS = 0x1B1;
	auto thread = soc.findThreadByOSID(0);

	if (!thread.expired())
	{
		auto Ptr = thread.lock();
		auto TempThreadMsrHandle = Ptr->GetMsr();
		if (!TempThreadMsrHandle.expired())
		{
			auto Msr = TempThreadMsrHandle.lock();
			auto PackageTjmax = GetTjMax(soc, Msr);
			Utils::CvtDs value{};

			if (Msr->read(IA32_PACKAGE_THERM_STATUS, value.ui64) && value.ui64)
			{
				spdlog::info("Intel CPU Support package temperature msr: {:x}, Tjmax: {}", IA32_PACKAGE_THERM_STATUS, PackageTjmax);
				soc.AddDecorator(std::make_unique<IntelTemperature>(IA32_PACKAGE_THERM_STATUS, "PackageTemperature", PackageTjmax));
			}
			else
			{
				spdlog::error("Intel CPU Can't Support package temperature, msr: {:x}", IA32_PACKAGE_THERM_STATUS);
			}
		}
	}
}

void Hardware::CPU::IntelCPU::AddCoreTemperature(const Socket& soc, std::weak_ptr<HyperThread> thread)
{
	constexpr auto IA32_THERM_STATUS_MSR = 0x019C;
	if (!thread.expired())
	{
		auto Ptr = thread.lock();
		auto TempMsrHandle = Ptr->GetMsr();
		if (!TempMsrHandle.expired())
		{
			auto Msr = Ptr->GetMsr().lock();
			auto PackageTjmax = GetTjMax(soc, Msr);
			Utils::CvtDs value{};
			if (Msr->read(IA32_THERM_STATUS_MSR, value.ui64) && value.ui64)
			{
				spdlog::info("Intel CPU Support core {} temperature msr: {:x}, Tjmax: {}", Msr->getCoreId(), IA32_THERM_STATUS_MSR, PackageTjmax);
				Ptr->AddDecorator(std::make_unique<IntelTemperature>(IA32_THERM_STATUS_MSR, "CoreTemperature", PackageTjmax));
			}
			else
			{
				spdlog::error("Intel CPU Can't Support core {:x} temperature msr: {}", Msr->getCoreId(), IA32_THERM_STATUS_MSR);
			}
		}
	}
}

void Hardware::CPU::IntelCPU::AddCoreVoltage(std::weak_ptr<HyperThread> thread)
{
	constexpr auto IA32_PERF_STATUS = 0x198;
	if (!thread.expired())
	{
		auto Ptr = thread.lock();
		auto TempMsrHandle = Ptr->GetMsr();
		if (!TempMsrHandle.expired())
		{
			auto Msr = Ptr->GetMsr().lock();
			Utils::CvtDs value{};
			auto res = Msr->read(IA32_PERF_STATUS, value.ui64);
			if (res && value.ui32.Edx)
			{
				spdlog::info("Intel CPU Support core {} voltage msr: {:x}", Msr->getCoreId(), IA32_PERF_STATUS);
				Ptr->AddDecorator(std::make_unique<IntelVoltage>(IA32_PERF_STATUS, "CoreVoltage"));
			}
			else
			{
				spdlog::error("Intel CPU Can't Support core {} voltage msr: {:x}, value: {:x}", Msr->getCoreId(), IA32_PERF_STATUS, value.ui32.Edx);
			}
		}
	}
}

void Hardware::CPU::IntelCPU::AddCoreFrequency(std::weak_ptr<HyperThread> thread)
{
	constexpr auto IA32_PERF_STATUS = 0x198;
	constexpr auto MSR_PLATFORM_INFO = 0xCE;

	if (!thread.expired())
	{
		auto Ptr = thread.lock();
		auto TempMsrHandle = Ptr->GetMsr();
		if (!TempMsrHandle.expired())
		{
			auto Msr = TempMsrHandle.lock();
			Utils::CvtDs value{};
			auto res = Msr->read(IA32_PERF_STATUS, value.ui64);
			if (res && value.ui32.Eax)
			{
				Utils::CvtDs cvtds{};
				res = Msr->read(MSR_PLATFORM_INFO, cvtds.ui64);
				uint64_t TSCFrequency = 0;
				if (res)
				{
					auto MaxNonTurboFre = Utils::extract_bits_ui(cvtds.ui32.Eax, 8, 15);
					Utils::CPUID_INFO info;
					Utils::GetCpuid(0, info);
					if (info.reg.eax >= 0x15)
					{
						// From https://github.com/torvalds/linux/blob/master/tools/power/x86/turbostat/turbostat.c#L3436
						unsigned int crystal_hz{ 0 };

						/*
						 * CPUID 15H TSC/Crystal ratio, possibly Crystal Hz
						 */
						Utils::GetCpuid(0x15, info);

						if (info.reg.ebx != 0)
						{
							if (crystal_hz == 0)
								switch (DisplayModel)
								{
								case INTEL_FAM6_SKYLAKE_L:	/* SKL */
									crystal_hz = 24000000;	/* 24.0 MHz */
									break;
								case INTEL_FAM6_ATOM_GOLDMONT_D:	/* DNV */
									crystal_hz = 25000000;	/* 25.0 MHz */
									break;
								case INTEL_FAM6_ATOM_GOLDMONT:	/* BXT */
								case INTEL_FAM6_ATOM_GOLDMONT_PLUS:
									crystal_hz = 19200000;	/* 19.2 MHz */
									break;
								default:
									crystal_hz = 0;
								}

							if (crystal_hz)
							{
								TSCFrequency = (unsigned long long) crystal_hz * info.reg.ebx / info.reg.eax;
							}
						}
						if (!TSCFrequency)
							TSCFrequency = MaxNonTurboFre * 100;
					}
				}
				if (TSCFrequency)
				{
					spdlog::info("Intel CPU Support core {} voltage tsc Frequency: {}", Msr->getCoreId(), TSCFrequency);
					Ptr->AddDecorator(std::make_unique<IntelFrequency>(IA32_PERF_STATUS, "CoreFrequency", TSCFrequency));
				}
				else
				{
					spdlog::error("Intel CPU Can't found TSC frequercy core {:x}  msr: {}", Msr->getCoreId(), MSR_PLATFORM_INFO);
				}
			}
			else
			{
				spdlog::error("Intel CPU Can't Support core {} frequercy msr: {:x}, value: {}", Msr->getCoreId(), IA32_PERF_STATUS, value.ui32.Eax);
			}
		}
	}
}

void Hardware::CPU::IntelCPU::AddPower(Socket& soc, const uint64_t MsrRegistry, const std::string& PowerName)
{
	constexpr auto MSR_RAPL_POWER_UNIT = 0x606;
	auto thread = soc.findThreadByOSID(0);
	if (!thread.expired())
	{
		auto Ptr = thread.lock();
		auto TempMsrHandle = Ptr->GetMsr();
		if (!TempMsrHandle.expired())
		{
			auto Msr = TempMsrHandle.lock();
			Utils::CvtDs cvtds{};
			auto res = Msr->read(MSR_RAPL_POWER_UNIT, cvtds.ui64);
			double energyUnitMultiplier{};
			if (res && cvtds.ui64)
			{
				if (soc.m_Data.CodeName == "Silvermont" || soc.m_Data.CodeName == "Airmont")
					energyUnitMultiplier = 1.0e-6f * (1 << (int)((cvtds.ui32.Eax >> 8) & 0x1F));
				else
					energyUnitMultiplier = 1.0 / (1 << (int)((cvtds.ui32.Eax >> 8) & 0x1F));
			}

			if (Msr->read(MsrRegistry, cvtds.ui64) && cvtds.ui64)
			{
				spdlog::info("Intel CPU Support core {} power msr: {}", Msr->getCoreId(), MsrRegistry);
				soc.AddDecorator(std::make_unique<IntelPower>(MsrRegistry, PowerName, energyUnitMultiplier, cvtds.ui32.Eax));
			}
			else
			{
				spdlog::error("Intel CPU Can't Support core {} power msr: {}", Msr->getCoreId(), MsrRegistry);
			}
		}
	}
}

#pragma endregion
Hardware::CPU::IntelTemperature::IntelTemperature(const uint64_t MsrRegistry, const std::string& Name, const int64_t TjMax) : CPUDecorator(MsrRegistry, Name), m_Tjmax(TjMax)
{
	m_TjCurrent = (std::numeric_limits<int64_t>::max)();
}

const std::string Hardware::CPU::IntelTemperature::GetDecoratorValue() const
{
	if (m_TjCurrent != (std::numeric_limits<int64_t>::max)())
	{
		if (IsUpdate)
		{
			IsUpdate = false;
			return std::to_string(m_TjCurrent) + " Degree";
		}
	}
	return {};
}

void Hardware::CPU::IntelTemperature::Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr)
{
	if (!Msr.expired())
	{
		auto MsrPtr = Msr.lock();
		Utils::CvtDs value{};
		auto res = MsrPtr->read(m_MsrRegistry, value.ui64);
		if (res)
		{
			IsUpdate = true;
			m_TjCurrent = m_Tjmax - Utils::extract_bits_ui(value.ui32.Eax, 16, 22);
		}
	}
}

Hardware::CPU::IntelPower::IntelPower(const uint64_t MsrRegistry, const std::string& Name, const double energyUnitMultiplier, const uint32_t lastEnergyConsumed) :
	CPUDecorator(MsrRegistry, Name), m_EnergyUnitMultiplier(energyUnitMultiplier), m_LastEnergyConsumed(lastEnergyConsumed)
{
	m_CurrentPower = (std::numeric_limits<double>::max)();
	m_LastEnergyTime = std::chrono::high_resolution_clock::now();
}

const std::string Hardware::CPU::IntelPower::GetDecoratorValue() const
{
	if (m_CurrentPower < (std::numeric_limits<double>::max)())
	{
		if (IsUpdate)
		{
			IsUpdate = false;
			return Utils::to_string_with_precision(m_CurrentPower) + " Watt";
		}
	}

	return {};
}

void Hardware::CPU::IntelPower::Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr)
{
	if (!Msr.expired())
	{
		auto MsrPtr = Msr.lock();
		Utils::CvtDs value{};
		auto res = MsrPtr->read(m_MsrRegistry, value.ui64);
		if (res)
		{
			auto CurrentTime = std::chrono::high_resolution_clock::now();
			auto energyConsumed = value.ui32.Eax;
			std::chrono::duration<double> deltaTime = CurrentTime - m_LastEnergyTime;
			if (deltaTime.count() < 0.01)
			{
				// 间隔时间太短会导致读取数据不准，将丢弃此次数据
				spdlog::debug("read tsc too fast ,from: {}", m_Name.c_str());
				return;
			}
			m_CurrentPower = (energyConsumed - m_LastEnergyConsumed) / deltaTime.count() * m_EnergyUnitMultiplier;

			m_LastEnergyTime = CurrentTime;
			m_LastEnergyConsumed = energyConsumed;
			IsUpdate = true;
		}
	}
}

Hardware::CPU::IntelVoltage::IntelVoltage(const uint64_t MsrRegistry, const std::string& Name) : CPUDecorator(MsrRegistry, Name)
{
	m_CurrentVoltage = std::numeric_limits<double>::infinity();
}

const std::string Hardware::CPU::IntelVoltage::GetDecoratorValue() const
{
	if (m_CurrentVoltage != std::numeric_limits<double>::infinity())
	{
		if (IsUpdate)
		{
			IsUpdate = false;
			return Utils::to_string_with_precision(m_CurrentVoltage) + " V";
		}
	}
	return {};
}

void Hardware::CPU::IntelVoltage::Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr)
{
	if (!Msr.expired())
	{
		auto MsrPtr = Msr.lock();
		Utils::CvtDs value{};
		auto res = MsrPtr->read(m_MsrRegistry, value.ui64);
		if (res && value.ui32.Edx)
		{
			// TODO 方法确认
			IsUpdate = true;
			m_CurrentVoltage = Utils::extract_bits(value.ui64, 32, 45) / (double)pow(2, 13);
		}
	}
}

Hardware::CPU::IntelFrequency::IntelFrequency(const uint64_t MsrRegistry, const std::string& Name, const uint64_t TscFrequencyMhz) : CPUDecorator(MsrRegistry, Name),
m_TscFrequencyMhz(TscFrequencyMhz * 1.0)
{
	m_CurrentFrequency = std::numeric_limits<double>::infinity();
}

const std::string Hardware::CPU::IntelFrequency::GetDecoratorValue() const
{
	if (m_CurrentFrequency != std::numeric_limits<double>::infinity())
	{
		if (IsUpdate)
		{
			IsUpdate = false;
			return Utils::to_string_with_precision(m_CurrentFrequency) + " Mhz";
		}
	}
	return {};
}

void Hardware::CPU::IntelFrequency::Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr)
{
	if (!Msr.expired())
	{
		auto MsrPtr = Msr.lock();
		Utils::CvtDs value{};
		auto res = MsrPtr->read(m_MsrRegistry, value.ui64);
		if (res)
		{
			IsUpdate = true;
			m_CurrentFrequency = GetBusSpeedByTsc(MsrPtr->getCoreId()) * Utils::extract_bits_ui(value.ui32.Eax, 8, 15);
		}
	}
}

double Hardware::CPU::IntelFrequency::GetBusSpeedByTsc(const int32_t& coreid)
{
	int i = 10;
	double res = 0.0;
	Utils::Ring0::SafeTscIntervalHandle TscReader(coreid);
	uint64_t value;
	while (i--)
	{
		TscReader.read(value);
		res += (value / 1000000.0);
	}
	return res / m_TscFrequencyMhz * 10.0;
}