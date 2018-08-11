#include "UsbHub.h"

_EnumUsbHub::_EnumUsbHub()
{
	EnumUSBDevice(GUID_DEVINTERFACE_USB_DEVICE);
	EnumUSBDevice(GUID_DEVINTERFACE_USB_HUB);
}

_EnumUsbHub::~_EnumUsbHub()
{
}

BOOL _EnumUsbHub::Excute()
{
	for (size_t i = 0; i < this->ControllerNum; ++i)
	{
		USB_ROOT_HUB UsbTree_Tmp;
		this->EnumRootUsbHub(this->UsbHostControllerInfo[i].RootHubName, true);

		Device_Hub_Tmp.UsbDeviceInfo = Device_Tmp;
		Root_Hub_Tmp[i].UsbDeviceProperties = UsbHostControllerInfo[i].UsbDeviceProperties;
		UsbTree_Tmp.UsbRootHubInfo = Root_Hub_Tmp[i];
		UsbTree_Tmp.UsbRootControllerInfo = UsbHostControllerInfo[i];
		UsbTree_Tmp.UsbDeviceInfo = Device_Hub_Tmp;
		Usb_Tree.emplace_back(UsbTree_Tmp);
		Device_Hub_Tmp.Clear();
		Device_Tmp.clear();
	}
	return TRUE;
}

BOOL _EnumUsbHub::EnumRootUsbHub(const wstring HubName, bool IsRootHub)
{
	HANDLE hHubDevice = INVALID_HANDLE_VALUE;
	DWORD nBytes = 0;
	BOOL success = FALSE;
	PUSB_NODE_INFORMATION hubInfo = new USB_NODE_INFORMATION;
	PUSB_HUB_INFORMATION_EX hubInfoEx = new USB_HUB_INFORMATION_EX;
	PUSB_HUB_CAPABILITIES_EX hubCapabilityEx = new USB_HUB_CAPABILITIES_EX;
	USBROOTHUBINFO UsbRootHubInfo;
	wstring FullHubName = _T("\\\\.\\");
	FullHubName.append(HubName);
	UsbRootHubInfo.Clear();

	hHubDevice = ::CreateFile(
		FullHubName.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (INVALID_HANDLE_VALUE == hHubDevice)
	{
		return FALSE;
	}
	success = ::DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_NODE_INFORMATION,
		hubInfo,
		sizeof(USB_NODE_INFORMATION),
		hubInfo,
		sizeof(USB_NODE_INFORMATION),
		&nBytes,
		NULL);
	if (!success)
	{
		success = GetLastError();
		goto EnumerateHubError;
	}

	success = ::DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_HUB_INFORMATION_EX,
		hubInfoEx,
		sizeof(USB_HUB_INFORMATION_EX),
		hubInfoEx,
		sizeof(USB_HUB_INFORMATION_EX),
		&nBytes,
		NULL);
	if (!success || (nBytes < sizeof(USB_HUB_INFORMATION_EX)))
	{
		SafeDeleteData(hubInfoEx);
	}

	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_HUB_CAPABILITIES_EX,
		hubCapabilityEx,
		sizeof(USB_HUB_CAPABILITIES_EX),
		hubCapabilityEx,
		sizeof(USB_HUB_CAPABILITIES_EX),
		&nBytes,
		NULL);
	if (!success || (nBytes < sizeof(USB_HUB_CAPABILITIES_EX)))
	{
		SafeDeleteData(hubCapabilityEx);
	}

	this->EnumUsbPort(hHubDevice, hubInfo->u.HubInformation.HubDescriptor.bNumberOfPorts, true);
	UsbRootHubInfo.DeviceInfoType = RootHubInfo;
	UsbRootHubInfo.HubName = HubName;
	UsbRootHubInfo.HubInfo = *hubInfo;
	UsbRootHubInfo.HubInfoEx = *hubInfoEx;
	UsbRootHubInfo.HubCapabilityEx = *hubCapabilityEx;
	Root_Hub_Tmp.emplace_back(UsbRootHubInfo);
EnumerateHubError:
	SafeDeleteData(hubInfo);
	SafeDeleteData(hubInfoEx);
	SafeDeleteData(hubCapabilityEx);
	::CloseHandle(hHubDevice);
	return success;
}

BOOL _EnumUsbHub::EnumExternalUsbHub(
	_In_	 const wstring HubName,
	_In_opt_ USB_NODE_CONNECTION_INFORMATION_EX_V1     ConnectionInfo,
	_In_opt_ USB_NODE_CONNECTION_INFORMATION_EX_V2	   ConnectionInfoV2,
	_In_opt_ USB_PIPE_INFO							   Usb_Pipe_Info[30],
	_In_opt_ USB_PORT_CONNECTOR_PROPERTIES_V2		   PortConnectorProps,
	_In_opt_ USB_CONFIGURATION_DESCRIPTOR			   ConfigDesc,
	_In_opt_ USB_BOS_DESCRIPTOR                        BosDesc,
	_In_opt_ vector<wstring>					       StringDescs,
	_In_opt_ vector<USB_ALL_DESCRIPTOR>				   AllDescs,
	_In_opt_ USB_DEVICE_PNP_STRINGS                    DevProps, 
	_In_	 bool IsRootHub)
{
	HANDLE hHubDevice = INVALID_HANDLE_VALUE;
	DWORD nBytes = 0;
	BOOL success = FALSE;
	PUSB_NODE_INFORMATION hubInfo = new USB_NODE_INFORMATION;
	PUSB_HUB_INFORMATION_EX hubInfoEx = new USB_HUB_INFORMATION_EX;
	PUSB_HUB_CAPABILITIES_EX hubCapabilityEx = new USB_HUB_CAPABILITIES_EX;
	USBEXTERNALHUBINFO UsbExternalHubInfo;
	wstring FullHubName = _T("\\\\.\\");
	FullHubName.append(HubName);
	UsbExternalHubInfo.Clear();

	hHubDevice = ::CreateFile(
		FullHubName.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (INVALID_HANDLE_VALUE == hHubDevice)
	{
		return FALSE;
	}
	success = ::DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_NODE_INFORMATION,
		hubInfo,
		sizeof(USB_NODE_INFORMATION),
		hubInfo,
		sizeof(USB_NODE_INFORMATION),
		&nBytes,
		NULL);
	if (!success)
	{
		success = GetLastError();
		goto EnumerateHubError;
	}

	success = ::DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_HUB_INFORMATION_EX,
		hubInfoEx,
		sizeof(USB_HUB_INFORMATION_EX),
		hubInfoEx,
		sizeof(USB_HUB_INFORMATION_EX),
		&nBytes,
		NULL);
	if (!success || (nBytes < sizeof(USB_HUB_INFORMATION_EX)))
	{
		SafeDeleteData(hubInfoEx);
	}

	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_HUB_CAPABILITIES_EX,
		hubCapabilityEx,
		sizeof(USB_HUB_CAPABILITIES_EX),
		hubCapabilityEx,
		sizeof(USB_HUB_CAPABILITIES_EX),
		&nBytes,
		NULL);
	if (!success || (nBytes < sizeof(USB_HUB_CAPABILITIES_EX)))
	{
		SafeDeleteData(hubCapabilityEx);
	}

	this->EnumUsbPort(hHubDevice, hubInfo->u.HubInformation.HubDescriptor.bNumberOfPorts, false);
	UsbExternalHubInfo.DeviceInfoType = ExternalHubInfo;
	UsbExternalHubInfo.HubName = HubName;
	UsbExternalHubInfo.HubInfo = *hubInfo;
	UsbExternalHubInfo.HubInfoEx = *hubInfoEx;
	UsbExternalHubInfo.HubCapabilityEx = *hubCapabilityEx;
	UsbExternalHubInfo.ConnectionInfo = ConnectionInfo;
	UsbExternalHubInfo.ConnectionInfoV2 = ConnectionInfoV2;
	for (size_t i = 0; i < 30; i++)
	{
		UsbExternalHubInfo.Usb_Pipe_Info[i] = Usb_Pipe_Info[i];
	}
	UsbExternalHubInfo.PortConnectorProps = PortConnectorProps;
	UsbExternalHubInfo.ConfigDesc = ConfigDesc;
	UsbExternalHubInfo.BosDesc = BosDesc;
	UsbExternalHubInfo.UsbDeviceProperties = DevProps; 
	UsbExternalHubInfo.StringDescs = StringDescs;
	UsbExternalHubInfo.AllDescs = AllDescs;
	Hub_tmp.UsbExternalHubInfo.emplace_back(UsbExternalHubInfo);
	Device_Hub_Tmp.UsbHubInfo.emplace_back(Hub_tmp);
	Hub_tmp.Clear();
EnumerateHubError:
	SafeDeleteData(hubInfo);
	SafeDeleteData(hubInfoEx);
	SafeDeleteData(hubCapabilityEx);
	::CloseHandle(hHubDevice);
	return success;
}

BOOL _EnumUsbHub::EnumUsbPort(HANDLE hHubDevice, ULONG PortNums, bool IsRootHub)
{
	BOOL success = FALSE;
	DWORD nBytes = 0, ConfigDataBytes = 0;
	DEVICE_INSTANCE DeviceInst;
	USB_HOST_CONTROLLER_BDF DeviceBDF;
	USB_DEVICE_PNP_STRINGS DevProps;
	DEVICE_INFO_NODE Device_Node;

	USB_NODE_CONNECTION_INFORMATION_EX_V1	  ConnectionInfo;
	USB_NODE_CONNECTION_INFORMATION_EX_V2	  ConnectionInfoV2;
	USB_PIPE_INFO							  Usb_Pipe_Info[30];
	USB_PORT_CONNECTOR_PROPERTIES_V2          portConnectorProps;
	USB_CONFIGURATION_DESCRIPTOR			  configDesc;
	USB_INTERFACE_DESCRIPTOR				  InterfaceDesc;
	USB_BOS_DESCRIPTOR		                  bosDesc;
	vector<wstring>					          stringDescs;
	vector<USB_ALL_DESCRIPTOR>				  AllDescs;
	USBDEVICEINFO                             info;
	BOOL									  IsGetPortConnectorProps = FALSE;
	BOOL									  IsGetConnectionInfo = FALSE;
	BOOL									  IsDriverNametoDevInstance = FALSE;
	BOOL									  IsFindMatchingDeviceNodeForDriverName = FALSE;
	BOOL									  IsGetConfigDescriptor = FALSE;
	BOOL									  IsGetInterFaceDescriptor = FALSE;
	BOOL									  IsGetBOSDescriptor = FALSE;
	BOOL									  IsStringDescriptors = FALSE;
	for (ULONG Index = 1; Index <= PortNums; ++Index)
	{
		//Initialization
		::ZeroMemory(&portConnectorProps, sizeof(USB_PORT_CONNECTOR_PROPERTIES_V2) - sizeof(wstring));
		portConnectorProps.CompanionHubSymbolicLinkName.clear();
		::ZeroMemory(&configDesc, sizeof(USB_CONFIGURATION_DESCRIPTOR));
		::ZeroMemory(&InterfaceDesc, sizeof(USB_INTERFACE_DESCRIPTOR));
		::ZeroMemory(&bosDesc, sizeof(USB_BOS_DESCRIPTOR));
		info.Clear();
		AllDescs.clear();
		stringDescs.clear();
		IsGetPortConnectorProps = FALSE;
		IsGetConnectionInfo = FALSE;
		IsDriverNametoDevInstance = FALSE;
		IsFindMatchingDeviceNodeForDriverName = FALSE;
		IsGetConfigDescriptor = FALSE;
		IsGetBOSDescriptor = FALSE;
		IsStringDescriptors = FALSE;
		for (USHORT j = 0; j < 30; ++j)
		{
			::ZeroMemory(&Usb_Pipe_Info[j], sizeof(USB_PIPE_INFO));
		}
		//Get Information
		success = this->GetPortConnectorProps(hHubDevice, Index, portConnectorProps);
		if (success == TRUE)	IsGetPortConnectorProps = TRUE;
		success = this->GetConnectionInfo(hHubDevice, Index, ConnectionInfo, Usb_Pipe_Info, ConnectionInfoV2);
		if (success == FALSE)	continue;
		else	IsGetConnectionInfo = TRUE;
		if (ConnectionInfo.ConnectionStatus != NoDeviceConnected)
		{
			wstring DriverKeyName = _T("");
			if (this->GetDriverKeyName(hHubDevice, Index, DriverKeyName) == TRUE)
			{
				success = this->DriverNametoDevInstance(DriverKeyName, DeviceInst, DevProps, DeviceBDF);
				if (success == TRUE) IsDriverNametoDevInstance = TRUE;
				success = this->FindMatchingDeviceNodeForDriverName(DriverKeyName, Device_Node, ConnectionInfo.DeviceIsHub);
				if (success == TRUE) IsFindMatchingDeviceNodeForDriverName = TRUE;
			}
		}

		if (ConnectionInfo.ConnectionStatus == DeviceConnected)
		{
			PUCHAR ConfigData;
			success = this->GetConfigDescriptor(hHubDevice, Index, 0, ConfigData, ConfigDataBytes);
			if (success == TRUE)
			{
				IsGetConfigDescriptor = TRUE;
				configDesc = *(PUSB_CONFIGURATION_DESCRIPTOR)ConfigData;
				if (ConnectionInfo.DeviceDescriptor.bcdUSB > 0x200)
				{
					success = this->GetBOSDescriptor(hHubDevice, Index, bosDesc);
					if (success == TRUE) IsGetBOSDescriptor = TRUE;
				}
				if (AreThereStringDescriptors(ConnectionInfo.DeviceDescriptor, ConfigData, ConfigDataBytes))
				{
					success = this->GetAllStringDescriptors(hHubDevice, Index, ConnectionInfo.DeviceDescriptor, ConfigData, ConfigDataBytes, stringDescs, AllDescs);
					if (success)	IsStringDescriptors = TRUE;
				}
				this->SafeDeleteArray(ConfigData);
			}
// 			success = this->GetInterFaceDescriptor( hHubDevice, Index, 0, 0x0409, InterfaceDesc);
// 			if (success == TRUE)
// 			{
// 				OutputDebugPrintf("直接找到InterFace Descriptor！\n");
// 			}
		}

		if (ConnectionInfo.DeviceIsHub)
		{
			wstring extHubName = _T("");
			if (this->GetExternalHubName(hHubDevice, Index, extHubName))
			{
				this->EnumExternalUsbHub(extHubName, ConnectionInfo, ConnectionInfoV2, Usb_Pipe_Info, portConnectorProps, configDesc, bosDesc, stringDescs, AllDescs, DevProps, false);
			}
		}
		else
		{
			info.DeviceInfoType = DEVICEInfo;
			if(IsGetBOSDescriptor)	info.BosDesc = bosDesc;
			if (IsGetConfigDescriptor)	info.ConfigDesc = configDesc;
			if (IsGetConnectionInfo)
			{
				info.ConnectionInfo = ConnectionInfo;
				info.ConnectionInfoV2 = ConnectionInfoV2;
				for (size_t i = 0; i < 30; i++)
				{
					info.Usb_Pipe_Info[i] = Usb_Pipe_Info[i];
				}
			}
			if (IsFindMatchingDeviceNodeForDriverName)	info.DeviceInfoNode = Device_Node;
			if (IsGetPortConnectorProps)	info.PortConnectorProps = portConnectorProps;
			if (IsDriverNametoDevInstance)	info.UsbDeviceProperties = DevProps;
			if (IsStringDescriptors) { info.StringDescs = stringDescs; info.AllDescs = AllDescs; }

			if (IsRootHub)
			{
				Device_Tmp.emplace_back(info);
			}
			else
			{
				Hub_tmp.UsbDeviceInfo.emplace_back(info);
			}
		}
	}
	return TRUE;
}

BOOL _EnumUsbHub::EnumUSBDevice(const GUID Guid)
{
	HDEVINFO hDevInfo;	//包含了设备的ClassGUID和设备实例句柄，大体上相当于设备信息元素(无device interface)
	DWORD i;

	//ZeroMemory(&tmp, sizeof(DEVICE_INFO_NODE));
	//创建一个设备信息集(device information set),其中包含了我们请求的设备信息元素(device information element)
	//该方法只用于获取本机设备，远程计算机上的设备需要使用SetupDiGetClassDevsEx
	//成功则返回一个设备信息集的Handle，失败则返回INVALID_HANDLE_VALUE

	hDevInfo = SetupDiGetClassDevs(
		(LPGUID)&Guid,  // 指定一个GUID(for a device setup class or a device interface class)用以过滤元素
		NULL,      // Enumerator 可为NULL
		NULL,      // hwndParent 可为NULL
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE); //列表的过滤选项，当为DIGCF_ALLCLASSES时，参数1指定的guid将不再起作用

	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	SP_DEVINFO_DATA DeviceInfoData;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	//枚举设备信息列表中的元素，将元素以SP_DEVINFO_DATA结构体的方式输出
	for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
	{
		DWORD DataT;
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;
		DEVICE_INFO_NODE tmp;
		// 获取详细信息
		tmp.DeviceInfo = hDevInfo;
		tmp.DeviceInfoData = DeviceInfoData;
		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_DEVICEDESC,   //属性标识，用以指示buffer输出的是哪个属性值，在此是要求输出设备描述字符串
			&DataT,
			(PBYTE)buffer,      //输出值
			buffersize,
			&buffersize))       //所需的buff空间
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				// 内存不足，则重新分配buff空间
				if (buffer)
					HeapFree(GetProcessHeap(), 0, buffer);

				buffer = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			}
			else
				break;
		}
		tmp.DeviceDescName = buffer;
		tmp.DeviceDescNameLength = buffersize;
		if (buffer)
		{
			HeapFree(GetProcessHeap(), 0, buffer);
			buffer = NULL;
		}
		buffersize = 0;

		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_DRIVER,   //属性标识，用以指示buffer输出的是哪个属性值，在此是要求输出设备描述字符串
			&DataT,
			(PBYTE)buffer,      //输出值
			buffersize,
			&buffersize))       //所需的buff空间
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				// 内存不足，则重新分配buff空间
				if (buffer)
					HeapFree(GetProcessHeap(), 0, buffer);

				buffer = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buffersize);
			}
			else
				break;
		}
		tmp.DeviceDriverName = buffer;
		tmp.DeviceDriverNameLength = buffersize;
		SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
		DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		for (USHORT j = 0; SetupDiEnumDeviceInterfaces(hDevInfo, &DeviceInfoData, (LPGUID)&Guid, j, &DeviceInterfaceData); ++j)
		{
			DWORD bufsize = 0;
			PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceDetailData = NULL;
			while (!SetupDiGetDeviceInterfaceDetail(
				hDevInfo,
				&DeviceInterfaceData,
				DeviceDetailData,
				bufsize,
				&bufsize,
				NULL))
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					// 内存不足，则重新分配buff空间
					if (DeviceDetailData)
						HeapFree(GetProcessHeap(), 0, DeviceDetailData);
					DeviceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufsize);
					DeviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
				}
				else
					break;
			}
			memcpy_s(&tmp.DeviceInterfaceData, sizeof(tmp.DeviceInterfaceData), &DeviceInterfaceData, sizeof(DeviceInterfaceData));
			tmp.DevicePath = DeviceDetailData->DevicePath;
			if (DeviceDetailData)
			{
				HeapFree(GetProcessHeap(), 0, DeviceDetailData);
				DeviceDetailData = NULL;
			}
		}
		if (GUID_DEVINTERFACE_USB_DEVICE == Guid)
		{
			Usb_Device_info.emplace_back(tmp);
		}
		if (GUID_DEVINTERFACE_USB_HUB == Guid)
		{
			Usb_Hub_info.emplace_back(tmp);
		}

		if (buffer)
		{
			HeapFree(GetProcessHeap(), 0, buffer);
			buffer = NULL;
		}
	}

	if (GetLastError() != NO_ERROR
		&& GetLastError() != ERROR_NO_MORE_ITEMS)
	{
		SetupDiDestroyDeviceInfoList(hDevInfo);
		return FALSE;
	}
	// 释放
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return TRUE;
}

BOOL _EnumUsbHub::GetExternalHubName(HANDLE Hub, ULONG ConnectionIndex, wstring & ExternalHubName)
{
	BOOL                        success = 0;
	ULONG                       nBytes = 0;
	USB_NODE_CONNECTION_NAME    extHubName;
	PUSB_NODE_CONNECTION_NAME   extHubNameW = NULL;
	PCHAR                       extHubNameA = NULL;

	// Get the length of the name of the external hub attached to the
	// specified port.
	//
	extHubName.ConnectionIndex = ConnectionIndex;

	success = DeviceIoControl(Hub,
		IOCTL_USB_GET_NODE_CONNECTION_NAME,
		&extHubName,
		sizeof(extHubName),
		&extHubName,
		sizeof(extHubName),
		&nBytes,
		NULL);

	if (!success)
	{
		goto GetExternalHubNameError;
	}

	// Allocate space to hold the external hub name
	//
	nBytes = extHubName.ActualLength;

	if (nBytes <= sizeof(extHubName))
	{
		goto GetExternalHubNameError;
	}

	extHubNameW = (PUSB_NODE_CONNECTION_NAME)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nBytes);

	if (extHubNameW == NULL)
	{
		goto GetExternalHubNameError;
	}

	// Get the name of the external hub attached to the specified port
	//
	extHubNameW->ConnectionIndex = ConnectionIndex;

	success = DeviceIoControl(Hub,
		IOCTL_USB_GET_NODE_CONNECTION_NAME,
		extHubNameW,
		nBytes,
		extHubNameW,
		nBytes,
		&nBytes,
		NULL);

	if (!success)
	{
		goto GetExternalHubNameError;
	}

	// Convert the External Hub name
	//
	this->Wchar_tToString(ExternalHubName, extHubNameW->NodeName);

	// All done, free the uncoverted external hub name and return the
	// converted external hub name
	//
	if (extHubNameW)
	{
		HeapFree(GetProcessHeap(), 0, extHubNameW);
		extHubNameW = NULL;
	}

	return TRUE;


GetExternalHubNameError:
	// There was an error, free anything that was allocated
	if (extHubNameW)
	{
		HeapFree(GetProcessHeap(), 0, extHubNameW);
		extHubNameW = NULL;
	}
	return FALSE;
}

BOOL _EnumUsbHub::GetDriverKeyName(HANDLE Hub, ULONG ConnectionIndex, wstring & DriverKeyName)
{
	BOOL                                success = 0;
	ULONG                               nBytes = 0;
	USB_NODE_CONNECTION_DRIVERKEY_NAME  driverKeyName;
	PUSB_NODE_CONNECTION_DRIVERKEY_NAME driverKeyNameW = NULL;
	PCHAR                               driverKeyNameA = NULL;

	// Get the length of the name of the driver key of the device attached to
	// the specified port.
	//
	driverKeyName.ConnectionIndex = ConnectionIndex;

	success = DeviceIoControl(Hub,
		IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
		&driverKeyName,
		sizeof(driverKeyName),
		&driverKeyName,
		sizeof(driverKeyName),
		&nBytes,
		NULL);

	if (!success)
	{
		goto GetDriverKeyNameError;
	}

	// Allocate space to hold the driver key name
	//
	nBytes = driverKeyName.ActualLength;

	if (nBytes <= sizeof(driverKeyName))
	{
		goto GetDriverKeyNameError;
	}

	driverKeyNameW = (PUSB_NODE_CONNECTION_DRIVERKEY_NAME)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nBytes);
	if (driverKeyNameW == NULL)
	{
		goto GetDriverKeyNameError;
	}

	// Get the name of the driver key of the device attached to
	// the specified port.
	//
	driverKeyNameW->ConnectionIndex = ConnectionIndex;

	success = DeviceIoControl(Hub,
		IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
		driverKeyNameW,
		nBytes,
		driverKeyNameW,
		nBytes,
		&nBytes,
		NULL);

	if (!success)
	{
		goto GetDriverKeyNameError;
	}

	// Convert the driver key name
	//
	this->Wchar_tToString(DriverKeyName, driverKeyNameW->DriverKeyName);

	// All done, free the uncoverted driver key name and return the
	// converted driver key name
	//
	HeapFree(GetProcessHeap(), 0, driverKeyNameW);
	driverKeyNameW = NULL;
	return TRUE;


GetDriverKeyNameError:
	// There was an error, free anything that was allocated
	//
	if (driverKeyNameW != NULL)
	{
		HeapFree(GetProcessHeap(), 0, driverKeyNameW);
		driverKeyNameW = NULL;
	}

	return FALSE;
}

BOOL _EnumUsbHub::GetPortConnectorProps(HANDLE hHubDevice, ULONG Index, USB_PORT_CONNECTOR_PROPERTIES_V2& PortConnectorProps)
{
	BOOL success = FALSE;
	DWORD nBytes = 0;
	PUSB_PORT_CONNECTOR_PROPERTIES         pPortConnectorProps = NULL;
	USB_PORT_CONNECTOR_PROPERTIES          portConnectorProps;
	ZeroMemory(&portConnectorProps, sizeof(portConnectorProps));
	portConnectorProps.ConnectionIndex = Index;
	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_PORT_CONNECTOR_PROPERTIES,
		&portConnectorProps,
		sizeof(USB_PORT_CONNECTOR_PROPERTIES),
		&portConnectorProps,
		sizeof(USB_PORT_CONNECTOR_PROPERTIES),
		&nBytes,
		NULL);
	if (!success)
	{
		OutputDebugPrintf("IOCTL_USB_GET_PORT_CONNECTOR_PROPERTIES ERROR Get Length, ERROR Code: %X\n", GetLastError());
		return GetLastError();
	}

	nBytes = portConnectorProps.ActualLength;

	if (nBytes < sizeof(USB_PORT_CONNECTOR_PROPERTIES))
	{
		OutputDebugPrintf("Incorrect length received by IOCTL USB GET PORT CONNECTOR PROPERTIES\n");
		return FALSE;
	}

	pPortConnectorProps = (PUSB_PORT_CONNECTOR_PROPERTIES)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nBytes);
	pPortConnectorProps->ConnectionIndex = Index;
	success = ::DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_PORT_CONNECTOR_PROPERTIES,
		pPortConnectorProps,
		nBytes,
		pPortConnectorProps,
		nBytes,
		&nBytes,
		NULL);
	if (!success || nBytes < portConnectorProps.ActualLength)
	{
		OutputDebugPrintf("IOCTL_USB_GET_PORT_CONNECTOR_PROPERTIES ERROR Get Info, ERROR Code: %X\n", GetLastError());
		if (pPortConnectorProps)
		{
			HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pPortConnectorProps);
			pPortConnectorProps = NULL;
		}
		return GetLastError();
	}
	PortConnectorProps.ActualLength = pPortConnectorProps->ActualLength;
	PortConnectorProps.UsbPortProperties = pPortConnectorProps->UsbPortProperties;
	PortConnectorProps.CompanionIndex = pPortConnectorProps->CompanionIndex;
	PortConnectorProps.CompanionPortNumber = pPortConnectorProps->CompanionPortNumber;
	PortConnectorProps.ConnectionIndex = pPortConnectorProps->ConnectionIndex;
	this->Wchar_tToString(PortConnectorProps.CompanionHubSymbolicLinkName ,pPortConnectorProps->CompanionHubSymbolicLinkName);
	if (pPortConnectorProps)
	{
		HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pPortConnectorProps);
		pPortConnectorProps = NULL;
	}
	return TRUE;
}

BOOL _EnumUsbHub::GetConnectionInfo(HANDLE hHubDevice, ULONG Index, USB_NODE_CONNECTION_INFORMATION_EX_V1 & ConnectionInfo, PUSB_PIPE_INFO Usb_Pipe_Info, USB_NODE_CONNECTION_INFORMATION_EX_V2 & ConnectionInfoV2)
{
	BOOL success = FALSE;
	DWORD nBytes = 0, nBytesEx = 0;
	PUSB_NODE_CONNECTION_INFORMATION_EX		  connectionInfoEx = NULL;
	PUSB_NODE_CONNECTION_INFORMATION_EX_V2	  connectionInfoExV2 = NULL;

	connectionInfoExV2 = new USB_NODE_CONNECTION_INFORMATION_EX_V2;
	ZeroMemory(connectionInfoExV2, sizeof(USB_NODE_CONNECTION_INFORMATION_EX_V2));
	connectionInfoExV2->ConnectionIndex = Index;
	connectionInfoExV2->Length = sizeof(USB_NODE_CONNECTION_INFORMATION_EX_V2);
	connectionInfoExV2->SupportedUsbProtocols.Usb300 = 1;
	success = ::DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX_V2,
		connectionInfoExV2,
		sizeof(USB_NODE_CONNECTION_INFORMATION_EX_V2),
		connectionInfoExV2,
		sizeof(USB_NODE_CONNECTION_INFORMATION_EX_V2),
		&nBytes,
		NULL);
	if (!success || nBytes < sizeof(USB_NODE_CONNECTION_INFORMATION_EX_V2))
	{
		this->SafeDeleteData(connectionInfoExV2);
	}
	else
	{
		ConnectionInfoV2 = *connectionInfoExV2;
	}


	nBytesEx = sizeof(USB_NODE_CONNECTION_INFORMATION_EX) + (sizeof(USB_PIPE_INFO) * 30);
	connectionInfoEx = (PUSB_NODE_CONNECTION_INFORMATION_EX)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nBytesEx);

	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX,
		connectionInfoEx,
		nBytesEx,
		connectionInfoEx,
		nBytesEx,
		&nBytesEx,
		NULL);
	if (success)
	{
		//
		// Since the USB_NODE_CONNECTION_INFORMATION_EX is used to display
		// the device speed, but the hub driver doesn't support indication
		// of superspeed, we overwrite the value if the super speed
		// data structures are available and indicate the device is operating
		// at SuperSpeed.
		// 

		if (connectionInfoEx->Speed == UsbHighSpeed
			&& connectionInfoExV2 != NULL
			&& (connectionInfoExV2->Flags.DeviceIsOperatingAtSuperSpeedOrHigher ||
				connectionInfoExV2->Flags.DeviceIsOperatingAtSuperSpeedPlusOrHigher))
		{
			connectionInfoEx->Speed = UsbSuperSpeed;
		}
	}
	else
	{
		PUSB_NODE_CONNECTION_INFORMATION    connectionInfo = NULL;
		nBytes = sizeof(USB_NODE_CONNECTION_INFORMATION) + sizeof(USB_PIPE_INFO) * 30;
		connectionInfo = (PUSB_NODE_CONNECTION_INFORMATION)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nBytes);

// 		if (connectionInfo)
// 		{
// 			if (connectionInfoEx)
// 			{
// 				HeapFree(GetProcessHeap(), 0, connectionInfoEx);
// 				connectionInfoEx = NULL;
// 			}
// 			this->SafeDeleteData(connectionInfoExV2);
// 			return FALSE;
// 		}
		connectionInfo->ConnectionIndex = Index;
		success = DeviceIoControl(hHubDevice,
			IOCTL_USB_GET_NODE_CONNECTION_INFORMATION,
			connectionInfo,
			nBytes,
			connectionInfo,
			nBytes,
			&nBytes,
			NULL);
		if (!success)
		{
			if (connectionInfo)
			{
				HeapFree(GetProcessHeap(), 0, connectionInfo);
				connectionInfo = NULL;
			}
			if (connectionInfoEx)
			{
				HeapFree(GetProcessHeap(), 0, connectionInfoEx);
				connectionInfoEx = NULL;
			}
			this->SafeDeleteData(connectionInfoExV2);
			return FALSE;
		}
		connectionInfoEx->ConnectionIndex = connectionInfo->ConnectionIndex;
		connectionInfoEx->DeviceDescriptor = connectionInfo->DeviceDescriptor;
		connectionInfoEx->CurrentConfigurationValue = connectionInfo->CurrentConfigurationValue;
		connectionInfoEx->Speed = connectionInfo->LowSpeed ? UsbLowSpeed : UsbFullSpeed;
		connectionInfoEx->DeviceIsHub = connectionInfo->DeviceIsHub;
		connectionInfoEx->DeviceAddress = connectionInfo->DeviceAddress;
		connectionInfoEx->NumberOfOpenPipes = connectionInfo->NumberOfOpenPipes;
		connectionInfoEx->ConnectionStatus = connectionInfo->ConnectionStatus;

		memcpy(&connectionInfoEx->PipeList[0],
			&connectionInfo->PipeList[0],
			sizeof(USB_PIPE_INFO) * 30);
		if (connectionInfo)
		{
			HeapFree(GetProcessHeap(), 0, connectionInfo);
			connectionInfo = NULL;
		}
		ConnectionInfo.ConnectionIndex = connectionInfoEx->ConnectionIndex;
		ConnectionInfo.ConnectionStatus = connectionInfoEx->ConnectionStatus;
		ConnectionInfo.CurrentConfigurationValue = connectionInfoEx->CurrentConfigurationValue;
		ConnectionInfo.DeviceAddress = connectionInfoEx->DeviceAddress;
		ConnectionInfo.DeviceDescriptor = connectionInfoEx->DeviceDescriptor;
		ConnectionInfo.DeviceIsHub = connectionInfoEx->DeviceIsHub;
		ConnectionInfo.NumberOfOpenPipes = connectionInfoEx->NumberOfOpenPipes;
		::CopyMemory(Usb_Pipe_Info, &connectionInfoEx->PipeList[0], sizeof(USB_PIPE_INFO) * 30);
	}

//	ConnectExit
	this->SafeDeleteData(connectionInfoExV2);
	if (connectionInfoEx)
	{
		HeapFree(GetProcessHeap(), 0, connectionInfoEx);
		connectionInfoEx = NULL;
	}
	return TRUE;
}

BOOL _EnumUsbHub::FindMatchingDeviceNodeForDriverName(wstring DriverKeyName, DEVICE_INFO_NODE & DeviceInfoNode, BOOLEAN IsHub)
{
	BOOL Success = FALSE;
	if (IsHub)
	{
		if (Usb_Hub_info.empty())	return FALSE;
		for (USHORT i = 0; i < Usb_Hub_info.size(); ++i)
		{
			if (DriverKeyName.compare(this->Usb_Hub_info[i].DeviceDriverName) == 0)
			{
				DeviceInfoNode = Usb_Hub_info[i];
				Success = TRUE;
				break;
			}
		}
	}
	else
	{
		if (this->Usb_Device_info.empty())	return FALSE;
		for (USHORT i = 0; i < Usb_Hub_info.size(); ++i)
		{
			if (DriverKeyName.compare(this->Usb_Device_info[i].DeviceDriverName) == 0)
			{
				DeviceInfoNode = Usb_Device_info[i];
				Success = TRUE;
				break;
			}
		}
	}
	return Success;
}

BOOL _EnumUsbHub::GetConfigDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, UCHAR DescriptorIndex, PUCHAR& ConfigDesc, DWORD& ConfigDescBytes)
{
	BOOL    success = 0;
	ULONG   nBytes = 0;
	ULONG   nBytesReturned = 0;

	UCHAR   configDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST) +
		sizeof(USB_CONFIGURATION_DESCRIPTOR)];

	PUSB_DESCRIPTOR_REQUEST         configDescReq = NULL;
	PUSB_CONFIGURATION_DESCRIPTOR   configDesc = NULL;


	// Request the Configuration Descriptor the first time using our
	// local buffer, which is just big enough for the Cofiguration
	// Descriptor itself.
	//
	nBytes = sizeof(configDescReqBuf);

	configDescReq = (PUSB_DESCRIPTOR_REQUEST)configDescReqBuf;
	configDesc = (PUSB_CONFIGURATION_DESCRIPTOR)(configDescReq + 1);

	// Zero fill the entire request structure
	//
	memset(configDescReq, 0, nBytes);

	// Indicate the port from which the descriptor will be requested
	//
	configDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	configDescReq->SetupPacket.wValue = (USB_CONFIGURATION_DESCRIPTOR_TYPE << 8)
		| DescriptorIndex;

	configDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//
	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		configDescReq,
		nBytes,
		configDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	if (!success)
	{
		return GetLastError();
	}

	if (nBytes != nBytesReturned)
	{
		return GetLastError();
	}

	if (configDesc->wTotalLength < sizeof(USB_CONFIGURATION_DESCRIPTOR))
	{
		return GetLastError();
	}

	// Now request the entire Configuration Descriptor using a dynamically
	// allocated buffer which is sized big enough to hold the entire descriptor
	//
	nBytes = sizeof(USB_DESCRIPTOR_REQUEST) + configDesc->wTotalLength;

	configDescReq = (PUSB_DESCRIPTOR_REQUEST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nBytes);
	if (configDescReq == NULL)
	{
		return GetLastError();
	}

	configDesc = (PUSB_CONFIGURATION_DESCRIPTOR)(configDescReq + 1);

	// Indicate the port from which the descriptor will be requested
	//
	configDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	configDescReq->SetupPacket.wValue = (USB_CONFIGURATION_DESCRIPTOR_TYPE << 8)
		| DescriptorIndex;
	configDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//

	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		configDescReq,
		nBytes,
		configDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	if (!success)
	{
		HeapFree(GetProcessHeap(), 0, configDescReq);
		configDescReq = NULL;
		return GetLastError();
	}

	if (nBytes != nBytesReturned)
	{
		HeapFree(GetProcessHeap(), 0, configDescReq);
		configDescReq = NULL;
		return GetLastError();
	}

	if (configDesc->wTotalLength != (nBytes - sizeof(USB_DESCRIPTOR_REQUEST)))
	{
		HeapFree(GetProcessHeap(), 0, configDescReq);
		configDescReq = NULL;
		return GetLastError();
	}
	ConfigDescBytes = nBytes - sizeof(USB_DESCRIPTOR_REQUEST);
	ConfigDesc = new UCHAR[ConfigDescBytes];

	::CopyMemory(ConfigDesc, configDescReq->Data, ConfigDescBytes);
	if (configDescReq)
	{
		HeapFree(GetProcessHeap(), 0, configDescReq);
		configDescReq = NULL;
	}
	return TRUE;
}

BOOL _EnumUsbHub::GetBOSDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, USB_BOS_DESCRIPTOR&  BOSDesc)
{
	BOOL    success = 0;
	ULONG   nBytes = 0;
	ULONG   nBytesReturned = 0;

	UCHAR   bosDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST) +
		sizeof(USB_BOS_DESCRIPTOR)];

	PUSB_DESCRIPTOR_REQUEST bosDescReq = NULL;
	PUSB_BOS_DESCRIPTOR     bosDesc = NULL;


	// Request the BOS Descriptor the first time using our
	// local buffer, which is just big enough for the BOS
	// Descriptor itself.
	//
	nBytes = sizeof(bosDescReqBuf);

	bosDescReq = (PUSB_DESCRIPTOR_REQUEST)bosDescReqBuf;
	bosDesc = (PUSB_BOS_DESCRIPTOR)(bosDescReq + 1);

	// Zero fill the entire request structure
	//
	memset(bosDescReq, 0, nBytes);

	// Indicate the port from which the descriptor will be requested
	//
	bosDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	bosDescReq->SetupPacket.wValue = (USB_BOS_DESCRIPTOR_TYPE << 8);

	bosDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//
	success = ::DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		bosDescReq,
		nBytes,
		bosDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	if (!success)
	{
		return GetLastError();
	}

	if (nBytes != nBytesReturned)
	{
		return GetLastError();
	}

	if (bosDesc->wTotalLength < sizeof(USB_BOS_DESCRIPTOR))
	{
		return GetLastError();
	}

	// Now request the entire BOS Descriptor using a dynamically
	// allocated buffer which is sized big enough to hold the entire descriptor
	//
	nBytes = sizeof(USB_DESCRIPTOR_REQUEST) + bosDesc->wTotalLength;

	bosDescReq = (PUSB_DESCRIPTOR_REQUEST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nBytes);

	if (bosDescReq == NULL)
	{
		return GetLastError();
	}

	bosDesc = (PUSB_BOS_DESCRIPTOR)(bosDescReq + 1);

	// Indicate the port from which the descriptor will be requested
	//
	bosDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	bosDescReq->SetupPacket.wValue = (USB_BOS_DESCRIPTOR_TYPE << 8);

	bosDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//

	success = ::DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		bosDescReq,
		nBytes,
		bosDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	if (!success)
	{
		HeapFree(GetProcessHeap(), 0, bosDescReq);
		bosDescReq = NULL;
		return GetLastError();
	}

	if (nBytes != nBytesReturned)
	{
		HeapFree(GetProcessHeap(), 0, bosDescReq);
		bosDescReq = NULL;
		return GetLastError();
	}

	if (bosDesc->wTotalLength != (nBytes - sizeof(USB_DESCRIPTOR_REQUEST)))
	{
		HeapFree(GetProcessHeap(), 0, bosDescReq);
		bosDescReq = NULL;
		return GetLastError();
	}
	BOSDesc = *bosDesc;

	if (bosDescReq)
	{
		HeapFree(GetProcessHeap(), 0, bosDescReq);
		bosDescReq = NULL;
	}
	return TRUE;
}

//*****************************************************************************
//
// AreThereStringDescriptors()
//
// DeviceDesc - Device Descriptor for which String Descriptors should be
// checked.
//
// ConfigDesc - Configuration Descriptor (also containing Interface Descriptor)
// for which String Descriptors should be checked.
//
//*****************************************************************************

BOOL _EnumUsbHub::AreThereStringDescriptors(
	const USB_DEVICE_DESCRIPTOR          DeviceDesc,
	const PUCHAR						 ConfigDesc,
	const DWORD							 ConfigDescBytes
)
{
	//
	// Check Device Descriptor strings
	//

	if (DeviceDesc.iManufacturer ||
		DeviceDesc.iProduct ||
		DeviceDesc.iSerialNumber
		)
	{
		return TRUE;
	}


	//
	// Check the Configuration and Interface Descriptor strings
	//
	PUCHAR                  descEnd = NULL;
	PUSB_COMMON_DESCRIPTOR  commonDesc = NULL;

	commonDesc = (PUSB_COMMON_DESCRIPTOR)ConfigDesc;

	descEnd = (PUCHAR)commonDesc + ConfigDescBytes - 1;



	while ((PUCHAR)commonDesc + sizeof(USB_COMMON_DESCRIPTOR) < descEnd &&
		(PUCHAR)commonDesc + commonDesc->bLength <= descEnd)
	{
		switch (commonDesc->bDescriptorType)
		{
		case USB_CONFIGURATION_DESCRIPTOR_TYPE:
		case USB_OTHER_SPEED_CONFIGURATION_DESCRIPTOR_TYPE:
			if (commonDesc->bLength != sizeof(USB_CONFIGURATION_DESCRIPTOR))
			{
				break;
			}
			if (((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration)
			{
				return TRUE;
			}
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;

		case USB_INTERFACE_DESCRIPTOR_TYPE:
			if (commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR) &&
				commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR2))
			{
				break;
			}
			if (((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface)
			{
				return TRUE;
			}
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;

		default:
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;
		}
		break;
	}
	return TRUE;
}

BOOL _EnumUsbHub::GetStringDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, UCHAR DescriptorIndex, USHORT LanguageID, wstring & Descriptor)
{
	BOOL    success = 0;
	ULONG   nBytes = 0;
	ULONG   nBytesReturned = 0;
	UCHAR   stringDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST) +
		MAXIMUM_USB_STRING_LENGTH];
	PUSB_STRING_DESCRIPTOR  stringDesc = NULL;
	PUSB_DESCRIPTOR_REQUEST stringDescReq = NULL;

	stringDescReq = (PUSB_DESCRIPTOR_REQUEST)stringDescReqBuf;
	stringDesc = (PUSB_STRING_DESCRIPTOR)(stringDescReq + 1);

	nBytes = sizeof(stringDescReqBuf);

	// Zero fill the entire request structure
	//
	memset(stringDescReq, 0, nBytes);

	// Indicate the port from which the descriptor will be requested
	//
	stringDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	stringDescReq->SetupPacket.wValue = (USB_STRING_DESCRIPTOR_TYPE << 8)
		| DescriptorIndex;

	stringDescReq->SetupPacket.wIndex = LanguageID;

	stringDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//
	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		stringDescReq,
		nBytes,
		stringDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	if (!success)
	{
		return GetLastError();
	}

	if (nBytesReturned < 2)
	{
		return GetLastError();
	}

	if (stringDesc->bDescriptorType != USB_STRING_DESCRIPTOR_TYPE)
	{
		return GetLastError();
	}

	if (stringDesc->bLength != nBytesReturned - sizeof(USB_DESCRIPTOR_REQUEST))
	{
		return GetLastError();
	}

	if (stringDesc->bLength % 2 != 0)
	{
		return GetLastError();
	}
	this->Wchar_tToString(Descriptor, stringDesc->bString);
	return TRUE;
}

BOOL _EnumUsbHub::GetInterFaceDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, UCHAR DescriptorIndex, USHORT LanguageID, USB_INTERFACE_DESCRIPTOR& Descriptor)
{
	BOOL    success = 0;
	ULONG   nBytes = 0;
	ULONG   nBytesReturned = 0;
	UCHAR   stringDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST) +
		sizeof(USB_INTERFACE_DESCRIPTOR)];
	PUSB_INTERFACE_DESCRIPTOR  stringDesc = NULL;
	PUSB_DESCRIPTOR_REQUEST stringDescReq = NULL;

	stringDescReq = (PUSB_DESCRIPTOR_REQUEST)stringDescReqBuf;
	stringDesc = (PUSB_INTERFACE_DESCRIPTOR)(stringDescReq + 1);

	nBytes = sizeof(stringDescReqBuf);

	// Zero fill the entire request structure
	//
	memset(stringDescReq, 0, nBytes);

	// Indicate the port from which the descriptor will be requested
	//
	stringDescReq->ConnectionIndex = ConnectionIndex;

	//
	// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
	// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
	//
	// USBD will automatically initialize these fields:
	//     bmRequest = 0x80
	//     bRequest  = 0x06
	//
	// We must inititialize these fields:
	//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
	//     wIndex    = Zero (or Language ID for String Descriptors)
	//     wLength   = Length of descriptor buffer
	//
	stringDescReq->SetupPacket.wValue = (USB_INTERFACE_DESCRIPTOR_TYPE << 8)
		| DescriptorIndex;

	//stringDescReq->SetupPacket.wIndex = 0;

	stringDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	// Now issue the get descriptor request.
	//
	success = DeviceIoControl(hHubDevice,
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		stringDescReq,
		nBytes,
		stringDescReq,
		nBytes,
		&nBytesReturned,
		NULL);

	if (!success)
	{
		return GetLastError();
	}

	if (nBytesReturned < 2)
	{
		return GetLastError();
	}

	if (stringDesc->bDescriptorType != USB_STRING_DESCRIPTOR_TYPE)
	{
		return GetLastError();
	}

	if (stringDesc->bLength != nBytesReturned - sizeof(USB_DESCRIPTOR_REQUEST))
	{
		return GetLastError();
	}

	if (stringDesc->bLength % 2 != 0)
	{
		return GetLastError();
	}
	Descriptor = *stringDesc;
	return TRUE;
}

BOOL _EnumUsbHub::GetAllStringDescriptors(
	HANDLE hHubDevice, 
	ULONG ConnectionIndex, 
	const USB_DEVICE_DESCRIPTOR DeviceDesc, 
	const PUCHAR ConfigDesc,
	const DWORD ConfigDescBytes,
	vector<wstring>& StringDesc,
	vector<USB_ALL_DESCRIPTOR>& AllDescs)
{

	PUCHAR                  descEnd = NULL;
	PUSB_COMMON_DESCRIPTOR  commonDesc = NULL;
	UCHAR                   bInterfaceClass = 0;
	BOOL                    getMoreStrings = FALSE;
	BOOL					hr = FALSE;
	USB_ALL_DESCRIPTOR		USBAllDescs;
	PUCHAR					USBDesc;
	wstring TmpDesc = _T("");
	USBDesc = new UCHAR[MAXIMUM_USB_STRING_LENGTH];
	::ZeroMemory(USBDesc, MAXIMUM_USB_STRING_LENGTH);

	if (DeviceDesc.iManufacturer)
	{
		this->GetStringDescriptor(hHubDevice,
			ConnectionIndex,
			DeviceDesc.iManufacturer,
			0x0409,
			TmpDesc);
	}
	StringDesc.emplace_back(TmpDesc);
	TmpDesc.clear();
	if (DeviceDesc.iProduct)
	{
		this->GetStringDescriptor(hHubDevice,
			ConnectionIndex,
			DeviceDesc.iProduct,
			0x0409,
			TmpDesc);
	}
	StringDesc.emplace_back(TmpDesc);
	TmpDesc.clear();
	if (DeviceDesc.iSerialNumber)
	{
		this->GetStringDescriptor(hHubDevice,
			ConnectionIndex,
			DeviceDesc.iSerialNumber,
			0x0409,
			TmpDesc);
	}
	StringDesc.emplace_back(TmpDesc);
	TmpDesc.clear();	

	//
	// Get the Configuration and Interface Descriptor strings
	//
	commonDesc = (PUSB_COMMON_DESCRIPTOR)ConfigDesc;

	descEnd = (PUCHAR)commonDesc + ConfigDescBytes - 1;

	while ((PUCHAR)commonDesc + sizeof(USB_COMMON_DESCRIPTOR) < descEnd &&
		(PUCHAR)commonDesc + commonDesc->bLength <= descEnd)
	{
		::ZeroMemory(&USBAllDescs.Descriptors, sizeof(USBAllDescs.Descriptors));
		::ZeroMemory(&USBAllDescs.Usb_Desc_Types, sizeof(USBAllDescs.Usb_Desc_Types));
		USBAllDescs.StringDescriptors.clear();
		switch (commonDesc->bDescriptorType)
		{
		case USB_CONFIGURATION_DESCRIPTOR_TYPE:
			if (commonDesc->bLength != sizeof(USB_CONFIGURATION_DESCRIPTOR))
			{
				break;
			}
			::CopyMemory(USBDesc, commonDesc, commonDesc->bLength);
			if (((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration)
			{
				hr = this->GetStringDescriptor(hHubDevice, ConnectionIndex, ((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration, 0x0409, TmpDesc);
				if (hr == TRUE)		USBAllDescs.StringDescriptors = TmpDesc;
				TmpDesc.clear();
			}
			USBAllDescs.Usb_Desc_Types = USB_CONFIGURATION_DESC;
			USBAllDescs.Descriptors.ConfigDesc = *(PUSB_CONFIGURATION_DESCRIPTOR)USBDesc;
			AllDescs.emplace_back(USBAllDescs);
			::ZeroMemory(USBDesc, sizeof(USB_CONFIGURATION_DESCRIPTOR));
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength) ;
			continue;

		case USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE:
			if (commonDesc->bLength < sizeof(USB_IAD_DESCRIPTOR))
			{
				break;
			}
			::CopyMemory(USBDesc, commonDesc, commonDesc->bLength);
			USBAllDescs.Usb_Desc_Types = USB_IAD_DESC;
			USBAllDescs.Descriptors.IADDesc = *(PUSB_IAD_DESCRIPTOR)USBDesc;
			if (((PUSB_IAD_DESCRIPTOR)commonDesc)->iFunction)
			{
				hr = this->GetStringDescriptor(hHubDevice, ConnectionIndex, ((PUSB_IAD_DESCRIPTOR)commonDesc)->iFunction, 0x0409, TmpDesc);
				if (hr == TRUE)		USBAllDescs.StringDescriptors = TmpDesc;
				TmpDesc.clear();
			}
			AllDescs.emplace_back(USBAllDescs);
			::ZeroMemory(USBDesc, sizeof(USB_IAD_DESCRIPTOR));
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;
		case USB_ENDPOINT_DESCRIPTOR_TYPE:
// 			if (commonDesc->bLength != sizeof(USB_ENDPOINT_DESCRIPTOR))
// 			{
// 				break;
// 			}
// 			::CopyMemory(USBDesc, commonDesc, commonDesc->bLength);
// 			USBAllDescs.Usb_Desc_Types = USB_ENDPOINT_DESC;
// 			USBAllDescs.Descriptors.EndPoint = *(PUSB_ENDPOINT_DESCRIPTOR)USBDesc;
// 			AllDescs.emplace_back(USBAllDescs);
// 			::ZeroMemory(USBDesc, sizeof(USB_ENDPOINT_DESCRIPTOR));
// 			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
// 			continue;
		case USB_HID_DESC:
// 			if (commonDesc->bLength != sizeof(USB_HID_DESCRIPTOR))
// 			{
// 				break;
// 			}
// 			::CopyMemory(USBDesc, commonDesc, commonDesc->bLength);
// 			USBAllDescs.Usb_Desc_Types = USB_HID_DESC;
// 			USBAllDescs.Descriptors.HIDDesc = *(PUSB_HID_DESCRIPTOR)USBDesc;
// 			AllDescs.emplace_back(USBAllDescs);
// 			::ZeroMemory(USBDesc, sizeof(USB_HID_DESCRIPTOR));
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;
		case USB_INTERFACE_DESCRIPTOR_TYPE:
			if (commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR) &&
				commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR2))
			{
				break;
			}
			::CopyMemory(USBDesc, commonDesc, commonDesc->bLength);
			USBAllDescs.Usb_Desc_Types = USB_INTERFACE_DESC;
			USBAllDescs.Descriptors.InterFaceDesc = *(PUSB_INTERFACE_DESCRIPTOR)USBDesc;
			if (((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface)
			{
				hr = this->GetStringDescriptor(hHubDevice, ConnectionIndex, ((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface, 0x0409, TmpDesc);
				if (hr == TRUE)		USBAllDescs.StringDescriptors = TmpDesc;
				TmpDesc.clear();
			}
			AllDescs.emplace_back(USBAllDescs);
			::ZeroMemory(USBDesc, sizeof(USB_INTERFACE_DESCRIPTOR));
			//
			// We need to display more string descriptors for the following
			// interface classes
			//
			bInterfaceClass = ((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->bInterfaceClass;
			if (bInterfaceClass == USB_DEVICE_CLASS_VIDEO)
			{
				getMoreStrings = TRUE;
			}
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;
		default:
			if (commonDesc->bLength <= 18)
			{
				::CopyMemory(USBDesc, commonDesc, commonDesc->bLength);
				USBAllDescs.Usb_Desc_Types = USB_Unknown;
				::CopyMemory(&USBAllDescs.Descriptors.UnknownDesc, USBDesc, commonDesc->bLength);
				AllDescs.emplace_back(USBAllDescs);
				::ZeroMemory(USBDesc, commonDesc->bLength);
			}
			commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
			continue;
		}
		break;
	}
	hr = TRUE;
	if (getMoreStrings)
	{
		//
		// We might need to display strings later that are referenced only in
		// class-specific descriptors. Get String Descriptors 1 through 32 (an
		// arbitrary upper limit for Strings needed due to "bad devices"
		// returning an infinite repeat of Strings 0 through 4) until one is not
		// found.
		//
		// There are also "bad devices" that have issues even querying 1-32, but
		// historically USBView made this query, so the query should be safe for
		// video devices.
		//
		for (UCHAR uIndex = 1; hr == TRUE && (uIndex < 256); uIndex++)
		{
			hr = this->GetStringDescriptor(hHubDevice,
				ConnectionIndex,
				uIndex,
				0x0409,
				TmpDesc);
			StringDesc.emplace_back(TmpDesc);
			TmpDesc.clear();
		}
	}
	this->SafeDeleteArray(USBDesc);
	return TRUE;
}


template<class T>
inline void _EnumUsbHub::SafeDeleteData(T *& Data)
{
	if (Data)
	{
		delete Data;
		Data = NULL;
	}
}
template<class T>
inline void _EnumUsbHub::SafeDeleteArray(T *& Data)
{
	if (Data)
	{
		delete[] Data;
		Data = NULL;
	}
}