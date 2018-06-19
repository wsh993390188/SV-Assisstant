#include "stdafx.h"
#pragma warning(disable:4996)
#include "gpu.h"
#include "Gpudata.h"
#include "Igpu.h"

class GPU
{
public:
	static const string& GPUName()
	{
		if (gpudata.amdinfo)
		{
			return gpudata.amdinfo->adapterInfo.strAdapterName;
		}
		else if (gpudata.nvinfo)
		{
			return gpudata.nvinfo->FUllName;
		}
		else
		{
			return "";
		}
	}

	static const string& GetGPUBIOSVersion()
	{
		if (gpudata.amdinfo)
		{
			return gpudata.amdinfo->biosInfo.strVersion;
		}
		else if (gpudata.nvinfo)
		{
			return gpudata.nvinfo->VbiosVersion;
		}
		else
		{
			return "";
		}
	}

	static double GetGPUtemperature()
	{
		if (gpudata.amdinfo)
		{
			return gpudata.amdinfo->OverDrive5.TemperatureAndFans.adlTemperature.iTemperature / 1000.0;
		}
		else if (gpudata.nvinfo)
		{
			return gpudata.nvinfo->Device_Tem.GPUCurrTem;
		}
		else
		{
			return 0;
		}
	}

	static int GetGPUFans(SV_ASSIST::GPU::FanTypes & type)
	{
		type = SV_ASSIST::GPU::FanTypes::RPM;
		if (gpudata.amdinfo)
		{
			if (gpudata.amdinfo->OverDrive5.TemperatureAndFans.fanSpeedValue.iSpeedType == ADL_DL_FANCTRL_SPEED_TYPE_PERCENT)
				type = SV_ASSIST::GPU::FanTypes::Percent;
			return gpudata.amdinfo->OverDrive5.TemperatureAndFans.fanSpeedValue.iFanSpeed;
		}
		else if (gpudata.nvinfo)
		{
			return gpudata.nvinfo->fans;
		}
		else
		{
			return 0;
		}
	}

	static double GetGPUCurrentClock()
	{
		if (gpudata.amdinfo)
		{
			return gpudata.amdinfo->OverDrive5.activity.iEngineClock / 100.0;
		}
		else if (gpudata.nvinfo)
		{
			return gpudata.nvinfo->Device_clock.GraphicsCurrent / 1000.0;
		}
		else
		{
			return 0;
		}
	}

	static double GetGPUMemoryClock()
	{
		if (gpudata.amdinfo)
		{
			return gpudata.amdinfo->OverDrive5.activity.iMemoryClock / 100.0;
		}
		else if (gpudata.nvinfo)
		{
			return gpudata.nvinfo->Device_clock.MemoryCurrent / 1000.0;
		}
		else
		{
			return 0;
		}
	}

	static double GetGPUPhysicalMemory()
	{
		if (gpudata.amdinfo)
		{
			return 0;
		}
		else if (gpudata.nvinfo)
		{
			return gpudata.nvinfo->dedicatedVideoMemory / 1024.0;
		}
		else
		{
			return 0;
		}
	}

	static double GetGPUAvailableMemory()
	{
		if (gpudata.amdinfo)
			return 0;
		else if (gpudata.nvinfo)
			return gpudata.nvinfo->curAvailableDedicatedVideoMemory / 1024.0;
		else
			return 0;
	}

	static double GetGPUVirtualMemory()
	{
		if (gpudata.amdinfo)
		{
			return 0;
		}
		else if (gpudata.nvinfo)
		{
			return gpudata.nvinfo->sharedSystemMemory / 1024.0;
		}
		else
		{
			return 0;
		}
	}

	static int GetGPUUsasge()
	{
		if (gpudata.amdinfo)
			return gpudata.amdinfo->OverDrive5.activity.iActivityPercent;
		else if (gpudata.nvinfo)
		{
			if (gpudata.nvinfo->percentage.utilization[0].bIsPresent)
				return gpudata.nvinfo->percentage.utilization[0].percentage;
			else
				return gpudata.nvinfo->Nvidia_Usage.GPUUsage;
		}
		return 0;
	}

	static int GetGPUMemoryControlUsasge()
	{
		if (gpudata.amdinfo)
		{
			return 0;
		}
		else if (gpudata.nvinfo)
		{
			if (gpudata.nvinfo->percentage.utilization[1].bIsPresent)
				return gpudata.nvinfo->percentage.utilization[1].percentage;
			else
				return gpudata.nvinfo->Nvidia_Usage.FrameBufferUsage;
		}
		return 0;
	}

	static int GetGPUVideoEngineUsasge()
	{
		if (gpudata.amdinfo)
		{
			return 0;
		}
		else if (gpudata.nvinfo)
		{
			if (gpudata.nvinfo->percentage.utilization[2].bIsPresent)
				return gpudata.nvinfo->percentage.utilization[2].percentage;
			else
				return gpudata.nvinfo->Nvidia_Usage.VideoEngineUsage;
		}
		return 0;
	}

	static void UpdateDate()
	{
		gpudata.UpdateData();
	}
private:
	static GPUData gpudata;
	explicit GPU(const GPU& x);
	GPU& operator=(const GPU& x);
	GPU() {}
	~GPU() {}
};

GPUData GPU::gpudata;

const std::string& SV_ASSIST::GPU::GPUName()
{
	return ::GPU::GPUName();
}

const std::string& SV_ASSIST::GPU::GPUBIOSVersion()
{
	return ::GPU::GetGPUBIOSVersion();
}

double SV_ASSIST::GPU::GPUtemperature()
{
	return ::GPU::GetGPUtemperature();
}

int SV_ASSIST::GPU::GPUFans(FanTypes & type)
{
	return ::GPU::GetGPUFans(type);
}

double SV_ASSIST::GPU::GPUCurrentClock()
{
	return ::GPU::GetGPUCurrentClock();
}

double SV_ASSIST::GPU::GPUMemoryClock()
{
	return ::GPU::GetGPUMemoryClock();
}

double SV_ASSIST::GPU::GPUPhysicalMemory()
{
	return ::GPU::GetGPUPhysicalMemory();
}

double SV_ASSIST::GPU::GPUVirtualMemory()
{
	return ::GPU::GetGPUVirtualMemory();
}

double SV_ASSIST::GPU::GetGPUAvailableMemory()
{
	return ::GPU::GetGPUAvailableMemory();
}

int SV_ASSIST::GPU::GetGPUUsasge()
{
	return ::GPU::GetGPUUsasge();
}

int SV_ASSIST::GPU::GetGPUMemoryControlUsasge()
{
	return ::GPU::GetGPUMemoryControlUsasge();
}

int SV_ASSIST::GPU::GetGPUVideoEngineUsasge()
{
	return ::GPU::GetGPUVideoEngineUsasge();
}

void SV_ASSIST::GPU::UpdateDate()
{
	::GPU::UpdateDate();
}

