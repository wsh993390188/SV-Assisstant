#pragma once
#include "Interface.h"
#include "GenericMonitor.h"
#include <string>
namespace Hardware
{
	/// @brief ��ʾ�����Ƚӿ���
	class MonitorManager : public Interface::Manager
	{
	public:
		/// @brief ��ʾ�����Ƚӿڹ��캯��
		MonitorManager();
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
		std::unique_ptr<Monitor::GenericMonitor> MonitorController;
	};
}
