#include "stdafx.h"
#include "Type42.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto InterfaceType = 0x04;
	constexpr auto InterfaceDataLength = 0x05;
	constexpr auto InterfaceData = 0x06;
	constexpr auto NumberOfProtocol = 0x06;
	constexpr auto ProtocolRecords = 0x07;

	constexpr auto ProtocolType = 0;
	constexpr auto ProtocolDataLength = 1;
	constexpr auto ProtocolData = 2;

	const std::string ProtocolTypesStrs[] =
	{
		"Reserved",
		"Reserved",
		"IPMI: Intelligent Platform Management Interface",
		"MCTP: Management Component Transport Protocol",
		"Redfish over IP"
	};
}

namespace
{
	class Type42Impl : public Smbios::TypeImpl
	{
	public:
		Type42Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(InterfaceType),std::make_pair(0,0)},
				{GetSmbiosType(InterfaceDataLength),std::make_pair(0,0)},
				{GetSmbiosType(InterfaceData),std::make_pair(0,0)},
				{GetSmbiosType(NumberOfProtocol),std::make_pair(0,0)},
				{GetSmbiosType(ProtocolRecords),std::make_pair(0,0)},
			};
		}
		~Type42Impl() = default;
		Type42Impl(const Type42Impl&) = delete;
		Type42Impl& operator=(const Type42Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type42";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(42, TableNum);
		}
	};
}

Smbios::Type42::Type42(const int32_t& TableNum) : pImpl(std::make_unique<Type42Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type42::Type42(void* PhysicalAddress) : pImpl(std::make_unique<Type42Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type42::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type42::~Type42() = default;

Smbios::Type42::Type42(Type42&&) = default;

Smbios::Type42& Smbios::Type42::operator=(Type42&&) = default;

std::string Smbios::Type42::InterfaceType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::InterfaceType, Type42Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num <= 0x3F)
			return std::string("MCTP Host Interfaces");
		else if (num == 0x40)
			return std::string("Network Host Interfaces");
		else if (num == 0xF0)
		{
			return std::string("OEM-defined");
		}
		else
		{
			return std::string("Reserved");
		}
	});
}

bool Smbios::Type42::InterfaceType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::InterfaceType, Type42Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num <= 0x3F)
			return std::string("MCTP Host Interfaces");
		else if (num == 0x40)
			return std::string("Network Host Interfaces");
		else if (num == 0xF0)
		{
			return std::string("OEM-defined");
		}
		else
		{
			return std::string("Reserved");
		}
	}, Str);
}

std::uint8_t Smbios::Type42::InterfaceDataLength() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::InterfaceDataLength, Type42Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type42::InterfaceDataLength(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::InterfaceDataLength, Type42Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::vector<uint8_t> Smbios::Type42::InterfaceData() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::vector<uint8_t>>
		(*pImpl, ::InterfaceDataLength, Type42Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector<uint8_t> ret;
		for (auto index = 0; index < length; ++index)
		{
			ret.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, ::InterfaceData + index));
		}
		return ret;
	});
}

bool Smbios::Type42::InterfaceData(std::vector<uint8_t> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::InterfaceDataLength, Type42Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector<uint8_t> ret;
		for (auto index = 0; index < length; ++index)
		{
			ret.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, ::InterfaceData + index));
		}
		return ret;
	}, Str);
}

uint8_t Smbios::Type42::NumberOfProtocol() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, uint8_t>
		(*pImpl, ::InterfaceDataLength, Type42Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		return GetSmbiosInformation<uint8_t>(*pImpl, ::NumberOfProtocol + length);
	});
}

bool Smbios::Type42::NumberOfProtocol(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::InterfaceDataLength, Type42Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		return GetSmbiosInformation<uint8_t>(*pImpl, ::NumberOfProtocol + length);
	}, Str);
}

std::vector<Smbios::ProtocolRecordDataField> Smbios::Type42::ProtocolRecords() const
{
	auto length = NumberOfProtocol();
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::vector<Smbios::ProtocolRecordDataField>>
		(*pImpl, ::InterfaceDataLength, Type42Impl::GetFuncRealName(__FUNCTION__),
			[this, length](const uint8_t) -> std::vector<Smbios::ProtocolRecordDataField>
	{
		std::vector<Smbios::ProtocolRecordDataField> ret;
		auto begin = length + ::ProtocolData;
		for (auto index = 0; index < length; ++index)
		{
			Smbios::ProtocolRecordDataField temp;
			auto ProtocolType = GetSmbiosInformation<uint8_t>(*pImpl, begin + ::ProtocolType);
			if (ProtocolType < _countof(ProtocolTypesStrs))
			{
				temp.Type = ProtocolTypesStrs[ProtocolType];
			}
			else if (ProtocolType == 0xF0)
			{
				temp.Type = "OEM-defined";
			}
			else
			{
				temp.Type = "Reserved";
			}

			temp.Length = GetSmbiosInformation<uint8_t>(*pImpl, begin + ::ProtocolDataLength);

			std::vector<uint8_t> ProtocolDatas;
			for (auto index = 0; index < temp.Length; ++index)
			{
				ProtocolDatas.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, begin + ::ProtocolData + index));
			}
			temp.ProtocolDatas = ProtocolDatas;
			begin += temp.Length;
			ret.emplace_back(std::move(temp));
		}
		return ret; });
}

bool Smbios::Type42::ProtocolRecords(std::vector<Smbios::ProtocolRecordDataField> & Str) const noexcept
{
	uint8_t length;
	auto res = NumberOfProtocol(length);
	if (!res)
		return false;
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::InterfaceDataLength, Type42Impl::GetFuncRealName(__FUNCTION__),
			[this, length](const uint8_t) -> std::vector<Smbios::ProtocolRecordDataField>
	{
		std::vector<Smbios::ProtocolRecordDataField> ret;
		auto begin = length + ::ProtocolData;
		for (auto index = 0; index < length; ++index)
		{
			Smbios::ProtocolRecordDataField temp;
			auto ProtocolType = GetSmbiosInformation<uint8_t>(*pImpl, begin + ::ProtocolType);
			if (ProtocolType < _countof(ProtocolTypesStrs))
			{
				temp.Type = ProtocolTypesStrs[ProtocolType];
			}
			else if (ProtocolType == 0xF0)
			{
				temp.Type = "OEM-defined";
			}
			else
			{
				temp.Type = "Reserved";
			}

			temp.Length = GetSmbiosInformation<uint8_t>(*pImpl, begin + ::ProtocolDataLength);

			std::vector<uint8_t> ProtocolDatas;
			for (auto index = 0; index < temp.Length; ++index)
			{
				ProtocolDatas.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, begin + ::ProtocolData + index));
			}
			temp.ProtocolDatas = ProtocolDatas;
			begin += temp.Length;
			ret.emplace_back(std::move(temp));
		}
		return ret;
	}, Str);
}