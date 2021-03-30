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
		/// @brief �Ƿ���VendorId
		constexpr auto InvaildVendorId = 0xFFFF;

		/// @brief GPU�豸������
		class GPUDeviceFactory final
		{
			GPUDeviceFactory() = default;
		public:
			/// @brief ����
			/// @return ��������
			static GPUDeviceFactory& Instance();

			/// @brief ����VendorId�����Կ��豸����
			/// @param AdapterData Dx��Ϣ
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
