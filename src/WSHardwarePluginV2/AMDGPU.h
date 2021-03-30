#pragma once
#include "GPUDeviceBase.h"
namespace Hardware
{
	namespace GPU
	{
		class AMDGPU : public GPUDeviceBase
		{
		public:
			AMDGPU(const GPUDevice& GpuData);

			/// @brief ����&����GPU����Ϣ
			/// @return Utf8 Json�ַ���
			std::string UpdateGPUInfo() override final;

			/// @brief ����GPU����Ϣ
			/// @return Utf8 Json�ַ���
			std::string GetGPUInfo() override final;
		};
	}
}