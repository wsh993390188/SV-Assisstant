#pragma once
namespace Hardware
{
	namespace Battery
	{
		/// @brief ��صĵ�λ
		enum class BatteryUnit
		{
			mW,///<����
			W, ///< ��
			PerHour,///< ÿСʱ������ʣ�������Rate
			Percent,///< �ٷֱȣ������ڵ������
		};

		/// @brief ����λ�ĵ����Ϣ
		struct BatteryCapacity
		{
			std::string Value;///< ��ص���Ϣ
			BatteryUnit Unit;///< ��ص�λ @ref BatteryUnit

			/// @brief bool����ת����������ƥ��ֵ�Ƿ����
			explicit operator bool() const
			{
				return !Value.empty();
			}

			std::string Tou8String() const;
		};

		/// @brief ���ͨ������
		struct BatteryCommonStruct
		{
			std::string	Chemistry;				///<��ѧ�ɷ�
			BatteryCapacity	DesignCapacity;		///<�������
			BatteryCapacity	FullChargedCapacity;///<����������
			BatteryCapacity BatteryDepletion;	///<�����İٷֱ�
			BatteryCapacity	DefaultAlert1;		///<Ĭ�Ͼ���1
			BatteryCapacity	DefaultAlert2;		///<Ĭ�Ͼ���2
			std::string ManufactureDate;		///<��������
			std::string	Manufacture;			///<������
			std::string	SerialNumber;			///<���к�
			std::string	UniqueID;				///<ΨһID ���к�
			std::string	PowerState;             ///<1��Դ���ӣ�2�ŵ磻3��磻4��ѹ�ڼ��ޣ�5 ��Դ�����ڳ��
			BatteryCapacity	Capacity;           ///<ʵʱ����
			std::string	Voltage;				///<��ѹ
			BatteryCapacity		Rate;			///<���ŵ��ٶ�  �����ǳ�磬�����ǳ�磬0��û�ŵ�Ҳû���
			std::string Percentage;				///<���ʵʱ�����ٷֱ�
			std::string Temperature;			///<��ص��¶�
			std::string Technology;				///<��صļ���
			std::string CycleCount;				///<������ڴ����� ��Ҫ���֧��
		};

		/// @brief ���Ԫ�صĻ�����
		class BatteryElement
		{
		public:
			/// @brief ���µ����Ϣ
			/// @param[in] BatterySlotId ���Id
			/// @param[out] BatteryInfo �����Ϣ
			/// @return �Ƿ�ɹ�
			virtual bool Update(const uint32_t& BatterySlotId, BatteryCommonStruct& BatteryInfo) = 0;

			/// @brief ����������
			virtual ~BatteryElement() = default;

			/// @brief �������Ϣת����Json�ַ���
			/// @param[in] BatteryInfo �����Ϣ
			/// @return Json�ַ���
			virtual std::string BuildBatteryElementToJsonString(const BatteryCommonStruct& BatteryInfo);
		};

		/// @brief ���Ԫ�ع�����
		class BatteryElementFactory final
		{
			/// @brief Ĭ�Ϲ��캯��
			BatteryElementFactory() = default;
		public:
			/// @brief ��������
			/// @return ��������
			static BatteryElementFactory& Instance();
			/// @brief �������
			/// @param[in] DevicePath ��ص��豸·��
			/// @return ���Ԫ�� @ref BatteryElement
			std::shared_ptr<BatteryElement> CreateBattery(const std::wstring& DevicePath);
		};
	}
}
