#pragma once

/*++

Module Name:

Function.h

Abstract:

This file contains the Function definitions.

Environment:

Kernel-mode Driver Framework

--*/

/*************************************************
Copyright:
Author:王硕
Date:2017-10-19
Description:实现对硬件的读写操作
**************************************************/
#ifndef FUNCTION_H
#define FUNCTION_H

#define PMIO_INDEX_REG			0xCD6
#define PMIO_DATA_REG			0xCD7

#define INTEL_PLATFORM  0x8086ul
#define AMD_PLATFORM	0x1022ul
#define ZX_PLATFORM		0x1106ul
#define ATI_PLATFORM	0x1002ul
#define NVI_PLATFORM	0x10DEul

NTSTATUS ExcuteOperator(
	_In_ WDFREQUEST Request,
	_In_ size_t InputBufferLength,
	_In_ size_t OutputBufferLength);

NTSTATUS ReadIo(_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ const Ring0Operator* Operator);

NTSTATUS WriteIo(_In_ const Ring0Operator* Operator);

NTSTATUS ReadMemory(_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ const Ring0Operator* Operator);

NTSTATUS WriteMemory(_In_ const Ring0Operator* Operator);

NTSTATUS ReadMsr(_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ const Ring0Operator* Operator);

NTSTATUS WriteMsr(_In_ const Ring0Operator* Operator);

NTSTATUS ReadPCI(_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ const Ring0Operator* Operator);

NTSTATUS WritePCI(_In_ const Ring0Operator* Operator);

NTSTATUS ReadTSC(_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ const Ring0Operator* Operator);
#endif