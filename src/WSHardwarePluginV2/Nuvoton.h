#pragma once
#include "SIOBase.h"
namespace Hardware
{
	namespace SIO
	{
		/// @brief Nuvoton SIO
		class Nuvoton : public SIOBase
		{
		public:
			/// @brief ��ʼ��Novoton
			Nuvoton();

			/// @brief ��ʼ��SIO
			/// @param port SIO�ĵ�ַ
			/// @return �Ƿ�ɹ�
			bool InitializeSIO(const USHORT& port) override;

			/// @brief ����ʼ��SIO�������Ҫ���٣�����һֱռ��
			/// @param port SIO�ĵ�ַ
			/// @return �Ƿ�ɹ�
			bool DeInitializeSIO(const USHORT& port) override;
		private:
			/// @brief ����IO����
			/// @param port SIO�Ľӿڵ�ַ
			/// @param data SIO�����ݵ�ַ
			void NuvotonDisableIOSpaceLock(const USHORT& port, const USHORT& data);
		};
	}
}
