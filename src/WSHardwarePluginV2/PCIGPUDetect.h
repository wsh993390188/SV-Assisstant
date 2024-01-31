#pragma once
#include "GPUDeviceBase.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief 通过PCIE总线探测GPU设备
		class PciGpuDetect
		{
		public:
			/// @brief 查找在PCI总线上的所有GPU设备
			/// @return PCI的BDF值数组
			bool FindGPUDeviceOnPCI();

			/// @brief 查找PCI上的GPU设备
			/// @param VendorId VendorID
			/// @param DeviceId DeviceID
			/// @param count 出现的次数
			/// @param Device 查找到的设备
			/// @return 查找是否成功
			bool FindDevice(const uint32_t VendorId, const uint32_t DeviceId, const uint8_t count, GPUDevice& Device);
		private:
			/// @brief 寻找Intel MCHBAR
			/// @param bus Pci的bus号
			/// @return MCHBAR的地址
			std::uint64_t FindIntelMCHBAR(const uint32_t& bus);

			/// @brief PCI上的GPU设备
			std::vector<GPUDevice> PciDevice;
		};
	}
}
