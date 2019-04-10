#include "stdafx.h"
#pragma warning(disable:4996)
#include <fstream>
#include <map>
#include "gpu.h"
#include "Gpudata.h"
#include "Igpu.h"
#include "..\PCIE\PCI-E.h"

class GPU
{
public:
	static GPU* Instance()
	{
		if (!temp.get())
			temp = std::make_unique<GPU>();
		return temp.get();
	}
	void UpdateData()
	{
		gpudata->UpdateData();
		if (gpudata->nvinfo)
			for (auto& var : *(gpudata->nvinfo))
			{
				for (auto &gpu : gpuinfo)
				{
					if (var.FullName == gpu.first.GPUname)
					{
						gpu.second.fans = std::to_string(var.fans) + " RPM";
						gpu.second.Temperature = std::to_wstring(var.Device_Tem.GPUCurrTem) + L"  °C";
						if (var.Device_clock.GraphicsCurrent > 1000)
							gpu.second.GPUClock = std::to_string(var.Device_clock.GraphicsCurrent / 1000.0f) + " MHz";
						else
							gpu.second.GPUClock = std::to_string(var.Device_clock.GraphicsCurrent) + " KHz";
						if (var.Device_clock.GraphicsBoost > 1000)
							gpu.second.GPUBoostClock = std::to_string(var.Device_clock.GraphicsBoost / 1000.0f) + " MHz";
						else
							gpu.second.GPUBoostClock = std::to_string(var.Device_clock.GraphicsBoost) + " KHz";
						if (var.Device_clock.GraphicsBase > 1000)
							gpu.second.GPUBaseclock = std::to_string(var.Device_clock.GraphicsBase / 1000.0f) + " MHz";
						else
							gpu.second.GPUBaseclock = std::to_string(var.Device_clock.GraphicsBase) + " KHz";
						if (var.Device_clock.MemoryBase > 1000)
							gpu.second.GPUMemBaseclock = std::to_string(var.Device_clock.MemoryBase / 1000.0f) + " MHz";
						else
							gpu.second.GPUMemBaseclock = std::to_string(var.Device_clock.MemoryBase) + " KHz";
						if (var.Device_clock.MemoryBoost > 1000)
							gpu.second.GPUMemBoostClock = std::to_string(var.Device_clock.MemoryBoost / 1000.0f) + " MHz";
						else
							gpu.second.GPUMemBoostClock = std::to_string(var.Device_clock.MemoryBoost) + " KHz";
						if (var.Device_clock.MemoryCurrent > 1000)
							gpu.second.GPUMemClock = std::to_string(var.Device_clock.MemoryCurrent / 1000.0f) + " MHz";
						else
							gpu.second.GPUMemClock = std::to_string(var.Device_clock.MemoryCurrent) + " KHz";
						gpu.second.GPUUsage = std::to_string(var.Nvidia_Usage.GPUUsage) + " %";
						gpu.second.GPUFrameBufferUsage = std::to_string(var.Nvidia_Usage.FrameBufferUsage) + " %";
						gpu.second.GPUVideoEngineUsage = std::to_string(var.Nvidia_Usage.VideoEngineUsage) + " %";
						gpu.second.GPUBUSInterFaceUsage = std::to_string(var.Nvidia_Usage.BUSInterFaceUsage) + " %";
						gpu.second.CurrentPCIESpeed = var.CurrentPCIESpeed;
						if (var.curAvailableDedicatedVideoMemory > 1024)
						{
							if (var.curAvailableDedicatedVideoMemory / 1024 > 1024)
							{
								gpu.second.systemAvailableMemory = std::to_string(var.curAvailableDedicatedVideoMemory / 1024 / 1024.0f) + " GBytes";
							}
							else
								gpu.second.systemAvailableMemory = std::to_string(var.curAvailableDedicatedVideoMemory / 1024.0f) + " MBytes";
						}
						else
							gpu.second.systemAvailableMemory = std::to_string(var.curAvailableDedicatedVideoMemory) + " KBytes";
						for (size_t i = 0; i < var.NV_PState.ov.numVoltages; i++)
						{
							if (var.NV_PState.ov.voltages[i].bIsEditable && !var.NV_PState.ov.voltages[i].domainId)
							{
								gpu.second.Voltages = std::to_string(var.NV_PState.ov.voltages[i].volt_uV / 10000000.f) + " V";
								break;
							}
						}

						break;
					}
				}
			}

		if (gpudata->amdinfo)
		{
			for (auto &var : *(gpudata->amdinfo))
			{
				for (auto &gpu : gpuinfo)
				{
					if (var.FullName == gpu.first.GPUname)
					{
						gpu.second.CurrentPCIESpeed = var.CurrentPCIESpeed;
						gpu.second.Temperature = std::to_wstring(var.OverDrive5.TemperatureAndFans.temperatureInDegreesCelsius) + L"  °C";
						gpu.second.fans = std::to_string(var.OverDrive5.TemperatureAndFans.fanSpeedValue.iFanSpeed);
						if (var.OverDrive5.TemperatureAndFans.fanSpeedValue.iSpeedType == ADL_DL_FANCTRL_SPEED_TYPE_RPM)
							gpu.second.fans += " RPM";
						else
							gpu.second.fans += " %";
						gpu.second.GPUMemClock = std::to_string(var.OverDrive5.activity.iMemoryClock / 100) + " MHz";
						gpu.second.GPUClock = std::to_string(var.OverDrive5.activity.iEngineClock / 100) + " MHz";
						gpu.second.Voltages = std::to_string(var.OverDrive5.activity.iVddc / 1000.0f) + " V";
						gpu.second.GPUUsage = std::to_string(var.OverDrive5.activity.iActivityPercent) + " %";
						break;
					}
				}
			}
		}

	}
	const std::vector<std::pair<GPUBaseInfo, GPUSensorInfo>>& ReturnGPUInfo() const
	{
		return this->gpuinfo;
	}
	GPU() : gpudata(std::make_shared<GPUData>())
	{
		gpudata->UpdateData();
		GetInfo();
	}
private:
	explicit GPU(const GPU& x);
	GPU& operator=(const GPU& x) {};
	void GetInfo()
	{
		if(gpudata->nvinfo)
		for(auto& var : *(gpudata->nvinfo))
		{
			GPUBaseInfo temp = {};
			temp.GPUArchitecture = var.Architecture;
			temp.GPUDriverVersion = gpudata->NV_DriverVer;
			temp.GPUBranchVersion = gpudata->NV_BranchVersion;
			temp.GPUname = var.FullName;
			temp.GPUBiosVersion = var.VbiosVersion;
			temp.GPUTechnology = std::to_string(var.Technology) + "nm";
			temp.GPUTransistors = std::to_string(var.Transistors) + "million";
			temp.GPUDieSize = std::to_wstring(var.DieSize) + _T(" mm²");
			temp.GPUCoreName = var.CoreName;
			temp.GPURevision = var.PCIIdentify.pRevisionId;
			temp.MaxPCIESpeed = var.MaxPCIESpeed;
			if (var.dedicatedVideoMemory > 1024)
			{
				if (var.dedicatedVideoMemory / 1024 > 1024)
				{
					temp.GPUMemorySize = std::to_string(var.dedicatedVideoMemory / 1024 / 1024.0f ) + " GBytes";
				}
				else
					temp.GPUMemorySize = std::to_string(var.dedicatedVideoMemory / 1024.0f) + " MBytes";
			}
			else
				temp.GPUMemorySize = std::to_string(var.dedicatedVideoMemory) + " KBytes";
			if (var.sharedSystemMemory > 1024)
			{
				if (var.sharedSystemMemory / 1024 > 1024)
				{
					temp.GPUSharedMemorySize = std::to_string(var.sharedSystemMemory / 1024 / 1024.0f) + " GBytes";
				}
				else
					temp.GPUSharedMemorySize = std::to_string(var.sharedSystemMemory / 1024.0f) + " MBytes";
			}
			else
				temp.GPUSharedMemorySize = std::to_string(var.sharedSystemMemory) + " KBytes";

			temp.GPUMemoryBus = std::to_string(var.MemoryBus) + " Bit";
			temp.GPUMemoryType = var.MemoryType;
			temp.GPUBusWidth = std::to_string(var.MemoryBusWidth) + " GB/s";
			if(var.TDP)
				temp.GPUTDP = std::to_string(var.TDP) + " W";
			temp.GPUShaders = var.Shaders;
			temp.GPUTMUs = var.TMUs;
			temp.GPUROPs = var.ROPs;
			temp.GPUPixelRate = std::to_string(var.PixelRate) + " GPixel/s";
			temp.GPUTextureRate = std::to_string(var.TextureRate) + " GTexel/s";
			SV_ASSIST::PCIE::GetPCIVendorName((USHORT)(var.PCIIdentify.pSubSystemId & 0xFFFF), temp.GPUSubVendor);
			GPUSensorInfo temp2 = {};
			temp2.fans = std::to_string(var.fans) + " RPM";
			temp2.Temperature = std::to_wstring(var.Device_Tem.GPUCurrTem) + L"  °C";
			if(var.Device_clock.GraphicsCurrent > 1000)
				temp2.GPUClock = std::to_string(var.Device_clock.GraphicsCurrent / 1000.0f) + " MHz";
			else
				temp2.GPUClock = std::to_string(var.Device_clock.GraphicsCurrent) + " KHz";
			if (var.Device_clock.GraphicsBoost > 1000)
				temp2.GPUBoostClock = std::to_string(var.Device_clock.GraphicsBoost / 1000.0f) + " MHz";
			else
				temp2.GPUBoostClock = std::to_string(var.Device_clock.GraphicsBoost) + " KHz";
			if (var.Device_clock.GraphicsBase > 1000)
				temp2.GPUBaseclock = std::to_string(var.Device_clock.GraphicsBase / 1000.0f) + " MHz";
			else
				temp2.GPUBaseclock = std::to_string(var.Device_clock.GraphicsBase) + " KHz";
			if (var.Device_clock.MemoryBase > 1000)
				temp2.GPUMemBaseclock = std::to_string(var.Device_clock.MemoryBase / 1000.0f) + " MHz";
			else
				temp2.GPUMemBaseclock = std::to_string(var.Device_clock.MemoryBase) + " KHz";
			if (var.Device_clock.MemoryBoost > 1000)
				temp2.GPUMemBoostClock = std::to_string(var.Device_clock.MemoryBoost / 1000.0f) + " MHz";
			else
				temp2.GPUMemBoostClock = std::to_string(var.Device_clock.MemoryBoost) + " KHz";
			if (var.Device_clock.MemoryCurrent > 1000)
				temp2.GPUMemClock = std::to_string(var.Device_clock.MemoryCurrent / 1000.0f) + " MHz";
			else
				temp2.GPUMemClock = std::to_string(var.Device_clock.MemoryCurrent) + " KHz";
			temp2.GPUUsage = std::to_string(var.Nvidia_Usage.GPUUsage) + " %";
			temp2.GPUFrameBufferUsage = std::to_string(var.Nvidia_Usage.FrameBufferUsage) + " %";
			temp2.GPUVideoEngineUsage = std::to_string(var.Nvidia_Usage.VideoEngineUsage) + " %";
			temp2.GPUBUSInterFaceUsage = std::to_string(var.Nvidia_Usage.BUSInterFaceUsage) + " %";
			temp2.CurrentPCIESpeed = var.CurrentPCIESpeed;
			if (var.curAvailableDedicatedVideoMemory > 1024)
			{
				if (var.curAvailableDedicatedVideoMemory / 1024 > 1024)
				{
					temp2.systemAvailableMemory = std::to_string(var.curAvailableDedicatedVideoMemory / 1024 / 1024.0f) + " GBytes";
				}
				else
					temp2.systemAvailableMemory = std::to_string(var.curAvailableDedicatedVideoMemory / 1024.0f) + " MBytes";
			}
			else
				temp2.systemAvailableMemory = std::to_string(var.curAvailableDedicatedVideoMemory) + " KBytes";
			for (size_t i = 0; i < var.NV_PState.ov.numVoltages; i++)
			{
				if (var.NV_PState.ov.voltages[i].bIsEditable && !var.NV_PState.ov.voltages[i].domainId)
				{
					temp2.Voltages = std::to_string(var.NV_PState.ov.voltages[i].volt_uV / 10000000.f) + " V";
					break;
				}
			}
			this->gpuinfo.emplace_back(std::make_pair(std::move(temp),std::move(temp2)));
		}
		if (gpudata->amdinfo)
		for (auto &var : *(gpudata->amdinfo))
		{
			GPUBaseInfo temp = {};
			temp.GPUDriverVersion = gpudata->AMD_BranchVersion;
			temp.GPUBranchVersion = gpudata->AMD_DriverVer;
			temp.GPUname = var.FullName;
			temp.GPUBiosVersion = var.biosInfo.strVersion;
			temp.MaxPCIESpeed = var.MaxPCIESpeed;
			if (var.dedicatedVideoMemory > 1024)
			{
				if (var.dedicatedVideoMemory / 1024 > 1024)
				{
					temp.GPUMemorySize = std::to_string(var.dedicatedVideoMemory / 1024 / 1024.0f) + " GBytes";
				}
				else
					temp.GPUMemorySize = std::to_string(var.dedicatedVideoMemory / 1024.0f) + " MBytes";
			}
			else
				temp.GPUMemorySize = std::to_string(var.dedicatedVideoMemory) + " KBytes";
			if (var.sharedSystemMemory > 1024)
			{
				if (var.sharedSystemMemory / 1024 > 1024)
				{
					temp.GPUSharedMemorySize = std::to_string(var.sharedSystemMemory / 1024 / 1024.0f) + " GBytes";
				}
				else
					temp.GPUSharedMemorySize = std::to_string(var.sharedSystemMemory / 1024.0f) + " MBytes";
			}
			else
				temp.GPUSharedMemorySize = std::to_string(var.sharedSystemMemory) + " KBytes";
			GPUSensorInfo tempsensor = {};
			tempsensor.CurrentPCIESpeed = var.CurrentPCIESpeed;
			tempsensor.Temperature = std::to_wstring(var.OverDrive5.TemperatureAndFans.temperatureInDegreesCelsius) + L"  °C";
			tempsensor.fans = std::to_string(var.OverDrive5.TemperatureAndFans.fanSpeedValue.iFanSpeed);
			if (var.OverDrive5.TemperatureAndFans.fanSpeedValue.iSpeedType == ADL_DL_FANCTRL_SPEED_TYPE_RPM)
				tempsensor.fans += " RPM";
			else
				tempsensor.fans += " %";
			tempsensor.GPUMemClock = std::to_string(var.OverDrive5.activity.iMemoryClock / 100) + " MHz";
			tempsensor.GPUClock = std::to_string(var.OverDrive5.activity.iEngineClock / 100) + " MHz";
			tempsensor.Voltages = std::to_string(var.OverDrive5.activity.iVddc / 1000.0f) + " V";
			tempsensor.GPUUsage = std::to_string(var.OverDrive5.activity.iActivityPercent) + " %";
			this->gpuinfo.emplace_back(std::make_pair(std::move(temp), std::move(tempsensor)));
		}

		if (gpudata->Intelinfo)
			for (auto& var : *(gpudata->Intelinfo))
			{
				GPUBaseInfo temp = {};
				temp.GPUname = WStringToString(var.FullName);
				this->gpuinfo.emplace_back(std::make_pair(temp, GPUSensorInfo()));
			}
	}
	std::string WStringToString(const std::wstring &wstr)
	{
		std::string str;
		int nLen = (int)wstr.length();
		str.resize(nLen, '\0');
		int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
		if (nResult == 0)
		{
			return "";
		}
		return str;
	}
	std::vector<std::pair<GPUBaseInfo, GPUSensorInfo>> gpuinfo;
	std::shared_ptr<GPUData> gpudata;
	static std::unique_ptr<GPU> temp;
};

std::unique_ptr<GPU> GPU::temp = nullptr;

const std::vector<std::pair<GPUBaseInfo, GPUSensorInfo>>& SV_ASSIST::GPU::GetGpuInfo()
{
	return ::GPU::Instance()->ReturnGPUInfo();
}

void SV_ASSIST::GPU::UpdateDate()
{
	::GPU::Instance()->UpdateData();
}

