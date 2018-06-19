#include "stdafx.h"
#include "startup.h"


void ShowAutoRunList()
{
	//HKLM Run
	wcout << L"HKLM Run ---------------------------------------------------\n";
	HKEY hKey = HKEY_LOCAL_MACHINE;
	std::wstring strType = L"HKLM Run", strTemp;
	size_t i;
	vector<RegKeyDetail > ListEnumKeyValue;
	// 	vector<string > ListEnumKey;
	std::wstring strFullName(L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	EnumRegistryKeyValue(hKey, strFullName, strType, ListEnumKeyValue);

	if (ListEnumKeyValue.size() > 0)
	{
		for (i = 0; i < ListEnumKeyValue.size(); i++)
		{
			wcout << ListEnumKeyValue[i].sRegKeyName.c_str();
			wcout << " " << ListEnumKeyValue[i].sRegKeyData.c_str() << endl;
		}
	}
	i = ListEnumKeyValue.size();
	//HKCU Run
	wcout << L"HKCU Run ---------------------------------------------------\n";
	hKey = HKEY_CURRENT_USER;
	strType = L"HKCU Run";
	EnumRegistryKeyValue(hKey, strFullName, strType, ListEnumKeyValue);
	if (ListEnumKeyValue.size() > 0)
	{
		for (; i < ListEnumKeyValue.size(); i++)
		{
			wcout << ListEnumKeyValue[i].sRegKeyName.c_str();
			wcout << " " << ListEnumKeyValue[i].sRegKeyData.c_str() << endl;
		}
	}
	i = ListEnumKeyValue.size();
	//HKLM Winlogon
	wcout << L"HKLM Winlogon ---------------------------------------------------\n";
	hKey = HKEY_LOCAL_MACHINE;
	strType = L"HKLM Winlogon";
	strFullName = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon";
	EnumRegistryKeyValue(hKey, strFullName, strType, ListEnumKeyValue);
	if (ListEnumKeyValue.size() > 0)
	{
		for (; i < ListEnumKeyValue.size(); i++)
		{
			if (ListEnumKeyValue[i].sRegKeyName == L"Shell"
				|| ListEnumKeyValue[i].sRegKeyName == L"UIHost"
				|| ListEnumKeyValue[i].sRegKeyName == L"Userinit")
			{
				wcout << ListEnumKeyValue[i].sRegKeyName.c_str();
				wcout << " " << ListEnumKeyValue[i].sRegKeyData.c_str() << endl;
			}
		}
	}
	i = ListEnumKeyValue.size();
// 
// 	//HKLM Winlogon Notify
// 	cout << "HKLM Winlogon Notify ---------------------------------------------------\n";
// 	hKey = HKEY_LOCAL_MACHINE;
// 	strType = "HKLM Winlogon Notify";
// 	strFullName = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Notify";
// 	EnumRegistryKey(hKey, strFullName, ListEnumKey);
// 
 	HKEY hKeyRes;

// 
// 	if (ListEnumKey.size() > 0)
// 	{
// 		for (i = 0; i < ListEnumKey.size(); i++)
// 		{
// 			cout << ListEnumKey[i].c_str();
// 			strTemp = strFullName + "\\" + ListEnumKey[i];
// 			if (RegCreateKey(hKey, strTemp.c_str(), &hKeyRes) == ERROR_SUCCESS)
// 			{
// 				RegQueryValueEx(hKeyRes, "DLLName", 0, NULL, (LPBYTE)szValue, &dwLen);
// 				szValue[dwLen] = '\0';
// 				cout << " " << szValue << endl;
// 				RegCloseKey(hKeyRes);
// 			}
// 		}
// 	}
// 
	//KnownDLLs
	cout << "KnownDLLs ---------------------------------------------------\n";
	hKey = HKEY_LOCAL_MACHINE;
	strType = L"KnownDLLs";
	strFullName = L"SYSTEM\\ControlSet001\\Control\\Session Manager\\KnownDLLs";
	EnumRegistryKeyValue(hKey, strFullName, strType, ListEnumKeyValue);

	if (ListEnumKeyValue.size() > 0)
	{
		for (; i < ListEnumKeyValue.size(); i++)
		{
			if (ListEnumKeyValue[i].sRegKeyName != L"DllDirectory")
			{
				wcout << ListEnumKeyValue[i].sRegKeyName.c_str();
				wcout << ListEnumKeyValue[i].sRegKeyData.c_str() << endl;
			}
		}
	}
// 
// 	//RightMenu
// 	cout << "RightMenu ---------------------------------------------------\n";
// 	hKey = HKEY_CLASSES_ROOT;
// 	strType = L"RightMenu";
// 	strFullName = L"*\\shellex\\ContextMenuHandlers";
// 	EnumRegistryKey(hKey, strFullName, strType, ListEnumKey);
// 
// 	if (ListEnumKey.size() > 0)
// 	{
// 		for (i = 0; i < ListEnumKey.size(); i++)
// 		{
// 			dwLen = 1024;
// 
// 			if (ListEnumKey[i][0] == '{')
// 				continue;
// 
// 			cout << ListEnumKey[i].c_str();
// 
// 			strTemp = strFullName + "\\" + ListEnumKey[i];
// 
// 			if (RegCreateKey(HKEY_CLASSES_ROOT, strTemp.c_str(), &hKey) == ERROR_SUCCESS)
// 			{
// 				if (RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)szValue, &dwLen) == ERROR_SUCCESS)
// 				{
// 					strTemp = "CLSID\\";
// 					strTemp += szValue;
// 					strTemp += "\\InprocServer32";
// 
// 					dwLen = MAX_PATH;
// 
// 					if (RegCreateKey(HKEY_CLASSES_ROOT, strTemp.c_str(), &hKeyRes) == ERROR_SUCCESS)
// 					{
// 						if (RegQueryValueEx(hKeyRes, NULL, NULL, NULL, (LPBYTE)szValue, &dwLen) == ERROR_SUCCESS)
// 						{
// 							cout << " " << szValue << endl;
// 						}
// 						RegCloseKey(hKeyRes);
// 					}
// 				}
// 				RegCloseKey(hKey);
// 			}
// 		}
// 	}
}

BOOLEAN EnumRegistryKeyValue(
	_In_ const HKEY hKey,
	_In_ const wstring& SubKey,
	_In_ const wstring& KeyType,
	_Out_ vector<RegKeyDetail>& Result
)
{
	HKEY hsubkey;
	int ret = RegOpenKeyEx(
		hKey,
		SubKey.c_str(),
		NULL,
		KEY_READ,
		&hsubkey
	);
	if (ret != ERROR_SUCCESS)
	{
		cout << "OPEN FAILED" << endl;
		return FALSE;
	}

	DWORD dwSubKeyCnt;          
	DWORD dwSubKeyNameMaxLen;   
	DWORD dwKeyValueCnt;        	
	DWORD dwKeyValueNameMaxLen;   
	DWORD dwKeyValueDataMaxLen; 
	ret = RegQueryInfoKey(hsubkey, NULL, NULL, NULL, &dwSubKeyCnt, &dwSubKeyNameMaxLen, NULL, &dwKeyValueCnt, &dwKeyValueNameMaxLen, &dwKeyValueDataMaxLen, NULL, NULL);
	if (ret != ERROR_SUCCESS) // Error  
	{
		cout << "RegQueryInfoKey FAILED" << endl;
		return FALSE;
	}
	dwKeyValueNameMaxLen++;
	dwKeyValueDataMaxLen++;
	unique_ptr<TCHAR[]> lpKeyName(new TCHAR[dwKeyValueNameMaxLen]);
	unique_ptr<TCHAR[]> lpData(new TCHAR[dwKeyValueDataMaxLen]);
	for (UINT Index = 0; Index < dwKeyValueCnt; ++Index)
	{
		DWORD Type = 0;
		DWORD Namelen = dwKeyValueNameMaxLen;
		DWORD Datalen = dwKeyValueDataMaxLen;
		if (RegEnumValue(hsubkey, Index, lpKeyName.get(), &Namelen, NULL, &Type, (LPBYTE)lpData.get(), &Datalen) == ERROR_SUCCESS)
		{
			if(KeyType == L"HKLM Winlogon")
			{
				if (!wcscmp(L"Shell", lpKeyName.get())
					|| !wcscmp(L"UIHost", lpKeyName.get())
					|| !wcscmp(L"Userinit", lpKeyName.get()))
				{
					RegKeyDetail temp;
					temp.sRegKeyName = lpKeyName.get();
					temp.sRegKeyData = lpData.get();
					temp.Type = KeyType;
					Result.push_back(temp);
				}
			}
			else
			{
				RegKeyDetail temp;
				temp.sRegKeyName = lpKeyName.get();
				temp.sRegKeyData = lpData.get();
				temp.Type = KeyType;
				Result.push_back(temp);
			}
		}
	}
	RegCloseKey(hsubkey);
	return TRUE;
}
