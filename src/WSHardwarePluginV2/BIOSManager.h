#pragma once
#include "Interface.h"
#include "GenericBios.h"
#include <string>

namespace Hardware
{
	/// @brief BIOS调度接口类
	class BIOSManager : public Interface::Manager
	{
	public:
		/// @brief BIOS调度接口构造函数
		BIOSManager();
		/// @brief 初始化命令操作
		/// @param[in] paramter JSON参数
		/// @param[out] response JSON回应
		/// @return @ref Data::ErrorType
		Data::ErrorType Init(LPCSTR paramter, std::string& response) override final;

		/// @brief 更新数据命令操作
		/// @param[in] paramter JSON参数
		/// @param[out] response JSON回应
		/// @return @ref Data::ErrorType
		Data::ErrorType Update(LPCSTR paramter, std::string& response) override final;

		/// @brief 获取命令操作
		/// @param[in] paramter JSON参数
		/// @param[out] response JSON回应
		/// @return @ref Data::ErrorType
		Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override final;
	private:
		/// @brief 主板相关访问控制器
		std::unique_ptr<BIOS::GenericBios> BiosControl;
	};
}
