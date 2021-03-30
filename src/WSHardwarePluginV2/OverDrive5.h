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
				/// @brief 初始化OD6
				/// @param hModule ADL的模块地址
				explicit OverDrive5(HMODULE hModule);
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
				/// @brief 获取显卡风扇转速信息(RPM值)
				/// @param hModule ADL的模块地址
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return RPM值
				std::string GetFanSpeedRPM(const uint32_t& AdapterId);

				/// @brief 获取显卡风扇转速信息(百分比)
				/// @param hModule ADL的模块地址
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 风扇转速的百分比
				std::string GetFanSpeedPercent(const uint32_t& AdapterId);
			private:
				/// @brief OD5的温度传感器接口
				ADL_OVERDRIVE5_TEMPERATURE_GET ADL_Overdrive5_Temperature_Get;
				/// @brief OD5的风扇传感器接口
				ADL_OVERDRIVE5_FANSPEED_GET ADL_Overdrive5_FanSpeed_Get;
				/// @brief OD5的状态sensor接口
				ADL_OVERDRIVE5_CURRENTACTIVITY_GET ADL_Overdrive5_CurrentActivity_Get;
			};
		}
	}
}
