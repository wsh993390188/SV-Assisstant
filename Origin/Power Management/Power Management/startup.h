#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <iostream>
using namespace std;

struct RegKeyDetail
{
	wstring Type;
	wstring sRegKeyName;
	wstring sRegKeyData;
};

void ShowAutoRunList();
BOOLEAN EnumRegistryKeyValue(
	_In_ const HKEY hKey,
	_In_ const wstring& SubKey,
	_In_ const wstring& KeyType,
	_Out_ vector<RegKeyDetail>& Result
);
