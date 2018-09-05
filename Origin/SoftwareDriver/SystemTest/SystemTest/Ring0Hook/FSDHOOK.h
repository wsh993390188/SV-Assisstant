#pragma once

EXTERN_C_START
NTSTATUS EnumFSDAddress(_In_ WDFREQUEST Request,
	_In_ size_t InputBufferLength,
	_In_ size_t OutputBufferLength, 
	size_t* Resquestlength);

EXTERN_C_END