#pragma once
#pragma warning(disable:4035)
#include <windowsx.h>
#include <initguid.h>
#include <devioctl.h>
#include <stdio.h>
#include <usbioctl.h>
#include <usbiodef.h>
#include <usb.h>
#include <usbuser.h>
#include <setupapi.h>
#include <winioctl.h>
#include <math.h>
#include <vector>
#include <string>

using namespace std;
#define USB_DESCRIPTOR_MAX_LENGTH 18

typedef enum _USBDESCRIPTORTYPES
{
	USB_Unknown = 0x00,
	USB_DEVICE_DESC = 0x01,
	USB_CONFIGURATION_DESC = 0x02,
	USB_INTERFACE_DESC = 0x04,
	USB_ENDPOINT_DESC = 0x05,
	USB_IAD_DESC = 0x0B,
	USB_HID_DESC = 0x21,
	USB_VIDEO_DESC = 0x24
} USBDESCRIPTORTYPES, *PUSBDESCRIPTORTYPES;

typedef enum _USBDEVICEINFOTYPE
{
	HostControllerInfo,
	RootHubInfo,
	ExternalHubInfo,
	DEVICEInfo
} USBDEVICEINFOTYPE, *PUSBDEVICEINFOTYPE;

typedef struct _DEVICE_INSTANCE
{
	HDEVINFO                         DeviceInfo;
	SP_DEVINFO_DATA                  DeviceInfoData;
}DEVICE_INSTANCE;

// HID Class HID Descriptor
//
typedef struct _USB_HID_DESCRIPTOR
{
	UCHAR   bLength;
	UCHAR   bDescriptorType;
	USHORT  bcdHID;
	UCHAR   bCountryCode;
	UCHAR   bNumDescriptors;
	struct
	{
		UCHAR   bDescriptorType;
		USHORT  wDescriptorLength;
	} OptionalDescriptors[1];
} USB_HID_DESCRIPTOR, *PUSB_HID_DESCRIPTOR;

typedef struct _USB_DEVICE_PNP_STRINGS
{
	wstring DeviceId;
	wstring DeviceDesc;
	wstring HwId;
	wstring Service;
	wstring DeviceClass;
} USB_DEVICE_PNP_STRINGS, *PUSB_DEVICE_PNP_STRINGS;

typedef struct _USB_HOST_CONTROLLER_BDF
{
	BOOL                                BusDeviceFunctionValid;
	ULONG                               BusNumber;
	USHORT                              BusDevice;
	USHORT                              BusFunction;
}USB_HOST_CONTROLLER_BDF;

//вт╦д
typedef struct _USBHOSTCONTROLLERINFO
{
	USBDEVICEINFOTYPE                   DeviceInfoType;
	wstring                             DriverKey;
	ULONG                               VendorID;
	ULONG                               DeviceID;
	ULONG                               SubSysID;
	ULONG                               Revision;
	USB_POWER_INFO                      USBPowerInfo[6];
	USB_HOST_CONTROLLER_BDF				UsbControllerBDF;
	USB_CONTROLLER_INFO_0               ControllerInfo;
	USB_DEVICE_PNP_STRINGS              UsbDeviceProperties;
	wstring								RootHubName;
}USBHOSTCONTROLLERINFO, *PUSBHOSTCONTROLLERINFO;

typedef struct _DEVICE_INFO_NODE {
	HDEVINFO                         DeviceInfo;
	SP_DEVINFO_DATA                  DeviceInfoData;
	SP_DEVICE_INTERFACE_DATA         DeviceInterfaceData;
	wstring							 DevicePath;
	wstring                          DeviceDescName;
	wstring                          DeviceDriverName;
	DEVICE_POWER_STATE               LatestDevicePowerState;
} DEVICE_INFO_NODE, *PDEVICE_INFO_NODE;

// IAD Descriptor
//
typedef struct _USB_IAD_DESCRIPTOR
{
	UCHAR   bLength;
	UCHAR   bDescriptorType;
	UCHAR   bFirstInterface;
	UCHAR   bInterfaceCount;
	UCHAR   bFunctionClass;
	UCHAR   bFunctionSubClass;
	UCHAR   bFunctionProtocol;
	UCHAR   iFunction;
} USB_IAD_DESCRIPTOR, *PUSB_IAD_DESCRIPTOR;

// Common Class Interface Descriptor
//
typedef struct _USB_INTERFACE_DESCRIPTOR2 {
	UCHAR  bLength;             // offset 0, size 1
	UCHAR  bDescriptorType;     // offset 1, size 1
	UCHAR  bInterfaceNumber;    // offset 2, size 1
	UCHAR  bAlternateSetting;   // offset 3, size 1
	UCHAR  bNumEndpoints;       // offset 4, size 1
	UCHAR  bInterfaceClass;     // offset 5, size 1
	UCHAR  bInterfaceSubClass;  // offset 6, size 1
	UCHAR  bInterfaceProtocol;  // offset 7, size 1
	UCHAR  iInterface;          // offset 8, size 1
	USHORT wNumClasses;         // offset 9, size 2
} USB_INTERFACE_DESCRIPTOR2, *PUSB_INTERFACE_DESCRIPTOR2;

typedef struct _STRING_DESCRIPTOR_NODE
{
	USB_COMMON_DESCRIPTOR           DescriptorHead;
	CHAR				            StringDescriptor[MAXIMUM_USB_STRING_LENGTH];
} STRING_DESCRIPTOR_NODE, *PSTRING_DESCRIPTOR_NODE;

typedef struct _USB_PORT_CONNECTOR_PROPERTIES_V2 {
	// one based port number
	ULONG  ConnectionIndex;

	// The number of bytes required to hold the entire USB_PORT_CONNECTOR_PROPERTIES
	// structure, including the full CompanionHubSymbolicLinkName wstring
	ULONG  ActualLength;

	// bitmask of flags indicating properties and capabilities of the port
	USB_PORT_PROPERTIES  UsbPortProperties;

	// Zero based index number of the companion port being queried.  
	USHORT                CompanionIndex;

	// Port number of the companion port
	USHORT                CompanionPortNumber;

	// Symbolic link name for the companion hub
	wstring                CompanionHubSymbolicLinkName;
} USB_PORT_CONNECTOR_PROPERTIES_V2, *PUSB_PORT_CONNECTOR_PROPERTIES_V2;

typedef struct _USB_NODE_CONNECTION_INFORMATION_EX_V1 {
	ULONG ConnectionIndex;  /* INPUT */
							/* usb device descriptor returned by this device
							during enumeration */
	USB_DEVICE_DESCRIPTOR DeviceDescriptor;/* OUTPUT */
	UCHAR CurrentConfigurationValue;/* OUTPUT */
									/* values for the speed field are defined in USB200.h */
	UCHAR Speed;/* OUTPUT */
	BOOLEAN DeviceIsHub;/* OUTPUT */
	USHORT DeviceAddress;/* OUTPUT */
	ULONG NumberOfOpenPipes;/* OUTPUT */
	USB_CONNECTION_STATUS ConnectionStatus;/* OUTPUT */
} USB_NODE_CONNECTION_INFORMATION_EX_V1, *PUSB_NODE_CONNECTION_INFORMATION_EX_V1;

typedef struct _USB_ALL_DESCRIPTOR
{
	USBDESCRIPTORTYPES Usb_Desc_Types;
	union
	{
		USB_CONFIGURATION_DESCRIPTOR		ConfigDesc;
		USB_INTERFACE_DESCRIPTOR			InterFaceDesc;
		USB_HID_DESCRIPTOR					HIDDesc;
		USB_IAD_DESCRIPTOR					IADDesc;
		USB_ENDPOINT_DESCRIPTOR				EndPoint;
		CHAR								UnknownDesc[USB_DESCRIPTOR_MAX_LENGTH];
	}Descriptors;
	wstring									StringDescriptors;
}USB_ALL_DESCRIPTOR;


typedef struct _USBROOTHUBINFO
{
	USBDEVICEINFOTYPE                   DeviceInfoType;
	USB_NODE_INFORMATION                HubInfo;
	USB_HUB_INFORMATION_EX              HubInfoEx;
	wstring                             HubName;
	USB_PORT_CONNECTOR_PROPERTIES_V2    PortConnectorProps;
	USB_DEVICE_PNP_STRINGS              UsbDeviceProperties;
	DEVICE_INFO_NODE                    DeviceInfoNode;
	USB_HUB_CAPABILITIES_EX             HubCapabilityEx;
} USBROOTHUBINFO, *PUSBROOTHUBINFO;

typedef struct _USBEXTERNALHUBINFO
{
	USBDEVICEINFOTYPE                      DeviceInfoType;
	USB_NODE_INFORMATION                   HubInfo;
	USB_HUB_INFORMATION_EX                 HubInfoEx;
	wstring                                HubName;
	USB_NODE_CONNECTION_INFORMATION_EX_V1  ConnectionInfo;
	USB_PIPE_INFO						   Usb_Pipe_Info[30];
	USB_PORT_CONNECTOR_PROPERTIES_V2       PortConnectorProps;
	USB_CONFIGURATION_DESCRIPTOR           ConfigDesc;
	USB_BOS_DESCRIPTOR					   BosDesc;
	vector<wstring>						   StringDescs;
	vector<USB_ALL_DESCRIPTOR>			   AllDescs;		 // NULL if root HUB
	USB_NODE_CONNECTION_INFORMATION_EX_V2  ConnectionInfoV2; // NULL if root HUB
	USB_DEVICE_PNP_STRINGS                 UsbDeviceProperties;
	DEVICE_INFO_NODE                       DeviceInfoNode;
	USB_HUB_CAPABILITIES_EX                HubCapabilityEx;
} USBEXTERNALHUBINFO, *PUSBEXTERNALHUBINFO;

// HubInfo, HubName may be in USBDEVICEINFOTYPE, so they can be removed
typedef struct _USBDEVICEINFO
{
	USBDEVICEINFOTYPE                      DeviceInfoType;
	USB_NODE_INFORMATION                   HubInfo;          // NULL if not a HUB
	USB_HUB_INFORMATION_EX                 HubInfoEx;        // NULL if not a HUB
	wstring                                HubName;          // NULL if not a HUB
	USB_NODE_CONNECTION_INFORMATION_EX_V1  ConnectionInfo;   // NULL if root HUB
	USB_PIPE_INFO						   Usb_Pipe_Info[30];// NULL if root HUB
	USB_PORT_CONNECTOR_PROPERTIES_V2       PortConnectorProps;
	USB_CONFIGURATION_DESCRIPTOR           ConfigDesc;       // NULL if root HUB
	USB_BOS_DESCRIPTOR					   BosDesc;          // NULL if root HUB
	vector<wstring>						   StringDescs;
	vector<USB_ALL_DESCRIPTOR>			   AllDescs;		 // NULL if root HUB
	USB_NODE_CONNECTION_INFORMATION_EX_V2  ConnectionInfoV2; // NULL if root HUB
	USB_DEVICE_PNP_STRINGS                 UsbDeviceProperties;
	DEVICE_INFO_NODE                       DeviceInfoNode;
	USB_HUB_CAPABILITIES_EX                HubCapabilityEx;  // NULL if not a HUB
} USBDEVICEINFO, *PUSBDEVICEINFO;

struct USB_Hub
{
	vector<USBEXTERNALHUBINFO>				UsbExternalHubInfo;
	vector<USBDEVICEINFO>					UsbDeviceInfo;
};

struct USB_Device
{
	vector<USB_Hub>							UsbHubInfo;
	vector<USBDEVICEINFO>					UsbDeviceInfo;
};

struct USB_ROOT_HUB
{
	USBHOSTCONTROLLERINFO					UsbRootControllerInfo;
	USBROOTHUBINFO							UsbRootHubInfo;
	USB_Device								UsbDeviceInfo;
};