#include "stdafx.h"
#include "SmbiosDefination.h"

namespace
{
	class Type0 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type0(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"BIOS Information";
		}
	};

	class Type1 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type1(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"System Information";
		}
	};

	class Type2 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type2(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Baseboard (or Module) Information";
		}
	};

	class Type3 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type3(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"System Enclosure or Chassis";
		}
	};

	class Type4 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type4(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Processor Information";
		}
	};

	class Type5 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type5(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Memory Controller Information";
		}
	};

	class Type6 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type6(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Memory Module Information";
		}
	};

	class Type7 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type7(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Cache Information";
		}
	};

	class Type8 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type8(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Port Connector Information";
		}
	};

	class Type9 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type9(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"System Slots";
		}
	};

	class Type10 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type10(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"On Board Devices Information";
		}
	};

	class Type11 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type11(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"OEM Strings";
		}
	};

	class Type12 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type12(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"System Configuration Options";
		}
	};

	class Type13 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type13(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"BIOS Language Information";
		}
	};

	class Type14 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type14(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Group Associations";
		}
	};

	class Type15 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type15(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"System Event Log";
		}
	};

	class Type16 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type16(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Physical Memory Array";
		}
	};

	class Type17 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type17(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Memory Device";
		}
	};

	class Type18 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type18(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"32-Bit Memory Error Information";
		}
	};

	class Type19 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type19(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Memory Array Mapped Address";
		}
	};

	class Type20 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type20(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Memory Device Mapped Address";
		}
	};

	class Type21 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type21(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Built-in Pointing Device";
		}
	};

	class Type22 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type22(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Portable Battery";
		}
	};

	class Type23 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type23(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"System Reset";
		}
	};

	class Type24 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type24(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Hardware Security";
		}
	};

	class Type25 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type25(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"System Power Controls";
		}
	};

	class Type26 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type26(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Voltage Probe";
		}
	};

	class Type27 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type27(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Cooling Device";
		}
	};

	class Type28 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type28(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Temperature Probe";
		}
	};

	class Type29 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type29(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Electrical Current Probe";
		}
	};

	class Type30 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type30(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Out-of-Band Remote Access";
		}
	};

	class Type31 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type31(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Boot Integrity Services (BIS) Entry Point";
		}
	};

	class Type32 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type32(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"System Boot Information";
		}
	};

	class Type33 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type33(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"64-Bit Memory Error Information";
		}
	};

	class Type34 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type34(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Management Device";
		}
	};

	class Type35 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type35(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Management Device Component";
		}
	};

	class Type36 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type36(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Management Device Threshold Data";
		}
	};

	class Type37 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type37(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Memory Channel";
		}
	};

	class Type38 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type38(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"IPMI Device Information";
		}
	};

	class Type39 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type39(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"System Power Supply";
		}
	};

	class Type40 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type40(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Additional Information";
		}
	};

	class Type41 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type41(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Onboard Devices Extended Information";
		}
	};

	class Type42 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type42(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Management Controller Host Interface";
		}
	};

	class Type43 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type43(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"TPM Device";
		}
	};

	class Type44 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type44(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Processor Additional Information";
		}
	};

	class Type126 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type126(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"Inactive";
		}
	};

	class Type127 final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		Type127(const Smbios::DMI_Header& Header) : TypeBase(Header) {}
		const std::wstring Descriptor() const override
		{
			return L"End-of-Table";
		}
	};

	class TypeUnknown final : public Smbios::SmbiosHeaderType::TypeBase
	{
	public:
		TypeUnknown(const std::wstring& Desc, const std::string& Vendor, const Smbios::DMI_Header& Header)
			: Desc(Desc), Vendor(Vendor), TypeBase(Header) {}
	private:
		const std::wstring Descriptor() const override
		{
			return Desc;
		}
	private:
		std::wstring Desc;
		std::string Vendor;
	};
}

Smbios::SmbiosHeaderType::TypeFactory& Smbios::SmbiosHeaderType::TypeFactory::Instance()
{
	static TypeFactory temp;
	return temp;
}

std::shared_ptr<Smbios::SmbiosHeaderType::TypeBase> Smbios::SmbiosHeaderType::TypeFactory::CreateFactory(const DMI_Header& Type)
{
	switch (Type.type)
	{
	case 0:
		return std::make_shared<::Type0>(Type);
	case 1:
		return std::make_shared<::Type1>(Type);
	case 2:
		return std::make_shared<::Type2>(Type);
	case 3:
		return std::make_shared<::Type3>(Type);
	case 4:
		return std::make_shared<::Type4>(Type);
	case 5:
		return std::make_shared<::Type5>(Type);
	case 6:
		return std::make_shared<::Type6>(Type);
	case 7:
		return std::make_shared<::Type7>(Type);
	case 8:
		return std::make_shared<::Type8>(Type);
	case 9:
		return std::make_shared<::Type9>(Type);
	case 10:
		return std::make_shared<::Type10>(Type);
	case 11:
		return std::make_shared<::Type11>(Type);
	case 12:
		return std::make_shared<::Type12>(Type);
	case 13:
		return std::make_shared<::Type13>(Type);
	case 14:
		return std::make_shared<::Type14>(Type);
	case 15:
		return std::make_shared<::Type15>(Type);
	case 16:
		return std::make_shared<::Type16>(Type);
	case 17:
		return std::make_shared<::Type17>(Type);
	case 18:
		return std::make_shared<::Type18>(Type);
	case 19:
		return std::make_shared<::Type19>(Type);
	case 20:
		return std::make_shared<::Type20>(Type);
	case 21:
		return std::make_shared<::Type21>(Type);
	case 22:
		return std::make_shared<::Type22>(Type);
	case 23:
		return std::make_shared<::Type23>(Type);
	case 24:
		return std::make_shared<::Type24>(Type);
	case 25:
		return std::make_shared<::Type25>(Type);
	case 26:
		return std::make_shared<::Type26>(Type);
	case 27:
		return std::make_shared<::Type27>(Type);
	case 28:
		return std::make_shared<::Type28>(Type);
	case 29:
		return std::make_shared<::Type29>(Type);
	case 30:
		return std::make_shared<::Type30>(Type);
	case 31:
		return std::make_shared<::Type31>(Type);
	case 32:
		return std::make_shared<::Type32>(Type);
	case 33:
		return std::make_shared<::Type33>(Type);
	case 34:
		return std::make_shared<::Type34>(Type);
	case 35:
		return std::make_shared<::Type35>(Type);
	case 36:
		return std::make_shared<::Type36>(Type);
	case 37:
		return std::make_shared<::Type37>(Type);
	case 38:
		return std::make_shared<::Type38>(Type);
	case 39:
		return std::make_shared<::Type39>(Type);
	case 40:
		return std::make_shared<::Type40>(Type);
	case 41:
		return std::make_shared<::Type41>(Type);
	case 42:
		return std::make_shared<::Type42>(Type);
	case 43:
		return std::make_shared<::Type43>(Type);
	case 44:
		return std::make_shared<::Type44>(Type);
	case 126:
		return std::make_shared<::Type126>(Type);
	case 127:
		return std::make_shared<::Type127>(Type);
	default:
		return OEMSmbiosType(Type);
	}
}

void Smbios::SmbiosHeaderType::TypeFactory::SetVendor(const std::string& Vendor)
{
	this->Vendor = Vendor;
}

const std::string& Smbios::SmbiosHeaderType::TypeFactory::GetVendor()
{
	return this->Vendor;
}

std::shared_ptr<Smbios::SmbiosHeaderType::TypeBase> Smbios::SmbiosHeaderType::TypeFactory::OEMSmbiosType(const DMI_Header& Type)
{
	if (!Vendor.empty())
	{
		if (Vendor == "Acer")
		{
		}
		else if (Vendor == "HP" || Vendor == "HPE" || Vendor == "Hewlett-Packard" || Vendor == "Hewlett Packard Enterprise")
		{
		}
		else if (Vendor == "IBM" || Vendor == "LENOVO")
		{
			switch (Type.type)
			{
			case 131:
				return std::make_shared<::TypeUnknown>(L"TVT Enablement", Vendor, Type);
			case 133:
				return std::make_shared<::TypeUnknown>(L"ThinkVantage Technologies feature", Vendor, Type);
			case 134:
				return std::make_shared<::TypeUnknown>(L"TPM Data", Vendor, Type);
			case 140:
				return std::make_shared<::TypeUnknown>(L"CMP Data", Vendor, Type);
			case 200:
				return std::make_shared<::TypeUnknown>(L"IdeaPad Identify Product Data", Vendor, Type);
			default:
				break;
			}
		}
	}
	return std::make_shared<::TypeUnknown>(std::wstring(L"Unknown Type ") + std::to_wstring(Type.type + 0), Vendor, Type);
}