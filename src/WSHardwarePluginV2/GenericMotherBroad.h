#pragma once
namespace Hardware
{
	namespace MotherBroad
	{
		/// @brief 主板通用数据，来自CPUZ
		struct MotherBroadCommonStruct
		{
			std::string Manufacturer;
			std::string Model;
			std::string SerialNumber;
		};

		/// @brief 主板通用初始化函数
		class GenericMotherBroad
		{
		public:
			/// @brief 构造函数，用于初始化主板信息
			GenericMotherBroad() = default;

			/// @brief 虚析构函数
			virtual ~GenericMotherBroad() = default;

			/// @brief 初始化主板
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief 更新主板信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief 获取主板基础数据操作
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief 主板信息
			MotherBroadCommonStruct BroadInfo;
		protected:
			/// @brief 根据现有数据构建主板的Json信息
			/// @return 含有Broad数据的Json对象
			virtual Json::Value BuildBroadToJson();

			/// @brief 根据初始化够结构构建JSON字符串
			/// @return Utf-8格式的Json字符串
			std::string BuildInitJson();
		};
	}
}
