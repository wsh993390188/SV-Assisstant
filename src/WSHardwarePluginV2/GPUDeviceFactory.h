#pragma once
#include "GenericGPU.h"
#include "IntelGPU.h"
#include "AmdGPU.h"
#include "NvidiaGPU.h"

namespace Hardware
{
	namespace GPU
	{
		/// @brief AMD VendorId
		constexpr auto AMDVendorId = 0x1002;
		/// @brief Intel VendorId
		constexpr auto IntelVendorId = 0x8086;
		/// @brief Nvidia VendorId
		constexpr auto NvidiaVendorId = 0x10DE;
		/// @brief VMWare VendorId
		constexpr auto VMWareVendorId = 0x15AD;
		/// @brief 非法的VendorId
		constexpr auto InvaildVendorId = 0xFFFF;

		/// @brief GPU设备工厂类
		class GPUDeviceFactory final
		{
			GPUDeviceFactory() = default;
		public:
			/// @brief 单例
			/// @return 单例对象
			static GPUDeviceFactory& Instance();

			/// @brief 根据VendorId创建显卡设备对象
			/// @param AdapterData Dx信息
			/// @return @ref Hardware::GPU::GPUDeviceBase
			std::unique_ptr<GPUDeviceBase> CreateDevice(const GPU::GPUDevice& AdapterData)
			{
				switch (AdapterData.VendorId)
				{
				case IntelVendorId:
					return std::make_unique<IntelGPU>(AdapterData);
				case NvidiaVendorId:
					return std::make_unique<NvidiaGpu>(AdapterData);
				case AMDVendorId:
					return std::make_unique<AMDGPU>(AdapterData);
				}
				return nullptr;
			}
		};
	}
}
