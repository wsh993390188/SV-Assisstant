#pragma once
#include "GenericNIC.h"
#include <NetCon.h>
#include "NICIFEntry.h"

namespace Hardware
{
	namespace NIC
	{
		class NICIphlpapi final :public GenericNIC
		{
		public:
			/// @brief 初始化函数
			NICIphlpapi();
			/// @brief 析构函数
			~NICIphlpapi();
			/// @brief 初始化网卡
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief 更新网卡信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		private:
			/// @brief 获取网卡的设备接口类型
			/// @param[in] state 设备接口值
			/// @return 设备接口字符串
			std::string ConvertDeviceIFToString(const DWORD& state);

			/// @brief 枚举网卡设备
			/// @return @ref Data::ErrorType
			Data::ErrorType EnumCurrentDevice();

			/// @brief 初始化WSA
			/// @return WSA是否成功初始化
			bool InitWSA();
		private:
			/// @brief WSA是否已经初始化
			bool WSAInit;

			/// @brief 查询Interface的信息
			std::unique_ptr<NICIFEnry> IfQuery;
		};
	}
}
