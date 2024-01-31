#pragma once
#include <Batclass.h>
#include "BatteryElement.h"
namespace Hardware
{
	namespace Battery
	{
		/// @brief 电池信息获取函数
		class BatteryInforamtion : public BatteryElement
		{
		public:
			/// @brief 构造函数
			/// @param[in] DevicePath 设备路径
			explicit BatteryInforamtion(const std::wstring& DevicePath);

			/// @brief 更新电池信息
			/// @param[in] BatterySlotId
			/// @param[out] BatteryInfo
			/// @return 是否成功
			bool Update(const uint32_t& BatterySlotId, BatteryCommonStruct& BatteryInfo) override;
		private:
			/// @brief 获取一个新的Tag
			/// @return
			ULONG GetNewTag();

			/// @brief 获取电池相关信息
			/// @param[in] batQueryInfo 输入查询信息结构
			/// @param[out] csOutStr 输出相关信息字符串
			/// @return 是否成功
			bool QueryString(BATTERY_QUERY_INFORMATION& batQueryInfo, std::string& csOutStr);

			/// @brief 获取电池相关信息
			/// @param[out] BatteryInfo 电池信息
			/// @return 是否成功
			bool QueryBatteryInfo(BATTERY_INFORMATION& BatteryInfo);

			/// @brief 获取电池生产日期
			/// @param[out] BatteryDate 电池生产日期
			/// @return 是否成功
			bool QueryBatteryManufactureDate(BATTERY_MANUFACTURE_DATE& BatteryDate);

			/// @brief 获取电池的状态
			/// @param[out] BatteryStatus 电池的状态
			/// @return 是否成功
			bool QueryBatteryStatus(BATTERY_STATUS& BatteryStatus);

			/// @brief 获取电池的温度信息
			/// @param[out] Temperature 输出电池温度
			/// @return 是否成功
			bool QueryBatteryTemperature(std::string& Temperature);

			/// @brief 获取电池的当前剩余电量信息
			/// @param[out] CurrentPercent 输出当前剩余电量
			/// @return 是否成功
			bool QuertBatteryCurrentPercent(std::string& CurrentPercent);
		private:
			/// @brief 序列化电池状态
			/// @param[in] BatteryStatus 电池的状态
			/// @return 序列化后的电池状态
			std::string BatteryPowerStatusToString(const BATTERY_STATUS& BatteryStatus);

			/// @brief 序列化电池电压值
			/// @param[in] BatteryStatus 电池的状态信息
			/// @return 电压值
			std::string BatteryVoltageToString(const BATTERY_STATUS& BatteryStatus);

			/// @brief 序列化电池容量值
			/// @param[in] CapacityValue 电池容量值
			/// @param[in] Unit 电池状态的单位
			/// @return 容量值 @ref BatteryCapacity
			BatteryCapacity BatteryCapacityToObject(const ULONG& CapacityValue, const ULONG& Unit);

			/// @brief 序列化电池当前速率
			/// @param[in] RateValue 电池速率信息
			/// @param[in] Unit 电池状态的单位
			/// @return 速率信息 @ref BatteryCapacity
			BatteryCapacity BatteryRateToObject(const ULONG& RateValue, const ULONG& Unit);

			/// @brief 电池的化成成分转String
			/// @param[in] Chemistry 化学成分简称
			/// @return 化学成分全称
			std::string BatteryChemistryToString(const std::string& Chemistry);

			/// @brief 电池的技术转换成字符串
			/// @param[in] Technology 技术的数值
			/// @return 技术的字符串
			std::string BatteryTechnologyToString(const UCHAR& Technology);

			/// @brief 计算电池损耗
			/// @param[in] FullCapacity 当前满电容量
			/// @param[in] DesignCapacity 设计容量
			/// @return 损耗百分比 @ref BatteryCapacity
			BatteryCapacity BatteryCalcDepletion(const ULONG& FullCapacity, const ULONG& DesignCapacity);
		private:
			/// @brief 电池设备对象句柄
			Utils::HANDLE_unique_ptr BatteryHandle;
			/// @brief 最新电池的Tag
			ULONG CurrentBatTag;
		};
	}
}
