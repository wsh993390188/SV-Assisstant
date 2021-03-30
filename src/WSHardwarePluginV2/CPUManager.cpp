#include "stdafx.h"
#include <array>
#include "CPUManager.h"

#include "IntelCPU.h"
#include "AmdCPU.h"
#include "ZhaoXinCPU.h"

constexpr auto CPUID_VENDOR_OLDAMD = "AMDisbetter!" /* early engineering samples of AMD K5 processor */;
constexpr auto CPUID_VENDOR_AMD = "AuthenticAMD";
constexpr auto CPUID_VENDOR_INTEL = "GenuineIntel";
constexpr auto CPUID_VENDOR_VIA = "CentaurHauls";
constexpr auto CPUID_VENDOR_OLDTRANSMETA = "TransmetaCPU";
constexpr auto CPUID_VENDOR_TRANSMETA = "GenuineTMx86";
constexpr auto CPUID_VENDOR_CYRIX = "CyrixInstead";
constexpr auto CPUID_VENDOR_CENTAUR = "CentaurHauls";
constexpr auto CPUID_VENDOR_NEXGEN = "NexGenDriven";
constexpr auto CPUID_VENDOR_UMC = "UMC UMC UMC ";
constexpr auto CPUID_VENDOR_SIS = "SiS SiS SiS ";
constexpr auto CPUID_VENDOR_NSC = "Geode by NSC";
constexpr auto CPUID_VENDOR_RISE = "RiseRiseRise";
constexpr auto CPUID_VENDOR_VORTEX = "Vortex86 SoC";

/*Vendor-strings from Virtual Machines.*/
constexpr auto CPUID_VENDOR_VMWARE = "VMwareVMware";
constexpr auto CPUID_VENDOR_XENHVM = "XenVMMXenVMM";
constexpr auto CPUID_VENDOR_MICROSOFT_HV = "Microsoft Hv";
constexpr auto CPUID_VENDOR_PARALLELS = " lrpepyh vr";

Hardware::Data::ErrorType Hardware::CPUManager::Init(LPCSTR, std::string& response)
{
	Data::ErrorType res = Data::ErrorType::SUCCESS;
	// TODO Dual Socket CPUÅÐ¶Ï
	switch (GetCPUBrandsFromCPUID())
	{
	case CPU_Brands::Intel:
		m_CPUs = std::make_unique< CPU::IntelCPU >();
		res = m_CPUs->Initialize(response);
		break;
	case CPU_Brands::Amd:
		m_CPUs = std::make_unique< CPU::AmdCPU >();
		res = m_CPUs->Initialize(response);
		break;
	case CPU_Brands::Zhaoxin:
		m_CPUs = std::make_unique< CPU::ZhaoXinCPU >();
		res = m_CPUs->Initialize(response);
		break;
	default:
		res = Data::ErrorType::UNKNOWNCPU;
		break;
	}
	return res;
}

Hardware::Data::ErrorType Hardware::CPUManager::Update(LPCSTR paramter, std::string& response)
{
	if (!paramter)
		return Data::ErrorType::UNKNOWNJSON;
	if (!m_CPUs)
		return Data::ErrorType::NOTINITIALIZE;
	return m_CPUs->Update(paramter, response);
}

Hardware::Data::ErrorType Hardware::CPUManager::GetElements(LPCSTR paramter, std::string& response)
{
	if (!paramter)
		return Data::ErrorType::UNKNOWNJSON;
	if (!m_CPUs)
		return Data::ErrorType::NOTINITIALIZE;
	return m_CPUs->GetElements(paramter, response);
}

const Hardware::CPUManager::CPU_Brands Hardware::CPUManager::GetCPUBrandsFromCPUID() noexcept
{
	std::array<int, 4> buf;
	__cpuidex(buf.data(), 0, 0);
	char vendor[0x20]{};
	*reinterpret_cast<int*>(vendor) = buf[1];
	*reinterpret_cast<int*>(vendor + 4) = buf[3];
	*reinterpret_cast<int*>(vendor + 8) = buf[2];
	if (strcmp(vendor, CPUID_VENDOR_INTEL) == 0)
	{
		spdlog::info("Found Intel CPU");
		return CPU_Brands::Intel;
	}
	else if ((strcmp(vendor, CPUID_VENDOR_AMD) == 0) || (strcmp(vendor, CPUID_VENDOR_OLDAMD) == 0))
	{
		spdlog::info("Found Amd CPU");
		return CPU_Brands::Amd;
	}
	else if (strcmp(vendor, CPUID_VENDOR_VIA) == 0)
	{
		// ÔÝÊ±×¢ÊÍµôÕ×Ð¾&VIACPUÐÅÏ¢
		// 		spdlog::info("Found Õ×Ð¾ CPU");
		// 		return CPU_Brands::Zhaoxin;
	}

	spdlog::info("Found Unknown CPU {}", vendor);
	return CPU_Brands::Unknown;
}