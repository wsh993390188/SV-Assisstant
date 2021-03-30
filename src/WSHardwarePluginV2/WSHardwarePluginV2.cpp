// WSHardwarePluginV2.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <Shlwapi.h>
#include <comdef.h>
#include "WSHardwarePluginV2.h"

/// @brief ִ�ж�������
///			���ݲ�ͬ�Ķ�����Ϣ��ִ�в�ͬ�Ĺ���
/// @param[in] action ���β����Ķ�����Ϣ
/// @param[in] paramter ���β����Ĳ���JSON��ʽ��Ϣ
/// @param[out] response ��������Ӧ��Json��Ϣ
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
	spdlog::set_level(spdlog::level::info);
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