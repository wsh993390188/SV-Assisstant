#pragma once

NTSTATUS EnumAllACPITable(_In_ WDFREQUEST Request,
	_In_ size_t InputBufferLength,
	_In_ size_t OutputBufferLength);