#include "stdafx.h"
#include "TaskScheduler.h"

#define _WIN32_DCOM

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

BOOLEAN EnumFolderTaskScheduler()
{
	//  ------------------------------------------------------
	//  Initialize COM.
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		printf("\nCoInitializeEx failed: %x", hr);
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

	if (FAILED(hr))
	{
		printf("\nCoInitializeSecurity failed: %x", hr);
		CoUninitialize();
		return FALSE;
	}


	CComPtr<ITaskService> m_pService = NULL;

	hr = CoCreateInstance(CLSID_TaskScheduler,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskService,
		(void**)&m_pService);

	if (FAILED(hr))
	{
		printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
		CoUninitialize();
	}

	//  Connect to the task service.
	hr = m_pService->Connect(_variant_t(), _variant_t(),
		_variant_t(), _variant_t());
	if (FAILED(hr))
	{
		printf("ITaskService::Connect failed: %x", hr);
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
		printf("Cannot get Root Folder pointer: %x", hr);
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
		printf("Cannot get the registered tasks.: %x", hr);
		CoUninitialize();
		return FALSE;
	}

	LONG numTasks = 0;
	hr = pTaskCollection->get_Count(&numTasks);

	if (numTasks == 0)
	{
		printf("\nNo Tasks are currently running");
		CoUninitialize();
		return FALSE;
	}

	printf("\nNumber of Tasks : %d", numTasks);

	TASK_STATE taskState;

	for (LONG i = 0; i < numTasks; i++)
	{
		CComPtr<IRegisteredTask> pRegisteredTask = NULL;
		hr = pTaskCollection->get_Item(_variant_t(i + 1), &pRegisteredTask);

		if (SUCCEEDED(hr))
		{
			BSTR taskName = NULL;
			hr = pRegisteredTask->get_Name(&taskName);
			if (SUCCEEDED(hr))
			{
				printf("\nTask Name: %S", taskName);
				//SysFreeString(taskName);

				hr = pRegisteredTask->get_State(&taskState);
				if (SUCCEEDED(hr))
					printf("\n\tState: %d", taskState);
				else
					printf("\n\tCannot get the registered task state: %x", hr);
			}
			else
			{
				printf("\nCannot get the registered task name: %x", hr);
			}
		}
		else
		{
			printf("\nCannot get the registered task item at index=%d: %x", i + 1, hr);
		}
	}
	pTaskCollection = NULL;
	CoUninitialize();
	return TRUE;
}

BOOLEAN EnumRuningTaskScheduler()
{
	//  ------------------------------------------------------
	//  Initialize COM.
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		printf("\nCoInitializeEx failed: %x", hr);
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

	if (FAILED(hr))
	{
		printf("\nCoInitializeSecurity failed: %x", hr);
		CoUninitialize();
		return FALSE;
	}

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
		printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
		CoUninitialize();
		return FALSE;
	}

	//  Connect to the task service.
	hr = pService->Connect(_variant_t(), _variant_t(),
		_variant_t(), _variant_t());
	if (FAILED(hr))
	{
		printf("ITaskService::Connect failed: %x", hr);
		CoUninitialize();
		return FALSE;
	}

	// Get the running tasks.
	CComPtr<IRunningTaskCollection> pRunningTasks = NULL;
	hr = pService->GetRunningTasks(TASK_ENUM_HIDDEN, &pRunningTasks);

	pService = NULL;
	if (FAILED(hr))
	{
		printf("Cannot get Root Folder pointer: %x", hr);
		CoUninitialize();
		return FALSE;
	}

	LONG numTasks = 0;
	hr = pRunningTasks->get_Count(&numTasks);

	if (numTasks == 0)
	{
		printf("\nNo Tasks are currently running");
		CoUninitialize();
		return FALSE;
	}

	printf("\nNumber of running tasks : %d", numTasks);

	TASK_STATE taskState;

	for (LONG i = 0; i < numTasks; i++)
	{
		CComPtr<IRunningTask> pRunningTask = NULL;
		hr = pRunningTasks->get_Item(_variant_t(i + 1), &pRunningTask);

		if (SUCCEEDED(hr))
		{
			BSTR taskName = NULL;
			hr = pRunningTask->get_Name(&taskName);
			if (SUCCEEDED(hr))
			{
				printf("\nTask Name: %S", taskName);
				SysFreeString(taskName);

				hr = pRunningTask->get_State(&taskState);
				if (SUCCEEDED(hr))
					printf("\n\tState: %d", taskState);
				else
					printf("\n\tCannot get the registered task state: %x", hr);
			}
			else
			{
				printf("\nCannot get the registered task name: %x", hr);
			}
		}
		else
		{
			printf("\nCannot get the registered task item at index=%d: %x", i + 1, hr);
		}
	}

	pRunningTasks = NULL;
	CoUninitialize();
	return TRUE;
}