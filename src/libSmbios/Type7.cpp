#include "stdafx.h"

#include "Type7.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto SocketDesignation = 0x04;
	constexpr auto CacheConfiguration = 0x05;
	constexpr auto MaximumCacheSize = 0x07;
	constexpr auto InstalledSize = 0x09;
	constexpr auto SupportedSRAMType = 0x0B;
	constexpr auto CurrentSRAMType = 0x0D;
	constexpr auto CacheSpeed = 0x0F;
	constexpr auto ErrorCorrectionType = 0x10;
	constexpr auto SystemCacheType = 0x11;
	constexpr auto Associativity = 0x12;
	constexpr auto MaximumCacheSize2 = 0x13;
	constexpr auto InstalledCacheSize2 = 0x17;

	const std::string ErrorCorrectionTypeStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("None"),
		("Parity"),
		("Single-bit ECC"),
		("Multi-bit ECC"),
		("Reserved")
	};

	const std::string SystemCacheTypeStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Instruction"),
		("Data"),
		("Unified")
	};

	const std::string CacheAssociativityStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Direct Mapped"),
		("2-way Set-Associative"),
		("4-way Set-Associative"),
		("Fully Associative"),
		("8-way Set-Associative"),
		("16-way Set-Associative"),
		("12-way Set-Associative"),
		("24-way Set-Associative"),
		("32-way Set-Associative"),
		("48-way Set-Associative"),
		("64-way Set-Associative"),
		("20-way Set-Associative"),
		("Reserved"),
	};
}
namespace
{
	class Type7Impl : public Smbios::TypeImpl
	{
	public:
		Type7Impl()
		{
			TypeVersion = {
				{GetSmbiosType(SocketDesignation), std::make_pair(2,0)},
				{ GetSmbiosType(CacheConfiguration),std::make_pair(2,0) },
				{ GetSmbiosType(MaximumCacheSize),std::make_pair(2,0) },
				{ GetSmbiosType(InstalledSize),std::make_pair(2,0) },
				{ GetSmbiosType(SupportedSRAMType),std::make_pair(2,0) },
				{ GetSmbiosType(CurrentSRAMType),std::make_pair(2,0) },
				{ GetSmbiosType(CacheSpeed),std::make_pair(2,1) },
				{ GetSmbiosType(ErrorCorrectionType),std::make_pair(2,1) },
				{ GetSmbiosType(SystemCacheType),std::make_pair(2,1) },
				{ GetSmbiosType(Associativity),std::make_pair(2,3) },
				{ GetSmbiosType(MaximumCacheSize2),std::make_pair(3,1) },
				{ GetSmbiosType(InstalledCacheSize2),std::make_pair(3,1) }
			};
		}
		~Type7Impl() = default;
		Type7Impl(const Type7Impl&) = delete;
		Type7Impl& operator=(const Type7Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type7";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(7, TableNum);
		}
	};
}

Smbios::Type7::Type7(const int32_t& TableNum) : pImpl(std::make_unique<Type7Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type7::Type7(void* PhysicalAddress) : pImpl(std::make_unique<Type7Impl>())
{
	pImpl->Create(PhysicalAddress);
}

Smbios::Type7::~Type7() = default;

Smbios::Type7::Type7(Type7&&) = default;

Smbios::Type7& Smbios::Type7::operator=(Type7&&) = default;

bool Smbios::Type7::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

std::string Smbios::Type7::SocketDesignation()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::SocketDesignation, Type7Impl::GetFuncRealName(__FUNCTION__));
}
bool Smbios::Type7::SocketDesignation(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::SocketDesignation, Type7Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type7::CacheConfiguration()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>(*pImpl, ::CacheConfiguration, Type7Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type7::CacheConfiguration(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::CacheConfiguration, Type7Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type7::MaximumCacheSize()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, std::string>
		(*pImpl, ::MaximumCacheSize, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		if (ret == 0xFFFF)
			return std::string(">2048MB");
		if (ret & 0x8000)
		{
			return std::to_string((ret & 0x7FFF) * 64) + "KB";
		}
		else
		{
			return std::to_string(ret) + "KB";
		}
	});
}

bool Smbios::Type7::MaximumCacheSize(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>
		(*pImpl, ::MaximumCacheSize, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		if (ret == 0xFFFF)
			return std::string(">2048MB");
		if (ret & 0x8000)
		{
			return std::to_string((ret & 0x7FFF) * 64) + "KB";
		}
		else
		{
			return std::to_string(ret) + "KB";
		}
	}, Str);
}

std::string Smbios::Type7::InstalledSize()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, std::string>
		(*pImpl, ::InstalledSize, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		if (ret == 0xFFFF)
			return std::string(">2048MB");
		if (ret & 0x8000)
		{
			return std::to_string((ret & 0x7FFF) * 64) + "KB";
		}
		else
		{
			return std::to_string(ret) + "KB";
		}
	});
}

bool Smbios::Type7::InstalledSize(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>
		(*pImpl, ::InstalledSize, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		if (ret == 0xFFFF)
			return std::string(">2048MB");
		if (ret & 0x8000)
		{
			return std::to_string((ret & 0x7FFF) * 64) + "KB";
		}
		else
		{
			return std::to_string(ret) + "KB";
		}
	}, Str);
}

Smbios::SRAMType Smbios::Type7::SupportedSRAMType()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, Smbios::SRAMType>
		(*pImpl, ::SupportedSRAMType, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		return static_cast<Smbios::SRAMType>(ret);
	});
}

bool Smbios::Type7::SupportedSRAMType(Smbios::SRAMType & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>
		(*pImpl, ::SupportedSRAMType, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		return static_cast<Smbios::SRAMType>(ret);
	}, Str);
}

Smbios::SRAMType Smbios::Type7::CurrentSRAMType()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, Smbios::SRAMType>
		(*pImpl, ::CurrentSRAMType, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		return static_cast<Smbios::SRAMType>(ret);
	});
}

bool Smbios::Type7::CurrentSRAMType(Smbios::SRAMType & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>
		(*pImpl, ::CurrentSRAMType, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& ret)
	{
		return static_cast<Smbios::SRAMType>(ret);
	}, Str);
}

uint8_t Smbios::Type7::CacheSpeed()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>(*pImpl, ::CacheSpeed, Type7Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type7::CacheSpeed(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::CacheSpeed, Type7Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type7::ErrorCorrectionType()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::ErrorCorrectionType, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret < _countof(ErrorCorrectionTypeStrs))
		{
			return ErrorCorrectionTypeStrs[ret];
		}
		return std::string();
	});
}

bool Smbios::Type7::ErrorCorrectionType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::ErrorCorrectionType, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret < _countof(ErrorCorrectionTypeStrs))
		{
			return ErrorCorrectionTypeStrs[ret];
		}
		return std::string();
	}, Str);
}

std::string Smbios::Type7::SystemCacheType()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::SystemCacheType, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret < _countof(SystemCacheTypeStrs))
		{
			return SystemCacheTypeStrs[ret];
		}
		return std::string();
	});
}

bool Smbios::Type7::SystemCacheType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::SystemCacheType, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret < _countof(SystemCacheTypeStrs))
		{
			return SystemCacheTypeStrs[ret];
		}
		return std::string();
	}, Str);
}

std::string Smbios::Type7::Associativity()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::Associativity, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret < _countof(CacheAssociativityStrs))
		{
			return CacheAssociativityStrs[ret];
		}
		return std::string();
	});
}

bool Smbios::Type7::Associativity(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::Associativity, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret < _countof(CacheAssociativityStrs))
		{
			return CacheAssociativityStrs[ret];
		}
		return std::string();
	}, Str);
}

std::string Smbios::Type7::MaximumCacheSize2()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t, std::string>
		(*pImpl, ::MaximumCacheSize2, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint32_t& ret)
	{
		if (ret & 0x8000'0000)
		{
			return std::to_string((ret & 0x7FFF'FFFF) * 64) + "KB";
		}
		else
		{
			return std::to_string(ret) + "KB";
		}
	});
}

bool Smbios::Type7::MaximumCacheSize2(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint32_t>
		(*pImpl, ::MaximumCacheSize2, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint32_t& ret)
	{
		if (ret & 0x8000'0000)
		{
			return std::to_string((ret & 0x7FFF'FFFF) * 64) + "KB";
		}
		else
		{
			return std::to_string(ret) + "KB";
		}
	}, Str);
}

std::string Smbios::Type7::InstalledCacheSize2()const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t, std::string>
		(*pImpl, ::InstalledCacheSize2, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint32_t& ret)
	{
		if (ret & 0x8000'0000)
		{
			return std::to_string((ret & 0x7FFF'FFFF) * 64) + "KB";
		}
		else
		{
			return std::to_string(ret) + "KB";
		}
	});
}

bool Smbios::Type7::InstalledCacheSize2(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint32_t>
		(*pImpl, ::InstalledCacheSize2, Type7Impl::GetFuncRealName(__FUNCTION__),
			[](const uint32_t& ret)
	{
		if (ret & 0x8000'0000)
		{
			return std::to_string((ret & 0x7FFF'FFFF) * 64) + "KB";
		}
		else
		{
			return std::to_string(ret) + "KB";
		}
	}, Str);
}