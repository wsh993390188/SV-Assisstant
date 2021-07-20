#pragma once
extern "C"
{
#include "NTD3DKMT.h"
}
namespace Hardware
{
	namespace D3DKMTHelper
	{
		NTSTATUS QueryAdapterInformation(
			_In_ D3DKMT_HANDLE AdapterHandle,
			_In_ KMTQUERYADAPTERINFOTYPE InformationClass,
			_Out_writes_bytes_opt_(InformationLength) PVOID Information,
			_In_ UINT32 InformationLength
		);

		bool IsGpuSoftwareDevice(
			_In_ D3DKMT_HANDLE AdapterHandle
		);

		bool CloseAdapterHandle(
			_In_ D3DKMT_HANDLE AdapterHandle
		);

		_Success_(return)
			bool QueryDeviceProperties(
				_In_ PWSTR DeviceInterface,
				_Out_ std::wstring& Description,
				_Out_ std::wstring& DriverDate,
				_Out_ std::wstring& DriverVersion,
				_Out_ std::wstring& LocationInfo,
				_Out_ ULONG64& InstalledMemory
			);
		std::wstring QueryDeviceProperty(DEVINST DeviceHandle, const DEVPROPKEY* DeviceProperty);

		ULONG64 QueryGpuInstalledMemory(DEVINST DeviceHandle);

		bool QueryAdapterMemoryInformation(_In_ D3DKMT_HANDLE  AdapterHandle, const LUID& AdapterLUID, uint64_t& SharedMemory, uint64_t& DedicatedMemory);
		std::string GetNodeEngineTypeString(D3DKMT_NODEMETADATA NodeMetaData);

		bool UpdateNodeInformation(_In_ const LUID& AdapterLUID, _In_ const ULONG NodeId, uint64_t& NewValue);
	}
}
