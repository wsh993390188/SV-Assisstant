#pragma once
namespace Hardware
{
	namespace Battery
	{
		/// @brief 电池的单位
		enum class BatteryUnit
		{
			mW,///<毫瓦
			W, ///< 瓦
			PerHour,///< 每小时充电速率，仅用于Rate
			Percent,///< 百分比，仅用于电池容量
		};

		/// @brief 带单位的电池信息
		struct BatteryCapacity
		{
			std::string Value;///< 电池的信息
			BatteryUnit Unit;///< 电池单位 @ref BatteryUnit

			/// @brief bool对象转换函数，仅匹配值是否存在
			explicit operator bool() const
			{
				return !Value.empty();
			}

			std::string Tou8String() const;
		};

		/// @brief 电池通用数据
		struct BatteryCommonStruct
		{
			std::string	Chemistry;				///<化学成分
			BatteryCapacity	DesignCapacity;		///<设计容量
			BatteryCapacity	FullChargedCapacity;///<充电的满容量
			BatteryCapacity BatteryDepletion;	///<电池损耗百分比
			BatteryCapacity	DefaultAlert1;		///<默认警告1
			BatteryCapacity	DefaultAlert2;		///<默认警告2
			std::string ManufactureDate;		///<制造日期
			std::string	Manufacture;			///<制造商
			std::string	SerialNumber;			///<序列号
			std::string	UniqueID;				///<唯一ID 序列号
			std::string	PowerState;             ///<1电源连接；2放电；3充电；4电压在极限；5 电源连接在充电
			BatteryCapacity	Capacity;           ///<实时容量
			std::string	Voltage;				///<电压
			BatteryCapacity		Rate;			///<充电放电速度  整数是充电，负数是充电，0是没放电也没充电
			std::string Percentage;				///<电池实时容量百分比
			std::string Temperature;			///<电池的温度
			std::string Technology;				///<电池的技术
			std::string CycleCount;				///<充电周期次数， 需要电池支持
		};

		/// @brief 电池元素的基础类
		class BatteryElement
		{
		public:
			/// @brief 更新电池信息
			/// @param[in] BatterySlotId 电池Id
			/// @param[out] BatteryInfo 电池信息
			/// @return 是否成功
			virtual bool Update(const uint32_t& BatterySlotId, BatteryCommonStruct& BatteryInfo) = 0;

			/// @brief 虚析构函数
			virtual ~BatteryElement() = default;

			/// @brief 将电池信息转换成Json字符串
			/// @param[in] BatteryInfo 电池信息
			/// @return Json字符串
			virtual std::string BuildBatteryElementToJsonString(const BatteryCommonStruct& BatteryInfo);
		};

		/// @brief 电池元素工厂类
		class BatteryElementFactory final
		{
			/// @brief 默认构造函数
			BatteryElementFactory() = default;
		public:
			/// @brief 单例函数
			/// @return 单例对象
			static BatteryElementFactory& Instance();
			/// @brief 创建电池
			/// @param[in] DevicePath 电池的设备路径
			/// @return 电池元素 @ref BatteryElement
			std::shared_ptr<BatteryElement> CreateBattery(const std::wstring& DevicePath);
		};
	}
}
