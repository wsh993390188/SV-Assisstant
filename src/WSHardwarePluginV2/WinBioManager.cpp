#include "stdafx.h"
#include "WinBioImpl.h"
#include "WinBioManager.h"

using namespace Hardware;

Hardware::WinBioManager::WinBioManager() : WinBioController(std::make_unique<WinBio::WinBioImpl>())
{
}

Data::ErrorType Hardware::WinBioManager::Init(LPCSTR paramter, std::string& response)
{
	if (!WinBioController)
		return Data::ErrorType::NOTSUPPORT;
	return WinBioController->Initialize(response);
}

Data::ErrorType Hardware::WinBioManager::Update(LPCSTR paramter, std::string& response)
{
	if (!WinBioController)
		return Data::ErrorType::NOTSUPPORT;
	return WinBioController->Update(paramter, response);
}

Data::ErrorType Hardware::WinBioManager::GetElements(LPCSTR paramter, std::string& response)
{
	if (!WinBioController)
		return Data::ErrorType::NOTSUPPORT;
	return WinBioController->GetElements(paramter, response);
}