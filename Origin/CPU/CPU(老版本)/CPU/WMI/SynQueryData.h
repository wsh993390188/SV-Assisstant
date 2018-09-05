#pragma once
#include "SynQuery.h"


class CSynQueryData : public CSynQuery
{
public:
    CSynQueryData(const wstring& wszNamespace, const wstring& wszWQLQuery);
    ~CSynQueryData(void);
	vector<string> ClassData();
	vector<string> ClassName();
private:
    HRESULT DealWithSingleItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
    HRESULT DealWithUnknownTypeItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
    VOID PrintfItem( CComBSTR bstrName, CComVariant Value, CIMTYPE type, LONG lFlavor );
	vector<string> class_name;
	vector<string> class_data;
};
