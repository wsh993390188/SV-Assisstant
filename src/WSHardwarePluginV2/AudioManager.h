#pragma once
#include "Interface.h"
#include "GenericAudio.h"
#include <string>
namespace Hardware
{
	/// @brief �������Ƚӿ���
	class AudioManager : public Interface::Manager
	{
	public:
		/// @brief �������Ƚӿڹ��캯��
		AudioManager();
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
		std::unique_ptr<Audio::GenericAudio> AudioController;
	};
}
