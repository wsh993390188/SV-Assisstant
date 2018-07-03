#include "Disk IDE.h"

BOOL Get_IDE_Identify()
{
	CRing0 ring0;
	USHORT Sata_Port = 2;
	WORD IDE_Data[256] = { 0 };
	ring0.ReadIDE(Sata_Port, IDE_Data); 
	return TRUE;
}
