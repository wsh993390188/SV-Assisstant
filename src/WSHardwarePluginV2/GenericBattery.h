#pragma once
namespace Hardware
{
	namespace Battery
	{
		/// @brief ���ͨ�ó�ʼ������
		class GenericBattery
		{
		public:
			/// @brief ���캯�������ڳ�ʼ�������Ϣ
			GenericBattery() = default;

			/// @brief ����������
			virtual ~GenericBattery() = default;

			/// @brief ��ʼ�����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief ���µ����Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief ��ȡ��ػ�������
			/// Json��ʽ {"BatteryId": 0}
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType GetElements(LPCSTR paramter, std::string& response) = 0;
		};
	}
}
