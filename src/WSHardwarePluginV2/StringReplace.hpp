/*!
* @file StringReplace.hpp
* @brief ������׼���ַ���
*
*
* @author ��˶(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020��5��29��
*/
#pragma once
#include <string>
#include <locale>
#include <codecvt>
#include <wctype.h>

inline namespace {
	/// @brief ��׼���ַ����滻����
	/// @tparam CharT �ַ�������
	/// @param[in,out] str �滻���ַ���
	/// @param[in] from ���滻���ַ�
	/// @param[in] to �滻���ַ�
	/// @return �����Ƿ�ɹ�
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

	/// @brief ���ַ��������ִ�Сд�Ƚ�
	/// @param[in] a �Ƚϵĵ�һ���ַ���
	/// @param[in] b �Ƚϵĵڶ����ַ���
	/// @return �����ַ����Ƿ�һ��
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
	/// @brief �ַ��������ִ�Сд�Ƚ�
	/// @param[in] a �Ƚϵĵ�һ���ַ���
	/// @param[in] b �Ƚϵĵڶ����ַ���
	/// @return �����ַ����Ƿ�һ��
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
	std::wstring utf8ToWstring(std::string str)
	{
		std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
		return strCnv.from_bytes(str);
	}
}