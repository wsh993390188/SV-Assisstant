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
		/// @brief 获取IntelGPU的温度
		class IntelGPUTemperature : public GPUDecorator
		{
		public:
			/// @brief 禁用默认构造函数
			IntelGPUTemperature() = delete;
			/// @brief 初始化函数，保存内存地址，当前修饰名
			/// @param[in] MemoryBase 内存地址
			/// @param[in] Name 当前名称
			/// @return
			explicit IntelGPUTemperature(const uint64_t& MemoryBase, const std::string& Name);

			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override final;

			/// @brief 更新信息
			/// @param MemoryPtr 内存的指针
			void Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr) override final;
		private:
			/// @brief 最新温度
			int64_t m_TjCurrent;
		};

		/// @brief 获取IntelGPU的Engine Clock
		class IntelGPUEngineClock : public GPUDecorator
		{
		public:
			/// @brief 禁用默认构造函数
			IntelGPUEngineClock() = delete;
			/// @brief 初始化函数，保存内存地址，当前修饰名
			/// @param[in] MemoryBase 内存地址
			/// @param[in] Name 当前名称
			/// @return
			explicit IntelGPUEngineClock(const uint64_t& MemoryBase, const std::string& Name);

			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override final;

			/// @brief 更新信息
			/// @param MemoryPtr 内存的指针
			void Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr) override final;
		private:
			/// @brief 最新的EngineClock
			int64_t m_EngineClock;
		};

		/// @brief 获取IntelGPU的Memory Clock
		class IntelGPUMemoryClock : public GPUDecorator
		{
		public:
			/// @brief 禁用默认构造函数
			IntelGPUMemoryClock() = delete;
			/// @brief 初始化函数，保存内存地址，当前修饰名
			/// @param[in] MemoryBase 内存地址
			/// @param[in] Name 当前名称
			/// @return
			explicit IntelGPUMemoryClock(const uint64_t& MemoryBase, const std::string& Name);

			/// @brief 获取装饰信息
			/// @return 装饰信息
			const std::string GetDecoratorValue() const override final;

			/// @brief 更新信息
			/// @param MemoryPtr 内存的指针
			void Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr) override final;
		private:
			/// @brief 最新的MemoryClock
			double m_MemoryClock;
		};

		/// @brief Intel GPU的数据结构
		struct IntelGpuData
		{
			uint32_t GPUMaxFreq;///<GPU的最大频率
			uint32_t GPUMinFreq;///<GPU的最小频率
		};

		/// @brief Intel GPU操作对象
		class IntelGPU :public GPUDeviceBase
		{
		public:
			/// @brief 构造函数
			/// @param GpuData GPU的PCI物理数据信息
			IntelGPU(const GPUDevice& GpuData);

			/// @brief 构建&更新GPU的信息
			/// @return Utf8 Json字符串
			std::string UpdateGPUInfo() override final;

			/// @brief 构建GPU的信息
			/// @return Utf8 Json字符串
			std::string GetGPUInfo() override final;
		private:
			/// @brief CPU 动态信息
			std::vector<std::shared_ptr<GPUDecorator>> m_Decorators;

			/// @brief GPU的数据，Intel专用
			IntelGpuData GpuDatas;
		};
	}
}
