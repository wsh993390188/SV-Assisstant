#pragma once
#include "..\..\Libcommon\adlsdk\include\adl_sdk.h"
#include "Overdrive.h"
namespace Hardware
{
	namespace GPU
	{
		/// @brief ADL�豸�����ڵ��쳣
		class ADLDeviceNotExist : public std::exception
		{
		public:
			/// @brief ��ʼ���쳣
			/// @param DeviceId �豸ID
			explicit ADLDeviceNotExist(const uint32_t& DeviceId) {
				Msg = "ADL Cant find gpu " + std::to_string(DeviceId);
			}
			/// @brief �쳣˵��
			/// @return �쳣˵��
			char const* what() const override;
		private:
			/// @brief �쳣��Ϣ
			std::string Msg;
		};

		/// @brief ADL�����ɹ��İ�װ
		/// @param status ADL�ĺ���ִ�н��
		/// @return �Ƿ�ִ�гɹ�
		bool ADL_Success(const int& status);

		/// @brief ADLִ��״̬������ַ�����ʽ
		/// @param status ADL����ִ�н��
		/// @return ִ�н�����ַ�����ʽ
		std::string GetADLStatus(const int& status);

		/// @brief adl sdk�İ�������
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

			/// @brief ADL ��ѯ�����ݽṹ
			struct ADLQueryData
			{
				/// @brief The BUS number
				uint32_t BusNumber;
				/// @brief The Device number
				uint32_t DeviceNumber;
				/// @brief The function number.
				uint32_t FunctionNumber;
				/// @brief �Կ���ADL�е�����
				int AdapterIndex;
				/// @brief �豸ID
				uint32_t DeviceId;
				/// @brief �Կ�����
				std::string FullName;
				/// @brief ADL���Կ�Ψһ��ʶ
				std::string UDID;
				/// @brief OD�İ汾
				std::unique_ptr<ADLSDK::Overdrive> OD;
			};
		public:

			/// @brief ADLģ��ĵ���ģʽ
			/// @return ����ģʽ @ref ADLSDKHelper
			static ADLSDKHelper& Instance();

			/// @brief ��ȡADLģ��ָ��
			/// @return ADLģ��ָ��
			HMODULE GetADLModule() const;

			/// @brief ��ȡ�Կ��¶���Ϣ
			/// @param DeviceId Ŀ���豸��ID
			/// @throw �����׳������쳣
			///	- @ref ADLDeviceNotExist�豸������
			///	- @ref std::exception ��ȡʧ��
			/// @return �¶�
			std::string GetTemperature(const uint32_t& DeviceId);

			/// @brief ��ȡ�Կ�����ת����Ϣ
			/// @param DeviceId Ŀ���豸��ID
			/// @throw �����׳������쳣
			///	- @ref ADLDeviceNotExist�豸������
			///	- @ref std::exception ��ȡʧ��
			/// @return ����ת��
			std::string GetFanSpeed(const uint32_t& DeviceId);

			/// @brief ��ȡ�Դ�Ƶ����Ϣ
			/// @param AdapterId Ŀ���豸��AdapterID
			/// @throw �����׳������쳣
			///	- @ref ADLDeviceNotExist�豸������
			///	- @ref std::exception ��ȡʧ��
			/// @return �Դ�Ƶ��
			std::string GetGPUMemoryClock(const uint32_t& DeviceId);

			/// @brief ��ȡ�Կ�����Ƶ����Ϣ
			/// @param AdapterId Ŀ���豸��AdapterID
			/// @throw �����׳������쳣
			///	- @ref ADLDeviceNotExist�豸������
			///	- @ref std::exception ��ȡʧ��
			/// @return �Կ�����Ƶ��
			std::string GetGPUCoreClock(const uint32_t& DeviceId);

			/// @brief ��ȡ�Կ��ĵĵ�ѹ
			/// @param DeviceId �Կ����豸ID
			/// @throw �����׳������쳣
			///	- @ref ADLDeviceNotExist�豸������
			///	- @ref std::exception ��ȡʧ��
			/// @return �Կ���ѹ
			std::string GetGPUVoltage(const uint32_t& DeviceId);

			/// @brief ��ȡ�Կ�������
			/// @param DeviceId �Կ����豸ID
			/// @throw �����׳������쳣
			/// - @ref ADLDeviceNotExist�豸������
			/// @return �Կ�����
			std::string GetGPUName(const uint32_t& DeviceId);

			/// @brief ��ȡ�Կ������汾
			/// @param DeviceId �Կ����豸ID
			/// @throw �����׳������쳣
			/// - @ref ADLDeviceNotExist�豸������
			/// @return �Կ������汾
			std::string GetGPUDriverVersion(const uint32_t& DeviceId);

			/// @brief ��ȡ�Կ���BIOS�汾
			/// @param DeviceId �Կ����豸ID
			/// @throw �����׳������쳣
			/// - @ref ADLDeviceNotExist�豸������
			/// @return BIOS�汾
			std::string GetGPUBIOSVersion(const uint32_t& DeviceId);

			/// @brief ��ȡ�Կ���BIOS����
			/// @param DeviceId �Կ����豸ID
			/// @throw �����׳������쳣
			/// - @ref ADLDeviceNotExist�豸������
			/// @return BIOS����
			std::string GetGPUBIOSDate(const uint32_t& DeviceId);
		private:
			/// @brief ö�����л��A��
			/// @return �Ƿ����A��
			bool EnumAdapter();

			/// @brief �����豸�Ƿ��Ѿ��ҵ�
			/// @param BusNumber The BUS number
			/// @param DevNumber The Device number
			/// @param FuncNumber The function number
			/// @return ����ҵ���Ϊtrue
			bool FindDeviceOnQueryDatas(const uint32_t& BusNumber, const uint32_t& DevNumber, const uint32_t& FuncNumber);

			/// @brief ���ADL Overdrive�İ汾
			/// @param AdapterIndex ������������
			/// @return @ref OverDriveVersion
			ADLSDK::OverDriveVersion CheckOverDriveSupportVersion(const int& AdapterIndex);

		private:
			/// @brief ���������ĳ�ʼ������
			ADLSDKHelper();

			/// @brief ������������������
			~ADLSDKHelper();

			/// @brief adl��ģ��ָ��
			HMODULE hModule;

			/// @brief ADL�������Ļ���
			ADL_CONTEXT_HANDLE context;

			/// @brief ADL�Ƿ����
			bool IsADLEnable;

			/// @brief GPU�Ĳ�ѯƥ����Ϣ
			std::vector<ADLQueryData> GpuQueryDatas;

			/// @brief ��ȡADL���Կ�BIOS��Ϣ
			ADL_ADAPTER_VIDEOBIOSINFO_GET ADL_Adapter_VideoBiosInfo_Get;
		};
	}
}
