/*!
* @file DiskManager.h
* @brief Ӳ�̹�����
*
* @author ��˶(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020��6��3��
*/
#pragma once
#include "Interface.h"
#include "CrystalDiskInfo\AtaSmart.h"

namespace Hardware
{
	/// @brief Ӳ�̹�������
	class DiskManager final :public Interface::Manager
	{
	public:
		/// @brief ��ʼ��Ӳ�̹�����
		/// @return ��
		DiskManager() = default;

	private:
		Data::ErrorType Init(LPCSTR paramter, std::string& response) override;

		Data::ErrorType Update(LPCSTR paramter, std::string& response) override;

		Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override;
	private:
		/// @brief ������������
		/// @param[in] paramter ����JSON����
		/// @param[out] CurrentId ������µ�Id
		/// @param[out] Elements ��Ҫ���µ�Ԫ��
		/// @return @ref Data::ErrorType
		Data::ErrorType PaserUpdateJson(LPCSTR paramter, int& CurrentId, std::vector<std::string>& Elements);

		/// @brief ��������Json����
		/// @param[out] response JSON��������
		/// @param[in] CurrentId ���µ�Id
		/// @param[in] Elements ���µ�Ԫ��
		/// @return @ref Data::ErrorType
		Data::ErrorType BuildJson(std::string& response, const int CurrentId, const std::vector<std::string>& Elements);

		Data::ErrorType BuildInitJson(std::string& response);

		void BuildStaticJson(Json::Value& Element, const CAtaSmart::ATA_SMART_INFO& var);
		void BuildDynamicJson(Json::Value& Element, const CAtaSmart::ATA_SMART_INFO& var);

	private:
		/// @brief CrystalDisk��Ӳ�����ݽṹ
		CAtaSmart m_Ata;

		/// @brief Ӳ�̴�СתString��ʽ
		/// @param DiskSize
		/// @return Ӳ�̴�С
		std::string DiskSizeToString(const uint64_t& DiskSize);
	};
}