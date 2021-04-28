#include "stdafx.h"
#include "BIOSManager.h"

using namespace Hardware;

Hardware::BIOSManager::BIOSManager() : BiosControl(std::make_unique<BIOS::GenericBios>())
{
}

Data::ErrorType Hardware::BIOSManager::Init(LPCSTR paramter, std::string& response)
{
	return BiosControl->Initialize(response);
}

Data::ErrorType Hardware::BIOSManager::Update(LPCSTR paramter, std::string& response)
{
	return BiosControl->Update(paramter, response);
}

Data::ErrorType Hardware::BIOSManager::GetElements(LPCSTR paramter, std::string& response)
{
	return BiosControl->GetElements(paramter, response);
}