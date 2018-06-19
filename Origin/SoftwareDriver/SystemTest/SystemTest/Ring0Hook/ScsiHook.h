#pragma once

EXTERN_C_START

extern const char* const IPR_FUNC_NAME[];
NTSTATUS EnumScsiAddress(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength, size_t* Resquestlength);

EXTERN_C_END