#include "stdafx.h"
#include <fstream>
#include <shlwapi.h>
#include "CPUConfig.h"

Hardware::CPU::CPUDB& Hardware::CPU::CPUDB::Instance()
{
	static CPUDB inst;
	return inst;
}

void Hardware::CPU::CPUDB::Initialize()
{
	std::string XmlContent;
	if (!GetConfigXml(L"CPUDatabase.dat", L"CPUDatabase.xml", XmlContent))
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

	CPUDBType TempDB{};
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
		std::swap(TempDB, this->m_CPUDB);
	}
	spdlog::info("End XML Config Init");
	return;
}

std::unique_ptr<Hardware::CPU::CPUExtendedInfoFromCPUDB> Hardware::CPU::CPUDB::FindElements(const CPUQueryInfo& QueryInfo) const
{
	auto Manufacture = m_CPUDB.find(QueryInfo.CpuManufacture);
	if (Manufacture != m_CPUDB.end())
	{
		auto Family = Manufacture->second.Element.find(QueryInfo.CpuFamily);
		if (Family != Manufacture->second.Element.end())
		{
			auto Model = Family->second.Element.find(QueryInfo.CpuModel);
			if (Model != Family->second.Element.end())
			{
				return std::make_unique<Hardware::CPU::CPUExtendedInfoFromCPUDB>(Model->second);
			}
		}
	}
	return nullptr;
}

const Hardware::CPU::CPUDB::CPUDBType& Hardware::CPU::CPUDB::GetCpuData() const
{
	std::lock_guard<decltype(m_Mutex)> locker(m_Mutex);
	return m_CPUDB;
}

bool Hardware::CPU::CPUDB::GetConfigXml(const std::wstring& fileName, const std::wstring& localConfigName, std::string& configcontent)
{
	std::ifstream file(localConfigName, std::ios::binary | std::ios::in);
	if (file)
	{
		std::stringstream buffer;
		buffer << file.rdbuf();
		configcontent = buffer.str();
		file.close();
		return true;
	}
	return false;
}

void Hardware::CPU::CPUDB::ParserManufacture(tinyxml2::XMLElement const* const ManufactureElement, CPUDBType& CpuDB)
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

	auto FamilyElement = ManufactureElement->FirstChildElement();
	CpuFamily TempCpuFamily{};
	while (FamilyElement)
	{
		ParserCpuFamily(FamilyElement, TempCpuFamily);
		FamilyElement = FamilyElement->NextSiblingElement();
	}
	if (TempCpuFamily)
		CpuDB.emplace(ManufactureName, std::move(TempCpuFamily));
}

Hardware::Data::ErrorType Hardware::CPU::CPUDB::ParserCpuFamily(tinyxml2::XMLElement const* const FamilyElement, CpuFamily& FamilyData)
{
	std::string FamilyName;
	if (FamilyElement->Attribute("name"))
	{
		FamilyName = FamilyElement->Attribute("name");
	}
	else
	{
		spdlog::error("Family dosent have Attribute(name)");
		return Data::ErrorType::XMLERROR;
	}
	CpuInformationFromCpudb TempCpuInfo{};
	auto CpuElement = FamilyElement->FirstChildElement();
	while (CpuElement)
	{
		ParserCpuInformaion(CpuElement, TempCpuInfo);
		CpuElement = CpuElement->NextSiblingElement();
	}
	if (TempCpuInfo)
		FamilyData.Element.emplace(FamilyName, std::move(TempCpuInfo));
	return Data::ErrorType::SUCCESS;
}

Hardware::Data::ErrorType Hardware::CPU::CPUDB::ParserCpuInformaion(tinyxml2::XMLElement const* const CpuElement, CpuInformationFromCpudb& CpuData)
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

	CPUExtendedInfoFromCPUDB CpuInfo{};

	auto SubNode = CpuElement->FirstChildElement();
	while (SubNode)
	{
		if (std::strstr(SubNode->Value(), "CodeName"))
		{
			if (SubNode->Attribute("priority"))
			{
				CpuInfo.CodeName.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			CpuInfo.CodeName.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "SocketPackage"))
		{
			if (SubNode->Attribute("priority"))
			{
				CpuInfo.SocketPackage.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			CpuInfo.SocketPackage.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "MaxTDP"))
		{
			if (SubNode->Attribute("priority"))
			{
				CpuInfo.MaxTDP.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			CpuInfo.MaxTDP.Element = SubNode->GetText();
		}
		else if (std::strstr(SubNode->Value(), "Technology"))
		{
			if (SubNode->Attribute("priority"))
			{
				CpuInfo.Technology.Priority = (std::strstr(SubNode->Attribute("priority"), "true"));
			}
			CpuInfo.Technology.Element = SubNode->GetText();
		}
		SubNode = SubNode->NextSiblingElement();
	}

	if (CpuInfo)
		CpuData.Element.emplace(std::move(CpuName), std::move(CpuInfo));
	else
		return Data::ErrorType::DATAEMPTY;
	return Data::ErrorType::SUCCESS;
}

Hardware::CPU::CPUDB::CpuFamily::operator bool() const
{
	return !this->Element.empty();
}

Hardware::CPU::CPUDB::CpuInformationFromCpudb::operator bool() const
{
	return !Element.empty();
}

Hardware::CPU::CPUDB::CPUQueryInfo::operator bool() const
{
	return !(CpuManufacture.empty() || CpuModel.empty() || CpuFamily.empty());
}

Hardware::CPU::CPUExtendedInfoFromCPUDB::operator bool() const
{
	return !(Technology.Element.empty() && CodeName.Element.empty() && SocketPackage.Element.empty() && this->MaxTDP.Element.empty());
}