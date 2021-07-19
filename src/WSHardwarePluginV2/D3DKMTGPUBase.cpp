#include "stdafx.h"
#include "D3DKMTGPUBase.h"
#include "NTD3DKMT.h"

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif
//
// Interface used by anyone listening for arrival of the display device
// {1CA05180-A699-450A-9A0C-DE4FBE3DDD89}
//

DEFINE_GUID(GUID_DISPLAY_DEVICE_ARRIVAL, 0x1CA05180, 0xA699, 0x450A, 0x9A, 0x0C, 0xDE, 0x4F, 0xBE, 0x3D, 0xDD, 0x89);

Hardware::Data::ErrorType Hardware::GPU::D3DKMTGPUBase::Initialize(std::string& response)
{
	return Data::ErrorType();
}

Hardware::Data::ErrorType Hardware::GPU::D3DKMTGPUBase::Update(const std::string& Args, std::string& response)
{
	return Data::ErrorType();
}

Hardware::Data::ErrorType Hardware::GPU::D3DKMTGPUBase::GetElements(LPCSTR paramter, std::string& response)
{
	return Data::ErrorType();
}

bool Hardware::GPU::D3DKMTGPUBase::InitializeD3DKMT()
{
	ULONG deviceInterfaceListLength = 0;
	if (CM_Get_Device_Interface_List_Size(
		&deviceInterfaceListLength,
		(PGUID)&GUID_DISPLAY_DEVICE_ARRIVAL,
		NULL,
		CM_GET_DEVICE_INTERFACE_LIST_PRESENT
	) != CR_SUCCESS)
	{
		return false;
	}

	auto deviceInterfaceList = std::make_unique<TCHAR[]>(deviceInterfaceListLength);
	memset(deviceInterfaceList.get(), 0, deviceInterfaceListLength * sizeof(TCHAR));

	if (CM_Get_Device_Interface_List(
		(PGUID)&GUID_DISPLAY_DEVICE_ARRIVAL,
		NULL,
		deviceInterfaceList,
		deviceInterfaceListLength,
		CM_GET_DEVICE_INTERFACE_LIST_PRESENT
	) != CR_SUCCESS)
	{
		return false;
	}

	for(auto deviceInterface = deviceInterfaceList.get(); *deviceInterface; deviceInterface = deviceInterface + _tcslen(deviceInterface))
	{
		D3DKMT_OPENADAPTERFROMDEVICENAME openAdapterFromDeviceName{}
		openAdapterFromDeviceName.DeviceName = deviceInterface;

		if (!NT_SUCCESS(D3DKMTOpenAdapterFromDeviceName(&openAdapterFromDeviceName)))
            continue;
		
		if(Utils::GetWindowsVersion() >= WINDOWS_10_RS4)
		{
			
		}
	}

	return true;
}