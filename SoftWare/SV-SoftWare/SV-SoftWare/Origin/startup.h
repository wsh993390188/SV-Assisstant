#pragma once

namespace SV_ASSIST
{
	namespace Software
	{
		namespace Origin
		{
			void ShowAutoRunList(std::vector<RegKeyDetail>& ListEnumKeyValue);
			BOOLEAN EnumRegistryKeyValue(
				_In_ const HKEY hKey,
				_In_ const std::wstring& SubKey,
				_In_ const std::wstring& KeyType,
				_Out_ std::vector<RegKeyDetail>& Result
			);
		}
	}
}