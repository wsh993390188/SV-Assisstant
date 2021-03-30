#include "stdafx.h"
#include <wrl.h>
#include <propvarutil.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include "AudioInfoFromWindowsAPI.h"

using namespace Hardware;

namespace
{
	/// @brief ����ָ���װ
	using prop_string_t = std::unique_ptr<WCHAR, decltype(&CoTaskMemFree)>;

#define prop_string_null prop_string_t(nullptr, CoTaskMemFree)
	/// @brief ��ȡ�豸ID
	/// @param[in] device COM�򿪵��豸
	/// @return WCHAR�ַ�����������ָ��������
	prop_string_t GetDeviceId(IN Microsoft::WRL::ComPtr<IMMDevice> const& device) {
		LPWSTR buffer = nullptr;
		if SUCCEEDED(device->GetId(&buffer)) {
			return prop_string_t(buffer, CoTaskMemFree);
		}
		return prop_string_null;
	}

	/// @brief ��ȡ�豸״̬
	/// @param[in] device COM�򿪵��豸
	/// @return �豸״̬�ַ���
	std::wstring GetDeviceState(IN Microsoft::WRL::ComPtr<IMMDevice> const& device) {
		DWORD state = 0;
		if SUCCEEDED(device->GetState(&state))
		{
			switch (state) {
			case DEVICE_STATE_ACTIVE:
				return L"ACTIVE";
			case DEVICE_STATE_DISABLED:
				return L"DISABLED";
			case DEVICE_STATE_NOTPRESENT:
				return L"NOTPRESENT";
			case DEVICE_STATE_UNPLUGGED:
				return L"UNPLUGGED";
			}
		}
		return L"UNKNOWN";
	}

	/// @brief �ַ�����װ
	/// @param[in] prop ���Լ���
	/// @param[in] key ��Ҫ��ѯ��������
	/// @return �������ַ�
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

	/// @brief ö���豸
	/// @param[in] enumerator �򿪵���Ƶ������
	/// @param[out] AudioInfos ��Ƶ�豸��Ϣ
	/// @param[in] DataType ö�ٵ�����
	void EnumRenderDevice(Microsoft::WRL::ComPtr<IMMDeviceEnumerator>& enumerator, std::map<uint32_t, Hardware::Audio::CommonInfo>& AudioInfos, const EDataFlow& DataType = EDataFlow::eRender)
	{
		// �����ȥǥХ������В�
		Microsoft::WRL::ComPtr<IMMDeviceCollection> devices;
		if SUCCEEDED(enumerator->EnumAudioEndpoints(DataType, // eRender, eCapture, eAll
			DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED,
			// DEVICE_STATE_ACTIVE: �Є�������Ƥ���
			// DEVICE_STATE_DISABLED: �o��������Ƥ���
			// DEVICE_STATE_NOTPRESENT: �����ƥ�˟o���h
			// DEVICE_STATE_UNPLUGGED: ����Ĥ˽ӾA����Ƥ��ʤ�
			devices.ReleaseAndGetAddressOf())) {
			// �ǥХ�������ȡ��.
			UINT uNumDevices;
			if SUCCEEDED(devices->GetCount(&uNumDevices)) {
				for (UINT i = 0; i < uNumDevices; ++i) {
					// �ǥХ�����ȡ��
					Microsoft::WRL::ComPtr<IMMDevice> device;
					if SUCCEEDED(devices->Item(i, device.ReleaseAndGetAddressOf())) {
						// �ǥХ���������ȡ��
						auto const id = GetDeviceId(device); // �ǥХ���ID
						auto const state = GetDeviceState(device);// �ǥХ�����״�B

						Microsoft::WRL::ComPtr<IMMEndpoint> endpoint;
						device.As<IMMEndpoint>(&endpoint);

						Microsoft::WRL::ComPtr<IPropertyStore> prop;
						if SUCCEEDED(device->OpenPropertyStore(STGM_READ, prop.ReleaseAndGetAddressOf()))
						{
							auto const name = GetPropString(prop, PKEY_Device_FriendlyName); // �ե�ͩ`��
							auto const desc = GetPropString(prop, PKEY_Device_DeviceDesc); // ����`�ȥͩ`��
							auto const audioif = GetPropString(prop, PKEY_DeviceInterface_FriendlyName); // ����ǥХ�����
							Hardware::Audio::CommonInfo temp
							{
								id.get() ? id.get() : L"",
								state,
								name.get() ? name.get() : L"",
								desc.get() ? desc.get() : L"",
								audioif.get() ? audioif.get() : L"" };
							AudioInfos.emplace(i, temp);
						}
						else
						{
							spdlog::error("devices->Item({}) OpenPropertyStore err", i);
						}
					}
					else
					{
						spdlog::error("devices->Item({}) err", i);
					}
				}
			}
			else
			{
				spdlog::error("devices->GetCount err");
			}
		}
		else
		{
			spdlog::error("EnumAudioEndpoints err");
		}
	}
}

Hardware::Audio::AudioInfoFromWindowsAPI::AudioInfoFromWindowsAPI()
{
}

Data::ErrorType Hardware::Audio::AudioInfoFromWindowsAPI::Initialize(std::string& response)
{
	AudioInfos.Audios.clear();
	AudioInfos.Mics.clear();
	Microsoft::WRL::ComPtr<IMMDeviceEnumerator> enumerator;
	if SUCCEEDED(::CoCreateInstance(__uuidof(MMDeviceEnumerator),
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(enumerator.ReleaseAndGetAddressOf())))
	{
		EnumRenderDevice(enumerator, this->AudioInfos.Audios);
		EnumRenderDevice(enumerator, this->AudioInfos.Mics, EDataFlow::eCapture);
	}
	else
	{
		spdlog::error("CoCreateInstance err");
	}
	if (AudioInfos.Audios.empty() && AudioInfos.Mics.empty())
		return Data::ErrorType::NOTSUPPORT;
	response = BuildInitializeJson();
	return Data::ErrorType::SUCCESS;
}

Data::ErrorType Hardware::Audio::AudioInfoFromWindowsAPI::Update(const std::string&, std::string&)
{
	return Data::ErrorType::NOTSUPPORT;
}