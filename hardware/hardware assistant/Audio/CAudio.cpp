#include "stdafx.h"
#include <memory>
#include <wrl.h>
#include <propvarutil.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include "CAudio.h"

using namespace SV_ASSIST::AUDIO;

namespace {

	using prop_string_t = std::unique_ptr<WCHAR, decltype(&CoTaskMemFree)>;

#define prop_string_null prop_string_t(nullptr, CoTaskMemFree)

	prop_string_t GetDeviceId(IN Microsoft::WRL::ComPtr<IMMDevice> const& device) {
		LPWSTR buffer = nullptr;
		if SUCCEEDED(device->GetId(&buffer)) {
			return prop_string_t(buffer, CoTaskMemFree);
		}
		return prop_string_null;
	}

	std::wstring GetDeviceState(IN Microsoft::WRL::ComPtr<IMMDevice> const& device) {
		DWORD state = 0;
		if SUCCEEDED(device->GetState(&state)) {
			switch (state) {
			case DEVICE_STATE_ACTIVE: return L"ACTIVE";
			case DEVICE_STATE_DISABLED: return L"DISABLED";
			case DEVICE_STATE_NOTPRESENT: return L"NOTPRESENT";
			case DEVICE_STATE_UNPLUGGED: return L"UNPLUGGED";
			}
		}
		return L"UNKNOWN";
	}

	prop_string_t GetPropString(IN Microsoft::WRL::ComPtr<IPropertyStore> const& prop,
		IN PROPERTYKEY const& key) {
		PROPVARIANT var;
		::PropVariantInit(&var);
		if SUCCEEDED(prop->GetValue(key, &var)) {
			LPWSTR str;
			if SUCCEEDED(::PropVariantToStringAlloc(var, &str)) {
				::PropVariantClear(&var);
				return prop_string_t(str, CoTaskMemFree);
			}
		}
		::PropVariantClear(&var);
		return prop_string_null;
	}

} // end of anonymous namespace

class CAudio
{
public:
	void Exec();
	void UpdateData();
	const std::vector<AudioStruct>& GetOutputAudio();
	static std::shared_ptr<CAudio> AudioInstance;
private:
	std::vector<AudioStruct> Output;
};

std::shared_ptr<CAudio> CAudio::AudioInstance = std::make_shared<CAudio>();

void SV_ASSIST::AUDIO::Exec()
{
	CAudio::AudioInstance->Exec();
}

void SV_ASSIST::AUDIO::UpdateData()
{
	CAudio::AudioInstance->UpdateData();
}

const std::vector<AudioStruct>& SV_ASSIST::AUDIO::GetOutputAudio()
{
	return CAudio::AudioInstance->GetOutputAudio();
}

void CAudio::Exec()
{
	if SUCCEEDED(::CoInitializeEx(nullptr, COINIT_MULTITHREADED))
	{
		{
			Microsoft::WRL::ComPtr<IMMDeviceEnumerator> enumerator;
			if SUCCEEDED(::CoCreateInstance(__uuidof(MMDeviceEnumerator),
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(enumerator.ReleaseAndGetAddressOf()))) {
				Microsoft::WRL::ComPtr<IMMDeviceCollection> devices;
				if SUCCEEDED(enumerator->EnumAudioEndpoints(EDataFlow::eAll, // eRender, eCapture, eAll
					DEVICE_STATEMASK_ALL,
					devices.ReleaseAndGetAddressOf())) {
					UINT uNumDevices;
					if SUCCEEDED(devices->GetCount(&uNumDevices)) {
						for (UINT i = 0; i < uNumDevices; ++i) {
							Microsoft::WRL::ComPtr<IMMDevice> device;
							if SUCCEEDED(devices->Item(i, device.ReleaseAndGetAddressOf()))
							{
								auto const id = GetDeviceId(device); 
								auto const state = GetDeviceState(device); 
								Microsoft::WRL::ComPtr<IPropertyStore> prop;
								if SUCCEEDED(device->OpenPropertyStore(STGM_READ, prop.ReleaseAndGetAddressOf())) {
									auto const name = GetPropString(prop, PKEY_Device_FriendlyName);
									auto const desc = GetPropString(prop, PKEY_Device_DeviceDesc);
									auto const audioif = GetPropString(prop, PKEY_DeviceInterface_FriendlyName);
									AudioStruct temp = { id.get()? id.get() : L"Unknown", state, name.get() ? name.get() : L"Unknown", desc.get() ? desc.get() : L"Unknown", audioif.get() ? audioif.get() : L"Unknown"};
									Output.emplace_back(temp);
								}
							}
						}
					}
				}
			}
		}
		::CoUninitialize();
	}
}

void CAudio::UpdateData()
{
	Output.clear();
	this->Exec();
}

const std::vector<AudioStruct>& CAudio::GetOutputAudio()
{
	return this->Output;
}
