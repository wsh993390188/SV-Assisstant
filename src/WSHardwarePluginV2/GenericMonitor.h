#pragma once
#include "EDIDdefination.h"
namespace Hardware
{
	namespace Monitor
	{
		/// @brief 显示器的信息数据结构
		using MonitorDataStruct = std::vector<std::pair<std::string, std::string>>;

		/// @brief 显示器通用初始化函数
		class GenericMonitor
		{
		public:
			/// @brief 构造函数，用于初始化显示器信息
			GenericMonitor() = default;

			/// @brief 虚析构函数
			virtual ~GenericMonitor() = default;

			/// @brief 初始化显示器
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief 更新显示器信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief 获取显示器基础数据操作
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief 显示器信息存储数据
			std::map<std::wstring, MonitorDataStruct> MonitorInfos;
		protected:
			/// @brief 构建初始化JSON数据
			/// @return JSON Utf-8字符
			std::string BuildInitializeJson();

			/// @brief 构建显示器信息的JSON字符
			/// @param[in] MonitorId 显示器ID
			/// @return JSON字符
			std::string BuildElementJson(const std::wstring& MonitorId);

			/// @brief 解析Json字符
			/// @param[in] JsonString JSON字符串
			/// @param[out] MonitorId 解析成功后的显示器Id
			/// @return 解析是否成功
			bool ParserJson(const std::string& JsonString, std::wstring& MonitorId);

			/// @brief 解析EDID
			/// @param[in] EDIDbuffer EDID的信息
			/// @return 是否成功
			bool ParserEDID(const EDID& EDIDbuffer, MonitorDataStruct& MonitorInfo);

			/// @brief 解析EDID制造商
			/// @param EDIDbuffer
			/// @return
			const std::string GetMonitorName(const Hardware::Monitor::EDID& EDIDbuffer);
		};
	}
}
