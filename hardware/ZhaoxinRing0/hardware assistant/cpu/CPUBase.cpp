#include "stdafx.h"
#include <fstream>
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
					Name{""}, ProcessorId("Unknown"), SocketDesignation("Unknown"), SouthBridge("Unknown"), MaxTDP(0),
					ExtClock(0), Core(0), Thread(0), Revision(0), UpgradeMethod(0), PackageTemperature{INFINITY}, BusSpeed{INFINITY}
{
	InitCPUDB();
	memset(&Feature, 0, sizeof(Feature));
	memset(&Cache, 0, sizeof(Cache));
	CWMI wmi;
	wmi.ExcuteFun();
	this->ProcessorId = wmi.ProcessorId;
	this->ExtClock = wmi.ExtClock;
	this->Core = wmi.Core;
	this->Thread = wmi.Thread;
	this->Revision = wmi.Revision;
	for (UINT i = 0; i < this->Core; i++)
		this->CurrentClockSpeed.emplace_back(INFINITY);
}

size_t CPUBASE::findcpuid(int value)
{
	auto& it = find(nIdsLeaf.begin(), nIdsLeaf.end(), value);
	if (it != nIdsLeaf.end())
	{
		return distance(nIdsLeaf.begin(), it);
	}
	return -1;
}

void CPUBASE::InitCPUDB()
{
	std::ifstream in("cpu.ids");
	std::string line, item;

	if (!in.is_open())
	{
		return ;
	}
	string cpuFM = {};
	while (std::getline(in, line)) {
		// Ignore any line starting with #
		if (line.size() == 0 || line[0] == '#')
			continue;

		if (line[0] == '\t' && line[1] == '\t')
		{
			string temp = line.substr(2);
			int technology = 0, TDP = 0;
			char a[0x20] = {};
			sscanf_s(temp.c_str(), "%s\t", a, _countof(a));
			temp = temp.substr(strlen(a) + 1);
			vector<string> itemlist;
			this->split(temp, ",", itemlist);
			CPUDB[cpuFM][string(a)] = itemlist;
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

void CPUBASE::split(const std::string & s, const std::string & delim, std::vector<std::string>& ret)
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

CPUBASE::~CPUBASE()
{
}
