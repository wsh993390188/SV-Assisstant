#include "stdafx.h"
#include "LibSMBIOS.h"
#include "RawSmbios.h"

std::vector<std::shared_ptr<Smbios::SmbiosHeaderType::TypeBase>> Smbios::GetAllTypes()
{
	std::vector<std::shared_ptr<SmbiosHeaderType::TypeBase>> temp;
	auto TypePhyAddress = reinterpret_cast<RawSMBIOSData*>(RawSmbios::Instance()->GetPhyAdderss())->SMBIOSTableData;
	if (!TypePhyAddress)
		return temp;

	auto test = reinterpret_cast<DMI_Header*>(TypePhyAddress);
	try
	{
		SmbiosHeaderType::TypeFactory::Instance().SetVendor(GetType(SmbiosHeaderType::Type1()).Manufacturer());
	}
	catch (const std::exception& e)
	{
		OutputDebugStringA(e.what());
	}

	while (true)
	{
		if (test->type == SmbiosEndofTable)
		{
			temp.emplace_back(Smbios::SmbiosHeaderType::TypeFactory::Instance().CreateFactory(*test));
			break;
		}

		temp.emplace_back(Smbios::SmbiosHeaderType::TypeFactory::Instance().CreateFactory(*test));

		TypePhyAddress += test->length;//Ìø¹ý½á¹¹¶Î
		while ((*reinterpret_cast<uint16_t*>(TypePhyAddress)) != 0) // Ìø¹ý×Ö·û¶Î
		{
			++TypePhyAddress;
		}
		TypePhyAddress += 2; //Ìø¹ýSMBIOS¶Î½áÊø·û 0000
		test = reinterpret_cast<DMI_Header*>(TypePhyAddress);
	}

	return temp;
}

const std::string& Smbios::GetBiosVendor()
{
	return SmbiosHeaderType::TypeFactory::Instance().GetVendor();
}

Smbios::Type0 Smbios::GetType(SmbiosHeaderType::Type0)
{
	return std::move(Smbios::Type0());
}

Smbios::Type1 Smbios::GetType(SmbiosHeaderType::Type1)
{
	return Smbios::Type1();
}

Smbios::Type2 Smbios::GetType(SmbiosHeaderType::Type2)
{
	return Smbios::Type2();
}

Smbios::Type3 Smbios::GetType(SmbiosHeaderType::Type3)
{
	return Smbios::Type3();
}

std::vector<Smbios::Type4> Smbios::GetType(SmbiosHeaderType::Type4)
{
	std::vector<Smbios::Type4> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type4 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type5> Smbios::GetType(SmbiosHeaderType::Type5)
{
	std::vector<Smbios::Type5> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type5 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type6> Smbios::GetType(SmbiosHeaderType::Type6)
{
	std::vector<Smbios::Type6> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type6 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type7> Smbios::GetType(SmbiosHeaderType::Type7)
{
	std::vector<Smbios::Type7> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type7 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type8> Smbios::GetType(SmbiosHeaderType::Type8)
{
	std::vector<Smbios::Type8> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type8 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type9> Smbios::GetType(SmbiosHeaderType::Type9)
{
	std::vector<Smbios::Type9> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type9 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type10> Smbios::GetType(SmbiosHeaderType::Type10)
{
	std::vector<Smbios::Type10> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type10 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type11> Smbios::GetType(SmbiosHeaderType::Type11)
{
	std::vector<Smbios::Type11> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type11 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type12> Smbios::GetType(SmbiosHeaderType::Type12)
{
	std::vector<Smbios::Type12> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type12 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type13> Smbios::GetType(SmbiosHeaderType::Type13)
{
	std::vector<Smbios::Type13> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type13 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type14> Smbios::GetType(SmbiosHeaderType::Type14)
{
	std::vector<Smbios::Type14> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type14 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type15> Smbios::GetType(SmbiosHeaderType::Type15)
{
	std::vector<Smbios::Type15> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type15 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type16> Smbios::GetType(SmbiosHeaderType::Type16)
{
	std::vector<Smbios::Type16> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type16 t7(count++);
		if (t7.IsAvailable())
			temp.emplace_back(std::move(t7));
		else
			break;
	}
	return std::move(temp);
}

std::vector<Smbios::Type17> Smbios::GetType(SmbiosHeaderType::Type17)
{
	std::vector<Smbios::Type17> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type17 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type18> Smbios::GetType(SmbiosHeaderType::Type18)
{
	std::vector<Smbios::Type18> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type18 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type19> Smbios::GetType(SmbiosHeaderType::Type19)
{
	std::vector<Smbios::Type19> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type19 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type20> Smbios::GetType(SmbiosHeaderType::Type20)
{
	std::vector<Smbios::Type20> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type20 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type21> Smbios::GetType(SmbiosHeaderType::Type21)
{
	std::vector<Smbios::Type21> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type21 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type22> Smbios::GetType(SmbiosHeaderType::Type22)
{
	std::vector<Smbios::Type22> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type22 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type23> Smbios::GetType(SmbiosHeaderType::Type23)
{
	std::vector<Smbios::Type23> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type23 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type24> Smbios::GetType(SmbiosHeaderType::Type24)
{
	std::vector<Smbios::Type24> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type24 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type25> Smbios::GetType(SmbiosHeaderType::Type25)
{
	std::vector<Smbios::Type25> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type25 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type26> Smbios::GetType(SmbiosHeaderType::Type26)
{
	std::vector<Smbios::Type26> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type26 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type27> Smbios::GetType(SmbiosHeaderType::Type27)
{
	std::vector<Smbios::Type27> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type27 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type28> Smbios::GetType(SmbiosHeaderType::Type28)
{
	std::vector<Smbios::Type28> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type28 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type29> Smbios::GetType(SmbiosHeaderType::Type29)
{
	std::vector<Smbios::Type29> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type29 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type30> Smbios::GetType(SmbiosHeaderType::Type30)
{
	std::vector<Smbios::Type30> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type30 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type32> Smbios::GetType(SmbiosHeaderType::Type32)
{
	std::vector<Smbios::Type32> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type32 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type33> Smbios::GetType(SmbiosHeaderType::Type33)
{
	std::vector<Smbios::Type33> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type33 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type34> Smbios::GetType(SmbiosHeaderType::Type34)
{
	std::vector<Smbios::Type34> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type34 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type35> Smbios::GetType(SmbiosHeaderType::Type35)
{
	std::vector<Smbios::Type35> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type35 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type36> Smbios::GetType(SmbiosHeaderType::Type36)
{
	std::vector<Smbios::Type36> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type36 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type37> Smbios::GetType(SmbiosHeaderType::Type37)
{
	std::vector<Smbios::Type37> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type37 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type38> Smbios::GetType(SmbiosHeaderType::Type38)
{
	std::vector<Smbios::Type38> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type38 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type39> Smbios::GetType(SmbiosHeaderType::Type39)
{
	std::vector<Smbios::Type39> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type39 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type40> Smbios::GetType(SmbiosHeaderType::Type40)
{
	std::vector<Smbios::Type40> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type40 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type41> Smbios::GetType(SmbiosHeaderType::Type41)
{
	std::vector<Smbios::Type41> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type41 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type42> Smbios::GetType(SmbiosHeaderType::Type42)
{
	std::vector<Smbios::Type42> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type42 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type43> Smbios::GetType(SmbiosHeaderType::Type43)
{
	std::vector<Smbios::Type43> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type43 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Type44> Smbios::GetType(SmbiosHeaderType::Type44)
{
	std::vector<Smbios::Type44> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Type44 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Lenovo::Type131> Smbios::GetType(SmbiosHeaderType::Type131)
{
	std::vector<Smbios::Lenovo::Type131> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Lenovo::Type131 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Lenovo::Type133> Smbios::GetType(SmbiosHeaderType::Type133)
{
	std::vector<Smbios::Lenovo::Type133> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Lenovo::Type133 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Lenovo::Type134> Smbios::GetType(SmbiosHeaderType::Type134)
{
	std::vector<Smbios::Lenovo::Type134> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Lenovo::Type134 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Lenovo::Type140> Smbios::GetType(SmbiosHeaderType::Type140)
{
	std::vector<Smbios::Lenovo::Type140> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Lenovo::Type140 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Acer::Type170> Smbios::GetType(SmbiosHeaderType::Type170)
{
	std::vector<Smbios::Acer::Type170> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Acer::Type170 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}

std::vector<Smbios::Lenovo::Type200> Smbios::GetType(SmbiosHeaderType::Type200)
{
	std::vector<Smbios::Lenovo::Type200> temp;
	auto count = 0;
	while (true)
	{
		Smbios::Lenovo::Type200 t17(count++);
		if (t17.IsAvailable())
			temp.emplace_back(std::move(t17));
		else
			break;
	}
	return  std::move(temp);
}