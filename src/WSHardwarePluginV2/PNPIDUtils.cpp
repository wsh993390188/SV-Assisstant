#include "stdafx.h"
#include "PNPIDUtils.h"
#include "resource.h"

Hardware::Utils::PNPIDUtils::PNPIDUtils()
{
	this->Initialize();
}

Hardware::Utils::PNPIDUtils& Hardware::Utils::PNPIDUtils::Instance()
{
	static PNPIDUtils inst;
	return inst;
}

std::string Hardware::Utils::PNPIDUtils::QueryNameByPnpId(const std::string& PnpId)
{
	std::unique_lock<decltype(m_Mutex)> locker(m_Mutex);
	if (auto item = this->m_PNPIDs.find(PnpId); item != this->m_PNPIDs.end())
	{
		return item->second.Element;
	}
	return {};
}

void Hardware::Utils::PNPIDUtils::Initialize()
{
	std::string XmlContent;
	if (!GetConfigXmlFromResource(XmlContent, IDR_XML4))
	{
		spdlog::error("Read Xml {} Config failure", IDR_XML4);
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

	PNPIDDBType TempDB{};
	auto RootChildNode = RootElement->FirstChildElement();
	while (RootChildNode)
	{
		if (std::strstr(RootChildNode->Value(), "PNPID"))
		{
			ParserPNPID(RootChildNode, TempDB);
		}
		RootChildNode = RootChildNode->NextSiblingElement();
	}

	{
		std::unique_lock<decltype(m_Mutex)> locker(m_Mutex);
		std::swap(TempDB, this->m_PNPIDs);
	}
	spdlog::info("End XML Config Init");
	return;
}

void Hardware::Utils::PNPIDUtils::ParserPNPID(tinyxml2::XMLElement const* const PNPIDElement, PNPIDDBType& PnpidDB)
{
	XMLConfig::ExtendedInfo ManufactureName;
	if (PNPIDElement->Attribute("Manufacture"))
	{
		ManufactureName.Element = PNPIDElement->Attribute("Manufacture");
	}
	else
	{
		spdlog::error("PNPID dosent have Attribute(Manufacture)");
		return;
	}

	auto text = PNPIDElement->GetText();
	if (text)
	{
		PnpidDB.emplace(text, std::move(ManufactureName));
	}
	else
	{
		spdlog::error("PNPID dosent have Value");
	}
}