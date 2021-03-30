#pragma once
#include "EDIDdefination.h"
namespace Hardware
{
	namespace Monitor
	{
		/// @brief ��ʾ������Ϣ���ݽṹ
		using MonitorDataStruct = std::vector<std::pair<std::string, std::string>>;

		/// @brief ��ʾ��ͨ�ó�ʼ������
		class GenericMonitor
		{
		public:
			/// @brief ���캯�������ڳ�ʼ����ʾ����Ϣ
			GenericMonitor() = default;

			/// @brief ����������
			virtual ~GenericMonitor() = default;

			/// @brief ��ʼ����ʾ��
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief ������ʾ����Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief ��ȡ��ʾ���������ݲ���
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief ��ʾ����Ϣ�洢����
			std::map<std::wstring, MonitorDataStruct> MonitorInfos;
		protected:
			/// @brief ������ʼ��JSON����
			/// @return JSON Utf-8�ַ�
			std::string BuildInitializeJson();

			/// @brief ������ʾ����Ϣ��JSON�ַ�
			/// @param[in] MonitorId ��ʾ��ID
			/// @return JSON�ַ�
			std::string BuildElementJson(const std::wstring& MonitorId);

			/// @brief ����Json�ַ�
			/// @param[in] JsonString JSON�ַ���
			/// @param[out] MonitorId �����ɹ������ʾ��Id
			/// @return �����Ƿ�ɹ�
			bool ParserJson(const std::string& JsonString, std::wstring& MonitorId);

			/// @brief ����EDID
			/// @param[in] EDIDbuffer EDID����Ϣ
			/// @return �Ƿ�ɹ�
			bool ParserEDID(const EDID& EDIDbuffer, MonitorDataStruct& MonitorInfo);

			/// @brief ����EDID������
			/// @param EDIDbuffer
			/// @return
			const std::string GetMonitorName(const Hardware::Monitor::EDID& EDIDbuffer);
		};
	}
}
