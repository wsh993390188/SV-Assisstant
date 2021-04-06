#pragma once
#include "MemoryHandle.h"
#include "GPUConfig.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief GPU��PCI����λ����Ϣ
		struct GPUDevice
		{
			uint32_t   pciAddress;///<PCI�ĵ�ַ
			uint64_t   BarAddress;///<�ڴ�ĵ�ַ
			uint16_t   VendorId;///<����ID
			uint16_t   DeviceId;///<�豸ID
		};

		/// @brief �Ƿ���64λ�ڴ��ַ
		constexpr auto InvaildMemoryBase = 0xFFFFFFFFFFFFFFFFull;

		/// @brief GPUװ�λ��࣬����������ǰCPU������
		class GPUDecorator
		{
		public:
			GPUDecorator() = delete;
			/// @brief ��ʼ�������������ڴ��ַ����ǰ������
			/// @param[in] MemoryBase �ڴ��ַ
			/// @param[in] Name ��ǰ����
			/// @return
			explicit GPUDecorator(const uint64_t MemoryBase, const std::string& Name);
			/// @brief ����������ʹ��Ĭ��ѡ��
			virtual ~GPUDecorator() = default;
			/// @brief ��ȡװ������
			/// @return װ������
			const std::string GetDecoratorName() const;

			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			virtual const std::string GetDecoratorValue() const = 0;

			/// @brief ������Ϣ
			/// @param MemoryPtr �ڴ��ָ��
			virtual void Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr) = 0;
		protected:
			const uint64_t MemoryBase;
			const std::string m_Name;
			mutable bool IsUpdate;
		};

		/// @brief GPUͨ������
		class GPUDeviceBase
		{
		public:
			/// @brief ���캯��
			/// @param[in] GpuData GPU����������
			explicit GPUDeviceBase(const GPUDevice& GpuData) : GPUBaseData(GpuData) {}

			/// @brief ����������
			virtual ~GPUDeviceBase() = default;

			/// @brief ����GPU����Ϣ
			/// @return Utf8 Json�ַ���
			virtual std::string UpdateGPUInfo() = 0;

			/// @brief ����GPU����Ϣ
			/// @return Utf8 Json�ַ���
			virtual std::string GetGPUInfo() = 0;
		protected:
			/// @brief GPU��PCI��������
			GPUDevice GPUBaseData;
		};
	}
}
