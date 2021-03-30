#pragma once
#include <sstream>
#include <string>
#include <exception>

#define GetSmbiosType(x) #x

namespace Smbios
{
	class Exception : public std::exception
	{
	public:
		explicit Exception(const std::string& FuncName)
			: FuncName(FuncName)
		{
		}
		char const* what() const override
		{
			return FuncName.c_str();
		}
	private:
		std::string FuncName;
	};
}