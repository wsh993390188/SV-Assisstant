#include "stdafx.h"
#include "AMDGPU.h"
#include "DXGIGPUBase.h"
#include "ADLSDKHelper.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

Hardware::GPU::AMDGPU::AMDGPU(const GPUDevice& GpuData) : GPUDeviceBase(GpuData)
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
	return Json::FastWriter().write(root);
}

std::string Hardware::GPU::AMDGPU::GetGPUInfo()
{
	Json::Value root;
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