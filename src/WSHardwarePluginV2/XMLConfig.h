#pragma once
namespace Hardware
{
	class XMLConfig
	{
	public:
		/// @brief 配置的扩展信息
		struct ExtendedInfo
		{
			std::string Element;///<信息内容
			bool Priority = true;///< 是否优先使用,不优先则使用代码判别的内容,默认优先使用
			operator bool() const;
		};

		/// @brief 从数据库中查询信息
		struct QueryInfo
		{
			std::string Manufacture; ///<厂商
			std::string Family; ///<家族
			std::string Model; ///<类型
			explicit operator bool() const;
		};

	protected:
		/// @brief 从资源中读取XML配置文件中的信息
		/// @param configcontent XML配置文件中二进制数据
		/// @param ResID 资源ID
		/// @return 读取配置文件是否成功
		bool GetConfigXmlFromResource(std::string& configcontent, const size_t ResID);
	};
}
