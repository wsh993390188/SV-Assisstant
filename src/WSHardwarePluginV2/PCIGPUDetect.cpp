#include "stdafx.h"
#include "PCIHandle.h"
#include "PCIGPUDetect.h"
#include "GPUDeviceFactory.h"

using namespace Hardware;

namespace
{
	template<typename T1, typename T2, typename T3>
	constexpr auto BuildPCIAddress(T1 Bus, T2  Dev, T3  Func) { return ((Bus & 0xFF) << 8) | ((Dev & 0x1F) << 3) | (Func & 7); }

	uint16_t GetBarAddressOffset(const uint32_t& VendorId)
	{
		switch (VendorId)
		{
		case GPU::IntelVendorId:
		case GPU::NvidiaVendorId:
			return 0x10;
		case GPU::AMDVendorId:
			return 0x18;
		case GPU::VMWareVendorId:
			return 0x18;
		default:
			break;
		}
		return 0x10;
	}
}

Hardware::GPU::PciGpuDetect::PciGpuDetect()
{
}

Hardware::GPU::PciGpuDetect::~PciGpuDetect()
{
}

Data::ErrorType Hardware::GPU::PciGpuDetect::Initialize(std::string& response)
{
	auto ret = InitializeDevice();
	if (ret == Data::ErrorType::SUCCESS)
	{
		response = BuildInitializeJson();
	}
	return ret;
}

Data::ErrorType Hardware::GPU::PciGpuDetect::Update(const std::string& Args, std::string& response)
{
	uint32_t GPUId;
	if (ParserJson(Args, GPUId))
	{
		if (auto Itr = GPUInfos.find(GPUId); Itr != GPUInfos.end())
		{
			response = Itr->second->UpdateGPUInfo();
			return Data::ErrorType::SUCCESS;
		}
	}
	return Data::ErrorType::UNKNOWNJSON;
}

Data::ErrorType Hardware::GPU::PciGpuDetect::GetElements(LPCSTR paramter, std::string& response)
{
	uint32_t GPUId;
	if (ParserJson(paramter, GPUId))
	{
		if (auto Itr = GPUInfos.find(GPUId); Itr != GPUInfos.end())
		{
			response = Itr->second->GetGPUInfo();
			return Data::ErrorType::SUCCESS;
		}
	}
	return Data::ErrorType::UNKNOWNJSON;
}

Data::ErrorType Hardware::GPU::PciGpuDetect::InitializeDevice()
{
	auto Devices = FindGPUDeviceOnPCI();
	for (const auto& Device : Devices)
	{
		auto Dev = GPUDeviceFactory::Instance().CreateDevice(Device);
		if (Dev)
			this->GPUInfos.emplace(Device.pciAddress, std::move(Dev));
		else
		{
			spdlog::error(R"(Cant create device vid:{:x}, did:{:x})", Device.VendorId, Device.DeviceId);
		}
	}

	if (GPUInfos.empty())
		return Data::ErrorType::DATAEMPTY;
	return Data::ErrorType::SUCCESS;
}

std::vector<GPU::GPUDevice> Hardware::GPU::PciGpuDetect::FindGPUDeviceOnPCI()
{
	DWORD VendorId = InvaildVendorId;
	Utils::Ring0::SafePCIHandle PciDeal;
	std::vector<GPUDevice> ret;
	for (auto bus = 0; bus <= 255; ++bus)
	{
		for (auto dev = 0; dev < 31; ++dev)
		{
			for (auto func = 0; func < 0x7; ++func)
			{
				if (PciDeal.ReadWORD(bus, dev, func, 0, VendorId) && VendorId != InvaildVendorId)
				{
					DWORD ClassCode{};
					if (PciDeal.ReadByte(bus, dev, func, 0x0B, ClassCode) && ClassCode == 0x03)
					{
						GPUDevice temp;
						DWORD DeviceId{ InvaildVendorId };
						temp.pciAddress = BuildPCIAddress(bus, dev, func);
						temp.VendorId = VendorId;
						if (PciDeal.ReadWORD(bus, dev, func, 2, DeviceId))
						{
							temp.DeviceId = DeviceId;
						}

						if (VendorId == IntelVendorId)
						{
							temp.BarAddress = FindIntelMCHBAR(bus);
						}
						else
						{
							DWORD BarAddress = 0;
							if (PciDeal.ReadDWORD(bus, dev, func, GetBarAddressOffset(VendorId), BarAddress))
							{
								temp.BarAddress = BarAddress & 0x0fffffff0;
							}
						}

						spdlog::info("Find GPU VID: {:x}, DID: {:x}", temp.VendorId, temp.DeviceId);
						ret.emplace_back(std::move(temp));
					}
				}
			}
		}
	}
	return ret;
}

std::uint64_t Hardware::GPU::PciGpuDetect::FindIntelMCHBAR(const uint32_t& bus)
{
	DWORD VendorId = InvaildVendorId;
	DWORD ClassCode{};
	Utils::Ring0::SafePCIHandle PciDeal;
	Utils::CvtDs MCHBAR;
	for (auto dev = 0; dev < 31; ++dev)
	{
		for (auto func = 0; func < 0x7; ++func)
		{
			if (PciDeal.ReadWORD(bus, dev, func, 0, VendorId) && VendorId == IntelVendorId)
			{
				if (PciDeal.ReadDWORD(bus, dev, func, 8, ClassCode))
				{
					if (Utils::extract_bits_ui(ClassCode, 8, 31) == 0x060000)
					{
						if (PciDeal.ReadDWORD(bus, dev, func, 0x48, MCHBAR.ui32.Eax) && PciDeal.ReadDWORD(bus, dev, func, 0x4C, MCHBAR.ui32.Edx))
						{
							if ((MCHBAR.ui32.Eax & 1) == 1)
							{
								spdlog::info(R"(Intel MCHBAR: {:x})", MCHBAR.ui64);
								return MCHBAR.ui64 & 0xFFFFFFFFFFFFFFFE;
							}
						}
					}
				}
			}
		}
	}
	spdlog::error("Not Find MCHBAR");
	return InvaildMemoryBase;
}