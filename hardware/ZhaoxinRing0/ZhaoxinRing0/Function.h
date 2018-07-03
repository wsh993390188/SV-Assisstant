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

#define MK_PCIADDR(bus, dev, func, off) (ULONG)(0x80000000L|((bus & 0xFF) << 16) | (dev & 0x1F) << 11 | (func & 0x07) << 8 | off)

//extern void Memory_Map(PULONG mmmp_entry, PULONG mmmp_count);


/*************************************************
Function:       // ReadIoPort
Description:    // 读取IO口的数据
Calls:          // NULL
Input:          
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS
ReadIoPort(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // WriteIoPort
Description:    // 将数据写入IO口
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS
WriteIoPort(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // ReadMsr
Description:    // 读取MSR的数据
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS
ReadMsr(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // WriteMsr
Description:    // 写入MSR的数据
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS
WriteMsr(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // ReadMsrThread
Description:    // 读取MSR的数据
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
	Others:         // NULL
*************************************************/

NTSTATUS
ReadMsrThread(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // WriteMsrThread
Description:    // 写入MSR的数据
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
	Others:         // NULL
*************************************************/

NTSTATUS
WriteMsrThread(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);


/*************************************************
Function:       // ReadPmc
Description:    // 读取PMC的数据
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS
ReadPmc(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // ReadMemory
Description:    // 读取内存地址中的数据
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS
ReadMemory(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // WriteMemory
Description:    // 写入内存地址的数据
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS
WriteMemory(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // ReadPciConfig
Description:    // 读取PCI配置空间的数据
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS
ReadPciConfig(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // WritePciConfig
Description:    // 写入PCI配置空间的的数据
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS
WritePciConfig(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // ReadSPDBYTE
Description:    // 读取SPD的数据
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS
ReadSPDBYTE(
	_In_ WDFREQUEST Request,
	_In_ size_t OutputBufferLength,
	_In_ size_t InputBufferLength
);

/*************************************************
Function:       // SMBUS_Read_Byte
Description:    // 按字节读取SPD中的数据
Calls:          // ReadSPDBYTE
Input:
Request 
	Base_Address SMBUS的地址
	Slave_Address 从设备中的地址
	Offset 读取数据的偏移量
Output:         
	Data 按字节输出读取的数据
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS __stdcall SMBUS_Read_Byte(size_t Base_Address, size_t Slave_Address, size_t Offset, OUT PUCHAR Data);

/*************************************************
Function:       // ReadITEDisk
Description:    // 读取ITE硬盘的Identify
Calls:          // NULL
Input:
	Request IRP请求对象
	InputBufferLength 输出数据的长度
	OutputBufferLength 输入数据的长度
Output:         // NULL。
Return:         // 函数返回值的说明
Others:         // NULL
*************************************************/

NTSTATUS ReadITEDisk(
	WDFREQUEST Request,
	size_t OutputBufferLength,
	size_t InputBufferLength
);
#endif