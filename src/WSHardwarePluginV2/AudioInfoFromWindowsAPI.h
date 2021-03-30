#pragma once
#include "GenericAudio.h"
namespace Hardware
{
	namespace Audio
	{
		/// @brief ����Windows Audio Core API��ȡ������Ϣ
		class AudioInfoFromWindowsAPI final : public GenericAudio
		{
		public:
			/// @brief ��ʼ��Audio Core����
			AudioInfoFromWindowsAPI();
			/// @brief ��ʼ�����弰BIOS��Ϣ
			/// @param[out] response ��Ӧ��Json����
			/// Ŀǰ��˵�޷�������
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief ����������Ϣ
			/// @param[in] Args Json���� �ݲ�����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		};
	}
}
