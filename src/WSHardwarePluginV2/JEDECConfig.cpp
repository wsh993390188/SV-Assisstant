#include "stdafx.h"
#include "JEDECConfig.h"
#include "resource.h"

Hardware::Memory::JEDECConfig& Hardware::Memory::JEDECConfig::Instance()
{
	static JEDECConfig inst;
	return inst;
}

std::string Hardware::Memory::JEDECConfig::GetManufacture(const size_t BankCode, const size_t ContinuationCode) const
{
	try
	{
		return this->m_Banks.at(BankCode).at(ContinuationCode);
	}
	catch (const std::exception&)
	{
		return {};
	}
}

Hardware::Memory::JEDECConfig::JEDECConfig()
{
	Initialize();
}

void Hardware::Memory::JEDECConfig::ParserBank(tinyxml2::XMLElement const* const BankElement, BanksType& BanksDB)
{
	std::string BankCode;
	if (BankElement->Attribute("id"))
	{
		BankCode = BankElement->Attribute("id");
	}
	else
	{
		spdlog::error("Bank dosent have Attribute(id)");
		return;
	}

	auto ManufactureElement = BankElement->FirstChildElement();
	BanksDB.emplace_back();
	while (ManufactureElement)
	{
		ParserManufacture(ManufactureElement, BanksDB.back());
		ManufactureElement = ManufactureElement->NextSiblingElement();
	}
}

void Hardware::Memory::JEDECConfig::ParserManufacture(tinyxml2::XMLElement const* const ManufactureElement, BankType& BankDB)
{
	std::string ManufactureCode;
	if (ManufactureElement->Attribute("id"))
	{
		ManufactureCode = ManufactureElement->Attribute("id");
	}
	else
	{
		spdlog::error("Manufacture dosent have Attribute(id)");
		return;
	}

	if (ManufactureElement->GetText())
	{
		BankDB.emplace(atol(ManufactureCode.c_str()), ManufactureElement->GetText());
	}
	else
	{
		return;
	}
}

void Hardware::Memory::JEDECConfig::Initialize()
{
	std::string XmlContent;
	if (!GetConfigXmlFromResource(XmlContent, IDR_XML3))
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

	BanksType TempDB;
	auto RootChildNode = RootElement->FirstChildElement();
	while (RootChildNode)
	{
		if (std::strstr(RootChildNode->Value(), "Bank"))
		{
			ParserBank(RootChildNode, TempDB);
		}
		RootChildNode = RootChildNode->NextSiblingElement();
	}

	{
		std::lock_guard<decltype(m_Mutex)> locker(m_Mutex);
		std::swap(TempDB, this->m_Banks);
	}
	spdlog::info("End XML Config Init");
	return;
}