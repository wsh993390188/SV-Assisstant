#include "stdafx.h"
#include "GenericGPU.h"
#include "PCIGPUDetect.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

using namespace Hardware;

Hardware::GPU::GenericGPU::GenericGPU()
{
}

std::string Hardware::GPU::GenericGPU::BuildInitializeJson()
{
	Json::Value root;
	for (const auto& GPUInfo : GPUInfos)
	{
		root["GPUIds"].append(GPUInfo.first);
	}
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

bool Hardware::GPU::GenericGPU::ParserJson(const std::string& JsonString, uint32_t& GPUId)
{
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(JsonString, root))
		{
			if (root["GPUId"].isIntegral())
			{
				GPUId = root["GPUId"].asUInt();
				return true;
			}
			else
			{
				spdlog::error("Json GPUId is not numberic");
			}
		}
		else
		{
			spdlog::error("parser json failed");
		}
	}
	catch (const std::exception& e)
	{
		spdlog::error("parser json failed {}", e.what());
	}
	return false;
}

GPU::GPUInstanceFactory& Hardware::GPU::GPUInstanceFactory::Instance()
{
	static GPUInstanceFactory inst;
	return inst;
}

std::unique_ptr<GPU::GenericGPU> Hardware::GPU::GPUInstanceFactory::CreateGPU()
{
	return std::make_unique<PciGpuDetect>();
}