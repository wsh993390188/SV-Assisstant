// WSHardwarePluginV2.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <Shlwapi.h>
#include <comdef.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "WSHardwarePluginV2.h"

/// @brief 执行动作函数
///			根据不同的动作信息，执行不同的功能
/// @param[in] action 本次操作的动作信息
/// @param[in] paramter 本次操作的参数JSON格式信息
/// @param[out] response 本操作回应的Json信息
/// @return
///			@ref Hardware::Data::ErrorType
Hardware::Data::ErrorType PcmHardwareActionPaserStr(LPCSTR action, LPCSTR paramter, std::string& response)
{
	auto CommandAction = Hardware::CommandDataBase::Instance()->SwitchToCommand(action);
	if (CommandAction)
	{
		auto ManagerImpl = Hardware::ManagerFactory::Instance()->GetManager(CommandAction->GetManagerType());
		if (ManagerImpl)
		{
			return ManagerImpl->DoAction(CommandAction->GetCommandType(), paramter, response);
		}
	}

	return Hardware::Data::ErrorType::UNKNOWNCOMMAND;
}

int PcmHardwareInitialize(LPCSTR paramter, PluginCallback callback)
{
	auto logger = spdlog::basic_logger_mt("hardware_logger", "logs/hardware.log");
	spdlog::set_level(spdlog::level::info);
	spdlog::set_default_logger(logger);
	return S_OK;
}

int PcmHardwareAction(LPCSTR action, LPCSTR paramter, BSTR* response)
{
	std::string strRet;
	auto ret = PcmHardwareActionPaserStr(action, paramter, strRet);

	if (response && strRet.length() > 0) {
		auto content = Hardware::Utils::utf8ToWstring(strRet);
		*response = SysAllocString(content.c_str());
	}
	return Hardware::Utils::ToUType(ret);
}