#include "stdafx.h"
#include "GenericMonitor.h"
#include "PNPIDUtils.h"
#include <format>

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

namespace
{
	template<class T>
	std::enable_if_t<std::is_integral_v<T>> LittleToBig(T& buf)
	{
		if constexpr (sizeof(T) == 2)
		{
			buf = ((buf & 0xFF) << 8) | ((buf & 0xFF00) >> 8);
		}
		else if constexpr (sizeof(T) == 4)
		{
			buf = ((buf & 0xFF) << 24) | (((buf & 0xFF00) >> 8)) | (((buf & 0xFF00) >> 8)) | ((buf & 0xFF00) >> 24);
		}
	}

	union EDIDManufacturer
	{
		struct
		{
			std::uint16_t _Third : 5;
			std::uint16_t _Second : 5;
			std::uint16_t _First : 5;
			std::uint16_t Reserved : 1;
		}bits;
		std::uint16_t __Manufacturer;
	};
}

Hardware::Data::ErrorType Hardware::Monitor::GenericMonitor::GetElements(LPCSTR paramter, std::string& response)
{
	std::wstring MonitorId;
	if (ParserJson(paramter, MonitorId))
	{
		response = BuildElementJson(MonitorId);
		return Data::ErrorType::SUCCESS;
	}

	return Data::ErrorType::UNKNOWNJSON;
}

std::string Hardware::Monitor::GenericMonitor::BuildInitializeJson()
{
	Json::Value root;
	for (const auto& MonitorInfo : MonitorInfos)
	{
		root["MonitorIds"].append(Utils::wstringToUtf8(MonitorInfo.first));
	}
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

std::string Hardware::Monitor::GenericMonitor::BuildElementJson(const std::wstring& MonitorId)
{
	auto itr = MonitorInfos.find(MonitorId);
	if (itr == MonitorInfos.end())
		return {};
	Json::Value root;
	{
		for (const auto& Ele : itr->second)
		{
			if (Ele.second.empty())
				continue;
			Json::Value temp;
			if (Ele.second.size() == 1)
				temp[Ele.first] = Ele.second[0];
			else
			{
				for (const auto& info : Ele.second)
				{
					temp[Ele.first].append(info);
				}
			}
			root.append(std::move(temp));
		}
	}

	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

bool Hardware::Monitor::GenericMonitor::ParserJson(const std::string& JsonString, std::wstring& MonitorId)
{
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(JsonString, root))
		{
			if (root["MonitorId"].isString())
			{
				MonitorId = Utils::utf8ToWstring(root["MonitorId"].asString());
				return true;
			}
			else
			{
				spdlog::error("Json MonitorId is not numberic");
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

bool Hardware::Monitor::GenericMonitor::ParserEDID(const std::vector<uint8_t>& EDIDRawBuffer, MonitorDataStruct& MonitorInfo)
{
	if (EDIDRawBuffer.size() < sizeof(EDIDCommon))
	{
		spdlog::error("EDID Raw Buffer {} was too small", EDIDRawBuffer.size());
		return false;
	}
	auto EDIDbuffer = reinterpret_cast<const EDIDCommon*>(EDIDRawBuffer.data());
	if (EDIDbuffer->Header != 0x00ffffffffffff00)
	{
		spdlog::error("Header ({}) is not EDID Header", EDIDbuffer->Header);
		return false;
	}
	bool HasSerialNumber = false, HasImageSize = false;
	{
		auto Str = GetMonitorName(EDIDbuffer->Identification.ManufacturerID.Manufacturer);
		MonitorInfo.emplace_back("Manufacture", MonitorDataStruct::value_type::second_type{ Str });
	}
	MonitorInfo.emplace_back("ProductCode", MonitorDataStruct::value_type::second_type{ Utils::to_string_hex(EDIDbuffer->Identification.Productcode) });
	MonitorInfo.emplace_back("Week/Year", MonitorDataStruct::value_type::second_type{ std::format("Week {}/{}",EDIDbuffer->Identification.Week , EDIDbuffer->Identification.Year + 1990) });
	MonitorInfo.emplace_back("EDIDVersion", MonitorDataStruct::value_type::second_type{ std::format("{}.{}",EDIDbuffer->EDIDVersion.Version  ,EDIDbuffer->EDIDVersion.Revision) });

	if (EDIDbuffer->BasicDisplayParametersAndFeatures.DisplayGamma != 0xFF)
		MonitorInfo.emplace_back("Gamma", MonitorDataStruct::value_type::second_type{ Utils::to_string_with_precision((EDIDbuffer->BasicDisplayParametersAndFeatures.DisplayGamma + 100ull) / 100.0) });

	if (!this->AddStandardTiming(MonitorInfo, EDIDbuffer->StandardTimings))
	{
		MonitorInfo.pop_back();
		if (!this->AddEstablishedTiming(MonitorInfo, EDIDbuffer->EstablishedTimings))
			MonitorInfo.pop_back();
	}

	for (auto i = 0; i < 4; ++i)
	{
		auto DescPtr = reinterpret_cast<const EDID_Other_Monitor_Descriptors*>(&EDIDbuffer->DataBlocks.Descriptor[i]);
		if (DescPtr->Reserve == 0 && DescPtr->Reserved[0] == 0 && DescPtr->Reserved[1] == 0 && DescPtr->Reserved[2] == 0)
		{
			if (DescPtr->Descriptor_Type == 0xFF)
			{
				HasSerialNumber = this->AddDescriptorString(MonitorInfo, EDIDbuffer->DataBlocks.Descriptor[i].CommonDesc, "SerialNumber");
			}
			else if (DescPtr->Descriptor_Type == 0xFE)
			{
				this->AddDescriptorString(MonitorInfo, EDIDbuffer->DataBlocks.Descriptor[i].CommonDesc, "TagNumber");
			}
			else if (DescPtr->Descriptor_Type == 0xFD)
			{
				this->AddDisplayRangeLimits(MonitorInfo, EDIDbuffer->DataBlocks.Descriptor[i].Display_Range_Limits_Descriptor);
			}
			else if (DescPtr->Descriptor_Type == 0xFC)
			{
				this->AddDescriptorString(MonitorInfo, EDIDbuffer->DataBlocks.Descriptor[i].CommonDesc, "DisplayName");
			}
			else if (DescPtr->Descriptor_Type == 0xFB)
			{
				// Todo Color Point
			}
			else if (DescPtr->Descriptor_Type == 0xFA)
			{
				this->AddStandardTiming(MonitorInfo, EDIDbuffer->DataBlocks.Descriptor[i].StandardTimingIdentifierDefinition);
			}
		}
		else
		{
			HasImageSize = this->AddPreferredDetailedTiming(MonitorInfo, EDIDbuffer->DataBlocks.Descriptor[i].DetailDesc);
		}
	}

	if (!HasSerialNumber && EDIDbuffer->Identification.SerialNumber)
		MonitorInfo.emplace_back("SerialNumber", MonitorDataStruct::value_type::second_type{ Utils::to_string_hex(EDIDbuffer->Identification.SerialNumber) });

	if (!HasImageSize && EDIDbuffer->BasicDisplayParametersAndFeatures.HorizontalScreenSize && EDIDbuffer->BasicDisplayParametersAndFeatures.VerticalScreenSize)
	{
		MonitorInfo.emplace_back("Max.Horizontal Size", MonitorDataStruct::value_type::second_type{ std::format("{} cm",EDIDbuffer->BasicDisplayParametersAndFeatures.HorizontalScreenSize) });
		MonitorInfo.emplace_back("Max.Vertical Size", MonitorDataStruct::value_type::second_type{ std::format("{} cm",EDIDbuffer->BasicDisplayParametersAndFeatures.VerticalScreenSize) });
		MonitorInfo.emplace_back("Max Display Size", MonitorDataStruct::value_type::second_type{ std::format("{:.1f} Inches", std::sqrt(std::pow(EDIDbuffer->BasicDisplayParametersAndFeatures.VerticalScreenSize / 2.54,2) + std::pow(EDIDbuffer->BasicDisplayParametersAndFeatures.HorizontalScreenSize / 2.54,2))) });
	}
	// paser edid full information
	// todo extension information
	return true;
}

const std::string Hardware::Monitor::GenericMonitor::GetMonitorName(const std::uint16_t& Manufacturer)
{
	std::string PnpId(3, 0);
	EDIDManufacturer  buf = {};
	buf.__Manufacturer = Manufacturer;
	buf.__Manufacturer = ((buf.__Manufacturer & 0xFF) << 8) | ((buf.__Manufacturer & 0xFF00) >> 8);
	PnpId[0] = buf.bits._First + 'A' - 1;
	PnpId[1] = buf.bits._Second + 'A' - 1;
	PnpId[2] = buf.bits._Third + 'A' - 1;

	return Utils::PNPIDUtils::Instance().QueryNameByPnpId(PnpId);
}

bool Hardware::Monitor::GenericMonitor::AddDescriptorString(MonitorDataStruct& MonitorInfo, const EDID_Descriptor_Common_String& Descriptor, const std::string& Name)
{
	MonitorInfo.emplace_back(Name, MonitorDataStruct::value_type::second_type{ Utils::trim_copy(std::string(reinterpret_cast<const char*>(Descriptor.Display_Name), 13)) });
	return true;
}

bool Hardware::Monitor::GenericMonitor::AddDisplayRangeLimits(MonitorDataStruct& MonitorInfo, const EDID_Display_Range_Limits_Descriptor& DisplayRangeLimits)
{
	int minvoffset{}, minhoffset{}, maxvoffset{}, maxhoffset{};
	DisplayRangeLimits.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x1 ? minvoffset = 0xFF : minvoffset = 0;
	DisplayRangeLimits.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x2 ? minhoffset = 0xFF : minhoffset = 0;
	DisplayRangeLimits.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x4 ? maxvoffset = 0xFF : maxvoffset = 0;
	DisplayRangeLimits.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x8 ? maxhoffset = 0xFF : maxhoffset = 0;
	MonitorInfo.emplace_back("Horizontal Frequency", MonitorDataStruct::value_type::second_type{ std::to_string(DisplayRangeLimits.Minimum_horizontal_field_rate + minhoffset) + "-" +
		std::to_string(DisplayRangeLimits.Maximum_horizontal_field_rate + maxhoffset) + " Khz" });
	MonitorInfo.emplace_back("Vertical Frequency", MonitorDataStruct::value_type::second_type{ std::to_string(DisplayRangeLimits.Minimum_vertical_field_rate + minvoffset) + "-" +
		std::to_string(DisplayRangeLimits.Maximum_vertical_field_rate + maxvoffset) + " hz" });
	MonitorInfo.emplace_back("Max. Pixel Clock	", MonitorDataStruct::value_type::second_type{ std::to_string(DisplayRangeLimits.Maximum_pixel_clock_rate * 10) + " Mhz" });
	return true;
}

bool Hardware::Monitor::GenericMonitor::AddEstablishedTiming(MonitorDataStruct& MonitorInfo, const EDIDCommon::EstablishedTimingSection& EstablishedTiming)
{
	MonitorInfo.emplace_back(std::string("EstablishedTiming"), MonitorDataStruct::value_type::second_type{});
	auto& Timings = MonitorInfo.back();
	if (EstablishedTiming.EstablishedTiming1 & 0x80)
		Timings.second.emplace_back("720x400 @ 70 Hz");
	if (EstablishedTiming.EstablishedTiming1 & 0x40)
		Timings.second.emplace_back("720x400 @ 88 Hz ");
	if (EstablishedTiming.EstablishedTiming1 & 0x20)
		Timings.second.emplace_back("640x480 @ 60 Hz ");
	if (EstablishedTiming.EstablishedTiming1 & 0x10)
		Timings.second.emplace_back("640x480 @ 67 Hz ");
	if (EstablishedTiming.EstablishedTiming1 & 0x08)
		Timings.second.emplace_back("640x480 @ 72 Hz ");
	if (EstablishedTiming.EstablishedTiming1 & 0x04)
		Timings.second.emplace_back("640x480 @ 75 Hz ");
	if (EstablishedTiming.EstablishedTiming1 & 0x02)
		Timings.second.emplace_back("800x600 @ 56 Hz ");
	if (EstablishedTiming.EstablishedTiming1 & 0x01)
		Timings.second.emplace_back("800x600 @ 60 Hz ");

	if (EstablishedTiming.EstablishedTiming2 & 0x80)
		Timings.second.emplace_back("800x600 @ 72 Hz ");
	if (EstablishedTiming.EstablishedTiming2 & 0x40)
		Timings.second.emplace_back("800x600 @ 75 Hz ");
	if (EstablishedTiming.EstablishedTiming2 & 0x20)
		Timings.second.emplace_back("832x624 @ 75 Hz ");
	if (EstablishedTiming.EstablishedTiming2 & 0x10)
		Timings.second.emplace_back("1024x768 @ 87 Hz ");
	if (EstablishedTiming.EstablishedTiming2 & 0x08)
		Timings.second.emplace_back("1024x768 @ 60 Hz ");
	if (EstablishedTiming.EstablishedTiming2 & 0x04)
		Timings.second.emplace_back("1024x768 @ 72 Hz ");
	if (EstablishedTiming.EstablishedTiming2 & 0x02)
		Timings.second.emplace_back("1024x768 @ 75 Hz ");
	if (EstablishedTiming.EstablishedTiming2 & 0x01)
		Timings.second.emplace_back("1280x1024 @ 75 Hz ");

	if (EstablishedTiming.ManufacturerTiming & 0x80)
		Timings.second.emplace_back("1152x870 @ 75Hz ");
	return !Timings.second.empty();
}

bool Hardware::Monitor::GenericMonitor::AddStandardTiming(MonitorDataStruct& MonitorInfo, const EDIDCommon::StandardTimingSection& StandardTiming)
{
	MonitorInfo.emplace_back(std::string("StandardTiming"), MonitorDataStruct::value_type::second_type{});
	auto& timings = MonitorInfo.back();
	for (const auto& timing : StandardTiming.StandardTiming)
	{
		if (((timing & 0xFF) == 0) || timing == 0x0101)
			continue;
		AddStandardTimingImpl(timings.second, timing);
	}

	return !timings.second.empty();
}

bool Hardware::Monitor::GenericMonitor::AddStandardTiming(MonitorDataStruct& MonitorInfo, const EDID_StandardTimingIdentifierDefinition& StandardTiming)
{
	auto timings = std::find_if(MonitorInfo.begin(), MonitorInfo.end(), [](const MonitorDataStruct::value_type& ele)
		{
			return ele.first == "StandardTiming";
		});
	if (timings == MonitorInfo.end())
	{
		MonitorInfo.emplace_back(std::string("StandardTiming"), MonitorDataStruct::value_type::second_type{});
		timings = --MonitorInfo.end();
	}

	for (const auto& timing : StandardTiming.Timing)
	{
		if (((timing & 0xFF) == 0) || timing == 0x0101)
			continue;
		AddStandardTimingImpl(timings->second, timing);
	}
	return false;
}

void Hardware::Monitor::GenericMonitor::AddStandardTimingImpl(std::vector<std::string>& Timing, const std::uint16_t value)
{
	auto horizontal = (Utils::extract_bits_ui(value, 0, 7) + 31) * 8;
	auto RefreshRate = Utils::extract_bits_ui(value, 8, 13) + 60;
	switch (Utils::extract_bits_ui(value, 14, 15))
	{
	case 0b00:
		Timing.emplace_back(std::format("{} x {} (16:10) @ {} Hz", horizontal, (horizontal / 16 * 10), RefreshRate));
		break;
	case 0b01:
		Timing.emplace_back(std::format("{} x {} (4:3) @ {} Hz", horizontal, (horizontal / 4 * 3), RefreshRate));
		break;
	case 0b10:
		Timing.emplace_back(std::format("{} x {} (5:4) @ {} Hz", horizontal, (horizontal / 5 * 4), RefreshRate));
		break;
	case 0b11:
		Timing.emplace_back(std::format("{} x {} (16:9) @ {} Hz", horizontal, (horizontal / 16 * 9), RefreshRate));
		break;
	}
}

bool Hardware::Monitor::GenericMonitor::AddPreferredDetailedTiming(MonitorDataStruct& MonitorInfo, const EDID_Detailed_Timing_Descriptor& DetailedTiming)
{
	MonitorInfo.emplace_back(std::string("PreferredDetailedTiming"), MonitorDataStruct::value_type::second_type{ std::format("{} x {}, Pixel Clock: {:.2f} Mhz",DetailedTiming.Horizental_pixels_upperbits.bits.Horizental_active_pixels << 8 | DetailedTiming.Horizental_active_pixels, DetailedTiming.Vertical_lines_msbits.bits.Vertical_active_pixels << 8 | DetailedTiming.Vertical_active_pixels ,DetailedTiming.Pixel_Clock / 100.0) });

	auto Vertical = DetailedTiming.Image_Size_msbits.bits.Vertical_image_size << 8 | DetailedTiming.Vertical_image_size;
	auto Horizontal = DetailedTiming.Image_Size_msbits.bits.Horizontal_image_size << 8 | DetailedTiming.Horizontal_image_size;

	if (Vertical && Horizontal)
	{
		MonitorInfo.emplace_back("Max.Vertical Size", MonitorDataStruct::value_type::second_type{ std::format("{} mm",Vertical) });
		MonitorInfo.emplace_back("Max.Horizontal Size", MonitorDataStruct::value_type::second_type{ std::format("{} mm",Horizontal) });
		MonitorInfo.emplace_back("Max Display Size", MonitorDataStruct::value_type::second_type{ std::format("{:.1f} Inches", std::sqrt(std::pow(Vertical / 25.4,2) + std::pow(Horizontal / 25.4,2))) });
		return true;
	}
	return false;
}