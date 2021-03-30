#include "stdafx.h"
#include "Command.h"

namespace Hardware
{
	namespace
	{
#pragma region ����ģʽ������
		/// @brief ����ģʽ������
		/// @tparam ManagerCommand ��������������
		/// @tparam OperatorCommand ������������
		template<class ManagerCommand, class OperatorCommand>
		class GenerateCommand final : public ManagerCommand, public OperatorCommand
		{
		};
#pragma endregion

#pragma region ����������
		/// @brief Ӳ�̹���������
		class DiskManagerCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ����������
			/// @return	@ref ManagerType
			ManagerType GetManagerType() override final
			{
				return ManagerType::Disk;
			}
		};

		/// @brief CPU����������
		class CpuManagerCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ����������
			/// @return	@ref ManagerType
			ManagerType GetManagerType() override final
			{
				return ManagerType::CPU;
			}
		};

		/// @brief �ڴ����������
		class MemoryManagerCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ����������
			/// @return	@ref ManagerType
			ManagerType GetManagerType() override final
			{
				return ManagerType::Memory;
			}
		};

		/// @brief ���弰BIOS����������
		class MotherBroadManagerCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ����������
			/// @return	@ref ManagerType
			ManagerType GetManagerType() override final
			{
				return ManagerType::Broad;
			}
		};

		/// @brief �Կ�����������
		class GPUManagerCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ����������
			/// @return	@ref ManagerType
			ManagerType GetManagerType() override final
			{
				return ManagerType::GPU;
			}
		};

		/// @brief ��������������
		class AudioManagerCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ����������
			/// @return	@ref ManagerType
			ManagerType GetManagerType() override final
			{
				return ManagerType::Audio;
			}
		};

		/// @brief ��������������
		class NetManagerCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ����������
			/// @return	@ref ManagerType
			ManagerType GetManagerType() override final
			{
				return ManagerType::Net;
			}
		};

		/// @brief Win10���ﴫ��������������
		class WinBioManagerCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ����������
			/// @return	@ref ManagerType
			ManagerType GetManagerType() override final
			{
				return ManagerType::WinBio;
			}
		};

		/// @brief ��ع���������
		class BatteryManagerCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ����������
			/// @return	@ref ManagerType
			ManagerType GetManagerType() override final
			{
				return ManagerType::Battery;
			}
		};

		/// @brief ��ʾ������������
		class MonitorManagerCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ����������
			/// @return	@ref ManagerType
			ManagerType GetManagerType() override final
			{
				return ManagerType::Monitor;
			}
		};

#pragma endregion

#pragma region ��������
		/// @brief ��ʼ������
		class InitOperatorCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ��������
			/// @return ��������@ref CommandType
			CommandType GetCommandType() override final
			{
				return CommandType::Init;
			}
		};

		/// @brief ���ݸ�������
		class UpdateOperatorCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ��������
			/// @return ��������@ref CommandType
			CommandType GetCommandType() override final
			{
				return CommandType::Update;
			}
		};

		/// @brief ��ȡ��������
		class GetOperatorCommand : virtual public Command
		{
		public:
			/// @brief ��ȡ��������
			/// @return ��������@ref CommandType
			CommandType GetCommandType() override final
			{
				return CommandType::Get;
			}
		};
#pragma endregion

#pragma region ����λ��

		template<typename T>
		constexpr decltype(auto) ToUType(T enumerator)
		{
			return static_cast<std::underlying_type_t<T>>(enumerator);
		}

		enum class Action : uint32_t
		{
			PCM = 0,
			PluginName = 1,
			Action = 2,
			Manage = 3,
			Operator = 4
		};
#pragma endregion
	}
}

Hardware::CommandDataBase* Hardware::CommandDataBase::Instance()
{
	static CommandDataBase inst;
	return &inst;
}

template<class ManagerCommand>
std::map<Hardware::CommandType, std::shared_ptr<Hardware::Command>> Hardware::CommandDataBase::InitCommand()
{
	// TODO �Ż�����
	std::map<CommandType, std::shared_ptr<Command>> temp;
	temp.emplace(CommandType::Init, std::make_shared<GenerateCommand<ManagerCommand, InitOperatorCommand>>());
	temp.emplace(CommandType::Update, std::make_shared<GenerateCommand<ManagerCommand, UpdateOperatorCommand>>());
	temp.emplace(CommandType::Get, std::make_shared<GenerateCommand<ManagerCommand, GetOperatorCommand>>());
	return std::move(temp);
}

Hardware::CommandDataBase::CommandDataBase()
{
	m_CommandDatabase.emplace(ManagerType::Disk, std::move(InitCommand<DiskManagerCommand>()));
	m_CommandDatabase.emplace(ManagerType::CPU, std::move(InitCommand<CpuManagerCommand>()));
	m_CommandDatabase.emplace(ManagerType::Memory, std::move(InitCommand<MemoryManagerCommand>()));
	m_CommandDatabase.emplace(ManagerType::Broad, std::move(InitCommand<MotherBroadManagerCommand>()));
	m_CommandDatabase.emplace(ManagerType::GPU, std::move(InitCommand<GPUManagerCommand>()));
	m_CommandDatabase.emplace(ManagerType::Audio, std::move(InitCommand<AudioManagerCommand>()));
	m_CommandDatabase.emplace(ManagerType::Net, std::move(InitCommand<NetManagerCommand>()));
	m_CommandDatabase.emplace(ManagerType::WinBio, std::move(InitCommand<WinBioManagerCommand>()));
	m_CommandDatabase.emplace(ManagerType::Battery, std::move(InitCommand<BatteryManagerCommand>()));
	m_CommandDatabase.emplace(ManagerType::Monitor, std::move(InitCommand<MonitorManagerCommand>()));
}

std::shared_ptr<Hardware::Command> Hardware::CommandDataBase::SwitchToCommand(const std::string& Action)
{
	try
	{
		auto SplitStr = Utils::split(Action, decltype(Action)("_"));
		auto ManagerItr = m_CommandDatabase.find(StringToMangerType(SplitStr.at(ToUType(Action::Manage))));
		if (ManagerItr != m_CommandDatabase.end())
		{
			auto CommandItr = ManagerItr->second.find(StringToOperatorType(SplitStr.at(ToUType(Action::Operator))));
			if (CommandItr != ManagerItr->second.end())
			{
				return CommandItr->second;
			}
		}
	}
	catch (...)
	{
	}

	return nullptr;
}

Hardware::ManagerType Hardware::CommandDataBase::StringToMangerType(const std::string& ManagerString)
{
	if (ManagerString.compare("DISK") == 0)
		return ManagerType::Disk;
	else if (ManagerString.compare("CPU") == 0)
		return ManagerType::CPU;
	else if (ManagerString.compare("MEMORY") == 0)
		return ManagerType::Memory;
	else if (ManagerString.compare("BROAD") == 0)
		return ManagerType::Broad;
	else if (ManagerString.compare("GPU") == 0)
		return ManagerType::GPU;
	else if (ManagerString.compare("AUDIO") == 0)
		return ManagerType::Audio;
	else if (ManagerString.compare("NIC") == 0)
		return ManagerType::Net;
	else if (ManagerString.compare("WINBIO") == 0)
		return ManagerType::WinBio;
	else if (ManagerString.compare("BATTERY") == 0)
		return ManagerType::Battery;
	else if (ManagerString.compare("MONITOR") == 0)
		return ManagerType::Monitor;
	return ManagerType::None;
}

Hardware::CommandType Hardware::CommandDataBase::StringToOperatorType(const std::string& OperatorString)
{
	if (OperatorString.compare("INIT") == 0)
		return CommandType::Init;
	else if (OperatorString.compare("UPDATE") == 0)
		return CommandType::Update;
	else if (OperatorString.compare("GET") == 0)
		return CommandType::Get;
	return CommandType::None;
}