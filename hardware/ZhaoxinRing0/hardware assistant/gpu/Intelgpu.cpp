#include "stdafx.h"
#include "Intelgpu.h"

#include <wrl.h>
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

#define GGF_SUCCESS 0
#define GGF_ERROR					-1
#define GGF_E_UNSUPPORTED_HARDWARE	-2
#define GGF_E_UNSUPPORTED_DRIVER	-3
#define GGF_E_D3D_ERROR				-4

#define INTEL_VENDOR_ID 0x8086

#define INTEL_DEVICE_INFO_COUNTERS         "Intel Device Information"

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

/******************************************************************************************************************************************
* getIntelDeviceInfo
*
* Description:
*       Gets device information which is stored in a D3D counter. First, a D3D device must be created, the Intel counter located, and
*       finally queried.
*
*       Supported device info: GPU Max Frequency, GPU Min Frequency, GT Generation, EU Count, Package TDP, Max Fill Rate
*
* Parameters:
*         unsigned int VendorId                         - [in]     - Input:  system's vendor id
*         IntelDeviceInfoHeader *pIntelDeviceInfoHeader - [in/out] - Input:  allocated IntelDeviceInfoHeader *
*                                                                    Output: Intel device info header, if found
*         void *pIntelDeviceInfoBuffer                  - [in/out] - Input:  allocated void *
*                                                                    Output: IntelDeviceInfoV[#], cast based on IntelDeviceInfoHeader
* Return:
*         GGF_SUCCESS: Able to find Data is valid
*         GGF_E_UNSUPPORTED_HARDWARE: Unsupported hardware, data is invalid
*         GGF_E_UNSUPPORTED_DRIVER: Unsupported driver on Intel, data is invalid
*
*****************************************************************************************************************************************/

long CIntelGPU::getIntelDeviceInfo(unsigned int VendorId, IntelDeviceInfoHeader *pIntelDeviceInfoHeader, void *pIntelDeviceInfoBuffer)
{
	if (pIntelDeviceInfoBuffer == NULL) {
		return GGF_ERROR;
	}

	if (VendorId != INTEL_VENDOR_ID) {
		return GGF_E_UNSUPPORTED_HARDWARE;
	}

	//
	// First create the Device, must be SandyBridge or later to create D3D11 device
	//
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = NULL;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pImmediateContext = NULL;
	HRESULT hr = NULL;

	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &pDevice, NULL, &pImmediateContext);

	if (FAILED(hr))
	{
		printf("D3D11CreateDevice failed\n");

		return GGF_ERROR;
	}

	//
	// Query the device to find the number of device dependent counters. If LastDeviceDependentCounter
	// is 0 then there are no device dependent counters supported on this device.
	//
	D3D11_COUNTER_INFO counterInfo;
	int numDependentCounters;

	ZeroMemory(&counterInfo, sizeof(D3D11_COUNTER_INFO));

	pDevice->CheckCounterInfo(&counterInfo);

	if (counterInfo.LastDeviceDependentCounter == 0)
	{
		printf("No device dependent counters\n");

		return GGF_E_UNSUPPORTED_DRIVER;
	}

	numDependentCounters = (counterInfo.LastDeviceDependentCounter - D3D11_COUNTER_DEVICE_DEPENDENT_0) + 1;

	//
	// Since there is at least 1 counter, we search for the apporpriate counter - INTEL_DEVICE_INFO_COUNTERS
	//
	D3D11_COUNTER_DESC pIntelCounterDesc;
	UINT uiSlotsRequired, uiNameLength, uiUnitsLength, uiDescLength;
	LPSTR sName, sUnits, sDesc;

	ZeroMemory(&pIntelCounterDesc, sizeof(D3D11_COUNTER_DESC));

	for (int i = 0; i < numDependentCounters; ++i)
	{
		D3D11_COUNTER_DESC counterDescription;
		D3D11_COUNTER_TYPE counterType;

		counterDescription.Counter = static_cast<D3D11_COUNTER>(i + D3D11_COUNTER_DEVICE_DEPENDENT_0);
		counterDescription.MiscFlags = 0;
		uiSlotsRequired = uiNameLength = uiUnitsLength = uiDescLength = 0;
		sName = sUnits = sDesc = NULL;

		if (SUCCEEDED(hr = pDevice->CheckCounter(&counterDescription, &counterType, &uiSlotsRequired, NULL, &uiNameLength, NULL, &uiUnitsLength, NULL, &uiDescLength)))
		{
			if (uiNameLength == 0 || uiUnitsLength == 0 || uiDescLength == 0)
			{
				return GGF_ERROR;
			}

			LPSTR sName = new char[uiNameLength];
			LPSTR sUnits = new char[uiUnitsLength];
			LPSTR sDesc = new char[uiDescLength];

			if (SUCCEEDED(hr = pDevice->CheckCounter(&counterDescription, &counterType, &uiSlotsRequired, sName, &uiNameLength, sUnits, &uiUnitsLength, sDesc, &uiDescLength)))
			{
				if (strcmp(sName, INTEL_DEVICE_INFO_COUNTERS) == 0)
				{
					int IntelCounterMajorVersion = 0;
					int IntelCounterSize = 0;

					pIntelCounterDesc.Counter = counterDescription.Counter;

					sscanf_s(sDesc, "Version %d", &IntelCounterMajorVersion);

					if (IntelCounterMajorVersion == 2)
					{
						IntelCounterSize = sizeof(IntelDeviceInfoV2);
					}
					else // Fall back to version 1.0
					{
						IntelCounterMajorVersion = 1;
						IntelCounterSize = sizeof(IntelDeviceInfoV1);
					}

					pIntelDeviceInfoHeader->Version = IntelCounterMajorVersion;
					pIntelDeviceInfoHeader->Size = IntelCounterSize;
				}
			}

			delete[] sName;
			delete[] sUnits;
			delete[] sDesc;
		}
	}

	//
	// Check if the Device Info Counter was found. If not, then it means
	// the driver doesn't support this counter.
	//
	if (pIntelCounterDesc.Counter == NULL)
	{
		OutputDebugString(L"Could not find counter\n");

		return GGF_E_UNSUPPORTED_DRIVER;
	}

	//
	// Now we know the driver supports the counter we are interested in. So create it and
	// capture the data we want.
	//
	Microsoft::WRL::ComPtr<ID3D11Counter> pIntelCounter = NULL;

	hr = pDevice->CreateCounter(&pIntelCounterDesc, &pIntelCounter);
	if (FAILED(hr))
	{
		OutputDebugString(L"CreateCounter failed\n");

		return GGF_E_D3D_ERROR;
	}

	pImmediateContext->Begin(pIntelCounter.Get());
	pImmediateContext->End(pIntelCounter.Get());

	hr = pImmediateContext->GetData(pIntelCounter.Get(), NULL, 0, 0);
	if (FAILED(hr) || hr == S_FALSE)
	{
		OutputDebugString(L"Getdata failed \n");

		return GGF_E_D3D_ERROR;
	}

	//
	// GetData will return the address of the data, not the actual data.
	//
	DWORD pData[2] = { 0 };
	hr = pImmediateContext->GetData(pIntelCounter.Get(), pData, 2 * sizeof(DWORD), 0);
	if (FAILED(hr) || hr == S_FALSE)
	{
		OutputDebugString(L"Getdata failed \n");
		return GGF_E_D3D_ERROR;
	}

	//
	// Copy data to passed in parameter and clean up
	//
	void *pDeviceInfoBuffer = *(void**)pData;
	memcpy(pIntelDeviceInfoBuffer, pDeviceInfoBuffer, pIntelDeviceInfoHeader->Size);

	return GGF_SUCCESS;
}

GPUTypes CIntelGPU::exec()
{
	GPUTypes re = OTHERS_GPU;
	IntelGPUInfo temp = {};
	if (true == getGraphicsDeviceInfo(&temp.VendorId, &temp.DeviceId, &temp.VideoMemory, &temp.FUllName))
	{
		//
		// In DirectX, Intel exposes additional information through the driver that can be obtained
		// querying a special DX counter
		//
		IntelDeviceInfoHeader intelDeviceInfoHeader = { 0 };
		unsigned char intelDeviceInfoBuffer[1024];

		if (GGF_SUCCESS == getIntelDeviceInfo(temp.VendorId, &intelDeviceInfoHeader, &intelDeviceInfoBuffer))
		{
			if (intelDeviceInfoHeader.Version == 2)
			{
				memcpy(&temp.DeviceInfo, intelDeviceInfoBuffer, intelDeviceInfoHeader.Size);
			}
			else if (intelDeviceInfoHeader.Version == 1)
			{
				memcpy(&temp.DeviceInfo, intelDeviceInfoBuffer, intelDeviceInfoHeader.Size);
			}
			else
			{
				OutputDebugString(L"Unrecognized Intel Device Header version\n");
			}
		}

		this->Intelinfo.emplace_back(temp);
		re = INTEL_GPU;
	}
	return re;
}

const void * CIntelGPU::Returninfo()
{
	return &this->Intelinfo;
}

GPUTypes CIntelGPU::UpdateData()
{
	for (auto& temp : Intelinfo)
	{
		//
		// In DirectX, Intel exposes additional information through the driver that can be obtained
		// querying a special DX counter
		//
		IntelDeviceInfoHeader intelDeviceInfoHeader = { 0 };
		unsigned char intelDeviceInfoBuffer[1024];

		if (GGF_SUCCESS == getIntelDeviceInfo(temp.VendorId, &intelDeviceInfoHeader, &intelDeviceInfoBuffer))
		{
			if (intelDeviceInfoHeader.Version == 2)
			{
				memcpy(&temp.DeviceInfo, intelDeviceInfoBuffer, intelDeviceInfoHeader.Size);
			}
			else if (intelDeviceInfoHeader.Version == 1)
			{
				memcpy(&temp.DeviceInfo, intelDeviceInfoBuffer, intelDeviceInfoHeader.Size);
			}
			else
			{
				OutputDebugString(L"Unrecognized Intel Device Header version\n");
			}
		}
	}
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
