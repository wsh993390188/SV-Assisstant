/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "..\stdafx.h"
#include "UtilityFx.h"
#include "..\StringHelper.hpp"

#include <io.h>
#pragma comment(lib,"version.lib")

////------------------------------------------------
//   Debug
////------------------------------------------------

void DebugPrint(const CString& cstr)
{
	auto log = Hardware::Utils::wstringToUtf8(cstr.GetString());
	spdlog::info(log);
}

////------------------------------------------------
//   File Information
////------------------------------------------------

int GetFileVersion(const TCHAR* file, TCHAR* version)
{
	ULONG reserved = 0;
	VS_FIXEDFILEINFO vffi;
	TCHAR* buf = NULL;
	int  Locale = 0;
	TCHAR str[256];
	str[0] = '\0';

	UINT size = GetFileVersionInfoSize((TCHAR*)file, &reserved);
	TCHAR* vbuf = new TCHAR[size];
	if (GetFileVersionInfo((TCHAR*)file, 0, size, vbuf))
	{
		VerQueryValue(vbuf, L"\\", (void**)&buf, &size);
		CopyMemory(&vffi, buf, sizeof(VS_FIXEDFILEINFO));

		VerQueryValue(vbuf, L"\\VarFileInfo\\Translation", (void**)&buf, &size);
		CopyMemory(&Locale, buf, sizeof(int));
		wsprintf(str, L"\\StringFileInfo\\%04X%04X\\%s",
			LOWORD(Locale), HIWORD(Locale), L"FileVersion");
		VerQueryValue(vbuf, str, (void**)&buf, &size);

		_tcscpy_s(str, 256, buf);
		if (version != NULL)
		{
			_tcscpy_s(version, 256, buf);
		}
	}
	delete[] vbuf;

	if (_tcscmp(str, L"") != 0)
	{
		return int(_tstof(str) * 100);
	}
	else
	{
		return 0;
	}
}

BOOL IsFileExist(const TCHAR* path)
{
	FILE* fp;
	errno_t err;

	err = _tfopen_s(&fp, path, L"rb");
	if (err != 0 || fp == NULL)
	{
		return FALSE;
	}
	fclose(fp);
	return TRUE;
}

////------------------------------------------------
//   Utility
////------------------------------------------------

typedef ULONGLONG(WINAPI* FuncGetTickCount64)();

ULONGLONG GetTickCountFx()
{
	static FuncGetTickCount64 pGetTickCount64 = (FuncGetTickCount64)GetProcAddress(GetModuleHandle(L"kernel32"), "GetTickCount64");

	if (pGetTickCount64 != NULL)
	{
		return (ULONGLONG)pGetTickCount64();
	}
	else
	{
		return (ULONGLONG)GetTickCount();
	}
}

ULONG64 B8toB64(BYTE b0, BYTE b1, BYTE b2, BYTE b3, BYTE b4, BYTE b5, BYTE b6, BYTE b7)
{
	ULONG64 data =
		((ULONG64)b7 << 56)
		+ ((ULONG64)b6 << 48)
		+ ((ULONG64)b5 << 40)
		+ ((ULONG64)b4 << 32)
		+ ((ULONG64)b3 << 24)
		+ ((ULONG64)b2 << 16)
		+ ((ULONG64)b1 << 8)
		+ ((ULONG64)b0 << 0);

	return data;
}

DWORD B8toB32(BYTE b0, BYTE b1, BYTE b2, BYTE b3)
{
	DWORD data =
		((DWORD)b3 << 24)
		+ ((DWORD)b2 << 16)
		+ ((DWORD)b1 << 8)
		+ ((DWORD)b0 << 0);

	return data;
}