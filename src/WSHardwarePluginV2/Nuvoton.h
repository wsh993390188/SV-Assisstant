#pragma once
#include "SIOCommon.h"
namespace Hardware
{
	namespace SIO
	{
		/// @brief Nuvoton SIO
		class Nuvoton : public SIOCommon
		{
		public:
			/// @brief ��ʼ��Novoton
			Nuvoton();

		private:
			/// @brief ����IO����
			/// @param port SIO�Ľӿڵ�ַ
			/// @param data SIO�����ݵ�ַ
			void NuvotonDisableIOSpaceLock(const USHORT& port, const USHORT& data);
		};
	}
}
