#include "stdafx.h"
#pragma warning(push)
#pragma warning(disable:4819)
#include "WMI.h"
#pragma warning(pop)

#include <objbase.h>
#include "DnpService.h"

CWMI::CWMI(): initWmi(true)
{
	CDnpService	cService;

	if (!cService.IsServiceRunning(_T("Winmgmt")))
	{
		initWmi = cService.EasyStart(_T("Winmgmt"));
	}
}

CWMI::~CWMI(void)
{
    CoUninitialize();
}

VOID CWMI::SetNamespace(std::wstring wstrNamespace)
{
    m_wstrNamespace = wstrNamespace;
}

HRESULT CWMI::InitialCom()
{
    CoInitialize(NULL);
    return S_OK;
}

HRESULT CWMI::SetComSecLevels()
{
    // Set general COM security levels --------------------------
    // Note: If you are using Windows 2000, you must specify -
    // the default authentication credentials for a user by using
    // a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
    // parameter of CoInitializeSecurity ------------------------

	CoInitializeSecurity(
        NULL, 
        -1,                          // COM negotiates service
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities 
        NULL                         // Reserved
        );
    return S_OK;
}

HRESULT CWMI::ObtainLocator2WMI(CComPtr<IWbemLocator>& pLoc)
{
    HRESULT hr = E_FAIL;
    do {
        hr = CoCreateInstance(
            CLSID_WbemLocator,             
            0, 
            CLSCTX_INPROC_SERVER, 
            IID_IWbemLocator, (LPVOID *) &pLoc);
        CHECKHR(hr);

    } while (0);
    return hr;
}

HRESULT CWMI::Connect2WMI( CComPtr<IWbemLocator> pLoc,CComPtr<IWbemServices>& pSvc )
{
    HRESULT hr = E_FAIL;
    do {
        hr = pLoc->ConnectServer(
            CComBSTR(m_wstrNamespace.c_str()), 
            NULL, NULL, NULL, NULL, NULL, NULL, &pSvc );
        CHECKHR(hr);
    } while (0);
    return hr;
}

HRESULT CWMI::SetProxySecLevels( CComPtr<IWbemServices> pSvc )
{
    HRESULT hr = E_FAIL;
    do {
        hr = CoSetProxyBlanket(
            pSvc,                        // Indicates the proxy to set
            RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx 
            RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx 
            NULL,                        // Server principal name 
            RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
            RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
            NULL,                        // client identity
            EOAC_NONE                    // proxy capabilities 
            );
        CHECKHR(hr);
    } while (0);
    return hr;
}

HRESULT CWMI::ExcuteFun()
{
    HRESULT hr = E_FAIL;
    CComPtr<IWbemLocator> pLoc = NULL;
    CComPtr<IWbemServices> pSvc = NULL;
	if (initWmi)
	{
		do {
			InitialCom();
			SetComSecLevels();

			hr = ObtainLocator2WMI(pLoc);
			CHECKHR(hr);

			hr = Connect2WMI(pLoc, pSvc);
			CHECKHR(hr);

			hr = SetProxySecLevels(pSvc);
			CHECKHR(hr);

			hr = Excute(pSvc);
			CHECKHR(hr);

		} while (0);
	}
	else
		hr = ERROR_SERVICE_DOES_NOT_EXIST;
    return hr;
}
