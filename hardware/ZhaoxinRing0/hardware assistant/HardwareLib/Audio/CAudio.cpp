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
	const std::vector<AudioStruct>& GetOutputAudio() const;
	static CAudio* Instance()
	{
		if (!AudioInstance.get())
			AudioInstance = std::make_shared<CAudio>();
		return AudioInstance.get();
	}
private:
	static std::shared_ptr<CAudio> AudioInstance;
	std::vector<AudioStruct> Output;
};

std::shared_ptr<CAudio> CAudio::AudioInstance = nullptr;

void SV_ASSIST::AUDIO::UpdateData()
{
	CAudio::Instance()->UpdateData();
}

const std::vector<AudioStruct>& SV_ASSIST::AUDIO::GetOutputAudio()
{
	return CAudio::Instance()->GetOutputAudio();
}

void CAudio::Exec()
{
#ifdef ZX_OutputLog
	Logger::Instance()->OutputLogInfo(el::Level::Debug, "***** Audio info *****");
#endif
	if SUCCEEDED(::CoInitialize(nullptr))
	{
		{
			Microsoft::WRL::ComPtr<IMMDeviceEnumerator> enumerator;
			if SUCCEEDED(::CoCreateInstance(__uuidof(MMDeviceEnumerator),
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(enumerator.ReleaseAndGetAddressOf()))) {
#ifdef ZX_OutputLog
				Logger::Instance()->OutputLogInfo(el::Level::Debug, "Initialize Audio COM Success");
#endif 
				Microsoft::WRL::ComPtr<IMMDeviceCollection> devices;
				if SUCCEEDED(enumerator->EnumAudioEndpoints(EDataFlow::eAll, // eRender, eCapture, eAll
					DEVICE_STATEMASK_ALL,
					devices.ReleaseAndGetAddressOf())) {
#ifdef ZX_OutputLog
					Logger::Instance()->OutputLogInfo(el::Level::Debug, "Enuming Audio Device");
#endif 
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
									AudioStruct temp{ id.get()? id.get() : L"Unknown", state, name.get() ? name.get() : L"Unknown", desc.get() ? desc.get() : L"Unknown", audioif.get() ? audioif.get() : L"Unknown"};
									Output.emplace_back(temp);
								}
							}
						}
					}
#ifdef ZX_OutputLog
					else
						Logger::Instance()->OutputLogInfo(el::Level::Debug, "Get Device Count Failed, Maybe haven't Audio Device");
#endif
				}
#ifdef ZX_OutputLog
				else
					Logger::Instance()->OutputLogInfo(el::Level::Debug, "Enum Audio Endpoints Failed");
#endif
			}
#ifdef ZX_OutputLog
			else
				Logger::Instance()->OutputLogInfo(el::Level::Debug, "Create Instance Failed");
#endif
		}
		::CoUninitialize();
	}
#ifdef ZX_OutputLog
	else
		Logger::Instance()->OutputLogInfo(el::Level::Debug, "Initialize COM Failed");
#endif

#ifdef ZX_OutputLog
	int count = 0;
	for (const auto& i : Output)
	{
		Logger::Instance()->OutputLogInfo(el::Level::Debug, std::wstring(L"Audio #") + std::to_wstring(++count) + std::wstring(L" Name: ") + i.name);
	}
	Logger::Instance()->OutputLogInfo(el::Level::Debug, "***** End Audio info *****\n");
#endif
}

void CAudio::UpdateData()
{
	Output.clear();
	this->Exec();
}

const std::vector<AudioStruct>& CAudio::GetOutputAudio() const
{
	return this->Output;
}

SV_ASSIST::AUDIO::AudioStruct::AudioStruct() : id{ L"Unknown" }, state{ L"Unknown" }, name{ L"Unknown" }, desc{ L"Unknown" }, audioif{ L"Unknown" }
{
}

SV_ASSIST::AUDIO::AudioStruct::AudioStruct(std::wstring id, std::wstring state, std::wstring name, std::wstring desc, std::wstring audioif )
{
	this->id = id;
	this->state = state;
	this->name = name;
	this->desc = desc;
	this->audioif = audioif;
}

SV_ASSIST::AUDIO::AudioStruct::AudioStruct(const AudioStruct & that) : id{that.id}, state{that.state}, name{that.name}, desc{that.desc}, audioif{that.audioif}
{
}

AudioStruct & SV_ASSIST::AUDIO::AudioStruct::operator=(const AudioStruct & that)
{
	this->id = that.id;
	this->state = that.state;
	this->desc = that.desc;
	this->name = that.name;
	this->audioif = that.audioif;
	return *this;
}
