#include "stdafx.h"
#include "CPUWMI.h"

// #pragma warning(push)
// #pragma warning(disable:4819)
// #include "WMI.h"
// #pragma warning(pop)

#include <objbase.h>

CWMI::CWMI() : m_wstrNamespace("root\\CIMV2"), Name("Unknown"), ProcessorId("Unknown"), SocketDesignation("Unknown"),
CurrentClockSpeed(0), ExtClock(0), Core(0), Thread(0), Revision(0), MaxClockSpeed(0), UpgradeMethod(0)
{}

CWMI::~CWMI(void)
{
	CoUninitialize();
}

HRESULT CWMI::InitialCom()
{
	HRESULT hr = E_FAIL;
	do {
		hr = CoInitialize(0);
		CHECKHR(hr);

	} while (0);
	return hr;
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
			IID_IWbemLocator, (LPVOID *)&pLoc);
		CHECKHR(hr);

	} while (0);
	return hr;
}

HRESULT CWMI::Connect2WMI(CComPtr<IWbemLocator> pLoc, CComPtr<IWbemServices>& pSvc)
{
	HRESULT hr = E_FAIL;
	do {
		hr = pLoc->ConnectServer(
			CComBSTR(m_wstrNamespace.c_str()),
			NULL, NULL, NULL, NULL, NULL, NULL, &pSvc);
		CHECKHR(hr);
	} while (0);
	return hr;
}

HRESULT CWMI::SetProxySecLevels(CComPtr<IWbemServices> pSvc)
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

HRESULT CWMI::Excute(CComPtr<IWbemServices> pSvc)
{
	HRESULT hr = WBEM_S_FALSE;
	ULONG m_Return = 0;
	size_t count = 0;
	do 
	{
		CComPtr<IEnumWbemClassObject> pEnumerator = nullptr;
		CComPtr<IWbemClassObject> pDev = nullptr;
		CComVariant pVal;
		hr = pSvc->ExecQuery(
			_bstr_t("WQL"),
			_bstr_t("SELECT Name,CurrentClockSpeed ,MaxClockSpeed,ExtClock,MaxClockSpeed,NumberOfCores,ProcessorId,Revision,SocketDesignation,UpgradeMethod,NumberOfLogicalProcessors FROM Win32_Processor"),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator
		);

		while (pEnumerator && SUCCEEDED(pEnumerator->Next(10000, 1, &pDev, &m_Return) && count < 256))
		{
			if(m_Return == 0)	break;
			count++;
			if (pDev->Get(L"Name", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				unique_ptr<char[]> t(_com_util::ConvertBSTRToString(pVal.bstrVal));
				Name = t.get();
				char* p = _com_util::ConvertBSTRToString(pVal.bstrVal);
				Name = p;
				delete[] p;
				p = nullptr;
			}
			if (pDev->Get(L"CurrentClockSpeed", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				CurrentClockSpeed = pVal.intVal;
			}
			if (pDev->Get(L"MaxClockSpeed", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				MaxClockSpeed = pVal.intVal;
			}
			if (pDev->Get(L"ExtClock", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				ExtClock = pVal.intVal;
			}
			if (pDev->Get(L"NumberOfCores", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				Core = pVal.intVal;
			}
			if (pDev->Get(L"ProcessorId", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				unique_ptr<char[]> t(_com_util::ConvertBSTRToString(pVal.bstrVal));
				ProcessorId = t.get();
				char* p = _com_util::ConvertBSTRToString(pVal.bstrVal);
				ProcessorId = p;
				delete[] p;
				p = nullptr;
			}
			if (pDev->Get(L"Revision", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				Revision = pVal.intVal;
			}
			if (pDev->Get(L"SocketDesignation", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
 				unique_ptr<char[]> t(_com_util::ConvertBSTRToString(pVal.bstrVal));
 				SocketDesignation = t.get();
			}
			if (pDev->Get(L"UpgradeMethod", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				UpgradeMethod = pVal.intVal;
			}
			if (pDev->Get(L"NumberOfLogicalProcessors", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				Thread = pVal.intVal;
			}
			if (pDev)
				pDev = nullptr;
			pVal.Clear();
		}
		if (pEnumerator)
			pEnumerator = nullptr;
		CHECKWMIHR(hr);

	} while (0);
	return hr;
}

HRESULT CWMI::ExcuteFun()
{
	HRESULT hr = E_FAIL;
	{
		CComPtr<IWbemLocator> pLoc = NULL;
		CComPtr<IWbemServices> pSvc = NULL;

		do {
			hr = InitialCom();
			CHECKHR(hr);

			hr = SetComSecLevels();
			CHECKHR(hr);

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
	return hr;
}
