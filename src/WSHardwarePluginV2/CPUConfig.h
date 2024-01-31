#pragma once
#include <mutex>
#include "XMLConfig.h"
namespace Hardware
{
	namespace CPU
	{
		/// @brief 从配置中获取CPU信息
		struct CPUExtendedInfoFromCPUDB
		{
			XMLConfig::ExtendedInfo CodeName; ///< cpu代号
			XMLConfig::ExtendedInfo SocketPackage; ///< cpu封装leixing
			XMLConfig::ExtendedInfo MaxTDP;///<cpu的标准最大的TDP
			XMLConfig::ExtendedInfo Technology;///<cpu的工艺
			explicit operator bool() const;
		};

		/// @brief CPU配置文件序列化类
		class CPUDB final : public XMLConfig
		{
		private:
			/// @brief cpudb中的信息序列化
			struct CpuInformationFromCpudb
			{
				/// @brief 从CPUDB中获取的cpu信息, First是CPU系列 Second是CPU扩展信息
				std::map<std::string, CPUExtendedInfoFromCPUDB> Element;
				explicit operator bool() const;
			};

			/// @brief cpudb中的信息序列化
			struct CpuFamily
			{
				/// @brief 从CPUDB中获取的cpu信息, First是CPU的家族 Second是CPU扩展信息
				std::map<std::string, CpuInformationFromCpudb> Element;
				explicit operator bool() const;
			};
			/// @brief 存储外部信息的类
			/// First是CPU的厂商，Second是CPU的家族信息
			std::map<std::string, CpuFamily> m_CPUDB;

			/// @brief XML默认版本号
			std::string m_DefaultVersion;

			/// @brief 配置文件更新的锁
			mutable std::mutex m_Mutex;
		public:
			/// @brief CPU序列化单例
			/// @return 单例引用 @ref CPUDB
			static CPUDB& Instance();

			/// @brief 初始化CPU配置文件
			void Initialize();

			/// @brief 根据CPU的特征寻找部分信息
			/// @param CpuManufacture CPU的厂商
			/// @param CpuFamily CPU的家族
			/// @param CpuModel CPU的型号
			/// @return Cpu额外信息的指针
			std::unique_ptr<CPUExtendedInfoFromCPUDB> FindElements(const QueryInfo& QueryInfo) const;

		private:
			/// @brief CPU序列化类的类型
			using CPUDBType = decltype(m_CPUDB);

			/// @brief 默认构造，禁止外部访问构造函数
			CPUDB() = default;

			/// @brief 解析Manufacture字段
			/// @param ManufactureElement 字段元素
			/// @param CpuDB 序列化类
			void ParserManufacture(tinyxml2::XMLElement const* const ManufactureElement, CPUDBType& CpuDB);

			/// @brief 解析Family字段
			/// @param FamilyElement 字段元素
			/// @param FamilyData 序列化类
			/// @return 错误类型 @ref Data::ErrorType
			Data::ErrorType ParserCpuFamily(tinyxml2::XMLElement const* const FamilyElement, CpuFamily& FamilyData);

			/// @brief 解析CPU字段
			/// @param CpuElement 字段元素
			/// @param CpuData 序列化类
			/// @return 错误类型 @ref Data::ErrorType
			Data::ErrorType ParserCpuInformaion(tinyxml2::XMLElement const* const CpuElement, CpuInformationFromCpudb& CpuData);
		};
	}
}
