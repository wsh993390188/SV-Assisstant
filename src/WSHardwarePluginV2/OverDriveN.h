#pragma once
#include "Overdrive.h"
namespace Hardware
{
	namespace GPU
	{
		namespace ADLSDK
		{
			/// @brief OD N�İ�װ
			class OverDriveN final : public Overdrive
			{
				typedef int(*ADL2_OVERDRIVEN_CAPABILITIESX2_GET)	(ADL_CONTEXT_HANDLE, int, ADLODNCapabilitiesX2*);
				typedef int(*ADL2_OVERDRIVEN_PERFORMANCESTATUS_GET) (ADL_CONTEXT_HANDLE, int, ADLODNPerformanceStatus*);
				typedef int(*ADL2_OVERDRIVEN_FANCONTROL_GET) (ADL_CONTEXT_HANDLE, int, ADLODNFanControl*);
				typedef int(*ADL2_OVERDRIVEN_FANCONTROL_SET) (ADL_CONTEXT_HANDLE, int, ADLODNFanControl*);
				typedef int(*ADL2_OVERDRIVEN_POWERLIMIT_GET) (ADL_CONTEXT_HANDLE, int, ADLODNPowerLimitSetting*);
				typedef int(*ADL2_OVERDRIVEN_POWERLIMIT_SET) (ADL_CONTEXT_HANDLE, int, ADLODNPowerLimitSetting*);
				typedef int(*ADL2_OVERDRIVEN_TEMPERATURE_GET) (ADL_CONTEXT_HANDLE, int, int, int*);
				typedef int(*ADL2_OVERDRIVEN_SYSTEMCLOCKSX2_GET)	(ADL_CONTEXT_HANDLE, int, ADLODNPerformanceLevelsX2*);
				typedef int(*ADL2_OVERDRIVEN_SYSTEMCLOCKSX2_SET)	(ADL_CONTEXT_HANDLE, int, ADLODNPerformanceLevelsX2*);
				typedef int(*ADL2_OVERDRIVEN_MEMORYCLOCKSX2_GET)	(ADL_CONTEXT_HANDLE, int, ADLODNPerformanceLevelsX2*);
				typedef int(*ADL2_OVERDRIVEN_MEMORYCLOCKSX2_SET)	(ADL_CONTEXT_HANDLE, int, ADLODNPerformanceLevelsX2*);
				typedef int(*ADL2_OVERDRIVEN_MEMORYTIMINGLEVEL_GET) (ADL_CONTEXT_HANDLE context, int iAdapterIndex, int* lpSupport, int* lpCurrentValue, int* lpDefaultValue, int* lpNumberLevels, int** lppLevelList);
				typedef int(*ADL2_OVERDRIVEN_MEMORYTIMINGLEVEL_SET) (ADL_CONTEXT_HANDLE context, int iAdapterIndex, int currentValue);
				typedef int(*ADL2_OVERDRIVEN_ZERORPMFAN_GET) (ADL_CONTEXT_HANDLE context, int iAdapterIndex, int* lpSupport, int* lpCurrentValue, int* lpDefaultValue);
				typedef int(*ADL2_OVERDRIVEN_ZERORPMFAN_SET) (ADL_CONTEXT_HANDLE context, int iAdapterIndex, int currentValue);
				typedef int(*ADL2_OVERDRIVEN_SETTINGSEXT_GET) (ADL_CONTEXT_HANDLE context, int iAdapterIndex, int* lpOverdriveNExtCapabilities, int* lpNumberOfODNExtFeatures, ADLODNExtSingleInitSetting** lppInitSettingList, int** lppCurrentSettingList);
				typedef int(*ADL2_OVERDRIVEN_SETTINGSEXT_SET) (ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iNumberOfODNExtFeatures, int* itemValueValidList, int* lpItemValueList);
			public:
				/// @brief �������ĵĳ�ʼ��OD N
				/// @param hModule ADL��ģ���ַ
				/// @param context ADL��������
				explicit OverDriveN(HMODULE hModule, ADL_CONTEXT_HANDLE context);

				/// @brief ��ʼ��OD N
				/// @param hModule ADL��ģ���ַ
				explicit OverDriveN(HMODULE hModule);

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
				/// @param hModule ADL��ģ���ַ
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
				/// @brief ADL�������Ļ���
				ADL_CONTEXT_HANDLE context;

				/// @brief ODN�Ĵ������ӿ�
				ADL2_OVERDRIVEN_PERFORMANCESTATUS_GET ADL2_OverdriveN_PerformanceStatus_Get;

				/// @brief OD N�ķ��ȿ��ƽӿ�
				ADL2_OVERDRIVEN_FANCONTROL_GET ADL2_OverdriveN_FanControl_Get;

				/// @brief OD N���¶�sensor�ӿ�
				ADL2_OVERDRIVEN_TEMPERATURE_GET ADL2_OverdriveN_Temperature_Get;
			};
		}
	}
}
