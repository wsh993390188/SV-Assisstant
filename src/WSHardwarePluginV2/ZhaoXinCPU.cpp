#include "stdafx.h"
#include <bitset>
#include "ZhaoXinCPU.h"
#include "CPUWMI.h"

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

Hardware::CPU::ZhaoXinCPU::ZhaoXinCPU() : GenericCPU()
{
	InitializeSocketFromWMI();
}

Hardware::Data::ErrorType Hardware::CPU::ZhaoXinCPU::Initialize(std::string& response)
{
	GetInfoFromCPUID();
	AddDynamicInfo();
	response = BuildInitializeJson();
	return Data::ErrorType::SUCCESS;
}

Hardware::Data::ErrorType Hardware::CPU::ZhaoXinCPU::Update(const std::string& Args, std::string& response)
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

bool Hardware::CPU::ZhaoXinCPU::InitializeSocketFromWMI()
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

void Hardware::CPU::ZhaoXinCPU::GetInfoFromCPUID()
{
	for (auto& soc : m_SystemRoot)
	{
		GetInfoFromCPUID1(soc.second);
		GetInfoFromCPUID80000005_6(soc.second);
		GetInfoFromCPUID80000002(soc.second);
	}
}

void Hardware::CPU::ZhaoXinCPU::GetInfoFromCPUID1(Socket& soc)
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

void Hardware::CPU::ZhaoXinCPU::GetInfoFromCPUID80000005_6(Socket& soc)
{
	Utils::CPUID_INFO cpuid_args;
	Utils::GetCpuid(0x80000005, cpuid_args);
	Cache_info temp;
	temp.Cache_level = 1;
	temp.Cache_type = 'D';
	temp.Cache_line_size = ((cpuid_args.array[2] & L1_DATA_CACHE_LINE_SIZE) >> 0);
	temp.Cache_Ways = ((cpuid_args.array[2] & L1_DATA_CACHE_ASSOC) >> 16);
	temp.Cache_Size = ((cpuid_args.array[2] & L1_DATA_CACHE_SIZE) >> 24);
	soc.m_Data.Cache.emplace_back(temp);

	temp.Cache_level = 1;
	temp.Cache_type = 'T';
	temp.Cache_line_size = ((cpuid_args.array[3] & L1_DATA_CACHE_LINE_SIZE) >> 0);
	temp.Cache_Ways = ((cpuid_args.array[3] & L1_DATA_CACHE_ASSOC) >> 16);
	temp.Cache_Size = ((cpuid_args.array[3] & L1_DATA_CACHE_SIZE) >> 24);
	soc.m_Data.Cache.emplace_back(temp);

	Utils::GetCpuid(0x80000006, cpuid_args);
	temp.Cache_level = 2;
	temp.Cache_type = 'D';
	temp.Cache_line_size = ((cpuid_args.array[2] & L2_DATA_CACHE_LINE_SIZE) >> 0);
	temp.Cache_Ways = ((cpuid_args.array[2] & L2_DATA_CACHE_ASSOC) >> 12);
	temp.Cache_Size = ((cpuid_args.array[2] & L2_DATA_CACHE_SIZE) >> 16);
	soc.m_Data.Cache.emplace_back(temp);

	temp.Cache_level = 3;
	temp.Cache_type = 'D';
	temp.Cache_line_size = ((cpuid_args.array[3] & L3_INSTRUCTION_CACHE_LINE_SIZE) >> 0);
	temp.Cache_Ways = ((cpuid_args.array[2] & L2_DATA_CACHE_ASSOC) >> 12);
	temp.Cache_Size = ((cpuid_args.array[3] & L3_INSTRUCTION_CACHE_SIZE) >> 18) * 512;
	soc.m_Data.Cache.emplace_back(temp);
}

void Hardware::CPU::ZhaoXinCPU::GetInfoFromCPUID80000002(Socket& soc)
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

void Hardware::CPU::ZhaoXinCPU::AddDynamicInfo()
{
	AddDynamicInfoForSocket();
	AddDynamicInfoForHyperThead();
}

void Hardware::CPU::ZhaoXinCPU::AddDynamicInfoForSocket()
{
	for (auto& Soc : m_SystemRoot)
	{
		AddPackageTemperature(Soc.second);
	}
}

void Hardware::CPU::ZhaoXinCPU::AddDynamicInfoForHyperThead()
{
	for (const auto& topologyEle : topology)
	{
		for (auto& Soc : m_SystemRoot)
		{
			if (Soc.second.ApicId() == topologyEle.socket)
			{
				auto TargetThread = Soc.second.findThreadByOSID(topologyEle.os_id);
				{
					AddCoreVoltage(TargetThread);
					// TODO PStatus Frequency Affirm
					AddCoreFrequency(TargetThread);
				}
			}
		}
	}
}

void Hardware::CPU::ZhaoXinCPU::AddPackageTemperature(Socket& soc)
{
	constexpr auto MSR_Temperature = 0x1423;
	auto TempThread = soc.findThreadByOSID(0);
	if (!TempThread.expired())
	{
		auto ThreadPtr = TempThread.lock();
		auto TempMsrPtr = ThreadPtr->GetMsr();
		if (!TempMsrPtr.expired())
		{
			auto MsrPtr = TempMsrPtr.lock();
			Utils::CvtDs cvtds{};
			if (MsrPtr->read(MSR_Temperature, cvtds.ui64) && cvtds.ui64)
			{
				soc.AddDecorator(std::make_unique<ZhaoxinTemperature>(MSR_Temperature, "PackageTemperature"));
			}
		}
	}
}

void Hardware::CPU::ZhaoXinCPU::AddCoreVoltage(std::weak_ptr<HyperThread> thread)
{
	constexpr auto MSR_FIDVID = 0x198;
	if (!thread.expired())
	{
		auto ThreadPtr = thread.lock();
		auto TempMsrPtr = ThreadPtr->GetMsr();
		if (!TempMsrPtr.expired())
		{
			auto MsrPtr = TempMsrPtr.lock();
			Utils::CvtDs cvtds{};
			if (MsrPtr->read(MSR_FIDVID, cvtds.ui64) && cvtds.ui64)
			{
				ThreadPtr->AddDecorator(std::make_unique<ZhaoxinVoltage>(MSR_FIDVID, "CoreVoltage"));
			}
		}
	}
}

void Hardware::CPU::ZhaoXinCPU::AddCoreFrequency(std::weak_ptr<HyperThread> thread)
{
	constexpr auto MSR_FIDVID = 0x198;
	if (!thread.expired())
	{
		auto ThreadPtr = thread.lock();
		auto TempMsrPtr = ThreadPtr->GetMsr();
		if (!TempMsrPtr.expired())
		{
			auto MsrPtr = TempMsrPtr.lock();
			Utils::CvtDs cvtds{};
			if (MsrPtr->read(MSR_FIDVID, cvtds.ui64))
			{
				double BusSpeed = 0.0;
				if (MsrPtr->read(0xCD, cvtds.ui64))
				{
					switch (cvtds.ui32.Eax & 0x07)
					{
					case 0b101:
						BusSpeed = 100.0;
						break;
					case 0b001:
						BusSpeed = 133.33;
						break;
					case 0b011:
						BusSpeed = 166.67;
						break;
					case 0b010:
						BusSpeed = 200.0;
						break;
					case 0b000:
						BusSpeed = 266.67;
						break;
					case 0b100:
						BusSpeed = 333.33;
						break;
					case 0b110:
						BusSpeed = 400.0;
						break;
					default:
						break;
					}
					if (BusSpeed)
					{
						ThreadPtr->AddDecorator(std::make_unique<ZhaoxinFrequency>(MSR_FIDVID, "CoreFrequency", BusSpeed));
					}
				}
			}
		}
	}
}

Hardware::CPU::ZhaoxinTemperature::ZhaoxinTemperature(const uint64_t MsrRegistry, const std::string& Name) :
	CPUDecorator(MsrRegistry, Name)
{
	m_CurrentTemperature = std::numeric_limits<double>::infinity();
}

const std::string Hardware::CPU::ZhaoxinTemperature::GetDecoratorValue() const
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

void Hardware::CPU::ZhaoxinTemperature::Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr)
{
	if (!Msr.expired())
	{
		auto MsrPtr = Msr.lock();
		Utils::CvtDs value{};
		auto res = MsrPtr->read(m_MsrRegistry, value.ui64);
		if (res && value.ui32.Eax)
		{
			m_CurrentTemperature = value.ui32.Eax * 1.0;
			IsUpdate = true;
		}
	}
}

Hardware::CPU::ZhaoxinVoltage::ZhaoxinVoltage(const uint64_t MsrRegistry, const std::string& Name) :
	CPUDecorator(MsrRegistry, Name)
{
	m_CurrentVoltage = std::numeric_limits<double>::infinity();
}

const std::string Hardware::CPU::ZhaoxinVoltage::GetDecoratorValue() const
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

void Hardware::CPU::ZhaoxinVoltage::Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr)
{
	if (!Msr.expired())
	{
		auto MsrPtr = Msr.lock();
		Utils::CvtDs value{};
		auto res = MsrPtr->read(m_MsrRegistry, value.ui64);
		if (res && value.ui32.Eax)
		{
			m_CurrentVoltage = 1.5 - (value.ui32.Eax & 0xFF) * 0.0125;
			if (m_CurrentVoltage < 0.0)
				return;
			IsUpdate = true;
		}
	}
}

Hardware::CPU::ZhaoxinFrequency::ZhaoxinFrequency(const uint64_t MsrRegistry, const std::string& Name, const double BusSpeed) :
	CPUDecorator(MsrRegistry, Name), m_BusSpeed(BusSpeed)
{
	m_CurrentFrequency = std::numeric_limits<double>::infinity();
}

const std::string Hardware::CPU::ZhaoxinFrequency::GetDecoratorValue() const
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

void Hardware::CPU::ZhaoxinFrequency::Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr)
{
	if (!Msr.expired())
	{
		auto MsrPtr = Msr.lock();
		Utils::CvtDs value{};
		auto res = MsrPtr->read(m_MsrRegistry, value.ui64);
		if (res && value.ui32.Eax)
		{
			m_CurrentFrequency = ((value.ui32.Eax >> 8) & 0xFF) / 2 * m_BusSpeed;
			IsUpdate = true;
		}
	}
}