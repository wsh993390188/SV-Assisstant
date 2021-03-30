#pragma once
#include <Batclass.h>
#include "BatteryElement.h"
namespace Hardware
{
	namespace Battery
	{
		/// @brief �����Ϣ��ȡ����
		class BatteryInforamtion : public BatteryElement
		{
		public:
			/// @brief ���캯��
			/// @param[in] DevicePath �豸·��
			explicit BatteryInforamtion(const std::wstring& DevicePath);

			/// @brief ���µ����Ϣ
			/// @param[in] BatterySlotId
			/// @param[out] BatteryInfo
			/// @return �Ƿ�ɹ�
			bool Update(const uint32_t& BatterySlotId, BatteryCommonStruct& BatteryInfo) override;
		private:
			/// @brief ��ȡһ���µ�Tag
			/// @return
			ULONG GetNewTag();

			/// @brief ��ȡ��������Ϣ
			/// @param[in] batQueryInfo �����ѯ��Ϣ�ṹ
			/// @param[out] csOutStr ��������Ϣ�ַ���
			/// @return �Ƿ�ɹ�
			bool QueryString(BATTERY_QUERY_INFORMATION& batQueryInfo, std::string& csOutStr);

			/// @brief ��ȡ��������Ϣ
			/// @param[out] BatteryInfo �����Ϣ
			/// @return �Ƿ�ɹ�
			bool QueryBatteryInfo(BATTERY_INFORMATION& BatteryInfo);

			/// @brief ��ȡ�����������
			/// @param[out] BatteryDate �����������
			/// @return �Ƿ�ɹ�
			bool QueryBatteryManufactureDate(BATTERY_MANUFACTURE_DATE& BatteryDate);

			/// @brief ��ȡ��ص�״̬
			/// @param[out] BatteryStatus ��ص�״̬
			/// @return �Ƿ�ɹ�
			bool QueryBatteryStatus(BATTERY_STATUS& BatteryStatus);

			/// @brief ��ȡ��ص��¶���Ϣ
			/// @param[out] Temperature �������¶�
			/// @return �Ƿ�ɹ�
			bool QueryBatteryTemperature(std::string& Temperature);

			/// @brief ��ȡ��صĵ�ǰʣ�������Ϣ
			/// @param[out] CurrentPercent �����ǰʣ�����
			/// @return �Ƿ�ɹ�
			bool QuertBatteryCurrentPercent(std::string& CurrentPercent);
		private:
			/// @brief ���л����״̬
			/// @param[in] BatteryStatus ��ص�״̬
			/// @return ���л���ĵ��״̬
			std::string BatteryPowerStatusToString(const BATTERY_STATUS& BatteryStatus);

			/// @brief ���л���ص�ѹֵ
			/// @param[in] BatteryStatus ��ص�״̬��Ϣ
			/// @return ��ѹֵ
			std::string BatteryVoltageToString(const BATTERY_STATUS& BatteryStatus);

			/// @brief ���л��������ֵ
			/// @param[in] CapacityValue �������ֵ
			/// @param[in] Unit ���״̬�ĵ�λ
			/// @return ����ֵ @ref BatteryCapacity
			BatteryCapacity BatteryCapacityToObject(const ULONG& CapacityValue, const ULONG& Unit);

			/// @brief ���л���ص�ǰ����
			/// @param[in] RateValue ���������Ϣ
			/// @param[in] Unit ���״̬�ĵ�λ
			/// @return ������Ϣ @ref BatteryCapacity
			BatteryCapacity BatteryRateToObject(const ULONG& RateValue, const ULONG& Unit);

			/// @brief ��صĻ��ɳɷ�תString
			/// @param[in] Chemistry ��ѧ�ɷּ��
			/// @return ��ѧ�ɷ�ȫ��
			std::string BatteryChemistryToString(const std::string& Chemistry);

			/// @brief ��صļ���ת�����ַ���
			/// @param[in] Technology ��������ֵ
			/// @return �������ַ���
			std::string BatteryTechnologyToString(const UCHAR& Technology);

			/// @brief ���������
			/// @param[in] FullCapacity ��ǰ��������
			/// @param[in] DesignCapacity �������
			/// @return ��İٷֱ� @ref BatteryCapacity
			BatteryCapacity BatteryCalcDepletion(const ULONG& FullCapacity, const ULONG& DesignCapacity);
		private:
			/// @brief ����豸������
			Utils::HANDLE_unique_ptr BatteryHandle;
			/// @brief ���µ�ص�Tag
			ULONG CurrentBatTag;
		};
	}
}
