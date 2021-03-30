/*!
* @file DiskManager.h
* @brief 硬盘管理者
*
* @author 王硕(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020年6月3日
*/
#pragma once
#include "Interface.h"
#include "CrystalDiskInfo\AtaSmart.h"

namespace Hardware
{
	/// @brief 硬盘管理者类
	class DiskManager final :public Interface::Manager
	{
	public:
		/// @brief 初始化硬盘管理者
		/// @return 空
		DiskManager() = default;

	private:
		Data::ErrorType Init(LPCSTR paramter, std::string& response) override;

		Data::ErrorType Update(LPCSTR paramter, std::string& response) override;

		Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override;
	private:
		/// @brief 解析更新数据
		/// @param[in] paramter 输入JSON数据
		/// @param[out] CurrentId 输出更新的Id
		/// @param[out] Elements 需要更新的元素
		/// @return @ref Data::ErrorType
		Data::ErrorType PaserUpdateJson(LPCSTR paramter, int& CurrentId, std::vector<std::string>& Elements);

		/// @brief 构建传输Json数据
		/// @param[out] response JSON传输数据
		/// @param[in] CurrentId 更新的Id
		/// @param[in] Elements 更新的元素
		/// @return @ref Data::ErrorType
		Data::ErrorType BuildJson(std::string& response, const int CurrentId, const std::vector<std::string>& Elements);

		Data::ErrorType BuildInitJson(std::string& response);

		void BuildStaticJson(Json::Value& Element, const CAtaSmart::ATA_SMART_INFO& var);
		void BuildDynamicJson(Json::Value& Element, const CAtaSmart::ATA_SMART_INFO& var);

	private:
		/// @brief CrystalDisk的硬盘数据结构
		CAtaSmart m_Ata;

		/// @brief 硬盘大小转String形式
		/// @param DiskSize
		/// @return 硬盘大小
		std::string DiskSizeToString(const uint64_t& DiskSize);
	};
}