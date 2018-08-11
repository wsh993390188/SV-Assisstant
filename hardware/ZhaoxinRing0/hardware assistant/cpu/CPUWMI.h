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
	string ProcessorId;
	UINT ExtClock;
	UINT Core;
	UINT Thread;
	UINT Revision;
	UINT MaxClockSpeed;
};

