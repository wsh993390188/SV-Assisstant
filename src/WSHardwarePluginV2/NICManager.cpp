#include "stdafx.h"
#include "NICIphlpapi.h"
#include "NICManager.h"

using namespace Hardware;

Hardware::NICManager::NICManager() : NICController(std::make_unique<NIC::NICIphlpapi>())
{
}

Data::ErrorType Hardware::NICManager::Init(LPCSTR paramter, std::string& response)
{
	if (!NICController)
		return Data::ErrorType::NOTSUPPORT;
	return NICController->Initialize(response);
}

Data::ErrorType Hardware::NICManager::Update(LPCSTR paramter, std::string& response)
{
	if (!NICController)
		return Data::ErrorType::NOTSUPPORT;
	return NICController->Update(paramter, response);
}

Data::ErrorType Hardware::NICManager::GetElements(LPCSTR paramter, std::string& response)
{
	if (!NICController)
		return Data::ErrorType::NOTSUPPORT;
	return NICController->GetElements(paramter, response);
}