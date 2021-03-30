#include "stdafx.h"
#include "MotherBroadManager.h"
#include "MotherBroadFromSMBIOS.h"

using namespace Hardware;

Hardware::MotherBroadManager::MotherBroadManager() : MotherBroadControl(std::make_unique<MotherBroad::MotherBroadFromSMBIOS>())
{
}

Data::ErrorType Hardware::MotherBroadManager::Init(LPCSTR paramter, std::string& response)
{
	return MotherBroadControl->Initialize(response);
}

Data::ErrorType Hardware::MotherBroadManager::Update(LPCSTR paramter, std::string& response)
{
	return MotherBroadControl->Update(paramter, response);
}

Data::ErrorType Hardware::MotherBroadManager::GetElements(LPCSTR paramter, std::string& response)
{
	return MotherBroadControl->GetElements(paramter, response);
}