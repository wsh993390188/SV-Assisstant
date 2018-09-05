#pragma once

EXTERN_C_START

NTSTATUS GetAllModuleNum(DWORD *dwcount);

NTSTATUS GetModuleBase(_In_ WDFREQUEST Request,
	_In_ size_t InputBufferLength,
	_In_ size_t OutputBufferLength);

EXTERN_C_END