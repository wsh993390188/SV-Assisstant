#pragma once
#include "GenericMonitor.h"
namespace Hardware
{
	namespace Monitor
	{
		/// @brief 从注册表中读取EDID
		class EDIDFromRegistry : public GenericMonitor
		{
		public:
			/// @brief 构造函数
			EDIDFromRegistry();

			/// @brief 析构函数
			~EDIDFromRegistry() = default;

			/// @brief 初始化显示器
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response);

			/// @brief 更新显示器信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response);
		private:
			/// @brief 获取当前显示器的信息
			/// @param Monitor 输出显示器在电脑端对应的Device等信息
			/// @return 是否成功
			bool GetCurrentMonitor(std::vector<DISPLAY_DEVICE>& Monitor);

			/// @brief 获取显示器的Model和Driver信息
			/// @param[in] lpDeviceid 显示器的DeviceID
			/// @param[out] Model 显示器的Model
			/// @param[out] Driver 显示器的Driver
			/// @return 是否成功
			bool GetModelandDriver(
				const std::wstring& lpDeviceid,
				std::wstring& Model,
				std::wstring& Driver
			);

			/// @brief 根据Model和Driver定位注册表 获取当前显示器的EDID
			/// @param[in] Model 显示器的Model
			/// @param[in] Driver 显示器的Driver
			/// @param[out] EDIDbuffer 输出缓冲区地址
			/// @return 是否成功
			bool GetEDID(
				const std::wstring& Model,
				const std::wstring& Driver,
				std::vector<uint8_t>& EDIDbuffer);
		private:
			/// @brief 显示器实际数据
			std::vector<DISPLAY_DEVICE> Monitor;
		};
	}
}