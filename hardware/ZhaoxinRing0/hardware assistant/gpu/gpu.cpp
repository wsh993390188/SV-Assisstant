#include "stdafx.h"
#pragma warning(disable:4996)
#include <fstream>
#include <map>
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
	static GPU* Instance()
	{
		if (!temp.get())
			temp = std::make_shared<GPU>();
		return temp.get();
	}
	void UpdateData()
	{
		gpuinfo.clear();
		gpudata->UpdateData();
		GetInfo();
	}
	const std::vector<IgpuSensor>* ReturnGPUInfo()
	{
		return &this->gpuinfo;
	}
	GPU() : gpudata(std::make_shared<GPUData>())
	{
		this->InitGPUDB();
	}
private:
	explicit GPU(const GPU& x);
	GPU& operator=(const GPU& x);
	void InitGPUDB()
	{
		std::ifstream in("gpu.ids");
		std::string line, item;

		if (!in.is_open())
		{
			return;
		}
		std::string cpuFM = {};
		while (std::getline(in, line)) {
			// Ignore any line starting with #
			if (line.size() == 0 || line[0] == '#')
				continue;

			if (line[0] == '\t' && line[1] == '\t')
			{
				std::string temp = line.substr(2);
				int technology = 0, TDP = 0;
				char a[0x20] = {};
				sscanf_s(temp.c_str(), "%s\t", a, _countof(a));
				temp = temp.substr(strlen(a) + 1);
				std::vector<std::string> itemlist;
				this->split(temp, ",", itemlist);
				GPUDB[cpuFM][std::string(a)] = itemlist;
				continue;
			}
			if (line[0] == '\t')
			{
				cpuFM = line.substr(1);
				continue;
			}
		}
		in.close();
	}

	void split(const std::string & s, const std::string & delim, std::vector<std::string>& ret)
	{
		size_t last = 0;
		size_t index = s.find_first_of(delim, last);
		while (index != std::string::npos)
		{
			ret.emplace_back(s.substr(last, index - last));
			last = index + 1;
			index = s.find_first_of(delim, last);
		}
		if (index - last > 0)
		{
			ret.emplace_back(s.substr(last, index - last));
		}
	}

	void GetInfo()
	{
		if(gpudata->nvinfo)
		for(auto var : *(gpudata->nvinfo))
		{
			GPUSensor temp = {};
			temp.SetGPUDriverVersion(gpudata->NV_DriverVer);
			temp.SetGPUBranchVersion(gpudata->NV_BranchVersion);
			temp.SetGPUName(var.FullName);
			temp.SetGPUBiosVersion(var.VbiosVersion);
			temp.SetGPUTemperature(var.Device_Tem.GPUCurrTem);
			temp.SetGPUfans(var.fans);
			temp.SetGPUClock(var.Device_clock.GraphicsCurrent / 1000.0);
			temp.SetGPUMemoryClock(var.Device_clock.MemoryCurrent / 1000.0);
			temp.SetGPUusage(var.percentage.utilization[0].bIsPresent ? var.percentage.utilization[0].percentage : var.Nvidia_Usage.GPUUsage);
			temp.SetsharedSystemMemory(var.sharedSystemMemory);
			temp.SetdedicatedVideoMemory(var.dedicatedVideoMemory);
			temp.SetsystemVideoMemory(var.systemVideoMemory);
			this->gpuinfo.emplace_back(IgpuSensor(temp));
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
			this->gpuinfo.emplace_back(IgpuSensor(temp));
		}

		if (gpudata->Intelinfo)
			for (auto var : *(gpudata->Intelinfo))
			{
				GPUSensor temp = {};
				temp.SetGPUBranchVersion(gpudata->AMD_BranchVersion);
				temp.SetGPUDriverVersion(gpudata->AMD_DriverVer);
				temp.SetGPUName(WStringToString(var.FullName));
				this->gpuinfo.emplace_back(IgpuSensor(temp));
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
	std::shared_ptr<GPUData> gpudata;
	std::map<std::string, std::map<std::string, std::vector<std::string>>> GPUDB; //gpuÊý¾Ý¿â
	static std::shared_ptr<GPU> temp;
};

std::shared_ptr<GPU> GPU::temp = nullptr;

const std::vector<IgpuSensor>& SV_ASSIST::GPU::GetGpuInfo()
{
	return *::GPU::Instance()->ReturnGPUInfo();
}

void SV_ASSIST::GPU::UpdateDate()
{
	::GPU::Instance()->UpdateData();
}

