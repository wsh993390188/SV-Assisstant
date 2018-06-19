#include "UsbDefination.h"

USB_ROOT_HUB::USB_ROOT_HUB()
{
}

USB_ROOT_HUB::~USB_ROOT_HUB()
{
	this->Clear();
}

void USB_ROOT_HUB::Clear()
{
	UsbRootHubInfo.Clear();
	UsbDeviceInfo.Clear();
	UsbRootControllerInfo.Clear();
}

USB_Hub::USB_Hub()
{
}

USB_Hub::~USB_Hub()
{
	this->Clear();
}

void USB_Hub::Clear()
{
	UsbExternalHubInfo.clear();
	UsbDeviceInfo.clear();
}

USB_Device::USB_Device()
{
}

USB_Device::~USB_Device()
{
	this->Clear();
}

void USB_Device::Clear()
{
	this->UsbDeviceInfo.clear();
	vector<USBDEVICEINFO>().swap(this->UsbDeviceInfo);
	this->UsbHubInfo.clear();
	vector<USB_Hub>().swap(this->UsbHubInfo);
}

void _USBEXTERNALHUBINFO::Clear()
{
	::ZeroMemory(&BosDesc, sizeof(BosDesc));
	::ZeroMemory(&ConfigDesc, sizeof(ConfigDesc));
	::ZeroMemory(&ConnectionInfo, sizeof(ConnectionInfo));
	::ZeroMemory(&ConnectionInfoV2, sizeof(ConnectionInfoV2));
	::ZeroMemory(&DeviceInfoType, sizeof(DeviceInfoType));
	::ZeroMemory(&HubCapabilityEx, sizeof(HubCapabilityEx));
	::ZeroMemory(&HubInfo, sizeof(HubInfo));
	::ZeroMemory(&HubInfoEx, sizeof(HubInfoEx));
	::ZeroMemory(&PortConnectorProps, sizeof(PortConnectorProps) - sizeof(string));
	PortConnectorProps.CompanionHubSymbolicLinkName.clear();
	DeviceInfoNode.DeviceDescName.clear();
	DeviceInfoNode.DeviceDriverName.clear();
	DeviceInfoNode.DevicePath.clear();
	DeviceInfoNode.DeviceDescNameLength = 0;
	DeviceInfoNode.DeviceDriverNameLength = 0;
	DeviceInfoNode.DeviceInfo = 0;
	::ZeroMemory(&DeviceInfoNode.DeviceInfoData, sizeof(SP_DEVINFO_DATA));
	DeviceInfoNode.LatestDevicePowerState = PowerDeviceUnspecified;
	::ZeroMemory(&DeviceInfoNode.DeviceInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
	UsbDeviceProperties.DeviceClass.clear();
	UsbDeviceProperties.DeviceDesc.clear();
	UsbDeviceProperties.DeviceId.clear();
	UsbDeviceProperties.HwId.clear();
	UsbDeviceProperties.PowerState.clear();
	UsbDeviceProperties.Service.clear();
	StringDescs.clear();
	AllDescs.clear();
	for (size_t i = 0; i < 30; i++)
	{
		::ZeroMemory(&Usb_Pipe_Info[i], sizeof(USB_PIPE_INFO));
	}
	HubName.clear();
}

void _USBROOTHUBINFO::Clear()
{
	DeviceInfoNode.DeviceDescName.clear();
	DeviceInfoNode.DeviceDriverName.clear();
	DeviceInfoNode.DevicePath.clear();
	DeviceInfoNode.DeviceDescNameLength = 0;
	DeviceInfoNode.DeviceDriverNameLength = 0;
	DeviceInfoNode.DeviceInfo = 0;
	::ZeroMemory(&DeviceInfoNode.DeviceInfoData, sizeof(SP_DEVINFO_DATA));
	DeviceInfoNode.LatestDevicePowerState = PowerDeviceUnspecified;
	::ZeroMemory(&DeviceInfoNode.DeviceInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
	::ZeroMemory(&DeviceInfoType, sizeof(DeviceInfoType));
	::ZeroMemory(&HubCapabilityEx, sizeof(HubCapabilityEx));
	::ZeroMemory(&HubInfo, sizeof(HubInfo));
	::ZeroMemory(&HubInfoEx, sizeof(HubInfoEx));
	::ZeroMemory(&PortConnectorProps, sizeof(PortConnectorProps) - sizeof(string));
	PortConnectorProps.CompanionHubSymbolicLinkName.clear();
	UsbDeviceProperties.DeviceClass.clear();
	UsbDeviceProperties.DeviceDesc.clear();
	UsbDeviceProperties.DeviceId.clear();
	UsbDeviceProperties.HwId.clear();
	UsbDeviceProperties.PowerState.clear();
	UsbDeviceProperties.Service.clear();
	HubName.clear();
}

void _USBDEVICEINFO::Clear()
{
	DeviceInfoNode.DeviceDescName.clear();
	DeviceInfoNode.DeviceDriverName.clear();
	DeviceInfoNode.DevicePath.clear();
	DeviceInfoNode.DeviceDescNameLength = 0;
	DeviceInfoNode.DeviceDriverNameLength = 0;
	DeviceInfoNode.DeviceInfo = 0;
	::ZeroMemory(&DeviceInfoNode.DeviceInfoData, sizeof(SP_DEVINFO_DATA));
	DeviceInfoNode.LatestDevicePowerState = PowerDeviceUnspecified;
	::ZeroMemory(&DeviceInfoNode.DeviceInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
	::ZeroMemory(&DeviceInfoType, sizeof(DeviceInfoType));
	::ZeroMemory(&HubCapabilityEx, sizeof(HubCapabilityEx));
	::ZeroMemory(&HubInfo, sizeof(HubInfo));
	::ZeroMemory(&ConfigDesc, sizeof(ConfigDesc));
	::ZeroMemory(&BosDesc, sizeof(BosDesc));
	::ZeroMemory(&HubInfoEx, sizeof(HubInfoEx));
	::ZeroMemory(&PortConnectorProps, sizeof(PortConnectorProps) - sizeof(string));
	PortConnectorProps.CompanionHubSymbolicLinkName.clear();
	UsbDeviceProperties.DeviceClass.clear();
	UsbDeviceProperties.DeviceDesc.clear();
	UsbDeviceProperties.DeviceId.clear();
	UsbDeviceProperties.HwId.clear();
	UsbDeviceProperties.PowerState.clear();
	UsbDeviceProperties.Service.clear();
	HubName.clear();
	AllDescs.clear();
	StringDescs.clear();
	for (auto& i : Usb_Pipe_Info)
	{
		::ZeroMemory(&i, sizeof(USB_PIPE_INFO));
	}
}

void _USBHOSTCONTROLLERINFO::Clear()
{
	DriverKey.clear();
	RootHubName.clear();
	DeviceInfoType = HostControllerInfo;
	Revision = 0;
	VendorID = 0;
	this->SubSysID = 0;
	this->DeviceID = 0;
	UsbDeviceProperties.DeviceClass.clear();
	UsbDeviceProperties.DeviceDesc.clear();
	UsbDeviceProperties.DeviceId.clear();
	UsbDeviceProperties.HwId.clear();
	UsbDeviceProperties.PowerState.clear();
	UsbDeviceProperties.Service.clear();
	::ZeroMemory(&UsbControllerBDF, sizeof(USB_HOST_CONTROLLER_BDF));
	::ZeroMemory(&ControllerInfo, sizeof(USB_CONTROLLER_INFO_0));
	for (auto& i : USBPowerInfo)
	{
        ::ZeroMemory(&i, sizeof(USB_POWER_INFO));
	}
}
