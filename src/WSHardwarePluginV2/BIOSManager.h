#pragma once
#include "Interface.h"
#include "GenericBios.h"
#include <string>

namespace Hardware
{
	/// @brief BIOS���Ƚӿ���
	class BIOSManager : public Interface::Manager
	{
	public:
		/// @brief BIOS���Ƚӿڹ��캯��
		BIOSManager();
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
		std::unique_ptr<BIOS::GenericBios> BiosControl;
	};
}
