#include "stdafx.h"
#include "GPUManager.h"

using namespace Hardware;

Hardware::GPUManager::GPUManager() : GPUController(GPU::GPUInstanceFactory::Instance().CreateGPU())
{
}

Data::ErrorType Hardware::GPUManager::Init(LPCSTR paramter, std::string& response)
{
	if (!GPUController)
		return Data::ErrorType::NOTSUPPORT;
	return GPUController->Initialize(response);
}

Data::ErrorType Hardware::GPUManager::Update(LPCSTR paramter, std::string& response)
{
	if (!GPUController)
		return Data::ErrorType::NOTSUPPORT;
	return GPUController->Update(paramter, response);
}

Data::ErrorType Hardware::GPUManager::GetElements(LPCSTR paramter, std::string& response)
{
	if (!GPUController)
		return Data::ErrorType::NOTSUPPORT;
	return GPUController->GetElements(paramter, response);
}