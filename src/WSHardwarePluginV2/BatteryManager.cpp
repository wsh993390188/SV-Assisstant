#include "stdafx.h"
#include "BatteryFromSys.h"
#include "BatteryManager.h"

using namespace Hardware;

Hardware::BatteryManager::BatteryManager() : BatteryController(std::make_unique<Battery::BatteryFromSys>())
{
}

Data::ErrorType Hardware::BatteryManager::Init(LPCSTR paramter, std::string& response)
{
	if (!BatteryController)
		return Data::ErrorType::NOTSUPPORT;
	return BatteryController->Initialize(response);
}

Data::ErrorType Hardware::BatteryManager::Update(LPCSTR paramter, std::string& response)
{
	if (!BatteryController)
		return Data::ErrorType::NOTSUPPORT;
	return BatteryController->Update(paramter, response);
}

Data::ErrorType Hardware::BatteryManager::GetElements(LPCSTR paramter, std::string& response)
{
	if (!BatteryController)
		return Data::ErrorType::NOTSUPPORT;
	return BatteryController->GetElements(paramter, response);
}