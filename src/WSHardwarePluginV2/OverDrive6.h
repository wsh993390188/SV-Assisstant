#pragma once
#include "Overdrive.h"
namespace Hardware
{
	namespace GPU
	{
		namespace ADLSDK
		{
			/// @brief OD6�İ�װ
			class OverDrive6 final : public Overdrive
			{
				typedef int(*ADL_OVERDRIVE6_FANSPEED_GET)(int iAdapterIndex, ADLOD6FanSpeedInfo* lpFanSpeedInfo);
				typedef int(*ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS)(int iAdapterIndex, ADLOD6ThermalControllerCaps* lpThermalControllerCaps);
				typedef int(*ADL_OVERDRIVE6_TEMPERATURE_GET)(int iAdapterIndex, int* lpTemperature);
				typedef int(*ADL_OVERDRIVE6_CAPABILITIES_GET) (int iAdapterIndex, ADLOD6Capabilities* lpODCapabilities);
				typedef int(*ADL_OVERDRIVE6_STATEINFO_GET)(int iAdapterIndex, int iStateType, ADLOD6StateInfo* lpStateInfo);
				typedef int(*ADL_OVERDRIVE6_CURRENTSTATUS_GET)(int iAdapterIndex, ADLOD6CurrentStatus* lpCurrentStatus);
				typedef int(*ADL_OVERDRIVE6_POWERCONTROL_CAPS) (int iAdapterIndex, int* lpSupported);
				typedef int(*ADL_OVERDRIVE6_POWERCONTROLINFO_GET)(int iAdapterIndex, ADLOD6PowerControlInfo* lpPowerControlInfo);
				typedef int(*ADL_OVERDRIVE6_POWERCONTROL_GET)(int iAdapterIndex, int* lpCurrentValue, int* lpDefaultValue);
				typedef int(*ADL_OVERDRIVE6_FANSPEED_SET)(int iAdapterIndex, ADLOD6FanSpeedValue* lpFanSpeedValue);
				typedef int(*ADL_OVERDRIVE6_STATE_SET)(int iAdapterIndex, int iStateType, ADLOD6StateInfo* lpStateInfo);
				typedef int(*ADL_OVERDRIVE6_POWERCONTROL_SET)(int iAdapterIndex, int iValue);
			public:
				/// @brief ��ʼ��OD6
				/// @param hModule ADL��ģ���ַ
				explicit OverDrive6(HMODULE hModule);

				/// @brief ��ȡ�Կ��¶���Ϣ
				/// @param hModule ADL��ģ���ַ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �¶�
				std::string GetTemperature(const uint32_t& AdapterId) override final;

				/// @brief ��ȡ�Կ�����ת����Ϣ
				/// @param hModule ADL��ģ���ַ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return ����ת��
				std::string GetFanSpeed(const uint32_t& AdapterId)  override final;

				/// @brief ��ȡ�Դ�Ƶ����Ϣ
				/// @param hModule ADL��ģ���ַ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �Դ�Ƶ��
				std::string GetGPUMemoryClock(const uint32_t& AdapterId) override final;

				/// @brief ��ȡ�Կ�����Ƶ����Ϣ
				/// @param hModule ADL��ģ���ַ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �Կ�����Ƶ��
				std::string GetGPUCoreClock(const uint32_t& AdapterId)  override final;

				/// @brief ��ȡ�Կ���ѹ��Ϣ
				/// @param hModule ADL��ģ���ַ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �Կ���ѹ
				std::string GetGPUVoltage(const uint32_t& AdapterId) override final;
			private:
				/// @brief OD6���¶ȴ�����
				ADL_OVERDRIVE6_TEMPERATURE_GET ADL_Overdrive6_Temperature_Get;
				/// @brief OD6�ķ��ȴ�����
				ADL_OVERDRIVE6_FANSPEED_GET ADL_Overdrive6_FanSpeed_Get;
				/// @brief OD6��״̬������
				ADL_OVERDRIVE6_CURRENTSTATUS_GET ADL_Overdrive6_CurrentStatus_Get;
			};
		}
	}
}
