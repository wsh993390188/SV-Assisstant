// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <objbase.h>

/// @brief DLL的模块名
HMODULE g_hModule = NULL;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Hardware::Utils::GetWindowsVersion();
		g_hModule = hModule;
		CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
		break;
	case DLL_PROCESS_DETACH:
		g_hModule = NULL;
		CoUninitialize();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}