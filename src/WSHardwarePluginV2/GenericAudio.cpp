#include "stdafx.h"
#include "GenericAudio.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

using namespace Hardware;

Data::ErrorType Hardware::Audio::GenericAudio::GetElements(LPCSTR paramter, std::string& response)
{
	uint32_t AudioId;
	DeviceType DevType;
	if (ParserJson(paramter, AudioId, DevType))
	{
		if (DevType == DeviceType::Audio)
		{
			response = BuildElementJson(AudioInfos.Audios, AudioId);
			return Data::ErrorType::SUCCESS;
		}
		else if (DevType == DeviceType::Mic)
		{
			response = BuildElementJson(AudioInfos.Mics, AudioId);
			return Data::ErrorType::SUCCESS;
		}
	}
	return Data::ErrorType::UNKNOWNJSON;
}

std::string Hardware::Audio::GenericAudio::BuildInitializeJson()
{
	Json::Value root;
	for (const auto& AudioInfo : AudioInfos.Audios)
	{
		root["AudioIds"].append(AudioInfo.first);
	}

	for (const auto& AudioInfo : AudioInfos.Mics)
	{
		root["MicIds"].append(AudioInfo.first);
	}
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

bool Hardware::Audio::GenericAudio::ParserJson(const std::string& JsonString, uint32_t& AudioId, DeviceType& DevType)
{
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(JsonString, root))
		{
			if (root["AudioId"].isIntegral())
			{
				DevType = DeviceType::Audio;
				AudioId = root["AudioId"].asUInt();
				return true;
			}
			else if (root["MicId"].isIntegral())
			{
				DevType = DeviceType::Mic;
				AudioId = root["MicId"].asUInt();
				return true;
			}
			else
			{
				spdlog::error("Json AudioId is not numberic");
			}
		}
		else
		{
			spdlog::error("parser json failed");
		}
	}
	catch (const std::exception& e)
	{
		spdlog::error("parser json failed {}", e.what());
	}
	return false;
}

std::string Hardware::Audio::GenericAudio::BuildElementJson(const std::map<uint32_t, CommonInfo>& DeviceInfos, const uint32_t& AudioId)
{
	auto itr = DeviceInfos.find(AudioId);
	if (itr == DeviceInfos.end())
		return {};
	Json::Value root;
	{
		Json::Value temp;
		if (!itr->second.id.empty())
		{
			temp["id"] = Utils::wstringToUtf8(itr->second.id);
			root.append(std::move(temp));
		}
	}

	{
		Json::Value temp;
		if (!itr->second.state.empty())
		{
			temp["state"] = Utils::wstringToUtf8(itr->second.state);
			root.append(std::move(temp));
		}
	}

	{
		Json::Value temp;
		if (!itr->second.name.empty())
		{
			temp["name"] = Utils::wstringToUtf8(itr->second.name);
			root.append(std::move(temp));
		}
	}

	{
		Json::Value temp;
		if (!itr->second.desc.empty())
		{
			temp["desc"] = Utils::wstringToUtf8(itr->second.desc);
			root.append(std::move(temp));
		}
	}

	{
		Json::Value temp;
		if (!itr->second.audioif.empty())
		{
			temp["audioif"] = Utils::wstringToUtf8(itr->second.audioif);
			root.append(std::move(temp));
		}
	}

	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}