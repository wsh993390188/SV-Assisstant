#pragma once
#include <mutex>
#include "XMLConfig.h"
namespace Hardware
{
	namespace Memory
	{
		/// @brief 内存厂商的配置表
		class JEDECConfig final : public XMLConfig
		{
			/// @brief 内存厂商的Banks
			std::vector<std::map<size_t, std::string>> m_Banks;

			/// @brief Bank的管理组织
			using BanksType = decltype(m_Banks);

			/// @brief Bank的内容
			using BankType = BanksType::value_type;

			/// @brief XML默认版本号
			std::string m_DefaultVersion;
			/// @brief 配置文件更新的锁
			mutable std::mutex m_Mutex;
		public:
			/// @brief 内存厂商的配置表序列化单例
			/// @return 单例引用 @ref JEDECConfig
			static JEDECConfig& Instance();

			/// @brief 从Bank中获取厂商信息
			/// @param BankCode Bank Code
			/// @param ContinuationCode Continuation Code
			/// @return 厂商的U8名
			std::string GetManufacture(const size_t BankCode, const size_t ContinuationCode) const;
		private:
			/// @brief 默认构造，禁止外部访问构造函数
			JEDECConfig();

			/// @brief 初始化内存的配置文件
			void Initialize();

			/// @brief 解析Bank字段
			/// @param BankElement 字段元素
			/// @param BanksDB 序列化类
			void ParserBank(tinyxml2::XMLElement const* const BankElement, BanksType& BanksDB);

			/// @brief 解析Manufacture字段
			/// @param ManufactureElement 字段元素
			/// @param BankDB 序列化类
			void ParserManufacture(tinyxml2::XMLElement const* const ManufactureElement, BankType& BankDB);
		};
	}
}
