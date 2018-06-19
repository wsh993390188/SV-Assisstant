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
					nExIds_(0), Manufacturer{}, CodeName{}, Technology(0), Family(0), Model(0),
					Stepping(0), ExtFamily(0), ExtModel(0)
{
	memset(&Feature, 0, sizeof(Feature));
	memset(&Cache, 0, sizeof(Cache));
	wmi.ExcuteFun();
}

CPUBASE::~CPUBASE()
{
}
