#include "stdafx.h"
#pragma warning(disable:4996)
#include "gpu.h"
#include "Gpudata.h"
#include "Igpu.h"

class GPUSensor : public IgpuSensor
{
public:
	using IgpuSensor::IgpuSensor;
	BOOL SetGPUName(const std::string& name)
	{
		BOOL status = FALSE;
		if (name.size())
		{
			this->GPUname = name;
			status = TRUE;
		}
		return status;
	}

	BOOL SetGPUBiosVersion(const std::string& biosversion)
	{
		BOOL status = FALSE;
		if (biosversion.size())
		{
			this->GPUBiosVersion = biosversion;
			status = TRUE;
		}
		return status;
	}

	BOOL SetGPUTemperature(INT Temperature)
	{
		BOOL status = FALSE;
		if (Temperature)
		{
			this->Temperature = Temperature;
			status = TRUE;
		}
		return status;
	}

	BOOL SetGPUClock(double gpuclock)
	{
		BOOL status = FALSE;
		if (gpuclock)
		{
			this->GPUClock = gpuclock;
			status = TRUE;
		}
		return status;
	}

	BOOL SetGPUMemoryClock(double gpumemoryclock)
	{
		BOOL status = FALSE;
		if (gpumemoryclock)
		{
			this->GPUMemclock = gpumemoryclock;
			status = TRUE;
		}
		return status;
	}
	BOOL SetGPUusage(ULONG gpusage)
	{
		BOOL status = FALSE;
		if (gpusage)
		{
			this->GPUusage = gpusage;
			status = TRUE;
		}
		return status;
	}
	BOOL SetGPUfans(ULONG fans)
	{
		BOOL status = FALSE;
		if (fans)
		{
			this->fans = fans;
			status = TRUE;
		}
		return status;
	}

	BOOL SetGPUDriverVersion(const std::string& DriverVer)
	{
		BOOL status = FALSE;
		if (!DriverVer.empty())
		{
			this->GPUDriverVersion = DriverVer;
			status = TRUE;
		}
		return status;
	}

	BOOL SetGPUBranchVersion(const std::string& BranchVer)
	{
		BOOL status = FALSE;
		if (!BranchVer.empty())
		{
			this->GPUBranchVersion = BranchVer;
			status = TRUE;
		}
		return status;
	}

	BOOL SetsharedSystemMemory(size_t mem)
	{
		BOOL status = FALSE;
		if (mem)
		{
			this->sharedSystemMemory = mem;
			status = TRUE;
		}
		return status;
	}

	BOOL SetsystemVideoMemory(size_t mem)
	{
		BOOL status = FALSE;
		if (mem)
		{
			this->systemVideoMemory = mem;
			status = TRUE;
		}
		return status;
	}

	BOOL SetdedicatedVideoMemory(size_t mem)
	{
		BOOL status = FALSE;
		if (mem)
		{
			this->dedicatedVideoMemory = mem;
			status = TRUE;
		}
		return status;
	}
private:
};

class GPU
{
public:
	static GPU Instance;
	void UpdateData()
	{
		gpuinfo.clear();
		gpudata->UpdateData();
		GetInfo();
	}
	const std::vector<IgpuSensor>& ReturnGPUInfo()
	{
		return this->gpuinfo;
	}
private:
	void GetInfo()
	{
		if(gpudata->nvinfo)
		for(auto var : *(gpudata->nvinfo))
		{
			GPUSensor temp = {};
			temp.SetGPUDriverVersion(gpudata->NV_DriverVer);
			temp.SetGPUBranchVersion(gpudata->NV_BranchVersion);
			temp.SetGPUName(var.FUllName);
			temp.SetGPUBiosVersion(var.VbiosVersion);
			temp.SetGPUTemperature(var.Device_Tem.GPUCurrTem);
			temp.SetGPUfans(var.fans);
			temp.SetGPUClock(var.Device_clock.GraphicsCurrent / 1000.0);
			temp.SetGPUMemoryClock(var.Device_clock.MemoryCurrent / 1000.0);
			temp.SetGPUusage(var.percentage.utilization[0].bIsPresent ? var.percentage.utilization[0].percentage : var.Nvidia_Usage.GPUUsage);
			temp.SetsharedSystemMemory(var.sharedSystemMemory);
			temp.SetdedicatedVideoMemory(var.dedicatedVideoMemory);
			temp.SetsystemVideoMemory(var.systemVideoMemory);
			this->gpuinfo.emplace_back(temp);
		}
		if (gpudata->amdinfo)
		for (auto var : *(gpudata->amdinfo))
		{
			GPUSensor temp = {};
			temp.SetGPUBranchVersion(gpudata->AMD_BranchVersion);
			temp.SetGPUDriverVersion(gpudata->AMD_DriverVer);
			temp.SetGPUName(var.FullName);
			temp.SetGPUBiosVersion(var.biosInfo.strVersion);
			temp.SetGPUTemperature(var.OverDrive5.TemperatureAndFans.adlTemperature.iTemperature / 1000);
			temp.SetGPUfans(var.OverDrive5.TemperatureAndFans.fanSpeedValue.iFanSpeed);
			temp.SetGPUClock(var.OverDrive5.activity.iEngineClock / 100.0);
			temp.SetGPUMemoryClock(var.OverDrive5.activity.iMemoryClock / 1000.0);
			temp.SetGPUusage(var.OverDrive5.activity.iActivityPercent);
			temp.SetsharedSystemMemory(var.sharedSystemMemory);
			temp.SetdedicatedVideoMemory(var.dedicatedVideoMemory);
			temp.SetsystemVideoMemory(var.systemVideoMemory);
			this->gpuinfo.emplace_back(temp);
		}

		if (gpudata->Intelinfo)
			for (auto var : *(gpudata->Intelinfo))
			{
				GPUSensor temp = {};
				temp.SetGPUBranchVersion(gpudata->AMD_BranchVersion);
				temp.SetGPUDriverVersion(gpudata->AMD_DriverVer);
				temp.SetGPUName(WStringToString(var.FUllName));
				this->gpuinfo.emplace_back(temp);
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
	std::vector<IgpuSensor> gpuinfo;
	shared_ptr<GPUData> gpudata;
	explicit GPU(const GPU& x);
	GPU& operator=(const GPU& x);
	GPU() : gpudata(make_shared<GPUData>()) { }
	~GPU() {}
};

GPU GPU::Instance;

const std::vector<IgpuSensor>& SV_ASSIST::GPU::GetGpuInfo()
{
	return ::GPU::Instance.ReturnGPUInfo();
}

void SV_ASSIST::GPU::UpdateDate()
{
	::GPU::Instance.UpdateData();
}

