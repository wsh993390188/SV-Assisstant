#pragma once

namespace SV_ASSIST
{
	namespace GPU
	{
		enum FanTypes
		{
			Percent = 0,
			RPM = 1
		};

		void GPUName(char* x);
		void GPUBIOSVersion(char* x);
		double GPUtemperature();
		int GPUFans(FanTypes & type);
		double GPUCurrentClock();
		double GPUMemoryClock();
		double GPUPhysicalMemory();
		double GPUVirtualMemory();
		double GetGPUAvailableMemory();
		int GetGPUUsasge();
		int GetGPUMemoryControlUsasge();
		int GetGPUVideoEngineUsasge();

		void UpdateDate();
	}
}

