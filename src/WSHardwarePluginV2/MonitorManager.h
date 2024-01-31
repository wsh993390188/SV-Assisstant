#pragma once
#include "Interface.h"
#include "GenericMonitor.h"
#include <string>
namespace Hardware
{
	/// @brief 显示器调度接口类
	class MonitorManager : public Interface::Manager
	{
	public:
		/// @brief 显示器调度接口构造函数
		MonitorManager();
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
		std::unique_ptr<Monitor::GenericMonitor> MonitorController;
	};
}
