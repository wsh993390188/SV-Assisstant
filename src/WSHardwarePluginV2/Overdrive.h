#pragma once
#include "..\..\Libcommon\adlsdk\include\adl_sdk.h"
namespace Hardware
{
	namespace GPU
	{
		namespace ADLSDK
		{
			/// @brief OverDrive版本
			enum class OverDriveVersion
			{
				NotSupport,///<不支持OverDrive
				OverDrive5,///< Version 5
				OverDrive6,///< Version 6
				OverDrive8,///< Version 8
				OverDriveN,///< Next Version
			};

			/// @brief OD支持的功能
			struct ODSupport
			{
				bool Temperature = false;///<温度
				bool FanSpeed = false;///<风扇转速
				bool Power = false;///<功耗
			};

			/// @brief OD的基类
			class Overdrive
			{
			public:
				/// @brief OD的析构函数
				virtual ~Overdrive() = default;

				/// @brief 获取显卡温度信息
				/// @param AdapterId 目标设备的AdapterID
				/// @param hModule ADL的模块地址
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 温度
				virtual std::string GetTemperature(const uint32_t& AdapterId) = 0;

				/// @brief 获取显卡风扇转速信息
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 风扇转速
				virtual std::string GetFanSpeed(const uint32_t& AdapterId) = 0;

				/// @brief 获取显存频率信息
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 显存频率
				virtual std::string GetGPUMemoryClock(const uint32_t& AdapterId) = 0;

				/// @brief 获取显卡核心频率信息
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 显卡核心频率
				virtual std::string GetGPUCoreClock(const uint32_t& AdapterId) = 0;

				/// @brief 获取显卡电压信息
				/// @param AdapterId 目标设备的AdapterID
				/// @throw 可能抛出以下异常
				///	- @ref std::exception 获取失败
				/// @return 显卡电压
				virtual std::string GetGPUVoltage(const uint32_t& AdapterId) = 0;
			};
		}
	}
}
