#pragma once
#include <array>
#include "IoHandle.h"
namespace Hardware
{
	/// @brief SuperIOģ��
	namespace SIO
	{
		/// @brief SuperIO�Ļ���
		class SIOBase
		{
		public:
			/// @brief ��ʼ��SIO�Ļ�������
			SIOBase();

			/// @brief ����������
			virtual ~SIOBase() = default;

			/// @brief ��ʼ��SIO
			/// @param port SIO�ĵ�ַ
			/// @return �Ƿ�ɹ�
			virtual bool InitializeSIO(const USHORT& port) = 0;

			/// @brief ����ʼ��SIO�������Ҫ���٣�����һֱռ��
			/// @param port SIO�ĵ�ַ
			/// @return �Ƿ�ɹ�
			virtual bool DeInitializeSIO(const USHORT& port) = 0;

			/// @brief оƬ�Ƿ����
			/// @return �Ƿ����
			bool IsActive()const;
		protected:
			/// @brief �л�SIO��Page
			/// @param port SIO��λ�ýӿ�
			/// @param data SIO�����ݽӿ�
			/// @param logicalDeviceNumber Page��Number
			/// @return �Ƿ�ɹ�
			bool SelectPage(const USHORT& port, const USHORT& data, const UCHAR& logicalDeviceNumber);

			/// @brief SIO��ȡһ��Word�ķ�װ
			/// @param port	  SIO��λ�ýӿ�
			/// @param data	  SIO�����ݽӿ�
			/// @param offset ��ȡ��ƫ����
			/// @param output �����ֵ
			/// @return �Ƿ�ɹ�
			bool SIOReadWord(const USHORT& port, const USHORT& data, const DWORD& offset, DWORD& output);
			/// @brief SIO��ȡһ��Byte�ķ�װ
			/// @param port	  SIO��λ�ýӿ�
			/// @param data	  SIO�����ݽӿ�
			/// @param offset ��ȡ��ƫ����
			/// @param output �����ֵ
			/// @return �Ƿ�ɹ�
			bool SIOReadByte(const USHORT& port, const USHORT& data, const DWORD& offset, DWORD& output);
		protected:
			/// @brief ��оƬ�Ƿ����
			bool active;

			/// @brief SIOռ�õ�IO��ȡ����
			Utils::Ring0::SafeIoHandle IoPtr;

			/// @brief SIO�Ľ���˿�
			std::array<std::pair<USHORT, USHORT>, 2> SIOPort;

			/// @brief Chip��ID
			const uint32_t CHIP_ID_REGISTER;
			/// @brief Chip��Revision
			const uint32_t CHIP_REVISION_REGISTER;
			/// @brief LPC��ַ
			const uint32_t BASE_ADDRESS_REGISTER;
		};
	}
}
