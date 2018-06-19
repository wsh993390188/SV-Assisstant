#pragma once
#pragma pack(push)
#pragma pack(1)  
//
// IDT Entry Definition
//
typedef union _KIDTENTRY64
{
	struct
	{
		USHORT OffsetLow;
		USHORT Selector;
		USHORT IstIndex : 3;
		USHORT Reserved0 : 5;
		USHORT Type : 5;
		USHORT Dpl : 2;
		USHORT Present : 1;
		USHORT OffsetMiddle;
		ULONG OffsetHigh;
		ULONG Reserved1;
	};
	UINT64 Alignment;
} KIDTENTRY64, *PKIDTENTRY64;

#pragma pack(pop)

NTSTATUS EnumIDTableAddress(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength, size_t* Resquestlength);

NTSTATUS Show_Idt(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength, size_t* Resquestlength, int num);
