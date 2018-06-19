#pragma once

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DRIVERDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// ZHAOXINSOFTWAREDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef ZHAOXINSOFTWAREDLL_EXPORTS
#define ZHAOXINSOFTWAREDLL_API __declspec(dllexport)
#else
#define ZHAOXINSOFTWAREDLL_API __declspec(dllimport)
#endif

#include <windows.h>
#include <list>

namespace SV_ASSIST
{
	namespace Software
	{
		namespace Ring0
		{
			ZHAOXINSOFTWAREDLL_API const std::list<M_EPROCESS>& GetCurrentProcess();
			ZHAOXINSOFTWAREDLL_API const std::list<DRIVER_MODULE>& GetCurrentModule();
			ZHAOXINSOFTWAREDLL_API const std::list<PLUGPLAY_STRUCT>& GetCurrentPowerSetting();
			ZHAOXINSOFTWAREDLL_API const std::list<PLUGPLAY_STRUCT>& GetCurrentFsNotifyChange();
			ZHAOXINSOFTWAREDLL_API const std::list<SHUTDOWN_STRUCT>& GetCurrentLastshutdown();
			ZHAOXINSOFTWAREDLL_API const std::list<SHUTDOWN_STRUCT>& GetCurrentShutdown();
			ZHAOXINSOFTWAREDLL_API const std::list<BUG_STRUCT>& GetCurrentBugReason();
			ZHAOXINSOFTWAREDLL_API const std::list<BUG_STRUCT>& GetCurrentBug();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentCmpCallback();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentLodImage();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentProcessCallback();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentThreadCallback();
			ZHAOXINSOFTWAREDLL_API const std::list<FILTERSYS_INFO>& GetCurrentFilter();
			ZHAOXINSOFTWAREDLL_API const std::list<DPCTIMER_STRUCT>& GetCurrentDpcTimer();
			ZHAOXINSOFTWAREDLL_API const std::list<GPTABLE_STRUCT>& GetCurrentGdtTable();
			ZHAOXINSOFTWAREDLL_API const std::list<IDTABLE_STRUCT>& GetCurrentIdtTable();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentSCSI();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentACPICallback();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentAtapi();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentDisk();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentPartmgr();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentI8042prt();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentKeybroad();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentMouse();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentFSD();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentTcpip();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentTdx();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentNsiproxy();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentSSDT();
			ZHAOXINSOFTWAREDLL_API const std::list<DWORD64>& GetCurrentShadowSSDT();
			ZHAOXINSOFTWAREDLL_API void UpdateData();
		}
	}
}