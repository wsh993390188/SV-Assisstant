#include "stdafx.h"
#include "MemoryManager.h"
#include "SPDMemoryController.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

using namespace Hardware;

Hardware::MemoryManager::MemoryManager() : MemoryControl(std::make_unique<Memory::SPDMemoryController>())
{
}

Data::ErrorType Hardware::MemoryManager::Init(LPCSTR, std::string& response)
{
	auto ret = MemoryControl->Initialize();
	if (ret == Data::ErrorType::SUCCESS)
		MemoryControl->BuildInitializeJson(response);
	return ret;
}

Data::ErrorType Hardware::MemoryManager::Update(LPCSTR paramter, std::string& response)
{
	return Data::ErrorType::NOTIMPLEMENTED;
}

Data::ErrorType Hardware::MemoryManager::GetElements(LPCSTR paramter, std::string& response)
{
	if (!paramter)
		return Data::ErrorType::UNKNOWNJSON;
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(paramter, root))
	{
		USHORT CurrentDIMM;
		if (MemoryControl->ParserDimmJson(root, CurrentDIMM) == Data::ErrorType::SUCCESS)
		{
			return MemoryControl->BuildDIMMJson(CurrentDIMM, response);
		}
	}
	return Data::ErrorType::UNKNOWNJSON;
}