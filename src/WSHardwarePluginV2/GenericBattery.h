#pragma once
namespace Hardware
{
	namespace Battery
	{
		/// @brief 电池通用初始化函数
		class GenericBattery
		{
		public:
			/// @brief 构造函数，用于初始化电池信息
			GenericBattery() = default;

			/// @brief 虚析构函数
			virtual ~GenericBattery() = default;

			/// @brief 初始化电池
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief 更新电池信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief 获取电池基础操作
			/// Json格式 {"BatteryId": 0}
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType GetElements(LPCSTR paramter, std::string& response) = 0;
		};
	}
}
