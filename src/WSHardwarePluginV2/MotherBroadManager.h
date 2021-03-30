#pragma once
#include "Interface.h"
#include "GenericMotherBroad.h"
#include <string>

namespace Hardware
{
	/// @brief ������Ƚӿ���
	class MotherBroadManager : public Interface::Manager
	{
	public:
		/// @brief ������Ƚӿڹ��캯��
		MotherBroadManager();
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
		/// @brief ������ط��ʿ�����
		std::unique_ptr<MotherBroad::GenericMotherBroad> MotherBroadControl;
	private:
	};
}
