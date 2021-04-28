#pragma once
namespace Hardware
{
	/// @brief BIOS�����Ϣ�Ŀռ�
	namespace BIOS
	{
		/// @brief BIOSͨ��������Ϣ������CPUZ
		struct BiosCommonStruct
		{
			std::string Brand;
			std::string Version;
			std::string DataTime;
			bool UEFI; ///< �Ƿ�ΪUEFI����
			bool SecureBoot; ///<�Ƿ�Ϊ��ȫ����
		};

		/// @brief BIOS��ͨ�ýӿ���
		class GenericBios final
		{
		public:
			/// @brief ���캯�������ڳ�ʼ��BIOS����Ϣ
			GenericBios() = default;

			/// @brief ����������
			~GenericBios() = default;

			/// @brief ��ʼ��BIOS
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response);

			/// @brief ����BIOS��Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response);

			/// @brief ��ȡBIOS�������ݲ���
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief Bios��Ϣ����֧��˫BIos��δ������һ��
			BiosCommonStruct BiosInfos;

		private:
			/// @brief �����������ݹ���Bios��Json��Ϣ
			/// @return ����Bios���ݵ�Json����
			Json::Value BuildBiosToJson();

			/// @brief ���ݳ�ʼ�����ṹ����JSON�ַ���
			/// @return Utf-8��ʽ��Json�ַ���
			std::string BuildInitJson();

			/// @brief �ж�ϵͳ�Ƿ�ΪUEFI����
			/// @return �Ƿ���UEFI����
			bool IsUEFI() const;

			/// @brief ��ȫ�����Ƿ���
			/// @return ����
			bool SecureBootEnabled();
		};
	}
}