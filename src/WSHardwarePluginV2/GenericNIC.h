#pragma once
namespace Hardware
{
	namespace NIC
	{
		/// @brief IP��ַ�Ľṹ
		struct IPAddress
		{
			/// @brief IP��ַ����
			enum class IPAddressType : uint16_t
			{
				UNKNOWN = 0xFFFF,///< δ֪����
				IPV4 = 2,///< IPV4����
				IPV6 = 23,///< IPV6����
			};

			IPAddressType Type; ///<IP��ַ������
			std::string Address;///<IP��ַ
		};

		/// @brief ������Ϣ
		struct NetStruct
		{
			uint32_t	If_Index;///<�ӿڵ�����(��ϵͳ����)
			bool		IsWifi;///< �Ƿ�ΪWifi�豸
			GUID		IfGuid;///<��������GUID
			std::string Description;///<��������
			std::string ConnectionName;///<��������
			std::string InterfaceType;///<�ӿ�����
			std::string MacAddress;///<Ӳ����ַ
			uint64_t ConnectionSpeed;///<�����ٶ�
			uint32_t MTU;///<MTU
			std::vector<IPAddress> UniAddress;///< ������ַ

			// ��̬��Ϣ
			uint64_t InOctets;///<���յ���Packages����
			uint64_t OutOctets;///<���͵�packages����
		};

		/// @brief ����ͨ�ó�ʼ������
		class GenericNIC
		{
		public:
			/// @brief ���캯�������ڳ�ʼ��������Ϣ
			GenericNIC() = default;

			/// @brief ����������
			virtual ~GenericNIC() = default;

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
			std::map<uint32_t, NetStruct> NICInfos;
		protected:
			/// @brief ������ʼ��JSON����
			/// @return JSON Utf-8�ַ�
			std::string BuildInitializeJson();

			/// @brief ����������Ϣ��JSON�ַ�
			/// @param[in] NICId ����ID
			/// @return JSON�ַ�
			std::string BuildElementJson(const uint32_t& NICId);

			/// @brief ���������Ķ�̬��Ϣ
			/// @param Info �����Ķ�̬��Ϣ
			/// @return JSON�ַ�
			std::string BuildElementUpdateJson(const NetStruct& Info);

			/// @brief ����Json�ַ�
			/// @param[in] JsonString JSON�ַ���
			/// @param[out] NICId �����ɹ��������Id
			/// @return �����Ƿ�ɹ�
			bool ParserJson(const std::string& JsonString, uint32_t& NICId);
		};
	}
}
