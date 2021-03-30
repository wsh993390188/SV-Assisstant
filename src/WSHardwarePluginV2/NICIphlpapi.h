#pragma once
#include "GenericNIC.h"
#include <NetCon.h>
#include "NICIFEntry.h"

namespace Hardware
{
	namespace NIC
	{
		class NICIphlpapi final :public GenericNIC
		{
		public:
			/// @brief ��ʼ������
			NICIphlpapi();
			/// @brief ��������
			~NICIphlpapi();
			/// @brief ��ʼ������
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief ����������Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;
		private:
			/// @brief ��ȡ�������豸�ӿ�����
			/// @param[in] state �豸�ӿ�ֵ
			/// @return �豸�ӿ��ַ���
			std::string ConvertDeviceIFToString(const DWORD& state);

			/// @brief ö�������豸
			/// @return @ref Data::ErrorType
			Data::ErrorType EnumCurrentDevice();

			/// @brief ��ʼ��WSA
			/// @return WSA�Ƿ�ɹ���ʼ��
			bool InitWSA();
		private:
			/// @brief WSA�Ƿ��Ѿ���ʼ��
			bool WSAInit;

			/// @brief ��ѯInterface����Ϣ
			std::unique_ptr<NICIFEnry> IfQuery;
		};
	}
}
