#pragma once
namespace Hardware
{
	/// @brief BIOS相关信息的空间
	namespace BIOS
	{
		/// @brief BIOS通用数据信息，来自CPUZ
		struct BiosCommonStruct
		{
			std::string Brand;
			std::string Version;
			std::string DataTime;
			bool UEFI; ///< 是否为UEFI启动
			bool SecureBoot; ///<是否为安全启动
		};

		/// @brief BIOS的通用接口类
		class GenericBios final
		{
		public:
			/// @brief 构造函数，用于初始化BIOS的信息
			GenericBios() = default;

			/// @brief 虚析构函数
			~GenericBios() = default;

			/// @brief 初始化BIOS
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response);

			/// @brief 更新BIOS信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response);

			/// @brief 获取BIOS基础数据操作
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief Bios信息，不支持双BIos，未来考虑一下
			BiosCommonStruct BiosInfos;

		private:
			/// @brief 根据现有数据构建Bios的Json信息
			/// @return 含有Bios数据的Json对象
			Json::Value BuildBiosToJson();

			/// @brief 根据初始化够结构构建JSON字符串
			/// @return Utf-8格式的Json字符串
			std::string BuildInitJson();

			/// @brief 判断系统是否为UEFI启动
			/// @return 是否是UEFI启动
			bool IsUEFI() const;

			/// @brief 安全启动是否开启
			/// @return 开启
			bool SecureBootEnabled();
		};
	}
}