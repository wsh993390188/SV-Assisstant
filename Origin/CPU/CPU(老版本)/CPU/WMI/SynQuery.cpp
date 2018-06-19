#include "../stdafx.h"
#include "SynQuery.h"

#include <atlbase.h>
#include <atlcom.h>

CSynQuery::CSynQuery(const wstring& wszNamespace, const wstring& wszWQLQuery)
{
    SetNamespace(wszNamespace);
    m_wszWQLQuery = wszWQLQuery;
}

CSynQuery::~CSynQuery(void)
{
}

HRESULT CSynQuery::Excute( CComPtr<IWbemServices> pSvc )
{
    HRESULT hr = WBEM_S_FALSE;

    do {
        CComPtr<IEnumWbemClassObject> pEnumerator = NULL;
        hr = pSvc->ExecQuery( 
            CComBSTR("WQL"),
            CComBSTR(m_wszWQLQuery.c_str()),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator );

        CHECKWMIHR(hr);

        ULONG uReturn = 0;

        while (pEnumerator) {
            CComPtr<IWbemClassObject> pclsObj = NULL;
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if ( 0 == uReturn) {
                break;
            }
            DealWithIWbemClassObject(pclsObj);
        }

    } while (0);

    return hr;  
}

HRESULT CSynQuery::DealWithIWbemClassObject( CComPtr<IWbemClassObject> pclsObj )
{
    HRESULT hr = WBEM_S_NO_ERROR; 
    do  {
        CComVariant vtClass; 

//         hr = pclsObj->Get(L"__CLASS", 0, &vtClass, NULL, NULL); 
//         CHECKWMIHR(hr);
//         if ( VT_BSTR == vtClass.vt ) {
//             wprintf(L"\n%s\n", vtClass.bstrVal);
//         }

       // wprintf(L"\n");

        hr = pclsObj->BeginEnumeration(WBEM_FLAG_LOCAL_ONLY);

        do {
            CComBSTR bstrName;
            CComVariant Value;
            CIMTYPE type;
            LONG lFlavor = 0;
            hr = pclsObj->Next(0, &bstrName, &Value, &type, &lFlavor);
            CHECKWMIHR(hr);
            hr = DealWithSingleItem(bstrName, Value, type, lFlavor);            
        }while ( WBEM_S_NO_ERROR == hr );

        hr = pclsObj->EndEnumeration();
    } while (0);

    return hr;
}

HRESULT CSynQuery::DealWithSingleItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor )
{   
    return WBEM_S_NO_ERROR;
}