#pragma once
#include "GenericMotherBroad.h"
#include "SIOBase.h"
namespace Hardware
{
	namespace MotherBroad
	{
		class MotherBroadFromSMBIOS final : public GenericMotherBroad
		{
		public:
			/// @brief ����Ĺ��캯��
			MotherBroadFromSMBIOS();
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
		private:
			std::unique_ptr<SIO::SIOBase> SuperIO;
		};
	}
}
