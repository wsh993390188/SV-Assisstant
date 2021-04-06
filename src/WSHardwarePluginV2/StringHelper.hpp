/*!
* @file StringSplit.hpp
* @brief �ָ��׼���ַ���
*
*
* @author ��˶(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020��5��29��
*/
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <locale>
#include <codecvt>
#include <wctype.h>
#include "..\..\Libcommon\utfcpp\source\utf8.h"

namespace Hardware
{
	namespace Utils
	{
		inline namespace
		{
			/// @brief �ָ��׼���ַ���
		/// @param[in] str ���ָ���ַ���
		/// @param[in] delim �ָ���
		/// @return ����һ��std::vector�����������ɱ��ָ����ַ���
			template <typename CharT>
			decltype(auto) split(const std::basic_string<CharT>& str,
				const std::basic_string<CharT>& delim)
			{
				std::vector<std::basic_string<CharT>> strVec{};

				// ���ַ������ָ�
				if (str.empty())
					return strVec;

				// �շָ������ָ�
				if (delim.empty()) {
					strVec.push_back(str);
					return strVec;
				}

				typename std::basic_string<CharT>::size_type start{};
				typename std::basic_string<CharT>::size_type end{};

				// �Ҳ����ָ������ԭʼ��
				end = str.find(delim, start);
				if (end == std::basic_string<CharT>::npos) {
					strVec.push_back(str);
					return strVec;
				}
				else {
					strVec.push_back(std::move(str.substr(start, (end - start))));
					start = end + delim.length();
				}

				// ���Ϸָ�����Ӵ�������
				while (end != std::basic_string<CharT>::npos) {
					end = str.find(delim, start);
					strVec.push_back(std::move(str.substr(start, (end - start))));
					start = end + delim.length();
				}

				return strVec;
			}

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

			/// @brief ��׼���ַ���ȫ���滻����
			/// @tparam CharT �ַ�������
			/// @param[in,out] str �滻���ַ���
			/// @param[in] from ���滻���ַ�
			/// @param[in] to �滻���ַ�
			/// @return �����Ƿ�ɹ�
			template <typename CharT>
			bool replaceAll(std::basic_string<CharT>& str,
				const std::basic_string<CharT>& from,
				const std::basic_string<CharT>& to)
			{
				bool res = false;
				if (from.empty())
					return res;
				size_t start_pos = 0;
				while ((start_pos = str.find(from, start_pos)) != std::basic_string<CharT>::npos)
				{
					res = true;
					str.replace(start_pos, from.length(), to);
					start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
				}
				return res;
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

			/// @brief Utf8��ʽ�ַ���ת���ַ���
			///			����codecvt_utf8��C++17�б����ã���ʱû�ҵ������ʽ
			/// @param[in] str Utf8�ַ���
			/// @return ���ַ���
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

			// trim from start (in place)
			template <typename CharT>
			inline void ltrim(std::basic_string<CharT>& s) {
				s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
					return !std::isspace(ch);
					}));
			}

			// trim from end (in place)
			template <typename CharT>
			inline void rtrim(std::basic_string<CharT>& s) {
				s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
					return !std::isspace(ch);
					}).base(), s.end());
			}

			// trim from both ends (in place)
			template <typename CharT>
			inline void trim(std::basic_string<CharT>& s) {
				ltrim(s);
				rtrim(s);
			}

			// trim from start (copying)
			template <typename CharT>
			inline std::basic_string<CharT> ltrim_copy(std::basic_string<CharT> s) {
				ltrim(s);
				return s;
			}

			// trim from end (copying)
			template <typename CharT>
			inline std::basic_string<CharT> rtrim_copy(std::basic_string<CharT> s) {
				rtrim(s);
				return s;
			}

			// trim from both ends (copying)
			template <typename CharT>
			inline std::basic_string<CharT> trim_copy(std::basic_string<CharT> s) {
				trim(s);
				return s;
			}
		}
	}
}