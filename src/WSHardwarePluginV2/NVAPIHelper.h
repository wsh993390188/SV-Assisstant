#pragma once
#include "..\..\Libcommon\NVAPI\nvapi.h"

namespace Hardware
{
	namespace GPU
	{
		/// @brief NVAPI不存在的异常
		class NVAPINotExist :public std::exception
		{
		public:
			/// @brief 异常说明
			/// @return 异常说明
			char const* what() const override;
		};

		/// @brief NVAPI设备不存在的异常
		class NVAPIDeviceNotExist :public std::exception
		{
		public:
			/// @brief 初始化异常
			/// @param DeviceId 设备ID
			explicit NVAPIDeviceNotExist(const NvU32& DeviceId)
			{
				Msg = "NVAPI Cant find gpu " + std::to_string(DeviceId);
			}
			/// @brief 异常说明
			/// @return 异常说明
			char const* what() const override;
		private:
			/// @brief 异常消息
			std::string Msg;
		};

		/// @brief 获取温度发生异常时抛出
		class NVAPITemperatureExp :public std::exception
		{
		public:
			/// @brief 获取的温度信息
			/// @param target 目标区域
			NVAPITemperatureExp(const NV_THERMAL_TARGET& target) : target(target) {
				Msg = "NVAPI Temperature zone: " + TargetToString();
			}

			/// @brief 异常说明
			/// @return 异常说明
			char const* what()const override;
		private:
			/// @brief 温度的目标区域
			NV_THERMAL_TARGET target;
			/// @brief 目标区域的字符串信息
			/// @return 字符串信息
			std::string TargetToString() const;
			/// @brief 异常消息
			std::string Msg;
		};
		/// @brief NVAPI的帮助类
		class NVAPIHelper final
		{
		public:
			/// @brief NVAPI帮助函数的单例
			/// @return 单例
			static NVAPIHelper& Instance();

			/// @brief 获取设备GPU的温度信息
			/// @param DeviceID 设备ID
			/// @exception 会抛出异常
			/// - @ref NVAPIException NVAPI未能加载
			/// - @ref NVAPIDeviceNotExist 找不到设备
			/// - @ref NVAPITemperatureExp 获取不到设备的温度
			/// @return 设备的温度
			std::string GetTemperature(const uint32_t& DeviceID);

			/// @brief 获取设备GPU的风扇转速
			/// @param DeviceID 设备ID
			/// @exception 会抛出异常
			/// - @ref NVAPIException NVAPI未能加载
			/// - @ref NVAPIDeviceNotExist 找不到设备
			///	- @ref std::exception 获取失败
			/// @return 设备的转速
			std::string GetFanSpeed(const uint32_t& DeviceID);

			/// @brief 获取设备GPU的功率
			/// @param DeviceID 设备ID
			/// @exception 会抛出异常
			/// - @ref NVAPIException NVAPI未能加载
			/// - @ref NVAPIDeviceNotExist 找不到设备
			///	- @ref std::exception 获取失败
			/// @return 设备的功率
			std::string GetGPUPower(const uint32_t& DeviceID);

			/// @brief 获取设备GPU的核心电压
			/// @param DeviceID 设备ID
			/// @exception 会抛出异常
			/// - @ref NVAPIException NVAPI未能加载
			/// - @ref NVAPIDeviceNotExist 找不到设备
			///	- @ref std::exception 获取失败
			/// @return 设备的核心电压
			std::string GetVoltage(const uint32_t& DeviceID);

			/// @brief 获取设备GPU的名称信息
			/// @param DeviceID 设备ID
			/// @exception 会抛出异常
			/// - @ref NVAPIException NVAPI未能加载
			/// - @ref NVAPIDeviceNotExist 找不到设备
			/// @return 设备名称信息
			std::string GetGPUName(const uint32_t& DeviceID);

			/// @brief 获取设备GPU的驱动版本
			/// @exception 会抛出异常
			/// - @ref NVAPIException NVAPI未能加载
			/// - @ref NVAPIDeviceNotExist 找不到设备
			///	- @ref std::exception 获取失败
			/// @return 驱动版本
			std::string GetGPUDriverVersion();

			/// @brief 获取显存频率信息
			/// @param AdapterId 目标设备的AdapterID
			/// @throw 可能抛出以下异常
			/// - @ref NVAPIException NVAPI未能加载
			/// - @ref NVAPIDeviceNotExist 找不到设备
			///	- @ref std::exception 获取失败
			/// @return 显存频率
			std::string GetGPUMemoryClock(const uint32_t& DeviceId);

			/// @brief 获取显卡核心频率信息
			/// @param AdapterId 目标设备的AdapterID
			/// @throw 可能抛出以下异常
			/// - @ref NVAPIException NVAPI未能加载
			/// - @ref NVAPIDeviceNotExist 找不到设备
			///	- @ref std::exception 获取失败
			/// @return 显卡核心频率
			std::string GetGPUCoreClock(const uint32_t& DeviceId);

		private:
			/// @brief 帮助函数初始化
			NVAPIHelper();
			/// @brief 帮助函数的析构操作
			~NVAPIHelper();

			/// @brief 获取NVAPI的错误信息
			/// @param nvapiErrorStatus 错误代码
			/// @return 错误信息
			std::string GetNvAPIStatusString(const NvAPI_Status& nvapiErrorStatus);

			/// @brief 枚举GPU，仅初始化时调用即可
			void EnumPhysicalGPUs();

			/// @brief NVAPI接口查询GPU当前温度
			/// @param GPUHandle GPU的句柄
			/// @param target GPU需要查询的温度信息
			/// @exception 会抛出异常
			/// - @ref NVAPITemperatureExp 获取不到设备的温度
			/// @return 温度的信息
			NvS32 GetTemperatureImpl(const NvPhysicalGpuHandle& GPUHandle, const NV_THERMAL_TARGET& target);

			/// @brief NVAPI接口查询GPU的动态信息
			/// @param GPUHandle GPU的句柄
			/// @exception 会抛出异常
			/// - @ref NVAPIException NVAPI未能加载
			/// - @ref NVAPIDeviceNotExist 找不到设备
			///	- @ref std::exception 获取失败
			/// @return 动态信息
			void GetDynamicInfo(const NvPhysicalGpuHandle& GPUHandle);
		private:
			/// @brief N卡GPU的查询信息
			struct NVGPUPhysicalInfo
			{
#pragma pack(push,1)
				union
				{
					struct
					{
						NvU16 VendorId;///<厂商ID
						NvU16 DeviceId;///<设备ID
					};
					NvU32 MajorId;///<主ID
				};

				union
				{
					struct
					{
						NvU16 SubVendorId;///<子厂商ID
						NvU16 SubDeviceId;///<子设备ID
					};
					NvU32 SubId;///<从ID
				};
#pragma pack(pop)
				NvU32 RevisionId;///<修订版本
				NvU32 ExtDeviceId;///<子设备ID
				NvPhysicalGpuHandle hPhysicalGpu;///<NVAPI定义的物理GPU句柄
			};
			/// @brief NVAPI 是否可用
			bool IsNvapiEnable;

			/// @brief GPU的查询信息
			std::vector<NVGPUPhysicalInfo> GpuQueryDatas;

			struct NV_VOLTAGES_INFO
			{
				NV_GPU_PERF_VOLTAGE_INFO_DOMAIN_ID       domainId;       //!< ID of the voltage domain
				NvU32                                   unknown1;
				NvU32                                   max;

				struct
				{
					NvU32                               unknown2;
					NvU32                               mvolt;          //!< Voltage in mV
				} info[128];
			};

			struct NV_VOLTAGES
			{
				NvU32                                   Version;        //!< Structure version
				NvU32                                   Flags;          //!< Reserved for future use. Must be set to 0
				NvU32                                   max;
				NV_VOLTAGES_INFO voltages[NVAPI_MAX_GPU_PERF_VOLTAGES];
			};
			/// @brief NvAPI未导出的查询电压的接口
			using TYPE_NvAPI_GPU_GetVoltages = NvAPI_Status(*)(HANDLE PhysicalGPU, NV_VOLTAGES* pPerfVoltages);
			TYPE_NvAPI_GPU_GetVoltages NvAPI_GPU_GetVoltages;

			enum class NV_POWER_TOPOLOGY_FLAGS : NvU32{
				NV_POWER_TOPOLOGY_FLAG_UNKNOWN1,
				NV_POWER_TOPOLOGY_FLAG_UNKNOWN2
			};

			struct NV_POWER_TOPOLOGY_STATUS {
				NvU32 version;
				NvU32 count = 0;

				struct {
					NV_POWER_TOPOLOGY_FLAGS flags;
					NvU32 unknown2 = 0;
					NvU32 power = 0;
					NvU32 unknown3 = 0;
				} entries[4];
			};
			using NvAPI_GPU_ClientPowerTopologyGetStatus_t = NvAPI_Status(*)(NvPhysicalGpuHandle hPhysicalGpu, NV_POWER_TOPOLOGY_STATUS* pTopologyStatus);
			NvAPI_GPU_ClientPowerTopologyGetStatus_t NvAPI_GPU_ClientPowerTopologyGetStatus;
		};
	}
}
