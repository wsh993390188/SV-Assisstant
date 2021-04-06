#include "stdafx.h"
#include "GPUConfig.h"
#include "resource.h"

Hardware::GPU::GPUExtendedInfoFromGPUDB::operator bool() const
{
	return !(ChipName.Element.empty() && GPUClock.Element.empty());
}

Hardware::GPU::GPUConfig::InformationFromDB::operator bool() const
{
	return !this->Element.empty();
}

Hardware::GPU::GPUConfig& Hardware::GPU::GPUConfig::Instance()
{
	static GPUConfig inst;
	return inst;
}

void Hardware::GPU::GPUConfig::Initialize()
{
	std::string XmlContent;
	if (!GetConfigXmlFromResource(XmlContent, IDR_XML2))
	{
		spdlog::error("Read Xml Config failure");
		return;
	}
	tinyxml2::XMLDocument Document;
	Document.Parse(XmlContent.c_str(), XmlContent.size());

	if (Document.Error())
	{
		spdlog::error("XML Parse error! Error Status: {}", Document.ErrorStr());
		return;
	}

	auto RootElement = Document.RootElement();
	if (!RootElement)
	{
		spdlog::error("XML No Root Element error!");
		return;
	}

	if (RootElement->Attribute("version"))
	{
		m_DefaultVersion = RootElement->Attribute("version");
	}

	GPUDBType TempDB{};
	auto RootChildNode = RootElement->FirstChildElement();
	while (RootChildNode)
	{
		if (std::strstr(RootChildNode->Value(), "Manufacture"))
		{
			ParserManufacture(RootChildNode, TempDB);
		}
		RootChildNode = RootChildNode->NextSiblingElement();
	}

	{
		std::lock_guard<decltype(m_Mutex)> locker(m_Mutex);
		std::swap(TempDB, this->m_GPUDB);
	}
	spdlog::info("End XML Config Init");
	return;
}

std::unique_ptr<Hardware::GPU::GPUExtendedInfoFromGPUDB> Hardware::GPU::GPUConfig::FindElements(const QueryInfo& QueryInfo) const
{
	auto Manufacture = m_GPUDB.find(QueryInfo.Manufacture);
	if (Manufacture != m_GPUDB.end())
	{
		auto Model = Manufacture->second.Element.find(QueryInfo.Model);
		if (Model != Manufacture->second.Element.end())
		{
			return std::make_unique<Hardware::GPU::GPUExtendedInfoFromGPUDB>(Model->second);
		}
	}
	return nullptr;
}

void Hardware::GPU::GPUConfig::BuildExtendedInfoToJson(Json::Value& root, const QueryInfo& QueryInfo)
{
	if (auto GPUDBInfo = FindElements(QueryInfo); GPUDBInfo)
	{
		if(GPUDBInfo->ChipName)
		{
			Json::Value temp;
			temp["Chip Name"] = GPUDBInfo->ChipName.Element;
			root.append(temp);
		}

		if (GPUDBInfo->Shaders)
		{
			Json::Value temp;
			temp["Shaders"] = GPUDBInfo->Shaders.Element;
			root.append(temp);
		}

		if (GPUDBInfo->TMUs)
		{
			Json::Value temp;
			temp["TMUs"] = GPUDBInfo->TMUs.Element;
			root.append(temp);
		}

		if (GPUDBInfo->ROPs)
		{
			Json::Value temp;
			temp["ROPs"] = GPUDBInfo->ROPs.Element;
			root.append(temp);
		}
	}
}

void Hardware::GPU::GPUConfig::ParserManufacture(tinyxml2::XMLElement const* const ManufactureElement, GPUDBType& DB)
{
	std::string ManufactureName;
	if (ManufactureElement->Attribute("name"))
	{
		ManufactureName = ManufactureElement->Attribute("name");
	}
	else
	{
		spdlog::error("Manufacture dosent have Attribute(name)");
		return;
	}

	auto GpuElement = ManufactureElement->FirstChildElement();
	InformationFromDB TempCpuInfo{};
	while (GpuElement)
	{
		ParserInformaion(GpuElement, TempCpuInfo);
		GpuElement = GpuElement->NextSiblingElement();
	}
	if (TempCpuInfo)
		DB.emplace(ManufactureName, std::move(TempCpuInfo));
}

Hardware::Data::ErrorType Hardware::GPU::GPUConfig::ParserInformaion(tinyxml2::XMLElement const* const CpuElement, InformationFromDB& Data)
{
	std::string CpuName;
	if (CpuElement->Attribute("name"))
	{
		CpuName = CpuElement->Attribute("name");
	}
	else
	{
		spdlog::error("Cpu dosent have Attribute(name)");
		return Data::ErrorType::XMLERROR;
	}

	GPUExtendedInfoFromGPUDB GpuInfo{};

	auto SubNode = CpuElement->FirstChildElement();
	while (SubNode)
	{
		if (std::strstr(SubNode->Value(), "ChipName"))
		{
			if (SubNode->Attribute("priority"))
			{
				GpuInfo.ChipName.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			GpuInfo.ChipName.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "Released"))
		{
			if (SubNode->Attribute("priority"))
			{
				GpuInfo.Released.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			GpuInfo.Released.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "MemorySize"))
		{
			if (SubNode->Attribute("priority"))
			{
				GpuInfo.MemorySize.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			GpuInfo.MemorySize.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "MemoryType"))
		{
			if (SubNode->Attribute("priority"))
			{
				GpuInfo.MemoryType.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			GpuInfo.MemoryType.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "MemoryBits"))
		{
			if (SubNode->Attribute("priority"))
			{
				GpuInfo.MemoryBits.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			GpuInfo.MemoryBits.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "MemoryClock"))
		{
			if (SubNode->Attribute("priority"))
			{
				GpuInfo.MemoryClock.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			GpuInfo.MemoryClock.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "Clock"))
		{
			if (SubNode->Attribute("priority"))
			{
				GpuInfo.GPUClock.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			GpuInfo.GPUClock.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "Shaders"))
		{
			if (SubNode->Attribute("priority"))
			{
				GpuInfo.Shaders.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			GpuInfo.Shaders.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "TMUs"))
		{
			if (SubNode->Attribute("priority"))
			{
				GpuInfo.TMUs.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			GpuInfo.TMUs.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "ROPs"))
		{
			if (SubNode->Attribute("priority"))
			{
				GpuInfo.ROPs.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			GpuInfo.ROPs.Element = SubNode->GetText();
		}
		SubNode = SubNode->NextSiblingElement();
	}

	if (GpuInfo)
		Data.Element.emplace(std::move(CpuName), std::move(GpuInfo));
	else
		return Data::ErrorType::DATAEMPTY;
	return Data::ErrorType::SUCCESS;
}