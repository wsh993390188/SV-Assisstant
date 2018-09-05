#pragma once

typedef struct _KTIMER_TABLE_ENTRY
{
	ULONG_PTR   Lock;
	LIST_ENTRY  Entry;
	ULONG_PTR   Time;
}KTIMER_TABLE_ENTRY, *PKTIMER_TABLE_ENTRY;

#define KTIMER_TABLE_ENTRY_MAX (256)

typedef struct _KTIMER_TABLE
{
	ULONG_PTR           TimerExpiry[64];
	KTIMER_TABLE_ENTRY  TimerEntries[KTIMER_TABLE_ENTRY_MAX];
}KTIMER_TABLE, *PKTIMER_TABLE;

typedef enum _Pcrb_Offset
{
	TimerTable = 0x3600
}Pcrb_Offset;


EXTERN_C_START
DWORD64 KeGetCurrentPrcr();
DWORD64 GetPcrbInfo(DWORD64 dwFlag);
NTSTATUS EnumDPCTimerInfo(WDFREQUEST Request, size_t InputBufferLength, size_t OutputBufferLength);
BOOLEAN GetKiWaitVariableAddress(PULONG64 KiWaitNever, PULONG64 KiWaitAlways);
KDPC* TransTimerDPCEx(PKTIMER Timer, ULONG64 KiWaitNever, ULONG64 KiWaitAlways);
PVOID
GetExportVariableFormNtosExportTableByVariableName(WCHAR *VariableName);
EXTERN_C_END