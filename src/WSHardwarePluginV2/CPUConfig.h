#pragma once
#include <mutex>
namespace Hardware
{
	namespace CPU
	{
		struct CPUExtendedInfo
		{
			std::string Element;
			bool Priority = true;
		};
		/// @brief 从配置中获取CPU信息
		struct CPUExtendedInfoFromCPUDB
		{
			CPUExtendedInfo CodeName; ///< cpu代号
			CPUExtendedInfo SocketPackage; ///< cpu封装leixing
			CPUExtendedInfo MaxTDP;///<cpu的标准最大的TDP
			CPUExtendedInfo Technology;///<cpu的工艺
			explicit operator bool() const;
		};

		/// @brief CPU配置文件序列化类
		class CPUDB final
		{
		private:
			/// @brief cpudb中的信息序列化
			struct CpuInformationFromCpudb
			{
				/// @brief 从CPUDB中获取的cpu信息, First是CPU系列 Second是CPU扩展信息
				std::map<std::string, CPUExtendedInfoFromCPUDB> Element;
				explicit operator bool() const;
			};

			/// @brief cpudb中的信息序列化
			struct CpuFamily
			{
				/// @brief 从CPUDB中获取的cpu信息, First是CPU的家族 Second是CPU扩展信息
				std::map<std::string, CpuInformationFromCpudb> Element;
				explicit operator bool() const;
			};
			/// @brief 存储外部信息的类
			/// First是CPU的厂商，Second是CPU的家族信息
			std::map<std::string, CpuFamily> m_CPUDB;

			/// @brief XML默认版本号
			std::string m_DefaultVersion;

			/// @brief 配置文件更新的锁
			mutable std::mutex m_Mutex;
		public:
			/// @brief CPU序列化类的类型
			using CPUDBType = decltype(m_CPUDB);

			/// @brief 从CPUDB中查询CPU信息
			struct CPUQueryInfo
			{
				std::string CpuManufacture; ///<cpu厂商
				std::string CpuFamily; ///<cpu家族
				std::string CpuModel; ///<cpu类型
				explicit operator bool() const;
			};
			/// @brief CPU序列化单例
			/// @return 单例引用 @ref CPUDB
			static CPUDB& Instance();

			/// @brief 初始化CPU配置文件
			void Initialize();

			/// @brief 根据CPU的特征寻找部分信息
			/// @param CpuManufacture CPU的厂商
			/// @param CpuFamily CPU的家族
			/// @param CpuModel CPU的型号
			/// @return Cpu额外信息的指针
			std::unique_ptr<CPUExtendedInfoFromCPUDB> FindElements(const CPUQueryInfo& QueryInfo) const;

			/// @brief 获取序列化后的数据
			/// @return 序列化后的数据
			const CPUDBType& GetCpuData()const;
		private:
			/// @brief 默认构造，禁止外部访问构造函数
			CPUDB() = default;
			/// @brief 读取XML配置文件中的信息
			/// @param fileName 加密后文件名
			/// @param localConfigName 本地未加密文件名
			/// @param configcontent XML配置文件中二进制数据
			/// @return 读取配置文件是否成功
			bool GetConfigXml(const std::wstring& fileName, const std::wstring& localConfigName, std::string& configcontent);

			/// @brief 解析Manufacture字段
			/// @param ManufactureElement 字段元素
			/// @param CpuDB 序列化类
			void ParserManufacture(tinyxml2::XMLElement const* const ManufactureElement, CPUDBType& CpuDB);

			/// @brief 解析Family字段
			/// @param FamilyElement 字段元素
			/// @param FamilyData 序列化类
			/// @return 错误类型 @ref Data::ErrorType
			Data::ErrorType ParserCpuFamily(tinyxml2::XMLElement const* const FamilyElement, CpuFamily& FamilyData);

			/// @brief 解析CPU字段
			/// @param CpuElement 字段元素
			/// @param CpuData 序列化类
			/// @return 错误类型 @ref Data::ErrorType
			Data::ErrorType ParserCpuInformaion(tinyxml2::XMLElement const* const CpuElement, CpuInformationFromCpudb& CpuData);

		private:
			/// @brief 判断测试配置文件是否存在,不调用共用库方法，自行实现
			/// @return 测试配置文件是否存在
			bool TestConfigIsExist();
		};
	}
}
