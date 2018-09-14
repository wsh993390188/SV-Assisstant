#include "stdafx.h"
#include <shellapi.h>
#include "DnpService.h"

CDnpService::CServiceThread::CServiceThread() : _bCancel{ false }
{

}

bool CDnpService::CServiceThread::IsCancel(bool bSave/* =false */, bool bNewValue/* =false */)
{
	bool	ret;

	_secbCancel.Lock();
	if (bSave)
	{
		_bCancel = bNewValue;
		ret = true;
	}
	else
		ret = _bCancel;
	_secbCancel.Unlock();

	return	ret;
}

bool CDnpService::CServiceThread::EasyStartStop(LPCTSTR pszName, bool b)
{
	bool			ret = false;
	BOOL			bRet = FALSE;
	SC_HANDLE		hManager = NULL;
	SC_HANDLE		hService = NULL;
	SERVICE_STATUS	sStatus;

	hManager = ::OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
	if (hManager == NULL)
	{
		return false;
	}

	hService = ::OpenService(hManager, pszName, SERVICE_START | SERVICE_QUERY_STATUS);
	if (hService == NULL)
	{
		if (hManager) { ::CloseServiceHandle(hManager); }
		return false;
	}

	::ZeroMemory(&sStatus, sizeof(SERVICE_STATUS));
	bRet = ::QueryServiceStatus(hService, &sStatus);
	if (bRet == FALSE)
	{
		if (hService) { ::CloseServiceHandle(hService); }
		if (hManager) { ::CloseServiceHandle(hManager); }
		return false;
	}

	if (sStatus.dwCurrentState == SERVICE_RUNNING)
	{
		if (hService) { ::CloseServiceHandle(hService); }
		if (hManager) { ::CloseServiceHandle(hManager); }
		return true;
	}

	OutputDebugPrintf(_T("sStatus.dwCurrentState:%08X"), sStatus.dwCurrentState);

	//サービス開始要求
	OutputDebugPrintf(_T("StartService - 1"));
	bRet = ::StartService(hService, NULL, NULL);

	//開始まで無限ループで待機
	OutputDebugPrintf(_T("QueryServiceStatus - 1"));
	int count = 0;
	while (::QueryServiceStatus(hService, &sStatus))
	{
		// 無限ループを回避 (最大 1 秒間 WMI の初期化を待つ)
		if (count >= 4)
		{
			break;
		}

		if (sStatus.dwCurrentState == SERVICE_RUNNING)
		{
			OutputDebugPrintf(_T("StartService Completed : SERVICE_RUNNING"));
			if (hService) { ::CloseServiceHandle(hService); }
			if (hManager) { ::CloseServiceHandle(hManager); }
			return true;
		}

		::Sleep(100 * count);
		OutputDebugPrintf(_T("Sleep"));
		count++;
	}

	// サービスの起動モードを auto に強制変更
	ShellExecute(NULL, NULL, _T("sc"), _T("config Winmgmt start= auto"), NULL, SW_HIDE);
	count = 0;
	OutputDebugPrintf(_T("QueryServiceStatus - 2"));
	while (::QueryServiceStatus(hService, &sStatus))
	{
		//サービス開始要求
		OutputDebugPrintf(_T("StartService - 2"));
		::StartService(hService, NULL, NULL);

		// 無限ループを回避 (最大 5 秒間 WMI の初期化を待つ)
		if (count >= 10)
		{
			break;
		}

		if (sStatus.dwCurrentState == SERVICE_RUNNING)
		{
			OutputDebugPrintf(_T("StartService Completed : SERVICE_RUNNING"));
			if (hService) { ::CloseServiceHandle(hService); }
			if (hManager) { ::CloseServiceHandle(hManager); }
			return true;
		}

		::Sleep(500);
		OutputDebugPrintf(_T("Sleep"));
		count++;
	}

	if (hService) { ::CloseServiceHandle(hService); }
	if (hManager) { ::CloseServiceHandle(hManager); }
	return false;
}

bool CDnpService::EasyStartStop(LPCTSTR pszName, bool bStart)
{
	CServiceThread	cThread;

	return	cThread.EasyStartStop(pszName, bStart);
}

bool CDnpService::EasyStart(LPCTSTR pszName)
{
	return	EasyStartStop(pszName, true);
}

bool CDnpService::EasyRestart(LPCTSTR pszName)
{
	bool			ret;
	CServiceThread	cThread;

	ret = cThread.EasyStartStop(pszName, false);
	if (ret)
		ret = cThread.EasyStartStop(pszName, true);

	return	ret;
}

bool CDnpService::EasyStop(LPCTSTR pszName)
{
	return	EasyStartStop(pszName, false);
}

bool CDnpService::IsServiceRunning(LPCTSTR pszName)
{
	bool			ret;
	BOOL			bRet;
	SC_HANDLE		hManager;
	SC_HANDLE		hService;
	SERVICE_STATUS	sStatus;

	ret = false;
	hManager = NULL;
	hService = NULL;
	while (1)			//無限ループではない！
	{
		hManager = ::OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
		ATLASSERT(hManager);
		if (hManager == NULL)
			break;

		hService = ::OpenService(hManager, pszName, SERVICE_QUERY_STATUS);
		ATLASSERT(hService);
		if (hService == NULL)
			break;

		::ZeroMemory(&sStatus, sizeof(SERVICE_STATUS));
		bRet = ::QueryServiceStatus(hService, &sStatus);
		ATLASSERT(bRet);
		if (bRet == FALSE)
			break;

		if (sStatus.dwCurrentState == SERVICE_RUNNING)
			ret = true;

		break;		//必須
	}

	if (hService)
		::CloseServiceHandle(hService);
	if (hManager)
		::CloseServiceHandle(hManager);

	return	ret;
}