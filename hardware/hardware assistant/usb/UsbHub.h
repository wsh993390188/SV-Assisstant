#pragma once
#include "UsbController.h"

class _EnumUsbHub : public _EnumUsbController
{
public:
	_EnumUsbHub();
	virtual ~_EnumUsbHub();
	BOOL Excute();
	BOOL Updatedata();
protected:
	BOOL EnumRootUsbHub(const wstring HubName, bool IsRootHub);
	BOOL EnumExternalUsbHub(const wstring HubName, USB_NODE_CONNECTION_INFORMATION_EX_V1 ConnectionInfo, USB_NODE_CONNECTION_INFORMATION_EX_V2 ConnectionInfoV2, USB_PIPE_INFO Usb_Pipe_Info[30], USB_PORT_CONNECTOR_PROPERTIES_V2 PortConnectorProps, USB_CONFIGURATION_DESCRIPTOR ConfigDesc, USB_BOS_DESCRIPTOR BosDesc, vector<wstring> StringDescs, vector<USB_ALL_DESCRIPTOR> AllDescs, USB_DEVICE_PNP_STRINGS DevProps, bool IsRootHub);
	BOOL EnumUsbPort(HANDLE hHubDevice, ULONG PortNums, bool IsRootHub);
	BOOL EnumUSBDevice(const GUID Guid);
private:
	BOOL GetExternalHubName(HANDLE Hub, ULONG ConnectionIndex, wstring & ExternalHubName);
	BOOL GetDriverKeyName(HANDLE Hub, ULONG ConnectionIndex, wstring & DriverKeyName);
	BOOL GetPortConnectorProps(HANDLE hHubDevie, ULONG Index, USB_PORT_CONNECTOR_PROPERTIES_V2& PortConnectorProps);
	BOOL GetConnectionInfo(HANDLE hHubDevice, ULONG Index, USB_NODE_CONNECTION_INFORMATION_EX_V1& ConnectionInfo, PUSB_PIPE_INFO Usb_Pipe_Info,USB_NODE_CONNECTION_INFORMATION_EX_V2& ConnectionInfoV2);
	BOOL FindMatchingDeviceNodeForDriverName(wstring DriverKeyName, DEVICE_INFO_NODE & DeviceInfoNode, BOOLEAN IsHub);
	BOOL GetConfigDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, UCHAR DescriptorIndex, PUCHAR & ConfigDesc, DWORD & ConfigDescBytes);
	BOOL GetBOSDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, USB_BOS_DESCRIPTOR & BOSDesc);
	BOOL AreThereStringDescriptors(const USB_DEVICE_DESCRIPTOR DeviceDesc, const PUCHAR ConfigDesc, const DWORD ConfigDescBytes);
	BOOL GetStringDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, UCHAR DescriptorIndex, USHORT LanguageID, wstring & Descriptor);
	BOOL GetInterFaceDescriptor(HANDLE hHubDevice, ULONG ConnectionIndex, UCHAR DescriptorIndex, USHORT LanguageID, USB_INTERFACE_DESCRIPTOR& Descriptor);
	BOOL GetAllStringDescriptors(HANDLE hHubDevice, ULONG ConnectionIndex, const USB_DEVICE_DESCRIPTOR DeviceDesc, const PUCHAR ConfigDesc, const DWORD ConfigDescBytes, vector<wstring>& StringDesc, vector<USB_ALL_DESCRIPTOR>& AllDescs);

	template<class T>
	void SafeDeleteData(T*& Data);
	template<class T>
	void SafeDeleteArray(T *& Data);
public:
	vector<USB_ROOT_HUB> Usb_Tree;
private:
	vector<DEVICE_INFO_NODE> Usb_Device_info;
	vector<DEVICE_INFO_NODE> Usb_Hub_info;
	// Temp Data
	vector<USBROOTHUBINFO> Root_Hub_Tmp;
	USB_Hub Hub_tmp;	//Hub Info
	USB_Device Device_Hub_Tmp;	//Root Port(Hub)
	vector<USBDEVICEINFO > Device_Tmp;	//Root Port
};

