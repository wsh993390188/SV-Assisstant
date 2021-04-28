#include "stdafx.h"
#include "Interface.h"

#include "DiskManager.h"
#include "CPUManager.h"
#include "MemoryManager.h"
#include "GPUManager.h"
#include "MotherBroadManager.h"
#include "AudioManager.h"
#include "NICManager.h"
#include "BatteryManager.h"
#include "WinBioManager.h"
#include "MonitorManager.h"
#include "BIOSManager.h"

namespace Hardware
{
	namespace Interface
	{
#pragma region 管理者
		int Manager::DoCallback(LPCSTR category, const char* message)
		{
			if (m_CallBack)
			{
				return m_CallBack(category, message);
			}
			return 0;
		}

		void Manager::InitManager(PluginCallback CallBack)
		{
			m_CallBack = CallBack;
		}

		Data::ErrorType Manager::DoAction(const CommandType& command, LPCSTR paramter, std::string& response)
		{
			switch (command)
			{
			case CommandType::Init:
				return this->Init(paramter, response);
			case CommandType::Update:
				return this->Update(paramter, response);
			case CommandType::Get:
				return this->GetElements(paramter, response);
			}
			return Data::ErrorType::UNKNOWNCOMMAND;
		}
#pragma endregion
	}
#pragma region 管理者工厂
	ManagerFactory* ManagerFactory::Instance()
	{
		static ManagerFactory inst;
		return &inst;
	}

	ManagerFactory::ManagerFactory()
	{
		m_Managers =
		{
			std::make_pair(ManagerType::Disk,std::make_shared<DiskManager>()),
			std::make_pair(ManagerType::CPU,std::make_shared<CPUManager>()),
			std::make_pair(ManagerType::Memory,std::make_shared<MemoryManager>()),
			std::make_pair(ManagerType::Broad,std::make_shared<MotherBroadManager>()),
			std::make_pair(ManagerType::GPU,std::make_shared<GPUManager>()),
			std::make_pair(ManagerType::Audio,std::make_shared<AudioManager>()),
			std::make_pair(ManagerType::Net,std::make_shared<NICManager>()),
			std::make_pair(ManagerType::Battery,std::make_shared<BatteryManager>()),
			std::make_pair(ManagerType::WinBio,std::make_shared<WinBioManager>()),
			std::make_pair(ManagerType::Monitor,std::make_shared<MonitorManager>()),
			std::make_pair(ManagerType::Bios,std::make_shared<BIOSManager>()),
		};
	}

	ManagerFactory::~ManagerFactory()
	{
		m_Managers.clear();
	}

	std::shared_ptr<Interface::Manager> ManagerFactory::GetManager(const ManagerType& Type)
	{
		auto itr = m_Managers.find(Type);
		if (itr != m_Managers.end())
		{
			return itr->second;
		}
		return nullptr;
	}
#pragma endregion
}