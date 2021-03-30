#pragma once
namespace Hardware
{
	namespace WinBio
	{
		/// @brief Win10�����ﴫ����ͨ����Ϣ
		struct WinBioCommonInfo
		{
			std::string Type; ///<����������
			std::string Desc; ///<����������
			std::string SerialNumber; ///<���������
			std::string Manufacturer; ///<��������������
			std::string Model; ///<�������ͺ�
			std::string Firmware; ///<�������̼��汾
		};

		/// @brief Win10�����ﴫ����ͨ�ó�ʼ������
		class GenericWinBio
		{
		public:
			/// @brief ���캯�������ڳ�ʼ��Win10�����ﴫ������Ϣ
			GenericWinBio() = default;

			/// @brief ����������
			virtual ~GenericWinBio() = default;

			/// @brief ��ʼ��Win10�����ﴫ����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief ����Win10�����ﴫ������Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief ��ȡWin10�����ﴫ�����������ݲ���
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief ������ʼ��JSON����
			/// @return JSON Utf-8�ַ�
			std::string BuildInitializeJson();

			/// @brief ����Win10�����ﴫ������Ϣ��JSON�ַ�
			/// @param[in] WinBioId Win10�����ﴫ����ID
			/// @return JSON�ַ�
			std::string BuildElementJson(const uint32_t& WinBioId);

			/// @brief ����Json�ַ�
			/// @param[in] JsonString JSON�ַ���
			/// @param[out] WinBioId �����ɹ��������Id
			/// @return �����Ƿ�ɹ�
			bool ParserJson(const std::string& JsonString, uint32_t& WinBioId);

			/// @brief Win10�����ﴫ�����ܵ���Ϣ
			std::map<uint32_t, WinBioCommonInfo> WinBioInfos;
		};
	}
}
