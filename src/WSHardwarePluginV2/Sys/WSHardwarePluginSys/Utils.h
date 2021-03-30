#pragma once

#define MAX_PATH 260

NTSTATUS NTAPI ZwQueryInformationProcess(
	HANDLE ProcessHandle,
	PROCESSINFOCLASS ProcessInformationClass,
	PVOID ProcessInformation,
	ULONG ProcessInformationLength,
	PULONG ReturnLength
);

NTSTATUS PsReferenceProcessFilePointer(IN PEPROCESS pProcess, OUT PFILE_OBJECT* ppFileObject);

PUNICODE_STRING GetProcessPath(PEPROCESS Process);

PUCHAR ReadFile(PUNICODE_STRING FilePath, PULONG FileSize);
