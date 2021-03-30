#pragma once
namespace Hardware
{
	namespace Audio
	{
		/// @brief ͨ����Ϣ
		struct CommonInfo
		{
			std::wstring id; ///< ע����豸id
			std::wstring state; ///< �豸״̬
			std::wstring name;///<�豸��
			std::wstring desc;///<�豸����
			std::wstring audioif;///<�����豸����
		};

		/// @brief �豸��Ϣ
		struct DeviceInfo
		{
			std::map<uint32_t, CommonInfo> Audios;///<���ڵķ����豸
			std::map<uint32_t, CommonInfo> Mics;///<���ڵ���˷��豸
		};

		/// @brief ����ͨ�ó�ʼ������
		class GenericAudio
		{
		public:
			/// @brief ���캯�������ڳ�ʼ��������Ϣ
			GenericAudio() = default;

			/// @brief ��ʼ������
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief ����������Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief ��ȡ�����������ݲ���
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief ������Ϣ�洢����
			DeviceInfo AudioInfos;

			enum class DeviceType
			{
				Audio = 0,
				Mic,
			};
		protected:
			/// @brief ������ʼ��Json
			/// @return Json�ַ�
			std::string BuildInitializeJson();

			/// @brief ����Json
			/// @param[in] JsonString Json�ַ�
			/// @param[out] AudioId ��ƵId
			/// @param[out] DeviceType ��Ƶ���� @ref DeviceType
			/// @return �Ƿ�����ɹ�
			bool ParserJson(const std::string& JsonString, uint32_t& AudioId, DeviceType& DevType);

			/// @brief ������ƵԪ��Json�ַ�
			/// @param[in] DeviceInfos �豸��Ϣ����
			/// @param[in] AudioId �豸Id
			/// @return Json�ַ�
			std::string BuildElementJson(const std::map<uint32_t, CommonInfo>& DeviceInfos, const uint32_t& AudioId);
		};
	}
}
