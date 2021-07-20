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

bool Hardware::GPU::PciGpuDetect::FindGPUDeviceOnPCI()
{
	DWORD VendorId = InvaildVendorId;
	Utils::Ring0::SafePCIHandle PciDeal;
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

						DWORD SubVendorId{ InvaildVendorId };
						if (PciDeal.ReadWORD(bus, dev, func, 0x2C, SubVendorId))
						{
							temp.SubVendorId = SubVendorId;
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
						this->PciDevice.emplace_back(std::move(temp));
					}
				}
			}
		}
	}
	return !this->PciDevice.empty();
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

bool Hardware::GPU::PciGpuDetect::FindDevice(const uint32_t VendorId, const uint32_t DeviceId, const uint8_t count,
	Hardware::GPU::GPUDevice& Device)
{
	uint8_t times = 0;
	for (const auto& Dev : PciDevice)
	{
		if ((Dev.VendorId == VendorId) && (Dev.DeviceId == DeviceId))
		{
			if (times++ == count)
			{
				Device = Dev;
				return true;
			}
		}
	}
	return false;
}