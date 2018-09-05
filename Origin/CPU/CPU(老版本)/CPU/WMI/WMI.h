#pragma once
#include "../stdafx.h"
#include <windows.h>
#include <comdef.h>
#include <wbemidl.h>
#include <comutil.h>
#include <atlbase.h>
#include <atlcom.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>  
#include <memory>
#define CHECKHR(x) \
    if ( FAILED((x))) break; \

#define CHECKWMIHR(x) \
    if ( (x) != WBEM_S_NO_ERROR ) break; \

using namespace std;

class CWMI  
{  
public:  
	CWMI();  
	~CWMI(void);  //清理 
public:  
	HRESULT ExcuteFun();  
protected:  
	VOID SetNamespace(wstring wstrNamespace);  //命名空间设置
private:  
	HRESULT InitialCom();  //初始化COM库
	HRESULT SetComSecLevels();  //设置进程COM安全信息
	HRESULT ObtainLocator2WMI(CComPtr<IWbemLocator>& pLoc);  // 创建进程内COM服务器
	HRESULT Connect2WMI(CComPtr<IWbemLocator> pLoc,CComPtr<IWbemServices>& pSvc);  //连接WMI命名空间
	HRESULT SetProxySecLevels(CComPtr<IWbemServices> pSvc);  //设置WMI连接的安全等级
	virtual HRESULT Excute(CComPtr<IWbemServices> pSvc) = 0;  
private:  
	wstring m_wstrNamespace;  
}; 