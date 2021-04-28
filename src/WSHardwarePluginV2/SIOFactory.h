#pragma once
#include "SIOBase.h"
namespace Hardware
{
	/// @brief SuperIOģ��
	namespace SIO
	{
		/// @brief SuperIO��������
		class SIOFactory final
		{
			/// @brief ���캯��
			SIOFactory();
			SIOFactory(const SIOFactory&) = delete;
			SIOFactory& operator=(const SIOFactory&) = delete;
		public:
			/// @brief ��������ں���
			/// @return
			static SIOFactory& Instance();

			/// @brief ̽��SIO����������
			/// @return �Ƿ����SIO
			bool DetectSIO();
		private:
			/// @brief SIO�Ľ���˿�
			std::array<std::pair<USHORT, USHORT>, 2> SIOPort;
			std::unique_ptr<SIOBase> SuperIO;
		};
	}
}
