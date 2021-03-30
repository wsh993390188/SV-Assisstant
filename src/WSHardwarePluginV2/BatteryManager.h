#pragma once
#include "Interface.h"
#include "GenericBattery.h"
#include <string>
namespace Hardware
{
	/// @brief ��ص��Ƚӿ���
	class BatteryManager : public Interface::Manager
	{
	public:
		/// @brief ��ص��Ƚӿڹ��캯��
		BatteryManager();
		/// @brief ��ʼ���������
		/// @param[in] paramter JSON����
		/// @param[out] response JSON��Ӧ
		/// @return @ref Data::ErrorType
		Data::ErrorType Init(LPCSTR paramter, std::string& response) override final;

		/// @brief ���������������
		/// @param[in] paramter JSON����
		/// @param[out] response JSON��Ӧ
		/// @return @ref Data::ErrorType
		Data::ErrorType Update(LPCSTR paramter, std::string& response) override final;

		/// @brief ��ȡ�������
		/// @param[in] paramter JSON����
		/// @param[out] response JSON��Ӧ
		/// @return @ref Data::ErrorType
		Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override final;
	private:
		/// @brief ��Դ������
		std::unique_ptr<Battery::GenericBattery> BatteryController;
	};
}
