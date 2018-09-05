#include "DDR3.h"

_DDR3::_DDR3()
{
	for (USHORT i = 0; i < DIMMSlotNum; ++i)
	{
		DDR3_info[i] = new DDR3_INFO;
		memset(DDR3_info[i], 0, sizeof(DDR3_INFO));
	}
}

_DDR3::~_DDR3()
{
	for (USHORT i = 0; i < DIMMSlotNum; ++i)
	{
		delete DDR3_info[i];
	}
	//delete DDR3_info;
}

BOOL _DDR3::Get_DDR3_SPD()
{
	USHORT SPD_Base_ADDR = 0;
	switch (VendorID)
	{
	case 0x1106:
		SPD_Base_ADDR = 0x400;
		break;
	case 0x8086:
		break;
	default:
		break;
	}
	if (SPD_Base_ADDR != 0)
	{
		for (USHORT i = 0; i < DIMMSlotNum; ++i)
		{
			PDDR3_INFO temp = DDR3_info[i];
			for (USHORT j = 0; j < 0x100; ++j)
			{
				this->ReadSPD(SPD_Base_ADDR, DIMMADDR[i], j);
				memcpy_s(temp + j, sizeof(DDR3_INFO), &Data, 1);
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
