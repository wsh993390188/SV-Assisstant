#pragma once
#include "EDIDdefination.h"
namespace Hardware
{
	namespace Monitor
	{
		/// @brief ��ʾ������Ϣ���ݽṹ
		using MonitorDataStruct = std::vector<std::pair<std::string, std::vector<std::string>>>;

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
			/// @param[in] EDIDRawBuffer EDID�Ķ�������Ϣ
			/// @return �Ƿ�ɹ�
			bool ParserEDID(const std::vector<uint8_t>& EDIDRawBuffer, MonitorDataStruct& MonitorInfo);

			/// @brief ����EDID������
			/// @param EDIDbuffer
			/// @return EDID����
			const std::string GetMonitorName(const std::uint16_t& EDIDbuffer);

		private:
			/// @brief ���������е���Ϣ
			/// @param Descriptor ������Ϣ
			/// @param Name ����
			/// @param MonitorInfo ��ʾ����Ϣ
			/// @return �Ƿ����ӳɹ�
			bool AddDescriptorString(MonitorDataStruct& MonitorInfo, const EDID_Descriptor_Common_String& Descriptor, const std::string& Name);

			/// @brief
			/// @param MonitorInfo
			/// @param Descriptor
			/// @return
			bool AddDisplayRangeLimits(MonitorDataStruct& MonitorInfo, const EDID_Display_Range_Limits_Descriptor& Descriptor);

			/// @brief
			/// @param MonitorInfo
			/// @param EstablishedTiming
			/// @return
			bool AddEstablishedTiming(MonitorDataStruct& MonitorInfo, const EDIDCommon::EstablishedTimingSection& EstablishedTiming);

			/// @brief ���ӱ�׼ʱ�򣨱�׼�ڣ�
			/// @param MonitorInfo �洢����Ϣ
			/// @param StandardTiming ʱ����Ϣ
			/// @return �Ƿ����ӳɹ�
			bool AddStandardTiming(MonitorDataStruct& MonitorInfo, const EDIDCommon::StandardTimingSection& StandardTiming);

			/// @brief ���ӱ�׼ʱ����������
			/// @param MonitorInfo �洢����Ϣ
			/// @param StandardTiming ʱ����Ϣ
			/// @return �Ƿ����ӳɹ�
			bool AddStandardTiming(MonitorDataStruct& MonitorInfo, const EDID_StandardTimingIdentifierDefinition& StandardTiming);

			/// @brief ���ӱ�׼ʱ���ʵ��
			/// @param Timing �洢������
			/// @param value ʱ��Ĵ洢����
			void AddStandardTimingImpl(std::vector<std::string>& Timing, const std::uint16_t value);

			/// @brief ������ʾ����ϸ����Ϣ
			/// @param MonitorInfo �洢����Ϣ
			/// @param DetailedTiming ��ʾ��ϸ��
			/// @return �Ƿ��Ѿ���ȡ������ʾ���Ĵ�С
			bool AddPreferredDetailedTiming(MonitorDataStruct& MonitorInfo, const EDID_Detailed_Timing_Descriptor& DetailedTiming);
		};
	}
}
