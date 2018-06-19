// Process.cpp: implementation of the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Process.h"
#include <tlhelp32.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcess::CProcess()
{

}

CProcess::~CProcess()
{

}

std::wstring CProcess::GetProcessNameById(DWORD nPId,BOOL bPath)
{
	std::wstring strName;
	if(nPId == 0) return L"";
	HANDLE	hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"进程快照失败!");
		return L"";
	}
	
	PROCESSENTRY32 pe = {sizeof(pe)};
	
	for(BOOL b=::Process32First(hProcessSnap,&pe);b;b=::Process32Next(hProcessSnap,&pe))
	{
		if(pe.th32ProcessID == nPId) 
		{
			if(!bPath)
			{
				strName = pe.szExeFile;	
			}
			else
			{
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,true,nPId);//获得指定进程的有关权限
				if(!hProcess) return L"";
				TCHAR szPath[MAX_PATH];
				GetModuleFileNameEx((HINSTANCE)hProcess,NULL,szPath,MAX_PATH);
				strName = szPath;
//				AfxMessageBox(szPath);
/*				HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,nPId);
				if(!hModuleSnap) return "";
				MODULEENTRY32 me32 = {sizeof(MODULEENTRY32)};
				if(Module32First(hModuleSnap,&me32))
				{
					strName.Format("%s",me32.szExePath);
				}
				CloseHandle(hModuleSnap);*/
//				AfxMessageBox(strName);
			}
			CloseHandle(hProcessSnap);
			return strName;
		}
	}
	CloseHandle(hProcessSnap);
	return L"";
}
