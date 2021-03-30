#pragma once
#include "..\..\Libcommon\adlsdk\include\adl_sdk.h"
namespace Hardware
{
	namespace GPU
	{
		namespace ADLSDK
		{
			/// @brief OverDrive�汾
			enum class OverDriveVersion
			{
				NotSupport,///<��֧��OverDrive
				OverDrive5,///< Version 5
				OverDrive6,///< Version 6
				OverDrive8,///< Version 8
				OverDriveN,///< Next Version
			};

			/// @brief OD֧�ֵĹ���
			struct ODSupport
			{
				bool Temperature = false;///<�¶�
				bool FanSpeed = false;///<����ת��
				bool Power = false;///<����
			};

			/// @brief OD�Ļ���
			class Overdrive
			{
			public:
				/// @brief OD����������
				virtual ~Overdrive() = default;

				/// @brief ��ȡ�Կ��¶���Ϣ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @param hModule ADL��ģ���ַ
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �¶�
				virtual std::string GetTemperature(const uint32_t& AdapterId) = 0;

				/// @brief ��ȡ�Կ�����ת����Ϣ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return ����ת��
				virtual std::string GetFanSpeed(const uint32_t& AdapterId) = 0;

				/// @brief ��ȡ�Դ�Ƶ����Ϣ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �Դ�Ƶ��
				virtual std::string GetGPUMemoryClock(const uint32_t& AdapterId) = 0;

				/// @brief ��ȡ�Կ�����Ƶ����Ϣ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �Կ�����Ƶ��
				virtual std::string GetGPUCoreClock(const uint32_t& AdapterId) = 0;

				/// @brief ��ȡ�Կ���ѹ��Ϣ
				/// @param AdapterId Ŀ���豸��AdapterID
				/// @throw �����׳������쳣
				///	- @ref std::exception ��ȡʧ��
				/// @return �Կ���ѹ
				virtual std::string GetGPUVoltage(const uint32_t& AdapterId) = 0;
			};
		}
	}
}
