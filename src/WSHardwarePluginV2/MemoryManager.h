#pragma once
#include "Interface.h"
#include "GenericMemory.h"
#include <string>
namespace Hardware
{
	/// @brief 内存调度接口类
	class MemoryManager : public Interface::Manager
	{
	public:
		/// @brief 内存调度接口构造函数
		MemoryManager();
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
		/// @brief 内存信息实现器
		std::unique_ptr<Memory::GenericMemory> MemoryControl;
	private:
	};
}
