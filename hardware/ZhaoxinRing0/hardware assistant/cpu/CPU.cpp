// CPU.cpp :
//
#include "stdafx.h"
#include "CPU.h"
#include "Intel.h"
#include "amd.h"
#include "zhaoxin.h"

class CPUDLL
{
	class CPUDLLDATA;
public:
	static CPUDLL* Instance() {
		if (!ttt.get())
			ttt = std::make_shared<CPUDLL>();
		return ttt.get();
	}
	const std::string& GetManufacturer() { return data->cpu->Manufacturer; }
	const short GetFamily() { return data->cpu->Family; }
	const short GetModel() { return data->cpu->Model; }
	const short GetStepping() { return data->cpu->Stepping; }
	const short GetExtFamily() { return data->cpu->ExtFamily; }
	const short GetExtModel() { return data->cpu->ExtModel; }
	const CPUFeature& GetFeature() { return data->cpu->Feature; }
	const Cache_info* GetCache() { return data->cpu->Cache; }

	const std::string& GetCPUName() { return data->cpu->Brand; }
	const std::string& GetProcessorID() { return data->cpu->ProcessorId; }
	const std::string& GetSocketDesignation() { return data->cpu->SocketDesignation; }
	const std::vector<double>& GetCurrentClockSpeed() { return data->cpu->CurrentClockSpeed; }
	const double GetExtClock() 
	{
		if (data->cpu->BusSpeed < INFINITY)
			return data->cpu->BusSpeed;
		else
			return data->cpu->ExtClock*1.0;
	}
	const UINT GetCore() { return data->cpu->Core; }
	const UINT GetThread() { return data->cpu->Thread; }
	const UINT GetRevision() { return data->cpu->Revision; }
	const UINT GetUpgradeMethod() { return data->cpu->UpgradeMethod; }
	const std::string& GetCodename() { return data->cpu->microarchitecture; }
	const std::string& GetSouthbridgeName() { return data->cpu->SouthBridge; }
	const UINT GetTechonology() { return data->cpu->Technology; }
	const double GetPackageTemperature() { return data->cpu->PackageTemperature; }
	const std::vector<double>& GetTemperature(){ return data->cpu->Temperature; }
	void Updatedata(){ data->Update(); }
	CPUDLL() : data(std::make_shared<CPUDLLDATA>())
	{
	}
private:
	static std::shared_ptr<CPUDLL> ttt;
	std::shared_ptr<CPUDLLDATA> data;
	class CPUDLLDATA
	{
	public:
		CPUDLLDATA()
		{
#ifdef ZX_OutputLog
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "********** CPU info **********");
#endif
			switch (CPUBASE::GetCPUBrands())
			{
			case INTEL_:
#ifdef ZX_OutputLog
				Logger::Instance()->OutputLogInfo(el::Level::Debug, "CPU Brands Intel");
#endif
				cpu = make_shared<Intel>();
				break;
			case ZHAOXIN_:
#ifdef ZX_OutputLog
				Logger::Instance()->OutputLogInfo(el::Level::Debug, "CPU Brands Zhaoxin");
#endif
				cpu = make_shared<Zhaoxin>();
				break;
			case AMD_:
#ifdef ZX_OutputLog
				Logger::Instance()->OutputLogInfo(el::Level::Debug, "CPU Brands AMD");
#endif
				cpu = make_shared<AMD>();
				break;
			default:
				break;
			}
			cpu->Init();
			this->Update();
#ifdef ZX_OutputLog
			Logger::Instance()->OutputLogInfo(el::Level::Debug, std::string("Name: ") + cpu->Brand);
			Logger::Instance()->OutputLogInfo(el::Level::Debug, std::string("Manufacturer: ") + cpu->Manufacturer);
			Logger::Instance()->OutputLogInfo(el::Level::Debug, std::string("Codename: ") + cpu->microarchitecture);
			Logger::Instance()->OutputLogInfo(el::Level::Debug, std::string("SouthBridge: ") + cpu->SouthBridge);
			Logger::Instance()->OutputLogInfo(el::Level::Debug, std::string("UpgradeMethod: ") + to_string(cpu->UpgradeMethod));
			Logger::Instance()->OutputLogInfo(el::Level::Debug, std::string("SocketDesignation: ") + cpu->SocketDesignation);
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "********** End CPU info **********\n");
#endif
		}
		void Update()
		{
			cpu->UpDateData();
		}
		std::shared_ptr<CPUBASE> cpu;
	};
};

std::shared_ptr<CPUDLL> CPUDLL::ttt = nullptr;

void SV_ASSIST::CPU::Updatedata()
{
	CPUDLL::Instance()->Updatedata();
}

const std::string& SV_ASSIST::CPU::GetCPUName()
{
	return CPUDLL::Instance()->GetCPUName();
}

const std::string& SV_ASSIST::CPU::GetProcessorID()
{
	return CPUDLL::Instance()->GetProcessorID();
}
const std::string& SV_ASSIST::CPU::GetSocketDesignation()
{
	return CPUDLL::Instance()->GetSocketDesignation();
}

const std::vector<double>& SV_ASSIST::CPU::GetCurrentClockSpeed()
{
	return CPUDLL::Instance()->GetCurrentClockSpeed();
}

const double SV_ASSIST::CPU::GetExtClock()
{
	return CPUDLL::Instance()->GetExtClock();
}

const UINT SV_ASSIST::CPU::GetCore()
{
	return CPUDLL::Instance()->GetCore();
}

const UINT SV_ASSIST::CPU::GetThread()
{
	return CPUDLL::Instance()->GetThread();
}

const UINT SV_ASSIST::CPU::GetRevision()
{
	return CPUDLL::Instance()->GetRevision();
}

const UINT SV_ASSIST::CPU::GetUpgradeMethod()
{
	return CPUDLL::Instance()->GetUpgradeMethod();
}

const std::string& SV_ASSIST::CPU::GetManufacturer()
{
	 return CPUDLL::Instance()->GetManufacturer();
}

const short SV_ASSIST::CPU::GetFamily()
{
	return CPUDLL::Instance()->GetFamily();
}

const short SV_ASSIST::CPU::GetModel()
{
	return CPUDLL::Instance()->GetModel();
}

const short SV_ASSIST::CPU::GetStepping()
{
	return CPUDLL::Instance()->GetStepping();
}

const short SV_ASSIST::CPU::GetExtFamily()
{
	return CPUDLL::Instance()->GetExtFamily();
}

const short SV_ASSIST::CPU::GetExtModel()
{
	return CPUDLL::Instance()->GetExtModel();
}

const CPUFeature& SV_ASSIST::CPU::GetFeature()
{
	return CPUDLL::Instance()->GetFeature();
}

const Cache_info * SV_ASSIST::CPU::GetCache()
{
	return CPUDLL::Instance()->GetCache();
}

const std::string & SV_ASSIST::CPU::GetSouthbridgeName()
{
	return CPUDLL::Instance()->GetSouthbridgeName();
}

const std::string& SV_ASSIST::CPU::GetCodeName()
{
	return CPUDLL::Instance()->GetCodename();
}

const unsigned int SV_ASSIST::CPU::GetTechnology()
{
	return CPUDLL::Instance()->GetTechonology();
}

const double SV_ASSIST::CPU::GetPackageTemperature()
{
	return CPUDLL::Instance()->GetPackageTemperature();
}

const std::vector<double>& SV_ASSIST::CPU::GetTemperature()
{
	return CPUDLL::Instance()->GetTemperature();
}

