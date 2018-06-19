#include "stdafx.h"
#include "SynQuery.h"

CDiskQuery::CDiskQuery(const wstring& wszNamespace, const wstring& wszWQLQuery)
{
	SetNamespace(wszNamespace);
    m_wszWQLQuery = wszWQLQuery;
}

CDiskQuery::~CDiskQuery(void)
{}

template<typename T>
inline void CDiskQuery::SafeDeleteArray(T *& Array)
{
	if (Array)
	{
		delete[] Array;
		Array = NULL;
	}
}

template<typename T>
inline void CDiskQuery::SafeDeleteData(T *& Data)
{
	if (Data)
	{
		delete Data;
		Data = NULL;
	}
}

HRESULT CDiskQuery::Excute( CComPtr<IWbemServices> pSvc )
{
	this->wmi_info.clear();
    HRESULT hr = S_OK;
	ULONG m_Return = 0;
	size_t count = 0;

	CComPtr<IEnumWbemClassObject> pEnumDev1st = NULL;
	CComPtr<IEnumWbemClassObject> pEnumDev2nd = NULL;
	CComPtr<IWbemClassObject> pDev = NULL;
	wstring cstr1 = L"";

	CComVariant pVal;
	try
	{
		count = m_wszWQLQuery.find(L"Win32");
		wstring QueryName = m_wszWQLQuery.substr(count, m_wszWQLQuery.size());
		const CString WQLQueryName(QueryName.c_str());
		hr = pSvc->ExecQuery(
			_bstr_t(L"WQL"),
			_bstr_t(m_wszWQLQuery.c_str()),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumDev1st
		);
		if (FAILED(hr))
		{
			return hr;
		}
		count = 0;
		while (pEnumDev1st && SUCCEEDED(pEnumDev1st->Next(10000, 1, &pDev, &m_Return)) && m_Return == 1 && count < 256)
		{
			CString DeviceID1 = _T("");
			++count;
			if (pDev->Get(L"DeviceID", 0L, &pVal, NULL, NULL) == WBEM_NO_ERROR && pVal.vt > VT_NULL)
			{
				DeviceID1 = pVal.bstrVal;
				DeviceID1.Replace(_T("\\"), _T("\\\\"));
				pVal.Clear();
			}
			if (pDev)
				pDev = NULL;
			CString mapping = _T("");
			mapping.Format(_T("ASSOCIATORS OF {%s.DeviceID=\"%s\"} WHERE AssocClass = %sDevice"), WQLQueryName, DeviceID1, WQLQueryName);
			hr = pSvc->ExecQuery(
				_bstr_t(L"WQL"),
				_bstr_t(mapping),
				WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
				NULL,
				&pEnumDev2nd
			);
			if (FAILED(hr))
			{
				break;
			}
			ULONG count2nd = 0;
			while (pEnumDev2nd && SUCCEEDED(pEnumDev2nd->Next(10000, 1, &pDev, &m_Return)) && m_Return == 1 && count2nd < 256)
			{
				CString DeviceID2 = _T(""), Name = _T("");
				++count2nd;
				if (pDev->Get(L"DeviceID", 0L, &pVal, NULL, NULL) == WBEM_NO_ERROR && pVal.vt > VT_NULL)
				{
					DeviceID2 = pVal.bstrVal;
					Disk_DeviceID.emplace_back(DeviceID2);
				}
				if (pDev->Get(L"Name", 0L, &pVal, NULL, NULL) == WBEM_NO_ERROR && pVal.vt > VT_NULL)
				{
					Name = pVal.bstrVal;
					Disk_Name.emplace_back(Name);
				}
				if (pDev)
					pDev = NULL;
			}
			if (pEnumDev2nd)
				pEnumDev2nd = NULL;
		}
		if (pEnumDev1st)
			pEnumDev1st = NULL;
	}
	catch (...)
	{
		//DebugPrint();
	}
	m_wszWQLQuery = L"SELECT * FROM Win32_DiskDrive";
	WMI_DISK_INFO disktmp;
	try
	{
		hr = pSvc->ExecQuery(
			_bstr_t(L"WQL"),
			_bstr_t(m_wszWQLQuery.c_str()),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumDev1st);
		if (FAILED(hr))
		{
			return hr;
		}
		count = 0;
		while (pEnumDev1st && SUCCEEDED(pEnumDev1st->Next(10000, 1, &pDev, &m_Return)) && count < 256)
		{
			++count;
			if (m_Return != 1)
			{
				break; 
			}
			if (pDev->Get(L"Size", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				disktmp.diskSize = pVal.bstrVal;
				pVal.Clear();
				//DebugPrint(_T("diskSize:") + diskSize);
				//VariantClear(&pVal);
			}
			if (pDev->Get(L"DeviceID", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				disktmp.deviceid = pVal.bstrVal;
				//DebugPrint(_T("deviceId:") + deviceId);
				disktmp.deviceid.Replace(_T("\\"), _T("\\\\"));
				if (_ttoi(disktmp.deviceid.Right(2)) >= 10)
				{
					disktmp.physicalDriveId = _ttoi(disktmp.deviceid.Right(2));
				}
				else
				{
					disktmp.physicalDriveId = _ttoi(disktmp.deviceid.Right(1));
				}
				pVal.Clear();
			}
			if (pDev->Get(L"Model", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				disktmp.model = pVal.bstrVal;
				disktmp.model.MakeUpper();
				//DebugPrint(_T("model:") + model);
				pVal.Clear();
			}
			if (pDev->Get(L"FirmwareRevision", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				disktmp.firmware = pVal.bstrVal;
				pVal.Clear();
			}
			if (pDev->Get(L"SCSIPort", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				disktmp.scsiPort = pVal.intVal;
				pVal.Clear();
			}
			if (pDev->Get(L"SCSITargetId", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				disktmp.scsiTargetId = pVal.intVal;
				pVal.Clear();
			}
			if (pDev->Get(L"SCSIBus", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				disktmp.scsiBus = pVal.intVal;
				pVal.Clear();
			}
			if (pDev->Get(L"MediaType", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				disktmp.mediaType = pVal.bstrVal;
				//DebugPrint(_T("mediaType:") + mediaType);
				disktmp.mediaType.MakeLower();
				pVal.Clear();
			}
			if (pDev->Get(L"InterfaceType", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				disktmp.interfaceTypeWmi = pVal.bstrVal;
				//DebugPrint(_T("interfaceTypeWmi:") + interfaceTypeWmi);
				pVal.Clear();
			}
			if (pDev->Get(L"PNPDeviceID", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
			{
				disktmp.pnpDeviceId = pVal.bstrVal;
				//DebugPrint(_T("interfaceTypeWmi:") + interfaceTypeWmi);
				pVal.Clear();
			}
			wmi_info.emplace_back(disktmp);
			if (pDev)
			{
				pDev = NULL;
			}

		}
		if (pEnumDev1st)
		{
			pEnumDev1st = NULL;
		}
	}
	catch (...)
	{
		//DEBUGPrint
	}
    return hr;  
}

WMI_DISK_INFO::WMI_DISK_INFO()
{
	this->model = _T("");
	this->deviceid = _T("");
	this->diskSize = _T("");
	this->mediaType = _T("");
	this->interfaceTypeWmi = _T("");
	this->pnpDeviceId = _T("");
	this->firmware = _T("");
	this->physicalDriveId = -1;
	this->scsiBus = -1;
	this->scsiPort = -1;
	this->scsiTargetId = -1;
}
