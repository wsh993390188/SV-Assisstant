#pragma once
#include "..\..\Libcommon\adlsdk\include\adl_sdk.h"
#include "Overdrive.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief ADL设备不存在的异常
		class ADLDeviceNotExist : public std::exception
		{
		public:
			/// @brief 初始化异常
			/// @param DeviceId 设备ID
			explicit ADLDeviceNotExist(const uint32_t& DeviceId) {
				Msg = "ADL Cant find gpu " + std::to_string(DeviceId);
			}
			/// @brief 异常说明
			/// @return 异常说明
			char const* what() const override;
		private:
			/// @brief 异常消息
			std::string Msg;
		};

		/// @brief ADL函数成功的包装
		/// @param status ADL的函数执行结果
		/// @return 是否执行成功
		bool ADL_Success(const int& status);

		/// @brief ADL执行状态翻译成字符串形式
		/// @param status ADL函数执行结果
		/// @return 执行结果的字符串形式
		std::string GetADLStatus(const int& status);

		/// @brief adl sdk的帮助函数
		class ADLSDKHelper final
		{
			typedef int(*ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int);
			typedef int(*ADL_MAIN_CONTROL_DESTROY)(void);
			typedef int (*ADL2_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int, ADL_CONTEXT_HANDLE*);
			typedef int (*ADL2_MAIN_CONTROL_DESTROY)(ADL_CONTEXT_HANDLE);
			typedef	int(*ADL_GRAPHICS_VERSIONS_GET)(ADLVersionsInfo*);
			typedef int(*ADL_GRAPHICS_PLATFORM_GET)(int*);
			typedef int(*ADL_ADAPTER_ACTIVE_GET)(int, int*);
			typedef int(*ADL_ADAPTER_NUMBEROFADAPTERS_GET)(int*);
			typedef int(*ADL_ADAPTER_ADAPTERINFO_GET)(LPAdapterInfo, int);
			typedef int(*ADL_ADAPTER_ASPECTS_GET)(int, char*, int);
			typedef int(*ADL_ADAPTER_ASICFAMILYTYPE_GET)(int, int*, int*);
			typedef int(*ADL_ADAPTER_SPEED_CAPS)(int, int*, int*);
			typedef	int(*ADL_ADAPTER_SPEED_GET)(int, int*, int*);
			typedef int(*ADL_ADAPTER_ACCESSIBILITY_GET)(int, int*);
			typedef int(*ADL_ADAPTER_VIDEOBIOSINFO_GET)(int, ADLBiosInfo*);
			typedef int(*ADL_ADAPTER_ID_GET)(int, int*);
			typedef int(*ADL_ADAPTERX2_CAPS)(int, ADLAdapterCapsX2*);
			typedef int(*ADL_DISPLAY_DISPLAYINFO_GET)(int, int*, ADLDisplayInfo**, int);
			typedef int(*ADL_DISPLAY_DPMSTINFO_GET)(int, int*, ADLDisplayDPMSTInfo**, int);
			typedef int(*ADL_DISPLAY_PRESERVEDASPECTRATIO_GET)(int, int, int*, int*, int*);
			typedef int(*ADL_DISPLAY_IMAGEEXPANSION_GET)(int, int, int*, int*, int*);
			typedef int(*ADL_DISPLAY_POSITION_GET)(int, int, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*);
			typedef int(*ADL_DISPLAY_SIZE_GET)(int, int, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*);
			typedef int(*ADL_DISPLAY_ADJUSTCAPS_GET)(int, int, int*);
			typedef int(*ADL_DISPLAY_CAPABILITIES_GET)(int, int*, int*);
			typedef int(*ADL_DISPLAY_CONNECTEDDISPLAYS_GET)(int, int*);
			typedef int(*ADL_DISPLAY_DEVICECONFIG_GET)(int, int, ADLDisplayConfig*);
			typedef int(*ADL_DISPLAY_PROPERTY_GET)(int, int, ADLDisplayProperty*);
			typedef int(*ADL_DISPLAY_SWITCHINGCAPABILITY_GET)(int, int*);
			typedef int(*ADL_DISPLAY_DITHERSTATE_GET)(int, int, int*);
			typedef int(*ADL_DISPLAY_SUPPORTEDPIXELFORMAT_GET)(int, int, int*);
			typedef int(*ADL_DISPLAY_PIXELFORMAT_GET)(int, int, int*);
			typedef int(*ADL_DISPLAY_SUPPORTEDCOLORDEPTH_GET)(int, int, int*);
			typedef int(*ADL_DISPLAY_COLORDEPTH_GET)(int, int, int*);
			typedef int(*ADL_DISPLAY_ODCLOCKINFO_GET)(int, ADLAdapterODClockInfo*);
			typedef int(*ADL_DISPLAY_ADJUSTMENTCOHERENT_GET)(int, int, int*, int*);
			typedef int(*ADL_DISPLAY_REDUCEDBLANKING_GET)(int, int, int*, int*);
			typedef int(*ADL_DISPLAY_FORMATSOVERRIDE_GET)(int, int, int*, int*, int*);
			typedef int(*ADL_DISPLAY_MVPUCAPS_GET)(int, ADLMVPUCaps*);
			typedef int(*ADL_DISPLAY_MVPUSTATUS_GET)(int, ADLMVPUStatus*);
			typedef int(*ADL_DISPLAY_MODES_GET)(int, int, int*, ADLMode**);
			typedef int(*ADL_DISPLAY_DISPLAYMAPCONFIG_GET)(int, int*, ADLDisplayMap**, int*, ADLDisplayTarget**, int);
			typedef int(*ADL_DISPLAY_MODETIMINGOVERRIDEX2_GET) (int, ADLDisplayID, ADLDisplayModeX2*, ADLDisplayModeInfo*);
			typedef int(*ADL_ADAPTER_MODETIMINGOVERRIDE_CAPS) (int, int*);
			typedef int(*ADL_OVERDRIVE_CAPS) (int iAdapterIndex, int* iSupported, int* iEnabled, int* iVersion);
			typedef int(*ADL2_OVERDRIVE_CAPS) (ADL_CONTEXT_HANDLE context, int iAdapterIndex, int* iSupported, int* iEnabled, int* iVersion);

			/// @brief ADL 查询的数据结构
			struct ADLQueryData
			{
				/// @brief The BUS number
				uint32_t BusNumber;
				/// @brief The Device number
				uint32_t DeviceNumber;
				/// @brief The function number.
				uint32_t FunctionNumber;
				/// @brief 显卡在ADL中的索引
				int AdapterIndex;
				/// @brief 设备ID
				uint32_t DeviceId;
				/// @brief 显卡名称
				std::string FullName;
				/// @brief ADL的显卡唯一标识
				std::string UDID;
				/// @brief OD的版本
				std::unique_ptr<ADLSDK::Overdrive> OD;
			};
		public:

			/// @brief ADL模块的单例模式
			/// @return 单例模式 @ref ADLSDKHelper
			static ADLSDKHelper& Instance();

			/// @brief 获取ADL模块指针
			/// @return ADL模块指针
			HMODULE GetADLModule() const;

			/// @brief 获取显卡温度信息
			/// @param DeviceId 目标设备的ID
			/// @throw 可能抛出以下异常
			///	- @ref ADLDeviceNotExist设备不存在
			///	- @ref std::exception 获取失败
			/// @return 温度
			std::string GetTemperature(const uint32_t& DeviceId);

			/// @brief 获取显卡风扇转速信息
			/// @param DeviceId 目标设备的ID
			/// @throw 可能抛出以下异常
			///	- @ref ADLDeviceNotExist设备不存在
			///	- @ref std::exception 获取失败
			/// @return 风扇转速
			std::string GetFanSpeed(const uint32_t& DeviceId);

			/// @brief 获取显存频率信息
			/// @param AdapterId 目标设备的AdapterID
			/// @throw 可能抛出以下异常
			///	- @ref ADLDeviceNotExist设备不存在
			///	- @ref std::exception 获取失败
			/// @return 显存频率
			std::string GetGPUMemoryClock(const uint32_t& DeviceId);

			/// @brief 获取显卡核心频率信息
			/// @param AdapterId 目标设备的AdapterID
			/// @throw 可能抛出以下异常
			///	- @ref ADLDeviceNotExist设备不存在
			///	- @ref std::exception 获取失败
			/// @return 显卡核心频率
			std::string GetGPUCoreClock(const uint32_t& DeviceId);

			/// @brief 获取显卡的的电压
			/// @param DeviceId 显卡的设备ID
			/// @throw 可能抛出以下异常
			///	- @ref ADLDeviceNotExist设备不存在
			///	- @ref std::exception 获取失败
			/// @return 显卡电压
			std::string GetGPUVoltage(const uint32_t& DeviceId);

			/// @brief 获取显卡的名称
			/// @param DeviceId 显卡的设备ID
			/// @throw 可能抛出以下异常
			/// - @ref ADLDeviceNotExist设备不存在
			/// @return 显卡名称
			std::string GetGPUName(const uint32_t& DeviceId);

			/// @brief 获取显卡驱动版本
			/// @param DeviceId 显卡的设备ID
			/// @throw 可能抛出以下异常
			/// - @ref ADLDeviceNotExist设备不存在
			/// @return 显卡驱动版本
			std::string GetGPUDriverVersion(const uint32_t& DeviceId);

			/// @brief 获取显卡的BIOS版本
			/// @param DeviceId 显卡的设备ID
			/// @throw 可能抛出以下异常
			/// - @ref ADLDeviceNotExist设备不存在
			/// @return BIOS版本
			std::string GetGPUBIOSVersion(const uint32_t& DeviceId);

			/// @brief 获取显卡的BIOS日期
			/// @param DeviceId 显卡的设备ID
			/// @throw 可能抛出以下异常
			/// - @ref ADLDeviceNotExist设备不存在
			/// @return BIOS日期
			std::string GetGPUBIOSDate(const uint32_t& DeviceId);
		private:
			/// @brief 枚举所有活动的A卡
			/// @return 是否存在A卡
			bool EnumAdapter();

			/// @brief 查找设备是否已经找到
			/// @param BusNumber The BUS number
			/// @param DevNumber The Device number
			/// @param FuncNumber The function number
			/// @return 如果找到则为true
			bool FindDeviceOnQueryDatas(const uint32_t& BusNumber, const uint32_t& DevNumber, const uint32_t& FuncNumber);

			/// @brief 检测ADL Overdrive的版本
			/// @param AdapterIndex 适配器的索引
			/// @return @ref OverDriveVersion
			ADLSDK::OverDriveVersion CheckOverDriveSupportVersion(const int& AdapterIndex);

		private:
			/// @brief 帮助函数的初始化操作
			ADLSDKHelper();

			/// @brief 帮助函数的析构操作
			~ADLSDKHelper();

			/// @brief adl的模块指针
			HMODULE hModule;

			/// @brief ADL的上下文环境
			ADL_CONTEXT_HANDLE context;

			/// @brief ADL是否可用
			bool IsADLEnable;

			/// @brief GPU的查询匹配信息
			std::vector<ADLQueryData> GpuQueryDatas;

			/// @brief 获取ADL的显卡BIOS信息
			ADL_ADAPTER_VIDEOBIOSINFO_GET ADL_Adapter_VideoBiosInfo_Get;
		};
	}
}
