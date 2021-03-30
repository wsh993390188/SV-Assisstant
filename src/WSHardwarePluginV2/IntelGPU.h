#pragma once
#include "GPUDeviceBase.h"
#include <atlcomcli.h>
#include <d3d11.h>
#ifdef _WIN32_WINNT_WIN10
#include <d3d11_3.h>
#endif

namespace Hardware
{
	namespace GPU
	{
		/// @brief ��ȡIntelGPU���¶�
		class IntelGPUTemperature : public GPUDecorator
		{
		public:
			/// @brief ����Ĭ�Ϲ��캯��
			IntelGPUTemperature() = delete;
			/// @brief ��ʼ�������������ڴ��ַ����ǰ������
			/// @param[in] MemoryBase �ڴ��ַ
			/// @param[in] Name ��ǰ����
			/// @return
			explicit IntelGPUTemperature(const uint64_t& MemoryBase, const std::string& Name);

			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override final;

			/// @brief ������Ϣ
			/// @param MemoryPtr �ڴ��ָ��
			void Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr) override final;
		private:
			/// @brief �����¶�
			int64_t m_TjCurrent;
		};

		/// @brief ��ȡIntelGPU��Engine Clock
		class IntelGPUEngineClock : public GPUDecorator
		{
		public:
			/// @brief ����Ĭ�Ϲ��캯��
			IntelGPUEngineClock() = delete;
			/// @brief ��ʼ�������������ڴ��ַ����ǰ������
			/// @param[in] MemoryBase �ڴ��ַ
			/// @param[in] Name ��ǰ����
			/// @return
			explicit IntelGPUEngineClock(const uint64_t& MemoryBase, const std::string& Name);

			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override final;

			/// @brief ������Ϣ
			/// @param MemoryPtr �ڴ��ָ��
			void Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr) override final;
		private:
			/// @brief ���µ�EngineClock
			int64_t m_EngineClock;
		};

		/// @brief ��ȡIntelGPU��Memory Clock
		class IntelGPUMemoryClock : public GPUDecorator
		{
		public:
			/// @brief ����Ĭ�Ϲ��캯��
			IntelGPUMemoryClock() = delete;
			/// @brief ��ʼ�������������ڴ��ַ����ǰ������
			/// @param[in] MemoryBase �ڴ��ַ
			/// @param[in] Name ��ǰ����
			/// @return
			explicit IntelGPUMemoryClock(const uint64_t& MemoryBase, const std::string& Name);

			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			const std::string GetDecoratorValue() const override final;

			/// @brief ������Ϣ
			/// @param MemoryPtr �ڴ��ָ��
			void Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr) override final;
		private:
			/// @brief ���µ�MemoryClock
			double m_MemoryClock;
		};

		/// @brief Intel GPU�����ݽṹ
		struct IntelGpuData
		{
			uint32_t GPUMaxFreq;///<GPU�����Ƶ��
			uint32_t GPUMinFreq;///<GPU����СƵ��
		};

		/// @brief Intel GPU��������
		class IntelGPU :public GPUDeviceBase
		{
		public:
			/// @brief ���캯��
			/// @param GpuData GPU��PCI����������Ϣ
			IntelGPU(const GPUDevice& GpuData);

			/// @brief ����&����GPU����Ϣ
			/// @return Utf8 Json�ַ���
			std::string UpdateGPUInfo() override final;

			/// @brief ����GPU����Ϣ
			/// @return Utf8 Json�ַ���
			std::string GetGPUInfo() override final;
		private:
			/// @brief CPU ��̬��Ϣ
			std::vector<std::shared_ptr<GPUDecorator>> m_Decorators;

			/// @brief GPU�����ݣ�Intelר��
			IntelGpuData GpuDatas;
		};
	}
}
