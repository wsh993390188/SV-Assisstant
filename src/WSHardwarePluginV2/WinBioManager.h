#pragma once
#include "Interface.h"
#include "GenericWinBio.h"
#include <string>
namespace Hardware
{
	/// @brief Win10֧�ֵ����ﴫ�������Ƚӿ���
	class WinBioManager : public Interface::Manager
	{
	public:
		/// @brief Win10֧�ֵ����ﴫ�������Ƚӿڹ��캯��
		WinBioManager();
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
		/// @brief Win10֧�ֵ����ﴫ����������
		std::unique_ptr<WinBio::GenericWinBio> WinBioController;
	};
}
