#include "stdafx.h"
#include "NVAPI/nvapi.h"
#include "..\driver\DriverOrigin.h"
#include "Nvidia.h"
#include "..\sv-assistant\sv-assistant\sqlite\sqlite3.h"

/************************************************************************/
/*                         Load Area                                    */
/************************************************************************/

int CNvidia::callback(void *NotUsed, int argc, char **argv, char **azColName) {
	NvidiaInfo &temp = *static_cast<NvidiaInfo*>(NotUsed);
	temp.Architecture = argv[1];
	temp.CoreName = argv[2];
	temp.Technology = std::stol(argv[3]);
	temp.Transistors = std::stol(argv[4]);
	temp.DieSize = std::stol(argv[5]);
	temp.MemoryType = argv[7];
	temp.MemoryBus = std::stol(argv[8]);
	temp.MemoryBusWidth = strtod(argv[9], nullptr);
	temp.TDP = std::stol(argv[10]);
	temp.Shaders = std::stol(argv[11]);
	temp.TMUs = std::stol(argv[12]);
	temp.ROPs = std::stol(argv[13]);
	temp.PixelRate = strtod(argv[14], nullptr);
	temp.TextureRate = strtod(argv[15], nullptr);
	return 0;
}

CNvidia::CNvidia() : hDisplay{}, phys{}
{
#ifdef ZX_OutputLog
	Logger::Instance()->OutputLogInfo(el::Level::Debug, "Initiallize Nvidia API Library");
#endif
	m_succeed_Nvidia = NvAPI_Initialize();

	if (NVAPI_OK == m_succeed_Nvidia)
	{
		BOOL status = NVAPI_ERROR;
		status = this->EnumPhysicalGPUs();
		if (status != NVAPI_OK)
		{
#ifdef _DEBUG
			NvAPI_ShortString str;
			NvAPI_GetErrorMessage((NvAPI_Status)status, str);
			OutputDebugStringA(str);
#endif // _DEBUG
		}
		status = this->EnumNvidiaDisplayHandle();
		if (status != NVAPI_OK)
		{
#ifdef _DEBUG
			NvAPI_ShortString str;
			NvAPI_GetErrorMessage((NvAPI_Status)status, str);
			OutputDebugStringA(str);
#endif // _DEBUG
		}
		this->GetsysDriverbranchVersion(DriverVer, BranchVersion);
		this->GetChipInfo();
		std::string InterfaceVersion{};
		this->GetInterfaceVersion(InterfaceVersion);
#ifdef ZX_OutputLog
		Logger::Instance()->OutputLogInfo(el::Level::Debug, "Initiallize Nvidia API Library Success");
#endif
	}
#ifdef ZX_OutputLog
	else
	{
		NvAPI_ShortString str;
		NvAPI_GetErrorMessage((NvAPI_Status)m_succeed_Nvidia, str);
		Logger::Instance()->OutputLogInfo(el::Level::Debug, str);
	}
#endif
}

CNvidia::~CNvidia()
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Unload();
	}
}

#pragma region Data Base Info
bool CNvidia::GetBaseinfoFromDB(NvidiaInfo & nvinfo)
{
	sqlite3* db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open("nvidia.db", &db);
	if (rc) {
		OutputDebugPrintf(L"Can't open database: %s\n", sqlite3_errmsg(db));
		return 0;
	}
	else {
		OutputDebugPrintf(L"Opened database successfully\n");
	}
	if (nvinfo.FullName.find("GeForce GTX") != std::string::npos)
	{
		size_t last = 0;
		size_t count = 0;
		size_t index = nvinfo.FullName.find_first_of(' ', last);
		while (index != std::string::npos)
		{
			if ((count++) == 3)
				break;
			auto temp = nvinfo.FullName.substr(last, index - last);
			last = index + 1;
			index = nvinfo.FullName.find_first_of(' ', last);
		}
		if (nvinfo.FullName.size() - last > 0)
		{
			std::string sql = boost::str(boost::format("SELECT * from GEFORCEGTX WHERE ID LIKE '%S'") % nvinfo.FullName.substr(last).c_str());
			rc = sqlite3_exec(db, sql.c_str(), callback, static_cast<void*>(&nvinfo), &zErrMsg);
			if (rc) {
				OutputDebugPrintf(L"Can't query Info: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
				return 0;
			}
			else {
				OutputDebugPrintf(L"Query Info successfully\n");
			}
		}

	}

	return true;
}
#pragma endregion

#pragma region Public Functions
bool CNvidia::CheckdllStatus()
{
	return (m_succeed_Nvidia == NVAPI_OK) ? TRUE : FALSE;
}
const void* CNvidia::Returninfo()
{
	return &this->realnv;
}

GPUTypes CNvidia::UpdateData()
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		for (UINT i = 0; i < this->physcount; i++)
		{
			this->UpdatePhysicalGPUs(i, realnv[i]);
		}
	}
	return GPUTypes::NVIDIA_GPU;
}
#pragma endregion

NvAPI_Status CNvidia::PhysicalGPUs(INT Index, NvidiaInfo & nvinfo)
{
	this->GetGPUFullName(nvinfo.FullName, Index);
	this->GetGpuType(Index, nvinfo.GPUType);
	this->GetVbiosVersion(Index, nvinfo.VbiosVersion);
	this->GetSysType(Index, nvinfo.SysType);
	this->GetPhysicalFrameBufferSize(Index, nvinfo.PhysicalFrame);
	this->GetVirtualFrameBufferSize(Index, nvinfo.VirtualFrame);
	this->GetHDCPSupport(Index, nvinfo.HDCP);
	this->GetGpuCoreCount(Index, nvinfo.corecount);
	this->GetGPUPState(Index, nvinfo.NV_PState);
	this->GetPCIIdentify(Index, nvinfo.PCIIdentify.pDeviceId, nvinfo.PCIIdentify.pSubSystemId, nvinfo.PCIIdentify.pRevisionId, nvinfo.PCIIdentify.pExtDeviceId);
	{
		USHORT VendorID = (USHORT)nvinfo.PCIIdentify.pDeviceId & 0xFFFF;
		USHORT DeviceID = (USHORT)(nvinfo.PCIIdentify.pDeviceId >> 16);
		for (const auto& i : ZhaoxinDriver::Instance()->GetAllPciInfo().pciconfig)
		{
			if ((i.second.VendorID == VendorID) && (i.second.DeviceID == DeviceID))
			{
			swscanf_s(i.first.c_str(),
				_T("bus:%hd dev:%hd func:%hd"),
				&nvinfo.PCIIdentify.bus, &nvinfo.PCIIdentify.dev, &nvinfo.PCIIdentify.func);
			}
		}
	}
	GetBaseinfoFromDB(nvinfo);
	this->UpdatePhysicalGPUs(Index, nvinfo);
	return NVAPI_OK;
}

NvAPI_Status CNvidia::UpdatePhysicalGPUs(INT Index, NvidiaInfo & nvinfo)
{
	NV_DISPLAY_DRIVER_MEMORY_INFO mem = {};
	this->GetGPUMem(mem, Index);
	nvinfo.dedicatedVideoMemory = mem.dedicatedVideoMemory;
	nvinfo.availableDedicatedVideoMemory = mem.availableDedicatedVideoMemory;
	nvinfo.dedicatedVideoMemoryEvictionCount = mem.dedicatedVideoMemoryEvictionCount;
	nvinfo.dedicatedVideoMemoryEvictionsSize = mem.dedicatedVideoMemoryEvictionsSize;
	nvinfo.curAvailableDedicatedVideoMemory = mem.curAvailableDedicatedVideoMemory;
	nvinfo.sharedSystemMemory = mem.sharedSystemMemory;
	nvinfo.systemVideoMemory = mem.systemVideoMemory;

	this->GetGPUtemperature(Index, nvinfo.Device_Tem.GPUMinTem, nvinfo.Device_Tem.GPUCurrTem, nvinfo.Device_Tem.GPUMAXtem);
	this->GetDynamicPstatesInfo(Index, nvinfo.percentage);
	this->GetDynamicInfo(Index, nvinfo.Nvidia_Usage);
	this->GetCurrentPState(Index, nvinfo.CurrentPState);
	this->GetGPUFans(Index, nvinfo.fans);
	this->GetGPUClock(Index, nvinfo.Device_clock);
	this->GetPCIESpeed(nvinfo);
	return NVAPI_OK;
}

NvAPI_Status CNvidia::DisplayGPUS()
{
	return NVAPI_OK;
}

GPUTypes CNvidia::exec()
{
	GPUTypes re = OTHERS_GPU;
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		if(physcount)
			re = NVIDIA_GPU;
		for (UINT i = 0; i < this->physcount; i++)
		{
			NvidiaInfo nvinfo = {};
			this->PhysicalGPUs(i, nvinfo);
			this->realnv.emplace_back(nvinfo);
		}

	}
	return re;
}

#pragma region PCI Function
BOOL CNvidia::GetPCIESpeed(NvidiaInfo & nvinfo)
{
	PCI_COMMON_CONFIG pci = {};
	if (ZhaoxinDriver::Instance()->ReadPci(nvinfo.PCIIdentify.bus, nvinfo.PCIIdentify.dev, nvinfo.PCIIdentify.func, pci) == 0)
	{
		UCHAR* capaaddr = reinterpret_cast<UCHAR*>(&pci);
		USHORT CapabilitiesPtr = pci.u.type0.CapabilitiesPtr; 
		bool IsPCIE10H = false;
		while (true)
		{
			if (capaaddr[CapabilitiesPtr] == 0x10)
			{
				IsPCIE10H = true;
				break;
			}
			if(capaaddr[CapabilitiesPtr + 1] == 0x00)
				break;
			CapabilitiesPtr = capaaddr[CapabilitiesPtr + 1];
		}
		if(IsPCIE10H)
		{
			nvinfo.MaxPCIESpeed.LinkSpeed = capaaddr[CapabilitiesPtr + 0x0C] & 0xFF;
			USHORT temp = capaaddr[CapabilitiesPtr + 0x0C] | (capaaddr[CapabilitiesPtr + 0x0D] << 8);
			nvinfo.MaxPCIESpeed.LinkWidth =(temp & 0x3F0) >> 4;

			nvinfo.CurrentPCIESpeed.LinkSpeed = capaaddr[CapabilitiesPtr + 0x12] & 0xFF;
			temp = capaaddr[CapabilitiesPtr + 0x12] | (capaaddr[CapabilitiesPtr + 0x13] << 8);
			nvinfo.CurrentPCIESpeed.LinkWidth = (temp & 0x3F0) >> 4;
			return TRUE;
		}
	}
	return FALSE;
}
#pragma endregion

#pragma region NVAPI Function
NvAPI_Status CNvidia::EnumPhysicalGPUs()
{
	NvAPI_Status status = NVAPI_ERROR;
	if (NVAPI_OK == m_succeed_Nvidia)
		status = NvAPI_EnumPhysicalGPUs(phys, &physcount);
	return status;
}

NvAPI_Status CNvidia::EnumNvidiaDisplayHandle()
{
	NvAPI_Status nvapiStatus = NVAPI_OK;
	NvU32 nvDisplayCount = 0;
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		for (unsigned int i = 0; nvapiStatus == NVAPI_OK; i++)
		{
			nvapiStatus = NvAPI_EnumNvidiaDisplayHandle(i, &hDisplay[i]);

			if (nvapiStatus == NVAPI_OK)
			{
				nvDisplayCount++;
			}
			else if (nvapiStatus != NVAPI_END_ENUMERATION)
			{
				return nvapiStatus;
			}
		}
	}
	return NVAPI_OK;
}

/************************************************************************/
/*                         Function Area                                */
/************************************************************************/

BOOL CNvidia::GetInterfaceVersion(std::string& version)
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

BOOL CNvidia::GetsysDriverbranchVersion(std::string& driver, std::string& Branch)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NvAPI_ShortString ver = "";
		NvU32 i = -1;
		status = NvAPI_SYS_GetDriverAndBranchVersion(&i, ver);
		if (status == NVAPI_OK)
		{
			driver = std::to_string(i);
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

BOOL CNvidia::GetGPUFullName(std::string& GPUName, INT Index)
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
		GPUName = std::string("Nvidia ") + std::string(name);
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

BOOL CNvidia::GetPCIIdentify(INT Index, NvU32& pDeviceId, NvU32&pSubSystemId, NvU32& pRevisionId, NvU32& pExtDeviceId)
{
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		status = NvAPI_GPU_GetPCIIdentifiers(phys[Index], &pDeviceId, &pSubSystemId, &pRevisionId, &pExtDeviceId);
		if (NVAPI_OK == status)
		{
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
		NV_GPU_THERMAL_SETTINGS Thermal = {};
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

BOOL CNvidia::GetGPUClock(INT Index, DEVICE_CLOCK& Device_clock)
{
	bool CurrentClock = false, BaseClock = false, BoostClock = false;
	if (NVAPI_OK == m_succeed_Nvidia)
	{
		NvAPI_Status status = NVAPI_OK;
		NV_GPU_CLOCK_FREQUENCIES Fres = {};
		Fres.version = NV_GPU_CLOCK_FREQUENCIES_VER;
		Fres.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;
		status = NvAPI_GPU_GetAllClockFrequencies(phys[Index], &Fres);
		if (status == NVAPI_OK)
		{
			Device_clock.GraphicsCurrent = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency;
			Device_clock.MemoryCurrent = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency;
			CurrentClock = true;
		}
		ZeroMemory(&Fres, sizeof(Fres));
		Fres.version = NV_GPU_CLOCK_FREQUENCIES_VER;
		Fres.ClockType = NV_GPU_CLOCK_FREQUENCIES_BASE_CLOCK;
		status = NvAPI_GPU_GetAllClockFrequencies(phys[Index], &Fres);
		if (status == NVAPI_OK)
		{
			Device_clock.GraphicsBase = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency;
			Device_clock.MemoryBase = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency;
			BaseClock = true;
		}
		ZeroMemory(&Fres, sizeof(Fres));
		Fres.version = NV_GPU_CLOCK_FREQUENCIES_VER;
		Fres.ClockType = NV_GPU_CLOCK_FREQUENCIES_BOOST_CLOCK;
		status = NvAPI_GPU_GetAllClockFrequencies(phys[Index], &Fres);
		if (status == NVAPI_OK)
		{
			Device_clock.GraphicsBoost = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency;
			Device_clock.MemoryBoost = Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency;
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

BOOL CNvidia::GetVbiosVersion(INT Index, std::string & VbiosVersion)
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

BOOL CNvidia::GetGpuType(INT Index, std::string & GPUType)
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

BOOL CNvidia::GetSysType(INT Index, std::string & SysType)
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

BOOL CNvidia::GetGPUPState(INT Index, NV_GPU_PERF_PSTATES20_INFO& NV_PState)
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
			NV_PState = PState;
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
						this->Chipset = chipset_info;
						return TRUE;
					}
				}
				else if (NVAPI_OK == status)
				{
					this->Chipset = chipset_info;
					return TRUE;
				}
			}
			else if(NVAPI_OK == status)
			{
				this->Chipset = chipset_info;
				return TRUE;
			}
		}
		else if(NVAPI_OK == status)
		{
			this->Chipset = chipset_info;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNvidia::GetDynamicInfo(INT Index, NVIDIA_USAGE& Nvidia_Usage)
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
				Nvidia_Usage.GPUUsage = Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_GPU].percentage;
			if (Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_FB].bIsPresent)
				Nvidia_Usage.FrameBufferUsage = Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_FB].percentage;
			if (Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_VID].bIsPresent)
				Nvidia_Usage.VideoEngineUsage = Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_VID].percentage;
			if (Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_BUS].bIsPresent)
				Nvidia_Usage.BUSInterFaceUsage = Dynamic_Pstates.utilization[NVAPI_GPU_UTILIZATION_DOMAIN_BUS].percentage;
			return TRUE;
		}
	}
	return FALSE;
}
#pragma endregion