#pragma once
#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <sstream>
#include <wctype.h>

inline namespace {
	/// @brief ���ַ���תUtf8��ʽ�ַ���
	///			����codecvt_utf8��C++17�б����ã���ʱû�ҵ������ʽ
	/// @param[in] str ���ַ���
	/// @return Utf8��ʽ�ַ���
	std::string wstringToUtf8(const std::wstring& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.to_bytes(str);
	}

	/// @brief Utf8��ʽ�ַ���ת���ַ���
	///			����codecvt_utf8��C++17�б����ã���ʱû�ҵ������ʽ
	/// @param[in] str Utf8�ַ���
	/// @return ���ַ���

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