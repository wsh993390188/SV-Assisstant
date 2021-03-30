#pragma once
#include <type_traits>
namespace Smbios
{
	//获取每个类型的入口地址
	class EntryPoint
	{
	public:
		EntryPoint();
		~EntryPoint() = default;

		//获取字符段中相应字符串
		std::string DMIToString(uint8_t Targetstring) const;

		//切换到对应的Type地址，TypeNum是Type号，TargetTable是对应表号。
		void SwitchToTypePhyAddress(uint8_t TypeNum, int8_t TargetTable = 0);

		void ResetPhyAddress();

		uint8_t* TypePhyAddress;
		uint8_t SmbiosMajorVersion;
		uint8_t SmbiosMinorVersion;
	};

	class TypeImpl
	{
	public:
		TypeImpl();
		virtual ~TypeImpl();
		virtual TypeImpl& Create(const int32_t&)
		{
			return *this;
		}

		TypeImpl& Create(void* PhysicalAddress)
		{
			type.TypePhyAddress = reinterpret_cast<uint8_t*>(PhysicalAddress);
			return *this;
		}

		uint8_t* GetTypePhyAddress() const
		{
			return type.TypePhyAddress;
		}

		std::string GetTargetString(uint8_t TargetStringNumber)
		{
			return type.DMIToString(TargetStringNumber);
		}

		virtual std::string GetType() = 0;

		bool CheckValueIsSupport(const std::string& Name, const size_t TargerLength)
		{
			if (!type.TypePhyAddress)
			{
				std::ostringstream oss;
				oss << "Smbios " << GetType() << " Version: " << (uint16_t)type.SmbiosMajorVersion << "." << (uint16_t)type.SmbiosMinorVersion << " " << " Not This Type";
				throw Smbios::Exception(oss.str());
			}
			bool res = false;
			auto TypeDecode = reinterpret_cast<Smbios::DMI_Header*>(type.TypePhyAddress);

			auto itr = TypeVersion.find(Name);
			if (itr != TypeVersion.end())
			{
				if (itr->second.first < type.SmbiosMajorVersion)
				{
					res = true;
				}
				else if (itr->second.first == type.SmbiosMajorVersion)
				{
					if (itr->second.second <= type.SmbiosMinorVersion)
					{
						res = true;
					}
				}
			}
			if (res)
			{
				if (TargerLength > TypeDecode->length)
				{
					std::ostringstream oss;
					oss << "Smbios " << GetType() << " Version: "
						<< (uint16_t)type.SmbiosMajorVersion << "." <<
						(uint16_t)type.SmbiosMinorVersion << " "
						<< Name << " due to length Too Small ";
					throw Smbios::Exception(oss.str());
				}
				return res;
			}
			std::ostringstream oss;
			oss << "Smbios " << GetType() << " Version: " << (uint16_t)type.SmbiosMajorVersion << "." << (uint16_t)type.SmbiosMinorVersion << " " << Name << " Not Support";
			throw Smbios::Exception(oss.str());
		}

		static const std::string GetFuncRealName(const char* FuncName)
		{
			std::string Str(FuncName);
			auto res = Str.find_last_of(":");
			if (res == std::string::npos)
				return FuncName;
			else
				return Str.substr(res + 1);
		}
	protected:
		EntryPoint type;
		std::map<std::string, std::pair<uint8_t, uint8_t>> TypeVersion;
	};

	template<class T>
	class ExceptionUsed
	{
	public:
		template<typename ...Args>
		static bool CheckValueIsSupport(T& impl, Args&&... args)
		{
			return impl.CheckValueIsSupport(std::forward<Args>(args)...);
		}

		using ReturnType = uint8_t;
	};

	template<class T>
	class ExceptionNoUsed
	{
	public:
		template<typename ...Args>
		static bool CheckValueIsSupport(T& impl, Args&&... args)
		{
			try
			{
				return impl.CheckValueIsSupport(std::forward<Args>(args)...);
			}
			catch (...)
			{
				return false;
			}
		}

		using ReturnType = bool;
	};

	template<typename T, class TypeImpl>
	typename std::enable_if_t<std::is_integral<T>::value, T>
		GetSmbiosInformation(TypeImpl& pimpl, const int Index)
	{
		T temp = *reinterpret_cast<T*>(&pimpl.GetTypePhyAddress()[Index]);
		return pimpl.GetTypePhyAddress() ? temp : 0;
	}

	template<typename T = std::string, class TypeImpl>
	typename std::enable_if_t<!std::is_integral<T>::value&& std::is_same<T, std::string>::value, T>
		GetSmbiosInformation(TypeImpl& pimpl, const int Index)
	{
		return pimpl.GetTypePhyAddress() ? pimpl.GetTargetString(pimpl.GetTypePhyAddress()[Index]) : T();
	}

	template<template <class> class ExceptionPolicy,
		typename BiosType,
		typename ReturnType,
		class TypeImpl,
		typename Func,
		typename std::enable_if_t<std::is_same<BiosType, std::string>::value, int> = 0>
		typename std::enable_if_t<!std::is_same<typename ExceptionPolicy<TypeImpl>::ReturnType, bool>::value, ReturnType>
		GetSmbiosInformationWrapper(TypeImpl& pimpl, const int Index, const std::string& FuncName, Func func)
	{
		ExceptionPolicy<TypeImpl>::CheckValueIsSupport(pimpl, FuncName, Index);
		auto temp = GetSmbiosInformation<BiosType>(pimpl, Index);
		auto ret = func(temp);
		return ret;
	}

	template<template <class> class ExceptionPolicy,
		typename BiosType,
		typename ReturnType,
		class TypeImpl,
		typename Func,
		typename std::enable_if_t<std::is_integral<BiosType>::value, int> = 0>
		typename std::enable_if_t<!std::is_same<typename ExceptionPolicy<TypeImpl>::ReturnType, bool>::value, ReturnType>
		GetSmbiosInformationWrapper(TypeImpl& pimpl, const int Index, const std::string& FuncName, Func func)
	{
		ExceptionPolicy<TypeImpl>::CheckValueIsSupport(pimpl, FuncName, Index + sizeof(BiosType) - 1);
		auto temp = GetSmbiosInformation<BiosType>(pimpl, Index);
		auto ret = func(temp);
		return ret;
	}

	template<template <class> class ExceptionPolicy,
		typename BiosType,
		typename ReturnType,
		class TypeImpl,
		typename Func,
		typename std::enable_if_t<std::is_same<BiosType, std::string>::value, int> = 0>
		typename std::enable_if_t<std::is_same<typename ExceptionPolicy<TypeImpl>::ReturnType, bool>::value, bool>
		GetSmbiosInformationWrapper(TypeImpl& pimpl, const int Index, const std::string& FuncName, Func func, ReturnType& ret)
	{
		auto res = ExceptionPolicy<TypeImpl>::CheckValueIsSupport(pimpl, FuncName);
		if (res)
		{
			auto temp = GetSmbiosInformation<BiosType>(pimpl, Index, Index);
			ret = func(temp);
		}
		return res;
	}

	template<template <class> class ExceptionPolicy,
		typename BiosType,
		typename ReturnType,
		class TypeImpl,
		typename Func,
		typename std::enable_if_t<std::is_integral<BiosType>::value, int> = 0>
		typename std::enable_if_t<std::is_same<typename ExceptionPolicy<TypeImpl>::ReturnType, bool>::value, bool>
		GetSmbiosInformationWrapper(TypeImpl& pimpl, const int Index, const std::string& FuncName, Func func, ReturnType& ret)
	{
		auto res = ExceptionPolicy<TypeImpl>::CheckValueIsSupport(pimpl, FuncName, Index + sizeof(BiosType) - 1);
		if (res)
		{
			auto temp = GetSmbiosInformation<BiosType>(pimpl, Index);
			ret = func(temp);
		}
		return res;
	}

	template<template <class> class ExceptionPolicy,
		typename T,
		class TypeImpl,
		typename std::enable_if_t<std::is_same<T, std::string>::value, int> = 0
	>
		typename std::enable_if_t<!std::is_same<typename ExceptionPolicy<TypeImpl>::ReturnType, bool>::value, T>
		GetSmbiosInformationWrapper(TypeImpl& pimpl, const int Index, const std::string& FuncName)
	{
		ExceptionPolicy<TypeImpl>::CheckValueIsSupport(pimpl, FuncName, Index);
		auto ret = GetSmbiosInformation<T>(pimpl, Index);
		return ret;
	}

	template<template <class> class ExceptionPolicy,
		typename T,
		class TypeImpl,
		typename std::enable_if_t<std::is_integral<T>::value, int> = 0
	>
		typename std::enable_if_t<!std::is_same<typename ExceptionPolicy<TypeImpl>::ReturnType, bool>::value, T>
		GetSmbiosInformationWrapper(TypeImpl& pimpl, const int Index, const std::string& FuncName)
	{
		ExceptionPolicy<TypeImpl>::CheckValueIsSupport(pimpl, FuncName, Index + sizeof(T) - 1);
		auto ret = GetSmbiosInformation<T>(pimpl, Index);
		return ret;
	}

	template<template <class> class ExceptionPolicy,
		typename T,
		class TypeImpl,
		typename std::enable_if_t<std::is_same<T, std::string>::value, int> = 0
	>
		typename std::enable_if_t<std::is_same<typename ExceptionPolicy<TypeImpl>::ReturnType, bool>::value, bool>
		GetSmbiosInformationWrapper(TypeImpl& pimpl, const int Index, const std::string& FuncName, T& ret)
	{
		auto res = ExceptionPolicy<TypeImpl>::CheckValueIsSupport(pimpl, FuncName, Index);
		if (res)
		{
			ret = GetSmbiosInformation<T>(pimpl, Index);
		}
		return res;
	}

	template<template <class> class ExceptionPolicy,
		typename T,
		class TypeImpl,
		typename std::enable_if_t<std::is_integral<T>::value, int> = 0
	>
		typename std::enable_if_t<std::is_same<typename ExceptionPolicy<TypeImpl>::ReturnType, bool>::value, bool>
		GetSmbiosInformationWrapper(TypeImpl& pimpl, const int Index, const std::string& FuncName, T& ret)
	{
		auto res = ExceptionPolicy<TypeImpl>::CheckValueIsSupport(pimpl, FuncName, Index + sizeof(T) - 1);
		if (res)
		{
			ret = GetSmbiosInformation<T>(pimpl, Index);
		}
		return res;
	}
}
