#pragma once
#include "EDIDdefination.h"
namespace Hardware
{
	namespace Monitor
	{
		/// @brief 显示器的信息数据结构
		using MonitorDataStruct = std::vector<std::pair<std::string, std::vector<std::string>>>;

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
			/// @param[in] EDIDRawBuffer EDID的二进制信息
			/// @return 是否成功
			bool ParserEDID(const std::vector<uint8_t>& EDIDRawBuffer, MonitorDataStruct& MonitorInfo);

			/// @brief 解析EDID制造商
			/// @param EDIDbuffer
			/// @return EDID厂商
			const std::string GetMonitorName(const std::uint16_t& EDIDbuffer);

		private:
			/// @brief 增加描述中的信息
			/// @param Descriptor 描述信息
			/// @param Name 名称
			/// @param MonitorInfo 显示器信息
			/// @return 是否增加成功
			bool AddDescriptorString(MonitorDataStruct& MonitorInfo, const EDID_Descriptor_Common_String& Descriptor, const std::string& Name);

			/// @brief
			/// @param MonitorInfo
			/// @param Descriptor
			/// @return
			bool AddDisplayRangeLimits(MonitorDataStruct& MonitorInfo, const EDID_Display_Range_Limits_Descriptor& Descriptor);

			/// @brief
			/// @param MonitorInfo
			/// @param EstablishedTiming
			/// @return
			bool AddEstablishedTiming(MonitorDataStruct& MonitorInfo, const EDIDCommon::EstablishedTimingSection& EstablishedTiming);

			/// @brief 增加标准时序（标准节）
			/// @param MonitorInfo 存储的信息
			/// @param StandardTiming 时序信息
			/// @return 是否增加成功
			bool AddStandardTiming(MonitorDataStruct& MonitorInfo, const EDIDCommon::StandardTimingSection& StandardTiming);

			/// @brief 增加标准时序（描述符）
			/// @param MonitorInfo 存储的信息
			/// @param StandardTiming 时序信息
			/// @return 是否增加成功
			bool AddStandardTiming(MonitorDataStruct& MonitorInfo, const EDID_StandardTimingIdentifierDefinition& StandardTiming);

			/// @brief 增加标准时序的实现
			/// @param Timing 存储的容器
			/// @param value 时序的存储代码
			void AddStandardTimingImpl(std::vector<std::string>& Timing, const std::uint16_t value);

			/// @brief 增加显示器的细节信息
			/// @param MonitorInfo 存储的信息
			/// @param DetailedTiming 显示器细节
			/// @return 是否已经获取到了显示器的大小
			bool AddPreferredDetailedTiming(MonitorDataStruct& MonitorInfo, const EDID_Detailed_Timing_Descriptor& DetailedTiming);
		};
	}
}
