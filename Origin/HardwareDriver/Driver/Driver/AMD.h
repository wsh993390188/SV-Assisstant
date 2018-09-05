#pragma once
#include "MyDriverClass.h"

#define AMD_DEF_PSTATE0								0xC0010064
#define AMD_DEF_PSTATE1								0xC0010065
#define AMD_DEF_PSTATE2								0xC0010066
#define AMD_DEF_PSTATE3								0xC0010067
#define AMD_DEF_PSTATE4								0xC0010068
#define AMD_DEF_PSTATE5								0xC0010069
#define AMD_DEF_PSTATE6								0xC001006A
#define AMD_DEF_PSTATE7								0xC001006B
BOOL Family17(USHORT Family, USHORT Model, ULONG Index, double& COF, ULONG& CpuVID, IN DWORD_PTR threadAffinityMask);

