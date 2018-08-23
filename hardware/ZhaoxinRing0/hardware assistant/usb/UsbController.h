#pragma once
#include <initguid.h>
#include <winioctl.h>
#include <math.h>
#include "UsbDefination.h"

using namespace std;
class _EnumUsbController
{
public:
	_EnumUsbController();
	virtual ~_EnumUsbController();
protected:
	BOOL EnumUsbController();
	BOOL GetHCDController(const wstring & ControllerName);
	BOOL GetHCDDriverKeyName(HANDLE HCD, wstring & HCDDriverName);
	BOOL GetRootHubName(HANDLE HostController, wstring & RootHubName);
	BOOL GetUsbPowerState(HANDLE hDevice, PUSB_POWER_INFO Usb_Power_Info);
	BOOL GetHostControllerInfo(HANDLE hDevice, USB_CONTROLLER_INFO_0 & Usb_Controller_Info);
	BOOL DriverNametoDevInstance(const wstring& DriverName, USB_DEVICE_PNP_STRINGS & DevicePNPInfoData, USB_HOST_CONTROLLER_BDF & DeviceBDF);
	BOOL GetDevInstInfo(HDEVINFO DeviceInfo, SP_DEVINFO_DATA& DeviceInfoData, USB_DEVICE_PNP_STRINGS & Device_PNP_String);
	BOOL GetDevInstInfo_Ext(HDEVINFO DeviceInfo, SP_DEVINFO_DATA & DeviceInfoData, USB_DEVICE_PNP_STRINGS & Device_PNP_String);
	BOOL GetUsbHostControllerBDF(HDEVINFO DeviceInfo, SP_DEVINFO_DATA& DeviceInfoData, USB_HOST_CONTROLLER_BDF & DeviceBDF);
protected:
	BOOL GetDeviceProperty(
		_In_    HDEVINFO         DeviceInfoSet,
		_In_    PSP_DEVINFO_DATA DeviceInfoData,
		_In_    DWORD            Property,
		_Outptr_ std::wstring&    ppBuffer
	);
	vector<wstring> ControllerDevicePath;	//Controller 设备路径
 	vector<USBHOSTCONTROLLERINFO> UsbHostControllerInfo; //Controller 设备信息
};

