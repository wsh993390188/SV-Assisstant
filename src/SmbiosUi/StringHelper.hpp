#pragma once
#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <sstream>
#include <wctype.h>

inline namespace {
	/// @brief 宽字符串转Utf8格式字符串
	///			由于codecvt_utf8在C++17中被弃用，暂时没找到替代方式
	/// @param[in] str 宽字符串
	/// @return Utf8格式字符串
	std::string wstringToUtf8(const std::wstring& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.to_bytes(str);
	}

	/// @brief Utf8格式字符串转宽字符串
	///			由于codecvt_utf8在C++17中被弃用，暂时没找到替代方式
	/// @param[in] str Utf8字符串
	/// @return 宽字符串

	std::wstring utf8ToWstring(const std::string& str)
	{
		std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.from_bytes(str);
	}

	template<typename T>
	constexpr decltype(auto) ToUType(T enumerator)
	{
		return static_cast<std::underlying_type_t<T>>(enumerator);
	}
}