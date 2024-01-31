/*!
* @file GenericCPU.h
* @brief CPU通用接口
*
* @author 王硕(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020年6月5日
*/
#pragma once
#include "Data.h"
#include "CPUConfig.h"
#include "msr.h"
#include "tsc.h"
namespace Hardware
{
	namespace CPU
	{
		/// @brief CPU装饰基类，用来阐述当前CPU的特性
		class CPUDecorator
		{
		public:
			CPUDecorator() = delete;
			/// @brief 初始化函数，初始化Msr寄存器，当前修饰名
			/// @param[in] MsrRegistry Msr寄存器
			/// @param[in] Name 当前名称
			/// @return
			explicit CPUDecorator(const uint64_t MsrRegistry, const std::string& Name);
			/// @brief 析构函数，使用默认选项
			virtual ~CPUDecorator() = default;
			/// @brief 获取装饰名称
			/// @return 装饰名称
			const std::string GetDecoratorName() const;

			/// @brief 获取装饰信息
			/// @return 装饰信息
			virtual const std::string GetDecoratorValue() const = 0;

			/// @brief 更新信息
			virtual void Update(std::weak_ptr<Utils::Ring0::SafeMsrHandle> Msr) = 0;
		protected:
			const uint64_t m_MsrRegistry;
			const std::string m_Name;
			mutable bool IsUpdate;
		};

#define CPUID_Stepping						0x0000000F
#define CPUID_Model							0x000000F0
#define CPUID_Family						0x00000F00
#define CPUID_Processor_Type				0x00003000
#define CPUID_Extended_Model				0x000F0000
#define CPUID_Extended_Family				0x0FF00000

		/// @brief CPUJson的信息
		struct CPUJsonInfo
		{
			/// @brief Socket Id
			int32_t SocketId;
			/// @brief Core Id
			int32_t CoreId;
			/// @brief Thread Id
			int32_t ThreadId;
			CPUJsonInfo() : SocketId(-1), CoreId(-1), ThreadId(-1) {}
		};
		/// @brief Cache信息
		struct Cache_info
		{
			char Cache_type;///< Cache类型
			unsigned int Cache_level;///< Cache级别
			unsigned int NumberOfCache;///< Cache数量
			unsigned int Cache_Ways;
			unsigned int Cache_line_size;
			unsigned int Cache_partitions;
			unsigned int Cache_Set;
			unsigned int Cache_Size;///< Cache大小
		};

		/// @brief CPU通用信息，来源自CPUZ界面
		struct CPUCommonInformation
		{
			/// @brief CPU显示名称
			std::string Name;
			std::string CodeName; ///< cpu代号
			std::string SocketPackage; ///< cpu封装leixing
			std::string MaxTDP;///<cpu的标准最大的TDP
			std::string Technology;///<cpu的工艺
			/// @brief CPU支持的特性
			std::string Instructions;
			/// @brief CPU系列
			uint32_t Family;
			/// @brief CPU扩展系列
			uint32_t ExtFamily;
			/// @brief CPU型号
			uint32_t Model;
			/// @brief CPU扩展型号
			uint32_t ExtModel;
			/// @brief CPU步进
			uint32_t Stepping;
			/// @brief Cache信息
			std::vector<Cache_info> Cache;
			/// @brief 修订后的Family
			uint32_t DisplayFamily;
			/// @brief 修订后的Model
			uint32_t DisplayModel;
			/// @brief CPU修订版本
			/// 注释原因，不知道如何获取
			//uint32_t Revision;
		};

		/// @brief 超线程数据类
		class HyperThread final
		{
		public:
			/// @brief 配置超线程数据
			/// @param[in] ThreadID 本超线程ID
			/// @param[in] OsID OS层ID
			/// @param[in] CoreID Core ID,用来创建Msr Handle
			HyperThread(const std::int32_t ThreadID, const int32_t OsID, const int32_t CoreID);

			/// @brief 更新数据
			void Update();

			/// @brief 构建更新数据Json
			/// @return Json数据
			Json::Value BuildJson();

			/// @brief 取得Msr操作权限
			/// @return Msr指针
			std::weak_ptr<Utils::Ring0::SafeMsrHandle> GetMsr();

			/// @brief 获取Os Id
			/// @return osID
			const int32_t osID() const;
			/// @brief 获取Thread Id
			/// @return ThreadID
			const int32_t ThreadID() const;

			/// @brief 加入Decorator
			/// @param[in] Decorator 动态数据类型
			void AddDecorator(std::unique_ptr<CPUDecorator> Decorator);
		private:
			/// @brief Ring0级调用本线程Msr寄存器指针
			std::shared_ptr<Utils::Ring0::SafeMsrHandle> m_MsrHandle;

			/// @brief CPU 动态信息
			std::vector<std::shared_ptr<CPUDecorator>> m_Decorators;
			/// @brief Thread Id
			int32_t m_ThreadID;
			/// @brief OS 层面上的ID
			int32_t m_OsID;
		};

		/// @brief 物理核心数据类
		class Core final
		{
		public:
			/// @brief 配置物理核心
			/// @param[in] CoreID 核心ID
			/// @param[in] TileID Tile ID 用于Socket查找
			/// @param[in] SocketID 所属Socket ID
			Core(const int32_t CoreID, const int32_t TileID, const int32_t SocketID);

			/// @brief 增加一个超线程
			/// @param[in] ThreadID 超线程的ID
			/// @param[in] OsID 超线程的OS层ID
			void AddThread(const int32_t ThreadID, const int32_t OsID);

			/// @brief 通过OsId查找超线程
			/// @param[in] OsID OsId
			/// @return 超线程指针
			std::weak_ptr < HyperThread> findThreadByOSID(const int32_t osID);

			/// @brief 获取超线程
			/// @return 超线程数组
			std::vector<std::shared_ptr<HyperThread>>& Threads();

			/// @brief 获取Core ID
			/// @return CoreID
			const int32_t CoreID() const;

			/// @brief 获取Tile ID
			/// @return TileID
			const int32_t TileID() const;

			/// @brief 获取Socket ID
			/// @return SocketID
			const int32_t SocketID() const;
		private:
			/// @brief 超线程存储数据
			std::vector<std::shared_ptr<HyperThread>> m_Thread;
			/// @brief Core Id
			int32_t m_CoreID;
			/// @brief 用于Socket查找Core的Id
			int32_t m_TileID;
			/// @brief 上层SocketId
			int32_t m_SocketID;
		};

		/// @brief 系统中存在的Socket
		class Socket final
		{
		public:
			/// @brief 配置Id
			/// @param[in] ApicID Apic ID
			/// @param[in] LogicalID Socket ID
			/// @return
			Socket(const int32_t ApicID, const int32_t LogicalID);

			/// @brief 采用默认方式析构
			~Socket() = default;

			/// @brief 增加一个Core
			/// @param[in] CoreID
			/// @param[in] TileID
			/// @param[in] SocketID
			void AddCore(const int32_t CoreID, const int32_t TileID, const int32_t SocketID);

			/// @brief 增加一个Core
			/// @param[in] core Core可移动副本
			void AddCore(Core&& core);

			/// @brief 加入Decorator
			/// @param[in] Decorator 动态数据类型
			void AddDecorator(std::unique_ptr<CPUDecorator> Decorator);

			/// @brief 通过OsId查找超线程
			/// @param[in] OsID OsId
			/// @return 超线程指针
			std::weak_ptr<HyperThread> findThreadByOSID(const int32_t OsID);

			/// @brief 通过TileID查找物理核心
			/// @param[in] tileID
			/// @return 物理核心指针
			std::weak_ptr<Core> findCoreByTileID(const int32_t tileID);
			/// @brief 获取ApicId
			/// @return ApicId
			const int32_t ApicId() const;

			/// @brief 构建更新数据Json
			/// @return Json数据
			Json::Value BuildJson();

			/// @brief 更新数据
			void Update();

			/// @brief 获取物理核心
			/// @return 物理核心数组
			std::vector<std::shared_ptr<Core>>& Cores();

			/// @brief 获取物理核心,Const函数
			/// @return 物理核心数组
			const std::vector<std::shared_ptr<Core>>& Cores() const;
			/// @brief 获取SocketId
			/// @return SocketId
			const int32_t SocketId() const;
		public:
			/// @brief CPU信息，采用CPUZ的信息
			CPUCommonInformation m_Data;
		private:
			/// @brief CPU 动态信息
			std::vector<std::shared_ptr<CPUDecorator>> m_Decorators;

			/// @brief 物理核存储数据
			std::vector<std::shared_ptr<Core>> m_Core;
			/// @brief Apic id
			int32_t   m_ApicID;
			/// @brief  Socket Id
			int32_t   m_LogicalID;
		};

		/// @brief 核心描述符用来查找CPU核心or线程
		struct TopologyEntry // decribes a core
		{
			/// @brief 系统线程Id
			int32_t os_id;
			/// @brief 线程Id
			int32_t thread_id;
			/// @brief 核心Id
			int32_t core_id;
			/// @brief 核心查找Id
			int32_t tile_id; // tile is a constalation of 1 or more cores sharing salem L2 cache. Unique for entire system
			/// @brief Socket Id
			int32_t socket;
			TopologyEntry() : os_id(-1), thread_id(-1), core_id(-1), tile_id(-1), socket(-1) { }
		};

		/// @brief 通用CPU
		class GenericCPU
		{
		public:
			/// @brief 构造函数，用于初始化CPU信息
			GenericCPU();

			/// @brief 虚析构函数
			virtual ~GenericCPU() = default;

			/// @brief 初始化CPU
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief 更新CPU信息
			/// @param[in] Args Json数据
			/// @param[out] response 回应的Json数据
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief 获取CPU基础操作
			/// Json格式 {"SocketId": 0}
			/// @param[in] paramter JSON参数
			/// @param[out] response JSON回应
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief 增加一个Socket
			/// @param[in] ApicID Apic Id
			/// @param[in] LogicalID Logical Id
			void AddSocket(const int32_t ApicID, const int32_t LogicalID);

			/// @brief 增加一个Socket
			/// @param[in] soc Socket可移动副本d
			void AddSocket(Socket&& soc);

			/// @brief 解析更新操作的Json
			/// @param[in] paramter Json数据
			/// @return 解析后的操作数组
			std::vector<CPUJsonInfo> PaserUpdateJson(const std::string& paramter);

			/// @brief 构建初始化JSON数据
			/// @return Utf-8格式的JSON数据
			std::string BuildInitializeJson();

			/// @brief 返回每个soc的静态信息
			/// @param Element JSon对象
			/// @param soc soc的数据结构
			/// @return Utf-8格式的JSON数据
			std::string BuildInitializeStaticJson(const Socket& soc);

			/// @brief 根据系统API更新Cache的数量
			/// @param l1Data L1Cache数据缓存数量
			/// @param l1Inst L1Cache指令缓存数量
			/// @param l2 L2缓存数量
			/// @param l3 L3缓存数量
			void UpdateCacheNumber(int& l1Data, int& l1Inst, int& l2, int& l3);
		protected:
			/// @brief 存储CPU信息的类
			/// First是Socket ApicID, Second是Socket数据
			std::map<uint32_t, Socket> m_SystemRoot;
		private:
		};
	}
}
