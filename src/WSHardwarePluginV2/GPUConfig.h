#pragma once
#include "XMLConfig.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief 从配置中获取CPU信息
		struct GPUExtendedInfoFromGPUDB
		{
			XMLConfig::ExtendedInfo ChipName; ///< 芯片名
			XMLConfig::ExtendedInfo Released; ///< Released Date
			XMLConfig::ExtendedInfo MemorySize;///< Memory Size
			XMLConfig::ExtendedInfo MemoryType;///< Memory Type
			XMLConfig::ExtendedInfo MemoryBits;///< Memory Bits
			XMLConfig::ExtendedInfo MemoryClock;///< Memory Clock
			XMLConfig::ExtendedInfo GPUClock;///< GPU Clock
			XMLConfig::ExtendedInfo Shaders;///< Shaders
			XMLConfig::ExtendedInfo TMUs;///< TMUs
			XMLConfig::ExtendedInfo ROPs;///< ROPs
			explicit operator bool() const;
		};

		/// @brief GPU的配置表
		class GPUConfig final : public XMLConfig
		{
		private:
			/// @brief cpudb中的信息序列化
			struct InformationFromDB
			{
				/// @brief 从CPUDB中获取的cpu信息, First是CPU系列 Second是CPU扩展信息
				std::map<std::string, GPUExtendedInfoFromGPUDB> Element;
				explicit operator bool() const;
			};

			/// @brief 存储外部信息的类
			/// First是CPU的厂商，Second是CPU的家族信息
			std::map<std::string, InformationFromDB> m_GPUDB;

			/// @brief XML默认版本号
			std::string m_DefaultVersion;

			/// @brief 配置文件更新的锁
			mutable std::mutex m_Mutex;
		public:
			/// @brief CPU序列化单例
			/// @return 单例引用 @ref CPUDB
			static GPUConfig& Instance();

			/// @brief 初始化CPU配置文件
			void Initialize();

			/// @brief 根据CPU的特征寻找部分信息
			/// @param CpuManufacture CPU的厂商
			/// @param CpuFamily CPU的家族
			/// @param CpuModel CPU的型号
			/// @return Cpu额外信息的指针
			std::unique_ptr<GPUExtendedInfoFromGPUDB> FindElements(const QueryInfo& QueryInfo) const;

		private:
			/// @brief CPU序列化类的类型
			using GPUDBType = decltype(m_GPUDB);

			/// @brief 默认构造，禁止外部访问构造函数
			GPUConfig() = default;

			/// @brief 解析Manufacture字段
			/// @param ManufactureElement 字段元素
			/// @param CpuDB 序列化类
			void ParserManufacture(tinyxml2::XMLElement const* const ManufactureElement, GPUDBType& DB);

			/// @brief 解析CPU字段
			/// @param CpuElement 字段元素
			/// @param CpuData 序列化类
			/// @return 错误类型 @ref Data::ErrorType
			Data::ErrorType ParserInformaion(tinyxml2::XMLElement const* const CpuElement, InformationFromDB& Data);
		};
	}
}
