#include "stdafx.h"
#include "UsbController.h"

_EnumUsbController::_EnumUsbController() : ControllerDevicePath{}, UsbHostControllerInfo{}
{
	if (this->EnumUsbController())
	{
		OutputDebugPrintf(_T("Get Usb Controller!\n"));
		for (const auto& name : ControllerDevicePath)
		{
			this->GetHCDController(name);
		}
	}
}


_EnumUsbController::~_EnumUsbController()
{
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

BOOL _EnumUsbController::GetHCDController(const wstring& ControllerName)
{
	HANDLE hHCDev = {};
	wstring RootHubName = {};	//ROOT Hub Name
	wstring HCDDriverName = {};	// HCD Driver Name
	DEVICE_INSTANCE Device_Instance = {}; //Device Instance
	USB_POWER_INFO Usb_Power_Info[6] = {};	// Power Info
	USB_DEVICE_PNP_STRINGS DevicePNPInfoData = {};	//PNP Info
	USB_HOST_CONTROLLER_BDF DeviceBDF = {};	//Bus Device Function
	USB_CONTROLLER_INFO_0 Usb_Controller_Info = {};	//
	USBHOSTCONTROLLERINFO UsbHostControllerInfo = {};	//Controller All Info (above)

	hHCDev = ::CreateFile(
		ControllerName.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (INVALID_HANDLE_VALUE != hHCDev)
	{
		UsbHostControllerInfo.DeviceInfoType = HostControllerInfo;
		if (this->GetHCDDriverKeyName(hHCDev, HCDDriverName))
		{
			UsbHostControllerInfo.DriverKey = HCDDriverName;
		}
		if (this->DriverNametoDevInstance(HCDDriverName, DevicePNPInfoData, DeviceBDF) == TRUE)
		{
			if (DeviceBDF.BusDeviceFunctionValid)
			{
				UsbHostControllerInfo.UsbControllerBDF = DeviceBDF;
			}
			if (!DevicePNPInfoData.DeviceId.empty())
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
		if (this->GetUsbPowerState(hHCDev, UsbHostControllerInfo.USBPowerInfo))
		{
			OutputDebugString(L"Get Usb PowerState Success!\n");
		}
		if (this->GetHostControllerInfo(hHCDev, Usb_Controller_Info))
		{
			UsbHostControllerInfo.ControllerInfo = Usb_Controller_Info;
		}
		if (this->GetRootHubName(hHCDev, RootHubName))
		{
			UsbHostControllerInfo.RootHubName = RootHubName;
		}

		this->UsbHostControllerInfo.emplace_back(UsbHostControllerInfo);
		::CloseHandle(hHCDev);
		return TRUE;
	}
	return GetLastError();
}

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
		OutputDebugPrintf(_T("First IOCTL_GET_HCD_DRIVERKEY_NAME request failed\n"));
		goto GetHCDDriverKeyNameDone;
	}

	//2. Get the length of the driver key name.
	nBytes = driverKeyName.ActualLength;

	if (nBytes <= sizeof(driverKeyName))
	{
		OutputDebugPrintf(_T("Incorrect length received by IOCTL_GET_HCD_DRIVERKEY_NAME.\n"));
		goto GetHCDDriverKeyNameDone;
	}

	// 3. Allocate memory for a USB_HCD_DRIVERKEY_NAME
	//    to hold the driver key name.
	driverKeyNameW = (PUSB_HCD_DRIVERKEY_NAME)malloc(nBytes);

	if (driverKeyNameW == NULL)
	{
		OutputDebugPrintf(_T("Failed to allocate memory.\n"));
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
		OutputDebugPrintf(_T("Second IOCTL_GET_HCD_DRIVERKEY_NAME request failed.\n"));
		goto GetHCDDriverKeyNameDone;
	}

	// print the driver key name. 
	OutputDebugPrintf(_T("Driver Key Name: %ls.\n"), driverKeyNameW->DriverKeyName);
	HCDDriverName.append(driverKeyNameW->DriverKeyName);

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
	RootHubName = RootTmpName->RootHubName;

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

BOOL _EnumUsbController::GetUsbPowerState(HANDLE hDevice, PUSB_POWER_INFO Usb_Power_Info)
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
			OutputDebugPrintf(_T("Get Usb Power State Error Code:%d\n"), GetLastError());
			return GetLastError();
		}
		Usb_Power_Info[Index] = *pUsbPower;
	}
	return TRUE;
}

BOOL _EnumUsbController::GetHostControllerInfo(HANDLE hDevice, USB_CONTROLLER_INFO_0& Usb_Controller_Info)
{
	USBUSER_CONTROLLER_INFO_0 UsbControllerinfoRequest = {};
	DWORD					   dwBytes = 0;

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
		OutputDebugPrintf(_T("Get Usb Host Controller Info Error Code:%d\n"), GetLastError());
		return GetLastError();
	}
	Usb_Controller_Info = UsbControllerinfoRequest.Info0;
	return TRUE;
}

BOOL _EnumUsbController::DriverNametoDevInstance(const wstring& DriverName, USB_DEVICE_PNP_STRINGS& DevicePNPInfoData, USB_HOST_CONTROLLER_BDF& DeviceBDF)
{
	if (DriverName.empty())
		return FALSE;
	HDEVINFO hDevinfo = INVALID_HANDLE_VALUE;
	SP_DEVINFO_DATA Devinfo_Data = {};
	BOOL Status = TRUE;
	DWORD DataT;
	DWORD buffersize = 0;

	hDevinfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (hDevinfo == INVALID_HANDLE_VALUE)
	{
		Status = FALSE;
	}
	else
	{
		Devinfo_Data.cbSize = sizeof(SP_DEVINFO_DATA);
		LPTSTR buffer = NULL;
		for (DWORD Index = 0; SetupDiEnumDeviceInfo(hDevinfo, Index, &Devinfo_Data); ++Index)
		{
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
					buffer = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
				}
				else	break;
			}

			if ((buffer != NULL) && (_wcsicmp(buffer, DriverName.c_str()) == 0))
			{
				this->GetDevInstInfo(hDevinfo, Devinfo_Data, DevicePNPInfoData);
				this->GetDevInstInfo_Ext(hDevinfo, Devinfo_Data, DevicePNPInfoData);
				this->GetUsbHostControllerBDF(hDevinfo, Devinfo_Data, DeviceBDF);
				break;
			}

		}
		if (buffer)
		{
			HeapFree(GetProcessHeap(), 0, buffer);
			buffer = NULL;
		}
		buffersize = 0;

	}

	if (hDevinfo != INVALID_HANDLE_VALUE)
	{
		SetupDiDestroyDeviceInfoList(hDevinfo);
	}

	return Status;
}

BOOL _EnumUsbController::GetDevInstInfo(HDEVINFO DeviceInfo, SP_DEVINFO_DATA &DeviceInfoData, USB_DEVICE_PNP_STRINGS & Device_PNP_String)
{
	GetDeviceProperty(DeviceInfo, &DeviceInfoData, SPDRP_DEVICEDESC, Device_PNP_String.DeviceDesc);
	GetDeviceProperty(DeviceInfo, &DeviceInfoData, SPDRP_HARDWAREID, Device_PNP_String.HwId);
	GetDeviceProperty(DeviceInfo, &DeviceInfoData, SPDRP_SERVICE, Device_PNP_String.Service);
	GetDeviceProperty(DeviceInfo, &DeviceInfoData, SPDRP_CLASS, Device_PNP_String.DeviceClass);
	return TRUE;
}

BOOL _EnumUsbController::GetDevInstInfo_Ext(HDEVINFO DeviceInfo, SP_DEVINFO_DATA& DeviceInfoData, USB_DEVICE_PNP_STRINGS & Device_PNP_String)
{
	DWORD buffersize = 0;
	LPTSTR buffer = nullptr;
	while (!SetupDiGetDeviceInstanceId(
		DeviceInfo,
		&DeviceInfoData,
		buffer,
		buffersize,
		&buffersize))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			if (buffer)
				HeapFree(GetProcessHeap(), 0, buffer);
			buffer = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize *= 2);
		}
		else	break;
	}
	if (buffer)
		Device_PNP_String.DeviceId = buffer;
	if (buffer)
	{
		HeapFree(GetProcessHeap(), 0, buffer);
		buffer = nullptr;
	}
	return TRUE;
}

BOOL _EnumUsbController::GetUsbHostControllerBDF(HDEVINFO DeviceInfo, SP_DEVINFO_DATA &DeviceInfoData, USB_HOST_CONTROLLER_BDF & DeviceBDF)
{
	PULONG buffer = nullptr;
	DWORD buffersize = 0;
	DeviceBDF.BusDeviceFunctionValid = FALSE;
	while (!SetupDiGetDeviceRegistryProperty(
		DeviceInfo,
		&DeviceInfoData,
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
				buffer = nullptr;
			}
			buffer = (PULONG)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
		}
		else	break;
	}
	if (buffer)		DeviceBDF.BusNumber = (USHORT)*buffer;
	buffersize = 0;
	while (!SetupDiGetDeviceRegistryProperty(
		DeviceInfo,
		&DeviceInfoData,
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

	if (buffer)
	{
		HeapFree(GetProcessHeap(), 0, buffer);
		buffer = nullptr;
	}
	return DeviceBDF.BusDeviceFunctionValid;
}

BOOL _EnumUsbController::GetDeviceProperty(HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData, DWORD Property, _Outptr_ std::wstring&    ppBuffer)
{
	DWORD buffersize = 0;
	LPTSTR buffer = nullptr;

	buffersize = 0;
	while (!SetupDiGetDeviceRegistryProperty(
		DeviceInfoSet,
		DeviceInfoData,
		Property,
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
	if (buffer)		ppBuffer = buffer;

	if (buffer)
		HeapFree(GetProcessHeap(), 0, buffer);
	return TRUE;
}
