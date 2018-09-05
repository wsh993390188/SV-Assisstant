#include "../Driver/Driver.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, GetAllModuleNum)
#pragma alloc_text (PAGE, GetModuleBase)
#endif

NTSTATUS GetAllModuleNum(DWORD *dwcount)
{
	UNICODE_STRING DriverName;
	RtlInitUnicodeString(&DriverName, L"\\Driver\\SystemTest");
	PDRIVER_OBJECT  driverObject;
	NTSTATUS status = STATUS_SUCCESS;

	status = ObReferenceObjectByName(
		&DriverName,
		OBJ_CASE_INSENSITIVE,//不区分大小写 驱动名可以写大写或小写   
		NULL,  // 结构比较复杂设计对象特性的东西access state  
		0,       // 访问权限可以写0 写0完全访问不受控制FILE_ALL_ACCESS,  
		*IoDriverObjectType,//对象类型 注意加*号   
		KernelMode,//内核模式 有三种模式 enum 类型    
		NULL,                 //不知道 parse context  parse context  
		(PVOID*)&driverObject);//输出对象 我们要得到的驱动对象    
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	KdPrint(("Driver Name: %wZ", driverObject->DriverName));
	LDR_DATA_TABLE_ENTRY *LDR_Table = (LDR_DATA_TABLE_ENTRY*)driverObject->DriverSection;
	LDR_DATA_TABLE_ENTRY *first_LDR = LDR_Table;
	LIST_ENTRY*     ActiveProcessLinks = NULL;
	__try {
		while (LDR_Table != NULL)
		{
			(*dwcount)++;
			ActiveProcessLinks = LDR_Table->InLoadOrderLinks.Flink;
			LDR_Table = (LDR_DATA_TABLE_ENTRY*)ActiveProcessLinks;
			if (LDR_Table == first_LDR)
				break;
		}
		ActiveProcessLinks = LDR_Table->InLoadOrderLinks.Blink;
		LDR_Table = (LDR_DATA_TABLE_ENTRY*)ActiveProcessLinks;
		KdPrint(("Driver Ends: %wZ", LDR_Table->BaseDllName));
		KdPrint(("Driver Count: %d", *dwcount));
	}
	__except (1) {
		KdPrint(("Get Module Num exception !"));
	}

	ObDereferenceObject(driverObject);
	return status;
}


NTSTATUS GetModuleBase(_In_ WDFREQUEST Request,
	_In_ size_t InputBufferLength,
	_In_ size_t OutputBufferLength)
{
	PEXCEPTION_POINTERS Ep = NULL;
	DWORD Error_Code = 0;
	if (!OutputBufferLength || !InputBufferLength)
	{
		return STATUS_INVALID_PARAMETER;
	}
	NTSTATUS status = STATUS_SUCCESS;
	PDRIVER_MODULE InputBuffer = NULL;//DeviceIoControl的输入buffer
	size_t InBufferlength = 0;
	status = WdfRequestRetrieveInputBuffer(Request, sizeof(DRIVER_MODULE), &InputBuffer, &InBufferlength);
	if (!NT_SUCCESS(status))
	{
		return STATUS_UNSUCCESSFUL;
	}

	LDR_DATA_TABLE_ENTRY *LDR_Table = NULL;
	__try
	{
		if (InputBuffer->address.Type == EMPTY)
		{
			UNICODE_STRING DriverName;
			RtlInitUnicodeString(&DriverName, L"\\Driver\\SystemTest");
			PDRIVER_OBJECT  driverObject;
			status = ObReferenceObjectByName(
				&DriverName,
				OBJ_CASE_INSENSITIVE,//不区分大小写 驱动名可以写大写或小写   
				NULL,  // 结构比较复杂设计对象特性的东西access state  
				0,       // 访问权限可以写0 写0完全访问不受控制FILE_ALL_ACCESS,  
				*IoDriverObjectType,//对象类型 注意加*号   
				KernelMode,//内核模式 有三种模式 enum 类型    
				NULL,                 //不知道 parse context  parse context  
				(PVOID*)&driverObject);//输出对象 我们要得到的驱动对象    
			if (!NT_SUCCESS(status))
			{
				return status;
			}
			LDR_Table = (LDR_DATA_TABLE_ENTRY*)driverObject->DriverSection;

			ObDereferenceObject(driverObject);
		}
		else if (InputBuffer->address.Type == DRIVEROBJ)
		{
			PDRIVER_OBJECT  driverObject = (PDRIVER_OBJECT)InputBuffer->address.Driver_Object_Address;
			if (driverObject->Size != 0)
			{
				LDR_Table = (LDR_DATA_TABLE_ENTRY*)driverObject->DriverSection;
				LIST_ENTRY*     ActiveProcessLinks = NULL;
				ActiveProcessLinks = LDR_Table->InLoadOrderLinks.Flink;
				LDR_Table = (LDR_DATA_TABLE_ENTRY*)ActiveProcessLinks;
			}
			else
			{
				LDR_Table = (LDR_DATA_TABLE_ENTRY*)InputBuffer->address.LDR_Table_Address;
			}
		}
		else
		{
			LDR_Table = (LDR_DATA_TABLE_ENTRY*)InputBuffer->address.LDR_Table_Address;
		}
		PDRIVER_MODULE OutputBuffer = NULL;//DeviceIoControl的输出buffer
		size_t OutputBufferlength = 0;
		status = WdfRequestRetrieveOutputBuffer(Request, sizeof(DRIVER_MODULE), &OutputBuffer, &OutputBufferlength);
		if (!NT_SUCCESS(status))
		{
			return STATUS_UNSUCCESSFUL;
		}
		RtlZeroMemory(OutputBuffer, OutputBufferlength);
		status = STATUS_SUCCESS;


		__try {
			ULONG offset = UFIELD_OFFSET(DRIVER_OBJECT, DriverSection);
			OutputBuffer->SizeOfImage = LDR_Table->SizeOfImage;
			OutputBuffer->Driver_Entry = (DWORD64)LDR_Table->EntryPoint;
			OutputBuffer->Driver_DllBase = (DWORD64)LDR_Table->DllBase;
			if (LDR_Table->FullDllName.Length != 0)
			{
				RtlCopyMemory(OutputBuffer->FullDllName, LDR_Table->FullDllName.Buffer, LDR_Table->FullDllName.Length);
				UNICODE_STRING DriverName = { 0 };
				DriverName.Buffer = ExAllocatePool(PagedPool, 0xff);
				RtlZeroMemory(DriverName.Buffer, 0xFF);
				DriverName.MaximumLength = 0xFF;
				UNICODE_STRING DriverHead;
				RtlInitUnicodeString(&DriverHead, L"\\Driver\\");
				RtlCopyUnicodeString(&DriverName, &DriverHead);
				RtlCopyMemory((PVOID64)((DWORD64)DriverName.Buffer + DriverHead.Length), LDR_Table->BaseDllName.Buffer, LDR_Table->BaseDllName.Length - 8);
				DriverName.Length += (LDR_Table->BaseDllName.Length - 8);
				PDRIVER_OBJECT  driverObject;
				status = ObReferenceObjectByName(
					&DriverName,
					OBJ_CASE_INSENSITIVE,//不区分大小写 驱动名可以写大写或小写   
					NULL,  // 结构比较复杂设计对象特性的东西access state  
					0,       // 访问权限可以写0 写0完全访问不受控制FILE_ALL_ACCESS,  
					*IoDriverObjectType,//对象类型 注意加*号   
					KernelMode,//内核模式 有三种模式 enum 类型    
					NULL,                 //不知道 parse context  parse context  
					(PVOID*)&driverObject);//输出对象 我们要得到的驱动对象    
				if (NT_SUCCESS(status) && driverObject->DriverSection)
				{
					OutputBuffer->address.Type = DRIVEROBJ;
					OutputBuffer->address.Driver_Object_Address = (DWORD64)driverObject;
					if (driverObject->DriverExtension->ServiceKeyName.Length != 0)
					{
						RtlCopyMemory(OutputBuffer->ServiceName, driverObject->DriverExtension->ServiceKeyName.Buffer, driverObject->DriverExtension->ServiceKeyName.Length);
					}
					ObDereferenceObject(driverObject);
				}
				else
				{
					KdPrint(("ObReferenceObjectByName Error Driver: %wZ", LDR_Table->BaseDllName));
					KdPrint(("ObReferenceObjectByName Error Code: %#X", status));
					LIST_ENTRY*     ActiveProcessLinks = LDR_Table->InLoadOrderLinks.Flink;
					LDR_Table = (LDR_DATA_TABLE_ENTRY*)ActiveProcessLinks;
					OutputBuffer->address.Type = LDR_TABLE;
					OutputBuffer->address.LDR_Table_Address = (DWORD64)LDR_Table;
					status = STATUS_SUCCESS;
				}
				RtlFreeUnicodeString(&DriverName);
			}
			else
			{
				LIST_ENTRY*     ActiveProcessLinks = LDR_Table->InLoadOrderLinks.Flink;
				LDR_Table = (LDR_DATA_TABLE_ENTRY*)ActiveProcessLinks;
				OutputBuffer->address.Type = LDR_TABLE;
				OutputBuffer->address.LDR_Table_Address = (DWORD64)LDR_Table;
			}
			// 		KdPrint(("Driver Object Address: %#X", (DWORD64)LDR_Table));
			// 		KdPrint(("Driver Full Name: %wZ", &LDR_Table->FullDllName));
			// 		KdPrint(("Driver Size: %#X", LDR_Table->SizeOfImage));
			// 		KdPrint(("Driver DLL Base: %#X", (DWORD64)LDR_Table->DllBase));
			// 		KdPrint(("Driver Entry: %#X", (DWORD64)LDR_Table->EntryPoint));
		}
		__except (Ep = GetExceptionInformation(), Error_Code = GetExceptionCode()) {
			KdPrint(("Get Module Info exception !"));
			status = STATUS_UNSUCCESSFUL;
		}
	}
	__except (Ep = GetExceptionInformation(), Error_Code = GetExceptionCode())
	{
		KdPrint(("Get Module Base exception"));
		status = STATUS_UNSUCCESSFUL;
	}
	return status;
}
