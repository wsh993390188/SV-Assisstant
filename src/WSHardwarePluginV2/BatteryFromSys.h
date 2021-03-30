#pragma once
#include "GenericBattery.h"
#include "BatteryElement.h"
namespace Hardware
{
	namespace Battery
	{
		/// @brief ͨ������������ȡ�����Ϣ
		class BatteryFromSys final :public GenericBattery
		{
		public:
			/// @brief ���캯��
			BatteryFromSys();

			/// @brief ��ʼ�����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Initialize(std::string& response) override final;

			/// @brief ���µ����Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			Data::ErrorType Update(const std::string& Args, std::string& response) override final;

			/// @brief ��ȡ��ػ�������
			/// Json��ʽ {"BatteryId": 0}
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response) override final;
		private:
			/// @brief ��ȡȫ������豸��·��
			/// @param[out] DevicePath ����豸����
			/// @return �Ƿ�ɹ�
			bool GetDevicePath();

			/// @brief �����ʼ��Jsonֵ
			/// @return Json�ַ�
			std::string BuildInitializeJson();

			/// @brief ����Json�ַ�
			/// @param[in] JsonString JSON�ַ���
			/// @param[out] BatteryId �����ɹ���ĵ��Id
			/// @return �����Ƿ�ɹ�
			bool ParserJson(const std::string& JsonString, uint32_t& BatteryId);
		protected:
			/// @brief �����Ϣʵ���豸 First��Ӧ��Id��
			std::map<uint32_t, std::shared_ptr<BatteryElement>> DevicePath;
		};
	}
}
