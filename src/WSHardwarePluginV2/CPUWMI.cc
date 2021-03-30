#include <objbase.h>
#include <memory>

#include "CPUWMI.h"

using namespace Hardware::CPU;
CPU_WMI::CPU_WMI() :
	ExtClock(0), Core(0), Thread(0), Revision(0)
{
	this->SetNamespace(L"root\\CIMV2");
}

HRESULT CPU_WMI::Excute(CComPtr<IWbemServices> pSvc)
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
			_bstr_t("SELECT MaxClockSpeed,ExtClock,MaxClockSpeed,NumberOfCores,ProcessorId,Revision,NumberOfLogicalProcessors FROM Win32_Processor"),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator
		);

		while (pEnumerator && SUCCEEDED(pEnumerator->Next(10000, 1, &pDev, &m_Return) && count < 256))
		{
			if (m_Return == 0)	break;
			count++;
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
			if (pDev->Get(L"Revision", 0L, &pVal, nullptr, nullptr) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				Revision = pVal.intVal;
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