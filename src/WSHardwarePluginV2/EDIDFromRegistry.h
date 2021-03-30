#pragma once
#include "GenericMonitor.h"
namespace Hardware
{
	namespace Monitor
	{
		/// @brief ��ע����ж�ȡEDID
		class EDIDFromRegistry : public GenericMonitor
		{
		public:
			/// @brief ���캯��
			EDIDFromRegistry();

			/// @brief ��������
			~EDIDFromRegistry() = default;

			/// @brief ��ʼ����ʾ��
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response);

			/// @brief ������ʾ����Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response);
		private:
			/// @brief ��ȡ��ǰ��ʾ������Ϣ
			/// @param Monitor �����ʾ���ڵ��Զ˶�Ӧ��Device����Ϣ
			/// @return �Ƿ�ɹ�
			bool GetCurrentMonitor(std::vector<DISPLAY_DEVICE>& Monitor);

			/// @brief ��ȡ��ʾ����Model��Driver��Ϣ
			/// @param[in] lpDeviceid ��ʾ����DeviceID
			/// @param[out] Model ��ʾ����Model
			/// @param[out] Driver ��ʾ����Driver
			/// @return �Ƿ�ɹ�
			bool GetModelandDriver(
				const std::wstring& lpDeviceid,
				std::wstring& Model,
				std::wstring& Driver
			);

			/// @brief ����Model��Driver��λע��� ��ȡ��ǰ��ʾ����EDID
			/// @param[in] Model ��ʾ����Model
			/// @param[in] Driver ��ʾ����Driver
			/// @param[out] EDIDbuffer �����������ַ
			/// @return �Ƿ�ɹ�
			bool GetEDID(
				const std::wstring& Model,
				const std::wstring& Driver,
				EDID& EDIDbuffer);
		private:
			/// @brief ��ʾ��ʵ������
			std::vector<DISPLAY_DEVICE> Monitor;
		};
	}
}