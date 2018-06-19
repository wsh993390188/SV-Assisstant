#pragma once
class IgpuSensor
{
protected:
	IgpuSensor();
	~IgpuSensor();
	const std::string& GetGPUName();
	const std::string& GetGPUBiosVersion();
	INT GetTemperature();
	double GetGPUClock();
	double GetGPUMemoryClock();
	ULONG GetGPUusage();
	ULONG GetGPUfans();

	BOOL SetGPUName(std::string name);
	BOOL SetGPUBiosVersion(std::string biosversion);
	BOOL SetTemperature(INT Temperature);
	BOOL SetGPUClock(double gpuclock);
	BOOL SetGPUMemoryClock(double gpumemoryclock);
	BOOL SetGPUusage(ULONG gpusage);
	BOOL SetGPUfans(ULONG fans);
private:
	std::string GPUname;
	std::string GPUBiosVersion;
	INT	Temperature;
	double GPUClock;
	double GPUMemclock;
	ULONG GPUusage;
	ULONG fans;
};

