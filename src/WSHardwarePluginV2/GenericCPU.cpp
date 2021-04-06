#include "stdafx.h"
#include "GenericCPU.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

namespace Hardware
{
	namespace CPU
	{
		Socket::Socket(const int32_t apicID, const int32_t logicalID) : m_ApicID(apicID), m_LogicalID(logicalID)
		{
		}

		void Socket::AddCore(const int32_t CoreID, const int32_t TileID, const int32_t SocketID)
		{
			this->m_Core.emplace_back(std::make_shared<Core>(CoreID, TileID, SocketID));
		}

		void Socket::AddCore(Core&& core)
		{
			m_Core.push_back(std::make_shared<Core>(core));
		}

		void Socket::AddDecorator(std::unique_ptr<CPUDecorator> Decorator)
		{
			m_Decorators.emplace_back(std::move(Decorator));
		}

		std::weak_ptr<HyperThread> Socket::findThreadByOSID(const int32_t OsID)
		{
			for (auto& core : m_Core)
			{
				auto thread = core->findThreadByOSID(OsID);
				if (!thread.expired())
					return thread;
			}
			return std::weak_ptr<HyperThread>();
		}

		std::weak_ptr<Core> Socket::findCoreByTileID(const int32_t tileID)
		{
			for (auto& core : m_Core) {
				if (core->TileID() == tileID)
					return core;
			}
			return std::weak_ptr<Core>();
		}

		const int32_t Socket::ApicId() const
		{
			return m_ApicID;
		}

		Json::Value Socket::BuildJson()
		{
			Json::Value root;
			for (auto& ele : m_Decorators)
			{
				auto StrValue = ele->GetDecoratorValue();
				if (!StrValue.empty())
				{
					Json::Value temp;
					temp["Name"] = ele->GetDecoratorName();
					temp["Value"] = StrValue;
					root["Element"].append(temp);
				}
			}
			if (root.isNull())
				return {};
			return root;
		}

		void Socket::Update()
		{
			auto TempThreadPtr = this->findThreadByOSID(0);
			if (!TempThreadPtr.expired())
			{
				auto Thread0 = TempThreadPtr.lock();
				for (auto& ele : m_Decorators)
				{
					ele->Update(Thread0->GetMsr());
				}
			}
		}

		std::vector<std::shared_ptr<Core>>& Socket::Cores()
		{
			return m_Core;
		}

		const std::vector<std::shared_ptr<Core>>& Socket::Cores() const
		{
			return m_Core;
		}

		const int32_t Socket::SocketId() const
		{
			return m_LogicalID;
		}

		Core::Core(const int32_t CoreID, const int32_t TileID, const int32_t SocketID) :
			m_CoreID(CoreID), m_TileID(TileID), m_SocketID(SocketID)
		{
		}

		void Core::AddThread(const int32_t ThreadID, const int32_t OsID)
		{
			m_Thread.emplace_back(std::make_shared<HyperThread>(ThreadID, OsID, m_CoreID));
		}

		std::weak_ptr<HyperThread> Core::findThreadByOSID(const int32_t osID)
		{
			for (auto& thread : m_Thread) {
				if (thread->osID() == osID)
					return thread;
			}
			return std::weak_ptr<HyperThread>();
		}

		std::vector<std::shared_ptr<HyperThread>>& Core::Threads()
		{
			return m_Thread;
		}

		const int32_t Core::CoreID() const
		{
			return m_CoreID;
		}

		const int32_t Core::TileID() const
		{
			return m_TileID;
		}

		const int32_t Core::SocketID() const
		{
			return m_SocketID;
		}

		HyperThread::HyperThread(const int32_t ThreadID, const int32_t OsID, const int32_t CoreID) : m_ThreadID(ThreadID), m_OsID(OsID),
			m_MsrHandle(std::make_shared<Utils::Ring0::SafeMsrHandle>(CoreID))
		{
		}

		void HyperThread::Update()
		{
			for (auto& ele : m_Decorators)
			{
				ele->Update(m_MsrHandle);
			}
		}

		Json::Value HyperThread::BuildJson()
		{
			Json::Value root;
			for (auto& ele : m_Decorators)
			{
				auto StrValue = ele->GetDecoratorValue();
				if (!StrValue.empty())
				{
					Json::Value temp;
					temp["Name"] = ele->GetDecoratorName();
					temp["Value"] = StrValue;
					root["Element"].append(temp);
				}
			}
			if (root.isNull())
				return {};
			root["ThreadId"] = this->m_OsID;
			return root;
		}

		std::weak_ptr<Utils::Ring0::SafeMsrHandle> HyperThread::GetMsr()
		{
			return m_MsrHandle;
		}

		const int32_t HyperThread::osID() const
		{
			return m_OsID;
		}

		const int32_t HyperThread::ThreadID() const
		{
			return m_ThreadID;
		}

		void HyperThread::AddDecorator(std::unique_ptr<CPUDecorator> Decorator)
		{
			m_Decorators.emplace_back(std::move(Decorator));
		}

		GenericCPU::GenericCPU()
		{
			CPUDB::Instance().Initialize();
		}

		Data::ErrorType GenericCPU::GetElements(LPCSTR paramter, std::string& response)
		{
			try
			{
				Json::Reader reader;
				Json::Value root;
				if (reader.parse(paramter, root))
				{
					if (root["SocketId"].isIntegral())
					{
						auto SocketId = root["SocketId"].asUInt();
						auto soc = m_SystemRoot.find(SocketId);
						if (soc != m_SystemRoot.end())
						{
							response = BuildInitializeStaticJson(soc->second);
							return Data::ErrorType::SUCCESS;
						}
						else
						{
							spdlog::error("can not find SocketId({})", SocketId);
						}
					}
					else
					{
						spdlog::error("need socketId to get socket info");
					}
				}
				return Data::ErrorType::DATAEMPTY;
			}
			catch (const std::exception& e)
			{
				spdlog::error("parser cpu json error:{}", e.what());
			}

			return Data::ErrorType::UNKNOWNJSON;
		}

		void GenericCPU::AddSocket(const int32_t ApicID, const int32_t LogicalID)
		{
			Socket soc(ApicID, LogicalID);
			this->m_SystemRoot.emplace(ApicID, std::move(soc));
		}

		void GenericCPU::AddSocket(Socket&& soc)
		{
			this->m_SystemRoot.emplace(soc.ApicId(), std::move(soc));
		}
		std::vector<CPUJsonInfo> GenericCPU::PaserUpdateJson(const std::string& paramter)
		{
			Json::Reader reader;
			Json::Value root;
			std::vector<CPUJsonInfo> res;

			try
			{
				if (reader.parse(paramter, root))
				{
					for (const auto& ele : root)
					{
						try
						{
							CPUJsonInfo temp;
							if (ele["SocketId"].isIntegral())
								temp.SocketId = ele["SocketId"].asInt();
							if (ele["CoreId"].isIntegral())
								temp.CoreId = ele["CoreId"].asInt();
							if (ele["ThreadId"].isIntegral())
								temp.ThreadId = ele["ThreadId"].asInt();
							res.push_back(std::move(temp));
						}
						catch (Json::Exception& e)
						{
							spdlog::debug("Unknown Json, Exception: {}", e.what());
							continue;
						}
					}
				}
			}
			catch (Json::Exception& e)
			{
				spdlog::debug("Unknown Json, Exception: {}", e.what());
			}

			return res;
		}

		std::string GenericCPU::BuildInitializeJson()
		{
			Json::Value root;

			for (const auto& soc : m_SystemRoot)
			{
				root["SocketId"] = soc.first;
				for (const auto& core : soc.second.Cores())
				{
					Json::Value CoreJson;
					CoreJson["CoreId"] = core->CoreID();
					for (const auto& thread : core->Threads())
					{
						Json::Value ThreadJson;
						ThreadJson["ThreadId"] = thread->osID();
						CoreJson["ThreadIds"].append(std::move(ThreadJson));
					}
					root["CoreIds"].append(std::move(CoreJson));
				}
			}
			if (root.isNull())
				return {};
			return Json::FastWriter().write(root);
		}

		std::string GenericCPU::BuildInitializeStaticJson(const Socket& soc)
		{
			Json::Value Element;
			if (!soc.m_Data.CodeName.empty())
			{
				Json::Value TempValue;
				TempValue["CodeName"] = soc.m_Data.CodeName;
				Element.append(std::move(TempValue));
			}
			if (!soc.m_Data.Technology.empty())
			{
				Json::Value TempValue;
				TempValue["Technology"] = soc.m_Data.Technology + " nm";
				Element.append(std::move(TempValue));
			}
			if (!soc.m_Data.MaxTDP.empty())
			{
				Json::Value TempValue;
				TempValue["Max TDP"] = soc.m_Data.MaxTDP + " W";
				Element.append(std::move(TempValue));
			}
			if (!soc.m_Data.SocketPackage.empty())
			{
				Json::Value TempValue;
				TempValue["Socket Package"] = soc.m_Data.SocketPackage;
				Element.append(std::move(TempValue));
			}
			if (!soc.m_Data.Name.empty())
			{
				Json::Value TempValue;
				TempValue["Name"] = soc.m_Data.Name;
				Element.append(std::move(TempValue));
			}
			if (!soc.m_Data.Instructions.empty())
			{
				Json::Value TempValue;
				TempValue["Instructions"] = soc.m_Data.Instructions;
				Element.append(std::move(TempValue));
			}

			{
				Json::Value TempValue;
				TempValue["Family"] = soc.m_Data.Family;
				Element.append(std::move(TempValue));
			}

			{
				Json::Value TempValue;
				TempValue["Model"] = soc.m_Data.Model;
				Element.append(std::move(TempValue));
			}

			{
				Json::Value TempValue;
				TempValue["Stepping"] = soc.m_Data.Stepping;
				Element.append(std::move(TempValue));
			}

			{
				Json::Value TempValue;
				TempValue["Ext.Family"] = soc.m_Data.ExtFamily;
				Element.append(std::move(TempValue));
			}

			{
				Json::Value TempValue;
				TempValue["Ext.Model"] = soc.m_Data.ExtModel;
				Element.append(std::move(TempValue));
			}
			{
				Json::Value CacheJson;
				for (const auto& Cache : soc.m_Data.Cache)
				{
					Json::Value CacheInfo;

					CacheInfo["Level"] = Cache.Cache_level;
					switch (Cache.Cache_type)
					{
					case 'D':
						CacheInfo["Type"] = u8" Data";
						break;
					case 'T':
						CacheInfo["Type"] = u8" Instruction";
						break;
					}
					CacheInfo["LineSize"] = Cache.Cache_line_size;
					CacheInfo["Partition"] = Cache.Cache_partitions;
					CacheInfo["Set"] = Cache.Cache_Set;
					CacheInfo["Ways"] = Cache.Cache_Ways;
					CacheInfo["Number"] = Cache.NumberOfCache;
					CacheInfo["Size"] = Utils::MemoryToStringWithUnit(Cache.Cache_Size);
					CacheJson["Caches"].append(std::move(CacheInfo));
				}
				Element.append(std::move(CacheJson));
			}

			if (Element.isNull())
				return {};
			return Json::FastWriter().write(Element);
		}

		void GenericCPU::UpdateCacheNumber(int& l1Data, int& l1Inst, int& l2, int& l3)
		{
			l1Data = 0; l1Inst = 0; l2 = 0; l3 = 0;
			std::unique_ptr<char[]>slpi(new char[sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)]);
			DWORD len = (DWORD)sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
			BOOL res = GetLogicalProcessorInformationEx(RelationAll, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(slpi.get()), &len);

			while (res == FALSE)
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					slpi.reset(new char[len]);
					res = GetLogicalProcessorInformationEx(RelationAll, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(slpi.get()), &len);
				}
				else
				{
					spdlog::debug("Error in Windows function 'GetLogicalProcessorInformationEx': {}", GetLastError());
					return;
				}
			}

			char* base_slpi = slpi.get();
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX pi = NULL;

			for (; base_slpi < slpi.get() + len; base_slpi += (DWORD)pi->Size)
			{
				pi = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(base_slpi);
				if (pi->Relationship == RelationCache)
				{
					if (pi->Cache.Level == 1 && pi->Cache.Type == CacheData)
					{
						l1Data++;
					}
					else if (pi->Cache.Level == 1 && pi->Cache.Type == CacheInstruction)
					{
						l1Inst++;
					}
					else if (pi->Cache.Level == 2)
					{
						l2++;
					}
					else if (pi->Cache.Level == 3)
					{
						l3++;
					}
				}
			}
		}

		CPUDecorator::CPUDecorator(const uint64_t MsrRegistry, const std::string& Name) : m_MsrRegistry(MsrRegistry), m_Name(Name), IsUpdate(false)
		{
		}
		const std::string CPUDecorator::GetDecoratorName() const
		{
			return m_Name;
		}
	}
}