#pragma once
namespace Hardware
{
	namespace MotherBroad
	{
		/// @brief ����ͨ�����ݣ�����CPUZ
		struct MotherBroadCommonStruct
		{
			std::string Manufacturer;
			std::string Model;
			std::string SerialNumber;
		};

		/// @brief ����ͨ�ó�ʼ������
		class GenericMotherBroad
		{
		public:
			/// @brief ���캯�������ڳ�ʼ��������Ϣ
			GenericMotherBroad() = default;

			/// @brief ����������
			virtual ~GenericMotherBroad() = default;

			/// @brief ��ʼ������
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief ����������Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief ��ȡ����������ݲ���
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief ������Ϣ
			MotherBroadCommonStruct BroadInfo;
		protected:
			/// @brief �����������ݹ��������Json��Ϣ
			/// @return ����Broad���ݵ�Json����
			virtual Json::Value BuildBroadToJson();

			/// @brief ���ݳ�ʼ�����ṹ����JSON�ַ���
			/// @return Utf-8��ʽ��Json�ַ���
			std::string BuildInitJson();
		};
	}
}
