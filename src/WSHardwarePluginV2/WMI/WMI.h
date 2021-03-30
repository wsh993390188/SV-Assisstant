#pragma once
#include <windows.h>
#include <comdef.h>
#include <wbemidl.h>

#include <atlbase.h>
#include <atlcom.h>
#include <string>

#define CHECKHR(x) \
    if ( FAILED((x))) break; \

#define CHECKWMIHR(x) \
    if ( (x) != WBEM_S_NO_ERROR ) break; \

class CWMI
{
public:
    CWMI();
	virtual ~CWMI(void);
public:
    HRESULT ExcuteFun();
protected:
    VOID SetNamespace(std::wstring wstrNamespace);
protected:
	bool initWmi;
private:
    HRESULT InitialCom();
    HRESULT SetComSecLevels();
    HRESULT ObtainLocator2WMI(CComPtr<IWbemLocator>& pLoc);
    HRESULT Connect2WMI(CComPtr<IWbemLocator> pLoc,CComPtr<IWbemServices>& pSvc);
    HRESULT SetProxySecLevels(CComPtr<IWbemServices> pSvc);
    virtual HRESULT Excute(CComPtr<IWbemServices> pSvc) = 0;
private:
    std::wstring m_wstrNamespace;
};