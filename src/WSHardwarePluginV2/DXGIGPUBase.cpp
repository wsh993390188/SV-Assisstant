#include "stdafx.h"
#include <atlcomcli.h>
#include "DXGIGPUBase.h"
#include "GPUDeviceFactory.h"

using namespace Hardware;

GPU::DXGIGPUBase& Hardware::GPU::DXGIGPUBase::Instance()
{
	static DXGIGPUBase inst;
	return inst;
}

Hardware::GPU::DXGIGPUBase::DXGIGPUBase() : hDxgi(nullptr), pCreateDXGIFactory(nullptr)
{
	hDxgi = ::LoadLibrary(_T("dxgi.dll"));
	if (hDxgi)
	{
		pCreateDXGIFactory = (LPCREATEDXGIFACTORY) ::GetProcAddress(hDxgi, "CreateDXGIFactory");
		Initialize();
	}
	else
	{
		Utils::ErrorCode("load dxgi error");
	}
}

Hardware::GPU::DXGIGPUBase::~DXGIGPUBase()
{
	if (hDxgi)
		::FreeLibrary(hDxgi);
}

Data::ErrorType Hardware::GPU::DXGIGPUBase::Initialize()
{
	GPUDatas.clear();
	return InitializeDevice();
}

Data::ErrorType Hardware::GPU::DXGIGPUBase::QueryGPUInfo(const GPUQuery& query, GPUDXBaseData& DxData)
{
	if (auto data = GPUDatas.find(query); data != GPUDatas.end())
	{
		DxData = data->second;
		return Data::ErrorType::SUCCESS;
	}

	spdlog::error("Not Find Device VID:{:x}, DID:{:x} On DXGI", query.VID, query.DID);
	return Data::ErrorType::DATAEMPTY;
}

Data::ErrorType Hardware::GPU::DXGIGPUBase::InitializeDevice()
{
	spdlog::info("Init Dxgi");
	//
	// We have the CreateDXGIFactory function so use it to actually create the factory and enumerate
	// through the adapters. Here, we are specifically looking for the Intel gfx adapter.
	//
	CComQIPtr<IDXGIFactory> factory = nullptr;
	if (auto hr = pCreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&factory)); FAILED(hr))
	{
		spdlog::error("pCreateDXGIFactory Error: {}", std::system_category().message(hr).c_str());
		return Data::ErrorType::COMCREATEERROR;
	}

	// Loop through existing adapters
	for (UINT i = 0;; ++i) {
		// Get next the adapter
		CComPtr<IDXGIAdapter> adapter;
		auto err = factory->EnumAdapters(i, &adapter);
		if (FAILED(err) || !adapter)
		{
			break;
		}

		// basic DXGI information
		DXGI_ADAPTER_DESC AdapterDesc = {};
		if (auto hr = adapter->GetDesc(&AdapterDesc); FAILED(hr))
		{
			spdlog::error("Cant Read device Desc, {}", std::system_category().message(hr).c_str());
			continue;
		}

		// 			CComQIPtr<ID3D11Device> device;
		// 			if (FAILED(pD3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &device, nullptr, nullptr)) || !device)
		// 			{
		// 				spdlog::error(_T("Cant create device from dx11"));
		// 				continue;
		// 			}

		GPUDXBaseData gpuData{};

		if (AdapterDesc.VendorId == IntelVendorId)
		{
			bool isUMAArchitecture = false;
			if (AdapterDesc.DedicatedVideoMemory <= 512 * 1024 * 1024)
			{
				isUMAArchitecture = true;
			}
		}

		gpuData.Description = AdapterDesc.Description;
		gpuData.AdapterLuid = AdapterDesc.AdapterLuid;
		gpuData.SubSysId = AdapterDesc.SubSysId;
		gpuData.Revision = AdapterDesc.Revision;
		gpuData.DedicatedVideoMemory = AdapterDesc.DedicatedVideoMemory;
		gpuData.DedicatedSystemMemory = AdapterDesc.DedicatedSystemMemory;
		gpuData.SharedSystemMemory = AdapterDesc.SharedSystemMemory;
		spdlog::info("Find Devive VID:{:x}, DID:{:x} on Dxgi", AdapterDesc.VendorId, AdapterDesc.DeviceId);
		GPUDatas.emplace(GPUQuery(AdapterDesc.VendorId, AdapterDesc.DeviceId), std::move(gpuData));
	}
	spdlog::info("End init Dxgi");
	return Data::ErrorType::SUCCESS;
}