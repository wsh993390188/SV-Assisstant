#pragma once
#include "XMLConfig.h"
namespace Hardware
{
	namespace Utils
	{
		/// @brief 读取PNPId的配置模块
		class PNPIDUtils final : public XMLConfig
		{
			/// @brief 构造函数
			PNPIDUtils();

			/// @brief XML默认版本号
			std::string m_DefaultVersion;

			/// @brief 配置文件更新的锁
			mutable std::mutex m_Mutex;

			/// @brief PNPIDs的数据库
			std::map<std::string, XMLConfig::ExtendedInfo> m_PNPIDs;
		public:
			/// @brief 单例模式
			/// @return @ref PNPIDUtils
			static PNPIDUtils& Instance();

			/// @brief 通过PNPID查询名称
			/// @param PnpId PNPID
			/// @return 名称
			std::string QueryNameByPnpId(const std::string& PnpId);
		private:
			/// @brief PNPID序列化类的类型
			using PNPIDDBType = decltype(m_PNPIDs);

			/// @brief 初始化PNP ID配置文件
			void Initialize();

			/// @brief 解析PNPID字段
			/// @param PNPIDElement 字段元素
			/// @param PnpidDB 序列化类
			void ParserPNPID(tinyxml2::XMLElement const* const PNPIDElement, PNPIDDBType& PnpidDB);
		};
	}
}
