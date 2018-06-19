#include "stdafx.h"
#include "ZhaoxinSoftwaredll.h"
#include "CSoftwareRing0.h"

using namespace SV_ASSIST::Software::Ring0;
static std::unique_ptr<CSoftwareRing0> origin(new CSoftwareRing0);

ZHAOXINSOFTWAREDLL_API const std::list<M_EPROCESS>& SV_ASSIST::Software::Ring0::GetCurrentProcess()
{
	return origin->m_Processor;
}

ZHAOXINSOFTWAREDLL_API const std::list<DRIVER_MODULE>& SV_ASSIST::Software::Ring0::GetCurrentModule()
{
	return origin->Driver_Module;
}

ZHAOXINSOFTWAREDLL_API const std::list<PLUGPLAY_STRUCT>& SV_ASSIST::Software::Ring0::GetCurrentPowerSetting()
{
	return origin->m_PowerSetting;
}

ZHAOXINSOFTWAREDLL_API const std::list<PLUGPLAY_STRUCT>& SV_ASSIST::Software::Ring0::GetCurrentFsNotifyChange()
{
	return origin->m_FsNotifyChange;
}

ZHAOXINSOFTWAREDLL_API const std::list<SHUTDOWN_STRUCT>& SV_ASSIST::Software::Ring0::GetCurrentLastshutdown()
{
	return origin->m_LastShutdown;
}

ZHAOXINSOFTWAREDLL_API const std::list<SHUTDOWN_STRUCT>& SV_ASSIST::Software::Ring0::GetCurrentShutdown()
{
	return origin->m_Shutdown;
}

ZHAOXINSOFTWAREDLL_API const std::list<BUG_STRUCT>& SV_ASSIST::Software::Ring0::GetCurrentBugReason()
{
	return origin->m_BugReason;
}

ZHAOXINSOFTWAREDLL_API const std::list<BUG_STRUCT>& SV_ASSIST::Software::Ring0::GetCurrentBug()
{
	return origin->m_Bug;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentCmpCallback()
{
	return origin->m_Cmp;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentLodImage()
{
	return origin->m_LoadImage;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentProcessCallback()
{
	return origin->m_ProcessorCallback;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentThreadCallback()
{
	return origin->m_ThreadCallback;
}

ZHAOXINSOFTWAREDLL_API const std::list<FILTERSYS_INFO>& SV_ASSIST::Software::Ring0::GetCurrentFilter()
{
	return origin->m_Filter;
}

ZHAOXINSOFTWAREDLL_API const std::list<DPCTIMER_STRUCT>& SV_ASSIST::Software::Ring0::GetCurrentDpcTimer()
{
	return origin->m_DpcTimer;
}

ZHAOXINSOFTWAREDLL_API const std::list<GPTABLE_STRUCT>& SV_ASSIST::Software::Ring0::GetCurrentGdtTable()
{
	return origin->m_GdtTable;
}

ZHAOXINSOFTWAREDLL_API const std::list<IDTABLE_STRUCT>& SV_ASSIST::Software::Ring0::GetCurrentIdtTable()
{
	return origin->m_IdtTable;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentSCSI()
{
	return origin->m_SCSI;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentACPICallback()
{
	return origin->m_ACPI;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentAtapi()
{
	return origin->m_ATAPI;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentDisk()
{
	return origin->m_DISK;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentPartmgr()
{
	return origin->m_PARTMGR;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentI8042prt()
{
	return origin->m_I8042PRT;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentKeybroad()
{
	return origin->m_KBD;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentMouse()
{
	return origin->m_MOUSE;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentFSD()
{
	return origin->m_FSD;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentTcpip()
{
	return origin->m_TCPIP;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentTdx()
{
	return origin->m_TDX;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentNsiproxy()
{
	return origin->m_NSIPROXY;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentSSDT()
{
	return origin->m_SSDT;
}

ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& SV_ASSIST::Software::Ring0::GetCurrentShadowSSDT()
{
	 return origin->m_SHADOWSSDT;
}

ZHAOXINSOFTWAREDLL_API void SV_ASSIST::Software::Ring0::UpdateData()
{
	origin->UpdateData();
}
