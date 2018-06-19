#include "stdafx.h"
#include "Intelgpu.h"

#include <InitGuid.h>
#include <D3D11.h>
#include <D3DCommon.h>
#ifdef _WIN32_WINNT_WIN10 
#include <DXGI1_4.h>
#else
#include <DXGI1_3.h>
#endif

#include <oleauto.h>
#include <wbemidl.h>
#include <ObjBase.h>

CIntelGPU::CIntelGPU() : Intelinfo{}
{
}

CIntelGPU::~CIntelGPU()
{
}

bool CIntelGPU::CheckdllStatus()
{
	return false;
}

CGPU::GPUTypes CIntelGPU::exec()
{
	CGPU::GPUTypes re = CGPU::OTHERS_GPU;
	unsigned int VendorId, DeviceId;
	unsigned __int64 VideoMemory;
	if (true == getGraphicsDeviceInfo(&VendorId, &DeviceId, &VideoMemory, &Intelinfo.FUllName))
		re = INTEL_GPU;
	return re;
}

const void * CIntelGPU::Returninfo()
{
	return &this->Intelinfo;
}

CGPU::GPUTypes CIntelGPU::UpdateData()
{
	return INTEL_GPU;
}

bool CIntelGPU::getGraphicsDeviceInfo(unsigned int * VendorId, unsigned int * DeviceId, unsigned __int64 * VideoMemory, std::wstring * GFXBrand)
{
		if ((VendorId == NULL) || (DeviceId == NULL)) {
			return false;
		}

		//
		// DXGI is supported on Windows Vista and later. Define a function pointer to the
		// CreateDXGIFactory function. DXGIFactory1 is supported by Windows Store Apps so
		// try that first.
		//
		HMODULE hDXGI = LoadLibrary(L"dxgi.dll");
		if (hDXGI == NULL) {
			return false;
		}

		typedef HRESULT(WINAPI*LPCREATEDXGIFACTORY)(REFIID riid, void** ppFactory);

		LPCREATEDXGIFACTORY pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory1");
		if (pCreateDXGIFactory == NULL) {
			pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory");

			if (pCreateDXGIFactory == NULL) {
				FreeLibrary(hDXGI);
				return false;
			}
		}

		//
		// We have the CreateDXGIFactory function so use it to actually create the factory and enumerate
		// through the adapters. Here, we are specifically looking for the Intel gfx adapter. 
		//
		IDXGIAdapter*     pAdapter;
		IDXGIFactory*     pFactory;
		DXGI_ADAPTER_DESC AdapterDesc;
		if (FAILED((*pCreateDXGIFactory)(__uuidof(IDXGIFactory), (void**)(&pFactory)))) {
			FreeLibrary(hDXGI);
			return false;
		}

		if (FAILED(pFactory->EnumAdapters(0, (IDXGIAdapter**)&pAdapter))) {
			FreeLibrary(hDXGI);
			return false;
		}

		unsigned int intelAdapterIndex = 0;
		while (pFactory->EnumAdapters(intelAdapterIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
			pAdapter->GetDesc(&AdapterDesc);
			if (AdapterDesc.VendorId == 0x8086) {
				break;
			}
			intelAdapterIndex++;
		}

		if (pAdapter == NULL) {
			FreeLibrary(hDXGI);
			return false;
		}
		*VendorId = AdapterDesc.VendorId;
		*DeviceId = AdapterDesc.DeviceId;
		*GFXBrand = AdapterDesc.Description;

		//
		// If we are on Windows 10 then the Adapter3 interface should be available. This is recommended over using
		// the AdapterDesc for getting the amount of memory available.
		//
#ifdef _WIN32_WINNT_WIN10
		IDXGIAdapter3* pAdapter3;
		pAdapter->QueryInterface(IID_IDXGIAdapter3, (void**)&pAdapter3);
		if (pAdapter3) {
			DXGI_QUERY_VIDEO_MEMORY_INFO memInfo;
			pAdapter3->QueryVideoMemoryInfo(intelAdapterIndex, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo);
			*VideoMemory = memInfo.Budget;
			pAdapter3->Release();
		}
		else
#endif // DEBUG
		{
			*VideoMemory = (unsigned __int64)(AdapterDesc.DedicatedVideoMemory + AdapterDesc.SharedSystemMemory);
		}


		pAdapter->Release();
		FreeLibrary(hDXGI);
		return true;
}
