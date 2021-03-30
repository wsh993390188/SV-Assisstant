#pragma once
#include "GenericMotherBroad.h"
namespace Hardware
{
	namespace MotherBroad
	{
		class MotherBroadFromSMBIOS : public GenericMotherBroad
		{
		public:
			/// @brief ��ʼ�����弰BIOS��Ϣ
			/// @param[out] response ��Ӧ��Json����
			/// Ŀǰ��˵�޷�������
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief ����������Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		};
	}
}
