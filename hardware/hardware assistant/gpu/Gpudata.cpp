#include "stdafx.h"
#include "Nvidia.h"
#include "AMDgpu.h"
#include "Intelgpu.h"

GPUData::GPUData() : amdinfo(nullptr), nvinfo(nullptr), Intelinfo{}
{
	m_gpu = std::make_shared<CNvidia>();
	if (m_gpu->exec() == CGPU::NVIDIA_GPU)
	{
		nvinfo = reinterpret_cast<const NvidiaInfo *>(m_gpu->Returninfo());
	}
	else
	{
		m_gpu.reset(new CAMD);
		if (m_gpu->exec() == CGPU::AMD_GPU)
		{
			amdinfo = reinterpret_cast<const AMDINFO*>(m_gpu->Returninfo());
		}
		else
		{
			m_gpu.reset(new CIntelGPU);
			if (m_gpu->exec() == CGPU::INTEL_GPU)
			{
				Intelinfo = reinterpret_cast<const IntelGPUInfo*>(m_gpu->Returninfo());
			}
			else
				m_gpu.reset();
		}
	}
}

GPUData::~GPUData()
{
}

void GPUData::UpdateData()
{
	switch (m_gpu->UpdateData())
	{
	case CGPU::AMD_GPU:
		amdinfo = reinterpret_cast<const AMDINFO*>(m_gpu->Returninfo());
		break;
	case CGPU::NVIDIA_GPU:
		nvinfo = reinterpret_cast<const NvidiaInfo*>(m_gpu->Returninfo());
		break;
	case CGPU::INTEL_GPU:
		Intelinfo = reinterpret_cast<const IntelGPUInfo*>(m_gpu->Returninfo());
		break;
	default:
		break;
	}
}

CGPU::CGPU()
{
}

CGPU::~CGPU()
{
}
