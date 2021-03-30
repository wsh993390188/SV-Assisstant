#pragma once
#include "SIOBase.h"
namespace Hardware
{
	namespace SIO
	{
		/// @brief ITE SIO̽����
		class ITE : public SIOBase
		{
		public:
			/// @brief ��ʼ��ITE SIO
			ITE();

			/// @brief ��ʼ��SIO
			/// @param port SIO�ĵ�ַ
			/// @return �Ƿ�ɹ�
			bool InitializeSIO(const USHORT& port) override;

			/// @brief ����ʼ��SIO�������Ҫ���٣�����һֱռ��
			/// @param port SIO�ĵ�ַ
			/// @return �Ƿ�ɹ�
			bool DeInitializeSIO(const USHORT& port) override;
		private:
			/// @brief ITE Enrironment register
			const UCHAR IT87_ENVIRONMENT_CONTROLLER;
			/// @brief ITE Chip version register
			const UCHAR IT87_CHIP_VERSION_REGISTER;
		};
	}
}