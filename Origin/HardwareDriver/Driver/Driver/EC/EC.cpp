#include "EC.h"

BOOL GetECData(Embedded_Controller_Data & EC_Data)
{
	CRing0 ring0;
	DWORD64 Data = 0;
	for (USHORT i = 0; i < 0x100; i++)
	{
		ring0.RdIOPort(EC_SC, 1, Data);
		if (Data == 0x08)
		{
			ring0.WrIOPort(EC_DATA, 1, 0x00);	// 存在读状态时完成读取状态
		}
		ring0.RdIOPort(EC_SC, 1, Data);
		if (Data)
		{
			OutputDebugString("EC存在问题\n");
			return FALSE;
		}
		ring0.WrIOPort(EC_SC, 1, RD_EC);		// 将EC设置为读状态
		ring0.RdIOPort(EC_SC, 1, Data);
		if (Data != 0x08)
		{
			OutputDebugString("读取EC失败!\n");
			return FALSE;
		}
		ring0.WrIOPort(EC_DATA, 1, i);
		ring0.RdIOPort(EC_DATA, 1, Data);
		memcpy_s(&EC_Data.Data[i], 1, &Data, 1);
	}
	return TRUE;
}

BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data)
{
	CRing0 ring0;
	DWORD64 Data = 0;
	ring0.WrIOPort(EC_SC, 1, WR_EC);	//将EC设置为写状态
	ring0.RdIOPort(EC_SC, 1, Data);
	if (Data != 0x08)		return FALSE;
	ring0.WrIOPort(EC_DATA, 1, EC_Addr);	//	将地址写入EC
	ring0.WrIOPort(EC_DATA, 1, EC_Write_Data);	//将数据写入EC
	return TRUE;
}
