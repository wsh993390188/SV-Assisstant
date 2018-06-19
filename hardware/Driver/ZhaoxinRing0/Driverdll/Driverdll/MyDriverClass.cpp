//#include "stdafx.h"
#include "Memory/DDR3/DDR3.h"

CRing0::CRing0():SuperIO_Ctrl_Port(0x2E),SuperIO_Data_Port(0x2F), hdevice(INVALID_HANDLE_VALUE)
{
	if(this->CRing0_Initialize())
	{
		this->ReadAllPci();
		VendorID = Pci_Config_space.Pci_Config_Space[0].VendorID; 
	}
}

CRing0::~CRing0()
{
	this->CRing0_Destory();
}

BOOL CRing0::WrMsr(IN DWORD Index, IN DWORD64 Data)
{
	ULONG nOutput;
	Msr_Reg MSR;
	MSR.Register = Index;
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

BOOL CRing0::RdIOPort(IN USHORT IO_Port_Addr,IN USHORT IO_DataSize, OUT DWORD64& IO_Data)
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

BOOL CRing0::RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data)
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
		if (sizeof(DWORD_PTR) == 4)
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

BOOL CRing0::WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
{
	WRITE_MEMORY_INPUT WRmem;
	ULONG nOutput;
	WRmem.Count = 1;
	WRmem.Data = Memory_Data;
	WRmem.UnitSize = Mem_DataSize;
	if ((WRmem.UnitSize == 1) || (WRmem.UnitSize == 2) || (WRmem.UnitSize == 4))
	{
		if (sizeof(DWORD_PTR) == 4)
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
	DWORD dwData = 0;
	pci_addr.bus = bus;
	pci_addr.dev = dev;
	pci_addr.func = func;
	pci_addr.Data = 0;
	/*for(int i =0; i < 0x100; i+=4)
	{
		pci_addr.Offset = i;
		if (!DeviceIoControl(hdevice,
			READ_PCI_CONFIG,
			&pci_addr,
			sizeof(pci_addr),
			&dwData,
			sizeof(dwData),
			&nOutput,
			NULL)
			)
		{
			return GetLastError();
		}
		if((0 == i) && (0xffffffff == dwData))
		{
			break;
		}
		memcpy(((PUCHAR)&pci_config) + i, &dwData, 4);
	}*/
	PCI_COMMON_CONFIG _pci;
	pci_addr.Offset = 0;
	if (!DeviceIoControl(hdevice,
			READ_PCI_CONFIG,
			&pci_addr,
			sizeof(pci_addr),
			&_pci,
			sizeof(_pci),
			&nOutput,
			NULL)
			)
		{
			return GetLastError();
		}
	memcpy(&pci_config, &_pci, sizeof(_pci));
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
		READ_PCI_CONFIG,
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
					wstring temp = _T("bus:") + to_wstring((_ULonglong)bus) + _T(" dev:") + to_wstring((_ULonglong)dev) + _T(" func:") + to_wstring((_ULonglong)func);
					unique_ptr<TCHAR[]> name_buffer(new TCHAR[temp.length() + 2]);
					//memset(name_buffer,0,temp.length());
					wcsncpy_s(name_buffer.get(), temp.length() + 2, temp.c_str(),temp.length() + 1);
					Pci_Config_space.Label.emplace_back(name_buffer.get());
					Pci_Config_space.Pci_Config_Space.emplace_back(Pci_Temp);
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
			&Data,
			sizeof(DDR3_INFO),
			&nOutput,
			NULL)
			)
		{
			return GetLastError();
		}
	::CopyMemory(&data, &this->Data, sizeof(Data));
	//printf("SPD OFF:%d Data:%X\n",offset ,Data);
	return TRUE;
}

BOOL CRing0::ReadITE()
{
	DWORD64 SuperIO_Data = 0;
	//进入ITE芯片
	for (USHORT i = 0; i < GetStaticArraylength(ITE_Entry); ++i)
	{
		this->WrIOPort(SuperIO_Ctrl_Port, 1, ITE_Entry[i]);
	}
	//枚举变量
	//cout << "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" << endl; 
	Super_IO SuperIOtmp;
	for (USHORT i = 1; i <= 0xC; ++i)
	{
		//cout << endl << "FUNC: " << i << endl;
		ZeroMemory(&SuperIOtmp, sizeof(SuperIOtmp));
		this->WrIOPort(SuperIO_Ctrl_Port, 1, ITE_DEVCIE_SELECT_REGISTER);
		this->WrIOPort(SuperIO_Data_Port, 1, i);
		for (USHORT j = 0; j < 0x100; ++j)
		{
			this->WrIOPort(SuperIO_Ctrl_Port, 1, j);
			this->RdIOPort(SuperIO_Data_Port, 1, SuperIO_Data);
// 			cout << hex << setfill('0') << setw(2) << SuperIO_Data << " ";
// 			if ((j + 1) % 16 == 0)
// 			{
// 				cout << endl;
// 			}
			SuperIOtmp.SuperioData[j] = (UCHAR)SuperIO_Data;
		}
		SuperIO.emplace_back(SuperIOtmp);
	}
	//退出芯片
	for (USHORT i = 0; i < GetStaticArraylength(ITE_Exit); ++i)
	{
		this->WrIOPort(SuperIO_Ctrl_Port, 1, ITE_Exit[i]);
		this->WrIOPort(SuperIO_Data_Port, 1, ITE_Exit[i]);
	}
	return TRUE;
}

const Pci_All_Config_Space CRing0::ReturnPCIConfigSpace()
{
	return this->Pci_Config_space;
}

BOOL CRing0::CRing0_Initialize()
{
// 	G_DevicePath = this->GetDevicePath((LPGUID)&GUID_DEVINTERFACE_CharTest);
// 	hdevice = CreateFile(
// 		G_DevicePath,
// 		GENERIC_READ | GENERIC_WRITE,
// 		FILE_SHARE_READ | FILE_SHARE_WRITE,
// 		NULL,
// 		OPEN_EXISTING,
// 		0,
// 		NULL
// 	);
// 	if (hdevice == INVALID_HANDLE_VALUE) {
// 		return GetLastError();
// 	}
// 	return TRUE;

	BOOL status = FALSE;
	//
	// Load WdfCoInstaller.dll.
	//
	library = LoadWdfCoInstaller();

	if (library == NULL) {
// 		printf("The WdfCoInstaller%s.dll library needs to be "
// 			"in same directory as nonpnpapp.exe\n", coinstallerVersion);
		return FALSE;
	}

	for (int i = 0; i < 4; i++)
	{
		status = Initialize();
		if (status == TRUE)
		{
			hdevice = gHandle;
			break;
		}
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

		ManageDriver(DRIVER_ID, gDriverPath, OLS_DRIVER_REMOVE);
	}

	//
	// Unload WdfCoInstaller.dll
	//
	if (library) {
		UnloadWdfCoInstaller(library);
	}

	return TRUE;
}

TCHAR* CRing0::GetDevicePath(
	IN  LPGUID InterfaceGuid
)
{
	HDEVINFO HardwareDeviceInfo;
	SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData = NULL;
	ULONG Length, RequiredLength = 0;
	BOOL bResult;

	HardwareDeviceInfo = SetupDiGetClassDevs(
		InterfaceGuid,
		NULL,
		NULL,
		(DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

	if (HardwareDeviceInfo == INVALID_HANDLE_VALUE) {
		printf("SetupDiGetClassDevs failed!\n");
		exit(1);
	}

	DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	bResult = SetupDiEnumDeviceInterfaces(HardwareDeviceInfo,
		0,
		InterfaceGuid,
		0,
		&DeviceInterfaceData);

	if (bResult == FALSE) {
		/*
		LPVOID lpMsgBuf;

		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR) &lpMsgBuf,
		0,
		NULL
		)) {

		printf("Error: %s", (LPSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		}
		*/
		printf("SetupDiEnumDeviceInterfaces failed.Error Code: %d\n", GetLastError());

		SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
		exit(1);
	}

	SetupDiGetDeviceInterfaceDetail(
		HardwareDeviceInfo,
		&DeviceInterfaceData,
		NULL,
		0,
		&RequiredLength,
		NULL
	);

	DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, RequiredLength);

	if (DeviceInterfaceDetailData == NULL) {
		SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
		printf("Failed to allocate memory.\n");
		exit(1);
	}

	DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	Length = RequiredLength;

	bResult = SetupDiGetDeviceInterfaceDetail(
		HardwareDeviceInfo,
		&DeviceInterfaceData,
		DeviceInterfaceDetailData,
		Length,
		&RequiredLength,
		NULL);

	if (bResult == FALSE) {
		/*
		LPVOID lpMsgBuf;

		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR) &lpMsgBuf,
		0,
		NULL
		)) {

		MessageBox(NULL, (LPCTSTR) lpMsgBuf, "Error", MB_OK);
		LocalFree(lpMsgBuf);
		}
		*/
		printf("Error in SetupDiGetDeviceInterfaceDetail\n");

		SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
		LocalFree(DeviceInterfaceDetailData);
		exit(1);
	}

	return DeviceInterfaceDetailData->DevicePath;

}

BOOL CRing0::GetECData(Embedded_Controller_Data & EC_Data)
{
	DWORD64 Data = 0;
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
	DWORD64 Data = 0;
	this->WrIOPort(EC_SC, 1, WR_EC);	//将EC设置为写状态
	this->RdIOPort(EC_SC, 1, Data);
	if (Data != 0x08)		return FALSE;
	this->WrIOPort(EC_DATA, 1, EC_Addr);	//	将地址写入EC
	this->WrIOPort(EC_DATA, 1, EC_Write_Data);	//将数据写入EC
	return TRUE;
}

template<typename T>
int CRing0::GetStaticArraylength(IN T& Array)
{
	return sizeof(Array)/sizeof(Array[0]) ;
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
	Msr_Reg MSR;
	MSR.Register = Index;
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


