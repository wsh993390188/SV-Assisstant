#include "stdafx.h"
#include "NVAPIHelper.h"
#include "NvidiaGpu.h"
#include "DXGIGPUBase.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

Hardware::GPU::NvidiaGpu::NvidiaGpu(const GPUDevice& GpuData) : GPUDeviceBase(GpuData)
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

	GPUDXBaseData data;
	if (DXGIGPUBase::Instance().QueryGPUInfo({ GPUBaseData.VendorId, GPUBaseData.DeviceId }, data) == Data::ErrorType::SUCCESS)
	{
		if (data.SharedSystemMemory)
		{
			Json::Value temp;
			temp["Shared memory"] = Utils::MemoryToStringWithUnit(data.SharedSystemMemory);
			root.append(temp);
		}

		if (data.DedicatedVideoMemory)
		{
			Json::Value temp;
			temp["GPU memory"] = Utils::MemoryToStringWithUnit(data.DedicatedVideoMemory);
			root.append(temp);
		}
	}

	return Json::FastWriter().write(root);
}