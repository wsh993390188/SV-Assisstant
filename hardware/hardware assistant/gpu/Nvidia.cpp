#include "stdafx.h"
#include "Nvidia.h"

/************************************************************************/
/*                         Load Area                                    */
/************************************************************************/

CNvidia::CNvidia()
{
	ZeroMemory(&hDisplay, sizeof(hDisplay));
	ZeroMemory(&phys, sizeof(phys));
	nvinfo = {};
	m_succeed_Nvidia = NvAPI_Initialize();
}

CNvidia::~CNvidia()
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Unload();
	}
}

bool CNvidia::CheckdllStatus()
{
	return (m_succeed_Nvidia == NVAPI_OK) ? TRUE : FALSE;
}
const void* CNvidia::Returninfo()
{
	return &this->nvinfo;
}

CGPU::GPUTypes CNvidia::UpdateData()
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NV_DISPLAY_DRIVER_MEMORY_INFO mem = {};
		this->GetGPUMem(mem, 0);
		nvinfo.dedicatedVideoMemory = mem.dedicatedVideoMemory;
		nvinfo.availableDedicatedVideoMemory = mem.availableDedicatedVideoMemory;
		nvinfo.dedicatedVideoMemoryEvictionCount = mem.dedicatedVideoMemoryEvictionCount;
		nvinfo.dedicatedVideoMemoryEvictionsSize = mem.dedicatedVideoMemoryEvictionsSize;
		nvinfo.curAvailableDedicatedVideoMemory = mem.curAvailableDedicatedVideoMemory;
		nvinfo.sharedSystemMemory = mem.sharedSystemMemory;
		nvinfo.systemVideoMemory = mem.systemVideoMemory;

		this->GetGPUFans(0, nvinfo.fans);
		this->GetGPUClock(0);
		this->GetDynamicPstatesInfo(0, nvinfo.percentage);
		this->GetDynamicInfo(0);
	}
	return CGPU::GPUTypes::NVIDIA_GPU;
}

CGPU::GPUTypes CNvidia::exec()
{
	CGPU::GPUTypes re = CGPU::OTHERS_GPU;
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		re = CGPU::NVIDIA_GPU;
		BOOL status = NVAPI_ERROR;
		NV_DISPLAY_DRIVER_MEMORY_INFO mem = {};
		status = this->EnumPhysicalGPUs();
		if (status != NVAPI_OK)
		{
			NvAPI_ShortString str;
			NvAPI_GetErrorMessage((NvAPI_Status)status, str);
		}
		status = this->EnumNvidiaDisplayHandle();
		if (status != NVAPI_OK)
		{
			NvAPI_ShortString str;
			NvAPI_GetErrorMessage((NvAPI_Status)status, str);
		}
		nvinfo.physcount = this->physcount;
		this->GetInterfaceVersion(nvinfo.InterfaceVersion);
		this->GetsysDriverbranchVersion(nvinfo.DriverVersion, nvinfo.BranchVersion);
		this->GetGPUtemperature(0, nvinfo.Device_Tem.GPUMinTem, nvinfo.Device_Tem.GPUCurrTem, nvinfo.Device_Tem.GPUMAXtem);
		this->GetGPUFullName(nvinfo.FUllName, 0);
		this->GetGpuType(0, nvinfo.GPUType);
		this->GetGPUMem(mem, 0);
		nvinfo.dedicatedVideoMemory = mem.dedicatedVideoMemory;
		nvinfo.availableDedicatedVideoMemory = mem.availableDedicatedVideoMemory;
		nvinfo.dedicatedVideoMemoryEvictionCount = mem.dedicatedVideoMemoryEvictionCount;
		nvinfo.dedicatedVideoMemoryEvictionsSize = mem.dedicatedVideoMemoryEvictionsSize;
		nvinfo.curAvailableDedicatedVideoMemory = mem.curAvailableDedicatedVideoMemory;
		nvinfo.sharedSystemMemory = mem.sharedSystemMemory;
		nvinfo.systemVideoMemory = mem.systemVideoMemory;
		this->GetGPUFans(0, nvinfo.fans);
		this->GetGPUClock(0);
		this->GetHDCPSupport(0, nvinfo.HDCP);
		this->GetGpuCoreCount(0, nvinfo.corecount);
		this->GetDynamicPstatesInfo(0, nvinfo.percentage);
		this->GetVbiosOEMRevision(0, nvinfo.VbiosOEMRevision);
		this->GetVbiosRevision(0, nvinfo.VbiosRevision);
		this->GetVbiosVersion(0, nvinfo.VbiosVersion);
		this->GetBUSID(0, nvinfo.BusID);
		this->GetBUSSlotID(0, nvinfo.BuSlotID);
		this->GetBusType(0, nvinfo.BusType);
		this->GetAGPAperture(0, nvinfo.AGPAperture);
		this->GetCurrentAGPRate(0, nvinfo.AGPRate);
		this->GetSysType(0, nvinfo.SysType);
		this->GetPhysicalFrameBufferSize(0, nvinfo.PhysicalFrame);
		this->GetVirtualFrameBufferSize(0, nvinfo.VirtualFrame);
		this->GetGPUPState(0);
		this->GetCurrentPState(0, nvinfo.CurrentPState);
		this->GetChipInfo();
		this->GetECCConfigurationInfo(0);
		this->GetPCIEWidth(0);
		this->GetDynamicInfo(0);
	}
	return re;
}

BOOL CNvidia::EnumPhysicalGPUs()
{
	NvAPI_Status status = NVAPI_ERROR;
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		status = NvAPI_EnumPhysicalGPUs(phys, &physcount);
		if (status == NVAPI_OK)
		{
			return TRUE;
		}
	}
	return status;
}

BOOL CNvidia::EnumNvidiaDisplayHandle()
{
	NvAPI_Status status = NVAPI_ERROR;
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		INT i = 0;
		do 
		{
			status = NvAPI_EnumNvidiaDisplayHandle(i, &hDisplay[i]);
			++i;
		} while (status == NVAPI_OK);
		if (i > 0)
		{
			return TRUE;
		}
	}
	return status;
}

/************************************************************************/
/*                         Function Area                                */
/************************************************************************/

BOOL CNvidia::GetInterfaceVersion(string& version)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvAPI_ShortString ver = "";
		status = NvAPI_GetInterfaceVersionString(ver);
		if (status == NVAPI_OK)
		{
			version = ver;
			return TRUE;
		}
		else
		{
			version = "";
			return status;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetsysDriverbranchVersion(string& driver, string& Branch)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvAPI_ShortString ver = "";
		NvU32 i = -1;
		status = NvAPI_SYS_GetDriverAndBranchVersion(&i, ver);
		if (status == NVAPI_OK)
		{
			driver = to_string(i);
			Branch = ver;
			return TRUE;
		}
		else
		{
			driver = "";
			Branch = "";
			return status;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetGPUFullName(string& GPUName, INT Index)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvAPI_ShortString name = "";
		status = NvAPI_GPU_GetFullName(phys[Index], name);
		if (status != NVAPI_OK)
		{
			GPUName = "";
			return FALSE;
		}
		GPUName = name;
		if (GPUName.empty())
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetGPUMem(NV_DISPLAY_DRIVER_MEMORY_INFO& mem, INT Index)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_DISPLAY_DRIVER_MEMORY_INFO memtmp;
		ZeroMemory(&memtmp, sizeof(memtmp));
		memtmp.version = NV_DISPLAY_DRIVER_MEMORY_INFO_VER;
		status = NvAPI_GPU_GetMemoryInfo(phys[Index], &memtmp);
		if (NVAPI_INCOMPATIBLE_STRUCT_VERSION == status)
		{
			ZeroMemory(&memtmp, sizeof(memtmp));
			memtmp.version = NV_DISPLAY_DRIVER_MEMORY_INFO_VER_2;
			status = NvAPI_GPU_GetMemoryInfo(phys[Index], &memtmp);
			if (NVAPI_INCOMPATIBLE_STRUCT_VERSION == status)
			{
				ZeroMemory(&memtmp, sizeof(memtmp));
				memtmp.version = NV_DISPLAY_DRIVER_MEMORY_INFO_VER_1;
				status = NvAPI_GPU_GetMemoryInfo(phys[Index], &memtmp);
				if (NVAPI_OK == status)
				{
					memcpy(&mem, &memtmp, sizeof(NV_DISPLAY_DRIVER_MEMORY_INFO_V1));
					return TRUE;
				}
			} 
			else if (status == NVAPI_OK)
			{
				memcpy(&mem, &memtmp, sizeof(NV_DISPLAY_DRIVER_MEMORY_INFO_V2));
				return TRUE;
			}
		}
		else if (status == NVAPI_OK)
		{
			memcpy(&mem, &memtmp, sizeof(NV_DISPLAY_DRIVER_MEMORY_INFO));
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetGPUtemperature(INT Index, INT & MinTemp, INT & CurrentTemp, INT & MaxTemp)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_GPU_THERMAL_SETTINGS Thermal;
		ZeroMemory(&Thermal, sizeof(Thermal));
		Thermal.version = NV_GPU_THERMAL_SETTINGS_VER;
		Thermal.sensor->target = NVAPI_THERMAL_TARGET_GPU;
		Thermal.count = NVAPI_MAX_THERMAL_SENSORS_PER_GPU;
		status = NvAPI_GPU_GetThermalSettings(phys[Index], 0, &Thermal);
		if (NVAPI_INCOMPATIBLE_STRUCT_VERSION  == status)
		{
			ZeroMemory(&Thermal, sizeof(NV_GPU_THERMAL_SETTINGS_V1));
			Thermal.version = NV_GPU_THERMAL_SETTINGS_VER_1;
			Thermal.count = NVAPI_MAX_THERMAL_SENSORS_PER_GPU;
			status = NvAPI_GPU_GetThermalSettings(phys[Index], 0, &Thermal);
			if (status == NVAPI_OK)
			{
				CurrentTemp = Thermal.sensor[0].currentTemp;
				MinTemp = Thermal.sensor[0].defaultMinTemp;
				MaxTemp = Thermal.sensor[0].defaultMaxTemp;
				return TRUE;
			}
		}
		else if(NVAPI_OK == status)
		{
			CurrentTemp = Thermal.sensor[0].currentTemp;
			MinTemp = Thermal.sensor[0].defaultMinTemp;
			MaxTemp = Thermal.sensor[0].defaultMaxTemp;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetGPUFans(INT Index, ULONG & FansValue)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 Fans = -1;
		status = NvAPI_GPU_GetTachReading(phys[Index], &Fans);
		if (status == NVAPI_OK)
		{
			FansValue = Fans;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetGPUClock(INT Index)
{
	bool CurrentClock = false, BaseClock = false, BoostClock = false;
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_GPU_CLOCK_FREQUENCIES Fres;
		ZeroMemory(&Fres, sizeof(Fres));
		Fres.version = NV_GPU_CLOCK_FREQUENCIES_VER;
		Fres.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;
		status = NvAPI_GPU_GetAllClockFrequencies(phys[Index], &Fres);
		if (status == NVAPI_OK)
		{
			nvinfo.Device_clock.GraphicsCurrent = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency;
			nvinfo.Device_clock.MemoryCurrent = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency;
			CurrentClock = true;
		}
		ZeroMemory(&Fres, sizeof(Fres));
		Fres.version = NV_GPU_CLOCK_FREQUENCIES_VER;
		Fres.ClockType = NV_GPU_CLOCK_FREQUENCIES_BASE_CLOCK;
		status = NvAPI_GPU_GetAllClockFrequencies(phys[Index], &Fres);
		if (status == NVAPI_OK)
		{
			nvinfo.Device_clock.GraphicsBase = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency;
			nvinfo.Device_clock.MemoryBase = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency;
			BaseClock = true;
		}
		ZeroMemory(&Fres, sizeof(Fres));
		Fres.version = NV_GPU_CLOCK_FREQUENCIES_VER;
		Fres.ClockType = NV_GPU_CLOCK_FREQUENCIES_BOOST_CLOCK;
		status = NvAPI_GPU_GetAllClockFrequencies(phys[Index], &Fres);
		if (status == NVAPI_OK)
		{
			nvinfo.Device_clock.GraphicsBoost = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency;
			nvinfo.Device_clock.MemoryBoost = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency;
			BoostClock = true;
		}
		if (CurrentClock && BaseClock && BoostClock)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetHDCPSupport(INT Index, BOOL& HDCPState)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_GPU_GET_HDCP_SUPPORT_STATUS HDCP;
		ZeroMemory(&HDCP, sizeof(NV_GPU_GET_HDCP_SUPPORT_STATUS));
		HDCP.version = NV_GPU_GET_HDCP_SUPPORT_STATUS_VER;
		status = NvAPI_GPU_GetHDCPSupportStatus(phys[Index], &HDCP);
		if (status == NVAPI_OK)
		{
			HDCPState = HDCP.hdcpFuseState;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetGpuCoreCount(INT Index, UINT & corecount)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 count = -1;
		status = NvAPI_GPU_GetGpuCoreCount(phys[Index], &count);
		if (status == NVAPI_OK)
		{
			corecount = count;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetDynamicPstatesInfo(INT Index, NV_GPU_DYNAMIC_PSTATES_INFO_EX& percentage)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_GPU_DYNAMIC_PSTATES_INFO_EX Pstate;
		ZeroMemory(&Pstate, sizeof(Pstate));
		Pstate.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
		status = NvAPI_GPU_GetDynamicPstatesInfoEx(phys[Index], &Pstate);
		if (status == NVAPI_OK)
		{
			percentage = Pstate;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetVbiosVersion(INT Index, string & VbiosVersion)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvAPI_ShortString Version = "";
		status = NvAPI_GPU_GetVbiosVersionString(phys[Index], Version);
		if (status == NVAPI_OK)
		{
			VbiosVersion = Version;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetVbiosRevision(INT Index, string & VbiosRevision)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 Revision = -1;
		status = NvAPI_GPU_GetVbiosRevision(phys[Index], &Revision);
		if (status == NVAPI_OK)
		{
			VbiosRevision = to_string((ULONG)Revision);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetVbiosOEMRevision(INT Index, string & VbiosOEMRevision)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 Revision = -1;
		status = NvAPI_GPU_GetVbiosOEMRevision(phys[Index], &Revision);
		if (status == NVAPI_OK)
		{
			VbiosOEMRevision = to_string(Revision);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetGpuType(INT Index, string & GPUType)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_GPU_TYPE GType;
		status = NvAPI_GPU_GetGPUType(phys[Index], &GType);
		if (status == NVAPI_OK)
		{
			switch (GType)
			{
			case 1:
				GPUType = "Integrated GPU";
				break;
			case 2:
				GPUType = "Discrete GPU";
				break;
			default:
				GPUType = "Unknown GPU";
				break;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetSysType(INT Index, string & SysType)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_SYSTEM_TYPE SType;
		status = NvAPI_GPU_GetSystemType(phys[Index], &SType);
		if (status == NVAPI_OK)
		{
			switch (SType)
			{
			case 1:
				SysType = "LAPTOP GPU";
				break;
			case 2:
				SysType = "DESKTOP GPU";
				break;
			default:
				SysType = "Unknown GPU";
				break;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetAGPAperture(INT Index, ULONG & AGPAperture)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 Aperture = -1;
		status = NvAPI_GPU_GetAGPAperture(phys[Index], &Aperture);
		if (status == NVAPI_OK)
		{
			AGPAperture = Aperture;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetCurrentAGPRate(INT Index, ULONG & AGPRate)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 Rate = -1;
		status = NvAPI_GPU_GetCurrentAGPRate(phys[Index], &Rate);
		if (status == NVAPI_OK)
		{
			AGPRate = Rate;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetBUSID(INT Index, ULONG & BusID)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 ID = -1;
		status = NvAPI_GPU_GetBusId(phys[Index], &ID);
		if (status == NVAPI_OK)
		{
			BusID = ID;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetBUSSlotID(INT Index, ULONG & BusSlotID)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 SlotID = -1;
		status = NvAPI_GPU_GetBusSlotId(phys[Index], &SlotID);
		if (status == NVAPI_OK)
		{
			BusSlotID = SlotID;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetBusType(INT Index, string & BusType)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_GPU_BUS_TYPE BType = NVAPI_GPU_BUS_TYPE_UNDEFINED;
		status = NvAPI_GPU_GetBusType(phys[Index], &BType);
		if (status == NVAPI_OK)
		{
			switch (BType)
			{
			case 1:
				BusType = "PCI";
				break;
			case 2:
				BusType = "AGP";
				break;
			case 3:
				BusType = "PCI-Express";
				break;
			case 4:
				BusType = "FPCI";
				break;
			case 5:
				BusType = "AXI";
				break;
			default:
				BusType = "Undefined";
				break;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetPhysicalFrameBufferSize(INT Index, ULONG & PhysicalFrame)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 pSize = -1;
		status = NvAPI_GPU_GetPhysicalFrameBufferSize(phys[Index], &pSize);
		if (status == NVAPI_OK)
		{
			PhysicalFrame = pSize;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetVirtualFrameBufferSize(INT Index, ULONG & VirtualFrame)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 pSize = -1;
		status = NvAPI_GPU_GetVirtualFrameBufferSize(phys[Index], &pSize);
		if (status == NVAPI_OK)
		{
			VirtualFrame = pSize;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetGPUPState(INT Index )
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_GPU_PERF_PSTATES20_INFO PState;
		ZeroMemory(&PState, sizeof(PState));
		PState.version = NV_GPU_PERF_PSTATES20_INFO_VER;
		status = NvAPI_GPU_GetPstates20(phys[Index], &PState);
		if (NVAPI_INCOMPATIBLE_STRUCT_VERSION == status)
		{
			ZeroMemory(&PState, sizeof(PState));
			PState.version = NV_GPU_PERF_PSTATES20_INFO_VER1;
			status = NvAPI_GPU_GetPstates20(phys[Index], &PState);
		}
		else if (status == NVAPI_OK)
		{
			this->nvinfo.PState.NV_PState = PState;
			return TRUE;
		}
	}
	return FALSE;
	
}

BOOL CNvidia::GetCurrentPState(INT Index, UINT& CurrentPState)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_GPU_PERF_PSTATE_ID PState;
		status = NvAPI_GPU_GetCurrentPstate(phys[Index], &PState);
		if (status == NVAPI_OK)
		{
			CurrentPState = PState;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetChipInfo()
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_CHIPSET_INFO chipset_info;
		chipset_info.version = NV_CHIPSET_INFO_VER;
		status = NvAPI_SYS_GetChipSetInfo(&chipset_info);
		if (NVAPI_INCOMPATIBLE_STRUCT_VERSION == status)
		{
			::ZeroMemory(&chipset_info, sizeof(chipset_info));
			chipset_info.version = NV_CHIPSET_INFO_VER_3;
			status = NvAPI_SYS_GetChipSetInfo(&chipset_info);
			if (NVAPI_INCOMPATIBLE_STRUCT_VERSION == status)
			{
				::ZeroMemory(&chipset_info, sizeof(chipset_info));
				chipset_info.version = NV_CHIPSET_INFO_VER_2;
				status = NvAPI_SYS_GetChipSetInfo(&chipset_info);
				if (NVAPI_INCOMPATIBLE_STRUCT_VERSION == status)
				{
					::ZeroMemory(&chipset_info, sizeof(chipset_info));
					chipset_info.version = NV_CHIPSET_INFO_VER_1;
					status = NvAPI_SYS_GetChipSetInfo(&chipset_info);
					if (NVAPI_OK == status)
					{
						this->nvinfo.Chipset = chipset_info;
						return TRUE;
					}
				}
				else if (NVAPI_OK == status)
				{
					this->nvinfo.Chipset = chipset_info;
					return TRUE;
				}
			}
			else if(NVAPI_OK == status)
			{
				this->nvinfo.Chipset = chipset_info;
				return TRUE;
			}
		}
		else if(NVAPI_OK == status)
		{
			this->nvinfo.Chipset = chipset_info;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetDynamicInfo(INT Index)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NV_GPU_DYNAMIC_PSTATES_INFO_EX Dynamic_Pstates = {};
		NvAPI_Status status = NVAPI_OK;
		Dynamic_Pstates.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
		status = NvAPI_GPU_GetDynamicPstatesInfoEx(phys[Index], &Dynamic_Pstates);
		if (NVAPI_OK == status)
		{
			if (Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_GPU].bIsPresent)
				this->nvinfo.Nvidia_Usage.GPUUsage = Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_GPU].percentage;
			if (Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_FB].bIsPresent)
				this->nvinfo.Nvidia_Usage.FrameBufferUsage = Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_FB].percentage;
			if (Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_VID].bIsPresent)
				this->nvinfo.Nvidia_Usage.VideoEngineUsage = Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_VID].percentage;
			if (Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_BUS].bIsPresent)
				this->nvinfo.Nvidia_Usage.BUSInterFaceUsage = Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_BUS].percentage;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetPCIEWidth(INT Index)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvU32 PCIEWIdth = 0;
		status = NvAPI_GPU_GetCurrentPCIEDownstreamWidth(phys[Index], &PCIEWIdth);
		if (NVAPI_OK == status)
		{
			nvinfo.CurrentPCIEWidth = PCIEWIdth;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetECCConfigurationInfo(INT Index)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_GPU_ECC_CONFIGURATION_INFO Ecc_info = {};
		Ecc_info.version = NV_GPU_ECC_CONFIGURATION_INFO_VER;
		status = NvAPI_GPU_GetECCConfigurationInfo(phys[Index], &Ecc_info);
		if (NVAPI_OK == status)
		{
			this->nvinfo.ECC_Config = Ecc_info;
			return TRUE;
		}
	}
	return FALSE;
}
