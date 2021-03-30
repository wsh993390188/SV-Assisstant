#pragma once
#include "Overdrive.h"
namespace Hardware
{
	namespace GPU
	{
		namespace ADLSDK
		{
			/// @brief OD5
			class OverDrive5 final : public Overdrive
			{
				typedef int(*ADL_OVERDRIVE5_THERMALDEVICES_ENUM) (int iAdapterIndex, int iThermalControllerIndex, ADLThermalControllerInfo* lpThermalControllerInfo);
				typedef int(*ADL_OVERDRIVE5_ODPARAMETERS_GET) (int  iAdapterIndex, ADLODParameters* lpOdParameters);
				typedef int(*ADL_OVERDRIVE5_TEMPERATURE_GET) (int iAdapterIndex, int iThermalControllerIndex, ADLTemperature* lpTemperature);
				typedef int(*ADL_OVERDRIVE5_FANSPEED_GET) (int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue* lpFanSpeedValue);
				typedef int(*ADL_OVERDRIVE5_FANSPEEDINFO_GET) (int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedInfo* lpFanSpeedInfo);
				typedef int(*ADL_OVERDRIVE5_ODPERFORMANCELEVELS_GET) (int iAdapterIndex, int iDefault, ADLODPerformanceLevels* lpOdPerformanceLevels);
				typedef int(*ADL_OVERDRIVE5_ODPARAMETERS_GET) (int iAdapterIndex, ADLODParameters* lpOdParameters);
				typedef int(*ADL_OVERDRIVE5_CURRENTACTIVITY_GET) (int iAdapterIndex, ADLPMActivity* lpActivity);
				typedef int(*ADL_OVERDRIVE5_FANSPEED_SET)(int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue* lpFanSpeedValue);
				typedef int(*ADL_OVERDRIVE5_ODPERFORMANCELEVELS_SET) (int iAdapterIndex, ADLODPerformanceLevels* lpOdPerformanceLevels);
				typedef int(*ADL_OVERDRIVE5_POWERCONTROL_CAPS)(int iAdapterIndex, int* lpSupported);
				typedef int(*ADL_OVERDRIVE5_POWERCONTROLINFO_GET)(int iAdapterIndex, ADLPowerControlInfo* lpPowerControlInfo);
				typedef int(*ADL_OVERDRIVE5_POWERCONTROL_GET)(int iAdapterIndex, int* lpCurrentValue, int* lpDefaultValue);
				typedef int(*ADL_OVERDRIVE5_POWERCONTROL_SET)(int iAdapterIndex, int iValue);
			public:
				/// @brief ��ʼ��OD6
				/// @param hModule ADL��ģ���ַ
				explicit OverDrive5(HMODULE hModule);
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
				/// @brief ��ȡ�Կ�����ת����Ϣ(RPMֵ)
				/// @param hModule ADL��ģ���ַ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return RPMֵ
				std::string GetFanSpeedRPM(const uint32_t& AdapterId);

				/// @brief ��ȡ�Կ�����ת����Ϣ(�ٷֱ�)
				/// @param hModule ADL��ģ���ַ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return ����ת�ٵİٷֱ�
				std::string GetFanSpeedPercent(const uint32_t& AdapterId);
			private:
				/// @brief OD5���¶ȴ������ӿ�
				ADL_OVERDRIVE5_TEMPERATURE_GET ADL_Overdrive5_Temperature_Get;
				/// @brief OD5�ķ��ȴ������ӿ�
				ADL_OVERDRIVE5_FANSPEED_GET ADL_Overdrive5_FanSpeed_Get;
				/// @brief OD5��״̬sensor�ӿ�
				ADL_OVERDRIVE5_CURRENTACTIVITY_GET ADL_Overdrive5_CurrentActivity_Get;
			};
		}
	}
}
