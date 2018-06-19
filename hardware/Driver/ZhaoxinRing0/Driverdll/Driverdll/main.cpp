//#include "stdafx.h"
#include <math.h>
//#include "AMD.h"
//#include "MyDriverClass.h"
#include "Zhaoxin.h"
#include "PCI\PCI-E.h"

// #define MAX_DEVPATH_LENGTH 256
// #define SMBUS_BASE 0x400
// #define HST_STS 0x400
// #define HST_CNT 0x402
// #define HST_CMD 0x403
// #define XMIT_SLVA 0x404

int main()
{
// 	ULONGLONG  PCIEBASEAddress = 0;;
// 	GetPCIEBaseAddress(PCIEBASEAddress);
// 	GetPCIEConfigSpace(PCIEBASEAddress);
//	string tmp = _T("");
//	openpci(_T(".\\PCI\\pciids"), ios::in, 0x8086, tmp);
	//Get_IDE_Identify();
//  	CRing0 ring0;
//  	ring0.ReadSPD(0x400, 0xA0,0);
// 	ring0.ReadITE();
	//_DDR3 ddr3;
	//ddr3.Get_DDR3_SPD();
	/*ULONG MSR[] = {
		0x0000001B,
		0xC0010114,
		0xC0010058,		
		0xC0010293,
		0xC0010063,
		0xC0010064,
		0xC0010065,
		0xC0010066,
		0xC0010067,
		0xC0010068,
		0xC0010069,
		0xC001006A,
		0xC001006B,
		0xC0010299,
		0xC001029B,
		0xC001029A,
		0xC0010293,
	};

	ULONG MSR[] = {
		0xC0010061,
		0xC0010062,
		0xC0010063,
		0xC0010064,
		0xC0010065,
		0xC0010066,
		0xC0010067,
		0xC0010068,
		0xC0010069,
		0xC001006A,
		0xC001006B,
		0xC0010070,
		0xC0010071,
		0xC0010072
	};*/
	/*ULONG Data = 0;
	ring0.RdMemory(0xFFFF,4,Data);
	cout << hex << setfill('0') << setw(8) << Data << endl;
	ring0.WrMemory(0xFFFF,4,0x23456);
	ring0.RdMemory(0xFFFF,4,Data);
	cout << hex << setfill('0') << setw(8) << Data << endl;
	ring0.RdIOPort(0x80, 1, Data1);
	cout << hex << setfill('0') << setw(2) << Data1 << endl;
	ring0.WrIOPort(0x80, 1, 0xA1);
	ring0.RdIOPort(0x80, 1, Data1);
	cout << hex << setfill('0') << setw(2) << Data1 << endl;

	PCI_COMMON_CONFIG Pci;
	memset(&Pci, 0xff, sizeof(Pci));
	ring0.ReadPci(0,0,0, Pci);
	
	ring0.ReadAllPci();
	*/
	//ITE_Chip();

	/*ULONG CpuVID;
	double CpuFID;
	if (Family17(0, 0, AMD_DEF_PSTATE0, CpuFID, CpuVID, 1))
		cout << "FID:" << CpuFID << " VID:" << CpuVID << endl;
	if (Family17(0, 0, AMD_DEF_PSTATE1, CpuFID, CpuVID, 1))
		cout << "FID:" << CpuFID << " VID:" << CpuVID << endl;
	if (Family17(0, 0, AMD_DEF_PSTATE2, CpuFID, CpuVID, 1))
		cout << "FID:" << CpuFID << " VID:" << CpuVID << endl;
	if (Family17(0, 0, AMD_DEF_PSTATE3, CpuFID, CpuVID, 1))
		cout << "FID:" << CpuFID << " VID:" << CpuVID << endl;
	if (Family17(0, 0, AMD_DEF_PSTATE4, CpuFID, CpuVID, 1))
		cout << "FID:" << CpuFID << " VID:" << CpuVID << endl;
	if (Family17(0, 0, AMD_DEF_PSTATE5, CpuFID, CpuVID, 1))
		cout << "FID:" << CpuFID << " VID:" << CpuVID << endl;
	if (Family17(0, 0, AMD_DEF_PSTATE6, CpuFID, CpuVID, 1))
		cout << "FID:" << CpuFID << " VID:" << CpuVID << endl;

	ring0.RdMsr(0xC0010042, Data1, 1);
	cout << hex << setfill('0') << setw(16) << Data1 << endl;

	ring0.RdMsr(0xC0010061, Data1, 1);
	cout << hex << setfill('0') << setw(16) << Data1 << endl;
	ring0.RdMsr(0xC0010062, Data1, 1);
	cout << hex << setfill('0') << setw(16) << Data1 << endl;
	ring0.RdMsr(0xC0010063, Data1, 1);
	cout << hex << setfill('0') << setw(16) << Data1 << endl;
	ring0.RdMsr(0xC0000104, Data1, 1);
	cout << "TSC Radio" << hex << setfill('0') << setw(16) << Data1 << endl;
	ring0.RdMsr(0xC0010015, Data1, 1);
	cout << "HWCR:" << hex << setfill('0') << setw(16) << Data1 << endl;
	cout << "TSC:" << endl;
	ring0.RdMsr(0x10, Data1, 1);
	cout << hex << setfill('0') << setw(16) << Data1 << endl;
	ring0.RdMsr(0x10, Data1, 1);
	cout << hex << setfill('0') << setw(16) << Data1 << endl;
	//ring0.ReadSPD(0x400,0xA0,0);
	DWORD64 APERF = 0, MPERF = 0,APERF_Init = 0, MPERF_Init = 0;
	ring0.RdMsr(0xE8, APERF, 1);
	cout << "APERF:" << hex << setfill('0') << setw(16) << APERF << endl;
	ring0.RdMsr(0xE7, MPERF, 1);
	cout << "MPERF:" << hex << setfill('0') << setw(16) << MPERF << endl;
	APERF_Init = APERF;
	MPERF_Init = MPERF;
	Sleep(1);

	ring0.RdMsr(0xE8, APERF, 1);
	cout << "APERF:" << hex << setfill('0') << setw(16) << APERF << endl;
	ring0.RdMsr(0xE7, MPERF, 1);
	cout << "MPERF:" << hex << setfill('0') << setw(16) << MPERF << endl;
	double results = ((APERF - APERF_Init * 1.0) / (MPERF - MPERF_Init * 1.0));
	cout << "Maybe FID:" << results * CpuFID << endl;
	//cout << "FID:" << results * 100 << endl;
	ring0.RdMsr(0xC00000E7, Data1, 1);
	cout << hex << setfill('0') << setw(16) << Data1 << endl;
	ring0.RdMsr(0x198, Data1, 1);
	cout << hex << setfill('0') << setw(16) << Data1 << endl;
	ring0.RdMsr(0xC0010042, Data1, 1);
	cout << "TSC_AUX Radio" << hex << setfill('0') << setw(16) << Data1 << endl;

	ring0.RdMsr(0xC0010293, Data1, 1);
	cout << "TSC Radio" << hex << setfill('0') << setw(16) << Data1 << endl;
	
	while (1)
	{
		DWORD64 Data_Pre;
		/*for (int i = 0; i < (sizeof(MSR) / sizeof(ULONG)); ++i)
		{
			ring0.RdMsr(MSR[i], Data1, 1);
			cout << "MSR " << hex << setfill('0') << setw(8) <<MSR[i]<<":" << hex << setfill('0') << setw(16) << Data1 << endl;

		}
		
		system("cls");
		for (USHORT i = 0; i < 4; ++i)
		{
			cout << "Core " << i << ": " << endl;
			if (Family17(0, 0, 0xC0010293, CpuFID, CpuVID, 1i64 << i))
				cout << dec << "FID:" << CpuFID << " VID:" << CpuVID << endl;
			ring0.RdMsr(0x10, Data1, 1i64 << i);
			Data_Pre = Data1;
			
			ring0.RdMsr(0x10, Data1, 1i64 << i);
			cout << "TSC:" << 1.0 * (Data1 * 1.0 - Data_Pre * 1.0) / 1024 / 1024 << "MHZ" << endl;
			if (i == 3) cout << endl;
		}
Sleep(990);
	}*/
DWORD64 Data1 = 0;
ZhaoxinDriver::RdIOPort(0x80, 1, Data1);
	return 0;
}
