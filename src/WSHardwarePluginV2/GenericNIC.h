#pragma once
namespace Hardware
{
	namespace NIC
	{
		/// @brief IP地址的结构
		struct IPAddress
		{
			/// @brief IP地址类型
			enum class IPAddressType : uint16_t
			{
				UNKNOWN = 0xFFFF,///< 未知类型
				IPV4 = 2,///< IPV4类型
				IPV6 = 23,///< IPV6类型
			};

			IPAddressType Type; ///<IP地址的类型
			std::string Address;///<IP地址
		};

		/// @brief 网卡信息
		struct NetStruct
		{
			uint32_t	If_Index;///<接口的索引(由系统分配)
			bool		IsWifi;///< 是否为Wifi设备
			GUID		IfGuid;///<适配器的GUID
			std::string Description;///<网卡描述
			std::string ConnectionName;///<连接名称
			std::string InterfaceType;///<接口类型
			std::string MacAddress;///<硬件地址
			uint64_t ConnectionSpeed;///<连接速度
			uint32_t MTU;///<MTU
			std::vector<IPAddress> UniAddress;///< 单播地址

			// 动态信息
			uint64_t InOctets;///<接收到的Packages数量
			uint64_t OutOctets;///<发送的packages数量
		};

		/// @brief 网卡通用初始化函数
		class GenericNIC
		{
		public:
			/// @brief 构造函数，用于初始化网卡信息
			GenericNIC() = default;

			/// @brief 虚析构函数
			virtual ~GenericNIC() = default;

			/// @brief 初始化网卡
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief 更新网卡信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief 获取网卡基础数据操作
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief 网卡信息存储数据
			std::map<uint32_t, NetStruct> NICInfos;
		protected:
			/// @brief 构建初始化JSON数据
			/// @return JSON Utf-8字符
			std::string BuildInitializeJson();

			/// @brief 构建网卡信息的JSON字符
			/// @param[in] NICId 网卡ID
			/// @return JSON字符
			std::string BuildElementJson(const uint32_t& NICId);

			/// @brief 构建网卡的动态信息
			/// @param Info 网卡的动态信息
			/// @return JSON字符
			std::string BuildElementUpdateJson(const NetStruct& Info);

			/// @brief 解析Json字符
			/// @param[in] JsonString JSON字符串
			/// @param[out] NICId 解析成功后的网卡Id
			/// @return 解析是否成功
			bool ParserJson(const std::string& JsonString, uint32_t& NICId);
		};
	}
}
