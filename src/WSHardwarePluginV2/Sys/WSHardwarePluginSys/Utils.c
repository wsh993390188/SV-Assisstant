#include "driver.h"

PUNICODE_STRING GetProcessPath(PEPROCESS Process)
{
	PFILE_OBJECT FileObject = NULL;
	POBJECT_NAME_INFORMATION ObjectNameInfo = NULL;

	if (NULL == Process)
	{
		return NULL;
	}

	if (!NT_SUCCESS(PsReferenceProcessFilePointer(Process, &FileObject)))
	{
		return NULL;
	}

	if (!NT_SUCCESS(IoQueryFileDosDeviceName(FileObject, &ObjectNameInfo)))
	{
		ObDereferenceObject(FileObject);
		return NULL;
	}

	ObDereferenceObject(FileObject);

	return (PUNICODE_STRING)ObjectNameInfo;
}

PUCHAR ReadFile(PUNICODE_STRING FilePath, PULONG FileSize)
{
	NTSTATUS Status = STATUS_SUCCESS;
	HANDLE FileHandle = NULL;
	OBJECT_ATTRIBUTES ObjectAttributes = { 0 };
	IO_STATUS_BLOCK StatusBlock = { 0 };
	FILE_STANDARD_INFORMATION FileStandardInfo = { 0 };
	PUCHAR FileBuffer = NULL;

	do
	{
		InitializeObjectAttributes(&ObjectAttributes, FilePath, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

		Status = ZwCreateFile(&FileHandle, FILE_GENERIC_READ, &ObjectAttributes,
			&StatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN,
			FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		Status = ZwQueryInformationFile(FileHandle, &StatusBlock, &FileStandardInfo, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		*FileSize = (ULONG)FileStandardInfo.EndOfFile.QuadPart;

		FileBuffer = ExAllocatePool(NonPagedPool, *FileSize);

		if (!FileBuffer)
		{
			Status = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}

		Status = ZwReadFile(FileHandle, NULL, NULL, NULL, &StatusBlock, FileBuffer, *FileSize, NULL, NULL);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		if (StatusBlock.Information != *FileSize)
		{
			Status = STATUS_INTERNAL_ERROR;
			break;
		}
	} while (FALSE);

	if (FileHandle)
	{
		ZwClose(FileHandle);
	}

	if (!NT_SUCCESS(Status))
	{
		if (FileBuffer)
		{
			ExFreePool(FileBuffer);
			FileBuffer = NULL;
		}
	}

	return FileBuffer;
}