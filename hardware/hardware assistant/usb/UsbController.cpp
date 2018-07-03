#include "stdafx.h"
#include "UsbController.h"

_EnumUsbController::_EnumUsbController() :ControllerNum(0)
{
	if (this->EnumUsbController())
	{
		OutputDebugPrintf("Get Usb Controller!\n");
		ControllerNum = ControllerDevicePath.size();
		for (size_t i = 0; i < ControllerNum; i++)
		{
			OutputDebugPrintf("Get HCD Driver Name num: %d!\n", i);
			this->GetHCDController(i);
		}
	}
}

_EnumUsbController::~_EnumUsbController()
{
	for (UCHAR i = 0; i < this->ControllerDevicePath.size(); ++i)
	{
		this->ControllerDevicePath[i].clear();
		this->ControllerDevicePath[i].swap(this->ControllerDevicePath[i]);
	}
	this->ControllerDevicePath.clear();
	this->ControllerDevicePath.swap(this->ControllerDevicePath);
	this->UsbHostControllerInfo.clear();
	this->UsbHostControllerInfo.swap(this->UsbHostControllerInfo);
}

const wstring _EnumUsbController::ReturnControllerDevicePath(size_t num)
{
	return ControllerDevicePath[num];
}

BOOL _EnumUsbController::EnumUsbController()
{
	HDEVINFO hDevice;
	wstring tmp = _T("");
	hDevice = SetupDiGetClassDevs(
		(LPGUID)&GUID_CLASS_USB_HOST_CONTROLLER,  // 指定一个GUID(for a device setup class or a device interface class)用以过滤元素
		NULL,      // Enumerator 可为NULL
		NULL,      // hwndParent 可为NULL
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE); //列表的过滤选项，当为DIGCF_ALLCLASSES时，参数1指定的guid将不再起作用
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	SP_DEVINFO_DATA                  DeviceInfoData;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevice, i, &DeviceInfoData); i++)
	{
		SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
		DWORD buffersize = 0;
		DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		if (!SetupDiEnumDeviceInterfaces(hDevice, 0, (LPGUID)&GUID_CLASS_USB_HOST_CONTROLLER, i, &DeviceInterfaceData))	break;
		PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData = NULL;
		while (!SetupDiGetDeviceInterfaceDetail(
			hDevice,
			&DeviceInterfaceData,
			DeviceInterfaceDetailData,
			buffersize,
			&buffersize,
			NULL))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (DeviceInterfaceDetailData)
					HeapFree(GetProcessHeap(), 0, DeviceInterfaceDetailData);
				DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
				DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			}
			else
				break;
		}
		tmp = DeviceInterfaceDetailData->DevicePath;
		if (DeviceInterfaceDetailData)
		{
			HeapFree(GetProcessHeap(), 0, DeviceInterfaceDetailData);
			DeviceInterfaceDetailData = NULL;
		}
		ControllerDevicePath.push_back(tmp);
		tmp.clear();
	}
	if (GetLastError() != NO_ERROR
		&& GetLastError() != ERROR_NO_MORE_ITEMS)
	{
		SetupDiDestroyDeviceInfoList(hDevice);
		return FALSE;
	}
	// 释放
	SetupDiDestroyDeviceInfoList(hDevice);
	return TRUE;
}

BOOL _EnumUsbController::GetHCDController(size_t num)
{
	HANDLE hHCDev;
	wstring RootHubName = _T("");	//ROOT Hub Name
	wstring HCDDriverName = _T("");	// HCD Driver Name
	DEVICE_INSTANCE Device_Instance; //Device Instance
	USB_POWER_INFO Usb_Power_Info[6];	// Power Info
	USB_DEVICE_PNP_STRINGS DevicePNPInfoData;	//PNP Info
	USB_HOST_CONTROLLER_BDF DeviceBDF;	//Bus Device Function
	USB_CONTROLLER_INFO_0 Usb_Controller_Info;	//
	USBHOSTCONTROLLERINFO UsbHostControllerInfo;	//Controller All Info (above)

	// Initialize
	::ZeroMemory(&Device_Instance, sizeof(Device_Instance));
	::ZeroMemory(&DeviceBDF, sizeof(DeviceBDF));
	::ZeroMemory(&Usb_Controller_Info, sizeof(Usb_Controller_Info));
	DevicePNPInfoData.DeviceClass.clear();
	DevicePNPInfoData.DeviceDesc.clear();
	DevicePNPInfoData.DeviceId.clear();
	DevicePNPInfoData.HwId.clear();
	DevicePNPInfoData.PowerState.clear();
	DevicePNPInfoData.Service.clear();

	hHCDev = ::CreateFile(
		ControllerDevicePath[num].c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (INVALID_HANDLE_VALUE != hHCDev)
	{
		UsbHostControllerInfo.DeviceInfoType = HostControllerInfo;
		if (this->GetHCDDriverKeyName(hHCDev, HCDDriverName) == TRUE)
		{
			UsbHostControllerInfo.DriverKey = HCDDriverName;
		}
		if (this->DriverNametoDevInstance(HCDDriverName, Device_Instance, DevicePNPInfoData, DeviceBDF) == TRUE)
		{
			if (DeviceBDF.BusDeviceFunctionValid)
			{
				UsbHostControllerInfo.UsbControllerBDF = DeviceBDF;
			}
			if (DevicePNPInfoData.DeviceId != _T(""))
			{
				ULONG   ven, dev, subsys, rev;
				ven = dev = subsys = rev = 0;

				swscanf_s(DevicePNPInfoData.DeviceId.c_str(),
					_T("PCI\\VEN_%x&DEV_%x&SUBSYS_%x&REV_%x"),
					&ven, &dev, &subsys, &rev);

				UsbHostControllerInfo.VendorID = ven;
				UsbHostControllerInfo.DeviceID = dev;
				UsbHostControllerInfo.SubSysID = subsys;
				UsbHostControllerInfo.Revision = rev;
				UsbHostControllerInfo.UsbDeviceProperties = DevicePNPInfoData;
			}
		}
		if (this->GetUsbPowerState(hHCDev, (UINT)num, Usb_Power_Info) == TRUE)
		{
			for (size_t i = 0; i < 6; i++)
			{
				UsbHostControllerInfo.USBPowerInfo[i] = Usb_Power_Info[i];
			}
		}
		if (this->GetHostControllerInfo(hHCDev, (UINT)num, Usb_Controller_Info))
		{
			UsbHostControllerInfo.ControllerInfo = Usb_Controller_Info;
		}
		if (this->GetRootHubName(hHCDev, RootHubName) == TRUE)
		{
			UsbHostControllerInfo.RootHubName = RootHubName;
		}
		this->UsbHostControllerInfo.emplace_back(UsbHostControllerInfo);
		::CloseHandle(hHCDev);
		return TRUE;
	}
	return GetLastError();
}

BOOL _EnumUsbController::GetUsbPowerState(HANDLE hDevice, const UINT num, PUSB_POWER_INFO Usb_Power_Info)
{
	USBUSER_POWER_INFO_REQUEST UsbPowerinfoRequest;
	PUSB_POWER_INFO pUsbPower = &UsbPowerinfoRequest.PowerInformation;
	DWORD					   dwBytes = 0;
	int						   Index = 0;
	int                        nPowerState = WdmUsbPowerSystemWorking;

	for (; nPowerState <= WdmUsbPowerSystemShutdown; ++nPowerState, ++Index)
	{

		ZeroMemory(&UsbPowerinfoRequest, sizeof(UsbPowerinfoRequest));
		UsbPowerinfoRequest.Header.UsbUserRequest = USBUSER_GET_POWER_STATE_MAP;
		UsbPowerinfoRequest.Header.RequestBufferLength = sizeof(UsbPowerinfoRequest);
		UsbPowerinfoRequest.PowerInformation.SystemState = (WDMUSB_POWER_STATE)nPowerState;

		if (!DeviceIoControl(
			hDevice,
			IOCTL_USB_USER_REQUEST,
			&UsbPowerinfoRequest,
			sizeof(UsbPowerinfoRequest),
			&UsbPowerinfoRequest,
			sizeof(UsbPowerinfoRequest),
			&dwBytes,
			NULL))
		{
			OutputDebugPrintf("Get Usb Power State Error Code:%d\n", GetLastError());
			return GetLastError();
		}
		Usb_Power_Info[Index] = *pUsbPower;
	}
	return TRUE;
}

BOOL _EnumUsbController::DriverNametoDevInstance(const wstring DriverName, DEVICE_INSTANCE & DeviceInst, USB_DEVICE_PNP_STRINGS& DevicePNPInfoData, USB_HOST_CONTROLLER_BDF& DeviceBDF)
{
	wstring tmpDriverName = _T("");
	HDEVINFO hDevinfo = INVALID_HANDLE_VALUE;
	SP_DEVINFO_DATA Devinfo_Data;
	BOOL Status = TRUE;
	DWORD DataT;
	DWORD buffersize = 0;

	tmpDriverName = DriverName;
	if (tmpDriverName == _T(""))
	{
		Status = FALSE;
	}
	hDevinfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (hDevinfo == INVALID_HANDLE_VALUE)
	{
		Status = FALSE;
	}
	if (Status != FALSE)
	{
		Devinfo_Data.cbSize = sizeof(SP_DEVINFO_DATA);
		for (DWORD Index = 0; SetupDiEnumDeviceInfo(hDevinfo, Index, &Devinfo_Data); ++Index)
		{
			//OutputDebugPrintf("%d\n", Index);
			LPSTR buffer = NULL;
			while (!SetupDiGetDeviceRegistryProperty(
				hDevinfo,
				&Devinfo_Data,
				SPDRP_DRIVER,
				&DataT,
				(PBYTE)buffer,
				buffersize,
				&buffersize
			))
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					if (buffer)
					{
						HeapFree(GetProcessHeap(), 0, buffer);
						buffer = NULL;
					}
					buffer = (LPSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
				}
				else	break;
			}

			if ((buffer != NULL) && (_wcsicmp((PWSTR)buffer, tmpDriverName.c_str()) == 0))
			{
				DeviceInst.DeviceInfo = hDevinfo;
				DeviceInst.DeviceInfoData = Devinfo_Data;
				if (buffer)
				{
					HeapFree(GetProcessHeap(), 0, buffer);
					buffer = NULL;
				}
				break;
			}
			if (buffer)
			{
				HeapFree(GetProcessHeap(), 0, buffer);
				buffer = NULL;
			}
			buffersize = 0;
		}
	}
	this->GetDevInstInfo(DeviceInst, DevicePNPInfoData);
	this->GetDevInstInfo_Ext(DeviceInst, DevicePNPInfoData);
	this->GetUsbHostControllerBDF(DeviceInst, DeviceBDF);
	if (hDevinfo != INVALID_HANDLE_VALUE)
	{
		SetupDiDestroyDeviceInfoList(hDevinfo);
	}
	return Status;
}

BOOL _EnumUsbController::GetDevInstInfo_Ext(const DEVICE_INSTANCE DeviceInst, USB_DEVICE_PNP_STRINGS & Device_PNP_String)
{
	DWORD buffersize = 0;
	DEVICE_INSTANCE tmp = DeviceInst;
	LPSTR buffer_STR = nullptr;
	while (!SetupDiGetDeviceInstanceIdA(
		tmp.DeviceInfo,
		&tmp.DeviceInfoData,
		buffer_STR,
		buffersize,
		&buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (buffer_STR)
				HeapFree(GetProcessHeap(), 0, buffer_STR);
			buffer_STR = (LPSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
		}
		else	break;
	}
	if (buffer_STR)
	{
		string str = buffer_STR;
		int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t *wide = new wchar_t[num];
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
		std::wstring w_str(wide);
		delete[] wide;
		Device_PNP_String.DeviceId = w_str;
		HeapFree(GetProcessHeap(), 0, buffer_STR);
		buffer_STR = nullptr;
	}
	return TRUE;
}

BOOL _EnumUsbController::GetDevInstInfo(const DEVICE_INSTANCE DeviceInst, USB_DEVICE_PNP_STRINGS & Device_PNP_String)
{
	DWORD buffersize = 0;
	DEVICE_INSTANCE tmp = DeviceInst;
	LPTSTR buffer = NULL;


	buffersize = 0;
	while (!SetupDiGetDeviceRegistryProperty(
		tmp.DeviceInfo,
		&tmp.DeviceInfoData,
		SPDRP_DEVICEDESC,
		NULL,
		(PBYTE)buffer,
		buffersize,
		&buffersize
	))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (buffer)
			{
				HeapFree(GetProcessHeap(), 0, buffer);
				buffer = NULL;
			}
			buffer = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
		}
		else	break;
	}
	if (buffer)		Device_PNP_String.DeviceDesc = buffer;

	buffersize = 0;
	while (!SetupDiGetDeviceRegistryProperty(
		tmp.DeviceInfo,
		&tmp.DeviceInfoData,
		SPDRP_HARDWAREID,
		NULL,
		(PBYTE)buffer,
		buffersize,
		&buffersize
	))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (buffer)
			{
				HeapFree(GetProcessHeap(), 0, buffer);
				buffer = NULL;
			}
			buffer = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
		}
		else	break;
	}
	if (buffer)		Device_PNP_String.HwId = buffer;
	buffersize = 0;
	while (!SetupDiGetDeviceRegistryProperty(
		tmp.DeviceInfo,
		&tmp.DeviceInfoData,
		SPDRP_SERVICE,
		NULL,
		(PBYTE)buffer,
		buffersize,
		&buffersize
	))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (buffer)
			{
				HeapFree(GetProcessHeap(), 0, buffer);
				buffer = NULL;
			}
			buffer = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
		}
		else	break;
	}
	if (buffer)		Device_PNP_String.Service = buffer;
	buffersize = 0;
	while (!SetupDiGetDeviceRegistryProperty(
		tmp.DeviceInfo,
		&tmp.DeviceInfoData,
		SPDRP_CLASS,
		NULL,
		(PBYTE)buffer,
		buffersize,
		&buffersize
	))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (buffer)
			{
				HeapFree(GetProcessHeap(), 0, buffer);
				buffer = NULL;
			}
			buffer = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
		}
		else	break;
	}
	if (buffer)		Device_PNP_String.DeviceClass = buffer;
	buffersize = 0;
	if (buffer)
	{
		HeapFree(GetProcessHeap(), 0, buffer);
		buffer = NULL;
	}
	return TRUE;
}

BOOL _EnumUsbController::GetUsbHostControllerBDF(const DEVICE_INSTANCE DeviceInst, USB_HOST_CONTROLLER_BDF & DeviceBDF)
{
	PULONG buffer = NULL;
	DWORD buffersize = 0;
	DEVICE_INSTANCE tmp = DeviceInst;
	DeviceBDF.BusDeviceFunctionValid = FALSE;
	while (!SetupDiGetDeviceRegistryProperty(
		tmp.DeviceInfo,
		&tmp.DeviceInfoData,
		SPDRP_BUSNUMBER,
		NULL,
		(PBYTE)buffer,
		buffersize,
		&buffersize
	))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (buffer)
			{
				HeapFree(GetProcessHeap(), 0, buffer);
				buffer = NULL;
			}
			buffer = (PULONG)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
		}
		else	break;
	}
	if (buffer)		DeviceBDF.BusNumber = (USHORT)*buffer;
	buffersize = 0;
	while (!SetupDiGetDeviceRegistryProperty(
		tmp.DeviceInfo,
		&tmp.DeviceInfoData,
		SPDRP_ADDRESS,
		NULL,
		(PBYTE)buffer,
		buffersize,
		&buffersize
	))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (buffer)
			{
				HeapFree(GetProcessHeap(), 0, buffer);
				buffer = NULL;
			}
			buffer = (PULONG)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
		}
		else	break;
	}
	if (buffer)
	{
		DeviceBDF.BusDevice = (*buffer) >> 16;
		DeviceBDF.BusFunction = (*buffer) & 0xFFFF;
		DeviceBDF.BusDeviceFunctionValid = TRUE;
	}
	else	DeviceBDF.BusDeviceFunctionValid = FALSE;
	return DeviceBDF.BusDeviceFunctionValid;
}

BOOL _EnumUsbController::GetHostControllerInfo(HANDLE hDevice, const UINT num, USB_CONTROLLER_INFO_0& Usb_Controller_Info)
{
	USBUSER_CONTROLLER_INFO_0 UsbControllerinfoRequest;
	PUSB_CONTROLLER_INFO_0 pUsbController = &UsbControllerinfoRequest.Info0;
	DWORD					   dwBytes = 0;

	ZeroMemory(&UsbControllerinfoRequest, sizeof(UsbControllerinfoRequest));
	UsbControllerinfoRequest.Header.UsbUserRequest = USBUSER_GET_CONTROLLER_INFO_0;
	UsbControllerinfoRequest.Header.RequestBufferLength = sizeof(UsbControllerinfoRequest);

	if (!DeviceIoControl(
		hDevice,
		IOCTL_USB_USER_REQUEST,
		&UsbControllerinfoRequest,
		sizeof(UsbControllerinfoRequest),
		&UsbControllerinfoRequest,
		sizeof(UsbControllerinfoRequest),
		&dwBytes,
		NULL))
	{
		OutputDebugPrintf("Get Usb Host Controller Info Error Code:%d\n", GetLastError());
		return GetLastError();
	}
	Usb_Controller_Info = *pUsbController;
	return TRUE;
}

BOOL _EnumUsbController::GetRootHubName(HANDLE HostController, wstring & RootHubName)
{
	BOOL                success = 0;
	ULONG               nBytes = 0;
	USB_ROOT_HUB_NAME   rootHubName;
	PUSB_ROOT_HUB_NAME  RootTmpName = NULL;

	// Get the length of the name of the Root Hub attached to the
	// Host Controller
	//
	success = DeviceIoControl(HostController,
		IOCTL_USB_GET_ROOT_HUB_NAME,
		0,
		0,
		&rootHubName,
		sizeof(rootHubName),
		&nBytes,
		NULL);

	if (!success)
	{
		goto GetRootHubNameError;
	}

	// Allocate space to hold the Root Hub name
	//
	nBytes = rootHubName.ActualLength;
	if (RootTmpName)
	{
		HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, RootTmpName);
		RootTmpName = NULL;
	}
	RootTmpName = (PUSB_ROOT_HUB_NAME)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nBytes);
	if (RootTmpName == NULL)
	{
		goto GetRootHubNameError;
	}

	// Get the name of the Root Hub attached to the Host Controller
	//
	success = DeviceIoControl(HostController,
		IOCTL_USB_GET_ROOT_HUB_NAME,
		NULL,
		0,
		RootTmpName,
		nBytes,
		&nBytes,
		NULL);
	if (!success)
	{
		goto GetRootHubNameError;
	}

	// Convert the Root Hub name
	//
	this->Wchar_tToString(RootHubName, RootTmpName->RootHubName);

	// All done, free the uncoverted Root Hub name and return the
	// converted Root Hub name
	//
	if (RootTmpName != NULL)
	{
		HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, RootTmpName);
		RootTmpName = NULL;
	}

	return TRUE;

GetRootHubNameError:
	// There was an error, free anything that was allocated
	//
	if (RootTmpName != NULL)
	{
		HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, RootTmpName);
		RootTmpName = NULL;
	}
	return FALSE;
}

//From msdn
BOOL _EnumUsbController::GetHCDDriverKeyName(HANDLE HCD, wstring& HCDDriverName)
{
	BOOL                    success;
	ULONG                   nBytes;
	USB_HCD_DRIVERKEY_NAME  driverKeyName;
	PUSB_HCD_DRIVERKEY_NAME driverKeyNameW = NULL;
	success = DeviceIoControl(HCD,
		IOCTL_GET_HCD_DRIVERKEY_NAME,
		NULL,
		0,
		&driverKeyName,
		sizeof(driverKeyName),
		&nBytes,
		NULL);

	if (!success)
	{
		OutputDebugPrintf("First IOCTL_GET_HCD_DRIVERKEY_NAME request failed\n");
		goto GetHCDDriverKeyNameDone;
	}

	//2. Get the length of the driver key name.
	nBytes = driverKeyName.ActualLength;

	if (nBytes <= sizeof(driverKeyName))
	{
		OutputDebugPrintf("Incorrect length received by IOCTL_GET_HCD_DRIVERKEY_NAME.\n");
		goto GetHCDDriverKeyNameDone;
	}

	// 3. Allocate memory for a USB_HCD_DRIVERKEY_NAME
	//    to hold the driver key name.
	driverKeyNameW = (PUSB_HCD_DRIVERKEY_NAME)malloc(nBytes);

	if (driverKeyNameW == NULL)
	{
		OutputDebugPrintf("Failed to allocate memory.\n");
		goto GetHCDDriverKeyNameDone;
	}

	// Get the name of the driver key of the device attached to
	// the specified port.
	success = DeviceIoControl(HCD,
		IOCTL_GET_HCD_DRIVERKEY_NAME,
		NULL,
		0,
		driverKeyNameW,
		nBytes,
		&nBytes,
		NULL);

	if (!success)
	{
		OutputDebugPrintf("Second IOCTL_GET_HCD_DRIVERKEY_NAME request failed.\n");
		goto GetHCDDriverKeyNameDone;
	}

	// print the driver key name. 
	OutputDebugPrintf("Driver Key Name: %ls.\n", driverKeyNameW->DriverKeyName);
	this->Wchar_tToString(HCDDriverName, driverKeyNameW->DriverKeyName);

GetHCDDriverKeyNameDone:

	// Cleanup.
	// Free the allocated memory for USB_HCD_DRIVERKEY_NAME.

	if (driverKeyNameW != NULL)
	{
		free(driverKeyNameW);
		driverKeyNameW = NULL;
	}

	return success;
}

void _EnumUsbController::Wchar_tToString(std::wstring & szDst, wchar_t * wchar)
{
// 	wchar_t * wText = wchar;
// 	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用  
// 	char *psText; // psText为char*的临时数组，作为赋值给std::wstring的中间变量  
// 	psText = new char[dwNum];
// 	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用  
// 	szDst = psText;// std::wstring赋值  
// 	delete[]psText;// psText的清除  
// 	psText = NULL;
	szDst = wchar;
}
