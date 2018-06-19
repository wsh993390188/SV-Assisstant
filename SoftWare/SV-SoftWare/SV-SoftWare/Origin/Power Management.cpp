#include "stdafx.h"
#include "Power Management.h"

BOOL SV_ASSIST::Software::Origin::PowerManagement()
{
	GUID guid[12] = { 0 };
	PUCHAR buffer = nullptr;
	DWORD buffersize = 0;
	DWORD Error = 0;
	BOOL HaveBalance = FALSE, HaveHighPerformance = FALSE, HavePowerSave = FALSE;
	std::wstring NowMode = _T("");
	for (ULONG Index = 0; ;++Index)
	{
		static USHORT i = 0;
		while (Error = PowerEnumerate(NULL, NULL, NULL, ACCESS_SCHEME, Index, buffer, &buffersize))
		{
			if (ERROR_MORE_DATA == Error)
			{
				if (buffer)
				{
					delete[] buffer;
					buffer = nullptr;
				}
				buffer = new UCHAR[buffersize];
			}
			else
				break;
		}
		if (Error == ERROR_SUCCESS)
		{
			::CopyMemory(&guid[i++], buffer, buffersize);
		}
		else if (Error == ERROR_NO_MORE_ITEMS)
			break;
	}

	if (buffer)
	{
		delete[] buffer;
		buffer = nullptr;
	}

	for (USHORT i = 0; i < 12; ++i)
	{
		if (guid[i] == HIGHPERFORMANCEMODE)
		{
			HaveHighPerformance = TRUE;
		}
		if (guid[i] == BALANCEMODE)
		{
			HaveBalance = TRUE;
		}
		if (guid[i] == POWERSAVEMODE)
		{
			HavePowerSave = TRUE;
		}
	}
	GUID* active = nullptr;
	PowerGetActiveScheme(NULL, &active);
	if (*active == HIGHPERFORMANCEMODE)
	{
		NowMode = _T("High Performance");
	}
	else if (*active == BALANCEMODE)
	{
		NowMode = _T("Balance Mode");
	}
	else if (*active == POWERSAVEMODE)
	{
		NowMode = _T("Power Save");
	}
	else
	{
		NowMode = _T("Unknown");
	}
	PowerWriteACValueIndex(NULL, active, &GUID_VIDEO_SUBGROUP, &GUID_VIDEO_POWERDOWN_TIMEOUT, 0);
	PowerWriteDCValueIndex(NULL, active, &GUID_VIDEO_SUBGROUP, &GUID_VIDEO_POWERDOWN_TIMEOUT, 0);

	if (*active != HIGHPERFORMANCEMODE && HaveHighPerformance)
	{
		Error = PowerSetActiveScheme(NULL, (GUID*)&HIGHPERFORMANCEMODE);
	}
	LocalFree(active);
	return FALSE;
}

BOOL SV_ASSIST::Software::Origin::ChangeBlueCrash(BOOL AutoReboot)
{
	const TCHAR* BlueCrash = _T("System\\CurrentControlSet\\Control\\CrashControl");
	HKEY hKey;
	int ret = -1;
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, BlueCrash, 0, KEY_WRITE|KEY_ALL_ACCESS, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		return FALSE;
	}

	LPCTSTR lpszKeyValueName = TEXT("AutoReboot");
	DWORD dwKeyValueType;
	DWORD dwKeyValueDataSize;
	ret = RegQueryValueEx(hKey, lpszKeyValueName, NULL, &dwKeyValueType, NULL, &dwKeyValueDataSize);
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugString(_T("可能没有这个键\n"));
		return FALSE;
	}

	// 获取键值项Data  
	std::unique_ptr<BYTE[]> lpbKeyValueData(new BYTE[dwKeyValueDataSize]);
	ret = RegQueryValueEx(hKey, lpszKeyValueName, NULL, &dwKeyValueType, lpbKeyValueData.get(), &dwKeyValueDataSize);
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugString(_T("可能没有这个键\n"));
		return FALSE;
	}
	if (AutoReboot == TRUE || AutoReboot == FALSE)
	{
		RegSetValueEx(hKey,
			lpszKeyValueName,
			NULL,
			REG_DWORD,
			(BYTE*)&AutoReboot,
			dwKeyValueDataSize
		);

	}
	RegCloseKey(hKey);

	return TRUE;
}

BOOL SV_ASSIST::Software::Origin::FireWall(BOOL EnableFireWall)
{
	const TCHAR* FireWallPath = _T("System\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile");
	HKEY hKey;
	int ret = -1;
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, FireWallPath, 0, KEY_WRITE | KEY_ALL_ACCESS, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		return FALSE;
	}

	LPCTSTR lpszKeyValueName = TEXT("EnableFirewall");
	DWORD dwKeyValueType;
	DWORD dwKeyValueDataSize;
	ret = RegQueryValueEx(hKey, lpszKeyValueName, NULL, &dwKeyValueType, NULL, &dwKeyValueDataSize);
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugString(_T("可能没有这个键\n"));
		return FALSE;
	}

	// 获取键值项Data  
	std::unique_ptr<BYTE[]> lpbKeyValueData(new BYTE[dwKeyValueDataSize]);
	ret = RegQueryValueEx(hKey, lpszKeyValueName, NULL, &dwKeyValueType, lpbKeyValueData.get(), &dwKeyValueDataSize);
	if (ret != ERROR_SUCCESS)
	{
		OutputDebugString(_T("可能没有这个键\n"));
		return FALSE;
	}
	if (EnableFireWall == TRUE || EnableFireWall == FALSE)
	{
		RegSetValueEx(hKey,
			lpszKeyValueName,
			NULL,
			REG_DWORD,
			(BYTE*)&EnableFireWall,
			dwKeyValueDataSize
		);
	}
	RegCloseKey(hKey);
	return TRUE;
}
