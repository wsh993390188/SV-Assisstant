#include "../Driver/Driver.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, OpenFileSysDir)
#endif

/************************************************************************/
/*查找文件过滤系统
*/
/************************************************************************/
NTSTATUS FindDevice(UNICODE_STRING FileSys, UNICODE_STRING ObjectType, PFILTERSYS_INFO PFilterInfo, ULONG Count)
{
	NTSTATUS Status;
	UNICODE_STRING DriverName1, DriverName2;
	PDRIVER_OBJECT DriverObject = NULL;
	PDEVICE_OBJECT AttachObject = NULL;
	WCHAR DriNa1[64] = { 0 };
	WCHAR DriNa2[64] = { 0 };

	PLDR_DATA_TABLE_ENTRY Ldr = NULL;
	wcscpy(DriNa1, L"\\Driver\\");

	wcscat(DriNa1, FileSys.Buffer);
	RtlInitUnicodeString(&DriverName1, DriNa1);

	Status = ObReferenceObjectByName(
		&DriverName1,
		OBJ_CASE_INSENSITIVE,//不区分大小写 驱动名可以写大写或小写   
		NULL,  // 结构比较复杂设计对象特性的东西access state  
		FILE_ALL_ACCESS,       // 访问权限可以写0 写0完全访问不受控制FILE_ALL_ACCESS,  
		*IoDriverObjectType,//对象类型 注意加*号   
		KernelMode,//内核模式 有三种模式 enum 类型    
		NULL,                 //不知道 parse context  parse context  
		(PVOID*)&DriverObject);//输出对象 我们要得到的驱动对象    

	if (NT_SUCCESS(Status))
	{
		if (DriverObject == NULL)
		{
			return STATUS_UNSUCCESSFUL;
		}
		__try
		{
			AttachObject = DriverObject->DeviceObject->AttachedDevice;

			if (AttachObject == NULL)
			{
				return STATUS_UNSUCCESSFUL;
			}
			KdPrint(("要查找的设备是:%wZ,类型:%wZ,0x%llX\n", FileSys, ObjectType, AttachObject));
			KdPrint(("ATTACHNAME :%wZ\n", AttachObject->DriverObject->DriverName));
			for (ULONG i = 0; i<Count; i++)
			{
				if (PFilterInfo[i].AttachDevice == 0)
				{
					PFilterInfo[i].ObjectType = AttachObject->DeviceType;
					PFilterInfo[i].AttachDevice = (DWORD64)AttachObject;

					RtlCopyMemory(PFilterInfo[i].SysName, AttachObject->DriverObject->DriverName.Buffer, AttachObject->DriverObject->DriverName.Length);
					RtlCopyMemory(PFilterInfo[i].HostSysName, DriverObject->DriverName.Buffer, DriverObject->DriverName.Length);
					Ldr = (PLDR_DATA_TABLE_ENTRY)(AttachObject->DriverObject->DriverSection);
					if (Ldr != NULL && Ldr->FullDllName.Buffer != NULL)
					{
						RtlCopyMemory(PFilterInfo[i].SysPath, Ldr->FullDllName.Buffer, Ldr->FullDllName.Length);
					}
					else
					{
						RtlCopyMemory(PFilterInfo[i].SysPath, AttachObject->DriverObject->DriverName.Buffer, AttachObject->DriverObject->DriverName.Length);
					}
					if (i == Count - 1)
					{
						return STATUS_BUFFER_TOO_SMALL;
					}
					return STATUS_SUCCESS;

				}
				if (i == Count - 1)
				{
					return STATUS_BUFFER_TOO_SMALL;
				}
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{

		}
		return STATUS_SUCCESS;
	}
	else
	{
		wcscpy(DriNa2, L"\\FileSystem\\");

		wcscat(DriNa2, FileSys.Buffer);
		RtlInitUnicodeString(&DriverName2, DriNa2);

		Status = ObReferenceObjectByName(
			&DriverName2,
			OBJ_CASE_INSENSITIVE,//不区分大小写 驱动名可以写大写或小写   
			NULL,  // 结构比较复杂设计对象特性的东西access state  
			FILE_ALL_ACCESS,       // 访问权限可以写0 写0完全访问不受控制FILE_ALL_ACCESS,  
			*IoDriverObjectType,//对象类型 注意加*号   
			KernelMode,//内核模式 有三种模式 enum 类型    
			NULL,                 //不知道 parse context  parse context  
			(PVOID*)&DriverObject);//输出对象 我们要得到的驱动对象    

		if (NT_SUCCESS(Status))
		{
			if (DriverObject == NULL)
			{
				return STATUS_UNSUCCESSFUL;
			}
			__try
			{
				AttachObject = DriverObject->DeviceObject->AttachedDevice;
				if (AttachObject == NULL)
				{
					return STATUS_UNSUCCESSFUL;
				}
				KdPrint(("要查找的设备是:%wZ,类型:%wZ,0x%llX\n", FileSys, ObjectType, AttachObject));
				KdPrint(("ATTACHNAME :%wZ\n", AttachObject->DriverObject->DriverName));
				for (ULONG i = 0; i<Count; i++)
				{
					if (PFilterInfo[i].AttachDevice == 0)
					{

						PFilterInfo[i].ObjectType = AttachObject->DeviceType;
						PFilterInfo[i].AttachDevice = (DWORD64)AttachObject;

						RtlCopyMemory(PFilterInfo[i].SysName, AttachObject->DriverObject->DriverName.Buffer, AttachObject->DriverObject->DriverName.Length);
						RtlCopyMemory(PFilterInfo[i].HostSysName, DriverObject->DriverName.Buffer, DriverObject->DriverName.Length);
						Ldr = (PLDR_DATA_TABLE_ENTRY)(AttachObject->DriverObject->DriverSection);
						if (Ldr != NULL && Ldr->FullDllName.Buffer != NULL)
						{
							RtlCopyMemory(PFilterInfo[i].SysPath, Ldr->FullDllName.Buffer, Ldr->FullDllName.Length);
						}
						else
						{
							RtlCopyMemory(PFilterInfo[i].SysPath, AttachObject->DriverObject->DriverName.Buffer, AttachObject->DriverObject->DriverName.Length);
						}
						if (i == Count - 1)
						{
							return STATUS_BUFFER_TOO_SMALL;
						}
						return STATUS_SUCCESS;

					}
					if (i == Count - 1)
					{
						return STATUS_BUFFER_TOO_SMALL;
					}
				}

			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{

			}
			return STATUS_SUCCESS;
		}
		else
		{
			return Status;
		}
	}
	return STATUS_UNSUCCESSFUL;
}

NTSTATUS OpenFileSysDir(UNICODE_STRING DriPath, PFILTERSYS_INFO PFilterInfo, ULONG Count)
{
	PAGED_CODE();

	OBJECT_ATTRIBUTES oa;
	NTSTATUS Status;
	HANDLE hDriver;
	ULONG Length = 0x8000;
	ULONG                context, dwRet, count = 0;
	PDIRECTORY_BASIC_INFORMATION pBuffer = NULL, pB2;

	InitializeObjectAttributes(&oa, &DriPath, OBJ_CASE_INSENSITIVE, NULL, NULL);
	Status = ZwOpenDirectoryObject(&hDriver, DIRECTORY_QUERY, &oa);
	if (NT_SUCCESS(Status))
	{
	next:
		pBuffer = (PDIRECTORY_BASIC_INFORMATION)ExAllocatePoolWithTag(PagedPool, Length, 'so');

		if (pBuffer != NULL)	
		{
			Status = ZwQueryDirectoryObject(hDriver, pBuffer, Length, FALSE, TRUE, &context, &dwRet);
			if (NT_SUCCESS(Status))
			{
				pB2 = pBuffer;
				while (pB2->ObjectName.Length != 0 && pB2->ObjectTypeName.Length != 0)
				{
					Status = FindDevice(pB2->ObjectName, pB2->ObjectTypeName, PFilterInfo, Count);
					if (Status == STATUS_BUFFER_TOO_SMALL)
					{
						return STATUS_BUFFER_TOO_SMALL;
					}
					pB2++;
					count++;
				}
				if (pBuffer)
				{
					ExFreePoolWithTag(pBuffer, 'so');
				}
				if (hDriver)
				{
					ZwClose(hDriver);
				}
				return STATUS_SUCCESS;
			}
			else if (Status == STATUS_BUFFER_TOO_SMALL || Status == STATUS_MORE_ENTRIES)
			{
				ExFreePoolWithTag(pBuffer, 'so');
				Length *= 2;
				goto next;
			}
			else {
				ExFreePoolWithTag(pBuffer, 'so');
				if (hDriver)
				{
					ZwClose(hDriver);
				}
				return STATUS_UNSUCCESSFUL;
			}
		}
		else
		{
			if (hDriver)
			{
				ZwClose(hDriver);
			}
			return STATUS_UNSUCCESSFUL;
		}
	}
	else
	{
		KdPrint(("ZwOpenDirectoryObject failed,status 0x%llX\n", Status));
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS EnumFilterDriverFunc(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength)
{
	if (!OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING DirPath1;
	PFILTERSYS_INFO POUTfilterInfo;
	POUTfilterInfo = ExAllocatePool(PagedPool, 30 * sizeof(FILTERSYS_INFO));
	RtlZeroMemory(POUTfilterInfo, 30 * sizeof(FILTERSYS_INFO));
	RtlInitUnicodeString(&DirPath1, L"\\FileSystem");
	status = OpenFileSysDir(DirPath1, POUTfilterInfo, 30);
	if (STATUS_BUFFER_TOO_SMALL == status)
	{
		KdPrint(("分配的内存太小\n"));
		goto END;
	}

	UNICODE_STRING DirPath2;
	RtlInitUnicodeString(&DirPath2, L"\\Driver");
	status = OpenFileSysDir(DirPath2, POUTfilterInfo, 30);
	if (STATUS_BUFFER_TOO_SMALL == status)
	{
		KdPrint(("分配的内存太小\n"));
		goto END;
	}

	size_t OutputBufferlength = 0;
	PFILTERSYS_INFO buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, 30 * sizeof(FILTERSYS_INFO), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
		return status;

	RtlZeroMemory(buffer, OutputBufferlength);
	PFILTERSYS_INFO temp = POUTfilterInfo;
	for (int i = 30; i > 0; i--, temp++)
	{
		if (temp != NULL && temp->AttachDevice != 0)
		{
			KdPrint(("%dth Driver\n", i));
			buffer->ObjectType = temp->ObjectType;
			KdPrint(("SysName: %S\n", temp->SysName));
			RtlCopyMemory(buffer->SysName, temp->SysName, 64);
			KdPrint(("SysPath: %S\n", temp->SysPath));
			RtlCopyMemory(buffer->SysPath, temp->SysPath, 260);
			KdPrint(("Host Sys Name: %S\n", temp->HostSysName));
			RtlCopyMemory(buffer->HostSysName, temp->HostSysName, 64);
			KdPrint(("Attach Device Name: 0x%llX\n", temp->AttachDevice));
			buffer->AttachDevice = temp->AttachDevice;
			buffer++;
		}
	}
END:
	ExFreePool(POUTfilterInfo);
	return status;
}