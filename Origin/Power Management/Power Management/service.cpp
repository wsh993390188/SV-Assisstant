#include "stdafx.h"
#include "service.h"

using namespace std;

void Enumservice()
{
	locale loc("chs");
	wcout.imbue(loc);
		SC_HANDLE SCMan = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); //打开系统服务控制器   
		if (SCMan == NULL)
		{
			wcout << L"OpenSCManager Error" << endl;
			return;
		}
		LPENUM_SERVICE_STATUS service_status; //保存系统服务的结构   
		DWORD cbBytesNeeded = NULL;
		DWORD ServicesReturned = NULL;
		DWORD ResumeHandle = NULL;

		service_status = (LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, 1024 * 64);//分配内存，注意大小   


																			//获取系统服务的简单信息   
		bool ESS = EnumServicesStatus(SCMan, //系统服务句柄   
			SERVICE_WIN32, //服务的类型   
			SERVICE_STATE_ALL,  //服务的状态   
			(LPENUM_SERVICE_STATUS)service_status,  //输出参数，系统服务的结构   
			1024 * 64,  // 结构的大小   
			&cbBytesNeeded, //输出参数，接收返回所需的服务   
			&ServicesReturned, //输出参数，接收返回服务的数量   
			&ResumeHandle); //输入输出参数，第一次调用必须为0，返回为0代表成功   
		if (ESS == NULL)
		{
			wcout << L"EnumServicesStatus Error" << endl;
			return;
		}
		for (size_t i = 0; i < ServicesReturned; i++)
		{
			wcout << L"服务名:" << service_status[i].lpDisplayName << endl;
			switch (service_status[i].ServiceStatus.dwCurrentState)// 服务状态   
			{
			case SERVICE_CONTINUE_PENDING:
				wcout << L"状态: 挂起" << endl;
				break;
			case SERVICE_PAUSE_PENDING:
				wcout << L"状态: 正在暂停" << endl;
				break;
			case SERVICE_PAUSED:
				wcout << L"状态: 已暂停 " << endl;
				break;
			case SERVICE_RUNNING:
				wcout << L"状态: 运行 " << endl;
				break;
			case SERVICE_START_PENDING:
				wcout << L"状态: 启动 " << endl;
				break;
			case SERVICE_STOPPED:
				wcout << L"状态: 停止 " << endl;
				break;
			default:
				wcout << L"状态: 未知 " << endl;
				break;
			}
			LPQUERY_SERVICE_CONFIG lpServiceConfig = NULL; //服务详细信息结构   
			SC_HANDLE service_curren = NULL; //当前的服务句柄   
			DWORD dw_Ret = 0;
			service_curren = OpenService(SCMan, service_status[i].lpServiceName, SERVICE_QUERY_CONFIG); //打开当前服务   
			while (!QueryServiceConfig(service_curren, lpServiceConfig, dw_Ret, &ResumeHandle))
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					if (lpServiceConfig)
						HeapFree(GetProcessHeap(), 0, lpServiceConfig);
					lpServiceConfig = (LPQUERY_SERVICE_CONFIG)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ResumeHandle);
					dw_Ret = ResumeHandle;
				}
				else
					break;
			}
			dw_Ret = 0;
			if (lpServiceConfig)
			{
				switch (lpServiceConfig->dwStartType)
				{
				case SERVICE_AUTO_START:
				case SERVICE_BOOT_START:
					wcout << L"启动类型: 自动 " << endl;
					break;
				case SERVICE_DEMAND_START:
				case SERVICE_SYSTEM_START:
					wcout << L"启动类型: 手动 " << endl;
					break;
				case SERVICE_DISABLED:
					wcout << L"启动类型: 禁用 " << endl;
					break;
				default:
					wcout << L"启动类型: 未知 " << endl;
					break;
				}
			}
			LPSERVICE_DESCRIPTION descripton = NULL;
			while (!QueryServiceConfig2(service_curren, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)descripton, dw_Ret, &ResumeHandle))
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					if (descripton)
						HeapFree(GetProcessHeap(), 0, descripton);
					descripton = (LPSERVICE_DESCRIPTION)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ResumeHandle);
					dw_Ret = ResumeHandle;
				}
				else
					break;
			}

			if (descripton)
			{
				wcout << L"服务描述符:" << descripton->lpDescription << endl;
				HeapFree(GetProcessHeap(), 0, descripton);
				descripton = NULL;
			}
			dw_Ret = 0;

			wcout << L"路径: " <<lpServiceConfig->lpBinaryPathName <<endl;//服务的路径   
			CloseServiceHandle(service_curren);//关闭当前服务的句柄   
			if (lpServiceConfig)
			{
				HeapFree(GetProcessHeap(), 0, lpServiceConfig);
				lpServiceConfig = NULL;
			}
		}
		CloseServiceHandle(SCMan);//关闭服务管理器的句柄   
}