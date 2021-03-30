#pragma once
#include "GPUDeviceBase.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief NvidiaGPU��Ϣ��ȡ
		class NvidiaGpu : public GPUDeviceBase
		{
		public:
			/// @brief ���캯������NVAPI
			/// @param GpuData GPU�Ļ�������
			NvidiaGpu(const GPUDevice& GpuData);

			/// @brief ��������ж��NVAPI
			~NvidiaGpu();

			/// @brief ����&����GPU����Ϣ
			/// @return Utf8 Json�ַ���
			std::string UpdateGPUInfo() override final;

			/// @brief ����GPU����Ϣ
			/// @return Utf8 Json�ַ���
			std::string GetGPUInfo() override final;
		};
	}
}
