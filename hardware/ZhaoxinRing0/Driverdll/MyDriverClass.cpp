#include "stdafx.h"
#include "MyDriverClass.h"

CRing0::CRing0():hdevice(INVALID_HANDLE_VALUE)
{
	if(this->CRing0_Initialize())
	{
		this->ReadAllPci();
		if (Pci_Config_space.pciconfig.size())
		{
			VendorID = Pci_Config_space.pciconfig.begin()->second.VendorID;
		}
	}
#ifdef _DEBUG
	else
	{
		OutputDebugPrintf(_T("Init Ring0 Failed!\n"));
	}
#endif
}


CRing0::~CRing0()
{
	this->CRing0_Destory();
}

BOOL CRing0::WrMsr(IN DWORD Index, IN DWORD64 Data)
{
	ULONG nOutput;
	MSR_Request MSR;
	MSR.msr_address = Index;
	MSR.Value = Data;
	if (!DeviceIoControl(hdevice,
		WRITE_MSR,
		&MSR,
		sizeof(MSR),
		NULL,
		NULL,
		&nOutput,
		NULL)
		)
	{
		return GetLastError();
	}
	return TRUE;
}

BOOL CRing0::RdMsrTx(IN DWORD Index, IN DWORD threadAffinityMask, OUT DWORD64 & Data)
{
	ULONG nOutput;
	MSR_Request MSR;
	MSR.msr_address = Index;
	MSR.Value = 0;
	MSR.core_id = threadAffinityMask;
	if (!DeviceIoControl(hdevice,
		READ_MSR_THREAD,
		&MSR,
		sizeof(MSR),
		&Data,
		sizeof(unsigned long long),
		&nOutput,
		NULL)
		)
	{
		return GetLastError();
	}
	return TRUE;
}

BOOL CRing0::WrMsrTx(IN DWORD Index, IN DWORD threadAffinityMask, IN DWORD64 Data)
{
	ULONG nOutput;
	MSR_Request MSR;
	MSR.msr_address = Index;
	MSR.Value = Data;
	MSR.core_id = threadAffinityMask;
	if (!DeviceIoControl(hdevice,
		WRITE_MSR_THREAD,
		&MSR,
		sizeof(MSR),
		NULL,
		NULL,
		&nOutput,
		NULL)
		)
	{
		return GetLastError();
	}
	return TRUE;
}

BOOL CRing0::RdIOPort(IN USHORT IO_Port_Addr,IN USHORT IO_DataSize, OUT DWORD& IO_Data)
{
	ULONG nOutput;
	TargetPortData IO_Port;
	IO_Port.Port_Addr = IO_Port_Addr;
	IO_Port.Port_Data = 0;
	IO_Port.Data_size = (UCHAR)IO_DataSize;
	if (IO_Port.Port_Addr == NULL)
	{
		return FALSE;
	}
	if ((IO_Port.Data_size == 1) || (IO_Port.Data_size == 2) || (IO_Port.Data_size == 4))
	{
		if (!DeviceIoControl(hdevice,
			READ_PORT,
			&IO_Port,
			sizeof(IO_Port),
			&IO_Data,
			sizeof(unsigned long long),
			&nOutput,
			NULL)
			)
		{
			return GetLastError();
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CRing0::WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data)
{
	ULONG nOutput;
	TargetPortData IO_Port;
	IO_Port.Port_Addr = IO_Port_Addr;
	IO_Port.Port_Data = IO_Data;
	IO_Port.Data_size = (UCHAR)IO_DataSize;
	if (IO_Port.Port_Addr == NULL)
	{
		return FALSE;
	}
	if ((IO_Port.Data_size == 1) ||(IO_Port.Data_size == 2)|| (IO_Port.Data_size == 4))
	{
		if (!DeviceIoControl(hdevice,
			WRITE_PORT,
			&IO_Port,
			sizeof(IO_Port),
			NULL,
			NULL,
			&nOutput,
			NULL)
			)
		{
			return GetLastError();
		}
		return TRUE; 
	}
	return FALSE;
}

BOOL CRing0::RdMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data)
{
	READ_MEMORY_INPUT MemData;
	ULONG membuffer;
	ULONG nOutput;
	MemData.Count = 1;
	MemData.UnitSize = Mem_DataSize;
	memset(&membuffer, 0, sizeof(membuffer));
/*
	PVOID membuffer = new UCHAR[256];
	ULONG nOutput;
	MemData.Count = 1;
	MemData.UnitSize = Mem_DataSize;
	memset(membuffer, 0, sizeof(membuffer));
*/
	if ((MemData.UnitSize == 1) || (MemData.UnitSize == 2) || (MemData.UnitSize == 4))
	{
		if constexpr (sizeof(void*) == 4)
		{
			MemData.Address.HighPart = 0;
			MemData.Address.LowPart = (DWORD)Memory_Addr;
		}
		else
		{
			MemData.Address.QuadPart = Memory_Addr;
		}

		if (!DeviceIoControl(hdevice,
			READ_MEMORY,
			&MemData,
			sizeof(READ_MEMORY_INPUT),
			&membuffer,
			MemData.Count * MemData.UnitSize,
			&nOutput,
			NULL)
			)
		{
			return GetLastError();
		}
		memcpy_s(&Memory_Data, sizeof(Memory_Data),&membuffer, sizeof(membuffer));
		return TRUE;
	}
	return FALSE;
}

BOOL CRing0::WrMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
{
	WRITE_MEMORY_INPUT WRmem;
	ULONG nOutput;
	WRmem.Count = 1;
	WRmem.Data = Memory_Data;
	WRmem.UnitSize = Mem_DataSize;
	if ((WRmem.UnitSize == 1) || (WRmem.UnitSize == 2) || (WRmem.UnitSize == 4))
	{
		if constexpr (sizeof(void*) == 4)
		{
			WRmem.Address.HighPart = 0;
			WRmem.Address.LowPart = (DWORD)Memory_Addr;
		}
		else
		{
			WRmem.Address.QuadPart = Memory_Addr;
		}

		if (!DeviceIoControl(hdevice,
			WRITE_MEMORY,
			&WRmem,
			sizeof(WRITE_MEMORY_INPUT),
			NULL,
			NULL,
			&nOutput,
			NULL)
			)
		{
			return GetLastError();
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CRing0::ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG& pci_config)
{
	ULONG nOutput;
	PCI_ADDRESS pci_addr;
	pci_addr.bus = bus;
	pci_addr.dev = dev;
	pci_addr.func = func;
	pci_addr.Data = 0;
	pci_addr.Offset = 0;
	pci_config = {};
	if (!DeviceIoControl(hdevice,
			READ_PCI_CONFIG,
			&pci_addr,
			sizeof(pci_addr),
			&pci_config,
			sizeof(pci_config),
			&nOutput,
			NULL)
			)
		{
			return GetLastError();
		}
	return TRUE;
}

BOOL CRing0::WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data)
{
	ULONG nOutput;
	PCI_ADDRESS pci_addr;
	pci_addr.bus = bus;
	pci_addr.dev = dev;
	pci_addr.func = func;
	pci_addr.Offset = offset;
	pci_addr.Data = Data;
	if (!DeviceIoControl(hdevice,
		WRITE_PCI_CONFIG,
		&pci_addr,
		sizeof(pci_addr),
		NULL,
		NULL,
		&nOutput,
		NULL)
		)
	{
		return GetLastError();
	}
	return TRUE;
}

BOOL CRing0::ReadAllPci()
{
	PCI_COMMON_CONFIG Pci_Temp;
	for (USHORT bus = 0; bus <= 0xFF; ++bus)
	{
		for (USHORT dev = 0; dev <= 0x1F; ++dev)
		{
			for (USHORT func = 0; func <= 0x07; ++func)
			{
				this->ReadPci(bus, dev, func, Pci_Temp);
				if (Pci_Temp.VendorID != 0xFFFF)
				{
					wstring temp = _T("bus:") + to_wstring(bus) + _T(" dev:") + to_wstring(dev) + _T(" func:") + to_wstring(func);
					Pci_Config_space.pciconfig.insert(std::map<std::wstring, PCI_COMMON_CONFIG>::value_type(temp, Pci_Temp));
				}
			}
		}
	}
	return TRUE;
}

BOOL CRing0::ReadSPD(USHORT Base_Address, USHORT Slave_Address, DDR3_INFO & data)
{
	SPD_Head spd;
	ULONG nOutput;
	//Data = 0;
	spd.Base_Address = Base_Address;
	spd.Slave_Address = Slave_Address;
	spd.Offset = 0;
	if (!DeviceIoControl(hdevice,
			READ_SPD,
			&spd,
			sizeof(spd),
			&ddr3,
			sizeof(DDR3_INFO),
			&nOutput,
			NULL)
			)
		{
			return GetLastError();
		}
	::CopyMemory(&data, &this->ddr3, sizeof(ddr3));
	//printf("SPD OFF:%d Data:%X\n",offset ,Data);
	return TRUE;
}

const Pci_All_Config_Space& CRing0::ReturnPCIConfigSpace()
{
	return this->Pci_Config_space;
}

BOOL CRing0::CRing0_Initialize()
{
	BOOL status = FALSE;

	for (int i = 0; i < 4; i++)
	{
		status = Initialize();
		if (status == TRUE)
		{
			OutputDebugPrintf(_T("Initialize Driver Success\n"));
			hdevice = gHandle;
			break;
		}
#ifdef _DEBUG
		else
		{
			OutputDebugPrintf(_T("Initialize Driver Failed Times: %d\n"), i);
		}
#endif
		Sleep(100 * i);
	}

	return status;

}

BOOL CRing0::CRing0_Destory()
{
	if(hdevice != INVALID_HANDLE_VALUE || gHandle != INVALID_HANDLE_VALUE) {
		//
		// Close the handle to the device before unloading the driver.
		//

		CloseHandle(gHandle);
		gHandle = INVALID_HANDLE_VALUE;
		hdevice = INVALID_HANDLE_VALUE;

		//
		// Unload the driver.  Ignore any errors.
		//

		ManageDriver(DRIVER_ID, gDriverPath, OLS_DRIVER_EXIT);
	}

	return TRUE;
}

BOOL CRing0::GetECData(Embedded_Controller_Data & EC_Data)
{
	DWORD Data = 0;
	for (USHORT i = 0; i < 0x100; i++)
	{
		this->RdIOPort(EC_SC, 1, Data);
		if (Data == 0x08)
		{
			this->WrIOPort(EC_DATA, 1, 0x00);	// 存在读状态时完成读取状态
		}
		this->RdIOPort(EC_SC, 1, Data);
		if (Data)
		{
			OutputDebugString(_T("EC存在问题\n"));
			return FALSE;
		}
		this->WrIOPort(EC_SC, 1, RD_EC);		// 将EC设置为读状态
		this->RdIOPort(EC_SC, 1, Data);
		if (Data != 0x08)
		{
			OutputDebugString(_T("读取EC失败!\n"));
			return FALSE;
		}
		this->WrIOPort(EC_DATA, 1, i);
		this->RdIOPort(EC_DATA, 1, Data);
		memcpy_s(&EC_Data.Data[i], 1, &Data, 1);
	}
	return TRUE;
}

BOOL CRing0::SetECData(BYTE EC_Addr, BYTE EC_Write_Data)
{
	DWORD Data = 0;
	this->WrIOPort(EC_SC, 1, WR_EC);	//将EC设置为写状态
	this->RdIOPort(EC_SC, 1, Data);
	if (Data != 0x08)		return FALSE;
	this->WrIOPort(EC_DATA, 1, EC_Addr);	//	将地址写入EC
	this->WrIOPort(EC_DATA, 1, EC_Write_Data);	//将数据写入EC
	return TRUE;
}

template<class T>
INT64 CRing0::find_location(vector<T> vecIntegerArray, string search_string)
{
	vector<T>::iterator iElement = find(
		vecIntegerArray.begin(),
		vecIntegerArray.end(),
		search_string
		);
	if (iElement != vecIntegerArray.end())
	{
		INT64 Positison = distance(vecIntegerArray.begin(), iElement);
		return Positison;
	}
	return -1;
}

BOOL CRing0::RdMsr(IN DWORD Index, OUT DWORD64& Data)
{
	ULONG nOutput;
	MSR_Request MSR;
	MSR.msr_address = Index;
	MSR.Value = 0;
	if (!DeviceIoControl(hdevice,
		READ_MSR,
		&MSR,
		sizeof(MSR),
		&Data,
		sizeof(unsigned long long),
		&nOutput,
		NULL)
		)
	{
		return GetLastError();
	}
	return TRUE;
}