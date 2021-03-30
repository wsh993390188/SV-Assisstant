#pragma once
#include "..\..\Libcommon\NVAPI\nvapi.h"

namespace Hardware
{
	namespace GPU
	{
		/// @brief NVAPI�����ڵ��쳣
		class NVAPINotExist :public std::exception
		{
		public:
			/// @brief �쳣˵��
			/// @return �쳣˵��
			char const* what() const override;
		};

		/// @brief NVAPI�豸�����ڵ��쳣
		class NVAPIDeviceNotExist :public std::exception
		{
		public:
			/// @brief ��ʼ���쳣
			/// @param DeviceId �豸ID
			explicit NVAPIDeviceNotExist(const NvU32& DeviceId)
			{
				Msg = "NVAPI Cant find gpu " + std::to_string(DeviceId);
			}
			/// @brief �쳣˵��
			/// @return �쳣˵��
			char const* what() const override;
		private:
			/// @brief �쳣��Ϣ
			std::string Msg;
		};

		/// @brief ��ȡ�¶ȷ����쳣ʱ�׳�
		class NVAPITemperatureExp :public std::exception
		{
		public:
			/// @brief ��ȡ���¶���Ϣ
			/// @param target Ŀ������
			NVAPITemperatureExp(const NV_THERMAL_TARGET& target) : target(target) {
				Msg = "NVAPI Temperature zone: " + TargetToString();
			}

			/// @brief �쳣˵��
			/// @return �쳣˵��
			char const* what()const override;
		private:
			/// @brief �¶ȵ�Ŀ������
			NV_THERMAL_TARGET target;
			/// @brief Ŀ��������ַ�����Ϣ
			/// @return �ַ�����Ϣ
			std::string TargetToString() const;
			/// @brief �쳣��Ϣ
			std::string Msg;
		};
		/// @brief NVAPI�İ�����
		class NVAPIHelper final
		{
		public:
			/// @brief NVAPI���������ĵ���
			/// @return ����
			static NVAPIHelper& Instance();

			/// @brief ��ȡ�豸GPU���¶���Ϣ
			/// @param DeviceID �豸ID
			/// @exception ���׳��쳣
			/// - @ref NVAPIException NVAPIδ�ܼ���
			/// - @ref NVAPIDeviceNotExist �Ҳ����豸
			/// - @ref NVAPITemperatureExp ��ȡ�����豸���¶�
			/// @return �豸���¶�
			std::string GetTemperature(const uint32_t& DeviceID);

			/// @brief ��ȡ�豸GPU�ķ���ת��
			/// @param DeviceID �豸ID
			/// @exception ���׳��쳣
			/// - @ref NVAPIException NVAPIδ�ܼ���
			/// - @ref NVAPIDeviceNotExist �Ҳ����豸
			///	- @ref std::exception ��ȡʧ��
			/// @return �豸��ת��
			std::string GetFanSpeed(const uint32_t& DeviceID);

			/// @brief ��ȡ�豸GPU�Ĺ���
			/// @param DeviceID �豸ID
			/// @exception ���׳��쳣
			/// - @ref NVAPIException NVAPIδ�ܼ���
			/// - @ref NVAPIDeviceNotExist �Ҳ����豸
			///	- @ref std::exception ��ȡʧ��
			/// @return �豸�Ĺ���
			std::string GetGPUPower(const uint32_t& DeviceID);

			/// @brief ��ȡ�豸GPU�ĺ��ĵ�ѹ
			/// @param DeviceID �豸ID
			/// @exception ���׳��쳣
			/// - @ref NVAPIException NVAPIδ�ܼ���
			/// - @ref NVAPIDeviceNotExist �Ҳ����豸
			///	- @ref std::exception ��ȡʧ��
			/// @return �豸�ĺ��ĵ�ѹ
			std::string GetVoltage(const uint32_t& DeviceID);

			/// @brief ��ȡ�豸GPU��������Ϣ
			/// @param DeviceID �豸ID
			/// @exception ���׳��쳣
			/// - @ref NVAPIException NVAPIδ�ܼ���
			/// - @ref NVAPIDeviceNotExist �Ҳ����豸
			/// @return �豸������Ϣ
			std::string GetGPUName(const uint32_t& DeviceID);

			/// @brief ��ȡ�豸GPU�������汾
			/// @exception ���׳��쳣
			/// - @ref NVAPIException NVAPIδ�ܼ���
			/// - @ref NVAPIDeviceNotExist �Ҳ����豸
			///	- @ref std::exception ��ȡʧ��
			/// @return �����汾
			std::string GetGPUDriverVersion();

			/// @brief ��ȡ�Դ�Ƶ����Ϣ
			/// @param AdapterId Ŀ���豸��AdapterID
			/// @throw �����׳������쳣
			/// - @ref NVAPIException NVAPIδ�ܼ���
			/// - @ref NVAPIDeviceNotExist �Ҳ����豸
			///	- @ref std::exception ��ȡʧ��
			/// @return �Դ�Ƶ��
			std::string GetGPUMemoryClock(const uint32_t& DeviceId);

			/// @brief ��ȡ�Կ�����Ƶ����Ϣ
			/// @param AdapterId Ŀ���豸��AdapterID
			/// @throw �����׳������쳣
			/// - @ref NVAPIException NVAPIδ�ܼ���
			/// - @ref NVAPIDeviceNotExist �Ҳ����豸
			///	- @ref std::exception ��ȡʧ��
			/// @return �Կ�����Ƶ��
			std::string GetGPUCoreClock(const uint32_t& DeviceId);

		private:
			/// @brief ����������ʼ��
			NVAPIHelper();
			/// @brief ������������������
			~NVAPIHelper();

			/// @brief ��ȡNVAPI�Ĵ�����Ϣ
			/// @param nvapiErrorStatus �������
			/// @return ������Ϣ
			std::string GetNvAPIStatusString(const NvAPI_Status& nvapiErrorStatus);

			/// @brief ö��GPU������ʼ��ʱ���ü���
			void EnumPhysicalGPUs();

			/// @brief NVAPI�ӿڲ�ѯGPU��ǰ�¶�
			/// @param GPUHandle GPU�ľ��
			/// @param target GPU��Ҫ��ѯ���¶���Ϣ
			/// @exception ���׳��쳣
			/// - @ref NVAPITemperatureExp ��ȡ�����豸���¶�
			/// @return �¶ȵ���Ϣ
			NvS32 GetTemperatureImpl(const NvPhysicalGpuHandle& GPUHandle, const NV_THERMAL_TARGET& target);

			/// @brief NVAPI�ӿڲ�ѯGPU�Ķ�̬��Ϣ
			/// @param GPUHandle GPU�ľ��
			/// @exception ���׳��쳣
			/// - @ref NVAPIException NVAPIδ�ܼ���
			/// - @ref NVAPIDeviceNotExist �Ҳ����豸
			///	- @ref std::exception ��ȡʧ��
			/// @return ��̬��Ϣ
			void GetDynamicInfo(const NvPhysicalGpuHandle& GPUHandle);
		private:
			/// @brief N��GPU�Ĳ�ѯ��Ϣ
			struct NVGPUPhysicalInfo
			{
#pragma pack(push,1)
				union
				{
					struct
					{
						NvU16 VendorId;///<����ID
						NvU16 DeviceId;///<�豸ID
					};
					NvU32 MajorId;///<��ID
				};

				union
				{
					struct
					{
						NvU16 SubVendorId;///<�ӳ���ID
						NvU16 SubDeviceId;///<���豸ID
					};
					NvU32 SubId;///<��ID
				};
#pragma pack(pop)
				NvU32 RevisionId;///<�޶��汾
				NvU32 ExtDeviceId;///<���豸ID
				NvPhysicalGpuHandle hPhysicalGpu;///<NVAPI���������GPU���
			};
			/// @brief NVAPI �Ƿ����
			bool IsNvapiEnable;

			/// @brief GPU�Ĳ�ѯ��Ϣ
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
			/// @brief NvAPIδ�����Ĳ�ѯ��ѹ�Ľӿ�
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
