#pragma once
#include "GenericBattery.h"
#include "BatteryElement.h"
namespace Hardware
{
	namespace Battery
	{
		/// @brief 通过部分驱动获取电池信息
		class BatteryFromSys final :public GenericBattery
		{
		public:
			/// @brief 构造函数
			BatteryFromSys();

			/// @brief 初始化电池
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief 更新电池信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;

			/// @brief 获取电池基础操作
			/// Json格式 {"BatteryId": 0}
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override final;
		private:
			/// @brief 获取全部电池设备的路径
			/// @param[out] DevicePath 电池设备对象
			/// @return 是否成功
			bool GetDevicePath();

			/// @brief 构造初始化Json值
			/// @return Json字符
			std::string BuildInitializeJson();

			/// @brief 解析Json字符
			/// @param[in] JsonString JSON字符串
			/// @param[out] BatteryId 解析成功后的电池Id
			/// @return 解析是否成功
			bool ParserJson(const std::string& JsonString, uint32_t& BatteryId);
		protected:
			/// @brief 电池信息实体设备 First对应的Id号
			std::map<uint32_t, std::shared_ptr<BatteryElement>> DevicePath;
		};
	}
}
