/*!
* @file StringReplace.hpp
* @brief 操作标准库字符串
*
*
* @author 王硕(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020年5月29日
*/
#pragma once
#include <string>
#include <locale>
#include <codecvt>
#include <wctype.h>

inline namespace {
	/// @brief 标准库字符串替换功能
	/// @tparam CharT 字符串类型
	/// @param[in,out] str 替换的字符串
	/// @param[in] from 被替换的字符
	/// @param[in] to 替换的字符
	/// @return 操作是否成功
	template <typename CharT>
	bool replace(std::basic_string<CharT>& str,
		const std::basic_string<CharT>& from,
		const std::basic_string<CharT>& to)
	{
		size_t start_pos = str.find(from);
		if (start_pos == std::basic_string<CharT>::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

	/// @brief 宽字符串不区分大小写比较
	/// @param[in] a 比较的第一个字符串
	/// @param[in] b 比较的第二个字符串
	/// @return 两个字符串是否一致
	bool StringIsEqualsNoCase(
		const std::wstring& a,
		const std::wstring& b)
	{
		return std::equal(a.begin(), a.end(),
			b.begin(), b.end(),
			[](wchar_t a, wchar_t b) {
				return towlower(a) == towlower(b);
			});
	}
	/// @brief 字符串不区分大小写比较
	/// @param[in] a 比较的第一个字符串
	/// @param[in] b 比较的第二个字符串
	/// @return 两个字符串是否一致
	bool StringIsEqualsNoCase(
		const std::string& a,
		const std::string& b)
	{
		return std::equal(a.begin(), a.end(),
			b.begin(), b.end(),
			[](char a, char b) {
				return tolower(a) == tolower(b);
			});
	}

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
	std::wstring utf8ToWstring(std::string str)
	{
		std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.from_bytes(str);
	}
}