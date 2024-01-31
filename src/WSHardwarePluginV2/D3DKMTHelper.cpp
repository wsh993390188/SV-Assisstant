#include "stdafx.h"
#include "D3DKMTHelper.h"
#include <devpkey.h>
#include "DXGIGPUBase.h"

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

namespace
{
	std::wstring FormatDate(
		_In_ PSYSTEMTIME Date,
		_In_ PWSTR Format
	)
	{
		auto bufferSize = GetDateFormat(LOCALE_USER_DEFAULT, 0, Date, Format, NULL, 0);
		std::wstring string;
		string.resize(bufferSize);
		GetDateFormat(LOCALE_USER_DEFAULT, 0, Date, Format, string.data(), string.size());
		return string;
	}
}

NTSTATUS Hardware::D3DKMTHelper::QueryAdapterInformation(D3DKMT_HANDLE AdapterHandle, KMTQUERYADAPTERINFOTYPE InformationClass, PVOID Information, UINT32 InformationLength)
{
	D3DKMT_QUERYADAPTERINFO queryAdapterInfo;

	memset(&queryAdapterInfo, 0, sizeof(D3DKMT_QUERYADAPTERINFO));

	queryAdapterInfo.AdapterHandle = AdapterHandle;
	queryAdapterInfo.Type = InformationClass;
	queryAdapterInfo.PrivateDriverData = Information;
	queryAdapterInfo.PrivateDriverDataSize = InformationLength;

	return D3DKMTQueryAdapterInfo(&queryAdapterInfo);
}

bool Hardware::D3DKMTHelper::IsGpuSoftwareDevice(D3DKMT_HANDLE AdapterHandle)
{
	D3DKMT_ADAPTERTYPE adapterType{};

	if (NT_SUCCESS(QueryAdapterInformation(
		AdapterHandle,
		KMTQAITYPE_ADAPTERTYPE,
		&adapterType,
		sizeof(D3DKMT_ADAPTERTYPE)
	)))
	{
		if (adapterType.SoftwareDevice) // adapterType.HybridIntegrated
		{
			return true;
		}
	}

	return false;
}

bool Hardware::D3DKMTHelper::CloseAdapterHandle(D3DKMT_HANDLE AdapterHandle)
{
	D3DKMT_CLOSEADAPTER closeAdapter;

	memset(&closeAdapter, 0, sizeof(D3DKMT_CLOSEADAPTER));
	closeAdapter.AdapterHandle = AdapterHandle;

	return NT_SUCCESS(D3DKMTCloseAdapter(&closeAdapter));
}

bool Hardware::D3DKMTHelper::QueryDeviceProperties(PWSTR DeviceInterface, std::wstring& Description, std::wstring& DriverDate, std::wstring& DriverVersion, std::wstring& LocationInfo, ULONG64& InstalledMemory)
{
	DEVPROPTYPE devicePropertyType;
	DEVINST deviceInstanceHandle;
	ULONG deviceInstanceIdLength = MAX_DEVICE_ID_LEN;
	WCHAR deviceInstanceId[MAX_DEVICE_ID_LEN];

	if (CM_Get_Device_Interface_Property(
		DeviceInterface,
		&DEVPKEY_Device_InstanceId,
		&devicePropertyType,
		(PBYTE)deviceInstanceId,
		&deviceInstanceIdLength,
		0
	) != CR_SUCCESS)
	{
		return false;
	}

	if (CM_Locate_DevNode(
		&deviceInstanceHandle,
		deviceInstanceId,
		CM_LOCATE_DEVNODE_NORMAL
	) != CR_SUCCESS)
	{
		return false;
	}

	Description = QueryDeviceProperty(deviceInstanceHandle, &DEVPKEY_Device_DeviceDesc);
	DriverDate = QueryDeviceProperty(deviceInstanceHandle, &DEVPKEY_Device_DriverDate);
	DriverVersion = QueryDeviceProperty(deviceInstanceHandle, &DEVPKEY_Device_DriverVersion);
	LocationInfo = QueryDeviceProperty(deviceInstanceHandle, &DEVPKEY_Device_LocationInfo);
	InstalledMemory = QueryGpuInstalledMemory(deviceInstanceHandle);

	// Undocumented device properties (Win10 only)
	//DEFINE_DEVPROPKEY(DEVPKEY_Gpu_Luid, 0x60b193cb, 0x5276, 0x4d0f, 0x96, 0xfc, 0xf1, 0x73, 0xab, 0xad, 0x3e, 0xc6, 2); // DEVPROP_TYPE_UINT64
	//DEFINE_DEVPROPKEY(DEVPKEY_Gpu_PhysicalAdapterIndex, 0x60b193cb, 0x5276, 0x4d0f, 0x96, 0xfc, 0xf1, 0x73, 0xab, 0xad, 0x3e, 0xc6, 3); // DEVPROP_TYPE_UINT32

	return true;
}

std::wstring Hardware::D3DKMTHelper::QueryDeviceProperty(
	_In_ DEVINST DeviceHandle,
	_In_ CONST DEVPROPKEY* DeviceProperty
)
{
	ULONG bufferSize = 0;
	DEVPROPTYPE propertyType = DEVPROP_TYPE_EMPTY;
	std::unique_ptr<BYTE[]> buffer;
	if (auto result = CM_Get_DevNode_Property(
		DeviceHandle,
		DeviceProperty,
		&propertyType,
		NULL,
		&bufferSize,
		0
	); result == CR_BUFFER_SMALL)
	{
		buffer = std::make_unique<BYTE[]>(bufferSize);
	}

	if (auto result = CM_Get_DevNode_Property(
		DeviceHandle,
		DeviceProperty,
		&propertyType,
		buffer.get(),
		&bufferSize,
		0
	); result == CR_SUCCESS)
	{
		switch (propertyType)
		{
		case DEVPROP_TYPE_STRING:
		{
			std::wstring string(reinterpret_cast<PWCHAR>(buffer.get()), (bufferSize - 1) / sizeof(TCHAR));
			return string;
		}
		break;
		case DEVPROP_TYPE_FILETIME:
		{
			SYSTEMTIME systemTime;
			auto fileTime = reinterpret_cast<PFILETIME>(buffer.get());
			if (FileTimeToSystemTime(fileTime, &systemTime))
			{
				return FormatDate(&systemTime, NULL);
			}
		}
		break;
		case DEVPROP_TYPE_UINT32:
		{
			return std::to_wstring(*reinterpret_cast<PULONG>(buffer.get()));
		}
		break;
		case DEVPROP_TYPE_UINT64:
		{
			return std::to_wstring(*reinterpret_cast<PULONG64>(buffer.get()));
		}
		break;
		}
	}
	return {};
}

ULONG64 Hardware::D3DKMTHelper::QueryGpuInstalledMemory(DEVINST DeviceHandle)
{
	// Todo Implement this function
	return 0;
}

bool Hardware::D3DKMTHelper::QueryAdapterMemoryInformation(D3DKMT_HANDLE AdapterHandle, const LUID& AdapterLUID, uint64_t& SharedMemory, uint64_t& DedicatedMemory)
{
	if (Utils::GetWindowsVersion() > WINDOWS_10_RS4)
	{
		D3DKMT_SEGMENTSIZEINFO segmentInfo;
		memset(&segmentInfo, 0, sizeof(D3DKMT_SEGMENTSIZEINFO));

		if (NT_SUCCESS(QueryAdapterInformation(
			AdapterHandle,
			KMTQAITYPE_GETSEGMENTSIZE,
			&segmentInfo,
			sizeof(D3DKMT_SEGMENTSIZEINFO)
		)))
		{
			SharedMemory = segmentInfo.DedicatedVideoMemorySize;
			DedicatedMemory = segmentInfo.SharedSystemMemorySize;
			return true;
		}
	}

	GPU::GPUDXBaseData GPUBase;
	if (GPU::DXGIGPUBase::Instance().QueryGPUInfo(AdapterLUID, GPUBase) == Data::ErrorType::SUCCESS)
	{
		SharedMemory = GPUBase.SharedSystemMemory;
		DedicatedMemory = GPUBase.DedicatedVideoMemory;
		return true;
	}

	return false;
}

std::string Hardware::D3DKMTHelper::GetNodeEngineTypeString(
	_In_ D3DKMT_NODEMETADATA NodeMetaData
)
{
	switch (NodeMetaData.NodeData.EngineType)
	{
	case DXGK_ENGINE_TYPE_OTHER:
		return Utils::wstringToUtf8(NodeMetaData.NodeData.FriendlyName);
	case DXGK_ENGINE_TYPE_3D:
		return "3D";
	case DXGK_ENGINE_TYPE_VIDEO_DECODE:
		return "Video Decode";
	case DXGK_ENGINE_TYPE_VIDEO_ENCODE:
		return "Video Encode";
	case DXGK_ENGINE_TYPE_VIDEO_PROCESSING:
		return "Video Processing";
	case DXGK_ENGINE_TYPE_SCENE_ASSEMBLY:
		return "Scene Assembly";
	case DXGK_ENGINE_TYPE_COPY:
		return "Copy";
	case DXGK_ENGINE_TYPE_OVERLAY:
		return "Overlay";
	case DXGK_ENGINE_TYPE_CRYPTO:
		return "Crypto";

	default:
		spdlog::debug("unknown engine type : {}", int(NodeMetaData.NodeData.EngineType));
	}
	return {};
}

bool Hardware::D3DKMTHelper::UpdateNodeInformation(const LUID& AdapterLUID, const ULONG& NodeId, uint64_t& NewValue)
{
	D3DKMT_QUERYSTATISTICS queryStatistics{};
	queryStatistics.Type = D3DKMT_QUERYSTATISTICS_NODE;
	queryStatistics.AdapterLuid = AdapterLUID;
	queryStatistics.QueryNode.NodeId = NodeId;

	if (NT_SUCCESS(D3DKMTQueryStatistics((const D3DKMT_QUERYSTATISTICS*)&queryStatistics)))
	{
		NewValue = queryStatistics.QueryResult.NodeInformation.GlobalInformation.RunningTime.QuadPart;
	}

	// Todo 对优化代码做分析,为啥不加这句就出毛病
	const auto FKComplier = std::to_wstring(NodeId);
	return true;
}
