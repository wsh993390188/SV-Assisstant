#pragma once
#include "Overdrive.h"
namespace Hardware
{
	namespace GPU
	{
		namespace ADLSDK
		{
			/// @brief OD8的包装
			class OverDrive8 final : public Overdrive
			{
			public:
				/// @brief 初始化OD8
				/// @param hModule ADL的模块地址
				explicit OverDrive8(HMODULE hModule);

				/// @brief 带上下文的初始化OD 8
				/// @param hModule ADL的模块地址
				/// @param context ADL的上下文
				explicit OverDrive8(HMODULE hModule, ADL_CONTEXT_HANDLE context);
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
				typedef int(*ADL2_NEW_QUERYPMLOGDATA_GET) (ADL_CONTEXT_HANDLE, int, ADLPMLogDataOutput*);
				/// @brief OD8的传感器接口
				ADL2_NEW_QUERYPMLOGDATA_GET ADL2_New_QueryPMLogData_Get;
				/// @brief ADL的上下文环境
				ADL_CONTEXT_HANDLE context;
			};
		}
	}
}
