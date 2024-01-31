#include "stdafx.h"
#include "AMDGPU.h"
#include "ADLSDKHelper.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

Hardware::GPU::AMDGPU::AMDGPU(const GPUDevice& GpuData, std::unique_ptr<GPUAdapter>&& Adapter) : GPUDeviceBase(GpuData, std::move(Adapter))
{
}

std::string Hardware::GPU::AMDGPU::UpdateGPUInfo()
{
	Json::Value root;
	try
	{
		Json::Value temp;
		temp["Temperature"] = ADLSDKHelper::Instance().GetTemperature(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["Fan"] = ADLSDKHelper::Instance().GetFanSpeed(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["Voltage"] = ADLSDKHelper::Instance().GetGPUVoltage(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["Core Clock"] = ADLSDKHelper::Instance().GetGPUCoreClock(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["Memory Clock"] = ADLSDKHelper::Instance().GetGPUMemoryClock(GPUBaseData.DeviceId);
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

std::string Hardware::GPU::AMDGPU::GetGPUInfo()
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
		temp["Description"] = ADLSDKHelper::Instance().GetGPUName(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["Driver Version"] = ADLSDKHelper::Instance().GetGPUDriverVersion(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["vBIOS Version"] = ADLSDKHelper::Instance().GetGPUBIOSVersion(GPUBaseData.DeviceId);
		root.append(temp);
	}
	catch (const std::exception&)
	{
	}

	try
	{
		Json::Value temp;
		temp["vBIOS Date"] = ADLSDKHelper::Instance().GetGPUBIOSDate(GPUBaseData.DeviceId);
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
