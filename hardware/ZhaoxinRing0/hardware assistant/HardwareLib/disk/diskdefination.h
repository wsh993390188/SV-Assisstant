#pragma once

#pragma pack(push,1)
typedef	struct _SMART_ATTRIBUTE
{
	BYTE	Id;
	WORD	StatusFlags;
	BYTE	CurrentValue;
	BYTE	WorstValue;
	BYTE	RawValue[6];
	BYTE	Reserved;
} SMART_ATTRIBUTE;

typedef	struct _SMART_THRESHOLD
{
	BYTE	Id;
	BYTE	ThresholdValue;
	BYTE	Reserved[10];
} SMART_THRESHOLD;
#pragma pack(pop)