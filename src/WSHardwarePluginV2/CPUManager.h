/*!
* @file CPUManager.h
* @brief CPU管理者
*
* @author 王硕(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020年6月3日
*/
#pragma once
#include "Interface.h"
#include "GenericCPU.h"

namespace Hardware
{
	/// @brief CPU管理者类
	class CPUManager : public Interface::Manager
	{
	public:
		/// @brief 初始化命令操作
		/// @param[in] paramter JSON参数
		/// @param[out] response JSON回应
		/// @return @ref Data::ErrorType
		Data::ErrorType Init(LPCSTR paramter, std::string& response) override;

		/// @brief 更新数据命令操作
		/// @param[in] paramter JSON参数
		/// @param[out] response JSON回应
		/// @return @ref Data::ErrorType
		Data::ErrorType Update(LPCSTR paramter, std::string& response) override;

		/// @brief 获取命令操作
		/// @param[in] paramter JSON参数
		/// @param[out] response JSON回应
		/// @return @ref Data::ErrorType
		Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override;
	private:
		/// @brief CPU类型
		/// @TODO ARM CPU应如何处理
		enum class CPU_Brands
		{
			/// @brief 未知的CPU
			Unknown,
			/// @brief IntelCPU
			Intel,
			/// @brief AmdCPU
			Amd,
			/// @brief 兆芯CPU
			Zhaoxin
		};
		/// @brief 从CPUID中获取CPU品牌
		/// @return @ref CPU_Brands
		static const CPU_Brands GetCPUBrandsFromCPUID() noexcept;
	private:
		/// @brief CPU数据实际存储位置
		std::unique_ptr<CPU::GenericCPU> m_CPUs;
	};
}
