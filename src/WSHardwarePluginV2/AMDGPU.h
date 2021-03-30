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

			/// @brief 构建&更新GPU的信息
			/// @return Utf8 Json字符串
			std::string UpdateGPUInfo() override final;

			/// @brief 构建GPU的信息
			/// @return Utf8 Json字符串
			std::string GetGPUInfo() override final;
		};
	}
}