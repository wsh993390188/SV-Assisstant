#pragma once
#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <sstream>
#include <wctype.h>
#include "..\..\Libcommon\utfcpp\source\utf8.h"

inline namespace {
	/// @brief 宽字符串转Utf8格式字符串
	///			由于codecvt_utf8在C++17中被弃用，暂时没找到替代方式
	/// @param[in] str 宽字符串
	/// @return Utf8格式字符串
	std::string wstringToUtf8(const std::wstring& wstr)
	{
		std::string utf8line;

		if (wstr.empty())
			return utf8line;

#ifdef _MSC_VER
		utf8::utf16to8(wstr.begin(), wstr.end(), std::back_inserter(utf8line));
#else
		utf8::utf32to8(wstr.begin(), wstr.end(), std::back_inserter(utf8line));
#endif
		return utf8line;
	}

	/// @brief Utf8格式字符串转宽字符串
	///			由于codecvt_utf8在C++17中被弃用，暂时没找到替代方式
	/// @param[in] str Utf8字符串
	/// @return 宽字符串

	std::wstring utf8ToWstring(const std::string& str)
	{
		std::wstring wide_line;

		if (str.empty())
			return wide_line;

#ifdef _MSC_VER
		utf8::utf8to16(str.begin(), str.end(), std::back_inserter(wide_line));
#else
		utf8::utf8to32(str.begin(), str.end(), std::back_inserter(wide_line));
#endif
		return wide_line;
	}

	template<typename T>
	constexpr decltype(auto) ToUType(T enumerator)
	{
		return static_cast<std::underlying_type_t<T>>(enumerator);
	}
}