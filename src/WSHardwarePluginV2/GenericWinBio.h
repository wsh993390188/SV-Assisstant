#pragma once
namespace Hardware
{
	namespace WinBio
	{
		/// @brief Win10的生物传感器通用信息
		struct WinBioCommonInfo
		{
			std::string Type; ///<传感器类型
			std::string Desc; ///<传感器描述
			std::string SerialNumber; ///<传感器序号
			std::string Manufacturer; ///<传感器生产厂商
			std::string Model; ///<传感器型号
			std::string Firmware; ///<传感器固件版本
		};

		/// @brief Win10的生物传感器通用初始化函数
		class GenericWinBio
		{
		public:
			/// @brief 构造函数，用于初始化Win10的生物传感器信息
			GenericWinBio() = default;

			/// @brief 虚析构函数
			virtual ~GenericWinBio() = default;

			/// @brief 初始化Win10的生物传感器
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief 更新Win10的生物传感器信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief 获取Win10的生物传感器基础数据操作
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief 构建初始化JSON数据
			/// @return JSON Utf-8字符
			std::string BuildInitializeJson();

			/// @brief 构建Win10的生物传感器信息的JSON字符
			/// @param[in] WinBioId Win10的生物传感器ID
			/// @return JSON字符
			std::string BuildElementJson(const uint32_t& WinBioId);

			/// @brief 解析Json字符
			/// @param[in] JsonString JSON字符串
			/// @param[out] WinBioId 解析成功后的网卡Id
			/// @return 解析是否成功
			bool ParserJson(const std::string& JsonString, uint32_t& WinBioId);

			/// @brief Win10的生物传感器总的信息
			std::map<uint32_t, WinBioCommonInfo> WinBioInfos;
		};
	}
}
