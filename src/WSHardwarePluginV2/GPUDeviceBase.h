#pragma once
#include "MemoryHandle.h"
#include "GPUConfig.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief GPU的PCI物理位置信息
		struct GPUDevice
		{
			uint32_t   pciAddress;///<PCI的地址
			uint64_t   BarAddress;///<内存的地址
			uint16_t   VendorId;///<厂商ID
			uint16_t   DeviceId;///<设备ID
			uint16_t   SubVendorId;///<子厂商ID
			uint16_t   SubDeviceId;///<子设备ID
		};

		struct GPUTimeCircle
		{
			ULONG64 Value;
			ULONG64 Delta;
		};

		struct GPUNode : GPUTimeCircle
		{
			ULONG Index;
			std::string Name;
		};

		/// @brief GPU的适配器
		struct GPUAdapter
		{
			LUID AdapterLuid; ///< LUID
			ULONG SegmentCount; ///< Segment Count
			std::vector<GPUNode> Nodes; ///< GPU的Nodes
			std::wstring DeviceInterface; ///< 设备接口

			std::wstring Descriptor; ///< 描述符
			std::wstring DriverDate;
			std::wstring DriverVersion;
			std::wstring LocationInfo;
			ULONG64 InstalledMemory;
			uint64_t SharedMemory;
			uint64_t DedicatedMemory;
		};

		/// @brief 非法的64位内存地址
		constexpr auto InvaildMemoryBase = 0xFFFFFFFFFFFFFFFFull;

		/// @brief GPU装饰基类，用来阐述当前CPU的特性
		class GPUDecorator
		{
		public:
			GPUDecorator() = delete;
			/// @brief 初始化函数，保存内存地址，当前修饰名
			/// @param[in] MemoryBase 内存地址
			/// @param[in] Name 当前名称
			/// @return
			explicit GPUDecorator(const uint64_t MemoryBase, const std::string& Name);
			/// @brief 析构函数，使用默认选项
			virtual ~GPUDecorator() = default;
			/// @brief 获取装饰名称
			/// @return 装饰名称
			const std::string GetDecoratorName() const;

			/// @brief 获取装饰信息
			/// @return 装饰信息
			virtual const std::string GetDecoratorValue() const = 0;

			/// @brief 更新信息
			/// @param MemoryPtr 内存的指针
			virtual void Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr) = 0;
		protected:
			const uint64_t MemoryBase;
			const std::string m_Name;
			mutable bool IsUpdate;
		};

		/// @brief GPU通用数据
		class GPUDeviceBase
		{
		public:
			/// @brief 构造函数
			/// @param[in] GpuData GPU的总线数据
			explicit GPUDeviceBase(const GPUDevice& GpuData, std::unique_ptr<GPUAdapter>&& Adapter);

			/// @brief 虚析构函数
			virtual ~GPUDeviceBase() = default;

			/// @brief 更新GPU的信息
			/// @return Utf8 Json字符串
			virtual std::string UpdateGPUInfo() = 0;

			/// @brief 构建GPU的信息
			/// @return Utf8 Json字符串
			virtual std::string GetGPUInfo() = 0;
		protected:
			/// @brief 更新GPU的Node信息
			/// @param Node @ref GPUNode
			/// @return 更新是否成功
			bool UpdateNode(GPUNode& Node);
		protected:
			/// @brief GPU的PCI总线数据
			GPUDevice GPUBaseData;
			/// @brief Sub Vendor
			std::string SubVendor;
			/// @brief GPU的适配器接口
			std::unique_ptr<GPUAdapter> m_Adapter;
			/// @brief GPU的运行时间
			GPUTimeCircle TotalRunningTime;
		};
	}
}
