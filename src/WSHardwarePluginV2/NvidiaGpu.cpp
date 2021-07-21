#include "stdafx.h"
#include "NVAPIHelper.h"
#include "NvidiaGpu.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

Hardware::GPU::NvidiaGpu::NvidiaGpu(const GPUDevice& GpuData, std::unique_ptr<GPUAdapter>&& Adapter) : GPUDeviceBase(GpuData, std::move(Adapter))
{
}

Hardware::GPU::NvidiaGpu::~NvidiaGpu()
{
}

std::string Hardware::GPU::NvidiaGpu::UpdateGPUInfo()
{
	Json::Value root;
	try
	{
		Json::Value temp;
		temp["Temperature"] = NVAPIHelper::Instance().GetTemperature(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["Fan"] = NVAPIHelper::Instance().GetFanSpeed(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["GPU Power"] = NVAPIHelper::Instance().GetGPUPower(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["Voltage"] = NVAPIHelper::Instance().GetVoltage(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["Core Clock"] = NVAPIHelper::Instance().GetGPUCoreClock(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["Memory Clock"] = NVAPIHelper::Instance().GetGPUMemoryClock(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["GPU Usage"] = NVAPIHelper::Instance().GetGPUUsage(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	LARGE_INTEGER nFreq, nEndTime;
	QueryPerformanceFrequency(&nFreq);
	for (auto& node : this->m_Adapter->Nodes)
	{
		this->UpdateNode(node);
	}
	QueryPerformanceCounter(&nEndTime);
	TotalRunningTime.Delta = nEndTime.QuadPart - TotalRunningTime.Value;
	TotalRunningTime.Value = nEndTime.QuadPart;
	auto elapsedTime = (double)(TotalRunningTime.Delta * 100000ULL / nFreq.QuadPart);
	for (const auto& node : this->m_Adapter->Nodes)
	{
		auto usage = node.Delta / elapsedTime;
		if (usage > 100.0)
			usage = 100.0;
		if (usage > 1e-2)
		{
			Json::Value temp;
			temp[node.Name] = std::format("{:.2f} %", usage);
			root.append(temp);
		}
	}

	return Json::FastWriter().write(root);
}

std::string Hardware::GPU::NvidiaGpu::GetGPUInfo()
{
	Json::Value root;
	if (!SubVendor.empty())
	{
		Json::Value temp;
		temp["Vendor"] = SubVendor;
		root.append(temp);
	}

	try
	{
		Json::Value temp;
		temp["Description"] = NVAPIHelper::Instance().GetGPUName(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["Driver Version"] = NVAPIHelper::Instance().GetGPUDriverVersion();
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	if (this->m_Adapter)
	{
		if (this->m_Adapter->SharedMemory)
		{
			Json::Value temp;
			temp["Shared memory"] = Utils::MemoryToStringWithUnit(this->m_Adapter->SharedMemory);
			root.append(temp);
		}

		if (this->m_Adapter->DedicatedMemory)
		{
			Json::Value temp;
			temp["GPU memory"] = Utils::MemoryToStringWithUnit(this->m_Adapter->DedicatedMemory);
			root.append(temp);
		}
	}

	return Json::FastWriter().write(root);
}