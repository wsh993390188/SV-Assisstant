#pragma once
#include "GPUDeviceBase.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief NvidiaGPU信息获取
		class NvidiaGpu : public GPUDeviceBase
		{
		public:
			/// @brief 构造函数加载NVAPI
			/// @param GpuData GPU的基础数据
			/// @param Adapter @ref GPUAdapter
			NvidiaGpu(const GPUDevice& GpuData, std::unique_ptr<GPUAdapter>&& Adapter);

			/// @brief 析构函数卸载NVAPI
			~NvidiaGpu();

			/// @brief 构建&更新GPU的信息
			/// @return Utf8 Json字符串
			std::string UpdateGPUInfo() override final;

			/// @brief 构建GPU的信息
			/// @return Utf8 Json字符串
			std::string GetGPUInfo() override final;
		};
	}
}
