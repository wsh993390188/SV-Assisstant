#pragma once
#include "GenericMemory.h"
#include "BaseSMBUS.h"

namespace Hardware
{
	namespace Memory
	{
		class SPDMemoryController : public GenericMemory
		{
		public:
			/// @brief ��ʼ���ڴ�����
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize() override final;

			/// @brief �����ڴ�����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update() override final;

			/// @brief ���л�SPD������
			/// @return @ref Data::ErrorType Ĭ�ϲ�֧��
			Data::ErrorType SerializationSPD() override final;
		private:
			/// @brief ��ȡSPD����Ϣ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetSPDInformation();

			/// @brief SMBUS��ȡ������
			std::unique_ptr<SMBUSControllerBase> SMBUSController;
		};
	}
}
