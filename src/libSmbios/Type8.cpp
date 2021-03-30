#include "stdafx.h"
#include "Type8.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto InternalReferenceDesignator = 0x04;
	constexpr auto InternalConnectorType = 0x05;
	constexpr auto ExternalReferenceDesignator = 0x06;
	constexpr auto ExternalConnectorType = 0x07;
	constexpr auto PortType = 0x08;
	//Bytes or Bit
	const std::string ConnectorTypeStrs[] =
	{
		("None"),
		("Centronics"),
		("Mini Centronics"),
		("Proprietary"),
		("DB-25 pin male"),
		("DB-25 pin female"),
		("DB-15 pin male"),
		("DB-15 pin female"),
		("DB-9 pin male"),
		("DB-9 pin female"),
		("RJ-11"),
		("RJ-45"),
		("50-pin MiniSCSI"),
		("Mini-DIN"),
		("Micro-DIN"),
		("PS/2"),
		("Infrared"),
		("HP-HIL"),
		("Access Bus (USB)"),
		("SSA SCSI"),
		("Circular DIN-8 male"),
		("Circular DIN-8 female"),
		("On Board IDE"),
		("On Board Floppy"),
		("9-pin Dual Inline (pin 10 cut)"),
		("25-pin Dual Inline (pin 26 cut)"),
		("50-pin Dual Inline"),
		("68-pin Dual Inline"),
		("On Board Sound Input from CD-ROM"),
		("Mini-Centronics Type-14"),
		("Mini-Centronics Type-26"),
		("Mini-jack (headphones)"),
		("BNC"),
		("1394"),
		("SAS/SATA Plug Receptacle"),
		("PC-98"),
		("PC-98Hireso"),
		("PC-H98"),
		("PC-98Note"),
		("PC-98Full"),
	};

	const std::string PortTypesStrs[] =
	{
		("None"),
		("Parallel Port XT/AT Compatible"),
		("Parallel Port PS/2"),
		("Parallel Port ECP"),
		("Parallel Port EPP"),
		("Parallel Port ECP/EPP"),
		("Serial Port XT/AT Compatible"),
		("Serial Port 16450 Compatible"),
		("Serial Port 16550 Compatible"),
		("Serial Port 16550A Compatible"),
		("SCSI Port"),
		("MIDI Port"),
		("Joy Stick Port"),
		("Keyboard Port"),
		("Mouse Port"),
		("SSA SCSI"),
		("USB"),
		("FireWire (IEEE P1394)"),
		("PCMCIA Type I2"),
		("PCMCIA Type II"),
		("PCMCIA Type III"),
		("Cardbus"),
		("Access Bus Port"),
		("SCSI II"),
		("SCSI Wide"),
		("PC-98"),
		("PC-98-Hireso"),
		("PC-H98"),
		("Video Port"),
		("Audio Port"),
		("Modem Port"),
		("Network Port"),
		("SATA"),
		("SAS"),
		("8251 Compatible"),
		("8251 FIFO Compatible")
	};
}

namespace
{
	class Type8Impl : public Smbios::TypeImpl
	{
	public:
		Type8Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(InternalReferenceDesignator),std::make_pair(0,0)},
				{GetSmbiosType(InternalConnectorType),std::make_pair(0,0)},
				{GetSmbiosType(ExternalReferenceDesignator),std::make_pair(0,0)},
				{GetSmbiosType(ExternalConnectorType),std::make_pair(0,0)},
				{GetSmbiosType(PortType),std::make_pair(0,0)}
			};
		}
		~Type8Impl() = default;
		Type8Impl(const Type8Impl&) = delete;
		Type8Impl& operator=(const Type8Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type8";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(8, TableNum);
		}
	};
}

Smbios::Type8::Type8(const int32_t& TableNum) : pImpl(std::make_unique<Type8Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type8::Type8(void* PhysicalAddress) : pImpl(std::make_unique<Type8Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type8::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type8::~Type8() = default;

Smbios::Type8::Type8(Type8&&) = default;

Smbios::Type8& Smbios::Type8::operator=(Type8&&) = default;

std::string Smbios::Type8::InternalReferenceDesignator() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::InternalReferenceDesignator, Type8Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type8::InternalReferenceDesignator(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::InternalReferenceDesignator, Type8Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type8::InternalConnectorType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::InternalConnectorType, Type8Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret == 0xFF)
			return std::string("Other");
		if (ret < _countof(ConnectorTypeStrs))
		{
			return ConnectorTypeStrs[ret];
		}
		return std::string();
	});
}

bool Smbios::Type8::InternalConnectorType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::InternalConnectorType, Type8Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret == 0xFF)
			return std::string("Other");
		if (ret < _countof(ConnectorTypeStrs))
		{
			return ConnectorTypeStrs[ret];
		}
		return std::string();
	}, Str);
}

std::string Smbios::Type8::ExternalReferenceDesignator() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::ExternalReferenceDesignator, Type8Impl::GetFuncRealName(__FUNCTION__));
}
bool Smbios::Type8::ExternalReferenceDesignator(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ExternalReferenceDesignator, Type8Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type8::ExternalConnectorType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::ExternalConnectorType, Type8Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret == 0xFF)
			return std::string("Other");
		if (ret < _countof(ConnectorTypeStrs))
		{
			return ConnectorTypeStrs[ret];
		}
		return std::string();
	});
}

bool Smbios::Type8::ExternalConnectorType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::ExternalConnectorType, Type8Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret == 0xFF)
			return std::string("Other");
		if (ret < _countof(ConnectorTypeStrs))
		{
			return ConnectorTypeStrs[ret];
		}
		return std::string();
	}, Str);
}

std::string Smbios::Type8::PortType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::PortType, Type8Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret == 0xFF)
			return std::string("Other");
		if (ret < _countof(PortTypesStrs))
		{
			return PortTypesStrs[ret];
		}
		return std::string();
	});
}

bool Smbios::Type8::PortType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::PortType, Type8Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret == 0xFF)
			return std::string("Other");
		if (ret < _countof(PortTypesStrs))
		{
			return PortTypesStrs[ret];
		}
		return std::string();
	}, Str);
}