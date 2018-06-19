#pragma once
#include "Gpudata.h"

class CIntelGPU : public CGPU
{
public:
	CIntelGPU();
	~CIntelGPU();
	bool CheckdllStatus();
	CGPU::GPUTypes exec();
	const void* Returninfo();
	CGPU::GPUTypes UpdateData();
private:
	IntelGPUInfo Intelinfo;
	bool getGraphicsDeviceInfo(unsigned int* VendorId,
		unsigned int* DeviceId,
		unsigned __int64* VideoMemory,
		std::wstring* GFXBrand);
};