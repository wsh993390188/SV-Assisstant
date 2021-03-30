#pragma once
#include "Overdrive.h"
namespace Hardware
{
	namespace GPU
	{
		namespace ADLSDK
		{
			/// @brief OD8�İ�װ
			class OverDrive8 final : public Overdrive
			{
			public:
				/// @brief ��ʼ��OD8
				/// @param hModule ADL��ģ���ַ
				explicit OverDrive8(HMODULE hModule);

				/// @brief �������ĵĳ�ʼ��OD 8
				/// @param hModule ADL��ģ���ַ
				/// @param context ADL��������
				explicit OverDrive8(HMODULE hModule, ADL_CONTEXT_HANDLE context);
				/// @brief ��ȡ�Կ��¶���Ϣ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �¶�
				std::string GetTemperature(const uint32_t& AdapterId) override final;

				/// @brief ��ȡ�Կ�����ת����Ϣ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return ����ת��
				std::string GetFanSpeed(const uint32_t& AdapterId)  override final;

				/// @brief ��ȡ�Դ�Ƶ����Ϣ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �Դ�Ƶ��
				std::string GetGPUMemoryClock(const uint32_t& AdapterId) override final;

				/// @brief ��ȡ�Կ�����Ƶ����Ϣ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �Կ�����Ƶ��
				std::string GetGPUCoreClock(const uint32_t& AdapterId)  override final;

				/// @brief ��ȡ�Կ���ѹ��Ϣ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �Կ���ѹ
				std::string GetGPUVoltage(const uint32_t& AdapterId) override final;
			private:
				typedef int(*ADL2_NEW_QUERYPMLOGDATA_GET) (ADL_CONTEXT_HANDLE, int, ADLPMLogDataOutput*);
				/// @brief OD8�Ĵ������ӿ�
				ADL2_NEW_QUERYPMLOGDATA_GET ADL2_New_QueryPMLogData_Get;
				/// @brief ADL�������Ļ���
				ADL_CONTEXT_HANDLE context;
			};
		}
	}
}
