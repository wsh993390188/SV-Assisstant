#include "stdafx.h"
#include "EDIDFromRegistry.h"
#include "MonitorManager.h"

using namespace Hardware;

Hardware::MonitorManager::MonitorManager() : MonitorController(std::make_unique<Monitor::EDIDFromRegistry>())
{
}

Data::ErrorType Hardware::MonitorManager::Init(LPCSTR paramter, std::string& response)
{
	if (!MonitorController)
		return Data::ErrorType::NOTSUPPORT;
	return MonitorController->Initialize(response);
}

Data::ErrorType Hardware::MonitorManager::Update(LPCSTR paramter, std::string& response)
{
	if (!MonitorController)
		return Data::ErrorType::NOTSUPPORT;
	return MonitorController->Update(paramter, response);
}

Data::ErrorType Hardware::MonitorManager::GetElements(LPCSTR paramter, std::string& response)
{
	if (!MonitorController)
		return Data::ErrorType::NOTSUPPORT;
	return MonitorController->GetElements(paramter, response);
}