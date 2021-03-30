/*!
* @file GenericCPU.h
* @brief CPUͨ�ýӿ�
*
* @author ��˶(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020��6��5��
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
		/// @brief CPUװ�λ��࣬����������ǰCPU������
		class CPUDecorator
		{
		public:
			CPUDecorator() = delete;
			/// @brief ��ʼ����������ʼ��Msr�Ĵ�������ǰ������
			/// @param[in] MsrRegistry Msr�Ĵ���
			/// @param[in] Name ��ǰ����
			/// @return
			explicit CPUDecorator(const uint64_t MsrRegistry, const std::string& Name);
			/// @brief ����������ʹ��Ĭ��ѡ��
			virtual ~CPUDecorator() = default;
			/// @brief ��ȡװ������
			/// @return װ������
			const std::string GetDecoratorName() const;

			/// @brief ��ȡװ����Ϣ
			/// @return װ����Ϣ
			virtual const std::string GetDecoratorValue() const = 0;

			/// @brief ������Ϣ
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

		/// @brief CPUJson����Ϣ
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
		/// @brief Cache��Ϣ
		struct Cache_info
		{
			char Cache_type;///< Cache����
			unsigned int Cache_level;///< Cache����
			unsigned int NumberOfCache;///< Cache����
			unsigned int Cache_Ways;
			unsigned int Cache_line_size;
			unsigned int Cache_partitions;
			unsigned int Cache_Set;
			unsigned int Cache_Size;///< Cache��С
		};

		/// @brief CPUͨ����Ϣ����Դ��CPUZ����
		struct CPUCommonInformation
		{
			/// @brief CPU��ʾ����
			std::string Name;
			std::string CodeName; ///< cpu����
			std::string SocketPackage; ///< cpu��װleixing
			std::string MaxTDP;///<cpu�ı�׼����TDP
			std::string Technology;///<cpu�Ĺ���
			/// @brief CPU֧�ֵ�����
			std::string Instructions;
			/// @brief CPUϵ��
			uint32_t Family;
			/// @brief CPU��չϵ��
			uint32_t ExtFamily;
			/// @brief CPU�ͺ�
			uint32_t Model;
			/// @brief CPU��չ�ͺ�
			uint32_t ExtModel;
			/// @brief CPU����
			uint32_t Stepping;
			/// @brief Cache��Ϣ
			std::vector<Cache_info> Cache;
			/// @brief �޶����Family
			uint32_t DisplayFamily;
			/// @brief �޶����Model
			uint32_t DisplayModel;
			/// @brief CPU�޶��汾
			/// ע��ԭ�򣬲�֪����λ�ȡ
			//uint32_t Revision;
		};

		/// @brief ���߳�������
		class HyperThread final
		{
		public:
			/// @brief ���ó��߳�����
			/// @param[in] ThreadID �����߳�ID
			/// @param[in] OsID OS��ID
			/// @param[in] CoreID Core ID,��������Msr Handle
			HyperThread(const std::int32_t ThreadID, const int32_t OsID, const int32_t CoreID);

			/// @brief ��������
			void Update();

			/// @brief ������������Json
			/// @return Json����
			Json::Value BuildJson();

			/// @brief ȡ��Msr����Ȩ��
			/// @return Msrָ��
			std::weak_ptr<Utils::Ring0::SafeMsrHandle> GetMsr();

			/// @brief ��ȡOs Id
			/// @return osID
			const int32_t osID() const;
			/// @brief ��ȡThread Id
			/// @return ThreadID
			const int32_t ThreadID() const;

			/// @brief ����Decorator
			/// @param[in] Decorator ��̬��������
			void AddDecorator(std::unique_ptr<CPUDecorator> Decorator);
		private:
			/// @brief Ring0�����ñ��߳�Msr�Ĵ���ָ��
			std::shared_ptr<Utils::Ring0::SafeMsrHandle> m_MsrHandle;

			/// @brief CPU ��̬��Ϣ
			std::vector<std::shared_ptr<CPUDecorator>> m_Decorators;
			/// @brief Thread Id
			int32_t m_ThreadID;
			/// @brief OS �����ϵ�ID
			int32_t m_OsID;
		};

		/// @brief �������������
		class Core final
		{
		public:
			/// @brief �����������
			/// @param[in] CoreID ����ID
			/// @param[in] TileID Tile ID ����Socket����
			/// @param[in] SocketID ����Socket ID
			Core(const int32_t CoreID, const int32_t TileID, const int32_t SocketID);

			/// @brief ����һ�����߳�
			/// @param[in] ThreadID ���̵߳�ID
			/// @param[in] OsID ���̵߳�OS��ID
			void AddThread(const int32_t ThreadID, const int32_t OsID);

			/// @brief ͨ��OsId���ҳ��߳�
			/// @param[in] OsID OsId
			/// @return ���߳�ָ��
			std::weak_ptr < HyperThread> findThreadByOSID(const int32_t osID);

			/// @brief ��ȡ���߳�
			/// @return ���߳�����
			std::vector<std::shared_ptr<HyperThread>>& Threads();

			/// @brief ��ȡCore ID
			/// @return CoreID
			const int32_t CoreID() const;

			/// @brief ��ȡTile ID
			/// @return TileID
			const int32_t TileID() const;

			/// @brief ��ȡSocket ID
			/// @return SocketID
			const int32_t SocketID() const;
		private:
			/// @brief ���̴߳洢����
			std::vector<std::shared_ptr<HyperThread>> m_Thread;
			/// @brief Core Id
			int32_t m_CoreID;
			/// @brief ����Socket����Core��Id
			int32_t m_TileID;
			/// @brief �ϲ�SocketId
			int32_t m_SocketID;
		};

		/// @brief ϵͳ�д��ڵ�Socket
		class Socket final
		{
		public:
			/// @brief ����Id
			/// @param[in] ApicID Apic ID
			/// @param[in] LogicalID Socket ID
			/// @return
			Socket(const int32_t ApicID, const int32_t LogicalID);

			/// @brief ����Ĭ�Ϸ�ʽ����
			~Socket() = default;

			/// @brief ����һ��Core
			/// @param[in] CoreID
			/// @param[in] TileID
			/// @param[in] SocketID
			void AddCore(const int32_t CoreID, const int32_t TileID, const int32_t SocketID);

			/// @brief ����һ��Core
			/// @param[in] core Core���ƶ�����
			void AddCore(Core&& core);

			/// @brief ����Decorator
			/// @param[in] Decorator ��̬��������
			void AddDecorator(std::unique_ptr<CPUDecorator> Decorator);

			/// @brief ͨ��OsId���ҳ��߳�
			/// @param[in] OsID OsId
			/// @return ���߳�ָ��
			std::weak_ptr<HyperThread> findThreadByOSID(const int32_t OsID);

			/// @brief ͨ��TileID�����������
			/// @param[in] tileID
			/// @return �������ָ��
			std::weak_ptr<Core> findCoreByTileID(const int32_t tileID);
			/// @brief ��ȡApicId
			/// @return ApicId
			const int32_t ApicId() const;

			/// @brief ������������Json
			/// @return Json����
			Json::Value BuildJson();

			/// @brief ��������
			void Update();

			/// @brief ��ȡ�������
			/// @return �����������
			std::vector<std::shared_ptr<Core>>& Cores();

			/// @brief ��ȡ�������,Const����
			/// @return �����������
			const std::vector<std::shared_ptr<Core>>& Cores() const;
			/// @brief ��ȡSocketId
			/// @return SocketId
			const int32_t SocketId() const;
		public:
			/// @brief CPU��Ϣ������CPUZ����Ϣ
			CPUCommonInformation m_Data;
		private:
			/// @brief CPU ��̬��Ϣ
			std::vector<std::shared_ptr<CPUDecorator>> m_Decorators;

			/// @brief ����˴洢����
			std::vector<std::shared_ptr<Core>> m_Core;
			/// @brief Apic id
			int32_t   m_ApicID;
			/// @brief  Socket Id
			int32_t   m_LogicalID;
		};

		/// @brief ������������������CPU����or�߳�
		struct TopologyEntry // decribes a core
		{
			/// @brief ϵͳ�߳�Id
			int32_t os_id;
			/// @brief �߳�Id
			int32_t thread_id;
			/// @brief ����Id
			int32_t core_id;
			/// @brief ���Ĳ���Id
			int32_t tile_id; // tile is a constalation of 1 or more cores sharing salem L2 cache. Unique for entire system
			/// @brief Socket Id
			int32_t socket;
			TopologyEntry() : os_id(-1), thread_id(-1), core_id(-1), tile_id(-1), socket(-1) { }
		};

		/// @brief ͨ��CPU
		class GenericCPU
		{
		public:
			/// @brief ���캯�������ڳ�ʼ��CPU��Ϣ
			GenericCPU();

			/// @brief ����������
			virtual ~GenericCPU() = default;

			/// @brief ��ʼ��CPU
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Initialize(std::string& response) = 0;

			/// @brief ����CPU��Ϣ
			/// @param[in] Args Json����
			/// @param[out] response ��Ӧ��Json����
			/// @return @ref Data::ErrorType
			virtual Data::ErrorType Update(const std::string& Args, std::string& response) = 0;

			/// @brief ��ȡCPU��������
			/// Json��ʽ {"SocketId": 0}
			/// @param[in] paramter JSON����
			/// @param[out] response JSON��Ӧ
			/// @return @ref Data::ErrorType
			Data::ErrorType GetElements(LPCSTR paramter, std::string& response);
		protected:
			/// @brief ����һ��Socket
			/// @param[in] ApicID Apic Id
			/// @param[in] LogicalID Logical Id
			void AddSocket(const int32_t ApicID, const int32_t LogicalID);

			/// @brief ����һ��Socket
			/// @param[in] soc Socket���ƶ�����d
			void AddSocket(Socket&& soc);

			/// @brief �������²�����Json
			/// @param[in] paramter Json����
			/// @return ������Ĳ�������
			std::vector<CPUJsonInfo> PaserUpdateJson(const std::string& paramter);

			/// @brief ������ʼ��JSON����
			/// @return Utf-8��ʽ��JSON����
			std::string BuildInitializeJson();

			/// @brief ����ÿ��soc�ľ�̬��Ϣ
			/// @param Element JSon����
			/// @param soc soc�����ݽṹ
			/// @return Utf-8��ʽ��JSON����
			std::string BuildInitializeStaticJson(const Socket& soc);

			/// @brief ����ϵͳAPI����Cache������
			/// @param l1Data L1Cache���ݻ�������
			/// @param l1Inst L1Cacheָ�������
			/// @param l2 L2��������
			/// @param l3 L3��������
			void UpdateCacheNumber(int& l1Data, int& l1Inst, int& l2, int& l3);
		protected:
			/// @brief �洢CPU��Ϣ����
			/// First��Socket ApicID, Second��Socket����
			std::map<uint32_t, Socket> m_SystemRoot;
		private:
		};
	}
}
