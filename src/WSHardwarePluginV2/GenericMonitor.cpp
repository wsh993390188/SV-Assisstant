#include "stdafx.h"
#include "GenericMonitor.h"

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
			WORD _Third : 5;
			WORD _Second : 5;
			WORD _First : 5;
			WORD Reserved : 1;
		}bits;
		WORD __Manufacturer;
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
			Json::Value temp;
			temp[Ele.first] = Ele.second;
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

bool Hardware::Monitor::GenericMonitor::ParserEDID(const EDID& EDIDbuffer, MonitorDataStruct& MonitorInfo)
{
	bool HasSerialNumber = false;
	{
		auto Str = GetMonitorName(EDIDbuffer);
		MonitorInfo.emplace_back("Manufacture", Str);
	}
	MonitorInfo.emplace_back("ProductCode", Utils::to_string_hex(EDIDbuffer.Productcode));
	MonitorInfo.emplace_back("Week/Year", std::to_string(EDIDbuffer.Week + 0) + "/" + std::to_string(EDIDbuffer.Year + 1990));
	MonitorInfo.emplace_back("EDIDVersion", std::to_string(EDIDbuffer.EDIDVersion[0] + 0) + "." + std::to_string(EDIDbuffer.EDIDVersion[1] + 0));
	MonitorInfo.emplace_back("Max.Vertical Size", std::to_string(EDIDbuffer.HorizontalScreenSize) + " cm");
	MonitorInfo.emplace_back("Max.Horizontal Size", std::to_string(EDIDbuffer.VerticalScreenSize) + " cm");
	MonitorInfo.emplace_back("Gamma", Utils::to_string_with_precision((EDIDbuffer.DisplayGamma + 100ull) / 100.0));
	for (auto i = 0; i < 4; ++i)
	{
		auto DescPtr = reinterpret_cast<const EDID_Other_Monitor_Descriptors*>(&EDIDbuffer.Descriptor[i]);
		if (DescPtr->Reserve == 0 && DescPtr->Reserved[0] == 0 && DescPtr->Reserved[1] == 0 && DescPtr->Reserved[2] == 0)
		{
			if (DescPtr->Descriptor_Type == 0xFF)
			{
				MonitorInfo.emplace_back("Serial Number", Utils::trim_copy(std::string(reinterpret_cast<const char*>(EDIDbuffer.Descriptor[i].Display_Serial_Number.Display_Serial_Number), 13)));
				HasSerialNumber = true;
			}
			else if (DescPtr->Descriptor_Type == 0xFC)
			{
				MonitorInfo.emplace_back("Model Type", Utils::trim_copy(std::string(reinterpret_cast<const char*>(EDIDbuffer.Descriptor[i].Display_Name.Display_Name), 13)));
			}
			else if (DescPtr->Descriptor_Type == 0xFD)
			{
				int minvoffset{}, minhoffset{}, maxvoffset{}, maxhoffset{};
				EDIDbuffer.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x1 ? minvoffset = 0xFF : minvoffset = 0;
				EDIDbuffer.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x2 ? minhoffset = 0xFF : minhoffset = 0;
				EDIDbuffer.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x4 ? maxvoffset = 0xFF : maxvoffset = 0;
				EDIDbuffer.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x8 ? maxhoffset = 0xFF : maxhoffset = 0;
				MonitorInfo.emplace_back("Horizontal Frequency", std::to_string(EDIDbuffer.Descriptor[i].Display_Range_Limits_Descriptor.Minimum_horizontal_field_rate + minhoffset) + "-" +
					std::to_string(EDIDbuffer.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_horizontal_field_rate + maxhoffset) + " Khz");
				MonitorInfo.emplace_back("Vertical Frequency", std::to_string(EDIDbuffer.Descriptor[i].Display_Range_Limits_Descriptor.Minimum_vertical_field_rate + minvoffset) + "-" +
					std::to_string(EDIDbuffer.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_vertical_field_rate + maxvoffset) + " hz");
				MonitorInfo.emplace_back("Max. Pixel Clock	", std::to_string(EDIDbuffer.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_pixel_clock_rate * 10) + " Mhz");
			}
		}
	}
	if (!HasSerialNumber)
		MonitorInfo.emplace_back("SerialNumber", Utils::to_string_hex(EDIDbuffer.SerialNumber));
	return true;
}

const std::string Hardware::Monitor::GenericMonitor::GetMonitorName(const Hardware::Monitor::EDID& EDIDbuffer)
{
	std::string edidname = {};
	EDIDManufacturer  buf = {};
	buf.__Manufacturer = EDIDbuffer.Manufacturer.Manufacturer;
	buf.__Manufacturer = ((buf.__Manufacturer & 0xFF) << 8) | ((buf.__Manufacturer & 0xFF00) >> 8);
	char temp[4] = {};
	temp[0] = buf.bits._First + 'A' - 1;
	temp[1] = buf.bits._Second + 'A' - 1;
	temp[2] = buf.bits._Third + 'A' - 1;

	if (strcmp(temp, "AAA") == 0)
		edidname = "Avolites Ltd";
	else if (strcmp(temp, "ACI") == 0)
		edidname = "Ancor Communications Inc";
	else if (strcmp(temp, "ACR") == 0)
		edidname = "Acer Technologies";
	else if (strcmp(temp, "ADA") == 0)
		edidname = "Addi-Data GmbH";
	else if (strcmp(temp, "APP") == 0)
		edidname = "Apple Computer Inc";
	else if (strcmp(temp, "ASK") == 0)
		edidname = "Ask A/S";
	else if (strcmp(temp, "AVT") == 0)
		edidname = "Avtek (Electronics) Pty Ltd";
	else if (strcmp(temp, "BNO") == 0)
		edidname = "Bang & Olufsen";
	else if (strcmp(temp, "CMN") == 0)
		edidname = "Chimei Innolux Corporation";
	else if (strcmp(temp, "CMO") == 0)
		edidname = "Chi Mei Optoelectronics corp.";
	else if (strcmp(temp, "CRO") == 0)
		edidname = "Extraordinary Technologies PTY Limited";
	else if (strcmp(temp, "DEL") == 0)
		edidname = "Dell Inc.";
	else if (strcmp(temp, "DGC") == 0)
		edidname = "Data General Corporation";
	else if (strcmp(temp, "DON") == 0)
		edidname = "DENON, Ltd.";
	else if (strcmp(temp, "ENC") == 0)
		edidname = "Eizo Nanao Corporation";
	else if (strcmp(temp, "EPH") == 0)
		edidname = "Epiphan Systems Inc. ";
	else if (strcmp(temp, "EXP") == 0)
		edidname = "Data Export Corporation";
	else if (strcmp(temp, "FNI") == 0)
		edidname = "Funai Electric Co., Ltd.";
	else if (strcmp(temp, "FUS") == 0)
		edidname = "Fujitsu Siemens Computers GmbH";
	else if (strcmp(temp, "GSM") == 0)
		edidname = "Goldstar Company Ltd";
	else if (strcmp(temp, "HIQ") == 0)
		edidname = "Kaohsiung Opto Electronics Americas, Inc.";
	else if (strcmp(temp, "HSD") == 0)
		edidname = "HannStar Display Corp";
	else if (strcmp(temp, "HTC") == 0)
		edidname = "Hitachi Ltd";
	else if (strcmp(temp, "HWP") == 0)
		edidname = "Hewlett Packard";
	else if (strcmp(temp, "INT") == 0)
		edidname = "Interphase Corporation";
	else if (strcmp(temp, "ITE") == 0)
		edidname = "Integrated Tech Express Inc";
	else if (strcmp(temp, "IVM") == 0)
		edidname = "Iiyama North America";
	else if (strcmp(temp, "LEN") == 0)
		edidname = "Lenovo Group Limited";
	else if (strcmp(temp, "MAX") == 0)
		edidname = "Rogen Tech Distribution Inc";
	else if (strcmp(temp, "MEG") == 0)
		edidname = "Abeam Tech Ltd";
	else if (strcmp(temp, "MEI") == 0)
		edidname = "Panasonic Industry Company";
	else if (strcmp(temp, "MTC") == 0)
		edidname = "Mars-Tech Corporation";
	else if (strcmp(temp, "MTX") == 0)
		edidname = "Matrox";
	else if (strcmp(temp, "NEC") == 0)
		edidname = "NEC Corporation";
	else if (strcmp(temp, "NEX") == 0)
		edidname = "Nexgen Mediatech Inc.";
	else if (strcmp(temp, "ONK") == 0)
		edidname = "ONKYO Corporation";
	else if (strcmp(temp, "ORN") == 0)
		edidname = "ORION ELECTRIC CO., LTD.";
	else if (strcmp(temp, "OTM") == 0)
		edidname = "Optoma Corporation";
	else if (strcmp(temp, "OVR") == 0)
		edidname = "Oculus VR, Inc.";
	else if (strcmp(temp, "PHL") == 0)
		edidname = "Philips Consumer Electronics Company";
	else if (strcmp(temp, "PIO") == 0)
		edidname = "Pioneer Electronic Corporation";
	else if (strcmp(temp, "PNR") == 0)
		edidname = "Planar Systems, Inc.";
	else if (strcmp(temp, "QDS") == 0)
		edidname = "Quanta Display Inc.";
	else if (strcmp(temp, "RAT") == 0)
		edidname = "Rent-A-Tech";
	else if (strcmp(temp, "REN") == 0)
		edidname = "Renesas Technology Corp.";
	else if (strcmp(temp, "SEC") == 0)
		edidname = "Seiko Epson Corporation";
	else if (strcmp(temp, "SHP") == 0)
		edidname = "Sharp Corporation";
	else if (strcmp(temp, "SII") == 0)
		edidname = "Silicon Image, Inc.";
	else if (strcmp(temp, "SNY") == 0)
		edidname = "Sony";
	else if (strcmp(temp, "STD") == 0)
		edidname = "STD Computer Inc.";
	else if (strcmp(temp, "SVS") == 0)
		edidname = "SVSI";
	else if (strcmp(temp, "SYN") == 0)
		edidname = "Synaptics Inc.";
	else if (strcmp(temp, "TCL") == 0)
		edidname = "Technical Concepts Ltd";
	else if (strcmp(temp, "TOP") == 0)
		edidname = "Orion Communications Co., Ltd.";
	else if (strcmp(temp, "TSB") == 0)
		edidname = "Toshiba America Info Systems Inc.";
	else if (strcmp(temp, "TST") == 0)
		edidname = "Transtream Inc.";
	else if (strcmp(temp, "VES") == 0)
		edidname = "Vestel Elektronik Sanayi ve Ticaret A. S.";
	else if (strcmp(temp, "VIZ") == 0)
		edidname = "VIZIO, Inc.";
	else if (strcmp(temp, "UNK") == 0)
		edidname = "Unknown";
	else if (strcmp(temp, "VSC") == 0)
		edidname = "ViewSonic Corporation";
	else if (strcmp(temp, "YMH") == 0)
		edidname = "Yamaha Corporation";
	else if (strcmp(temp, "SAM") == 0)
		edidname = "Samsung Electric Company";
	else
		edidname = "Unknown Manufacturer";
	return edidname;
}