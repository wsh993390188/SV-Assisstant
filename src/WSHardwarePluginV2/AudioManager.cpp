#include "stdafx.h"
#include "AudioInfoFromWindowsAPI.h"
#include "AudioManager.h"

using namespace Hardware;

Hardware::AudioManager::AudioManager() : AudioController(std::make_unique<Audio::AudioInfoFromWindowsAPI>())
{
}

Data::ErrorType Hardware::AudioManager::Init(LPCSTR paramter, std::string& response)
{
	if (!AudioController)
		return Data::ErrorType::NOTSUPPORT;
	return AudioController->Initialize(response);
}

Data::ErrorType Hardware::AudioManager::Update(LPCSTR paramter, std::string& response)
{
	if (!AudioController)
		return Data::ErrorType::NOTSUPPORT;
	return AudioController->Update(paramter, response);
}

Data::ErrorType Hardware::AudioManager::GetElements(LPCSTR paramter, std::string& response)
{
	if (!AudioController)
		return Data::ErrorType::NOTSUPPORT;
	return AudioController->GetElements(paramter, response);
}