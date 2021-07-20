#include "stdafx.h"
#include "D3DKMTGPUBase.h"
#include "GPUDeviceFactory.h"

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

//
// Interface used by anyone listening for arrival of the display device
// {1CA05180-A699-450A-9A0C-DE4FBE3DDD89}
//
#ifndef GUID_DISPLAY_DEVICE_ARRIVAL
#include <initguid.h>
DEFINE_GUID(GUID_DISPLAY_DEVICE_ARRIVAL, 0x1CA05180, 0xA699, 0x450A, 0x9A, 0x0C, 0xDE, 0x4F, 0xBE, 0x3D, 0xDD, 0x89);
#endif // !GUID_DISPLAY_DEVICE_ARRIVAL

Hardware::Data::ErrorType Hardware::GPU::D3DKMTGPUBase::Initialize(std::string& response)
{
	if (InitializeD3DKMT())
	{
		GPUConfig::Instance().Initialize();
		response = this->BuildInitializeJson();
		return Data::ErrorType::SUCCESS;
	}
	return Data::ErrorType::NOTSUPPORT;
}

Hardware::Data::ErrorType Hardware::GPU::D3DKMTGPUBase::Update(const std::string& Args, std::string& response)
{
	uint32_t GPUId;
	if (ParserJson(Args, GPUId))
	{
		if (auto Itr = GPUInfos.find(GPUId); Itr != GPUInfos.end())
		{
			response = Itr->second->UpdateGPUInfo();
			return Data::ErrorType::SUCCESS;
		}
	}
	return Data::ErrorType::UNKNOWNJSON;
}

Hardware::Data::ErrorType Hardware::GPU::D3DKMTGPUBase::GetElements(LPCSTR paramter, std::string& response)
{
	uint32_t GPUId;
	if (ParserJson(paramter, GPUId))
	{
		if (auto Itr = GPUInfos.find(GPUId); Itr != GPUInfos.end())
		{
			response = Itr->second->GetGPUInfo();
			return Data::ErrorType::SUCCESS;
		}
	}
	return Data::ErrorType::UNKNOWNJSON;
}

bool Hardware::GPU::D3DKMTGPUBase::InitializeD3DKMT()
{
	ULONG deviceInterfaceListLength = 0;
	if (CM_Get_Device_Interface_List_Size(
		&deviceInterfaceListLength,
		(LPGUID)&GUID_DISPLAY_DEVICE_ARRIVAL,
		NULL,
		CM_GET_DEVICE_INTERFACE_LIST_PRESENT
	) != CR_SUCCESS)
	{
		return false;
	}

	auto deviceInterfaceList = std::make_unique<TCHAR[]>(deviceInterfaceListLength);
	memset(deviceInterfaceList.get(), 0, deviceInterfaceListLength * sizeof(TCHAR));

	if (CM_Get_Device_Interface_List(
		(LPGUID)&GUID_DISPLAY_DEVICE_ARRIVAL,
		NULL,
		deviceInterfaceList.get(),
		deviceInterfaceListLength,
		CM_GET_DEVICE_INTERFACE_LIST_PRESENT
	) != CR_SUCCESS)
	{
		return false;
	}

	std::map<uint16_t, std::map<uint16_t, size_t>> DeviceCount;
	auto PciDevices = this->FindGPUDeviceOnPCI();
	for (auto deviceInterface = deviceInterfaceList.get(); *deviceInterface; deviceInterface = deviceInterface + _tcslen(deviceInterface) + 1)
	{
		D3DKMT_OPENADAPTERFROMDEVICENAME openAdapterFromDeviceName{};
		openAdapterFromDeviceName.DeviceName = deviceInterface;

		if (!NT_SUCCESS(D3DKMTOpenAdapterFromDeviceName(&openAdapterFromDeviceName)))
			continue;

		if (Utils::GetWindowsVersion() >= WINDOWS_10_RS4)
		{
			if (D3DKMTHelper::IsGpuSoftwareDevice(openAdapterFromDeviceName.AdapterHandle))
			{
				D3DKMTHelper::CloseAdapterHandle(openAdapterFromDeviceName.AdapterHandle);
				continue;
			}
		}

		D3DKMT_QUERYSTATISTICS queryStatistics{};
		queryStatistics.Type = D3DKMT_QUERYSTATISTICS_ADAPTER;
		queryStatistics.AdapterLuid = openAdapterFromDeviceName.AdapterLuid;
		if (NT_SUCCESS(D3DKMTQueryStatistics(&queryStatistics)))
		{
			auto GpuAdapter = CreateAdapter(openAdapterFromDeviceName.DeviceName, openAdapterFromDeviceName.AdapterHandle, openAdapterFromDeviceName.AdapterLuid, queryStatistics.QueryResult.AdapterInformation.NbSegments,
				queryStatistics.QueryResult.AdapterInformation.NodeCount);
			if (GpuAdapter)
			{
				ULONG   ven{}, dev{}, subsys{}, subdev{}, rev{};
				if (auto errorcode = _stscanf_s(openAdapterFromDeviceName.DeviceName, _T("\\\\?\\PCI#VEN_%04x&DEV_%04x&SUBSYS_%04x%04x&REV_%02x"), &ven, &dev, &subsys, &subdev, &rev); errorcode >= 2)
				{
					GPUDevice Device;
					if (FindDevice(ven, dev, DeviceCount[ven][dev]++, Device))
					{
						auto Dev = GPUDeviceFactory::Instance().CreateDevice(Device, std::move(GpuAdapter));
						if (Dev)
							this->GPUInfos.emplace(Device.pciAddress, std::move(Dev));
						else
						{
							spdlog::error(R"(Cant create device vid:{:x}, did:{:x})", Device.VendorId, Device.DeviceId);
						}
					}
					else
					{
						spdlog::error(R"(can find pci device vid:{:x}, did:{:x})", ven, dev);
					}
				}
				else
				{
					spdlog::error(R"(parse Devica name {} error: {})", Utils::wstringToUtf8(openAdapterFromDeviceName.DeviceName), errorcode);
				}
			}
		}
		D3DKMTHelper::CloseAdapterHandle(openAdapterFromDeviceName.AdapterHandle);
	}

	return true;
}

std::unique_ptr<Hardware::GPU::GPUAdapter> Hardware::GPU::D3DKMTGPUBase::CreateAdapter(PWSTR DeviceInterface, D3DKMT_HANDLE AdapterHandle, const LUID& AdapterLuid, ULONG NumberOfSegments, ULONG NumberOfNodes)
{
	GPUAdapter Adapter{};
	if (D3DKMTHelper::QueryDeviceProperties(DeviceInterface, Adapter.Descriptor, Adapter.DriverDate, Adapter.DriverVersion, Adapter.LocationInfo, Adapter.InstalledMemory))
	{
		D3DKMTHelper::QueryAdapterMemoryInformation(AdapterHandle, AdapterLuid, Adapter.SharedMemory, Adapter.DedicatedMemory);

		for (ULONG i = 0; i < NumberOfNodes; i++)
		{
			D3DKMT_NODEMETADATA metaDataInfo{};
			metaDataInfo.NodeOrdinalAndAdapterIndex = MAKEWORD(i, 0);
			GPUNode Node{};
			Node.Index = i;
			if (NT_SUCCESS(D3DKMTHelper::QueryAdapterInformation(
				AdapterHandle,
				KMTQAITYPE_NODEMETADATA,
				&metaDataInfo,
				sizeof(D3DKMT_NODEMETADATA)
			)))
			{
				Node.Name = D3DKMTHelper::GetNodeEngineTypeString(metaDataInfo);
			}

			if (Node.Name.empty())
			{
				Node.Name = std::format("Engine {}", i);
			}
			Adapter.Nodes.emplace_back(Node);
		}
		Adapter.DeviceInterface = DeviceInterface;
		Adapter.SegmentCount = NumberOfSegments;
		Adapter.AdapterLuid = AdapterLuid;
		return std::make_unique<GPUAdapter>(std::move(Adapter));
	}
	return nullptr;
}