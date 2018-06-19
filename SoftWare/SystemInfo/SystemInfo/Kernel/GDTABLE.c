#include "../Driver/Driver.h"
#pragma intrinsic(_sgdt) 
/*
根据：Table 3-1. Code- and Data-Segment Types，仿照WINDBG的dg命令定义。
*/
const char SegmentTypes[][256] = {
	"<Reserved>",//Data Read-Only缩写是：Data RO，也可认为是： <Reserved>。如果结构（UINT64）全部为零，也可认为是Reserved。
	"Data RO AC",//Data Read-Only, accessed
	"Data RW",//Data Read/Write
	"Data RW AC",//Data Read/Write, accessed
	"Data RO ED",//Data Read-Only, expand-down
	"Data RO ED AC",//Data Read-Only, expand-down, accessed
	"Data RW ED",//Data Read/Write, expand-down
	"Data RW ED AC",//Data Read/Write, expand-down, accessed

	"Code EO",//Code Execute-Only
	"Code EO AC",//Code Execute-Only, accessed
	"Code RE",//Code Execute/Read 加空格以便显示的对齐。
	"Code RE AC",//Code Execute/Read, accessed
	"Code EO CO",//Code Execute-Only, conforming
	"Code EO CO AC",//Code Execute-Only, conforming, accessed
	"Code RE CO",//Code Execute/Read, conforming
	"Code RE CO AC",//Code Execute/Read, conforming, accessed
	"TSS32 Busy ",//这个也可显示只要识别了TSS及内容。
	"TSS32 Avl" //这个在X86上出现了。
};

NTSTATUS EnumGdtTable(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength, _Out_opt_ size_t* Resquestlength)
{
	if (!InputBufferLength || !OutputBufferLength)
		return STATUS_BUFFER_TOO_SMALL;
	NTSTATUS status = STATUS_SUCCESS;

	size_t InputBufferlength = 0;
	PDWORD64 InputBuffer = NULL;
	status = WdfRequestRetrieveInputBuffer(Request, 1, &InputBuffer, &InputBufferlength);
	if (!NT_SUCCESS(status))
		return status;
	size_t i = *InputBuffer;
	if (i >= KeNumberProcessors)
		i = KeNumberProcessors - 1;
	show_gdt(Request, InputBufferLength, OutputBufferLength, Resquestlength, i);

	return STATUS_SUCCESS;
};
NTSTATUS show_gdt(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength, _Out_opt_ size_t* Resquestlength, size_t i)
{
	GDTINFO GdtLimit = { 0 };
	KeSetSystemAffinityThread(i + 1);
	PKGDTENTRY64 pkgdte = KeGetPcr()->GdtBase;//没有__sgdt,也不用sgdt汇编指令的办法。但是这个获取的没有长度。
	_sgdt(&GdtLimit);//一般等于0x7f.
	KeRevertToUserAffinityThread();
	SIZE_T maximun = (GdtLimit.Limit + 1) / sizeof(KGDTENTRY64);

	/*
	显示格式：
	CPU SN Sel        Base              Limit          Type    Pl Size Gran Pres Long Flags
	--- -- ---- ----------------- ----------------- ---------- -- ---- ---- ---- ---- --------

	注释：CPU和SN是自己添加的。SN即Segment Name,如：CS，DS，FS等.
	*/
	KdPrint(("Sel        Base             Limit             Type   DPl Size Gran Pres Long Flags\n"));//CPU SN 
	KdPrint(("---- ---------------- ---------------- ------------- --- ---- ---- ---- ---- --------\n"));//--- -- 
	KdPrint(("\n"));

	NTSTATUS status = STATUS_SUCCESS;
	size_t OutputBufferlength = 0;
	PGPTABLE_STRUCT buffer = NULL;
	status = WdfRequestRetrieveOutputBuffer(Request, (maximun - 1) * sizeof(GPTABLE_STRUCT), &buffer, &OutputBufferlength);
	if (!NT_SUCCESS(status))
	{
		if (STATUS_BUFFER_TOO_SMALL == status)
		{
			status = WdfRequestRetrieveOutputBuffer(Request, sizeof(WORD), &buffer, &OutputBufferlength);
			if (NT_SUCCESS(status))
			{
				*(PDWORD64)buffer = (maximun - 1) * sizeof(GPTABLE_STRUCT);
				*Resquestlength = OutputBufferlength;
				status = STATUS_SUCCESS;
			}
		}
		return status;
	}
	RtlZeroMemory(buffer, OutputBufferlength);
	*Resquestlength = (maximun - 1) * sizeof(GPTABLE_STRUCT);

	for (SIZE_T index = 1; index < maximun; index++)
	{
		PKGDTENTRY64 pkgdte_t = &pkgdte[index];
		SIZE_T Base = 0;
		SIZE_T Limit = 0;
		LONG  Type = 0;
		char * size = NULL;
		char * Granularity = NULL;
		char * Present = NULL;
		char * LongMode = NULL;
		int    Flags = 0;

		Base = pkgdte_t->Bits.BaseHigh;
		Base = (Base << 24);
		Base += (pkgdte_t->BaseLow + (pkgdte_t->Bits.BaseMiddle << 16));

		Limit = pkgdte_t->LimitLow + (pkgdte_t->Bits.LimitHigh << 16);

		if (pkgdte_t->Bits.DefaultBig && Base)
		{
			//扩充高位为1.即F.
			Base += 0xffffffff00000000;
		}

		if (pkgdte_t->Bits.DefaultBig && pkgdte_t->Bits.Granularity)
		{
			//扩充高位为1.即F.
			SIZE_T t = Limit;
			Limit = (Limit << 12);
			Limit += PAGE_SIZE - 1;
		}

		Type = pkgdte_t->Bits.Type;
		_bittestandreset(&Type, 4);//因为这个包含了S位，所以要清除这个位标志。

		if (pkgdte_t->Bits.DefaultBig)
		{
			size = "Bg  ";//Big 加空格是为了对齐显示。
		}
		else
		{
			size = "Nb  ";//Not Big 加空格是为了对齐显示。
		}

		if (pkgdte_t->Bits.Granularity)
		{
			Granularity = "Pg  ";//Page 加空格是为了对齐显示。
		}
		else
		{
			Granularity = "By  ";//Byte 加空格是为了对齐显示。
		}

		if (pkgdte_t->Bits.Present)
		{
			Present = "P   ";//Present 加空格是为了对齐显示。
		}
		else
		{
			Present = "NP  ";//NO Present 加空格是为了对齐显示。
		}

		if (pkgdte_t->Bits.LongMode)
		{
			LongMode = "Lo  ";//Long 加空格是为了对齐显示。
		}
		else
		{
			LongMode = "Nl  ";//NO long 加空格是为了对齐显示。
		}

		Flags = (pkgdte_t->Bytes.Flags2 >> 4);//去掉Segment limit的那几位。
		Flags = Flags << 8;
		Flags = Flags + pkgdte_t->Bytes.Flags1;

		KdPrint(("%04x %p %p %13s %03x %s %s %s %s 0x%04x\n",
			index * 8, //sizeof (KGDTENTRY)
			Base,
			Limit,
			SegmentTypes[Type],
			pkgdte_t->Bits.Dpl,
			size,
			Granularity,
			Present,
			LongMode,
			Flags
			));
		buffer->Base = Base;
		buffer->Flags = Flags;
		buffer->Limit = Limit;
		buffer->Index = index * 8;
		buffer->DPl = pkgdte_t->Bits.Dpl;
		RtlCopyMemory(buffer->SegmentTypes, SegmentTypes[Type], strlen(SegmentTypes[Type]));
		RtlCopyMemory(buffer->Granularity, Granularity, 2);
		RtlCopyMemory(buffer->Present, Present, 2);
		RtlCopyMemory(buffer->LongMode, LongMode, 2);
		RtlCopyMemory(buffer->size, size, 2);
		buffer++;
	}
	return STATUS_SUCCESS;
}