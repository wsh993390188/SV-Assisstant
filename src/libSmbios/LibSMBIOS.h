#pragma once
#include <vector>
#include "SmbiosDefination.h"
#include "TypeAll.h"

namespace Smbios
{
	template<typename TypeName>
	decltype(auto) GetType(TypeName type)
	{
		return Smbios::GetType(type);
	}

	std::vector<std::shared_ptr<SmbiosHeaderType::TypeBase>> GetAllTypes();
	const std::string& GetBiosVendor();

	Smbios::Type0 GetType(SmbiosHeaderType::Type0);
	Smbios::Type1 GetType(SmbiosHeaderType::Type1);
	Smbios::Type2 GetType(SmbiosHeaderType::Type2);
	Smbios::Type3 GetType(SmbiosHeaderType::Type3);
	std::vector<Smbios::Type4> GetType(SmbiosHeaderType::Type4);
	std::vector<Smbios::Type5> GetType(SmbiosHeaderType::Type5);
	std::vector<Smbios::Type6> GetType(SmbiosHeaderType::Type6);
	std::vector<Smbios::Type7> GetType(SmbiosHeaderType::Type7);
	std::vector<Smbios::Type8> GetType(SmbiosHeaderType::Type8);
	std::vector<Smbios::Type9> GetType(SmbiosHeaderType::Type9);
	std::vector<Smbios::Type10> GetType(SmbiosHeaderType::Type10);
	std::vector<Smbios::Type11> GetType(SmbiosHeaderType::Type11);
	std::vector<Smbios::Type12> GetType(SmbiosHeaderType::Type12);
	std::vector<Smbios::Type13> GetType(SmbiosHeaderType::Type13);
	std::vector<Smbios::Type14> GetType(SmbiosHeaderType::Type14);
	std::vector<Smbios::Type15> GetType(SmbiosHeaderType::Type15);
	std::vector<Smbios::Type16> GetType(SmbiosHeaderType::Type16);
	std::vector<Smbios::Type17> GetType(SmbiosHeaderType::Type17);
	std::vector<Smbios::Type18> GetType(SmbiosHeaderType::Type18);
	std::vector<Smbios::Type19> GetType(SmbiosHeaderType::Type19);
	std::vector<Smbios::Type20> GetType(SmbiosHeaderType::Type20);
	std::vector<Smbios::Type21> GetType(SmbiosHeaderType::Type21);
	std::vector<Smbios::Type22> GetType(SmbiosHeaderType::Type22);
	std::vector<Smbios::Type23> GetType(SmbiosHeaderType::Type23);
	std::vector<Smbios::Type24> GetType(SmbiosHeaderType::Type24);
	std::vector<Smbios::Type25> GetType(SmbiosHeaderType::Type25);
	std::vector<Smbios::Type26> GetType(SmbiosHeaderType::Type26);
	std::vector<Smbios::Type27> GetType(SmbiosHeaderType::Type27);
	std::vector<Smbios::Type28> GetType(SmbiosHeaderType::Type28);
	std::vector<Smbios::Type29> GetType(SmbiosHeaderType::Type29);
	std::vector<Smbios::Type30> GetType(SmbiosHeaderType::Type30);
	std::vector<Smbios::Type32> GetType(SmbiosHeaderType::Type32);
	std::vector<Smbios::Type33> GetType(SmbiosHeaderType::Type33);
	std::vector<Smbios::Type34> GetType(SmbiosHeaderType::Type34);
	std::vector<Smbios::Type35> GetType(SmbiosHeaderType::Type35);
	std::vector<Smbios::Type36> GetType(SmbiosHeaderType::Type36);
	std::vector<Smbios::Type37> GetType(SmbiosHeaderType::Type37);
	std::vector<Smbios::Type38> GetType(SmbiosHeaderType::Type38);
	std::vector<Smbios::Type39> GetType(SmbiosHeaderType::Type39);
	std::vector<Smbios::Type40> GetType(SmbiosHeaderType::Type40);
	std::vector<Smbios::Type41> GetType(SmbiosHeaderType::Type41);
	std::vector<Smbios::Type42> GetType(SmbiosHeaderType::Type42);
	std::vector<Smbios::Type43> GetType(SmbiosHeaderType::Type43);
	std::vector<Smbios::Type44> GetType(SmbiosHeaderType::Type44);
	std::vector<Smbios::Lenovo::Type131> GetType(SmbiosHeaderType::Type131);
	std::vector<Smbios::Lenovo::Type133> GetType(SmbiosHeaderType::Type133);
	std::vector<Smbios::Lenovo::Type134> GetType(SmbiosHeaderType::Type134);
	std::vector<Smbios::Lenovo::Type140> GetType(SmbiosHeaderType::Type140);
	std::vector<Smbios::Acer::Type170> GetType(SmbiosHeaderType::Type170);
	std::vector<Smbios::Lenovo::Type200> GetType(SmbiosHeaderType::Type200);
}
