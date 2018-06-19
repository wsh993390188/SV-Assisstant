#pragma once
#include "cpulibdefination.h"
namespace SV_ASSIST
{
	namespace CPU
	{
		void GetCPUName(char* x);
		void GetProcessorID(char* x);
		void GetSocketDesignation(char* x);
		const unsigned int GetCurrentClockSpeed();
		const unsigned int GetExtClock();
		const unsigned int GetCore();
		const unsigned int GetThread();
		const unsigned int GetRevision();
		const unsigned int GetMaxClockSpeed();
		const unsigned int GetUpgradeMethodExtClock();

		void GetManufacturer(char* x);
		const short GetFamily();
		const short GetModel();
		const short GetStepping();
		const short GetExtFamily();
		const short GetExtModel();

		const CPUFeature GetFeature();
		const Cache_info* GetCache();
	}
}
