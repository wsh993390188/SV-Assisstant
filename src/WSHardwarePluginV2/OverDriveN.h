#pragma once
#include "Overdrive.h"
namespace Hardware
{
	namespace GPU
	{
		namespace ADLSDK
		{
			/// @brief OD N的包装
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
				/// @brief 带上下文的初始化OD N
				/// @param hModule ADL的模块地址
				/// @param context ADL的上下文
				explicit OverDriveN(HMODULE hModule, ADL_CONTEXT_HANDLE context);

				/// @brief 初始化OD N
				/// @param hModule ADL的模块地址
				explicit OverDriveN(HMODULE hModule);

				/// @brief 获取显卡温度信息
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 温度
				std::string GetTemperature(const uint32_t& AdapterId) override final;

				/// @brief 获取显卡风扇转速信息
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 风扇转速
				std::string GetFanSpeed(const uint32_t& AdapterId)  override final;

				/// @brief 获取显存频率信息
				/// @param hModule ADL的模块地址
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 显存频率
				std::string GetGPUMemoryClock(const uint32_t& AdapterId) override final;

				/// @brief 获取显卡核心频率信息
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 显卡核心频率
				std::string GetGPUCoreClock(const uint32_t& AdapterId)  override final;

				/// @brief 获取显卡电压信息
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 显卡电压
				std::string GetGPUVoltage(const uint32_t& AdapterId) override final;
			private:
				/// @brief ADL的上下文环境
				ADL_CONTEXT_HANDLE context;

				/// @brief ODN的传感器接口
				ADL2_OVERDRIVEN_PERFORMANCESTATUS_GET ADL2_OverdriveN_PerformanceStatus_Get;

				/// @brief OD N的风扇控制接口
				ADL2_OVERDRIVEN_FANCONTROL_GET ADL2_OverdriveN_FanControl_Get;

				/// @brief OD N的温度sensor接口
				ADL2_OVERDRIVEN_TEMPERATURE_GET ADL2_OverdriveN_Temperature_Get;
			};
		}
	}
}
