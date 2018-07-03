#include "stdafx.h"
#include "TaskScheduler.h"

#define _WIN32_DCOM

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

using namespace SV_ASSIST::Software::Origin;

BOOLEAN SV_ASSIST::Software::Origin::EnumFolderTaskScheduler(std::list<TaskScheduler_Struct>& data)
{
	//  ------------------------------------------------------
	//  Initialize COM.
	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		OutputDebugPrintf("\nCoInitializeEx failed: %x", hr);
		return FALSE;
	}

	//  Set general COM security levels.
	CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		0,
		NULL);

	CComPtr<ITaskService> m_pService = NULL;

	hr = CoCreateInstance(CLSID_TaskScheduler,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskService,
		(void**)&m_pService);

	if (FAILED(hr))
	{
		OutputDebugPrintf("Failed to CoCreate an instance of the TaskService class: %x", hr);
		CoUninitialize();
	}

	//  Connect to the task service.
	hr = m_pService->Connect(_variant_t(), _variant_t(),
		_variant_t(), _variant_t());
	if (FAILED(hr))
	{
		OutputDebugPrintf("ITaskService::Connect failed: %x", hr);
		CoUninitialize();
		return FALSE;
	}
	
	//  ------------------------------------------------------
	//  Get the pointer to the root task folder.
	CComPtr<ITaskFolder> pRootFolder = NULL;
	hr = m_pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
	m_pService = NULL;
	if (FAILED(hr))
	{
		OutputDebugPrintf("Cannot get Root Folder pointer: %x", hr);
		CoUninitialize();
		return FALSE;
	}

	//  -------------------------------------------------------
	//  Get the registered tasks in the folder.
	CComPtr<IRegisteredTaskCollection> pTaskCollection = NULL;
	hr = pRootFolder->GetTasks(NULL, &pTaskCollection);
	pRootFolder = NULL;

	if (FAILED(hr))
	{
		OutputDebugPrintf("Cannot get the registered tasks.: %x", hr);
		CoUninitialize();
		return FALSE;
	}

	LONG numTasks = 0;
	hr = pTaskCollection->get_Count(&numTasks);

	if (numTasks == 0)
	{
		OutputDebugPrintf("\nNo Tasks are currently running");
		CoUninitialize();
		return FALSE;
	}

	OutputDebugPrintf("\nNumber of Tasks : %d", numTasks);

	TASK_STATE taskState;

	for (LONG i = 0; i < numTasks; i++)
	{
		CComPtr<IRegisteredTask> pRegisteredTask = NULL;
		TaskScheduler_Struct temp = {};
		hr = pTaskCollection->get_Item(_variant_t(i + 1), &pRegisteredTask);

		if (SUCCEEDED(hr))
		{
			BSTR taskName = NULL;
			hr = pRegisteredTask->get_Name(&taskName);
			if (SUCCEEDED(hr))
			{
				OutputDebugPrintf("\nTask Name: %S", taskName);
				temp.TaskName = taskName;
				SysFreeString(taskName);

				hr = pRegisteredTask->get_State(&taskState);
				if (SUCCEEDED(hr))
					temp.taskstate = taskState;
				else
					OutputDebugPrintf("\n\tCannot get the registered task state: %x", hr);
				data.emplace_back(temp);
			}
			else
			{
				OutputDebugPrintf("\nCannot get the registered task name: %x", hr);
			}
		}
		else
		{
			OutputDebugPrintf("\nCannot get the registered task item at index=%d: %x", i + 1, hr);
		}
	}
	pTaskCollection = NULL;
	CoUninitialize();
	return TRUE;
}

BOOLEAN SV_ASSIST::Software::Origin::EnumRuningTaskScheduler(std::list<TaskScheduler_Struct>& data)
{
	//  ------------------------------------------------------
	//  Initialize COM.
	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		OutputDebugPrintf("\nCoInitializeEx failed: %x", hr);
		return FALSE;
	}

	//  Set general COM security levels.
	hr = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		0,
		NULL);

	//  ------------------------------------------------------
	//  Create an instance of the Task Service. 
	CComPtr<ITaskService> pService = NULL;
	hr = CoCreateInstance(CLSID_TaskScheduler,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskService,
		(void**)&pService);
	if (FAILED(hr))
	{
		OutputDebugPrintf("Failed to CoCreate an instance of the TaskService class: %x", hr);
		CoUninitialize();
		return FALSE;
	}

	//  Connect to the task service.
	hr = pService->Connect(_variant_t(), _variant_t(),
		_variant_t(), _variant_t());
	if (FAILED(hr))
	{
		OutputDebugPrintf("ITaskService::Connect failed: %x", hr);
		CoUninitialize();
		return FALSE;
	}

	// Get the running tasks.
	CComPtr<IRunningTaskCollection> pRunningTasks = NULL;
	hr = pService->GetRunningTasks(TASK_ENUM_HIDDEN, &pRunningTasks);

	pService = NULL;
	if (FAILED(hr))
	{
		OutputDebugPrintf("Cannot get Root Folder pointer: %x", hr);
		CoUninitialize();
		return FALSE;
	}

	LONG numTasks = 0;
	hr = pRunningTasks->get_Count(&numTasks);

	if (numTasks == 0)
	{
		OutputDebugPrintf("\nNo Tasks are currently running");
		CoUninitialize();
		return FALSE;
	}

	OutputDebugPrintf("\nNumber of running tasks : %d", numTasks);

	TASK_STATE taskState;

	for (LONG i = 0; i < numTasks; i++)
	{
		TaskScheduler_Struct temp = {};
		CComPtr<IRunningTask> pRunningTask = NULL;
		hr = pRunningTasks->get_Item(_variant_t(i + 1), &pRunningTask);

		if (SUCCEEDED(hr))
		{
			BSTR taskName = NULL;
			hr = pRunningTask->get_Name(&taskName);
			if (SUCCEEDED(hr))
			{
				OutputDebugPrintf("\nTask Name: %S", taskName);
				temp.TaskName = taskName;
				SysFreeString(taskName);

				hr = pRunningTask->get_State(&taskState);
				if (SUCCEEDED(hr))
					temp.taskstate = taskState;
				else
					OutputDebugPrintf("\n\tCannot get the registered task state: %x", hr);
				data.emplace_back(temp);
			}
			else
			{
				OutputDebugPrintf("\nCannot get the registered task name: %x", hr);
			}
		}
		else
		{
			OutputDebugPrintf("\nCannot get the registered task item at index=%d: %x", i + 1, hr);
		}
	}

	pRunningTasks = NULL;
	CoUninitialize();
	return TRUE;
}