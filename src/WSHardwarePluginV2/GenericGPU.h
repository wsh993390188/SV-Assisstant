/// @bug Ŀǰ������DeviceID����GPU�豸�ģ����������ͬ��DID��Ϲ��
#pragma once
#include "GPUDeviceBase.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief GPUͨ�ó�ʼ������
		class GenericGPU
		{
		public:
			/// @brief ���캯�������ڳ�ʼ��GPU��Ϣ
			GenericGPU();

			/// @brief ����������
			virtual ~GenericGPU() = default;

			/// @brief ��ʼ��GPU
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief ����GPU��Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief ��ȡGPU��������
			/// Json��ʽ {"SocketId": 0}
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType GetElements(LPCSTR paramter, std::string& response) = 0;

		protected:
			/// @brief GPU��ʵ����
			std::map<uint32_t, std::shared_ptr<GPUDeviceBase>> GPUInfos;
		protected:
			/// @brief ������ʼ��JSON����
			/// @return JSON Utf-8�ַ�
			std::string BuildInitializeJson();

			/// @brief �����Կ���Ϣ��JSON�ַ�
			/// @param[in] GPUId �Կ�ID
			/// @return JSON�ַ�
			bool ParserJson(const std::string& JsonString, uint32_t& GPUId);
		};

		/// @brief �Կ����ƶ��󹤳���
		class GPUInstanceFactory final
		{
			/// @brief Ĭ�Ϲ��캯��
			GPUInstanceFactory() = default;
		public:
			/// @brief ��������
			/// @return ��������
			static GPUInstanceFactory& Instance();
			/// @brief �������
			/// @param[in] DevicePath ��ص��豸·��
			/// @return ���Ԫ�� @ref BatteryElement
			std::unique_ptr<GenericGPU> CreateGPU();
		};
	}
}
