#pragma once
#include "Gpudata.h"

class CIntelGPU : public CGPU
{
public:
	CIntelGPU();
	using CGPU::CGPU;
	~CIntelGPU();
	bool CheckdllStatus();
	long getIntelDeviceInfo(unsigned int VendorId, IntelDeviceInfoHeader * pIntelDeviceInfoHeader, void * pIntelDeviceInfoBuffer);
	GPUTypes exec();
	const void* Returninfo();
	GPUTypes UpdateData();
private:
	std::vector<IntelGPUInfo> Intelinfo;
	bool getGraphicsDeviceInfo(unsigned int* VendorId,
		unsigned int* DeviceId,
		unsigned __int64* VideoMemory,
		std::wstring* GFXBrand);
};