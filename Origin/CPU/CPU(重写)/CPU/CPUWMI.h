#pragma once

#include <windows.h>
#include <comdef.h>
#include <wbemidl.h>

#include <atlbase.h>
#include <atlcom.h>
#include <string>
#include <memory>
using namespace std;

#define CHECKHR(x) \
    if ( FAILED((x))) break; \

#define CHECKWMIHR(x) \
    if ( (x) != WBEM_S_NO_ERROR ) break; \

class CWMI
{
public:
	CWMI();
	~CWMI(void);
private:
	HRESULT InitialCom();
	HRESULT SetComSecLevels();
	HRESULT ObtainLocator2WMI(CComPtr<IWbemLocator>& pLoc);
	HRESULT Connect2WMI(CComPtr<IWbemLocator> pLoc, CComPtr<IWbemServices>& pSvc);
	HRESULT SetProxySecLevels(CComPtr<IWbemServices> pSvc);
	HRESULT Excute(CComPtr<IWbemServices> pSvc);
public:
	HRESULT ExcuteFun();
private:
	string m_wstrNamespace;
public:
	string Name;
	string ProcessorId;
	string SocketDesignation;
	UINT CurrentClockSpeed;
	UINT ExtClock;
	UINT Core;
	UINT Thread;
	UINT Revision;
	UINT MaxClockSpeed;
	UINT UpgradeMethod;
};

// class InstructionSet
// {
// public:
// 	static const string GetCPUName();
// 	static const string GetProcessorID();
// 	static const string GetSocketDesignation();
// 	static const UINT GetCurrentClockSpeed();
// 	static const UINT GetExtClock();
// 	static const UINT GetCore();
// 	static const UINT GetThread();
// 	static const UINT GetRevision();
// 	static const UINT GetMaxClockSpeed();
// 	static const UINT GetUpgradeMethod();
// private:
// 	static const CWMI wmi;
// };
