#pragma once

EXTERN_C_START

extern const char* const IPR_FUNC_NAME[];
NTSTATUS EnumI8042PrtAddress(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength, size_t* Resquestlength);

EXTERN_C_END