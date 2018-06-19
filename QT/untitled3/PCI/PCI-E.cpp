#include "stdafx.h"
#include "../MyDriverClass.h"
#include "PCI-E.h"

typedef CACPI* (*ACPIExport)(void);
typedef CACPI* (*ACPI_Free)(CACPI*);

BOOL Initialize(HMODULE& hMoudle)
{
	hMoudle = LoadLibrary(_T("ACPI.dll"));
	if (hMoudle == NULL)
	{
		OutputDebugString(_T("Dll未能成功加载"));
		return GetLastError();
	}
	return TRUE;
}

void Free(HMODULE& hMoudle)
{
	if (hMoudle)
	{
		FreeLibrary(hMoudle);
	}
}
BOOL GetPCIEBaseAddress(ULONGLONG& PCIE_BaseAddress)
{
	HMODULE hMoudule;
	if (Initialize(hMoudule) != TRUE)
	{
		return FALSE;
	}
	ACPIExport ACPIGet = (ACPIExport)GetProcAddress(hMoudule, "Instantiate");// 将指针指向函数首地址  
	ACPI_Free ACPIFree = (ACPI_Free)GetProcAddress(hMoudule, "Free");// 将指针指向函数首地址  
	if (ACPIGet == NULL || ACPIFree == NULL)
	{
		OutputDebugString(_T("Dll中new类未能成功加载"));
		FreeLibrary(hMoudule);
		return FALSE;
	}
	CACPI* acpi = ACPIGet();
	acpi->Excute();
	ACPI_MCFG_STRUCTURE mcfg;
	mcfg = acpi->ReturnMCFG();
	PCIE_BaseAddress = mcfg.Configuration_space_base_address.Base_Address;

	ACPIFree(acpi);
	FreeLibrary(hMoudule);

	if (PCIE_BaseAddress)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL GetPCIEConfigSpace(const ULONGLONG PCIE_BaseAddress, PVOID* Data, size_t DataSize)
{
	if(DataSize < 4096)	return FALSE;
	CRing0 ring0;
	Pci_All_Config_Space pci = ring0.ReturnPCIConfigSpace();
	::ZeroMemory(*Data, DataSize);
	ULONG bus = 0, dev = 0, func = 0, tmp = 0;

	for (USHORT i = 0; i < ring0.ReturnPCIConfigSpace().Label.size(); ++i)
	{
		bus = 0; 
		dev = 0; 
		func = 0;
		swscanf_s(pci.Label[i],
			_T("bus:%x dev:%x func:%x"),
			&bus,&dev,&func);
		cout << pci.Label[i] << ":" << endl;
		for (USHORT offset = 0; offset < 0x100; offset+=4)
		{
			ring0.RdMemory(PCIE_BaseAddress + MK_PCIEADDR(bus, dev, func, offset), 4, tmp);
			memcpy(*Data, &tmp, 4);
		}
		//测试 输出
// 		cout << "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" << endl;
// 		for (size_t j = 0; j < 0x1000; ++j)
// 		{
// 			cout << hex << setfill('0') << setw(2) << int(Data[j]) << " ";
// 			if ((j + 1) % 16 == 0)
// 			{
// 				cout << endl;
// 			}
// 		}
// 		cout << endl;
// 		system("cls");
	}
	return TRUE;
}