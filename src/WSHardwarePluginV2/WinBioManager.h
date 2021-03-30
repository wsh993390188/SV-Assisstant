#pragma once
#include "Interface.h"
#include "GenericWinBio.h"
#include <string>
namespace Hardware
{
	/// @brief Win10支持的生物传感器调度接口类
	class WinBioManager : public Interface::Manager
	{
	public:
		/// @brief Win10支持的生物传感器调度接口构造函数
		WinBioManager();
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
		/// @brief Win10支持的生物传感器控制器
		std::unique_ptr<WinBio::GenericWinBio> WinBioController;
	};
}
