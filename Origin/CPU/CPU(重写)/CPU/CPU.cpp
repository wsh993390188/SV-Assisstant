// CPU.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "CPU.h"

class CPUDLL
{
	class CPUDLLDATA;
public:
	static const string GetManufacturer() { return data.cpu->Manufacturer; }
	static const short GetFamily() { return data.cpu->Family; }
	static const short GetModel() { return data.cpu->Model; }
	static const short GetStepping() { return data.cpu->Stepping; }
	static const short GetExtFamily() { return data.cpu->ExtFamily; }
	static const short GetExtModel() { return data.cpu->ExtModel; }
	static const CPUFeature GetFeature() { return data.cpu->Feature; }
	static const Cache_info* GetCache() { return data.cpu->Cache; }

	static const string GetCPUName() { return data.cpu->wmi.Name; }
	static const string GetProcessorID() { return data.cpu->wmi.ProcessorId; }
	static const string GetSocketDesignation() { return data.cpu->wmi.SocketDesignation; }
	static const UINT GetCurrentClockSpeed() { return data.cpu->wmi.CurrentClockSpeed; }
	static const UINT GetExtClock() { return data.cpu->wmi.ExtClock; }
	static const UINT GetCore() { return data.cpu->wmi.Core; }
	static const UINT GetThread() { return data.cpu->wmi.Thread; }
	static const UINT GetRevision() { return data.cpu->wmi.Revision; }
	static const UINT GetMaxClockSpeed() { return data.cpu->wmi.MaxClockSpeed; }
	static const UINT GetUpgradeMethod() { return data.cpu->wmi.UpgradeMethod; }
protected:
private:
	static const CPUDLLDATA data;
	class CPUDLLDATA
	{
	public:
		CPUDLLDATA()
		{
			switch (CPUBASE::GetCPUBrands())
			{
			case INTEL_:
				cpu = make_shared<Intel>();
				break;
			case ZHAOXIN_:
				cpu = make_shared<Zhaoxin>();
				break;
			case AMD_:
				cpu = make_shared<AMD>();
				break;
			default:
				break;
			}
			this->Update();
		}
		void Update()
		{
			cpu->Init();
			cpu->UpDateData();
		}
		std::tr1::shared_ptr<CPUBASE> cpu;
	};
};

const CPUDLL::CPUDLLDATA CPUDLL::data;

void GetCPUName(char* x)
{
	const string a = CPUDLL::GetCPUName();
	strncpy(x, a.c_str(), a.size());
}

void GetProcessorID(char* x)
{
	 const string a = CPUDLL::GetProcessorID();
	 strncpy(x, a.c_str(), a.size());
}
void GetSocketDesignation(char* x)
{
	const string a = CPUDLL::GetSocketDesignation();
	strncpy(x, a.c_str(), a.size());
}

 const UINT GetCurrentClockSpeed()
{
	return CPUDLL::GetCurrentClockSpeed();
}

 const UINT GetExtClock()
{
	return CPUDLL::GetExtClock();
}

 const UINT GetCore()
{
	return CPUDLL::GetCore();
}

 const UINT GetThread()
{
	return CPUDLL::GetThread();
}

 const UINT GetRevision()
{
	return CPUDLL::GetRevision();
}

 const UINT GetMaxClockSpeed()
{
	return CPUDLL::GetMaxClockSpeed();
}

 const UINT GetUpgradeMethodExtClock()
{
	return CPUDLL::GetUpgradeMethod();
}

void GetManufacturer(char *x)
{
	 const string a = CPUDLL::GetManufacturer();
	 strncpy(x, a.c_str(), a.size());
}

 const short GetFamily()
{
	return CPUDLL::GetFamily();
}

 const short GetModel()
{
	return CPUDLL::GetModel();
}

 const short GetStepping()
{
	return CPUDLL::GetStepping();
}

 const short GetExtFamily()
{
	return CPUDLL::GetExtFamily();
}

 const short GetExtModel()
{
	return CPUDLL::GetExtModel();
}

 const CPUFeature GetFeature()
{
	return CPUDLL::GetFeature();
}

 const Cache_info * GetCache()
{
	return CPUDLL::GetCache();
}

