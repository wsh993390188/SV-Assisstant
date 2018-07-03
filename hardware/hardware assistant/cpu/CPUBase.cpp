#include "stdafx.h"
#include "CPUBase.h"
#include "CPUWMI.h"

using namespace std;

CPU_Brands CPUBASE::GetCPUBrands()
{
	array<int, 4> buf;
	__cpuidex(buf.data(), 0, 0);
	char vendor[0x20];
	memset(vendor, 0, 0x20);
	*reinterpret_cast<int*>(vendor) = buf[1];
	*reinterpret_cast<int*>(vendor + 4) = buf[3];
	*reinterpret_cast<int*>(vendor + 8) = buf[2];
	string vendor_ = vendor;
	if (vendor_ == "GenuineIntel")
		return INTEL_;
	else if (vendor_ == "AuthenticAMD")
		return AMD_;
	else if (vendor_ == "CentaurHauls")
		return ZHAOXIN_;
	else
		return OTHERS_;
}

CPUBASE::CPUBASE() : data__{}, extdata__{}, nIdsLeaf{}, nIds_(0),
					nExIds_(0), Manufacturer{}, Technology(0), Family(0), Model(0),
					Stepping(0), ExtFamily(0), ExtModel(0), microarchitecture{}, Brand{},
					ProcessorId("Unknown"), SocketDesignation("Unknown"),
					CurrentClockSpeed(0), ExtClock(0), Core(0), Thread(0), Revision(0), MaxClockSpeed(0), UpgradeMethod(0)
{
	memset(&Feature, 0, sizeof(Feature));
	memset(&Cache, 0, sizeof(Cache));
	CWMI wmi;
	wmi.ExcuteFun();
	this->ProcessorId = wmi.ProcessorId;
	this->SocketDesignation = wmi.SocketDesignation;
	this->CurrentClockSpeed = wmi.CurrentClockSpeed;
	this->ExtClock = wmi.ExtClock;
	this->Core = wmi.Core;
	this->Thread = wmi.Thread;
	this->Revision = wmi.Revision;
	this->MaxClockSpeed = wmi.MaxClockSpeed;
	this->UpgradeMethod = wmi.UpgradeMethod;
}

CPUBASE::~CPUBASE()
{
}
