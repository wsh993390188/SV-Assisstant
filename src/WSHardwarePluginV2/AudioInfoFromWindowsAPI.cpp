#include "stdafx.h"
#include <wrl.h>
#include <propvarutil.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include "AudioInfoFromWindowsAPI.h"

using namespace Hardware;

namespace
{
	/// @brief 智能指针封装
	using prop_string_t = std::unique_ptr<WCHAR, decltype(&CoTaskMemFree)>;

#define prop_string_null prop_string_t(nullptr, CoTaskMemFree)
	/// @brief 获取设备ID
	/// @param[in] device COM打开的设备
	/// @return WCHAR字符串，由智能指针做管理
	prop_string_t GetDeviceId(IN Microsoft::WRL::ComPtr<IMMDevice> const& device) {
		LPWSTR buffer = nullptr;
		if SUCCEEDED(device->GetId(&buffer)) {
			return prop_string_t(buffer, CoTaskMemFree);
		}
		return prop_string_null;
	}

	/// @brief 获取设备状态
	/// @param[in] device COM打开的设备
	/// @return 设备状态字符串
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

	/// @brief 字符串封装
	/// @param[in] prop 属性集合
	/// @param[in] key 需要查询的属性名
	/// @return 属性名字符
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

	/// @brief 枚举设备
	/// @param[in] enumerator 打开的音频管理者
	/// @param[out] AudioInfos 音频设备信息
	/// @param[in] DataType 枚举的类型
	void EnumRenderDevice(Microsoft::WRL::ComPtr<IMMDeviceEnumerator>& enumerator, std::map<uint32_t, Hardware::Audio::CommonInfo>& AudioInfos, const EDataFlow& DataType = EDataFlow::eRender)
	{
		// 出力先デバイスを列挙
		Microsoft::WRL::ComPtr<IMMDeviceCollection> devices;
		if SUCCEEDED(enumerator->EnumAudioEndpoints(DataType, // eRender, eCapture, eAll
			DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED,
			// DEVICE_STATE_ACTIVE: 有効化されている
			// DEVICE_STATE_DISABLED: 無効化されている
			// DEVICE_STATE_NOTPRESENT: システムに無登録
			// DEVICE_STATE_UNPLUGGED: 物理的に接続されていない
			devices.ReleaseAndGetAddressOf())) {
			// デバイス数を取得.
			UINT uNumDevices;
			if SUCCEEDED(devices->GetCount(&uNumDevices)) {
				for (UINT i = 0; i < uNumDevices; ++i) {
					// デバイスを取得
					Microsoft::WRL::ComPtr<IMMDevice> device;
					if SUCCEEDED(devices->Item(i, device.ReleaseAndGetAddressOf())) {
						// デバイスの情報を取得
						auto const id = GetDeviceId(device); // デバイスID
						auto const state = GetDeviceState(device);// デバイスの状態

						Microsoft::WRL::ComPtr<IMMEndpoint> endpoint;
						device.As<IMMEndpoint>(&endpoint);

						Microsoft::WRL::ComPtr<IPropertyStore> prop;
						if SUCCEEDED(device->OpenPropertyStore(STGM_READ, prop.ReleaseAndGetAddressOf()))
						{
							auto const name = GetPropString(prop, PKEY_Device_FriendlyName); // フルネーム
							auto const desc = GetPropString(prop, PKEY_Device_DeviceDesc); // ショートネーム
							auto const audioif = GetPropString(prop, PKEY_DeviceInterface_FriendlyName); // 物理デバイス名
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