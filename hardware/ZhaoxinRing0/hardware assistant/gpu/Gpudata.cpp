#include "stdafx.h"
#include "Nvidia.h"
#include "AMDgpu.h"
#include "Intelgpu.h"

GPUData::GPUData() : amdinfo(nullptr), nvinfo(nullptr), Intelinfo(nullptr), Types(0)
{
#ifdef ZX_OutputLog
	Logger::Instance()->OutputLogInfo(el::Level::Debug, "********** GPU info **********");
#endif 
	nv_gpu = std::make_shared<CNvidia>();
	if (nv_gpu->exec() == NVIDIA_GPU)
	{
		nvinfo = reinterpret_cast<const vector<NvidiaInfo>*>(nv_gpu->Returninfo());
		Types |= NVIDIA_GPU;
		NV_DriverVer = nv_gpu->GetDriverVersion();
		NV_BranchVersion = nv_gpu->GetBranchVersion();
#ifdef ZX_OutputLog
		int i = 0;
		for (auto var : *(this->nvinfo))
		{
			boost::format ft = boost::format("Exist Nvidia GPU #%1% Name: ") % i++;
			Logger::Instance()->OutputLogInfo(el::Level::Debug, ft.str() + var.FullName);
		}
#endif
	}
	else
		nv_gpu = nullptr;

	amd_gpu = std::make_shared<CAMD>();
	if (amd_gpu->exec() == AMD_GPU)
	{
		amdinfo = reinterpret_cast<const vector<AMDINFO>*> (amd_gpu->Returninfo());
		Types |= AMD_GPU;
		AMD_DriverVer = amd_gpu->GetDriverVersion();
		AMD_BranchVersion = amd_gpu->GetBranchVersion();
#ifdef ZX_OutputLog
		int i = 0;
		for (auto var : *(this->amdinfo))
		{
			boost::format ft = boost::format("Exist AMD GPU #%1% Name: ") % i++;
			Logger::Instance()->OutputLogInfo(el::Level::Debug, ft.str() + var.FullName);
		}
#endif
	}
	else
		amd_gpu = nullptr;

	intel_gpu = std::make_shared<CIntelGPU>();
	if (intel_gpu->exec() == INTEL_GPU)
	{
		Intelinfo = reinterpret_cast<const vector<IntelGPUInfo>*> (intel_gpu->Returninfo());
		Types |= INTEL_GPU;
		IN_DriverVer = intel_gpu->GetDriverVersion();
		IN_BranchVersion = intel_gpu->GetBranchVersion();
#ifdef ZX_OutputLog
		int i = 0;
		for (auto var : *(this->amdinfo))
		{
			boost::format ft = boost::format("Exist Intel GPU #%1% Name: ") % i++;
			Logger::Instance()->OutputLogInfo(el::Level::Debug, ft.str() + var.FullName);
		}
#endif
	}
	else
		intel_gpu = nullptr;
#ifdef ZX_OutputLog
	Logger::Instance()->OutputLogInfo(el::Level::Debug, "********** End GPU info **********\n");
#endif
}

GPUData::~GPUData()
{
}

void GPUData::UpdateData()
{
	if (Types & 0x1)
		nv_gpu->UpdateData();
	
	if (Types & 0x02)
		amd_gpu->UpdateData();

	if (Types & 0x04)
		intel_gpu->UpdateData();
}

CGPU::CGPU() : DriverVer{}, BranchVersion{}
{
}

CGPU::~CGPU()
{
}

const std::string CGPU::GetDriverVersion()
{
	return this->DriverVer;
}

const std::string CGPU::GetBranchVersion()
{
	return this->BranchVersion;
}
