#pragma once

#include <intrin.h>
EXTERN_C_START
NTSTATUS FindDevice(UNICODE_STRING FileSys, UNICODE_STRING ObjectType, PFILTERSYS_INFO PFilterInfo, ULONG Count);
NTSTATUS OpenFileSysDir(UNICODE_STRING DriPath, PFILTERSYS_INFO PFilterInfo, ULONG Count);
NTSTATUS EnumFilterDriverFunc(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength);
EXTERN_C_END