#pragma once
#include "UsbDefination.h"

class _EnumUsbController
{
public:
	_EnumUsbController();
	virtual ~_EnumUsbController();
	const wstring ReturnControllerDevicePath(size_t num);
//	virtual BOOL Excute();
protected:
	BOOL EnumUsbController();
	BOOL GetHCDController(size_t num);	// HCD Driver Info
	BOOL GetUsbPowerState(HANDLE hDevice, const UINT num, PUSB_POWER_INFO Usb_Power_Info);
	BOOL DriverNametoDevInstance(const wstring DriverName, DEVICE_INSTANCE & DeviceInst, USB_DEVICE_PNP_STRINGS& DevicePNPInfoData, USB_HOST_CONTROLLER_BDF& DeviceBDF);
	BOOL GetDevInstInfo_Ext(const DEVICE_INSTANCE DeviceInst, USB_DEVICE_PNP_STRINGS & Device_PNP_String);
	BOOL GetDevInstInfo(const DEVICE_INSTANCE DeviceInst, USB_DEVICE_PNP_STRINGS & Device_PNP_String);
	BOOL GetUsbHostControllerBDF(const DEVICE_INSTANCE DeviceInst, USB_HOST_CONTROLLER_BDF & DeviceBDF);
	void Wchar_tToString(std::wstring & szDst, wchar_t * wchar);
// 	virtual BOOL EnumUsbHub(const string HubName, bool IsRootHub) = 0;
// 	virtual BOOL EnumUsbPort(HANDLE hHubDevice, bool IsRootHub) = 0;

private:
	BOOL GetHostControllerInfo(HANDLE hDevice, const UINT num, USB_CONTROLLER_INFO_0& Usb_Controller_Info);
	BOOL GetRootHubName(HANDLE HostController, wstring & RootHubName);
	BOOL GetHCDDriverKeyName(HANDLE HCD, wstring & HCDDriverName);	//DevicePath ->	DriverName

public:
	size_t ControllerNum;
protected:
	vector<wstring> ControllerDevicePath;	//Controller 设备路径
 	vector<USBHOSTCONTROLLERINFO> UsbHostControllerInfo; //Controller 设备信息
};

