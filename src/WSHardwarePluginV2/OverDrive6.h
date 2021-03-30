#pragma once
#include "Overdrive.h"
namespace Hardware
{
	namespace GPU
	{
		namespace ADLSDK
		{
			/// @brief OD6的包装
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
				/// @brief 初始化OD6
				/// @param hModule ADL的模块地址
				explicit OverDrive6(HMODULE hModule);

				/// @brief 获取显卡温度信息
				/// @param hModule ADL的模块地址
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 温度
				std::string GetTemperature(const uint32_t& AdapterId) override final;

				/// @brief 获取显卡风扇转速信息
				/// @param hModule ADL的模块地址
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
				/// @param hModule ADL的模块地址
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 显卡核心频率
				std::string GetGPUCoreClock(const uint32_t& AdapterId)  override final;

				/// @brief 获取显卡电压信息
				/// @param hModule ADL的模块地址
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 显卡电压
				std::string GetGPUVoltage(const uint32_t& AdapterId) override final;
			private:
				/// @brief OD6的温度传感器
				ADL_OVERDRIVE6_TEMPERATURE_GET ADL_Overdrive6_Temperature_Get;
				/// @brief OD6的风扇传感器
				ADL_OVERDRIVE6_FANSPEED_GET ADL_Overdrive6_FanSpeed_Get;
				/// @brief OD6的状态传感器
				ADL_OVERDRIVE6_CURRENTSTATUS_GET ADL_Overdrive6_CurrentStatus_Get;
			};
		}
	}
}
