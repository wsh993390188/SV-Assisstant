#include "../stdafx.h"
#include "definition.h"
// 
// CPU_RES::CPU_RES()
// {
// 
// }
// 
// CPU_RES::CPU_RES(const CPU_RES &x)
// {
// 	this->CPU_LEAF_NAME.assign(x.CPU_LEAF_NAME.begin(), x.CPU_LEAF_NAME.end());
// 	this->CPU_EAX.assign(x.CPU_EAX.begin(), x.CPU_EAX.end());
// 	this->CPU_EBX.assign(x.CPU_EBX.begin(), x.CPU_EBX.end());
// 	this->CPU_ECX.assign(x.CPU_ECX.begin(), x.CPU_ECX.end());
// 	this->CPU_EDX.assign(x.CPU_EDX.begin(), x.CPU_EDX.end());
// }

void CPUID_Data::erase()
{

	::ZeroMemory(&this->Architectural_performance_monitoring, sizeof(this->Architectural_performance_monitoring));
	::ZeroMemory(&this->Processor_extended_state, sizeof(this->Processor_extended_state));
	::ZeroMemory(&this->Extened_Feature, sizeof(this->Extened_Feature));
	::ZeroMemory(&this->Power_Mgmt, sizeof(this->Power_Mgmt));
	this->EffFreq = 0;
	for (auto& i : this->Extended_topology_enumeration)
	{
		i.erase();
	}
	for (auto& i : this->C_STATE_info)
	{
		i = 0;
	}
	this->Smallest_monitor_line_size = 0;
	this->Largest_monitor_line_size = 0;
	this->IBE = 0;
	this->EMX = 0;
	this->Intel_cache.clear();
	this->AMD_cache.clear();
	this->Cache_TLB.clear();
	::ZeroMemory(&this->Feature_info, sizeof(this->Feature_info));
	this->CPU_Type.clear();
	this->Family = 0;
	this->Model = 0;
	this->Stepping = 0;
	this->Ext_Family = 0;
	this->Ext_Model = 0;
	for (auto& i : this->Manufacturer)
	{
		i = 0;
	}
	this->Architecture.clear();
	this->ProcessorID.clear();
	this->Name.clear();
	this->Code_Name.clear();
	this->SocketDesignation.clear();
	this->Core = 0;
	this->Thread = 0;
	this->Revision = 0;
	this->MaxClockSpeed = 0;
	this->UpgradeMethod = 0;
	this->ExtClock = 0;
	this->CurrentClockSpeed = 0;
	this->Technology = 0;
	this->MAXTDP = 0;
}

// CPUID_Data & CPUID_Data::operator=(const CPUID_Data & x)
// {
// 	// TODO: 在此处插入 return 语句
// 	CPUID_Data temp(x);
// 	swap(x);
// 	return *this;
// }
// 
// void CPUID_Data::swap(const CPUID_Data & x)
// {
// 	if(x.AMD_cache.size() != 0)
// 		this->AMD_cache.assign(x.AMD_cache.begin(), x.AMD_cache.end());
// 	this->Architectural_performance_monitoring = x.Architectural_performance_monitoring;
// 	this->Architecture.assign(x.Architecture.begin(), x.Architecture.end());
// 	for (short i = 0; i < 7; i++)
// 		this->C_STATE_info[i] = x.C_STATE_info[i];
// 	if (x.Cache_TLB.size() != 0)
// 		this->Cache_TLB.assign(x.Cache_TLB.begin(), x.Cache_TLB.end());
// 	this->Code_Name = x.Code_Name;
// 	this->Core = x.Core;
// 	this->CPU_Type = x.CPU_Type;
// 	this->CurrentClockSpeed = x.CurrentClockSpeed;
// 	this->EffFreq = x.EffFreq;
// 	this->Ext_Family = x.Ext_Family;
// 	this->EMX = x.EMX;
// 	this->Ext_Model = x.Ext_Model;
// 	this->ExtClock = x.ExtClock;
// 	this->Extened_Feature = x.Extened_Feature;
// 	for (int i = 0; i < 4; i++)
// 	{
// 		this->Extended_topology_enumeration[i] = x.Extended_topology_enumeration[i];
// 	}
// 	this->Family = x.Family;
// 	this->Feature_info = x.Feature_info;
// 	this->IBE = x.IBE;
// 	if(x.Intel_cache.size() != 0)
// 		this->Intel_cache.assign(x.Intel_cache.begin(), x.Intel_cache.end());
// 	this->Largest_monitor_line_size = x.Largest_monitor_line_size;
// 	memcpy(this->Manufacturer, x.Manufacturer, 32);
// 	this->MaxClockSpeed = x.MaxClockSpeed;
// 	this->MAXTDP = x.MAXTDP;
// 	this->Model = x.Model;
// 	this->Name = x.Name;
// 	this->Power_Mgmt = x.Power_Mgmt;
// 	this->Processor_extended_state = x.Processor_extended_state;
// 	this->ProcessorID = x.ProcessorID;
// 	this->Stepping = x.Stepping;
// 	this->Revision = x.Revision;
// 	this->Smallest_monitor_line_size = x.Smallest_monitor_line_size;
// 	this->SocketDesignation = x.SocketDesignation;
// 	this->Technology = x.Technology;
// 	this->Thread = x.Thread;
// 	this->UpgradeMethod = x.UpgradeMethod;
// }

void CPU_RES::erase()
{
	this->CPU_LEAF_NAME.clear();
	this->CPU_EAX.clear(); 
	this->CPU_EBX.clear();
	this->CPU_ECX.clear();
	this->CPU_EDX.clear();
}

void AMD_Cache_info::erase()
{
	this->Cache_type.clear();
	this->Cache_level = 0;
	this->Cache_line_per_tag = 0;
	this->Cache_line_size = 0;
	this->Cache_Size = 0;
	this->Cache_Ways = 0;
}

void EXTENDED_TOPOLOGY_ENUMERATION::erase()
{
	this->Level_Type.clear();
	this->Number_of_Logical_Processor = 0;
	this->Shift_X2APIC_ID = 0;
	this->X2APIC_ID = 0;
}

void Cache_info::erase()
{
	this->Cache_type.clear();
	this->Cache_Complex_Indexing = 0;
	this->Cache_Inclusiveness = 0;
	this->CACHE_Invalidate = 0;
	this->Cache_level = 0;
	this->Cache_line_size = 0;
	this->Cache_max_addr_logical = 0;
	this->Cache_max_addr_physical = 0;
	this->Cache_partitions = 0;
	this->Cache_Set = 0;
	this->Cache_Size = 0;
	this->Cache_Ways = 0;
}
