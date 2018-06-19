#pragma once

EXTERN_C_START

NTSTATUS EnumSSDTable(_In_ WDFREQUEST Request,
	_In_ size_t InputBufferLength,
	_In_ size_t OutputBufferLength, 
	_Out_ size_t* Resquestlength);

NTSTATUS EnumShadowSSDTable(_In_ WDFREQUEST Request,
	_In_ size_t InputBufferLength,
	_In_ size_t OutputBufferLength, 
	_Out_ size_t* Resquestlength);

EXTERN_C_END